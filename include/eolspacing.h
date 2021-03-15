#ifndef EOLSPACING_H
#define EOLSPACING_H

#include "header.h"

namespace phydb {
    
class EOLSpacing {
    private: 
        //metal layer
        float _spacing;
        float _eolWidth;
        float _eolWithin;
        float _parEdge;
        float _parWithin;
        
        //cut layer
        int _adjacentCuts;
        float _cutWithin;
    
    public:
        EOLSpacing( ) : _spacing(0), _eolWidth(0), _eolWithin(0),
            _parEdge(0), _parWithin(0), _adjacentCuts(0), _cutWithin(0) {}
        
        //constructor for metal layer
        EOLSpacing(float spacing, float eolWidth, float eolWithin, 
            float parEdge, float parWithin) : _spacing(spacing), _eolWidth(eolWidth),
            _eolWithin(eolWithin), _parEdge(parEdge), _parWithin(parWithin) {}
        
        //constructor for cut layer
        EOLSpacing(int adjacentCuts, float cutWithin) : _adjacentCuts(adjacentCuts),
            _cutWithin(cutWithin) {}

        void reset();
};

ostream& operator<< (ostream& , const EOLSpacing& );

}

#endif
