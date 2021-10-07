#ifndef PHYDB_INCLUDE_LOGGING_H_
#define PHYDB_INCLUDE_LOGGING_H_

#include <cassert>
#include <cstring>

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace phydb {
#define PHYDB_LOC std::string(__FILE__) + " : " + std::to_string(__LINE__) + " : " + std::string(__FUNCTION__)

inline void PhyDBExpects(
    bool e,
    const std::string &error_message,
    const std::string &location = ""
) {
    if (!e) {
        std::cout << "\033[0;31m" << "FATAL ERROR:\n    "
                  << error_message << "\n";
        std::cout << location << "\033[0m" << std::endl;
        exit(1);
    }
}

inline void PhyDBWarns(bool e, const std::string &message) {
    if (e) {
        std::cout << "WARNING:\n    " << message << "\n";
    }
}

}

#endif //PHYDB_INCLUDE_LOGGING_H_
