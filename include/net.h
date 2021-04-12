#ifndef NET_H
#define NET_H

#include "phydb_header.h"
#include "enumtypes.h"

namespace phydb {

class Net {
  public:
    std::string name_;
    SignalUse use_;

    double weight_;

    std::vector<string> component_names_;
    std::vector<string> pin_names_;
    std::vector<string> iopin_names_;

    Net() {}
    Net(std::string name, double weight) : name_(name), weight_(weight) {}

    void AddIoPin(std::string const &iopin_name);
    void AddCompPin(std::string const &comp_name, std::string const &pin_name);

    const std::string &GetName();
    vector<std::string> &GetComponentNamesRef();
    vector<std::string> &GetPinNamesRef();
    vector<std::string> &GetIoPinNamesRef();

    void Report();
};

ostream &operator<<(ostream &, const Net &);

}

#endif
