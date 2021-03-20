#include "row.h"

namespace phydb {

ostream& operator<< (ostream& os, const Row& r) {
    os << r._name << " " << r._siteName << " " << r._siteOrient << endl;
    os << "orig: " << r._origX << ", " << r._origY << endl;
    os << "num: " << r._numX << ", " << r._numY << endl;
    os << "step: " << r._stepX << ", " << r._stepY << endl;
    return os;
}

}
