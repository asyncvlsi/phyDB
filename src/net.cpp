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

void Net::Report() {
    int sz  = component_names_.size();
    std::cout << "NET: " << name_ << "  weight: " << weight_ << " size: " << sz << "\n";
    for (auto &iopin_name: iopin_names_) {
        std::cout << "  (PIN " << iopin_name << ") ";
    }

    for (int i=0; i<sz; ++i) {
        std::cout << "  (" << component_names_[i] << " " << pin_names_[i] << ") ";
    }
    std::cout << "\n";
}

}