#include "iopin.h"

namespace phydb {

string IOPin::GetName() {
    return name_;
}

void IOPin::SetNetName(std::string const &net_name) {
    net_name_ = net_name;
}

}