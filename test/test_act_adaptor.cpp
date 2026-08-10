/*************************************************************************
 *
 *  Copyright (c) 2026
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *************************************************************************
 */
#include <cstdio>
#include <string>
#include <vector>

#include <galois/eda/utility/ExtNetlistAdaptor.h>

#include "phydb/phydb.h"

class TestNetlistAdaptor : public galois::eda::utility::ExtNetlistAdaptor {
public:
  void *getPinFromFullName(const std::string &name, char, char, char,
                           char) const override {
    last_pin_name_ = name;
    return name == "driver:Y" ? const_cast<int *>(&driver_pin_) : nullptr;
  }

  void *getInstFromFullName(const std::string &, char, char,
                            char) const override {
    return nullptr;
  }

  void *getNetFromFullName(const std::string &, char, char,
                           char) const override {
    return nullptr;
  }

  std::string getFullName4Pin(void *const) const override { return "driver:Y"; }
  std::string getFullName4Inst(void *const) const override { return "driver"; }
  std::string getFullName4Net(void *const) const override { return "out"; }
  void *getInst4Pin(void *const) const override { return nullptr; }
  std::string getInstTypeName4Inst(void *const) const override {
    return "CELL";
  }
  std::string getPinNameInInst4Pin(void *const) const override { return "Y"; }
  void *getNet4Pin(void *const) const override { return nullptr; }
  bool isPin1LessThanPin2(void *const a, void *const b) const override {
    return a < b;
  }
  bool isNet1LessThanNet2(void *const a, void *const b) const override {
    return a < b;
  }
  bool isInst1LessThanInst2(void *const a, void *const b) const override {
    return a < b;
  }
  bool isSameNet(void *const a, void *const b) const override { return a == b; }
  bool isSameInst(void *const a, void *const b) const override {
    return a == b;
  }
  bool isSamePin(void *const a, void *const b) const override { return a == b; }

  void *driver_pin() { return &driver_pin_; }
  const std::string &last_pin_name() const { return last_pin_name_; }

private:
  int driver_pin_ = 0;
  mutable std::string last_pin_name_;
};

namespace {

int source_pin;
int target_pin;
int timing_net;
int unmapped_timing_net;
void *witness_net = &timing_net;

void GetWitness(int, std::vector<phydb::ActEdge> &path) {
  path.push_back({&source_pin, &target_pin, witness_net, 12.5});
}

bool CheckOptionalIoNetPinBinding() {
  phydb::PhyDB db;
  phydb::Macro *macro = db.AddMacro("CELL");
  macro->AddPin("Y", phydb::SignalDirection::OUTPUT, phydb::SignalUse::SIGNAL);
  db.AddComponent("driver", macro, phydb::PlaceStatus::PLACED, 0, 0,
                  phydb::CompOrient::N);
  db.AddIoPin("out", phydb::SignalDirection::OUTPUT, phydb::SignalUse::SIGNAL);
  db.AddNet("out");
  db.AddCompPinToNet("driver", "Y", "out");
  db.AddIoPinToNet("out", "out");

  TestNetlistAdaptor adaptor;
  db.SetNetlistAdaptor(&adaptor);
  db.CreatePhydbActAdaptor(false);

  if (!db.GetTimingApi().IsActComPinPtrExisting(adaptor.driver_pin())) {
    fprintf(stderr,
            "component endpoint on optional I/O net was not bound; pins=%zu "
            "iopins=%zu lookup=%s\n",
            db.GetNetPtr("out")->GetPinsRef().size(),
            db.GetNetPtr("out")->GetIoPinIdsRef().size(),
            adaptor.last_pin_name().c_str());
    return false;
  }

  return true;
}

bool CheckPhysicalEndpointNetIdentity() {
  phydb::ActPhyDBTimingAPI timing_api;
  phydb::PhydbPin source(3, 4);
  phydb::PhydbPin target(5, 6);
  constexpr int physical_net_id = 17;
  constexpr int colliding_timing_net_id = 23;

  timing_api.BindActPinAndPhydbPin(&source_pin, source);
  timing_api.BindActPinAndPhydbPin(&target_pin, target);
  timing_api.BindPhydbPinToNet(source, physical_net_id);
  timing_api.BindPhydbPinToNet(target, physical_net_id);
  timing_api.AddActNetPtrIdPair(&timing_net, colliding_timing_net_id);
  timing_api.SetGetSlowWitnessCB(GetWitness);

  phydb::PhydbPath path;
  timing_api.GetSlowWitness(0, path);
  if (path.edges.size() != 1 ||
      path.edges.front().net_index != physical_net_id) {
    fprintf(stderr,
            "witness edge used timing-net id instead of physical endpoint net; "
            "edges=%zu net=%d expected=%d\n",
            path.edges.size(),
            path.edges.empty() ? -1 : path.edges.front().net_index,
            physical_net_id);
    return false;
  }

  witness_net = &unmapped_timing_net;
  timing_api.GetSlowWitness(0, path);
  if (path.edges.size() != 1 ||
      path.edges.front().net_index != physical_net_id) {
    fprintf(stderr,
            "unmapped timing net did not recover physical endpoint net; "
            "edges=%zu net=%d expected=%d\n",
            path.edges.size(),
            path.edges.empty() ? -1 : path.edges.front().net_index,
            physical_net_id);
    return false;
  }
  return true;
}

} // namespace

int main() {
  return CheckOptionalIoNetPinBinding() && CheckPhysicalEndpointNetIdentity()
             ? 0
             : 1;
}
