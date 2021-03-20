#ifndef NET_H
#define NET_H

#include "header.h"

namespace phydb {

class Net {
  public:
    string _name;
    string _source;
    string _use;
    string _direction;

    int _start;
    int _numTracks;
    int _step;

    vector<string> componentNames;
    vector<string> pinNames;

    Net() {}
    Net(string name, string source, string use, string direction, int start, int numTracks, int step) :
        _name(name),
        _direction(direction),
        _start(start),
        _numTracks(numTracks),
        _step(step) {}

    string getName() { return _name; }

};

ostream &operator<<(ostream &, const Track &);

}

#endif
