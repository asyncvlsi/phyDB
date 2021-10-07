#ifndef PHYDB_INCLUDE_VIARULEGENERATE_H
#define PHYDB_INCLUDE_VIARULEGENERATE_H

#include "datatype.h"

namespace phydb {

class ViaRuleGenerateLayer {
  public:
    std::string layer_name_;
    Rect2D<double> rect_;
    Size2D<double> spacing_;
    Size2D<double> enclosure_;

    ViaRuleGenerateLayer() {}
    ViaRuleGenerateLayer(std::string &layerName) : layer_name_(layerName) {}

    void SetLayerName(const std::string &layer_name);
    void SetRect(double lx, double ly, double ux, double uy);
    void SetSpacing(double x, double y);
    void SetEnclosure(double x, double y);

    std::string GetLayerName() const;
    Rect2D<double> GetRect() const;
    Size2D<double> GetSpacing() const;
    Size2D<double> GetEnclosure() const;
};

class ViaRuleGenerate {
  public:
    std::string name_;
    bool is_default_;
    ViaRuleGenerateLayer layers_[3];

    ViaRuleGenerate() : is_default_(false) {}
    ViaRuleGenerate(std::string &name) : name_(name), is_default_(false) {}

    void SetDefault();
    void UnsetDefault();
    void SetLayers(
        ViaRuleGenerateLayer &,
        ViaRuleGenerateLayer &,
        ViaRuleGenerateLayer &
    );
};

}

#endif //PHYDB_INCLUDE_VIARULEGENERATE_H
