#ifndef VIARULEGENERATE_H
#define VIARULEGENERATE_H

namespace phydb {

class ViaRuleGenerateLayer {
  public:
    string layer_name_;
    Rect2D<float> rect_;
    Size2D<float> spacing_;
    Size2D<float> enclosure_overhang_;

    ViaRuleGenerateLayer() {}
    ViaRuleGenerateLayer(string &layerName) : layer_name_(layerName) {}

    void SetRect(float lx, float ly, float ux, float uy);
    void SetSpacing(float x, float y);
    void SetEnclosure(float x, float y);
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
