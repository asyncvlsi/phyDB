#ifndef PIN_H
#define PIN_H

#include "DataType.h"
#include "enumtypes.h"

namespace phydb {

class Pin {
  private:
    string name_;
    SignalDirection direction_;
    string use_;
    string shape_;
    string antenna_diff_area_layer_;
    float antenna_diff_area_;

    vector<LayerRect> layer_rects_;

  public:
    Pin() : name_(""), direction_(INPUT), use_(""), shape_(""),
            antenna_diff_area_layer_(""), antenna_diff_area_(0) {}
    Pin(std::string &name, SignalDirection direction) : name_(name), direction_(direction) {}
    Pin(string name, SignalDirection direction, string use, string shape,
        string antennaDiffAreaLayer, float antennaDiffArea,
        vector<string> netExpr, vector<LayerRect> layerRects) :
        name_(name),
        direction_(direction),
        use_(use),
        shape_(shape),
        antenna_diff_area_layer_(antennaDiffAreaLayer),
        antenna_diff_area_(antennaDiffArea),
        layer_rects_(layerRects) {}

    void SetName(string &name) { name_ = name; }
    void SetUse(string &use) { use_ = use; }
    void SetNetExpr(vector<string>);
    void SetLayerRect(vector<LayerRect>);

    void AddLayerRect(LayerRect &layer_rect) { layer_rects_.push_back(layer_rect); }

    vector<string> GetNetExpr() const;
    vector<LayerRect> GetLayerRect() const;

    friend ostream &operator<<(ostream &, const Pin &);
};

ostream &operator<<(ostream &, const Pin &);

}

#endif

