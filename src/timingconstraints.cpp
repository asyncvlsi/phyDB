//
// Created by yihang on 4/18/21.
//

#include "timingconstraints.h"

#include "phydb_header.h"

namespace phydb {

bool ActPhyDBTimingAPI::IsActNetPtrExisting(void *act_net) {
    return net_act_2_id_.find(act_net) != net_act_2_id_.end();
}

int ActPhyDBTimingAPI::ActNetPtr2Id(void *act_net) {
    if (IsActNetPtrExisting(act_net)) {
        return net_act_2_id_[act_net];
    }
    return -1;
}

void ActPhyDBTimingAPI::AddActNetPtrIdPair(void *act_net, int net_id) {
    std::pair<void *, int> tmp_pair_0(act_net, net_id);
    std::pair<int, void *> tmp_pair_1(net_id, act_net);
    net_act_2_id_.insert(tmp_pair_0);
    net_id_2_act_.insert(tmp_pair_1);
}

bool ActPhyDBTimingAPI::IsActComPinPtrExisting(void *act_comp) {
    return component_pin_act_2_id_.find(act_comp) != component_pin_act_2_id_.end();
}

std::pair<int, int> ActPhyDBTimingAPI::ActCompPinPtr2Id(void *act_comp) {
    if (IsActComPinPtrExisting(act_comp)) {
        return component_pin_act_2_id_[act_comp];
    }
    return std::make_pair<int, int>(-1, -1);
}

void ActPhyDBTimingAPI::AddActCompPinPtrIdPair(void *act_comp, int comp_id, int pin_id) {
    std::pair<int, int> id_pair(comp_id, pin_id);
    std::pair<void *, std::pair<int, int>> tmp_pair_0(act_comp, id_pair);
    std::pair<std::pair<int, int>, void *> tmp_pair_1(id_pair, act_comp);
    component_pin_act_2_id_.insert(tmp_pair_0);
    component_pin_id_2_act_.insert(tmp_pair_1);
}

int ActPhyDBTimingAPI::GetNumConstraints() {
    if (GetNumConstraintsCB != nullptr) {
        return GetNumConstraintsCB();
    }
}

void ActPhyDBTimingAPI::UpdateTimingIncremental() {
    if (UpdateTimingIncrementalCB != nullptr) {
        UpdateTimingIncrementalCB();
    }
}

double ActPhyDBTimingAPI::GetRequiredMargin(int tc_num) {
    if (GetRequiredMarginCB != nullptr) {
        return GetRequiredMarginCB(tc_num);
    }
}

double ActPhyDBTimingAPI::GetActualMargin(int tc_num) {
    if (GetActualMarginCB != nullptr) {
        return GetActualMarginCB(tc_num);
    }
}

void ActPhyDBTimingAPI::GetWitness(int tc_num,
                                   std::vector<PhyDBEdge> &phydb_fast_path,
                                   std::vector<PhyDBEdge> &phydb_slow_path) {
    if (GetWitnessCB != nullptr) {
        std::vector<ActEdge> act_fast_path;
        std::vector<ActEdge> act_slow_path;

        GetWitnessCB(tc_num, act_fast_path, act_slow_path);

        Translate(act_fast_path, phydb_fast_path);
        Translate(act_slow_path, phydb_slow_path);
    }
}

void ActPhyDBTimingAPI::GetViolatedTimingConstraints(std::vector<int> &violated_tc_nums) {
    if (GetViolatedTimingConstraintsCB != nullptr) {
        GetViolatedTimingConstraintsCB(violated_tc_nums);
    }
}

void ActPhyDBTimingAPI::Translate(std::vector<ActEdge> &act_path, std::vector<PhyDBEdge> &phydb_path) {
    phydb_path.clear();

    for (auto &act_edge: act_path) {
        PhyDBEdge &phydb_edge = phydb_path.emplace_back();
        PhyDBExpects(IsActComPinPtrExisting(act_edge.driv_pin),
                     "Cannot translate ActEdge to PhyDBEdge, ActEdge not in the database");
        PhyDBExpects(IsActComPinPtrExisting(act_edge.load_pin),
                     "Cannot translate ActEdge to PhyDBEdge, ActEdge not in the database");
        PhyDBExpects(IsActNetPtrExisting(act_edge.net_ptr),
                     "Cannot translate ActEdge to PhyDBEdge, ActEdge not in the database");
        PhyDBExpects(act_edge.delay >= 0, "Negative delay?");
        phydb_edge.driv_pin = ActCompPinPtr2Id(act_edge.driv_pin);
        phydb_edge.load_pin = ActCompPinPtr2Id(act_edge.load_pin);
        phydb_edge.net_index = ActNetPtr2Id(act_edge.net_ptr);
        phydb_edge.delay = act_edge.delay;
    }
}

}