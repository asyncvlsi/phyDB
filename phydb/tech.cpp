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
#include "tech.h"

#include <cstring>
#include <unordered_set>

namespace phydb {

Tech::~Tech() {
  delete n_layer_ptr_;
  n_layer_ptr_ = nullptr;
  delete p_layer_ptr_;
  p_layer_ptr_ = nullptr;
}

void Tech::SetVersion(double version) {
  version_ = version;
}

void Tech::SetDatabaseMicron(int database_micron) {
  if (database_micron != database_micron_) {
    std::cout << "Warning: changing DATABASE MICRONS from "
              << database_micron_ << " to " << database_micron << "\n";
  }
  database_micron_ = database_micron;
}

int Tech::GetDatabaseMicron() const {
  return database_micron_;
}

void Tech::SetManufacturingGrid(double manufacture_grid) {
  PhyDBExpects(
      manufacture_grid > 0,
      "Cannot Set negative manufacturing grid: " << manufacture_grid
  );

  if (manufacturing_grid_ > -1 && manufacture_grid != manufacturing_grid_) {
    std::cout << "Warning: changing MANUFACTURINGGRID from "
              << manufacturing_grid_ << " to " << manufacture_grid << "\n";
  }

  manufacturing_grid_ = manufacture_grid;
}

double Tech::GetManufacturingGrid() const {
  return manufacturing_grid_;
}

bool Tech::IsSiteExisting(const std::string &site_name) {
  return site_2_id_.find(site_name) != site_2_id_.end();
}

Site *Tech::AddSite(
    std::string const &site_name,
    const std::string &class_name,
    double width,
    double height
) {
  PhyDBExpects(
      !IsSiteExisting(site_name),
      "Site site_name exists, cannot use it again: " << site_name
  );
  SiteClass site_class = StrToSiteClass(class_name);
  int id = static_cast<int>(sites_.size());
  sites_.emplace_back(site_name, site_class, width, height);
  site_2_id_[site_name] = id;
  return &(sites_[id]);
}

std::vector<Site> &Tech::GetSitesRef() {
  return sites_;
}

int Tech::GetSiteId(std::string const &site_name) {
  if (!IsSiteExisting(site_name)) {
    return -1;
  }
  int id = site_2_id_[site_name];
  return id;
}

void Tech::SetPlacementGrids(
    double placement_grid_value_x,
    double placement_grid_value_y
) {
  PhyDBExpects(
      placement_grid_value_x > 0 && placement_grid_value_y > 0,
      "negative placement grid value not allowed"
  );
  placement_grid_value_x_ = placement_grid_value_x;
  placement_grid_value_y_ = placement_grid_value_y;
  is_placement_grid_set_ = true;
}

bool Tech::GetPlacementGrids(
    double &placement_grid_value_x,
    double &placement_grid_value_y
) const {
  placement_grid_value_x = placement_grid_value_x_;
  placement_grid_value_y = placement_grid_value_y_;
  return is_placement_grid_set_;
}

bool Tech::IsLayerExisting(std::string const &layer_name) {
  return layer_2_id_.find(layer_name) != layer_2_id_.end();
}

Layer *Tech::AddLayer(
    std::string const &layer_name,
    LayerType type,
    MetalDirection direction
) {
  PhyDBExpects(
      !IsLayerExisting(layer_name),
      "LAYER name_ exists, cannot use again: " << layer_name
  );
  int id = static_cast<int>(layers_.size());
  layers_.emplace_back(layer_name, type, direction);
  layer_2_id_[layer_name] = id;
  layers_[id].SetID(id);
  return &(layers_[id]);
}

Layer *Tech::GetLayerPtr(std::string const &layer_name) {
  if (!IsLayerExisting(layer_name)) {
    return nullptr;
  }
  int id = layer_2_id_[layer_name];
  return &(layers_[id]);
}

int Tech::GetLayerId(std::string const &layer_name) {
  if (!IsLayerExisting(layer_name)) {
    return -1;
  }
  int id = layer_2_id_[layer_name];
  return id;
}

const std::string &Tech::GetLayerName(int layer_id) {
  if (layer_id >= (int) layers_.size()) {
    std::cout << "accessing layer_id > num. of layers" << std::endl;
    exit(1);
  }
  return layers_[layer_id].GetName();
}

std::vector<Layer> &Tech::GetLayersRef() {
  return layers_;
}

std::vector<Layer *> &Tech::GetMetalLayersRef() {
  return metal_layers_;
}

bool Tech::IsMacroExisting(std::string const &macro_name) {
  return macro_2_ptr_.find(macro_name) != macro_2_ptr_.end();
}

Macro *Tech::AddMacro(std::string const &macro_name) {
  PhyDBExpects(
      !IsMacroExisting(macro_name),
      "Macro name_ exists, cannot use it again: " << macro_name
  );
  macros_.emplace_back(macro_name);
  macro_2_ptr_[macro_name] = &macros_.back();
  return &(macros_.back());
}

Macro *Tech::GetMacroPtr(std::string const &macro_name) {
  if (!IsMacroExisting(macro_name)) {
    return nullptr;
  }
  return macro_2_ptr_[macro_name];
}

std::list<Macro> &Tech::GetMacrosRef() {
  return macros_;
}

bool Tech::IsLefViaExisting(std::string const &via_name) {
  return via_2_id_.find(via_name) != via_2_id_.end();
}

LefVia *Tech::AddLefVia(std::string const &via_name) {
  PhyDBExpects(!IsLefViaExisting(via_name),
               "VIA name_ exists, cannot use it again: " << via_name);
  int id = (int) vias_.size();
  vias_.emplace_back(via_name);
  via_2_id_[via_name] = id;
  return &(vias_[id]);
}

LefVia *Tech::GetLefViaPtr(std::string const &via_name) {
  if (!IsLefViaExisting(via_name)) {
    return nullptr;
  }
  int id = via_2_id_[via_name];
  return &(vias_[id]);
}

std::vector<LefVia> &Tech::GetLefViasRef() {
  return vias_;
}

bool Tech::IsViaRuleGenerateExisting(std::string const &name) {
  return via_rule_generate_2_id_.find(name) != via_rule_generate_2_id_.end();
}

ViaRuleGenerate *Tech::AddViaRuleGenerate(std::string const &name) {
  PhyDBExpects(!IsViaRuleGenerateExisting(name),
               "Macro name_ exists, cannot use it again");
  int id = (int) via_rule_generates_.size();
  via_rule_generates_.emplace_back(name);
  via_rule_generate_2_id_[name] = id;
  return &(via_rule_generates_[id]);
}

ViaRuleGenerate *Tech::GetViaRuleGeneratePtr(std::string const &name) {
  if (!IsViaRuleGenerateExisting(name)) {
    return nullptr;
  }
  int id = via_rule_generate_2_id_[name];
  return &(via_rule_generates_[id]);
}

std::vector<ViaRuleGenerate> &Tech::GetViaRuleGeneratesRef() {
  return via_rule_generates_;
}

void Tech::SetNwellLayer(
    double width,
    double spacing,
    double op_spacing,
    double max_plug_dist,
    double overhang
) {
  if (is_n_well_layer_set_) {
    n_layer_ptr_->SetParams(
        width,
        spacing,
        op_spacing,
        max_plug_dist,
        overhang
    );
  } else {
    n_layer_ptr_ =
        new WellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
    is_n_well_layer_set_ = true;
  }
}

void Tech::SetPwellLayer(
    double width,
    double spacing,
    double op_spacing,
    double max_plug_dist,
    double overhang
) {
  if (is_p_well_layer_set_) {
    p_layer_ptr_->SetParams(
        width,
        spacing,
        op_spacing,
        max_plug_dist,
        overhang
    );
  } else {
    p_layer_ptr_ =
        new WellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
    is_p_well_layer_set_ = true;
  }
}

void Tech::SetNpwellSpacing(double same_diff, double any_diff) {
  PhyDBExpects(same_diff >= 0 && any_diff >= 0,
               "Negative values not allowed: Tech::SetNpwellSpacing()");
  same_diff_spacing_ = same_diff;
  any_diff_spacing_ = any_diff;
}

bool Tech::IsWellInfoSet() const {
  return (n_layer_ptr_ != nullptr) || (p_layer_ptr_ != nullptr);
}

WellLayer *Tech::GetNwellLayerPtr() {
  return n_layer_ptr_;
}

WellLayer *Tech::GetPwellLayerPtr() {
  return p_layer_ptr_;
}

void Tech::GetDiffWellSpacing(
    double &same_diff_spacing,
    double &any_diff_spacing
) const {
  same_diff_spacing = same_diff_spacing_;
  any_diff_spacing = any_diff_spacing_;
}

void Tech::ReportWellShape() {
  for (auto &macro : macros_) {
    std::unique_ptr<MacroWell> &well_ptr = macro.WellPtrRef();
    if (well_ptr != nullptr) {
      well_ptr->Report();
    }
  }
}

std::string Tech::GetLefName() const {
  return lef_name_;
}

void Tech::SetLefName(std::string const &lef_file_name) {
  lef_name_ = lef_file_name;
}

void Tech::FindAllMetalLayers() {
  for (auto &layer : layers_) {
    if (layer.GetType() == phydb::LayerType::ROUTING) {
      metal_layers_.emplace_back(&layer);
    }
  }
}

TechConfig &Tech::GetTechConfigRef() {
  return tech_config_;
}

void Tech::SetTechConfigLayerCount(int number_of_layers) {
  if (metal_layers_.empty()) {
    FindAllMetalLayers();
  }
  PhyDBExpects(number_of_layers == (int) metal_layers_.size(),
               "metal layers given in the technology configuration file does not match those in the database");
  tech_config_.SetLayerCount(number_of_layers);
}

void Tech::AddTechConfigCorner(int corner_index) {
  for (auto &metal_ptr : metal_layers_) {
    metal_ptr->AddTechConfigCorner(corner_index);
  }
}

ConfigTable &Tech::InitConfigTable(
    TableType type,
    int layer_index,
    int index0,
    int index1,
    int corner_index
) {
  auto metal_ptr = metal_layers_[layer_index];
  auto &corner = metal_ptr->GetLayerTechConfig()->CornersRef()[corner_index];
  if (type == RES_OVER) {
    return corner.InitResOverTable(layer_index, index0);
  } else if (type == CAP_OVER) {
    return corner.InitCapOverTable(layer_index, index0);
  } else if (type == CAP_UNDER) {
    return corner.InitCapUnderTable(layer_index, index0);
  } else if (type == CAP_DIAGUNDER) {
    return corner.InitCapDiagUnderTable(layer_index, index0);
  } else {
    return corner.InitCapOverUnderTable(layer_index, index0, index1);
  }
}

void Tech::FixResOverTable() {
  for (auto &metal_ptr : metal_layers_) {
    auto layer_tech_config = metal_ptr->GetLayerTechConfig();
    if (layer_tech_config != nullptr) {
      layer_tech_config->FixResOverTable();
    }
  }
}

void Tech::SetResistanceUnit(bool from_tech_config, bool is_report) {
  if (from_tech_config) {
    for (auto &metal_ptr : metal_layers_) {
      metal_ptr->SetResistanceUnitFromTechConfig();
    }
  } else {
    for (auto &metal_ptr : metal_layers_) {
      metal_ptr->SetResistanceUnitFromLef();
    }
  }

  if (is_report) {
    int num_of_corners_ = tech_config_.corner_count_;
    for (int i = 0; i < num_of_corners_; ++i) {
      std::cout << "corner index: " << i << "\nres, ";
      size_t sz = metal_layers_.size();
      for (size_t j = 0; j < sz; ++j) {
        std::cout << j + 1 << ": " << metal_layers_[j]->unit_res_[i]
                  << ", ";
      }
      std::cout << "\n";
    }
  }
}

void Tech::SetCapacitanceUnit(bool from_tech_config, bool is_report) {
  if (from_tech_config) {
    for (auto &metal_ptr : metal_layers_) {
      metal_ptr->SetCapacitanceUnitFromTechConfig();
    }
  } else {
    for (auto &metal_ptr : metal_layers_) {
      metal_ptr->SetCapacitanceUnitFromLef();
    }
  }

  if (is_report) {
    int num_of_corners_ = tech_config_.corner_count_;
    for (int i = 0; i < num_of_corners_; ++i) {
      std::cout << "model index: " << i << "\nedge cap, ";
      size_t sz = metal_layers_.size();
      for (size_t j = 0; j < sz; ++j) {
        std::cout << j + 1 << ": "
                  << metal_layers_[j]->unit_edge_cap_[i] << ", ";
      }
      std::cout << "\narea cap, ";
      for (size_t j = 0; j < sz; ++j) {
        std::cout << j + 1 << ": "
                  << metal_layers_[j]->unit_area_cap_[i] << ", ";
      }
      std::cout << "\n";
    }
  }
}

void Tech::ReportLayersTechConfig() {
  for (auto &layer : layers_) {
    if (layer.GetType() == phydb::LayerType::ROUTING) {
      auto tech_config = layer.GetLayerTechConfig();
      if (tech_config != nullptr) {
        tech_config->Report();
      } else {
        std::cout
            << layer.GetName()
            << " does not contain a tech_config\n";
      }
    }
  }
}

void Tech::SetUnitResAndCap(
    double unit_res,
    double unit_fringe_cap,
    double unit_area_cap
) {
  PhyDBExpects(unit_res > 0, "Non positive resistance?");
  PhyDBExpects(unit_fringe_cap >= 0, "Negative fringe capacitance?");
  PhyDBExpects(unit_area_cap >= 0, "Negative area capacitance?");
  for (auto &metal_ptr : metal_layers_) {
    metal_ptr->unit_res_.assign(1, unit_res);
    metal_ptr->unit_edge_cap_.assign(1, unit_fringe_cap);
    metal_ptr->unit_area_cap_.assign(1, unit_area_cap);
  }
}

void Tech::ReportSites() {
  for (auto &site : sites_) {
    std::cout << site << "\n";
  }
}

void Tech::ReportLayers() {
  std::cout << "Total number of layer: " << layers_.size() << "\n";
  for (auto &layer : layers_) {
    layer.Report();
  }
  std::cout << "\n";
}

void Tech::ReportVias() {
  std::cout << "Total number of via: " << vias_.size() << "\n";
  for (auto &via : vias_) {
    via.Report();
  }
  std::cout << "\n";
}

void Tech::ReportMacros() {
  std::cout << "Total number of macro: " << macros_.size() << "\n";
  for (auto &macro : macros_) {
    std::cout << macro << "\n";
  }
  std::cout << "\n";
}

void Tech::ReportMacroWell() {
  std::cout << "SAME_DIFF_SPACING " << same_diff_spacing_ << "\n";
  std::cout << "ANY_DIFF_SPACING " << any_diff_spacing_ << "\n";

  if (p_layer_ptr_ != nullptr) {
    std::cout << "LAYER pwell\n";
    p_layer_ptr_->Report();
  }
  if (n_layer_ptr_ != nullptr) {
    std::cout << "LAYER nwell\n";
    n_layer_ptr_->Report();
  }

  ReportWellShape();
}

void Tech::Report() {
  std::cout << "VERSION " << version_ << ";\n";
  std::cout << "BUSBITCHARS " << bus_bit_char_ << ";\n";
  std::cout << "DIVIDERCHAR " << divier_char_ << ";\n";
  std::cout << "DATABASE MICRONS " << database_micron_ << ";\n";
  std::cout << "MANUFACTURINGGRID " << manufacturing_grid_ << ";\n";
  std::cout << "CLEARANCEMEASURE " << clearance_measure_ << ";\n\n";

  ReportSites();
  ReportLayers();
  ReportVias();
  ReportMacros();
  ReportMacroWell();
}

}
