#ifndef PHYDB_INCLUDE_LOGGING_H_
#define PHYDB_INCLUDE_LOGGING_H_

#include <cassert>
#include <cstring>

#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace phydb {

inline void PhyDBExpects(bool e, const std::string &error_message) {
    if (!e) {
        std::cout << "\033[0;31m" << "FATAL ERROR:\n"
                  << "    " << error_message << "\033[0m" << std::endl;
        exit(1);
    }
}

inline void PhyDBWarns(bool e, const std::string &message) {
    if (e) {
        std::cout << "\033[0;31m" << "WARNING:\n"
                  << "    " << message << "\033[0m" << std::endl;
        std::cout
            << "Please delete the current phydb, create a new phydb and reload correct files!"
            << std::endl;
    }
}

}

#endif //PHYDB_INCLUDE_LOGGING_H_
