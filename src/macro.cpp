#include "macro.h"

namespace phydb {

string Macro::getName() const {
    return _name;
}

void Macro::setName(string &name) {
    _name = name;
}

void Macro::setOrigin(Point2D<float> origin) {
    _origin = origin;
}

void Macro::setOrigin(float x, float y) {
    _origin.x = x;
    _origin.y = y;
}

void Macro::setSize(Point2D<float> size) {
    _size = size;
}

void Macro::setSize(float width, float height) {
    _size.x = width;
    _size.y = height;
}

Point2D<float> Macro::getOrigin() const {
    return _origin;
}

Point2D<float> &Macro::getOriginRef() {
    return _origin;
}

ostream &operator<<(ostream &os, const Macro &macro) {
    os << macro._name << endl;
    os << macro._origin << endl;
    os << macro._size << endl;

    for (auto p : macro._pins) {
        os << p << endl;
    }
    os << macro._obs << endl;
    return os;

}

}

