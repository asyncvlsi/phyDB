#include <iostream>

#include "spacingtableinfluence.h"

namespace phydb {

void SpacingTableInfluence::SetWidth(double width) {
    width_ = width;
}

void SpacingTableInfluence::SetWithin(double within) {
    within_ = within;
}

void SpacingTableInfluence::SetSpacing(double spacing) {
    spacing_ = spacing;
}


double SpacingTableInfluence::GetWidth() const {
    return width_;
}

double SpacingTableInfluence::GetWithin() const {
    return within_;
}

double SpacingTableInfluence::GetSpacing() const {
    return spacing_;
}

void SpacingTableInfluence::Report() const {
    std::cout << "width: " << width_;
    std::cout << "within: " << within_;
    std::cout << "spacing: " << spacing_;
    std::cout << std::endl;
    
}


}




