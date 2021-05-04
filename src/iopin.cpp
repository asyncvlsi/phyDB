#include "iopin.h"

namespace phydb {

void IOPin::SetNetName(std::string const &net_name) {
    net_name_ = net_name;
}

void IOPin::SetShape(std::string &layer_name, int lx, int ly, int ux, int uy) {
    layer_name_ = layer_name;
    rect_.Set(lx, ly, ux, uy);
}

void IOPin::SetPlacement(PlaceStatus place_status, int x, int y, CompOrient orient) {
    place_status_ = place_status;
    location_.x = x;
    location_.y = y;
    orient_ = orient;
}

const string &IOPin::GetName() {
    return name_;   
}

const string& IOPin::GetNetName() {
    return net_name_;
}

SignalDirection IOPin::GetDirection() {
    return direction_;
}

SignalUse IOPin::GetUse() {
    return use_;
}

const string &IOPin::GetLayerName() {
    return layer_name_;
}

Rect2D<int> IOPin::GetRect() {
    return rect_;
}

Point2D<int> IOPin::GetLocation() {
    return location_;
}

CompOrient IOPin::GetOrientation() {
    return orient_;
}

PlaceStatus IOPin::GetPlacementStatus() {
    return place_status_;
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
