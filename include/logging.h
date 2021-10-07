#ifndef PHYDB_INCLUDE_LOGGING_H_
#define PHYDB_INCLUDE_LOGGING_H_

#include <cassert>
#include <cstring>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#define PHYDB_LOC std::string(__FILE__) + " : " + std::to_string(__LINE__) + " : " + std::string(__FUNCTION__)
#define PhyDBExpects(e, error_message) PhyDBExpects_(e, error_message, PHYDB_LOC)

namespace phydb {

inline void PhyDBExpects_(
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
