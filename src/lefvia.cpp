#include "lefvia.h"

namespace phydb {

void LefVia::SetDefault() {
    is_default_ = true;
}

void LefVia::UnsetDefault() {
    is_default_ = false;
}
 
void LefVia::SetLayerRect(const string& layer_name0, const vector<Rect2D<float>>& rects0,
        const string& layer_name1, const vector<Rect2D<float>>& rects1,
        const string& layer_name2, const vector<Rect2D<float>>& rects2) {
    layer_rects_[0] = LayerRect(layer_name0, rects0);
    layer_rects_[1] = LayerRect(layer_name1, rects1);
    layer_rects_[2] = LayerRect(layer_name2, rects2);
}


LayerRect* LefVia::GetLayerRects() {
    return layer_rects_;
}

void LefVia::Report() {
    std::cout << "LefVia name: " << name_ << "\n";
    for (auto &layer_rect : layer_rects_) {
        layer_rect.Report();
    }
}

}
