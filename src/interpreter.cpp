//
// Created by yihangyang on 9/21/21.
//

#include "interpreter.h"

#include <sstream>

namespace phydb {

Interpreter::Interpreter() :
    m_scanner(*this),
    m_parser(m_scanner, *this),
    m_location(0) {

}

int Interpreter::parse() {
    m_location = 0;
    return m_parser.parse();
}

void Interpreter::clear() {
    m_location = 0;
}

void Interpreter::switchInputStream(std::istream *is) {
    m_scanner.switch_streams(is, NULL);
}

void Interpreter::increaseLocation(unsigned int loc) {
    m_location += loc;
}

unsigned int Interpreter::location() const {
    return m_location;
}

}