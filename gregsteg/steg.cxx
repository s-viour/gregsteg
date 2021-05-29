#include <algorithm>
#include <iostream>
#include "steg.hxx"
#include <fmt/core.h>


steg_state::steg_state(ppm& img, int sd, const std::string& k) :
  seed(sd), key(k), image_hist(image_histogram(img)) {

  pixel_list.reserve(img.nrows() * img.ncols());
  for (int c = 0; c < img.ncols(); ++c) {
    for (int r = 0; r < img.nrows(); ++r) {
      pixel_list.push_back(std::make_pair(r, c));
    }
  }

  rnumgen rgen(seed, image_hist);
  for (size_t i = pixel_list.size() - 1; i > 0; --i) {
    auto r1_12 = rgen.rand();
    auto r2_12 = rgen.rand();

    auto r_24 = ((r1_12 << 12) | r2_12) % (i + 1);

    std::swap(pixel_list[i], pixel_list[r_24]);
  }
}

std::vector<unsigned char> iterbits(unsigned char c) {
  std::vector<unsigned char> v;
  for (auto i = 0; i < 8; ++i) {
    v.push_back((c >> i) & 1);
  }
  std::reverse(v.begin(), v.end());
  return v;
}

void encrypt(ppm& img, steg_state& s) {
  auto px_iterator = s.pixel_list.begin();
  auto comp = 0;
  auto k = 0;

  bool wetx = false;
  unsigned char c;
  while (std::cin >> c || !wetx) {
    if (std::cin.eof()) {
      c = ETX;
      wetx = true;
    }
    c ^= s.key[k % s.key.length()];
    k += 1;

    for (auto bit : iterbits(c)) {
      // grab pixel index and increment iterator
      auto& img_px = img.at(px_iterator->first, px_iterator->second);
      ++px_iterator;

      if (comp == 0) {
        img_px.r &= 0xfe;
        img_px.r |= bit;
        comp = 1;
      } else if (comp == 1) {
        img_px.g &= 0xfe;
        img_px.g |= bit;
        comp = 2;
      } else {
        img_px.b &= 0xfe;
        img_px.b |= bit;
        comp = 0;
      }
    }
  }
}

std::string decrypt(ppm& img, steg_state& s) {
  auto px_iterator = s.pixel_list.begin();
  auto comp = 0;
  int k = 0;
  unsigned char extracted = 0;
  std::string msg;

  while (true) {
    for (auto i = 7; i >= 0; --i) {
      auto& px = img.at(px_iterator->first, px_iterator->second);
      px_iterator++;

      if (comp == 0) {
        extracted |= (px.r & 1) << i;
        //fmt::print("extracted {} from red\n", (px.r & 1) << i);
        comp = 1;
      } else if (comp == 1) {
        extracted |= (px.g & 1) << i;
        //fmt::print("extracted {} from green\n", (px.g & 1) << i);
        comp = 2;
      } else {
        extracted |= (px.b & 1) << i;
        //fmt::print("extracted {} from blue\n", (px.b & 1) << i);
        comp = 0;
      }
    }

    extracted ^= s.key[k % s.key.length()];
    if (extracted == ETX) {
      break;
    }
    fmt::print("{}", (char)extracted);
    k += 1;
    extracted = 0;
  }

  return msg;
}