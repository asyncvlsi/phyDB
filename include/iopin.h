#ifndef IOPIN_H
#define IOPIN_H

#include "DataType.h"
#include "header.h"

namespace phydb {

class IOPin {
  public:
    int _id;
    string _name;
    string _netName;
    string _direction;
    string _use;

    string _layerName;
    Rect2D<int> _rect;

    Point2D<int> _location;
    string _orient;
    string _status;

    IOPin() : _id(-1) {}
    IOPin(std::string &name, std::string &netName, std::string &direction, std::string &use, int lx, int ly) :
        _name(name), _netName(netName), _direction(direction), _use(use) {
        _location.x = lx;
        _location.y = ly;
    }
    IOPin(string name, string netName, string direction, string use, string layerName,
          Rect2D<int> rect, Point2D<int> location, string orient, string status) :
        _name(name),
        _netName(netName),
        _direction(direction),
        _use(use),
        _layerName(layerName),
        _rect(rect),
        _location(location),
        _orient(orient),
        _status(status) {}

    string getName();
    void SetNetName(std::string const &net_name);

};

ostream &operator<<(ostream &, const IOPin &);

}

#endif




