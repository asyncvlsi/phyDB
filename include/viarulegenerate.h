#ifndef VIARULEGENERATE_H
#define VIARULEGENERATE_H

#include "datatype.h"

namespace phydb {

class ViaRuleGenerateLayer {
  public:
    string layer_name_;
    Rect2D<double> rect_;
    Size2D<double> spacing_;
    Size2D<double> enclosure_;

    ViaRuleGenerateLayer() {}
    ViaRuleGenerateLayer(string &layerName) : layer_name_(layerName) {}
    
    void SetLayerName(const string& layer_name);
    void SetRect(double lx, double ly, double ux, double uy);
    void SetSpacing(double x, double y);
    void SetEnclosure(double x, double y);

    string GetLayerName() const;
    Rect2D<double> GetRect() const;
    Size2D<double> GetSpacing() const;
    Size2D<double> GetEnclosure() const;
};

class ViaRuleGenerate {
  public:
    string name_;
    bool is_default_;
    ViaRuleGenerateLayer layers_[3];

    ViaRuleGenerate() : is_default_(false) {}
    ViaRuleGenerate(string &name) : name_(name), is_default_(false) {}

    void SetDefault();
    void UnsetDefault();
    void SetLayers(ViaRuleGenerateLayer &, ViaRuleGenerateLayer &, ViaRuleGenerateLayer &);
};

}

#endif
