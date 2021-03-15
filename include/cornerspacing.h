#ifndef CORNERSPACING_H
#define CORNERSPACING_H

#include "header.h"

namespace phydb {
    
class CornerSpacing {
    private: 
        float _eolWidth;
        vector<float> _width;
        vector<float> _spacing;

    public:
        CornerSpacing( ) : _eolWidth(0) {}
        
        //constructor for metal layer
        CornerSpacing(float eolWidth, vector<float> width, vector<float> spacing) : 
            _eolWidth(eolWidth), _width(width), _spacing(spacing) { }

        void reset();
};

ostream& operator<< (ostream& , const CornerSpacing& );

}

#endif
