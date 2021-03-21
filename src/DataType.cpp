#include "DataType.h"

namespace phydb {

ostream& operator<< (ostream& os, const LayerRect& lr) {
    os << lr.layer_name_ << endl;
    for(auto rect : lr.rects)
        os << rect << endl;
    return os;
}

}

