#include <iostream>
#include <fmt/core.h>
#include "image.hxx"
#include "steg.hxx"


int main(int argc, char* argv[]) {
  if (argc < 3) {
    fmt::print("not enough arguments supplied\n");
    return 0;
  }

  auto mode = std::string(argv[1]);
  auto inpath = std::string(argv[2]);
  auto key = std::string(argv[argc - 1]);
  ppm image;

  try {
    image.read_into(inpath);
  } catch (std::runtime_error& e) {
    fmt::print(stderr, "error in reading image {}: {}\n", inpath, e.what());
  }

  // initialize the state for the encryption/decryption functions
  steg_state state(image, 0, key);

  if (mode == "encrypt") {
  encrypt(image, state);
  auto outpath = std::string(argv[3]);

  try {
    image.write_to(outpath);
  } catch (std::runtime_error& e) {
    fmt::print(stderr, "error in writing output: {}\n", e.what());
  }
  } else if (mode == "decrypt") {
    auto output = decrypt(image, state);
    fmt::print("{}", output);
  } else {
    fmt::print("unknown mode: {}\n", mode);
  }
}