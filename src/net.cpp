#include "net.h"

namespace phydb {

string Net::getName() {
    return _name;
}

void Net::AddIoPin(std::string const &iopin_name) {
    iopin_names_.push_back(iopin_name);
}

void Net::AddCompPin(std::string const &comp_name, std::string const &pin_name) {
    componentNames.push_back(comp_name);
    pinNames.push_back(pin_name);
}

}