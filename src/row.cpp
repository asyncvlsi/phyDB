#include "row.h"

namespace phydb {

std::ostream &operator<<(std::ostream &os, const Row &r) {
    os << r.name_ << " " << r.site_name_ << " " << r.site_orient_ << "\n";
    os << "orig: " << r.orig_x_ << ", " << r.orig_y_ << "\n";
    os << "num: " << r.num_x_ << ", " << r.num_y_ << "\n";
    os << "step: " << r.step_x_ << ", " << r.step_y_ << "\n";
    return os;
}

}
