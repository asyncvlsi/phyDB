#include "site.h"

namespace phydb {

void Site::SetName(string name) {
    name_ = name;
}

void Site::SetClassName(string className) {
    class_name_ = className;
}

void Site::SetWidth(float width) {
    width_ = width;
}

void Site::SetHeight(float height) {
    height_ = height;
}

string Site::GetName( ) const {
    return name_;
}

string Site::GetClassName( ) const {
    return class_name_;
}

float Site::GetWidth( ) const {
    return width_;
}

float Site::GetHeight( ) const {
    return height_;
}

ostream& operator<< (ostream& os, const Site& s) {
    os << s.GetName() << " " << s.GetClassName() << endl;
    os << s.GetWidth() << " " << s.GetHeight() << endl;
    return os;

}

}
