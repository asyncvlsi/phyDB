//
// Created by yihang on 4/18/21.
//

#include "actphydbtimingapi.h"

#include "logging.h"

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

PhydbPin ActPhyDBTimingAPI::ActCompPinPtr2Id(void *act_comp) {
    if (IsActComPinPtrExisting(act_comp)) {
        return component_pin_act_2_id_[act_comp];
    }
    return PhydbPin(-1, -1);
}

void ActPhyDBTimingAPI::AddActCompPinPtrIdPair(void *act_comp, int comp_id, int pin_id) {
    PhydbPin id_pair(comp_id, pin_id);
    std::pair<void *, PhydbPin> tmp_pair_0(act_comp, id_pair);
    std::pair<PhydbPin, void *> tmp_pair_1(id_pair, act_comp);
    component_pin_act_2_id_.insert(tmp_pair_0);
    component_pin_id_2_act_.insert(tmp_pair_1);
}

void ActPhyDBTimingAPI::SetGetNumConstraintsCB(int (*callback_function)()) {
    GetNumConstraintsCB = callback_function;
}

void ActPhyDBTimingAPI::SetUpdateTimingIncrementalCB(void (*callback_function)()) {
    UpdateTimingIncrementalCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetSlackCB(double (*callback_function)(int)) {
    GetActualMarginCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetWitnessCB(void (*callback_function)(int, std::vector<ActEdge> &, std::vector<ActEdge> &)) {
    GetWitnessCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetViolatedTimingConstraintsCB(void (*callback_function)(std::vector<int> &)) {
    GetViolatedTimingConstraintsCB = callback_function;
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

double ActPhyDBTimingAPI::GetSlack(int tc_num) {
    if (GetActualMarginCB != nullptr) {
        return GetActualMarginCB(tc_num);
    }
}

void ActPhyDBTimingAPI::GetWitness(int tc_num,
                                   std::vector<PhydbEdge> &phydb_fast_path,
                                   std::vector<PhydbEdge> &phydb_slow_path) {
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

void ActPhyDBTimingAPI::Translate(std::vector<ActEdge> &act_path, std::vector<PhydbEdge> &phydb_path) {
    phydb_path.clear();

    for (auto &act_edge: act_path) {
        PhydbEdge &phydb_edge = phydb_path.emplace_back();
        PhyDBExpects(IsActComPinPtrExisting(act_edge.source),
                     "Cannot translate ActEdge to PhydbEdge, ActEdge not in the database");
        PhyDBExpects(IsActComPinPtrExisting(act_edge.target),
                     "Cannot translate ActEdge to PhydbEdge, ActEdge not in the database");
        PhyDBExpects(IsActNetPtrExisting(act_edge.net_ptr),
                     "Cannot translate ActEdge to PhydbEdge, ActEdge not in the database");
        PhyDBExpects(act_edge.delay >= 0, "Negative delay?");
        phydb_edge.source = ActCompPinPtr2Id(act_edge.source);
        phydb_edge.target = ActCompPinPtr2Id(act_edge.target);
        phydb_edge.net_index = ActNetPtr2Id(act_edge.net_ptr);
        phydb_edge.delay = act_edge.delay;
    }
}

}