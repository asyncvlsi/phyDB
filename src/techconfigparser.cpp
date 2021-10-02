#include "techconfigparser.h"

#include <fstream>

namespace phydb {

TechConfigParser::TechConfigParser(Tech *tech) :
    scanner_(*this),
    parser_(scanner_, *this),
    tech_(tech) {
    PhyDBExpects(tech_ != nullptr,
                 "Cannot store technology configuration file in a nullptr");
}

int TechConfigParser::Parse() {
    int res = parser_.parse();
    tech_->FixResOverTable();
    return res;
}

void TechConfigParser::SetInputStream(std::istream *is) {
    scanner_.switch_streams(is, nullptr);
}

Tech *TechConfigParser::UserData() {
    return tech_;
}

}