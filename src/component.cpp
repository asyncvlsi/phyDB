#include "component.h"

namespace phydb {

ostream& operator<< (ostream& os, const Component& c) {
    os << c._name << " " << c._macroName << " " << c._source << " " << c._locationType << endl;
    os << c._orient << endl;
    os << "weight: " << c._weight << " location: " << c._location << endl;
    return os;
}

}

