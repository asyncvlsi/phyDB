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
#ifndef PHYDB_TIMING_ACTPHYDBTIMINGAPI_H_
#define PHYDB_TIMING_ACTPHYDBTIMINGAPI_H_

#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <vector>

#include "config.h"

#if PHYDB_USE_GALOIS
#include <galois/eda/model/DelayLib.h>
#include <galois/eda/parasitics/Edge.h>
#include <galois/eda/parasitics/Manager.h>
#include <galois/eda/parasitics/Node.h>
#include <galois/eda/utility/ExtNetlistAdaptor.h>
#endif

namespace phydb {

/*
 * This is a data structure to represent pins in a net.
 * There are two kind of pins:
 *   1. component pin, this kind of pin is the input/output of a component;
 *   2. I/O pin, this kind of pin is the input/output of the whole design,
 *   which belongs to an I/O pad.
 *
 * This class contains two attributes:
 *   1. instance_id_, the id of a component;
 *   2. pin_id_, the id of the component pin in its macro definition.
 *
 * To represent an I/O pin, the value of instance_id_ will be -1, and the value
 * of pin_id_ is the id of this I/O pin.
 */
class PhydbPin {
 public:
  explicit PhydbPin(int comp = -1, int pin = -1)
      : instance_id_(comp), pin_id_(pin) {}

  bool operator==(PhydbPin const &rhs) const {
    return (instance_id_ == rhs.instance_id_) && (pin_id_ == rhs.pin_id_);
  }

  bool operator!=(PhydbPin const &rhs) const {
    return (instance_id_ != rhs.instance_id_) || (pin_id_ != rhs.pin_id_);
  }

  // positive instance id means a component; instance id -1 means IOPIN
  int InstanceId() const { return instance_id_; }
  // id of the pin in a MACRO or the IOPIN list
  int PinId() const { return pin_id_; }

  bool IsValid() { return !((instance_id_ < -1) || (pin_id_ < 0)); }

  bool IsComponentPin() { return instance_id_ >= 0; }

  void Reset() {
    instance_id_ = -1;
    pin_id_ = -1;
  }
  friend std::ostream &operator<<(std::ostream &ost, const PhydbPin &pin);

 private:
  int instance_id_;
  int pin_id_;
};

struct PhydbPinHasher {
  std::size_t operator()(const PhydbPin &phydb_pin) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, boost::hash_value(phydb_pin.InstanceId()));
    boost::hash_combine(seed, boost::hash_value(phydb_pin.PinId()));
    return seed;
  }
};

struct PhydbTimingEdge {
  explicit PhydbTimingEdge(PhydbPin &tgt) : target(tgt) {}
  PhydbPin target;
  int net_index = -1;
  double delay = -1;
  int count = 1;
};

struct PhydbTimingNode {
  explicit PhydbTimingNode(PhydbPin &pin) : source(pin) {}
  PhydbPin source;
  std::unordered_map<PhydbPin, int, PhydbPinHasher> out_pin2index_;
  std::vector<PhydbTimingEdge> out_edges;

  bool IsTargetPinExisting(PhydbPin &pin) const {
    return out_pin2index_.find(pin) != out_pin2index_.end();
  }
  PhydbTimingEdge *AddPinToOutEdges(PhydbPin &pin);
  PhydbTimingEdge *GetOutEdge(PhydbPin &pin);
  void AddEdge(PhydbTimingEdge &edge);
};

struct PhydbPath {
  PhydbPin root;
  std::vector<PhydbTimingEdge> edges;
  void Clear();
  void AddEdge(PhydbPin &src, PhydbPin &tgt, int net_id, double dly, int cnt);
};

struct TimingDAG {
  // bool root_sign;
  // bool stb_slow_sign;
  // bool stb_fast_sign... we may or may not need this

  // this is likely to be a linked-list, and we may not need to store this
  std::vector<PhydbTimingNode> stb_slow_nodes;
  std::unordered_map<PhydbPin, int, PhydbPinHasher> fast_pin2index_;
  std::vector<PhydbTimingNode> stb_fast_nodes;

  bool IsPinInDag(PhydbPin &pin) const {
    return fast_pin2index_.find(pin) != fast_pin2index_.end();
  }
  PhydbTimingNode *AddPinToDag(PhydbPin &pin);
  PhydbTimingNode *GetPinNode(PhydbPin &pin);
  void AddFastPath(PhydbPath &fast_path);
};

// TODO : is arc delay independent of load net SPEF? If yes, no need to store
// arcs using PhydbTimingEdge
struct ForkConstraint {
  PhydbPin root;
  PhydbPin stb_slow;               // supposed to be slow
  std::vector<PhydbPin> stb_fast;  // supposed to be fast

  // TODO: for different root sign and stb_slow sign combinations, put them in
  // one or multiple DAGs?
  TimingDAG timing_dag;
};

struct ActEdge {
  void *source = nullptr;
  void *target = nullptr;
  void *net_ptr = nullptr;
  double delay = -1;
};

class ActPhyDBTimingAPI {
  friend class PhyDB;

 public:
  // APIs for ACT
  void AddActNetPtrIdPair(void *act_net, int net_id);
  void BindActPinAndPhydbPin(void *act_pin, PhydbPin phydb_pin);

  void SetGetNumConstraintsCB(int (*callback_function)());
  void SetSpecifyTopKsCB(void (*callback_function)(int));
  void SetSpecifyTopKCB(void (*callback_function)(int, int));
  void SetUpdateTimingIncrementalCB(void (*callback_function)());
  void SetGetSlackCB(
      std::vector<double> (*callback_function)(const std::vector<int> &));
  void SetGetViolatedTimingConstraintsCB(
      void (*callback_function)(std::vector<int> &));
  void SetGetSlowWitnessCB(void (*callback_function)(
      int timing_constraint_id, std::vector<ActEdge> &path));
  void SetGetFastWitnessCB(void (*callback_function)(
      int timing_constraint_id, std::vector<ActEdge> &path));
  void SetGetNumPerformanceConstraintsCB(int (*callback_function)());
  void SetSpecifyPerformanceTopKsCB(void (*callback_function)(int top_k));
  void SetSpecifyPerformanceTopKCB(void (*callback_function)(int performance_id,
                                                             int top_k));
  void SetGetPerformanceConstraintWeightCB(
      double (*callback_function)(int performance_id));
  void SetGetPerformanceSlackCB(
      double (*callback_function)(int performance_id));
  void SetGetViolatedPerformanceConstraintsCB(
      void (*callback_function)(std::vector<int> &));
  void SetGetPerformanceWitnessCB(void (*callback_function)(
      int performance_id, std::vector<ActEdge> &path));

  // APIs for Dali and SPRoute
  bool ReadyForTimingDriven();
  bool IsActNetPtrExisting(void *act_net);
  int ActNetPtr2Id(void *act_net);
  void *PhydbNetId2ActPtr(int net_id);
  bool IsActComPinPtrExisting(void *act_pin);
  PhydbPin ActCompPinPtr2Id(void *act_pin);
  void *PhydbCompPin2ActPtr(PhydbPin phydb_pin);
  int GetNumConstraints();
  void SpecifyTopKs(int k);
  void SpecifyTopK(int tc_num, int k);
  void UpdateTimingIncremental();
  double GetSlack(int tc_num);
  void GetViolatedTimingConstraints(std::vector<int> &violated_tc_nums);

#if PHYDB_USE_GALOIS
  void SetParaManager(galois::eda::parasitics::Manager *manager);
  void AddCellLib(galois::eda::model::CellLib *lib);
  void SetNetlistAdaptor(galois::eda::utility::ExtNetlistAdaptor *adaptor);

  galois::eda::parasitics::Manager *GetParaManager();
  std::vector<galois::eda::model::CellLib *> &GetCellLibs();
  galois::eda::utility::ExtNetlistAdaptor *GetNetlistAdaptor();

  galois::eda::parasitics::Node *PhyDBPinToSpefNode(PhydbPin phydb_pin);

  void GetWitness(int tc_num, PhydbPath &phydb_fast_path,
                  PhydbPath &phydb_slow_path);
  void GetSlowWitness(int tc_num, PhydbPath &phydb_path);
  void GetFastWitness(int tc_num, PhydbPath &phydb_path);
  int GetNumPerformanceConstraints();
  void SpecifyPerformanceTopKs(int top_k);
  void SpecifyPerformanceTopK(int performance_id, int top_k);
  double GetPerformanceConstraintWeight(int performance_id);
  double GetPerformanceSlack(int performance_id);
  void GetViolatedPerformanceConstraints(std::vector<int> &performance_ids);
  void GetPerformanceWitness(int performance_id, PhydbPath &phydb_path);
#endif
 private:
  int (*GetNumConstraintsCB)() = nullptr;
  void (*SpecifyTopKsCB)(int) = nullptr;
  void (*SpecifyTopKCB)(int, int) = nullptr;
  void (*UpdateTimingIncrementalCB)() = nullptr;
  std::vector<double> (*GetSlackCB)(const std::vector<int> &) = nullptr;
  void (*GetViolatedTimingConstraintsCB)(std::vector<int> &) = nullptr;
  void (*GetSlowWitnessCB)(int timing_constraint_id,
                           std::vector<ActEdge> &path) = nullptr;
  void (*GetFastWitnessCB)(int timing_constraint_id,
                           std::vector<ActEdge> &path) = nullptr;
  int (*GetNumPerformanceConstraintsCB)() = nullptr;
  void (*SpecifyPerformanceTopKsCB)(int top_k) = nullptr;
  void (*SpecifyPerformanceTopKCB)(int performance_id, int top_k) = nullptr;
  double (*GetPerformanceConstraintWeightCB)(int performance_id) = nullptr;
  double (*GetPerformanceSlackCB)(int performance_id) = nullptr;
  void (*GetViolatedPerformanceConstraintsCB)(
      std::vector<int> &performance_ids) = nullptr;
  void (*GetPerformanceWitnessCB)(int performance_id,
                                  std::vector<ActEdge> &path) = nullptr;

  // act net pointer <=> phydb net index
  std::unordered_map<void *, int> net_act_2_id_;
  std::unordered_map<int, void *> net_id_2_act_;

  // act component-pin pointer <=> phydb component-pin index
  std::unordered_map<void *, PhydbPin> component_pin_act_2_id_;
  std::unordered_map<PhydbPin, void *, PhydbPinHasher> component_pin_id_2_act_;
#if PHYDB_USE_GALOIS
  galois::eda::parasitics::Manager *para_manager_;
  std::vector<galois::eda::model::CellLib *> libs_;
  galois::eda::utility::ExtNetlistAdaptor *adaptor_;
#endif

  void TranslateActPathToPhydbPath(std::vector<ActEdge> &act_path,
                                   PhydbPath &phydb_path);
};

}  // namespace phydb

#endif  // PHYDB_TIMING_ACTPHYDBTIMINGAPI_H_
