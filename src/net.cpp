#include "net.h"

namespace phydb {

void Net::AddIoPin(std::string const &iopin_name) {
    iopin_names_.push_back(iopin_name);
}

void Net::AddCompPin(int comp_id, int pin_id) {
    pins_.emplace_back(comp_id, pin_id);
}

void Net::AddRoutingGuide(int llx, int lly, int urx, int ury, int layerID) {
    guides_.emplace_back(llx, lly, layerID, urx, ury, layerID);
}

const string &Net::GetName() const {
    return name_;
}

vector<PhydbPin> &Net::GetPinsRef() {
    return pins_;
}

vector<string> &Net::GetIoPinNamesRef() {
    return iopin_names_;
}

vector<Rect3D<int>> &Net::GetRoutingGuidesRef() {
    return guides_;
}

void Net::Report() {
    int sz = (int) pins_.size();
    std::cout << "NET: " << name_ << "  weight: " << weight_ << " size: " << sz
              << "\n";
    for (auto &iopin_name: iopin_names_) {
        std::cout << "  (PIN " << iopin_name << ") ";
    }

    for (int i = 0; i < sz; ++i) {
        std::cout << "  ("
                  << pins_[i].comp_id << " "
                  << pins_[i].pin_id
                  << ") ";
    }
    std::cout << "\n";
}

}