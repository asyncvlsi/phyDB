#ifndef COMPONENT_H
#define COMPONENT_H

#include "header.h"
#include "DataType.h"

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

    Component() {}
    Component(string name, string macroName, string source, string locationType,
              string orient, int weight, Point2D<int> location) :
        _name(name),
        _macroName(macroName),
        _source(source),
        _locationType(locationType),
        _orient(orient),
        _weight(weight),
        _location(location) {}
    Component(std::string &comp_name, std::string &macro_name, std::string &place_status,
              int llx, int lly, std::string &orient)
        : _name(comp_name), _macroName(macro_name), _locationType(place_status), _orient(orient) {
        _location.x = llx;
        _location.y = lly;
    }

    string getName() { return _name; }

};

ostream &operator<<(ostream &, const Component &);

}

#endif

