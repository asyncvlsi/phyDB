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
#include "phydb.h"

#include <cmath>

#include <fstream>

#include "defwriter.h"
#include "phydb/common/helper.h"
#include "phydb/timing/techconfigparser.h"
#include "lefdefparser.h"

namespace phydb {

PhyDB::~PhyDB() = default;

Tech *PhyDB::GetTechPtr() {
  return &tech_;
}

Tech &PhyDB::tech() {
  return tech_;
}

Design *PhyDB::GetDesignPtr() {
  return &design_;
}

Design &PhyDB::design() {
  return design_;
}

void PhyDB::SetLefVersion(double version) {
  tech_.SetVersion(version);
}

void PhyDB::SetDatabaseMicron(int database_micron) {
  tech_.SetDatabaseMicron(database_micron);
}

void PhyDB::SetManufacturingGrid(double manufacture_grid) {
  tech_.SetManufacturingGrid(manufacture_grid);
}

Site *PhyDB::AddSite(
    std::string const &name,
    std::string const &class_name,
    double width,
    double height
) {
  return tech_.AddSite(name, class_name, width, height);
}

std::vector<Site> &PhyDB::GetSitesRef() {
  return tech_.sites_;
}

Site &PhyDB::GetSite(int id) {
  return tech_.sites_[id];
}

void PhyDB::SetPlacementGrids(
    double placement_grid_value_x_,
    double placement_grid_value_y_
) {
  tech_.SetPlacementGrids(placement_grid_value_x_, placement_grid_value_y_);
}

bool PhyDB::IsLayerExisting(std::string const &layer_name) {
  return tech_.IsLayerExisting(layer_name);
}

Layer *PhyDB::AddLayer(
    std::string const &layer_name,
    LayerType type,
    MetalDirection direction
) {
  return tech_.AddLayer(layer_name, type, direction);
}

Layer *PhyDB::GetLayerPtr(std::string const &layer_name) {
  return tech_.GetLayerPtr(layer_name);
}

std::vector<Layer> &PhyDB::GetLayersRef() {
  return tech_.GetLayersRef();
}

bool PhyDB::IsMacroExisting(std::string const &macro_name) {
  return tech_.IsMacroExisting(macro_name);
}

Macro *PhyDB::AddMacro(std::string const &macro_name) {
  return tech_.AddMacro(macro_name);
}

Macro *PhyDB::GetMacroPtr(std::string const &macro_name) {
  return tech_.GetMacroPtr(macro_name);
}

bool PhyDB::IsLefViaExisting(std::string const &name) {
  return tech_.IsLefViaExisting(name);
}

LefVia *PhyDB::AddLefVia(std::string const &via_name) {
  return tech_.AddLefVia(via_name);
}

LefVia *PhyDB::GetLefViaPtr(std::string const &via_name) {
  return tech_.GetLefViaPtr(via_name);
}

bool PhyDB::IsViaRuleGenerateExisting(std::string const &name) {
  return tech_.IsViaRuleGenerateExisting(name);
}

ViaRuleGenerate *PhyDB::AddViaRuleGenerate(std::string const &name) {
  return tech_.AddViaRuleGenerate(name);
}

ViaRuleGenerate *PhyDB::GetViaRuleGeneratePtr(std::string const &name) {
  return tech_.GetViaRuleGeneratePtr(name);
}

void PhyDB::SetDefName(std::string const &name) {
  design_.SetName(name);
}

void PhyDB::SetDefVersion(double version) {
  design_.SetVersion(version);
}

void PhyDB::SetDefDividerChar(std::string const &divider_char) {
  design_.SetDividerChar(divider_char);
}

void PhyDB::SetDefBusBitChar(std::string const &bus_bit_char) {
  design_.SetBusBitChar(bus_bit_char);
}

std::string PhyDB::GetDefName() const {
  return design_.GetName();
}

double PhyDB::GetDefVersion() const {
  return design_.GetVersion();
}

std::string PhyDB::GetDefDividerChar() const {
  return design_.GetDividerChar();
}

std::string PhyDB::GetDefBusBitChar() const {
  return design_.GetBusBitChar();
}

void PhyDB::SetUnitsDistanceMicrons(int distance_microns) {
  design_.SetUnitsDistanceMicrons(distance_microns);
}

void PhyDB::SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y) {
  design_.SetDieArea(lower_x, lower_y, upper_x, upper_y);
}

Rect2D<int> PhyDB::GetDieArea() const {
  return design_.GetDieArea();
}

void PhyDB::SetComponentCount(int count) {
  design_.SetComponentCount(count);
}

bool PhyDB::IsComponentExisting(std::string const &component_name) {
  return design_.IsComponentExisting(component_name);
}

Component *PhyDB::AddComponent(
    std::string const &comp_name,
    Macro *macro_ptr,
    PlaceStatus place_status,
    int llx,
    int lly,
    CompOrient orient,
    CompSource source
) {
  return design_.AddComponent(
      comp_name,
      macro_ptr,
      place_status,
      llx,
      lly,
      orient,
      source
  );
}

Component *PhyDB::GetComponentPtr(std::string const &comp_name) {
  return design_.GetComponentPtr(comp_name);
}

int PhyDB::GetComponentId(std::string const &comp_name) {
  return design_.GetComponentId(comp_name);
}

Track *PhyDB::AddTrack(
    XYDirection direction,
    int start,
    int nTracks,
    int step,
    std::vector<std::string> &layer_names
) {
  return design_.AddTrack(direction, start, nTracks, step, layer_names);
}

std::vector<Track> &PhyDB::GetTracksRef() {
  return design_.GetTracksRef();
}

Row *PhyDB::AddRow(
    std::string const &name,
    std::string const &site_name,
    std::string const &row_orient,
    int origX,
    int origY,
    int numX,
    int numY,
    int stepX,
    int stepY
) {
  int site_id = tech_.GetSiteId(site_name);
  PhyDBExpects(
      site_id != -1,
      "Site name is not in the database: " << site_name
  );
  CompOrient orient = StrToCompOrient(row_orient);
  return design_.AddRow(
      name,
      site_id,
      orient,
      origX,
      origY,
      numX,
      numY,
      stepX,
      stepY
  );
}

std::vector<Row> &PhyDB::GetRowVec() {
  return design_.GetRowVec();
}

void PhyDB::SetIoPinCount(int count) {
  design_.SetIoPinCount(count);
}

bool PhyDB::IsIoPinExisting(std::string const &iopin_name) {
  return design_.IsIoPinExisting(iopin_name);
}

IOPin *PhyDB::AddIoPin(
    std::string const &iopin_name,
    SignalDirection signal_direction,
    SignalUse signal_use
) {
  return design_.AddIoPin(iopin_name, signal_direction, signal_use);
}

IOPin *PhyDB::GetIoPinPtr(std::string const &iopin_name) {
  return design_.GetIoPinPtr(iopin_name);
}

int PhyDB::GetIoPinId(std::string const &iopin_name) {
  return design_.GetIoPinId(iopin_name);
}

void PhyDB::SetBlockageCount(int count) {
  design_.SetBlockageCount(count);
}

Blockage *PhyDB::AddBlockage() {
  return design_.AddBlockage();
}

Blockage *PhyDB::GetBlockage(int index) {
  return design_.GetBlockage(index);
}

std::vector<Blockage> &PhyDB::GetBlockagesRef() {
  return design_.GetBlockagesRef();
}

bool PhyDB::IsDefViaExisting(std::string const &name) {
  return design_.IsDefViaExisting(name);
}

DefVia *PhyDB::AddDefVia(std::string const &via_name) {
  return design_.AddDefVia(via_name);
}

DefVia *PhyDB::GetDefViaPtr(std::string const &via_name) {
  return design_.GetDefViaPtr(via_name);
}

void PhyDB::SetNetCount(int count) {
  design_.SetNetCount(count);
}

bool PhyDB::IsNetExisting(std::string const &net_name) {
  return design_.IsNetExisting(net_name);
}

Net *PhyDB::AddNet(
    std::string const &net_name,
    double weight,
    void *act_net_ptr
) {
  auto *ret = design_.AddNet(net_name, weight);

  if (act_net_ptr != nullptr) {
    if (timing_api_.IsActNetPtrExisting(act_net_ptr)) {
      int id = timing_api_.ActNetPtr2Id(act_net_ptr);
      PhyDBExpects(
          false,
          "Net " << design_.GetNetsRef()[id].GetName()
                 << " has the same Act pointer as "
                 << net_name
      );
    }
    int id = static_cast<int>(design_.GetNetsRef().size()) - 1;
    timing_api_.AddActNetPtrIdPair(act_net_ptr, id);
  }
  return ret;
}

Net *PhyDB::GetNetPtr(std::string const &net_name) {
  return design_.GetNetPtr(net_name);
}

int PhyDB::GetNetId(std::string const &net_name) {
  return design_.GetNetId(net_name);
}

void PhyDB::AddIoPinToNet(
    std::string const &io_pin_name,
    std::string const &net_name,
    void *act_io_pin_ptr
) {
  PhyDBExpects(
      IsIoPinExisting(io_pin_name),
      "Cannot add a nonexistent iopin to a net: " << io_pin_name
  );
  PhyDBExpects(
      IsNetExisting(net_name),
      "Cannot add iopin to a nonexistent Net: " << net_name
  );

  int io_pin_id = GetIoPinId(io_pin_name);
  int net_id = GetNetId(net_name);
  design_.AddIoPinToNet(io_pin_id, net_id);

  if (act_io_pin_ptr == nullptr) return;

  if (timing_api_.IsActComPinPtrExisting(act_io_pin_ptr)) {
    PhydbPin tmp_pin = timing_api_.ActCompPinPtr2Id(act_io_pin_ptr);
    if (tmp_pin.IsComponentPin()) {
      Component &comp = design_.GetComponentsRef()[tmp_pin.InstanceId()];
      Macro *tmp_macro_ptr = comp.GetMacro();
      Pin &pin = tmp_macro_ptr->GetPinsRef()[tmp_pin.PinId()];
      PhyDBExpects(
          false,
          "IO pin, " << io_pin_name
                     << ", has the same Act pointer as component pin ("
                     << comp.GetName() << " " << pin.GetName() << ")"
      );
    } else {
      IOPin &tmp_io_pin = design_.GetIoPinsRef()[tmp_pin.PinId()];
      PhyDBExpects(
          false,
          "IO pin, " << io_pin_name
                     << ", has the same Act pointer as IO pin, "
                     << tmp_io_pin.GetName()
      );
    }
  }
  PhydbPin phydb_pin(-1, io_pin_id);
  timing_api_.BindActPinAndPhydbPin(act_io_pin_ptr, phydb_pin);

}

void PhyDB::AddCompPinToNet(
    std::string const &comp_name,
    std::string const &pin_name,
    std::string const &net_name,
    void *act_comp_pin_ptr
) {
  PhyDBExpects(
      IsNetExisting(net_name),
      "Cannot add a component pin to a nonexistent Net: " << net_name
  );
  PhyDBExpects(
      IsComponentExisting(comp_name),
      "Cannot add a nonexistent component to a net: " << comp_name
  );
  Component *comp_ptr = GetComponentPtr(comp_name);
  Macro *macro_ptr = comp_ptr->GetMacro();
  PhyDBExpects(
      macro_ptr->IsPinExisting(pin_name),
      "Macro " << macro_ptr->GetName() << " does not contain a pin with name "
               << pin_name
  );

  int comp_id = GetComponentId(comp_name);
  int pin_id = macro_ptr->GetPinId(pin_name);
  int net_id = GetNetId(net_name);
  design_.AddCompPinToNet(comp_id, pin_id, net_id);

  if (act_comp_pin_ptr == nullptr) return;

  if (timing_api_.IsActComPinPtrExisting(act_comp_pin_ptr)) {
    PhydbPin tmp_pin = timing_api_.ActCompPinPtr2Id(act_comp_pin_ptr);
    Component &comp = design_.GetComponentsRef()[tmp_pin.InstanceId()];
    Macro *tmp_macro_ptr = comp.GetMacro();
    Pin &pin = tmp_macro_ptr->GetPinsRef()[tmp_pin.PinId()];
    PhyDBExpects(
        false,
        "Component pin: " << comp.GetName() << " " << pin.GetName()
                          << " has the same Act pointer as "
                          << comp_name << " " << pin_name
    );
  }
  PhydbPin phydb_pin(comp_id, pin_id);
  timing_api_.BindActPinAndPhydbPin(act_comp_pin_ptr, phydb_pin);
}

SNet *PhyDB::AddSNet(std::string const &net_name, SignalUse use) {
  return design_.AddSNet(net_name, use);
}

SNet *PhyDB::GetSNet(std::string const &net_name) {
  return design_.GetSNet(net_name);
}

std::vector<SNet> &PhyDB::GetSNetRef() {
  return design_.GetSNetRef();
}

void PhyDB::SetNwellLayer(
    double width,
    double spacing,
    double op_spacing,
    double max_plug_dist,
    double overhang
) {
  tech_.SetNwellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
}

void PhyDB::SetPwellLayer(
    double width,
    double spacing,
    double op_spacing,
    double max_plug_dist,
    double overhang
) {
  tech_.SetPwellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
}

void PhyDB::SetNpwellSpacing(double same_spacing, double any_spacing) {
  tech_.SetNpwellSpacing(same_spacing, any_spacing);
}

MacroWell *PhyDB::AddMacrowell(std::string const &macro_name) {
  Macro *macro_ptr = GetMacroPtr(macro_name);
  PhyDBExpects(macro_ptr != nullptr,
               "Macro does not exist, cannot add well info: " + macro_name);
  macro_ptr->InitWellPtr(macro_ptr);
  return macro_ptr->WellPtrRef().get();
}

/****
 * This function will add dummy well for all macros if they do not have well info.
 * P-well will be at the bottom and N-well at the top.
 * The boundary is the floor of the half of the cell height with grid value y as the unit.
 */
void PhyDB::AddDummyWell(double p_well_height) {
  for (auto &macro : tech_.macros_) {
    std::unique_ptr<MacroWell> &well_ptr = macro.WellPtrRef();
    // skip macros with a well
    if (well_ptr != nullptr) {
      return;
    }
    double width = macro.GetWidth();
    double height = macro.GetHeight();
    double n_p_boundary = p_well_height;
    if (n_p_boundary < 0) {
      n_p_boundary = height / 2.0;
    }
    well_ptr = std::make_unique<MacroWell>(&macro);
    well_ptr->SetPwellRect(0, 0, width, n_p_boundary);
    well_ptr->SetNwellRect(0, n_p_boundary, width, height);
  }
}

ClusterCol *PhyDB::AddClusterCol(
    std::string const &name,
    std::string const &bot_signal
) {
  return design_.AddClusterCol(name, bot_signal);
}

std::vector<ClusterCol> &PhyDB::GetClusterColsRef() {
  return design_.GetClusterColsRef();
}

SpecialMacroRectLayout *PhyDB::CreatePpNpMacroAndComponent(
    int llx,
    int lly,
    int urx,
    int ury
) {
  std::string plus_filling_macro_name("circuitppnp");
  Macro *plus_filling_macro = AddMacro(plus_filling_macro_name);
  plus_filling_macro->SetClass(MacroClass::COVER);
  plus_filling_macro->SetOrigin(0, 0);
  plus_filling_macro->SetSize(urx - llx, ury - lly);

  std::string plus_filling_component_name("ppnps");
  AddComponent(
      plus_filling_component_name,
      plus_filling_macro,
      PlaceStatus::COVER,
      llx,
      lly,
      CompOrient::N,
      CompSource::USER
  );
  return design_.CreatePpNpMacroAndComponent(
      plus_filling_macro,
      llx,
      lly,
      urx,
      ury
  );
}

void PhyDB::SavePpNpToRectFile(std::string const &file_name) {
  design_.SavePpNpToRectFile(file_name);
}

SpecialMacroRectLayout *PhyDB::CreateWellLayerMacroAndComponent(
    int llx,
    int lly,
    int urx,
    int ury
) {
  std::string well_filling_macro_name("circuitwell");
  Macro *well_filling_macro = AddMacro(well_filling_macro_name);
  well_filling_macro->SetClass(MacroClass::COVER);
  well_filling_macro->SetOrigin(0, 0);
  well_filling_macro->SetSize(urx - llx, ury - lly);

  std::string well_filling_component_name("npwells");
  AddComponent(
      well_filling_component_name,
      well_filling_macro,
      PlaceStatus::COVER,
      llx,
      lly,
      CompOrient::N,
      CompSource::USER
  );
  return design_.CreateWellLayerMacroAndComponent(
      well_filling_macro,
      llx,
      lly,
      urx,
      ury
  );
}

void PhyDB::SaveWellToRectFile(std::string const &file_name) {
  design_.SaveWellToRectFile(file_name);
}

GcellGrid *PhyDB::AddGcellGrid(
    XYDirection direction,
    int start,
    int nBoundaries,
    int step
) {
  return design_.AddGcellGrid(direction, start, nBoundaries, step);
}

std::vector<GcellGrid> &PhyDB::GetGcellGridsRef() {
  return design_.GetGcellGridsRef();
}

void PhyDB::SetGetNumConstraintsCB(int (*callback_function)()) {
  timing_api_.SetGetNumConstraintsCB(callback_function);
}

void PhyDB::SetSpecifyTopKsCB(void (*callback_function)(int)) {
  timing_api_.SetSpecifyTopKsCB(callback_function);
}

void PhyDB::SetSpecifyTopKCB(
    void (*callback_function)(
        int timing_constraint_id,
        int top_k
    )
) {
  timing_api_.SetSpecifyTopKCB(callback_function);
}

void PhyDB::SetUpdateTimingIncrementalCB(void (*callback_function)()) {
  timing_api_.SetUpdateTimingIncrementalCB(callback_function);
}

void PhyDB::SetGetSlackCB(
    std::vector<double> (*callback_function)(const std::vector<int> &)
) {
  timing_api_.SetGetSlackCB(callback_function);
}

void PhyDB::SetGetViolatedTimingConstraintsCB(
    void (*callback_function)(std::vector<int> &timing_constraint_ids)
) {
  timing_api_.SetGetViolatedTimingConstraintsCB(callback_function);
}

void PhyDB::SetGetSlowWitnessCB(
    void (*callback_function)(
        int timing_constraint_id,
        std::vector<ActEdge> &path
    )
) {
  timing_api_.SetGetSlowWitnessCB(callback_function);
}

void PhyDB::SetGetFastWitnessCB(
    void (*callback_function)(
        int timing_constraint_id,
        std::vector<ActEdge> &path
    )
) {
  timing_api_.SetGetFastWitnessCB(callback_function);
}

void PhyDB::SetGetNumPerformanceConstraintsCB(int (*callback_function)()) {
  timing_api_.SetGetNumPerformanceConstraintsCB(callback_function);
}

void PhyDB::SetSpecifyPerformanceTopKsCB(void (*callback_function)(int top_k)) {
  timing_api_.SetSpecifyPerformanceTopKsCB(callback_function);
}

void PhyDB::SetSpecifyPerformanceTopKCB(
    void (*callback_function)(
        int performance_id,
        int top_k
    )
) {
  timing_api_.SetSpecifyPerformanceTopKCB(callback_function);
}

void PhyDB::SetGetPerformanceConstraintWeightCB(
    double (*callback_function)(int performance_id)
) {
  timing_api_.SetGetPerformanceConstraintWeightCB(callback_function);
}

void PhyDB::SetGetPerformanceSlack(
    double (*callback_function)(int performance_id)
) {
  timing_api_.SetGetPerformanceSlackCB(callback_function);
}

void PhyDB::SetGetViolatedPerformanceConstraintsCB(
    void (*callback_function)(std::vector<int> &)
) {
  timing_api_.SetGetViolatedPerformanceConstraintsCB(callback_function);
}

void PhyDB::SetGetPerformanceWitnessCB(
    void (*callback_function)(
        int timing_constraint_id,
        std::vector<ActEdge> &path
    )
) {
  timing_api_.SetGetPerformanceWitnessCB(callback_function);
}

bool PhyDB::IsDriverPin(PhydbPin &phydb_pin) {
  int comp_id = phydb_pin.InstanceId();
  Macro *macro_ptr = design_.GetComponentsRef()[comp_id].GetMacro();
  int pin_id = phydb_pin.PinId();
  return macro_ptr->GetPinsRef()[pin_id].IsDriverPin();
}

std::string PhyDB::GetFullCompPinName(
    PhydbPin &phydb_pin,
    char delimiter
) {
  int comp_id = phydb_pin.InstanceId();
  int pin_id = phydb_pin.PinId();
  Component &comp = design_.GetComponentsRef()[comp_id];
  const std::string &comp_name = comp.GetName();
  const std::string &pin_name =
      comp.GetMacro()->GetPinsRef()[pin_id].GetName();
  std::string full_name(comp_name);
  full_name.push_back(delimiter);
  full_name.append(pin_name);
  return full_name;
}

ActPhyDBTimingAPI &PhyDB::GetTimingApi() {
  return timing_api_;
}

#if PHYDB_USE_GALOIS
void PhyDB::SetParaManager(galois::eda::parasitics::Manager *manager) {
  timing_api_.SetParaManager(manager);
}

void PhyDB::AddCellLib(galois::eda::liberty::CellLib *lib) {
  timing_api_.AddCellLib(lib);
}

void PhyDB::SetNetlistAdaptor(galois::eda::utility::ExtNetlistAdaptor *adaptor) {
  timing_api_.SetNetlistAdaptor(adaptor);
}

galois::eda::parasitics::Manager *PhyDB::GetParaManager() {
  return timing_api_.GetParaManager();
}

std::vector<galois::eda::liberty::CellLib *> &PhyDB::GetCellLibs() {
  return timing_api_.GetCellLibs();
}

galois::eda::utility::ExtNetlistAdaptor *PhyDB::GetNetlistAdaptor() {
  return timing_api_.GetNetlistAdaptor();
}

void PhyDB::CreatePhydbActAdaptor() {
  auto *timer_adaptor = GetNetlistAdaptor();
  PhyDBExpects(timer_adaptor != nullptr,
               "Timer netlist adaptor no found! Cannot build phydb-act adaptor");
  int number_of_nets = static_cast<int>(design_.GetNetsRef().size());
  for (int i = 0; i < number_of_nets; ++i) {
    Net &net = design_.GetNetsRef()[i];
    void *act_net = timer_adaptor->getNetFromFullName(net.GetName(), '.');
    PhyDBExpects(
        act_net != nullptr,
        "Net cannot be found in the timer netlist adaptor: " << net.GetName()
    );
    timing_api_.AddActNetPtrIdPair(act_net, i);

    for (auto &physb_pin : net.GetPinsRef()) {
      BindPhydbPinToActPin_(physb_pin);
    }
  }
}

void PhyDB::AddNetsAndCompPinsToSpefManager() {
  auto *spef_manager = GetParaManager();
  PhyDBExpects(spef_manager != nullptr,
               "Cannot push RC to the timer because the SPEF manager is not set");
  std::vector<galois::eda::liberty::CellLib *> &libs = GetCellLibs();
  PhyDBExpects(!libs.empty(), "No cell library found in the timer?");

  //std::cout << "before adding anything\n";
  //spef_manager->dump();
  //std::cout << "--------------------------------------------------\n";
  // add nets and pins to the SPEF manager
  int number_of_nets = (int) design_.GetNetsRef().size();
  for (int i = 0; i < number_of_nets; ++i) {
    Net &net = design_.GetNetsRef()[i];
    void *act_net = timing_api_.net_id_2_act_[i];
    PhyDBExpects(
        act_net != nullptr,
        "Cannot map from a PhyDB net to an ACT net, net name: "
            << design_.GetNetsRef()[i].GetName()
    );
    if (spef_manager->findNet(act_net) == nullptr) {
      spef_manager->addNet(act_net);
    }

    int number_of_pins = (int) net.GetPinsRef().size();
    for (int j = 0; j < number_of_pins; ++j) {
      auto &phydb_pin = net.GetPinsRef()[j];
      void *act_pin = timing_api_.PhydbCompPin2ActPtr(phydb_pin);
      if (act_pin == nullptr) {
        std::string phydb_pin_name = GetFullCompPinName(phydb_pin, ':');
        PhyDBExpects(
            false,
            "Cannot map from a PhyDB component pin to an ACT pin: "
                << phydb_pin_name
        );
      }
      bool is_act_pin_in_spef = spef_manager->findPin(act_pin) != nullptr;
      if (IsDriverPin(phydb_pin)) {
        net.SetDriverPin(false, j); // TODO
        if (!is_act_pin_in_spef) {
          spef_manager->addDriverPin(act_pin);
        }
      } else {
        if (!is_act_pin_in_spef) {
          spef_manager->addLoadPin(act_pin);
        }
      }

    }
  }
}
#endif

void PhyDB::ReadLef(std::string const &lef_file_name) {
  tech_.SetLefName(lef_file_name);
  Si2ReadLef(this, lef_file_name);
}

void PhyDB::ReadDef(std::string const &def_file_name) {
  design_.SetDefName(def_file_name);
  Si2ReadDef(this, def_file_name);
}

/**
 * @brief Override component locations from a DEF file.
 *
 * @param def_file_name: the DEF file name which contains new component locations.
 * @return nothing
 */
void PhyDB::OverrideComponentLocsFromDef(std::string const &def_file_name) {
  Si2LoadPlacedDef(this, def_file_name);
}

void PhyDB::ReadCell(std::string const &cell_file_name) {
  std::ifstream ist(cell_file_name.c_str());
  PhyDBExpects(ist.is_open(), "Cannot open input file " + cell_file_name);

  std::string line;
  while (!ist.eof()) {
    getline(ist, line);
    if (line.empty()) continue;
    if (line.find("LAYER") != std::string::npos) {
      if (line.find("LEGALIZER") != std::string::npos) {
        double same_diff_spacing = 0;
        double any_diff_spacing = 0;
        do {
          getline(ist, line);
          std::vector<std::string> legalizer_fields = StrTokenize(line);
          if (legalizer_fields.size() != 2) {
            std::cout << "Expect: SPACING + Value, get: " + line
                      << std::endl;
            exit(1);
          }
          if (legalizer_fields[0] == "SAME_DIFF_SPACING") {
            try {
              same_diff_spacing = std::stod(legalizer_fields[1]);
            } catch (...) {
              std::cout << "Invalid stod conversion: " + line
                        << std::endl;
              exit(1);
            }
          } else if (legalizer_fields[0] == "ANY_DIFF_SPACING") {
            try {
              any_diff_spacing = std::stod(legalizer_fields[1]);
            } catch (...) {
              std::cout << "Invalid stod conversion: " + line
                        << std::endl;
              exit(1);
            }
          }
        } while (line.find("END LEGALIZER") == std::string::npos
            && !ist.eof());
        SetNpwellSpacing(same_diff_spacing, any_diff_spacing);
      } else {
        std::vector<std::string> well_fields = StrTokenize(line);
        bool is_n_well = (well_fields[1] == "nwell");
        if (!is_n_well) {
          if (well_fields[1] != "pwell") {
            std::cout << "Unknow N/P well type: " + well_fields[1]
                      << std::endl;
            exit(1);
          }
        }
        std::string end_layer_flag = "END " + well_fields[1];
        double width = 0;
        double spacing = 0;
        double op_spacing = 0;
        double max_plug_dist = 0;
        double overhang = 0;
        do {
          if (line.find("MINWIDTH") != std::string::npos) {
            std::vector<std::string> width_fields = StrTokenize(line);
            try {
              width = std::stod(width_fields[1]);
            } catch (...) {
              std::cout
                  << "Invalid stod conversion: " + width_fields[1]
                  << std::endl;
              exit(1);
            }
          } else if (line.find("OPPOSPACING") != std::string::npos) {
            std::vector<std::string> oppo_spacing_fields = StrTokenize(line);
            try {
              op_spacing = std::stod(oppo_spacing_fields[1]);
            } catch (...) {
              std::cout
                  << "Invalid stod conversion: " + oppo_spacing_fields[1]
                  << std::endl;
              exit(1);
            }
          } else if (line.find("SPACING") != std::string::npos) {
            std::vector<std::string> spacing_fields = StrTokenize(line);
            try {
              spacing = std::stod(spacing_fields[1]);
            } catch (...) {
              std::cout
                  << "Invalid stod conversion: " + spacing_fields[1]
                  << std::endl;
              exit(1);
            }
          } else if (line.find("MAXPLUGDIST") != std::string::npos) {
            std::vector<std::string> max_plug_dist_fields = StrTokenize(line);
            try {
              max_plug_dist = std::stod(max_plug_dist_fields[1]);
            } catch (...) {
              std::cout
                  << "Invalid stod conversion: " + max_plug_dist_fields[1]
                  << std::endl;
              exit(1);
            }
          } else if (line.find("OVERHANG") != std::string::npos) {
            std::vector<std::string> overhang_fields = StrTokenize(line);
            try {
              overhang = std::stod(overhang_fields[1]);
            } catch (...) {
              std::cout
                  << "Invalid stod conversion: " + overhang_fields[1]
                  << std::endl;
              exit(1);
            }
          } else {}
          getline(ist, line);
        } while (line.find(end_layer_flag) == std::string::npos
            && !ist.eof());
        if (is_n_well) {
          SetNwellLayer(
              width,
              spacing,
              op_spacing,
              max_plug_dist,
              overhang
          );
        } else {
          SetPwellLayer(
              width,
              spacing,
              op_spacing,
              max_plug_dist,
              overhang
          );
        }
      }
    }

    if (line.find("MACRO") != std::string::npos) {
      std::vector<std::string> macro_fields = StrTokenize(line);
      std::string end_macro_flag = "END " + macro_fields[1];
      MacroWell *well_ptr = AddMacrowell(macro_fields[1]);
      do {
        getline(ist, line);
        bool is_n = false;
        if (line.find("LAYER") != std::string::npos) {
          do {
            if (line.find("nwell") != std::string::npos) {
              is_n = true;
            }
            if (line.find("RECT") != std::string::npos) {
              double lx = 0, ly = 0, ux = 0, uy = 0;
              std::vector<std::string> shape_fields = StrTokenize(line);
              try {
                lx = std::stod(shape_fields[1]);
                ly = std::stod(shape_fields[2]);
                ux = std::stod(shape_fields[3]);
                uy = std::stod(shape_fields[4]);
              } catch (...) {
                std::cout << "Invalid stod conversion: " + line
                          << std::endl;
                exit(1);
              }
              well_ptr->SetWellRect(is_n, lx, ly, ux, uy);
            }
            getline(ist, line);
          } while (line.find("END VERSION") == std::string::npos
              && !ist.eof());
        }
      } while (line.find(end_macro_flag) == std::string::npos
          && !ist.eof());
    }
  }
  PhyDBExpects(
      tech_.IsWellInfoSet(),
      "N/P well technology information not found!"
  );
}

void PhyDB::ReadCluster(std::string const &cluster_file_name) {
  std::ifstream infile(cluster_file_name.c_str());
  if (infile.is_open()) {
    std::cout << "Loading cluster file: " << cluster_file_name << "\n";
  } else {
    PhyDBExpects(false, "cannot open input file " + cluster_file_name);
  }

  std::string tmp1, tmp2, tmp3;
  int lx, ux, ly, uy;
  ClusterCol *col = nullptr;
  while (!infile.eof()) {
    infile >> tmp1 >> tmp2;
    if (tmp1 == "STRIP") {
      infile >> lx >> ux >> tmp3;
      col = AddClusterCol(tmp2, tmp3);
      col->SetXRange(lx, ux);
    } else if (tmp1 == "END") {
      assert(col != nullptr && tmp2 == col->GetName());
    } else {
      try {
        ly = stoi(tmp1);
        uy = stoi(tmp2);
      } catch (...) {
        PhyDBExpects(false, "Cannot convert string to integers");
      }
      col->AddRow(ly, uy);
    }
  }
}

/****
 * @brief Read a technology configuration file
 *
 * @param tech_config_file_name
 * @return true if there is no errors, false if there is anything wrong
 */
bool PhyDB::ReadTechConfigFile(std::string const &tech_config_file_name) {
  // resistance and capacitance information will be saved into metal layers,
  // so we need to make sure metal layers are in the database
  PhyDBExpects(
      !tech_.layers_.empty(),
      "Layers in PhyDB are needed for loading technology configuration file"
  );

  ReadTechnologyConfigurationFile(this, tech_config_file_name);

  // fix the last entry in the resistance over table
  // and use this technology configuration table to set r/c units
  tech_.FixResOverTable();
  tech_.SetResistanceUnit(true, false);
  tech_.SetCapacitanceUnit(true, false);

  return true;
}

bool PhyDB::ReadTechConfigFile(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Please provide a technology configuration file name\n";
    return false;
  }

  std::string file_name(argv[1]);
  if (file_name == "__manual__") {
    if (argc < 5) {
      std::cout << "Please provide unit resistance and unit capacitance\n";
      return false;
    }
    double unit_res, unit_fringe_cap, unit_area_cap;
    try {
      unit_res = std::stod(argv[2]);
      unit_fringe_cap = std::stod(argv[3]);
      unit_area_cap = std::stod(argv[4]);
    } catch (...) {
      return false;
    }
    tech_.FindAllMetalLayers();
    tech_.SetUnitResAndCap(unit_res, unit_fringe_cap, unit_area_cap);
  } else {
    return ReadTechConfigFile(file_name);
  }

  return true;
}

void PhyDB::WriteDef(std::string const &def_file_name) {
  Si2WriteDef(this, def_file_name);
}

void PhyDB::WriteCluster(std::string const &cluster_file_name) {
  std::ofstream outfile(cluster_file_name.c_str());
  if (outfile.is_open()) {
    std::cout << "writing cluster file: " << cluster_file_name << "\n";
  } else {
    PhyDBExpects(
        false,
        "Cannot open output cluster file " + cluster_file_name
    );
  }

  auto cluster_cols = this->GetClusterColsRef();
  for (auto cluster_col : cluster_cols) {
    outfile << "STRIP " << cluster_col.GetName() << std::endl;
    outfile << cluster_col.GetLX() << " " << cluster_col.GetUX() << " "
            << cluster_col.GetBotSignal() << std::endl;
    auto ly = cluster_col.GetLY();
    auto uy = cluster_col.GetUY();
    for (size_t i = 0; i < ly.size(); i++) {
      outfile << ly[i] << " " << uy[i] << std::endl;
    }
    outfile << "END " << cluster_col.GetName() << std::endl;
    outfile << std::endl;
  }
}

void PhyDB::WriteGuide(std::string const &guide_file_name) {
  std::ofstream outfile(guide_file_name.c_str());
  if (outfile.is_open()) {
    std::cout << "writing guide file: " << guide_file_name << "\n";
  } else {
    PhyDBExpects(false, "Cannot open output guide file " + guide_file_name);
  }

  auto design_p = this->GetDesignPtr();
  auto tech_p = this->GetTechPtr();
  auto netlist = design_p->GetNetsRef();
  for (auto net : netlist) {
    std::string netName = net.GetName();
    outfile << netName << std::endl;
    outfile << "(" << std::endl;

    auto routing_guides = net.GetRoutingGuidesRef();
    for (auto gcell : routing_guides) {
      std::string layer_name = tech_p->GetLayerName(gcell.ll.z);
      outfile << gcell.ll.x << " ";
      outfile << gcell.ll.y << " ";
      outfile << gcell.ur.x << " ";
      outfile << gcell.ur.y << " ";
      outfile << layer_name << std::endl;
    }
    outfile << ")" << std::endl;
  }
}

#if PHYDB_USE_GALOIS
void PhyDB::BindPhydbPinToActPin_(PhydbPin &phydb_pin) {
  auto *timer_adaptor = GetNetlistAdaptor();
  PhyDBExpects(
      timer_adaptor != nullptr,
      "Timer netlist adaptor no found! Cannot build phydb-act adaptor"
  );
  std::string pin_name = GetFullCompPinName(phydb_pin, ':');
  void *act_pin = timer_adaptor->getPinFromFullName(pin_name);
  if (timing_api_.IsActComPinPtrExisting(act_pin)) {
    if (timing_api_.ActCompPinPtr2Id(act_pin) != phydb_pin) {
      PhydbPin existing_pin = timing_api_.ActCompPinPtr2Id(act_pin);
      std::string tmp_pin_name = GetFullCompPinName(existing_pin, ':');
      PhyDBExpects(false, "ACT pin pointer, "
          << act_pin << ", is associated with the following PhyDB pin:\n"
          << "    " << tmp_pin_name << ".\n"
          << "Now this ACT pin needs to be associated with another PhyDB pin:\n"
          << pin_name << "\n"
      );
    }
  } else {
    timing_api_.BindActPinAndPhydbPin(act_pin, phydb_pin);
  }
}
#endif

}
