#include "phydb.h"
#include "rcestimator.h"

namespace phydb {

void StarNetPiRCEstimator::InitializeRCTree(Net &net) {
    // initialize an empty RC-tree for a given net

}

void StarNetPiRCEstimator::ExtractNetRC(int net_id) {
    Net &net = phy_db_->GetDesignPtr()->nets_[net_id];
    double cap_per_sq_dist = phy_db_->GetTechPtr()->GetLayersRef()[0].GetCPerSqDist();
    double res_per_sq_unit = phy_db_->GetTechPtr()->GetLayersRef()[0].GetRPerSqUnit();

}

void StarNetPiRCEstimator::PushNetRCToManager(int net_id) {

}

}