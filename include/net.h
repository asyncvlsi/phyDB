#ifndef NET_H
#define NET_H

#include "logging.h"
#include "enumtypes.h"
#include "datatype.h"

namespace phydb {

class Net {
  public:
    std::string name_;
    SignalUse use_;

    double weight_;

    std::vector<string> component_names_;
    std::vector<string> pin_names_;
    std::vector<string> iopin_names_;

    std::vector<Rect3D<int>> guides_;

    Net() {}
    Net(std::string name, double weight) : name_(name), weight_(weight) {}

    void AddIoPin(std::string const &iopin_name);
    void AddCompPin(std::string const &comp_name, std::string const &pin_name);
    void AddRoutingGuide(int llx, int lly, int urx, int ury, int layerID);

    const std::string &GetName();
    vector<std::string> &GetComponentNamesRef();
    vector<std::string> &GetPinNamesRef();
    vector<std::string> &GetIoPinNamesRef();
    vector<Rect3D<int>>& GetRoutingGuidesRef();

    void Report();
};

ostream &operator<<(ostream &, const Net &);

}

#endif
