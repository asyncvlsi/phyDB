#ifndef MACRO_H
#define MACRO_h

#include "DataType.h"
#include "pin.h"
#include "obs.h"

namespace phydb {

class Macro {
    private:
        string _name;

        Point2D<float> _origin;
        Point2D<float> _size;

        vector<Pin> _pins;
        OBS _obs;

    public:
        Macro( ) : _name("") { }
        Macro(string name, Point2D<float> origin, Point2D<float> size, vector<Pin> pins, OBS obs) :
            _name(name),
            _origin(origin),
            _size(size),
            _pins(pins),
            _obs(obs)
            { }

        void setName(string );
        void setOrigin(Point2D<float> _origin);
        void setOrigin(float x, float y);
        void setSize(Point2D<float> _size);
        void setSize(float width, float height);
        void setPins(vector<Pin> );
        void addPin(Pin );
        void setOBS(OBS );

        string getName( ) const;
        Point2D<float> getOrigin( ) const;
        Point2D<float>& getOriginRef( );
        float getOriginX( ) const;
        float getOriginY( ) const;
        Point2D<float> getSize( ) const;
        float getWidth( ) const;
        float getHeight( ) const;
        vector<Pin> getPinVec( ) const;
        vector<Pin>& getPinVecRef( ) const;
        bool getPin(string pinName, Pin& pin) const;
        bool getOBS(OBS& ) const;

        friend ostream& operator<< (ostream& , const Macro& );
};

ostream& operator<< (ostream& , const Macro& );

}


#endif



