#include "obs.h"

namespace phydb { 

ostream& operator<< (ostream& os, const OBS& obs) {
    for(auto l : obs.layer_rects_)
        os << l << endl;
    return os;
}


}


