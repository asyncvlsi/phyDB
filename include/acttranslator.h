//
// Created by Yihang Yang on 4/13/21.
//

#ifndef PHYDB_INCLUDE_ACTTRANSLATOR_H_
#define PHYDB_INCLUDE_ACTTRANSLATOR_H_

#include <unordered_map>

#include <boost/functional/hash.hpp>

namespace phydb {

struct ActTranslator {
    ActTranslator() {}
    // act macro pointer <=> phydb macro index
    std::unordered_map<void *, int> macro_act_2_id_;
    std::unordered_map<int, void *> macro_id_2_act_;

    // act macro-pin pointer <=> phydb macro-pin index
    std::unordered_map<void *, std::pair<int, int>> macro_pin_act_2_id_;
    std::unordered_map<std::pair<int, int>, void *, boost::hash<pair<int, int>>> macro_pin_id_2_act_;

    // act component pointer <=> phydb component index
    std::unordered_map<void *, int> component_act_2_id_;
    std::unordered_map<int, void *> component_id_2_act_;

    // act net pointer <=> phydb net index
    std::unordered_map<void *, int> net_act_2_id_;
    std::unordered_map<int, void *> net_id_2_act_;

    // act component-pin pointer <=> phydb component-pin index
    std::unordered_map<void *, std::pair<int, int>> component_pin_act_2_id_;
    std::unordered_map<std::pair<int, int>, void *, boost::hash<pair<int, int>>> component_pin_id_2_act_;
};

}

#endif //PHYDB_INCLUDE_ACTTRANSLATOR_H_
