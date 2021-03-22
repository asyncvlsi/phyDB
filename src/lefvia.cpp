#include "lefvia.h"

namespace phydb {

void LefVia::SetDefault() {
    is_default_ = true;
}

void LefVia::UnsetDefault() {
    is_default_ = false;
}
 
void LefVia::SetLayerRect(const string& bot_layer_name, const vector<Rect2D<float>>& bot_rects,
        const string& cut_layer_name, const vector<Rect2D<float>>& cut_rects,
        const string& top_layer_name, const vector<Rect2D<float>>& top_rects) {
    layer_rects_[0] = LayerRect(bot_layer_name, bot_rects);
    layer_rects_[1] = LayerRect(cut_layer_name, cut_rects);
    layer_rects_[2] = LayerRect(top_layer_name, top_rects);
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
