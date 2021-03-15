#ifndef OBS_H
#define OBS_H

#include "DataType.h"

namespace phydb {

class OBS {
    private:
       vector<LayerRect> _layerRects;

    public:
        OBS( ) { }
        OBS(vector<LayerRect> layerRects): _layerRects(layerRects) { }

        void setLayerRect(vector<LayerRect> );

        vector<LayerRect> getLayerRect( ) const;

        friend ostream& operator<< (ostream&, const OBS& );
};


ostream& operator<< (ostream&, const OBS& );


}


#endif

