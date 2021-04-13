#ifndef PHYDB_HEADER_H
#define PHYDB_HEADER_H

#include <cassert>
#include <cstring>

#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace phydb {

inline void PhyDbExpects(bool e, const std::string &error_message) {
    if (!e) {
        std::cout << "\033[0;31m" << "FATAL ERROR:" << "\n"
                                 << "    " << error_message << "\033[0m" << std::endl;
        exit(1);
    }
}

}

#endif


