#include "rcestimator.h"

#include <cmath>

#include "config.h"
#include "phydb.h"

namespace phydb {

void StarPiModelEstimator::PushNetRCToManager() {
#if PHYDB_USE_GALOIS
    FindFirstHorizontalAndVerticalMetalLayer();
    AddEdgesToManager();
    auto maxMode = galois::eda::utility::AnalysisMode::ANALYSIS_MAX;
    auto &timing_api = phy_db_->GetTimingApi();
    auto *spef_manager = phy_db_->GetParaManager();
    auto &libs = phy_db_->GetCellLibs();
    PhyDBExpects(!libs.empty(), "CellLibs empty?");
    for (auto &net: phy_db_->design().nets_) {
        int driver_id = net.driver_pin_id_;
        auto &net_pins = net.GetPinsRef();
        auto &driver = net_pins[driver_id];
        std::string driver_name = phy_db_->GetFullCompPinName(driver);
        auto *driver_node = timing_api.PhyDBPinToSpefNode(driver);
        double driver_cap = 0;
        // this is just a rough estimation
        Point2D<int> driver_pin_loc =
            phy_db_->design().components_[driver.comp_id].GetLocation();
        int net_sz = (int) net_pins.size();
        for (int i = 0; i < net_sz; ++i) {
            if (i == driver_id) continue;
            PhydbPin &load = net_pins[i];
            std::string load_name = phy_db_->GetFullCompPinName(load);
            auto load_node = timing_api.PhyDBPinToSpefNode(load);
            double res, cap;
            // this is just a rough estimation
            Point2D<int> load_pin_loc =
                phy_db_->design().components_[load.comp_id].GetLocation();
            GetResistanceAndCapacitance(
                driver_pin_loc,
                load_pin_loc,
                res,
                cap
            );
            load_node->setC(libs[0], maxMode, cap / 2.0);
            std::cout << "Set C for load pin: " << load_name << " " << cap / 2.0 << "\n";
            driver_cap += cap / 2.0;
            auto edge = spef_manager->findEdge(driver_node, load_node);
            PhyDBExpects(edge!=nullptr, "Cannot find edge!");
            edge->setR(libs[0], maxMode, 0.12345);
            std::cout << "Set R for edge, "
                      << "driver: " << driver_name << ", "
                      << "load: " << load_name << ", "
                      << res << "\n";
        }
        driver_node->setC(libs[0], maxMode, driver_cap);
        std::cout << "Set C for driver pin: " << driver_name << " " << driver_cap << "\n";
    }
    //std::cout << "after adding\n";
    //spef_manager->dump();
    //std::cout << "--------------------------------------------------\n";
#endif
}

void StarPiModelEstimator::AddEdgesToManager() {
#if PHYDB_USE_GALOIS
    if (edge_pushed_to_spef_manager_) return;
    edge_pushed_to_spef_manager_ = true;
    ActPhyDBTimingAPI &timing_api = phy_db_->GetTimingApi();
    auto spef_manager = phy_db_->GetParaManager();
    for (auto &net: phy_db_->design().nets_) {
        int driver_id = net.driver_pin_id_;
        auto &net_pins = net.GetPinsRef();
        PhydbPin &driver = net_pins[driver_id];
        std::string driver_name = phy_db_->GetFullCompPinName(driver);
        auto driver_node = timing_api.PhyDBPinToSpefNode(driver);
        int net_sz = (int) net_pins.size();
        for (int i = 0; i < net_sz; ++i) {
            if (i == driver_id) continue;
            PhydbPin &load = net_pins[i];
            std::string load_name = phy_db_->GetFullCompPinName(load);
            auto load_node = timing_api.PhyDBPinToSpefNode(load);
            auto ret = spef_manager->addEdge(driver_node, load_node);
            PhyDBExpects(ret != nullptr, "Fail to add an edge\n");
            std::cout << "Add an edge to spef_manager, "
                      << "driver: " << driver_name << ", "
                      << "load: " << load_name << "\n";
        }
    }
#endif
}

void StarPiModelEstimator::FindFirstHorizontalAndVerticalMetalLayer() {
    distance_micron_ = phy_db_->design().GetUnitsDistanceMicrons();
    if (horizontal_layer_ != nullptr && vertical_layer_ != nullptr) return;
    for (auto &metal: phy_db_->tech().metal_layers_) {
        if (metal->GetDirection() == HORIZONTAL
            && horizontal_layer_ == nullptr) {
            horizontal_layer_ = metal;
        }
        if (metal->GetDirection() == VERTICAL && vertical_layer_ == nullptr) {
            vertical_layer_ = metal;
        }
    }

    PhyDBExpects(horizontal_layer_ != nullptr,
                 "Cannot find a horizontal layer?");
    PhyDBExpects(vertical_layer_ != nullptr, "Cannot find a vertical layer?");
}

void StarPiModelEstimator::GetResistanceAndCapacitance(
    Point2D<int> &driver_loc,
    Point2D<int> &load_loc,
    double &resistance,
    double &capacitance
) {
    double x_span =
        std::abs(driver_loc.x - load_loc.x) / (double) distance_micron_;
    double y_span =
        std::abs(driver_loc.y - load_loc.y) / (double) distance_micron_;

    double hor_res = horizontal_layer_->GetResistance(
        horizontal_layer_->GetWidth(),
        x_span,
        0
    );
    double ver_res = vertical_layer_->GetResistance(
        vertical_layer_->GetWidth(),
        y_span,
        0
    );
    resistance = hor_res + ver_res;

    double hor_cap = horizontal_layer_->GetFringeCapacitance(
        horizontal_layer_->GetWidth(),
        x_span,
        0
    );
    double ver_cap = vertical_layer_->GetFringeCapacitance(
        vertical_layer_->GetWidth(),
        y_span,
        0
    );
    capacitance = hor_cap + ver_cap;
}

}