#include <boost/filesystem.hpp>
#include <iostream>
#include "liberty.hpp"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " file.lib" << std::endl;
    return -1;
  }

  // Read liberty file
  liberty::liberty lib{argv[1]};

  // Access element
  auto aio22_pin_zn = lib["library:typical"]["cell:AOI22_X1"]["pin:ZN"];
  std::cout << "Matched nodes: " << aio22_pin_zn.size() << std::endl;

  // Just for testing:
  // Print to new liberty file
  lib.write("out.lib");

  return 0;
}