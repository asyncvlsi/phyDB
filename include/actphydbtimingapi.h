//
// Created by yihang on 4/18/21.
//

#ifndef PHYDB_INCLUDE_ACTPHYDBTIMINGAPI_H_
#define PHYDB_INCLUDE_ACTPHYDBTIMINGAPI_H_

#include <unordered_map>
#include <vector>

#include <boost/functional/hash.hpp>

namespace phydb {

struct PhydbPin {
    explicit PhydbPin(int comp=-1, int pin=-1) : comp_id(comp), pin_id(pin) {}
    int comp_id;
    int pin_id;

    bool operator==(PhydbPin const& rhs) const {
        return (comp_id == rhs.comp_id) && (pin_id == rhs.pin_id);
    }
};

struct PhydbPinHasher {
    std::size_t operator () (const PhydbPin &phydb_pin) const {
        std::size_t seed = 0;
        boost::hash_combine(seed, boost::hash_value(phydb_pin.comp_id));
        boost::hash_combine(seed, boost::hash_value(phydb_pin.pin_id));
        return seed;
    }
};

struct PhydbEdge {
    PhydbPin source; // (comp, pin) pair
    PhydbPin target;
    int net_index = -1;
    double delay = -1;
};

struct PathBundle {
    // bool root_sign;
    // bool d_sign;
    // bool ack_sign;
    std::vector<PhydbEdge> a_path;
    std::vector<std::vector<PhydbEdge>> d_paths;
};

struct ForkConstraint {
    PhydbPin root;
    std::vector<PhydbPin> d;
    PhydbPin ack;

    std::vector<PathBundle> paths;
};

struct ActEdge {
    void *source = nullptr;
    void *target = nullptr;
    void *net_ptr = nullptr;
    double delay = -1;
};

class ActPhyDBTimingAPI {
  public:
    bool IsActNetPtrExisting(void *act_net);
    int ActNetPtr2Id(void *act_net);
    void AddActNetPtrIdPair(void *act_net, int net_id);
    bool IsActComPinPtrExisting(void *act_comp);
    PhydbPin ActCompPinPtr2Id(void *act_comp);
    void AddActCompPinPtrIdPair(void *act_comp, int comp_id, int pin_id);

    void SetGetNumConstraintsCB(int (*callback_function)());
    void SetUpdateTimingIncrementalCB(void (*callback_function)());
    void SetGetSlackCB(double (*callback_function)(int));
    void SetGetWitnessCB(void (*callback_function)(int, std::vector<ActEdge> &, std::vector<ActEdge> &));
    void SetGetViolatedTimingConstraintsCB(void (*callback_function)(std::vector<int> &));

    int GetNumConstraints();
    void UpdateTimingIncremental();
    double GetSlack(int tc_num);
    void GetWitness(int tc_num, std::vector<PhydbEdge> &phydb_fast_path, std::vector<PhydbEdge> &phydb_slow_path);
    void GetViolatedTimingConstraints(std::vector<int> &violated_tc_nums);

  private:
    int (*GetNumConstraintsCB)() = nullptr;
    void (*UpdateTimingIncrementalCB)() = nullptr;
    double (*GetActualMarginCB)(int) = nullptr;
    void (*GetWitnessCB)(int, std::vector<ActEdge> &, std::vector<ActEdge> &) = nullptr;
    void (*GetViolatedTimingConstraintsCB)(std::vector<int> &) = nullptr;

    // act net pointer <=> phydb net index
    std::unordered_map<void *, int> net_act_2_id_;
    std::unordered_map<int, void *> net_id_2_act_;

    // act component-pin pointer <=> phydb component-pin index
    std::unordered_map<void *, PhydbPin> component_pin_act_2_id_;
    std::unordered_map<PhydbPin, void *, PhydbPinHasher> component_pin_id_2_act_;

    void Translate(std::vector<ActEdge> &act_path, std::vector<PhydbEdge> &phydb_path);
};

}

#endif //PHYDB_INCLUDE_ACTPHYDBTIMINGAPI_H_
