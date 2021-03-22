#include <iostream>

#include "adjacentcutspacing.h"

namespace phydb {
void AdjacentCutSpacing::Report() {
    std::cout << "spacing: " << spacing_;
    std::cout << " adj cuts: " << adjacent_cuts_;
    std::cout << " cut within: " << cut_within_ << std::endl;
}


}



