#include "liberty.hpp"
#include <boost/filesystem.hpp>
#include <iostream>

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

    // read liberty file
    liberty::liberty lib{file};

    // just for testing...
    std::cout << "Root node name: " << lib.root().name << std::endl;

    std::cout << "Cells in root node:" << std::endl;
    for (auto &elem: lib.root().elements) {
        if (auto *c = boost::get<liberty::ast::container>(&elem)) {
            if (c->name == "cell") {
                std::cout << c->arg.front() << std::endl;
            }
        }
    }

    return 0;
}