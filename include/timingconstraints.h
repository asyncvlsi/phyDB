//
// Created by yihang on 4/18/21.
//

#ifndef PHYDB_INCLUDE_TIMINGCONSTRAINTS_H_
#define PHYDB_INCLUDE_TIMINGCONSTRAINTS_H_

#include <unordered_map>
#include <vector>

#include <boost/functional/hash.hpp>

// root, fast, slow (t_root: t_fast + margin < t_slow)

namespace phydb {

struct PhyDBEdge {
    std::pair<int, int> driv_pin;
    std::pair<int, int> load_pin;
    int net_index;
    double delay;
};

struct ActEdge {
    void *driv_pin; // (comp, pin) pair
    void *load_pin;
    void *net_ptr;
    double delay;
};

class ActPhyDBTimingAPI {
  public:
    bool IsActNetPtrExisting(void *act_net);
    int ActNetPtr2Id(void *act_net);
    void AddActNetPtrIdPair(void *act_net, int net_id);

    bool IsActComPinPtrExisting(void *act_comp);
    std::pair<int, int> ActCompPinPtr2Id(void *act_comp);
    void AddActCompPinPtrIdPair(void *act_comp, int comp_id, int pin_id);

    void SetGetNumConstraintsCB(int (*callback_function)()) {
        GetNumConstraintsCB = callback_function;
    }
    void SetGetRequiredMarginCB(double (*callback_function)(int)) {
        GetRequiredMarginCB = callback_function;
    }
    void SetGetActualMarginCB(double (*callback_function)(int)) {
        GetActualMarginCB = callback_function;
    }
    void SetGetWitnessCB(void (*callback_function)(int, std::vector<ActEdge> &, std::vector<ActEdge> &)) {
        GetWitnessCB = callback_function;
    }
    void SetGetViolatedTimingConstraintsCB(void (*callback_function)(std::vector<int> &)) {
        GetViolatedTimingConstraintsCB = callback_function;
    }

    int GetNumConstraints();
    void UpdateTimingIncremental();
    double GetRequiredMargin(int tc_num);
    double GetActualMargin(int tc_num);
    void GetWitness(int tc_num, std::vector<PhyDBEdge> &phydb_fast_path, std::vector<PhyDBEdge> &phydb_slow_path);
    void GetViolatedTimingConstraints(std::vector<int> &violated_tc_nums);

  private:
    int (*GetNumConstraintsCB)() = nullptr;
    void (*UpdateTimingIncrementalCB)() = nullptr;
    double (*GetRequiredMarginCB)(int) = nullptr;
    double (*GetActualMarginCB)(int) = nullptr;
    void (*GetWitnessCB)(int, std::vector<ActEdge> &, std::vector<ActEdge> &) = nullptr;
    void (*GetViolatedTimingConstraintsCB)(std::vector<int> &) = nullptr;

    // act net pointer <=> phydb net index
    std::unordered_map<void *, int> net_act_2_id_;
    std::unordered_map<int, void *> net_id_2_act_;

    // act component-pin pointer <=> phydb component-pin index
    std::unordered_map<void *, std::pair<int, int>> component_pin_act_2_id_;
    std::unordered_map<std::pair<int, int>, void *, boost::hash<std::pair<int, int>>> component_pin_id_2_act_;

    void Translate(std::vector<ActEdge> &act_path, std::vector<PhyDBEdge> &phydb_path);
};

}

#endif //PHYDB_INCLUDE_TIMINGCONSTRAINTS_H_
