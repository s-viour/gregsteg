#include <vector>
#include <fstream>
#include <stdexcept>
#include <fmt/core.h>
#include "image.hxx"


void ppm::read_into(const std::string& path) {
  fname = path;

  file.open(path);
  if (!file.good()) {
    throw std::runtime_error("could not open file " + path);
  }

  file >> p6 >> cols >> rows >> max_value;
  // skip past newlines
  while (file.get() != '\n') {}

  if (p6 != "P6") { 
    throw std::runtime_error("invalid header"); 
  }
  if (max_value < 0 || max_value > 255) {
    throw std::runtime_error("max value out of range 0-255");
  }

  buffer.resize(rows * cols);
  file.read((char*)buffer.data(), rows * cols * 3);
}

void ppm::write_to(const std::string& path) {
  std::ofstream file(path);
  if (!file.good()) {
    throw std::runtime_error("could not open file " + path);
  }

  file << p6 << '\n'
    << cols << ' ' << rows << '\n'
    << max_value << '\n';
  file.write((char*)buffer.data(), buffer.size() * 3);
}