#ifndef PHYDB_INCLUDE_NET_H_
#define PHYDB_INCLUDE_NET_H_

#include "actphydbtimingapi.h"
#include "datatype.h"
#include "enumtypes.h"
#include "logging.h"
#include "snet.h"

namespace phydb {

class Net {
  public:
    std::string name_;
    SignalUse use_;

    double weight_;

    std::vector<PhydbPin> pins_;
    std::vector<std::string> iopin_names_;
    int driver_pin_id_ = 0;
    std::vector<Rect3D<int>> guides_;

    std::vector<Path> paths_;

    Net() {}
    Net(const std::string &name, double weight)
        : name_(name), weight_(weight) {}

    void AddIoPin(std::string const &iopin_name);
    void AddCompPin(int comp_id, int pin_id);
    void AddRoutingGuide(int llx, int lly, int urx, int ury, int layerID);

    Path *AddPath();
    Path *AddPath(std::string &layer_name, std::string shape, int width = 0); //by default, width of signal nets is the standard width

    const std::string &GetName() const;
    std::vector<PhydbPin> &GetPinsRef();
    std::vector<std::string> &GetIoPinNamesRef();
    std::vector<Rect3D<int>> &GetRoutingGuidesRef();
    std::vector<Path> &GetPathsRef();

    void Report();
};

std::ostream &operator<< (std::ostream &, const Net &);

}

#endif //PHYDB_INCLUDE_NET_H_
