/*******************************************************************************
 *
 * Copyright (c) 2021 Jiayuan He, Yihang Yang
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 ******************************************************************************/

#include "actphydbtimingapi.h"

#include "phydb/common/logging.h"

namespace phydb {

std::ostream &operator<<(std::ostream &ost, const PhydbPin &pin) {
  ost << "(" << pin.InstanceId() << ", " << pin.PinId() << ")";
  return ost;
}

PhydbTimingEdge *PhydbTimingNode::AddPinToOutEdges(PhydbPin &pin) {
  PhyDBExpects(!IsTargetPinExisting(pin),
               "Pin in OutEdges already, cannot add it again!" << source
                                                               << pin);
  int id = (int) out_edges.size();
  out_edges.emplace_back(pin);
  out_pin2index_[pin] = id;
  return &out_edges[id];
}

PhydbTimingEdge *PhydbTimingNode::GetOutEdge(PhydbPin &pin) {
  PhyDBExpects(IsTargetPinExisting(pin),
               "Pin not in OutEdges, cannot find it!" << source << pin);
  int id = out_pin2index_[pin];
  return &out_edges[id];
}

void PhydbTimingNode::AddEdge(PhydbTimingEdge &edge) {
  PhydbPin pin = edge.target;
  PhydbTimingEdge *edge_ptr = nullptr;
  if (IsTargetPinExisting(pin)) {
    edge_ptr = GetOutEdge(pin);
    PhyDBExpects((edge_ptr->delay == edge.delay)
                     && (edge_ptr->net_index == edge.net_index),
                 "Data value inconsistency during adding edge");
    edge_ptr->count += edge.count;
  } else {
    edge_ptr = AddPinToOutEdges(pin);
    edge_ptr->net_index = edge.net_index;
    edge_ptr->delay = edge.delay;
    edge_ptr->count = edge.count;
  }
}

void PhydbPath::Clear() {
  edges.clear();
  root.Reset();
}

void PhydbPath::AddEdge(
    PhydbPin &src,
    PhydbPin &tgt,
    int net_id,
    double dly,
    int cnt
) {
  if (edges.empty()) {
    root = src;
  } else {
    PhyDBExpects(src == edges.back().target,
                 "This edge does not form a linked list?");
  }
  PhydbTimingEdge &edge = edges.emplace_back(tgt);
  edge.net_index = net_id;
  edge.delay = dly;
  edge.count = cnt;
}

PhydbTimingNode *TimingDAG::AddPinToDag(PhydbPin &pin) {
  PhyDBExpects(!IsPinInDag(pin),
               "Pin in DAG already, cannot add it again!" << pin);
  int id = (int) stb_fast_nodes.size();
  stb_fast_nodes.emplace_back(pin);
  fast_pin2index_[pin] = id;
  return &stb_fast_nodes[id];
}

PhydbTimingNode *TimingDAG::GetPinNode(PhydbPin &pin) {
  PhyDBExpects(IsPinInDag(pin),
               "Pin not in DAG, cannot find it!" << pin);
  int id = fast_pin2index_[pin];
  return &stb_fast_nodes[id];
}

void TimingDAG::AddFastPath(PhydbPath &fast_path) {
  PhydbTimingNode *node = nullptr;
  if (!IsPinInDag(fast_path.root)) {
    node = AddPinToDag(fast_path.root);
  } else {
    node = GetPinNode(fast_path.root);
  }
  for (auto &edge : fast_path.edges) {
    node->AddEdge(edge);
    PhydbPin target = edge.target;
    node = GetPinNode(target);
  }
}

bool ActPhyDBTimingAPI::ReadyForTimingDriven() {
#if PHYDB_USE_GALOIS == 0
  return false;
#elif PHYDB_USE_GALOIS == 1
  if (GetNumConstraintsCB == nullptr) return false;
  if (UpdateTimingIncrementalCB == nullptr) return false;
  if (GetSlackCB == nullptr) return false;
  if (GetSlowWitnessCB == nullptr) return false;
  if (GetFastWitnessCB == nullptr) return false;
  if (GetViolatedTimingConstraintsCB == nullptr) return false;
#else
  PhyDBExpects(false, "This is not supposed to happen!");
#endif
  return true;
}

bool ActPhyDBTimingAPI::IsActNetPtrExisting(void *act_net) {
  return net_act_2_id_.find(act_net) != net_act_2_id_.end();
}

int ActPhyDBTimingAPI::ActNetPtr2Id(void *act_net) {
  if (IsActNetPtrExisting(act_net)) {
    return net_act_2_id_[act_net];
  }
  return -1;
}

void *ActPhyDBTimingAPI::PhydbNetId2ActPtr(int net_id) {
  auto ret = net_id_2_act_.find(net_id);
  if (ret != net_id_2_act_.end()) {
    return ret->second;
  }
  return nullptr;
}

void ActPhyDBTimingAPI::AddActNetPtrIdPair(void *act_net, int net_id) {
  PhyDBExpects(!IsActNetPtrExisting(act_net),
               "Cannot add ACT net again, it is in already in the PhyDB, net id: "
                   << net_id);
  std::pair<void *, int> tmp_pair_0(act_net, net_id);
  std::pair<int, void *> tmp_pair_1(net_id, act_net);
  net_act_2_id_.insert(tmp_pair_0);
  net_id_2_act_.insert(tmp_pair_1);
}

void ActPhyDBTimingAPI::BindActPinAndPhydbPin(
    void *act_pin,
    PhydbPin phydb_pin
) {
  std::pair<void *, PhydbPin> tmp_pair_0(act_pin, phydb_pin);
  std::pair<PhydbPin, void *> tmp_pair_1(phydb_pin, act_pin);
  component_pin_act_2_id_.insert(tmp_pair_0);
  component_pin_id_2_act_.insert(tmp_pair_1);
}

bool ActPhyDBTimingAPI::IsActComPinPtrExisting(void *act_pin) {
  return component_pin_act_2_id_.find(act_pin)
      != component_pin_act_2_id_.end();
}

PhydbPin ActPhyDBTimingAPI::ActCompPinPtr2Id(void *act_pin) {
  if (IsActComPinPtrExisting(act_pin)) {
    return component_pin_act_2_id_[act_pin];
  }
  return PhydbPin(-1, -1);
}

void *ActPhyDBTimingAPI::PhydbCompPin2ActPtr(PhydbPin phydb_pin) {
  auto ret = component_pin_id_2_act_.find(phydb_pin);
  if (ret != component_pin_id_2_act_.end()) {
    return ret->second;
  }
  return nullptr;
}

void ActPhyDBTimingAPI::SetGetNumConstraintsCB(int (*callback_function)()) {
  GetNumConstraintsCB = callback_function;
}

void ActPhyDBTimingAPI::SetSpecifyTopKsCB(void (*callback_function)(int)) {
  SpecifyTopKsCB = callback_function;
}

void ActPhyDBTimingAPI::SetSpecifyTopKCB(void (*callback_function)(int, int)) {
  SpecifyTopKCB = callback_function;
}

void ActPhyDBTimingAPI::SetUpdateTimingIncrementalCB(void (*callback_function)()) {
  UpdateTimingIncrementalCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetSlackCB(std::vector<double> (*callback_function)(
    const std::vector<int> &)) {
  GetSlackCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetViolatedTimingConstraintsCB(
    void (*callback_function)(std::vector<int> &)
) {
  GetViolatedTimingConstraintsCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetSlowWitnessCB(
    void (*callback_function)(
        int timing_constraint_id,
        std::vector<ActEdge> &path
    )
) {
  GetSlowWitnessCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetFastWitnessCB(
    void (*callback_function)(
        int timing_constraint_id,
        std::vector<ActEdge> &path
    )
) {
  GetFastWitnessCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetNumPerformanceConstraintsCB(
    int (*callback_function)()
) {
  GetNumPerformanceConstraintsCB = callback_function;
}

void ActPhyDBTimingAPI::SetSpecifyPerformanceTopKsCB(
    void (*callback_function)(int top_k)
) {
  SpecifyPerformanceTopKsCB = callback_function;
}

void ActPhyDBTimingAPI::SetSpecifyPerformanceTopKCB(
    void (*callback_function)(
        int performance_id,
        int top_k
    )
) {
  SpecifyPerformanceTopKCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetPerformanceConstraintWeightCB(
    double (*callback_function)(int performance_id)
) {
  GetPerformanceConstraintWeightCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetPerformanceSlackCB(
    double (*callback_function)(int performance_id)
) {
  GetPerformanceSlackCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetViolatedPerformanceConstraintsCB(
    void (*callback_function)(std::vector<int> &performance_ids)
) {
  GetViolatedPerformanceConstraintsCB = callback_function;
}

void ActPhyDBTimingAPI::SetGetPerformanceWitnessCB(
    void (*callback_function)(
        int performance_id,
        std::vector<ActEdge> &path
    )
) {
  GetPerformanceWitnessCB = callback_function;
}

int ActPhyDBTimingAPI::GetNumConstraints() {
  PhyDBExpects(GetNumConstraintsCB != nullptr,
               "Callback function for GetNumConstraints() is not set");
  return GetNumConstraintsCB();
}

void ActPhyDBTimingAPI::SpecifyTopKs(int k) {
  PhyDBExpects(SpecifyTopKsCB != nullptr,
               "Callback function for SpecifyTopKs() is not set");
  SpecifyTopKsCB(k);
}

void ActPhyDBTimingAPI::SpecifyTopK(int tc_num, int k) {
  PhyDBExpects(SpecifyTopKCB != nullptr,
               "Callback function for SpecifyTopK() is not set");
  SpecifyTopKCB(tc_num, k);
}

void ActPhyDBTimingAPI::UpdateTimingIncremental() {
  PhyDBExpects(UpdateTimingIncrementalCB != nullptr,
               "Callback function for UpdateTimingIncremental() is not set");
  UpdateTimingIncrementalCB();
}

double ActPhyDBTimingAPI::GetSlack(int tc_num) {
  PhyDBExpects(GetSlackCB != nullptr,
               "Callback function for GetSlack() is not set");
  return GetSlackCB(std::vector(1, tc_num))[0];
}

void ActPhyDBTimingAPI::GetViolatedTimingConstraints(std::vector<int> &violated_tc_nums) {
  PhyDBExpects(GetViolatedTimingConstraintsCB != nullptr,
               "Callback GetWitness for GetViolatedTimingConstraints() is not set");
  GetViolatedTimingConstraintsCB(violated_tc_nums);
}

#if PHYDB_USE_GALOIS
void ActPhyDBTimingAPI::SetParaManager(galois::eda::parasitics::Manager *manager) {
  para_manager_ = manager;
}
void ActPhyDBTimingAPI::AddCellLib(galois::eda::liberty::CellLib *lib) {
  libs_.push_back(lib);
}
void ActPhyDBTimingAPI::SetNetlistAdaptor(galois::eda::utility::ExtNetlistAdaptor *adaptor) {
  adaptor_ = adaptor;
}

galois::eda::parasitics::Manager *ActPhyDBTimingAPI::GetParaManager() {
  return para_manager_;
}

std::vector<galois::eda::liberty::CellLib *> &ActPhyDBTimingAPI::GetCellLibs() {
  return libs_;
}

galois::eda::utility::ExtNetlistAdaptor *ActPhyDBTimingAPI::GetNetlistAdaptor() {
  return adaptor_;
}

galois::eda::parasitics::Node *ActPhyDBTimingAPI::PhyDBPinToSpefNode(PhydbPin phydb_pin) {
  void *act_pin = PhydbCompPin2ActPtr(phydb_pin);
  PhyDBExpects(act_pin != nullptr, "Cannot map PhyDB pin to ACT?");
  auto node = para_manager_->findPin(act_pin);
  PhyDBExpects(node != nullptr, "Cannot map ACT pin to SPEF node?");
  return node;
}

void ActPhyDBTimingAPI::GetSlowWitness(
    int tc_num,
    PhydbPath &phydb_path
) {
  PhyDBExpects(GetSlowWitnessCB != nullptr,
               "Callback function for GetSlowWitness() is not set");
  std::vector<ActEdge> act_path;
  GetSlowWitnessCB(tc_num, act_path);
  TranslateActPathToPhydbPath(act_path, phydb_path);
}

void ActPhyDBTimingAPI::GetFastWitness(
    int tc_num,
    PhydbPath &phydb_path
) {
  PhyDBExpects(GetFastWitnessCB != nullptr,
               "Callback function for GetFastWitness() is not set");
  std::vector<ActEdge> act_path;
  GetFastWitnessCB(tc_num, act_path);
  TranslateActPathToPhydbPath(act_path, phydb_path);
}

int ActPhyDBTimingAPI::GetNumPerformanceConstraints() {
  PhyDBExpects(GetNumPerformanceConstraintsCB != nullptr,
               "Callback function for GetNumPerformanceConstraints() is not set");
  return GetNumPerformanceConstraintsCB();
}

void ActPhyDBTimingAPI::SpecifyPerformanceTopKs(int top_k) {
  PhyDBExpects(SpecifyPerformanceTopKsCB != nullptr,
               "Callback function for SpecifyPerformanceTopKs() is not set");
  SpecifyPerformanceTopKsCB(top_k);
}

void ActPhyDBTimingAPI::SpecifyPerformanceTopK(
    int performance_id,
    int top_k
) {
  PhyDBExpects(SpecifyPerformanceTopKCB != nullptr,
               "Callback function for SpecifyPerformanceTopK() is not set");
  SpecifyPerformanceTopKCB(performance_id, top_k);
}

double ActPhyDBTimingAPI::GetPerformanceConstraintWeight(int performance_id) {
  PhyDBExpects(GetPerformanceConstraintWeightCB != nullptr,
               "Callback function for GetPerformanceConstraintWeight() is not set");
  return GetPerformanceConstraintWeightCB(performance_id);
}

double ActPhyDBTimingAPI::GetPerformanceSlack(int performance_id) {
  PhyDBExpects(GetPerformanceSlackCB != nullptr,
               "Callback function for GetPerformanceSlack() is not set");
  return GetPerformanceSlackCB(performance_id);
}

void ActPhyDBTimingAPI::GetViolatedPerformanceConstraints(
    std::vector<int> &performance_ids
) {
  PhyDBExpects(GetViolatedPerformanceConstraintsCB != nullptr,
               "Callback function for GetViolatedPerformanceConstraints() is not set");
  GetViolatedPerformanceConstraintsCB(performance_ids);
}

void ActPhyDBTimingAPI::GetPerformanceWitness(
    int performance_id,
    PhydbPath &phydb_path
) {
  PhyDBExpects(GetPerformanceWitnessCB != nullptr,
               "Callback function for GetPerformanceWitness() is not set");
  std::vector<ActEdge> act_path;
  GetPerformanceWitnessCB(performance_id, act_path);
  TranslateActPathToPhydbPath(act_path, phydb_path);
}

void ActPhyDBTimingAPI::TranslateActPathToPhydbPath(
    std::vector<ActEdge> &act_path,
    PhydbPath &phydb_path
) {
  phydb_path.Clear();

  size_t sz = act_path.size();

  for (size_t i = 0; i < sz; ++i) {
    ActEdge &act_edge = act_path[i];
    if (!IsActComPinPtrExisting(act_edge.source)) {
      std::cout << "before getFullName4Pin() for source" << std::endl;
      std::string pin_name = adaptor_->getFullName4Pin(act_edge.source);
      PhyDBExpects(false,
                   "ActEdge source pin, " + pin_name
                       + " corresponds to no PhyDB pin");
    }
    if (!IsActComPinPtrExisting(act_edge.target)) {
      std::cout << "before getFullName4Pin() for target" << std::endl;
      std::string pin_name = adaptor_->getFullName4Pin(act_edge.target);
      PhyDBExpects(false,
                   "ActEdge target pin, " + pin_name
                       + " corresponds to no PhyDB pin");
    }
    if ((act_edge.net_ptr != nullptr)
        && !IsActNetPtrExisting(act_edge.net_ptr)) {
      std::cout << "before getFullName4Net()" << std::endl;
      std::string net_name = adaptor_->getFullName4Net(act_edge.net_ptr);
      PhyDBExpects(false,
                   "ActEdge net_ptr, " + net_name
                       + " corresponds to no PhyDB net");
    }
    PhyDBExpects(act_edge.delay >= 0, "Negative delay?");

    if (i == 0) {
      phydb_path.root = ActCompPinPtr2Id(act_edge.source);
    }

    PhydbPin source = ActCompPinPtr2Id(act_edge.source);
    PhydbPin target = ActCompPinPtr2Id(act_edge.target);
    int net_index = ActNetPtr2Id(act_edge.net_ptr);
    double delay = act_edge.delay;
    phydb_path.AddEdge(source, target, net_index, delay, 1);
  }
}
#endif

}
