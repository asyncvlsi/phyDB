#include "row.h"

namespace phydb {

ostream& operator<< (ostream& os, const Row& r) {
    os << r.name_ << " " << r.site_name_ << " " << r.site_orient_ << endl;
    os << "orig: " << r.orig_x_ << ", " << r.orig_y_ << endl;
    os << "num: " << r.num_x_ << ", " << r.num_y_ << endl;
    os << "step: " << r.step_x_ << ", " << r.step_y_ << endl;
    return os;
}

}
