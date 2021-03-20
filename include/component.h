#ifndef COMPONENT_H
#define COMPONENT_H

#include "header.h"

namespace phydb {

class Component {
    public: 
        int _id;
        string _name;
        string _macroName;
        string _source;
        string _locationType;
        string _orient;

        int _weight;
        Point2D<int> _location;
        
        Component() { }
        Component(string name, string macroName, string source, string locationType,
            string orient, int weight, Point2D<int> location):
            _name(name),
            _macroName(macroName),
            _source(source),
            _locationType(locationType),
            _orient(orient),
            _weight(weight),
            _location(location)
            { }

};

ostream& operator<< (ostream& , const Component &);


}


#endif

