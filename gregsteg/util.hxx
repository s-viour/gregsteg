#ifndef __GREGSTEG_UTIL_HXX__
#define __GREGSTEG_UTIL_HXX__

#include <vector>
#include "image.hxx"


class image_histogram {
public:
  std::vector<int> histogram;

  explicit image_histogram(const ppm& image);
  const int valof(size_t hidx) const { return histogram[hidx]; };
};

class rnumgen {
private:
  std::vector<float> f;

public:
  rnumgen(int, image_histogram&);
  int rand();
};


#endif
