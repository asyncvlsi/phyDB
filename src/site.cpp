#include "site.h"

namespace phydb {

void Site::SetName(string name) {
    name_ = name;
}

void Site::SetClassName(string className) {
    class_name_ = className;
}

void Site::SetWidth(double width) {
    width_ = width;
}

void Site::SetHeight(double height) {
    height_ = height;
}

string Site::GetName( ) const {
    return name_;
}

string Site::GetClassName( ) const {
    return class_name_;
}

double Site::GetWidth( ) const {
    return width_;
}

double Site::GetHeight( ) const {
    return height_;
}

ostream& operator<< (ostream& os, const Site& s) {
    os << s.GetName() << " " << s.GetClassName() << endl;
    os << s.GetWidth() << " " << s.GetHeight() << endl;
    return os;

}

}
