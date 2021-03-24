#include "component.h"

namespace phydb {

void Component::SetPlacementStatus(PlaceStatus status) {
	place_status_ = status;
}

void Component::SetOrientation(CompOrient orient) {
	orient_ = orient;
}

void Component::SetLocation(int lx, int ly) {
	location_.x = lx;
	location_.y = ly;
}

const string &Component::GetName() {
    return name_;
}

const string &Component::GetMacroName() {
    return macro_name_;
}

PlaceStatus Component::GetPlacementStatus() {
	return place_status_;
}

CompOrient Component::GetOrientation() {
	return orient_;
}

Point2D<int> Component::GetLocation() {
	return location_;
}

ostream &operator<<(ostream &os, const Component &c) {
    os << c.name_ << " " << c.macro_name_ << " "
       << c.source_ << " " << PlaceStatusStr(c.place_status_) << " "
       << CompOrientStr(c.orient_) << endl;
    os << "weight: " << c.weight_ << " location: " << c.location_ << endl;
    return os;
}

}

