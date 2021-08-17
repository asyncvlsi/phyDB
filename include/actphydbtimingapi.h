//
// Created by yihang on 4/18/21.
//

#ifndef PHYDB_INCLUDE_ACTPHYDBTIMINGAPI_H_
#define PHYDB_INCLUDE_ACTPHYDBTIMINGAPI_H_

#include <unordered_map>
#include <vector>

#include <boost/functional/hash.hpp>

#include "galois/eda/parasitics/Manager.h"
#include "galois/eda/liberty/CellLib.h"
#include "galois/eda/utility/ExtNetlistAdaptor.h"

namespace phydb {

struct PhydbPin {
    explicit PhydbPin(int comp = -1, int pin = -1) : comp_id(comp), pin_id(pin) {}
    int comp_id;
    int pin_id;

    bool operator==(PhydbPin const &rhs) const {
        return (comp_id == rhs.comp_id) && (pin_id == rhs.pin_id);
    }

    std::string Str() const {
        return "(" + std::to_string(comp_id) + ", " + std::to_string(pin_id) + ")";
    }

    void Reset() {
        comp_id = -1;
        pin_id = -1;
    }
};

struct PhydbPinHasher {
    std::size_t operator()(const PhydbPin &phydb_pin) const {
        std::size_t seed = 0;
        boost::hash_combine(seed, boost::hash_value(phydb_pin.comp_id));
        boost::hash_combine(seed, boost::hash_value(phydb_pin.pin_id));
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

// TODO : is arc delay independent of load net SPEF? If yes, no need to store arcs using PhydbTimingEdge
struct ForkConstraint {
    PhydbPin root;
    PhydbPin stb_slow; // supposed to be slow
    std::vector<PhydbPin> stb_fast; // supposed to be fast

    // TODO: for different root sign and stb_slow sign combinations, put them in one or multiple DAGs?
    TimingDAG timing_dag;
};

struct ActEdge {
    void *source = nullptr;
    void *target = nullptr;
    void *net_ptr = nullptr;
    double delay = -1;
};

class ActPhyDBTimingAPI {
  public:
    //APIs for ACT
    void AddActNetPtrIdPair(void *act_net, int net_id);
    void AddActCompPinPtrIdPair(void *act_comp, int comp_id, int pin_id);

    void SetGetNumConstraintsCB(int (*callback_function)());
    void SetUpdateTimingIncrementalCB(void (*callback_function)());
    void SetGetSlackCB(double (*callback_function)(int));
    void SetGetBatchSlackCB(std::vector<double> (*callback_function)(std::vector<int> ));
    void SetGetWitnessCB(void (*callback_function)(int, std::vector<ActEdge> &, std::vector<ActEdge> &));
    void SetGetViolatedTimingConstraintsCB(void (*callback_function)(std::vector<int> &));

    void SetParaManager(galois::eda::parasitics::Manager* manager);
    void AddCellLib(galois::eda::liberty::CellLib* lib);
    void SetNetlistAdaptor(galois::eda::utility::ExtNetlistAdaptor* adaptor);

    galois::eda::parasitics::Manager* GetParaManager();
    std::vector<galois::eda::liberty::CellLib*> GetCelllibs();
    galois::eda::utility::ExtNetlistAdaptor* GetNetlistAdaptor();

    //APIs for Dali and SPRoute
    bool IsActNetPtrExisting(void *act_net);
    int ActNetPtr2Id(void *act_net);
    bool IsActComPinPtrExisting(void *act_comp);
    PhydbPin ActCompPinPtr2Id(void *act_comp);
    int GetNumConstraints();
    void UpdateTimingIncremental();
    double GetSlack(int tc_num);
    void GetWitness(int tc_num, PhydbPath &phydb_fast_path, PhydbPath &phydb_slow_path);
    void GetViolatedTimingConstraints(std::vector<int> &violated_tc_nums);

  private:
    int (*GetNumConstraintsCB)() = nullptr;
    void (*UpdateTimingIncrementalCB)() = nullptr;
    double (*GetSlackCB)(int) = nullptr;
    std::vector<double> (*GetBatchSlackCB)(std::vector<int>) = nullptr;
    void (*GetWitnessCB)(int, std::vector<ActEdge> &, std::vector<ActEdge> &) = nullptr;
    void (*GetViolatedTimingConstraintsCB)(std::vector<int> &) = nullptr;

    // act net pointer <=> phydb net index
    std::unordered_map<void *, int> net_act_2_id_;
    std::unordered_map<int, void *> net_id_2_act_;

    // act component-pin pointer <=> phydb component-pin index
    std::unordered_map<void *, PhydbPin> component_pin_act_2_id_;
    std::unordered_map<PhydbPin, void *, PhydbPinHasher> component_pin_id_2_act_;

    galois::eda::parasitics::Manager* para_manager_; //include galois/eda/parasitics/manager.h
    std::vector<galois::eda::liberty::CellLib*> libs_; //include galois/eda/liberty/CellLib.h
    galois::eda::utility::ExtNetlistAdaptor* adaptor_; //include galois/eda/utility/ExtNetlistAdaptor.h

    void Translate(std::vector<ActEdge> &act_path, PhydbPath &phydb_path);
};

}

#endif //PHYDB_INCLUDE_ACTPHYDBTIMINGAPI_H_
