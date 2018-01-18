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

  // Access element for testing
  auto zn_pins = lib["library:typical"]["cell:.*_X1"]["pin:ZN"];
  std::cout << "ZN pins matched: " << zn_pins.size() << std::endl;
  std::cout << zn_pins["direction|function"] << std::endl;

  auto nand2_x1 = lib["library:typical"]["cell:NAND2_X1"];
  auto timingA1 = nand2_x1["pin:ZN"]["timing"]["related_pin:A1"];
  std::cout << timingA1 << std::endl;

  // Just for testing:
  // Print to new liberty file
  lib.write("out.lib");

  return 0;
}