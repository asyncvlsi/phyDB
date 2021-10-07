#include "pin.h"

namespace phydb {

void Pin::SetName(std::string &name) {
    name_ = name;
}

void Pin::SetUse(SignalUse &use) {
    use_ = use;
}

LayerRect *Pin::AddLayerRect(std::string &layer_name) {
    layer_rects_.emplace_back(layer_name);
    return &(layer_rects_.back());
}

const std::string &Pin::GetName() {
    return name_;
}

SignalDirection Pin::GetDirection() {
    return direction_;
}

bool Pin::IsDriverPin() const {
    return direction_ == OUTPUT || direction_ == OUTPUT_TRISTATE;
}

SignalUse Pin::GetUse() {
    return use_;
}

std::vector<LayerRect> &Pin::GetLayerRectRef() {
    return layer_rects_;
}

std::vector<LayerRect> Pin::GetLayerRectCpy() {
    return layer_rects_;
}

std::ostream &operator<<(std::ostream &os, const Pin &p) {
    os << p.name_ << " "
       << SignalDirectionStr(p.direction_) << " "
       << SignalUseStr(p.use_) << " "
       << p.shape_ << std::endl;
    os << p.antenna_diff_area_layer_ << " " << p.antenna_diff_area_ << "\n";

    for (auto &l: p.layer_rects_)
        os << l << std::endl;
    return os;
}

}




