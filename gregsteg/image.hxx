#ifndef __GREGSTEG_IMAGE_HXX__
#define __GREGSTEG_IMAGE_HXX__

#include <string>
#include <fstream>
#include <vector>


struct pixel {
  unsigned char r, g, b;
};

class ppm {
private:
  std::string fname;
  std::ifstream file;
  std::vector<pixel> buffer;

  // ppm header data
  std::string p6;
  unsigned int rows;
  unsigned int cols;
  unsigned int max_value;

public:
  ppm() {}
  void read_into(const std::string& path);
  void write_to(const std::string& path);
  pixel& at(size_t i, size_t j) { return buffer[cols * i + j]; }

  const std::string& file_name() const { return fname; }
  const std::vector<pixel>& internal_buffer() const { return buffer; }
  const unsigned int ncols() const { return cols; }
  const unsigned int nrows() const { return rows; }
};


#endif