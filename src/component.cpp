#include "component.h"

namespace phydb {

string Component::GetName() {
    return name_;
}

string Component::GetMacroName() {
    return macro_name_;
}

ostream &operator<<(ostream &os, const Component &c) {
    os << c.name_ << " " << c.macro_name_ << " " << c.source_ << " " << c.location_type_ << endl;
    os << c.orient_ << endl;
    os << "weight: " << c.weight_ << " location: " << c.location_ << endl;
    return os;
}

}

