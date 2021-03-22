#include <iostream>

#include "spacingtableinfluence.h"

namespace phydb {

void SpacingTableInfluence::SetWidth(float width) {
    width_ = width;
}

void SpacingTableInfluence::SetWithin(float within) {
    within_ = within;
}

void SpacingTableInfluence::SetSpacing(float spacing) {
    spacing_ = spacing;
}


float SpacingTableInfluence::GetWidth() const {
    return width_;
}

float SpacingTableInfluence::GetWithin() const {
    return within_;
}

float SpacingTableInfluence::GetSpacing() const {
    return spacing_;
}

float SpacingTableInfluence::Report() const {
    std::cout << "width: " << width_;
    std::cout << "within: " << within_;
    std::cout << "spacing: " << spacing_;
    std::cout << endl;
    
}


}




