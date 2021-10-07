#ifndef PHYDB_INCLUDE_NET_H_
#define PHYDB_INCLUDE_NET_H_

#include "actphydbtimingapi.h"
#include "datatype.h"
#include "enumtypes.h"
#include "logging.h"

namespace phydb {

class Net {
  public:
    std::string name_;
    SignalUse use_;

    double weight_;

    std::vector<PhydbPin> pins_;
    std::vector<std::string> iopin_names_;

    std::vector<Rect3D < int>> guides_;

    Net() {}
    Net(const std::string &name, double weight)
        : name_(name), weight_(weight) {}

    void AddIoPin(std::string const &iopin_name);
    void AddCompPin(int comp_id, int pin_id);
    void AddRoutingGuide(int llx, int lly, int urx, int ury, int layerID);

    const std::string &GetName() const;
    std::vector<PhydbPin> &GetPinsRef();
    std::vector<std::string> &GetIoPinNamesRef();
    std::vector<Rect3D < int>>&
    GetRoutingGuidesRef();

    void Report();
};

std::ostream &operator<<(std::ostream &, const Net &);

}

#endif //PHYDB_INCLUDE_NET_H_
