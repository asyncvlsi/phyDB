#ifndef LEFVIA_H
#define LEFVIA_H

#include <string>

#include "DataType.h"

namespace phydb {

class LefVia {
  private:
    string name_;
    bool is_default_;
    LayerRect layer_rects_[3];
  public:
    LefVia(): is_default_(false) {}
    explicit LefVia(std::string &name) : name_(name) {}

    void SetDefault();
    void UnsetDefault();
    void SetLayerRect(const string& layer_name0, const vector<Rect2D<double>>& rect0,
            const string& layer_name1, const vector<Rect2D<double>>& rect1,
            const string& layer_name2, const vector<Rect2D<double>>& rect2);
    
    LayerRect* GetLayerRects();
    
    void Report();
};

}

#endif
