#include "track.h"

namespace phydb {

ostream& operator<< (ostream& os, const Track& t) {
    os << t.direction_ << endl;
    os << "start: " << t.start_ << " numTracks:" << t.n_tracks_ << " step: " << t.step_ << endl;

    return os;
}


}

