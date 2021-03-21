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

    double weight_;

    vector<string> componentNames;
    vector<string> pinNames;
    vector<string> iopin_names_;

    Net() {}
    Net(std::string name, double weight): _name(name), weight_(weight) {}
    Net(string name, string source, string use, string direction, int start, int numTracks, int step) :
        _name(name),
        _direction(direction),
        _start(start),
        _numTracks(numTracks),
        _step(step) {}

    string getName();

    void AddIoPin(std::string const &iopin_name);
    void AddCompPin(std::string const &comp_name, std::string const &pin_name);

};

ostream &operator<<(ostream &, const Net &);

}

#endif
