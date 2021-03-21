#ifndef TRACK_H
#define TRACK_H

#include "header.h"

namespace phydb {

class Track {
  public:
    string direction_;
    int start_;
    int n_tracks_;
    int step_;

    vector<string> layer_names_;

    Track() {}
    Track(string &direction, int start, int nTracks, int step, vector<string> &layerNames) :
        direction_(direction),
        start_(start),
        n_tracks_(nTracks),
        step_(step),
        layer_names_(layerNames) {}

};

ostream &operator<<(ostream &, const Track &);

}

#endif

