#ifndef PIN_H
#define PIN_H

#include "DataType.h"
#include "enumtypes.h"

namespace phydb {

class Pin {
  private:
    string name_;
    SignalDirection direction_;
    SignalUse use_;
    string shape_;
    string antenna_diff_area_layer_;
    double antenna_diff_area_;

    vector<LayerRect> layer_rects_;

  public:
    Pin() : name_(""), direction_(INPUT), use_(SIGNAL), shape_(""),
            antenna_diff_area_layer_(""), antenna_diff_area_(0) {}
    Pin(std::string &name, SignalDirection direction, SignalUse use) :
        name_(name), direction_(direction), use_(use) {}
    Pin(string name, SignalDirection direction, SignalUse use, string shape,
        string antennaDiffAreaLayer, double antennaDiffArea, vector<LayerRect> layerRects) :
        name_(name),
        direction_(direction),
        use_(use),
        shape_(shape),
        antenna_diff_area_layer_(antennaDiffAreaLayer),
        antenna_diff_area_(antennaDiffArea),
        layer_rects_(layerRects) {}

    void SetName(string &name);
    void SetUse(SignalUse &use);
    void SetLayerRect(vector<LayerRect>);

    LayerRect *AddLayerRect(std::string &layer_name);

    const string &GetName();
    SignalDirection GetDirection();
    SignalUse GetUse();
    vector<LayerRect> &GetLayerRectRef();
    vector<LayerRect> GetLayerRectCpy();

    friend ostream &operator<<(ostream &, const Pin &);
};

ostream &operator<<(ostream &, const Pin &);

}

#endif

