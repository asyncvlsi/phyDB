#include "site.h"

namespace phydb {

void Site::setName(string name) {
    _name = name;
}

void Site::setClassName(string className) {
    _className = className;
}

void Site::setWidth(float width) {
    _width = width;
}

void Site::setHeight(float height) {
    _height = height;
}

string Site::getName( ) const {
    return _name;
}

string Site::getClassName( ) const {
    return _className;
}

float Site::getWidth( ) const {
    return _width;
}

float Site::getHeight( ) const {
    return _height;
}

ostream& operator<< (ostream& os, const Site& s) {
    os << s.getName() << " " << s.getClassName() << endl;
    os << s.getWidth() << " " << s.getHeight() << endl;
    return os;

}

}
