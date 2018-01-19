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

  // Access some elements
  auto zn_pins = lib["library:typical"]["cell:.*_X1"]["pin:ZN"];
  std::cout << "ZN pins matched: " << zn_pins.size() << std::endl;
  std::cout << zn_pins["direction|function"] << std::endl;

  auto nand2_x1 = lib["library:typical"]["cell:NAND2_X1"];
  auto timingA1 = nand2_x1["pin:ZN"]["timing"]["related_pin:A1"].up();
  std::cout << timingA1 << std::endl;

  // cell_rise is the rise delay through the cell based on the input slew and
  // output load. rise_transition is the slew at the output pin based on the
  // input slew and output load.
  auto cell_rise = timingA1["cell_rise"];
  auto rise_transition = timingA1["rise_transition"];
  auto cell_fall = timingA1["cell_fall"];
  auto fall_transition = timingA1["fall_transition"];
  std::cout << cell_rise << rise_transition << cell_fall << fall_transition
            << std::endl;

  // Print to new liberty file
  lib.write("out.lib");

  return 0;
}