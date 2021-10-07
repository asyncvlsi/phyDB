#include "site.h"

namespace phydb {

void Site::SetName(std::string name) {
    name_ = name;
}

void Site::SetClassName(std::string className) {
    class_name_ = className;
}

void Site::SetWidth(double width) {
    width_ = width;
}

void Site::SetHeight(double height) {
    height_ = height;
}

std::string Site::GetName( ) const {
    return name_;
}

std::string Site::GetClassName( ) const {
    return class_name_;
}

double Site::GetWidth( ) const {
    return width_;
}

double Site::GetHeight( ) const {
    return height_;
}

std::ostream& operator<< (std::ostream& os, const Site& s) {
    os << s.GetName() << " " << s.GetClassName() << "\n";
    os << s.GetWidth() << " " << s.GetHeight() << "\n";
    return os;

}

}
