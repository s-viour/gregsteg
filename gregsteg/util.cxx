#include <algorithm>
#include <functional>
#include <numeric>
#include "util.hxx"

using namespace std;


rnumgen::rnumgen(int seedvalue, image_histogram& hist) {
  srand(seedvalue);
  auto& v = hist.histogram;

  f.resize(v.size());
  partial_sum(v.begin(), v.end(), f.begin());
  transform(f.begin(), f.end(), f.begin(),
    bind2nd(divides<float>(), f.back()));
}

int rnumgen::rand() {
  const double randmax = RAND_MAX + 1.0;
  const double p = (double)std::rand() / randmax;

  return upper_bound(f.begin(), f.end(), p) - f.begin();
}

image_histogram::image_histogram(const ppm& image) {
  histogram = std::vector<int>(4096, 0);

  for (auto& px : image.internal_buffer()) {
  unsigned char r, g, b;
  r = px.r << 5;
  g = px.g << 1;
  b = px.b >> 3;

  size_t hidx = (r & 0xf00) | (g & 0xf0) | (b & 0xf);
  histogram[hidx] += 1;
  }
}