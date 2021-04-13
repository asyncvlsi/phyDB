#include "pin.h"

namespace phydb {

void Pin::SetName(string &name) { 
  name_ = name; 
}

void Pin::SetUse(SignalUse &use) { 
  use_ = use; 
}

LayerRect *Pin::AddLayerRect(std::string &layer_name) {
    layer_rects_.emplace_back(layer_name);
    return &(layer_rects_.back());
}

const string &Pin::GetName() {
    return name_;
}

SignalDirection Pin::GetDirection() {
    return direction_;
}

SignalUse Pin::GetUse() {
    return use_;
}

vector<LayerRect> &Pin::GetLayerRectRef() {
    return layer_rects_;
}

vector<LayerRect> Pin::GetLayerRectCpy() {
    return layer_rects_;
}

ostream &operator<<(ostream &os, const Pin &p) {
    os << p.name_ << " "
       << SignalDirectionStr(p.direction_) << " "
       << SignalUseStr(p.use_) << " "
       << p.shape_ << endl;
    os << p.antenna_diff_area_layer_ << " " << p.antenna_diff_area_ << endl;

    for (auto &l : p.layer_rects_)
        os << l << endl;
    return os;
}

}




