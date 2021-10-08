#ifndef PHYDB_INCLUDE_LOGGING_H_
#define PHYDB_INCLUDE_LOGGING_H_

#include <cassert>
#include <cstring>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#define PhyDBExpects(e, error_message) PhyDBExpects_(e, error_message, __FILE__, __LINE__, __FUNCTION__)

namespace phydb {

inline void PhyDBExpects_(
    bool e,
    const std::string &error_message,
    const char *file,
    size_t line,
    const char *function
) {
    if (!e) {
        std::cout << "\033[0;31m" << "FATAL ERROR:\n    "
                  << error_message << "\n";
        std::cout << file << " : " << line << " : " << function << "\033[0m" << std::endl;
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
