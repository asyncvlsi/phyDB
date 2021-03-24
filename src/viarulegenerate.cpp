#include "viarulegenerate.h"

namespace phydb {

void ViaRuleGenerateLayer::SetLayerName(const string& layer_name) {
    layer_name_ = layer_name;
}

void ViaRuleGenerateLayer::SetRect(double lx, double ly, double ux, double uy) {
    rect_.Set(lx, ly, ux, uy);
}

void ViaRuleGenerateLayer::SetSpacing(double x, double y) {
    spacing_.Set(x, y);
}

void ViaRuleGenerateLayer::SetEnclosure(double x, double y) {
    enclosure_.Set(x, y);
}

string ViaRuleGenerateLayer::GetLayerName() const {
    return layer_name_;
}   

Rect2D<double> ViaRuleGenerateLayer::GetRect() const {
    return rect_;
}

Size2D<double> ViaRuleGenerateLayer::GetSpacing() const {
    return spacing_;
}

Size2D<double> ViaRuleGenerateLayer::GetEnclosure() const {
    return enclosure_;
}

void ViaRuleGenerate::SetDefault() {
    is_default_ = true;
}

void ViaRuleGenerate::UnsetDefault() {
    is_default_ = false;
}   

void ViaRuleGenerate::SetLayers(ViaRuleGenerateLayer& layer0,
        ViaRuleGenerateLayer& layer1, ViaRuleGenerateLayer& layer2) {
    layers_[0] = layer0;
    layers_[1] = layer1;
    layers_[2] = layer2;
}

}



