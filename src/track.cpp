#include "track.h"

namespace phydb {

XYDirection Track::GetDirection() const {
    return direction_;
}

int Track::GetStart() const {
    return start_;
}

int Track::GetNTracks() const {
    return n_tracks_;
}

int Track::GetStep() const {
    return step_;
}

vector<string>& Track::GetLayerNames() {
    return layer_names_;
}

ostream& operator<< (ostream& os, const Track& t) {
    os << "direction: " << XYDirectionToStr(t.direction_) ;
    os << " start: " << t.start_ << " numTracks:" << t.n_tracks_ << " step: " << t.step_ ;

    return os;
}


}

