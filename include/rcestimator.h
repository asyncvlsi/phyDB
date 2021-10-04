#ifndef PHYDB_INCLUDE_RCESTIMATOR_H_
#define PHYDB_INCLUDE_RCESTIMATOR_H_

#include "net.h"

namespace phydb {

class PhyDB;

enum RCEstimatorType {
    STARPIMODEL = 0,
    FLUTEMODEL = 1
};

class RCEstimator {
  protected:
    PhyDB *phy_db_;
  public:
    explicit RCEstimator(PhyDB *phydb_ptr) : phy_db_(phydb_ptr) {}
    virtual ~RCEstimator() = default;

    virtual void PushNetRCToManager(int net_id) = 0;
};

class StarNetPiRCEstimator : public RCEstimator {
  public:
    explicit StarNetPiRCEstimator(PhyDB *phydb_ptr) : RCEstimator(phydb_ptr) {}
    ~StarNetPiRCEstimator() override = default;
    void PushNetRCToManager(int net_id) override;

    void InitializeRCTree(Net &net);
};

}

#endif //PHYDB_INCLUDE_RCESTIMATOR_H_
