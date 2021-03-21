#ifndef IOPIN_H
#define IOPIN_H

#include "DataType.h"
#include "header.h"

namespace phydb {

class IOPin {
  public:
    int id_;
    string name_;
    string net_name_;
    string direction_;
    string use_;

    string layer_name_;
    Rect2D<int> rect_;

    Point2D<int> location_;
    string orient_;
    string status_;

    IOPin() : id_(-1) {}
    IOPin(std::string &name, std::string &netName, std::string &direction, std::string &use, int lx, int ly) :
        name_(name), net_name_(netName), direction_(direction), use_(use) {
        location_.x = lx;
        location_.y = ly;
    }
    IOPin(string name, string netName, string direction, string use, string layerName,
          Rect2D<int> rect, Point2D<int> location, string orient, string status) :
        name_(name),
        net_name_(netName),
        direction_(direction),
        use_(use),
        layer_name_(layerName),
        rect_(rect),
        location_(location),
        orient_(orient),
        status_(status) {}

    string GetName();
    void SetNetName(std::string const &net_name);

};

ostream &operator<<(ostream &, const IOPin &);

}

#endif




