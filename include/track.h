#ifndef TRACK_H
#define TRACK_H

#include "header.h"

namespace phydb {

class Track {
  public:
    string _direction;
    int _start;
    int _nTracks;
    int _step;

    vector<string> layerNames;

    Track() {}
    Track(string& direction, int start, int nTracks, int step, vector<string>& layerNames):
        _direction(direction),
        _start(start),
        _nTracks(nTracks),
        _step(step),
        _layerNames(layerNames)
        {}

};

ostream &operator<<(ostream &, const Track &);

}

#endif

