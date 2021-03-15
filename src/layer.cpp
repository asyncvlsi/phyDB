#include "layer.h"

namespace phydb {

void Layer::setSpacingTable(SpacingTable& st) {
    _spacingTable = st;
}

void Layer::setEOLSpacing(EOLSpacing& eolSpacing) {
    _eolSpacing = eolSpacing;
}

void Layer::setCornerSpacing(CornerSpacing& cornerSpacing) {
    _cornerSpacing = cornerSpacing;
}
    
SpacingTable Layer::getSpacingTable( ) {
    return _spacingTable;
}

EOLSpacing Layer::getEOLSpacing( ) {
    return _eolSpacing;
}

CornerSpacing Layer::getCornerSpacing( ) {
    return _cornerSpacing;
}

ostream& operator<< (ostream& os, const Layer& l) {
    os << l._name << " " << l._type << " " << l._idx << " " << l._direction << endl;
    os << l._pitchx << " " << l._pitchy << " " << l._width << " " << l._area << endl;
    os << l._minWidth << " " << l._offset << endl;
    os << l._spacing << endl;
    return os;
}


}
