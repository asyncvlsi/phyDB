#include "iopin.h"

namespace phydb {

string IOPin::getName() {
    return _name;
}

void IOPin::SetNetName(std::string const &net_name) {
    _netName = net_name;
}

}