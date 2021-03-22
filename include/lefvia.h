#ifndef LEFVIA_H
#define LEFVIA_H

#include <string>

#include "DataType.h"

namespace phydb {

class LefVia {
  private:
    string name_;
    bool is_default_;
    LayerRect layer_rects_[3];// 0 = bot layer, 1 = cut layer, 2 = top layer
  public:
    LefVia(): is_default_(false) {}
    explicit LefVia(std::string &name) : name_(name) {}

    void SetDefault();
    void UnsetDefault();
    void SetLayerRect(const string& bot_layer_name, const vector<Rect2D<float>>& bot_rect,
            const string& cut_layer_name, const vector<Rect2D<float>>& cut_rect,
            const string& top_layer_name, const vector<Rect2D<float>>& top_rect);
    
    LayerRect* GetLayerRects();
    
    void Report();
};

}

#endif
