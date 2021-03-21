#ifndef VIARULEGENERATE_H
#define VIARULEGENERATE_H

namespace phydb {

class ViaRuleGenerateLayer {
    public:
        string layerName_; 
        Rect2D<float> rect_;
        Size2D<float> spacing_;
        Size2D<float> enclosureOverhang_;
    
        ViaRuleGenerateLayer( ) { }
};

class ViaRuleGenerate
{
    public: 
        string name_;
        bool isDefault_;
        ViaRuleGenerateLayer layers_[3];
    
        ViaRuleGenerate() { 
            isDefault = true;
        }
     
};

}

#endif
