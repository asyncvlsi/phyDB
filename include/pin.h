#ifndef PIN_H
#define PIN_H

#include "DataType.h"

namespace phydb {

class Pin {
    private:
        string _name;
        string _direction;
        string _use;
        string _shape;
        string _antennaDiffAreaLayer;
        float _antennaDiffArea;
        
        vector<string> _netExpr;
        vector<LayerRect> _layerRects;

    public:
        Pin( ): _name(""), _direction(""), _use(""), _shape(""),
            _antennaDiffAreaLayer(""), _antennaDiffArea(0) { }
        Pin(string name, string direction, string use, string shape, 
            string antennaDiffAreaLayer, float antennaDiffArea,
            vector<string> netExpr, vector<LayerRect> layerRects):
            _name(name),
            _direction(direction),
            _use(use),
            _shape(shape),
            _antennaDiffAreaLayer(antennaDiffAreaLayer),
            _antennaDiffArea(antennaDiffArea),
            _netExpr(netExpr),
            _layerRects(layerRects)
            { }

        void setNetExpr(vector<string> );
        void setLayerRect(vector<LayerRect> );

        vector<string> getNetExpr( ) const;
        vector<LayerRect> getLayerRect( ) const;


        friend ostream& operator<< (ostream&, const Pin& );
};


ostream& operator<< (ostream&, const Pin& );


}


#endif

