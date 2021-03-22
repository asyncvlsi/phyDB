#include "pin.h"

namespace phydb {

ostream &operator<<(ostream &os, const Pin &p) {
    os << p.name_ << " "
       << SignalDirectionStr(p.direction_) << " "
       << SignalUseStr(p.use_) << " "
       << p.shape_ << endl;
    os << p.antenna_diff_area_layer_ << " " << p.antenna_diff_area_ << endl;

    for (auto &l : p.layer_rects_)
        os << l << endl;
    return os;
}

}




