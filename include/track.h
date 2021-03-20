#ifndef TRACK_H
#define TRACK_H

#include "header.h"

namespace phydb {

class Track {
  public:
    string _direction;
    int _start;
    int _numTracks;
    int _step;

    std::vector<string> layerNames;

    Track() {}
    Track(string direction, int start, int numTracks, int step) :
        _direction(direction),
        _start(start),
        _numTracks(numTracks),
        _step(step) {}

};

ostream &operator<<(ostream &, const Track &);

}

#endif

