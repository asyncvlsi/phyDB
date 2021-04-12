#ifndef IOPIN_H
#define IOPIN_H

#include "DataType.h"
#include "phydb_header.h"
#include "enumtypes.h"

namespace phydb {

class IOPin {
  public:
    int id_;
    string name_;
    string net_name_;
    SignalDirection direction_;
    SignalUse use_;

    string layer_name_;
    Rect2D<int> rect_;

    Point2D<int> location_;
    CompOrient orient_;
    PlaceStatus place_status_;

    IOPin() : id_(-1) {}
    IOPin(std::string &name, SignalDirection direction, SignalUse use) :
        name_(name), direction_(direction), use_(use) {}
    IOPin(string name, string netName, SignalDirection direction, SignalUse use, string layerName,
          Rect2D<int> rect, Point2D<int> location, CompOrient orient, PlaceStatus status) :
        name_(name),
        net_name_(netName),
        direction_(direction),
        use_(use),
        layer_name_(layerName),
        rect_(rect),
        location_(location),
        orient_(orient),
        place_status_(status) {}

    void SetNetName(std::string const &net_name);
    void SetShape(std::string &layer_name, int lx, int ly, int ux, int uy);
    void SetPlacement(PlaceStatus place_status, int x, int y, CompOrient orient);

    const string &GetName();
    SignalDirection GetDirection();
    SignalUse GetUse();
    const string &GetLayerName();
    Rect2D<int> GetRect();
    Point2D<int> GetLocation();
    CompOrient GetOrientation();
    PlaceStatus GetPlacementStatus();

    void Report();
};

ostream &operator<<(ostream &, const IOPin &);

}

#endif




