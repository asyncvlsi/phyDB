#ifndef NET_H
#define NET_H

#include "header.h"
#include "enumtypes.h"

namespace phydb {

class Net {
  public:
    string name_;
    string source_;
    SignalUse use_;
    string direction_;

    int start_;
    int num_tracks_;
    int step_;

    double weight_;

    vector<string> component_names_;
    vector<string> pin_names_;
    vector<string> iopin_names_;

    Net() {}
    Net(std::string name, double weight) : name_(name), weight_(weight) {}
    Net(string name, string source, SignalUse use, string direction, int start, int numTracks, int step) :
        name_(name),
        direction_(direction),
        start_(start),
        num_tracks_(numTracks),
        step_(step) {}

    void AddIoPin(std::string const &iopin_name);
    void AddCompPin(std::string const &comp_name, std::string const &pin_name);

    const string &GetName();
    vector<string> &GetComponentNamesRef();
    vector<string> &GetPinNamesRef();
    vector<string> &GetIoPinNamesRef();

    void Report();
};

ostream &operator<<(ostream &, const Net &);

}

#endif
