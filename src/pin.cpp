#include "pin.h"

namespace phydb {

ostream& operator<< (ostream& os, const Pin& p) {
    os << p._name << " " << p._direction << " " << p._use << " " << p._shape << endl;
    os << p._antennaDiffAreaLayer << " " << p._antennaDiffArea << endl;
    
    for(auto l : p._layerRects) 
        os << l << endl;
    return os;
}

}




