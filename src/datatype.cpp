#include "datatype.h"

namespace phydb {

std::ostream &operator<<(std::ostream &os, const LayerRect &lr) {
    os << lr.layer_name_ << std::endl;
    for (auto rect: lr.rects_)
        os << rect << std::endl;
    return os;
}

}

