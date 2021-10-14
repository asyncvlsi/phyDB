#ifndef PHYDB_INCLUDE_RCESTIMATOR_H_
#define PHYDB_INCLUDE_RCESTIMATOR_H_

#include "datatype.h"
#include "layer.h"
#include "net.h"
#include "phydb.h"

namespace phydb {

class RCEstimator {
  protected:
    PhyDB *phy_db_;
  public:
    explicit RCEstimator(PhyDB *phydb_ptr) : phy_db_(phydb_ptr) {}
    virtual ~RCEstimator() = default;

    virtual void PushNetRCToManager() = 0;
};

}

#endif //PHYDB_INCLUDE_RCESTIMATOR_H_
