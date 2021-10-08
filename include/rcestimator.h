#ifndef PHYDB_INCLUDE_RCESTIMATOR_H_
#define PHYDB_INCLUDE_RCESTIMATOR_H_

#include "datatype.h"
#include "layer.h"
#include "net.h"

namespace phydb {

class PhyDB;

enum RCEstimatorType {
    STARPIMODEL = 0
};

class RCEstimator {
  protected:
    PhyDB *phy_db_;
  public:
    explicit RCEstimator(PhyDB *phydb_ptr) : phy_db_(phydb_ptr) {}
    virtual ~RCEstimator() = default;

    virtual void PushNetRCToManager() = 0;
};

class StarPiModelEstimator : public RCEstimator {
  public:
    explicit StarPiModelEstimator(PhyDB *phydb_ptr) : RCEstimator(phydb_ptr) {}
    ~StarPiModelEstimator() override = default;
    void PushNetRCToManager() override;
  private:
    int distance_micron_ = 0;
    bool edge_pushed_to_spef_manager_ = false;
    Layer *horizontal_layer_ = nullptr;
    Layer *vertical_layer_ = nullptr;
    void AddEdgesToManager();
    void FindFirstHorizontalAndVerticalMetalLayer();
    void GetResistanceAndCapacitance(
        Point2D<int> &driver_loc,
        Point2D<int> &load_loc,
        double &resistance,
        double &capacitance
    );
};

}

#endif //PHYDB_INCLUDE_RCESTIMATOR_H_
