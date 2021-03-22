#include "iopin.h"

namespace phydb {

string IOPin::GetName() {
    return name_;
}

void IOPin::SetNetName(std::string const &net_name) {
    net_name_ = net_name;
}

void IOPin::SetShape(std::string &layer_name, int lx, int ly, int ux, int uy) {
    layer_name_ = layer_name;
    rect_.ll.x = lx;
    rect_.ll.y = ly;
    rect_.ur.x = ux;
    rect_.ur.y = uy;
}

void IOPin::SetPlacement(PlaceStatus place_status, int x, int y, CompOrient orient) {
    place_status_ = place_status;
    location_.x = x;
    location_.y = y;
    orient_ = orient;
}

void IOPin::Report() {
    std::cout << "IOPIN name: " << name_ << "  Net: " << net_name_ << " "
              << " DIRECTION: " << SignalDirectionStr(direction_) << " "
              << " USE: " << SignalUseStr(use_) << "\n";
    std::cout << "LAYER: " << layer_name_ << " " << rect_.Str() << "\n";
    std::cout << PlaceStatusStr(place_status_) << " "
              << location_.Str() << " "
              << CompOrientStr(orient_) << "\n";
}

}