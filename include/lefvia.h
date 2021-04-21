#ifndef LEFVIA_H
#define LEFVIA_H

#include <string>

#include "datatype.h"

namespace phydb {

class LefVia {
  private:
    string name_;
    bool is_default_;
    vector<LayerRect> layer_rects_;
  public:
    LefVia(): is_default_(false) {}
    explicit LefVia(std::string &name) : name_(name) {}

    string GetName() const;
    void SetDefault();
    void UnsetDefault();
    void SetLayerRect(const string& layer_name0, const vector<Rect2D<double>>& rect0,
            const string& layer_name1, const vector<Rect2D<double>>& rect1,
            const string& layer_name2, const vector<Rect2D<double>>& rect2);
    
    vector<LayerRect>& GetLayerRectsRef();
    
    void Report();
};

}

#endif
