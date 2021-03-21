#include "net.h"

namespace phydb {

string Net::GetName() {
    return name_;
}

void Net::AddIoPin(std::string const &iopin_name) {
    iopin_names_.push_back(iopin_name);
}

void Net::AddCompPin(std::string const &comp_name, std::string const &pin_name) {
    component_names_.push_back(comp_name);
    pin_names_.push_back(pin_name);
}

}