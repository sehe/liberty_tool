#include <boost/filesystem.hpp>
#include <iostream>
#include "liberty.hpp"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " libertyfile" << std::endl;
    return -1;
  }

  boost::filesystem::path file{argv[1]};
  if (!boost::filesystem::is_regular_file(file)) {
    std::cout << "Liberty file " << boost::filesystem::absolute(file)
              << " does not exists." << std::endl;
    return -1;
  }

  // Read liberty file
  liberty::liberty lib{file};

  // Just for testing...
  std::cout << "Cells in root node:" << std::endl;
  for (auto &elem : lib.root().elements) {
    if (auto *c = boost::get<liberty::ast::container>(&elem)) {
      if (c->name == "cell") {
        std::cout << c->args.front() << std::endl;
      }
    }
  }

  // Print to new liberty file
  lib.write("out.lib");

  return 0;
}