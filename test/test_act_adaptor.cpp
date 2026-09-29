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
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

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

  std::string getFullName4Pin(void *const pin) const override {
    return pin == pseudo_pin ? "kernel.cx0:pi$1" : "driver:Y";
  }
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
  void *pseudo_pin = nullptr;  // named as the timer's primary-input pin
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

bool GetConstraintEndpoints(int constraint_id, phydb::PhydbPin &root,
                            phydb::PhydbPin &fast_terminal,
                            phydb::PhydbPin &slow_terminal) {
  if (constraint_id != 7) return false;
  root = phydb::PhydbPin(1, 2);
  fast_terminal = phydb::PhydbPin(3, 4);
  slow_terminal = phydb::PhydbPin(5, 6);
  return true;
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

  if (db.GetTimingApi().PhydbPin2NetId(phydb::PhydbPin(-1, 0)) != 0) {
    fprintf(stderr, "the net's I/O pin was not bound to it\n");
    return false;
  }
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

bool CheckConstraintEndpointCallback() {
  phydb::ActPhyDBTimingAPI timing_api;
  phydb::PhydbPin root;
  phydb::PhydbPin fast_terminal;
  phydb::PhydbPin slow_terminal;
  if (timing_api.GetConstraintEndpoints(7, root, fast_terminal,
                                        slow_terminal)) {
    fprintf(stderr, "constraint endpoints succeeded without a callback\n");
    return false;
  }
  timing_api.SetGetConstraintEndpointsCB(GetConstraintEndpoints);
  if (!timing_api.GetConstraintEndpoints(7, root, fast_terminal,
                                         slow_terminal) ||
      root != phydb::PhydbPin(1, 2) ||
      fast_terminal != phydb::PhydbPin(3, 4) ||
      slow_terminal != phydb::PhydbPin(5, 6) ||
      timing_api.GetConstraintEndpoints(8, root, fast_terminal,
                                        slow_terminal)) {
    fprintf(stderr, "constraint endpoint callback did not preserve values\n");
    return false;
  }
  return true;
}

bool IsForkVacuous(int constraint_id) { return constraint_id == 3; }

bool CheckForkVacuousCallback() {
  phydb::ActPhyDBTimingAPI timing_api;
  // Hosts that never register the callback report no vacuous forks.
  if (timing_api.IsForkVacuous(3)) {
    fprintf(stderr, "fork reported vacuous without a callback\n");
    return false;
  }
  timing_api.SetIsForkVacuousCB(IsForkVacuous);
  if (!timing_api.IsForkVacuous(3) || timing_api.IsForkVacuous(4)) {
    fprintf(stderr, "vacuous-fork callback did not preserve values\n");
    return false;
  }
  return true;
}

int primary_input_pin;  // a timer `pi$N` pseudo-pin: bound to nothing
int load_pin;
int input_net;

void GetPrimaryInputWitness(int, std::vector<phydb::ActEdge> &path) {
  path.push_back({&primary_input_pin, &load_pin, &input_net, 4.0});
}

// Runs `body` in a child and reports whether it aborted.
bool Aborts(const std::function<void()> &body) {
  pid_t child = fork();
  if (child == 0) {
    freopen("/dev/null", "w", stderr);
    freopen("/dev/null", "w", stdout);
    body();
    _exit(0);
  }
  int status = 0;
  waitpid(child, &status, 0);
  return !(WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

// A witness that starts at a primary input translates, with the one I/O pin
// on the load's physical net standing in for the pseudo-pin. The witness's ACT
// net deliberately maps to another net with its own I/O pin, as colliding
// flattened names did on asymmetric_fork_join_io: the physical net must win.
// Binding the same I/O pin twice changes nothing. Without an I/O pin, or for
// an unbound pin that is not a pseudo-pin, translation still aborts.
bool CheckPrimaryInputMapsToIoPin() {
  constexpr int net_id = 5;
  constexpr int colliding_net_id = 9;
  const phydb::PhydbPin io_pin(-1, 2);
  const phydb::PhydbPin colliding_io_pin(-1, 0);
  const phydb::PhydbPin load(7, 1);
  auto build = [&](phydb::ActPhyDBTimingAPI &timing_api,
                   TestNetlistAdaptor &adaptor, bool with_io_pin) {
    timing_api.SetNetlistAdaptor(&adaptor);
    timing_api.BindActPinAndPhydbPin(&load_pin, load);
    timing_api.BindPhydbPinToNet(load, net_id);
    if (with_io_pin) {
      timing_api.BindIoPinToNet(io_pin, net_id);
      timing_api.BindIoPinToNet(io_pin, net_id);
    }
    timing_api.BindIoPinToNet(colliding_io_pin, colliding_net_id);
    timing_api.AddActNetPtrIdPair(&input_net, colliding_net_id);
    timing_api.SetGetSlowWitnessCB(GetPrimaryInputWitness);
  };
  {
    TestNetlistAdaptor adaptor;
    adaptor.pseudo_pin = &primary_input_pin;
    phydb::ActPhyDBTimingAPI timing_api;
    build(timing_api, adaptor, true);
    phydb::PhydbPath path;
    timing_api.GetSlowWitness(0, path);
    if (path.root != io_pin || path.edges.size() != 1 ||
        path.edges.front().target != load ||
        path.edges.front().net_index != net_id) {
      fprintf(stderr, "primary-input witness did not map to its I/O pin\n");
      return false;
    }
  }
  if (!Aborts([&] {
        TestNetlistAdaptor adaptor;
        adaptor.pseudo_pin = &primary_input_pin;
        phydb::ActPhyDBTimingAPI timing_api;
        build(timing_api, adaptor, false);
        phydb::PhydbPath path;
        timing_api.GetSlowWitness(0, path);
      })) {
    fprintf(stderr, "a primary input with no I/O pin was translated\n");
    return false;
  }
  if (!Aborts([&] {
        TestNetlistAdaptor adaptor;  // names the unbound pin "driver:Y"
        phydb::ActPhyDBTimingAPI timing_api;
        build(timing_api, adaptor, true);
        phydb::PhydbPath path;
        timing_api.GetSlowWitness(0, path);
      })) {
    fprintf(stderr, "an unbound ordinary pin was given an I/O pin\n");
    return false;
  }
  return true;
}

// Runs CreatePhydbActAdaptor on a one-net design and returns what it wrote
// to stderr.
std::string AdaptorTrace(bool adaptor_debug) {
  phydb::PhyDB db;
  phydb::Macro *macro = db.AddMacro("CELL");
  macro->AddPin("Y", phydb::SignalDirection::OUTPUT, phydb::SignalUse::SIGNAL);
  db.AddComponent("driver", macro, phydb::PlaceStatus::PLACED, 0, 0,
                  phydb::CompOrient::N);
  db.AddNet("out");
  db.AddCompPinToNet("driver", "Y", "out");
  TestNetlistAdaptor adaptor;
  db.SetNetlistAdaptor(&adaptor);
  db.SetAdaptorDebug(adaptor_debug);

  char path[] = "/tmp/phydb_adaptor_trace.XXXXXX";
  int fd = mkstemp(path);
  if (fd < 0) return "<mkstemp failed>";
  std::cerr.flush();
  fflush(stderr);
  int saved = dup(STDERR_FILENO);
  dup2(fd, STDERR_FILENO);
  db.CreatePhydbActAdaptor(false);
  std::cerr.flush();
  fflush(stderr);
  dup2(saved, STDERR_FILENO);
  close(saved);
  close(fd);
  std::ifstream in(path);
  std::stringstream text;
  text << in.rdbuf();
  unlink(path);
  return text.str();
}

// Tracing is controlled only by SetAdaptorDebug: off by default, on when set,
// whatever the environment holds.
bool CheckAdaptorDebugIsExplicit() {
  phydb::PhyDB db;
  if (db.IsAdaptorDebug()) {
    fprintf(stderr, "adaptor debug is on by default\n");
    return false;
  }
  setenv("PHYDB_ADAPTOR_DEBUG", "1", 1);
  std::string off = AdaptorTrace(false);
  unsetenv("PHYDB_ADAPTOR_DEBUG");
  std::string on = AdaptorTrace(true);
  if (off.find("[adaptor]") != std::string::npos) {
    fprintf(stderr, "adaptor traced with debug off: %s\n", off.c_str());
    return false;
  }
  if (on.find("[adaptor] net 0/1 out") == std::string::npos) {
    fprintf(stderr, "adaptor did not trace with debug on: '%s'\n",
            on.c_str());
    return false;
  }
  return true;
}

/*
 * A net the timer cannot bind is skipped: its wire RC never reaches timing.
 * That must be visible in one summary line, even with the trace off. The
 * fake adaptor knows no net, so "out" is unbound.
 */
bool CheckUnboundNetsAreSummarized() {
  const std::string text = AdaptorTrace(false);
  if (text.find("PhyDB: 1 net not bound to the timer") == std::string::npos ||
      text.find(" out") == std::string::npos) {
    fprintf(stderr, "unbound net not summarized: '%s'\n", text.c_str());
    return false;
  }
  return true;
}

} // namespace

int main() {
  return CheckOptionalIoNetPinBinding() && CheckPhysicalEndpointNetIdentity() &&
                 CheckConstraintEndpointCallback() &&
                 CheckForkVacuousCallback() && CheckAdaptorDebugIsExplicit() &&
                 CheckPrimaryInputMapsToIoPin() && CheckUnboundNetsAreSummarized()
             ? 0
             : 1;
}
