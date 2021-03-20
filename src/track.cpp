#include "track.h"

namespace phydb {

ostream& operator<< (ostream& os, const Track& t) {
    os << t._direction << endl;
    os << "start: " << t._start << " numTracks:" << t._numTracks << " step: "<< t._step << endl; 

    return os;
}


}

