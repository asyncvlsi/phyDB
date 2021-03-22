#include "adjacentcutspacing.h"

namespace phydb {
void AdjacentCutSpacing::Report() {
    cout << "spacing: " << spacing_;
    cout << " adj cuts: " << adjacent_cut_;
    cout << " cut within: " << cut_within_ << endl;
}


}



