#ifndef LEFVIA_H
#define LEFVIA_H

namespace phydb {

class LefVia {
    public:
        string name_;
        bool isDefault;
        std::vector<LayerRect> layerRects;
        
        LefVia( ) { }
       
};

}

#endif
