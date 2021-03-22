#ifndef VIARULEGENERATE_H
#define VIARULEGENERATE_H

#include "DataType.h"

namespace phydb {

class ViaRuleGenerateLayer {
  public:
    string layer_name_;
    Rect2D<float> rect_;
    Size2D<float> spacing_;
    Size2D<float> enclosure_;

    ViaRuleGenerateLayer() {}
    ViaRuleGenerateLayer(string &layerName) : layer_name_(layerName) {}
    
    void SetLayerName(const string& layer_name);
    void SetRect(float lx, float ly, float ux, float uy);
    void SetSpacing(float x, float y);
    void SetEnclosure(float x, float y);

    string GetLayerName() const;
    Rect2D<float> GetRect() const;
    Size2D<float> GetSpacing() const;
    Size2D<float> GetEnclosure() const;
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
