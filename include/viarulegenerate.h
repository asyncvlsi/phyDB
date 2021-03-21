#ifndef VIARULEGENERATE_H
#define VIARULEGENERATE_H

namespace phydb {

class ViaRuleGenerateLayer {
    public:
        string layerName_; 
        Rect2D<float> rect_;
        Size2D<float> spacing_;
        Size2D<float> enclosureOverhang_;
    
        ViaRuleGenerateLayer() { }
        ViaRuleGenerateLayer(string& layerName) : layerName(layerName_) { }

        void SetRect(float lx, float ly, float ux, float uy);
        void SetSpacing(float x, float y);
        void SetEnclosure(float x, float y);
};

class ViaRuleGenerate
{
    public: 
        string name_;
        bool isDefault_;
        ViaRuleGenerateLayer layers_[3];
    
        ViaRuleGenerate(): isDefault_(false) { }
        ViaRuleGenerate(string& name): name_(name), isDefault_(false) { }
     
        void SetDefault();
        void UnsetDefault();
        void SetLayers(ViaRuleGenerateLayer&, ViaRuleGenerateLayer&, ViaRuleGenerateLayer&); 
};

}

#endif
