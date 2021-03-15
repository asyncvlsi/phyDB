#include "obs.h"

namespace phydb { 

ostream& operator<< (ostream& os, const OBS& obs) {
    for(auto l : obs._layerRects)
        os << l << endl;
    return os;
}


}


