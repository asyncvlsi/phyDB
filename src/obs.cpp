#include "obs.h"

namespace phydb {

LayerRect *OBS::AddLayerRect(std::string &layer_name) {
    layer_rects_.emplace_back(layer_name);
    return &(layer_rects_.back());
}

ostream &operator<<(ostream &os, const OBS &obs) {
    if (!obs.layer_rects_.empty()) {
        os << "OBS\n";
    }
    for (auto &l : obs.layer_rects_)
        os << l << endl;
    return os;
}

}


