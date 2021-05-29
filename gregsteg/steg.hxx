#ifndef __GREGSTEG_STEG_HXX__
#define __GREGSTEG_STEG_HXX__

#include <vector>
#include "image.hxx"
#include "util.hxx"


const auto ETX = 0x30;


struct steg_state {
  int seed;
  std::string key;

  std::vector<std::pair<int, int>> pixel_list;
  image_histogram image_hist;
  std::vector<int> key_hist;

  steg_state(ppm&, int, const std::string&);
};

void encrypt(ppm&, steg_state&);
std::string decrypt(ppm&, steg_state&);
#endif