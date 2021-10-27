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

#include "layer.h"

namespace phydb {

ConfigTable &LayerTechConfigCorner::InitResOverTable(
    int layer_index,
    int over_index
) {
  res_over_.emplace_back(RES_OVER, layer_index, over_index);
  return res_over_.back();
}

ConfigTable &LayerTechConfigCorner::InitCapOverTable(
    int layer_index,
    int over_index
) {
  cap_over_.emplace_back(CAP_OVER, layer_index, over_index);
  return cap_over_.back();
}

ConfigTable &LayerTechConfigCorner::InitCapUnderTable(
    int layer_index,
    int under_index
) {
  cap_under_.emplace_back(CAP_UNDER, layer_index, under_index);
  return cap_under_.back();
}

ConfigTable &LayerTechConfigCorner::InitCapDiagUnderTable(
    int layer_index,
    int diagunder_index
) {
  cap_diagunder_.emplace_back(CAP_DIAGUNDER, layer_index, diagunder_index);
  return cap_diagunder_.back();
}

ConfigTable &LayerTechConfigCorner::InitCapOverUnderTable(
    int layer_index,
    int over_index,
    int under_index
) {
  cap_overunder_.emplace_back(
      CAP_OVERUNDER,
      layer_index,
      over_index,
      under_index
  );
  return cap_overunder_.back();
}

std::vector<ConfigTable> &LayerTechConfigCorner::GetResOverRef() {
  return res_over_;
}

std::vector<ConfigTable> &LayerTechConfigCorner::GetCapOverRef() {
  return cap_over_;
}

std::vector<ConfigTable> &LayerTechConfigCorner::GetCapUnderRef() {
  return cap_under_;
}

std::vector<ConfigTable> &LayerTechConfigCorner::GetCapDiagUnderRef() {
  return cap_diagunder_;
}

std::vector<ConfigTable> &LayerTechConfigCorner::GetCapOverUnderRef() {
  return cap_overunder_;
}

int LayerTechConfigCorner::ModelIndex() const {
  return model_index_;
}

void LayerTechConfigCorner::FixResOverTableLastEntry() {
  for (auto &res_over_table: res_over_) {
    res_over_table.FixLastEntryIfWrong();
  }
}

/****
 * @brief Returns the unit resistance for metal segment over substrate with no
 * surrounding wires.
 *
 * This function calculate the simplified unit resistance in the following way:
 * 1. iterate over all ResOver tables
 * 2. find the first ResOver table with over_index -1 (substrate, 0 used in technology configuration file)
 * 3. return res_ in the first entry as the result
 * 4. if there is no ResOver table satisfies this condition, return -1;
 *
 * @return unit resistance
 */
double LayerTechConfigCorner::GetOverSubstrateNoSurroundingWireRes() {
  double res = -1;
  for (auto &table: res_over_) {
    if (table.Index0() == -1) {
      if (!table.GetTable().empty()) {
        res = table.GetTable()[0].res_;
        break;
      }
    }
  }
  return res;
}

/****
 * @brief Returns the unit capacitance for metal segment over substrate with no
 * surrounding wires.
 *
 * This function calculate the simplified unit fringe capacitance in the following way:
 * 1. iterate over all OVER tables
 * 2. find the first OVER tables with over_index -1
 * 3. return fringe_cap_ in the last entry as the result
 * 4. if there is no OVER table satisfies this condition, return -1;
 *
 * @return unit fringe capacitance
 */
double LayerTechConfigCorner::GetOverSubstrateNoSurroundingWireCap() {
  double res = -1;
  for (auto &table: cap_over_) {
    if (table.Index0() == -1) {
      if (!table.GetTable().empty()) {
        res = table.GetTable().back().fringe_cap_;
        break;
      }
    }
  }
  return res;
}

void LayerTechConfigCorner::Report() {
  for (auto &table: res_over_) {
    table.Report();
  }
  for (auto &table: cap_over_) {
    table.Report();
  }
  for (auto &table: cap_under_) {
    table.Report();
  }
  for (auto &table: cap_diagunder_) {
    table.Report();
  }
  for (auto &table: cap_overunder_) {
    table.Report();
  }
}

void LayerTechConfig::AddCorner(int corner_index) {
  corners_.emplace_back(corner_index);
}

std::vector<LayerTechConfigCorner> &LayerTechConfig::CornersRef() {
  return corners_;
}

LayerTechConfigCorner *LayerTechConfig::GetLastCorner() {
  if (corners_.empty()) {
    return nullptr;
  }
  return &corners_.back();
}

void LayerTechConfig::FixResOverTable() {
  for (auto &corner: corners_) {
    corner.FixResOverTableLastEntry();
  }
}

void LayerTechConfig::Report() {
  for (auto &corner: corners_) {
    corner.Report();
  }
}

Layer::~Layer() {
  delete layer_tech_config_;
}

void Layer::SetName(std::string &name) {
  name_ = name;
}

void Layer::SetType(LayerType type) {
  type_ = type;
}

void Layer::SetID(int id) {
  id_ = id;
}

void Layer::SetDirection(MetalDirection direction) {
  direction_ = direction;
}

void Layer::SetWidth(double width) {
  width_ = width;
}

void Layer::SetMinWidth(double min_width) {
  min_width_ = min_width;
}

void Layer::SetPitch(double pitch_x, double pitch_y) {
  pitchx_ = pitch_x;
  pitchy_ = pitch_y;
}

void Layer::SetOffset(double offset) {
  offset_ = offset;
}

void Layer::SetArea(double area) {
  area_ = area;
}

void Layer::SetSpacing(double spacing) {
  spacing_ = spacing;
}

void Layer::SetCPerSqDist(double cpersqdist) {
  PhyDBExpects(cpersqdist > 0, "Negative capacitance per square distance?");
  capacitance_cpersqdist_ = cpersqdist;
}

void Layer::SetCapMultiplier(double capmultiplier) {
  PhyDBExpects(capmultiplier > 0, "Negative capacitance multiplier?");
  capmultiplier_ = capmultiplier;
}

void Layer::SetEdgeCPerDist(double edgecapacitance) {
  PhyDBExpects(edgecapacitance > 0, "Negative peripheral capacitance?");
  edgecapacitance_ = edgecapacitance;
}

void Layer::SetRPerSqUnit(double rpersq) {
  PhyDBExpects(rpersq > 0, "Negative resistance per square unit?");
  resistance_rpersq_ = rpersq;
}

const std::string &Layer::GetName() {
  return name_;
}

int Layer::GetID() const {
  return id_;
}

LayerType Layer::GetType() const {
  return type_;
}

MetalDirection Layer::GetDirection() const {
  return direction_;
}

double Layer::GetWidth() const {
  return width_;
}

double Layer::GetMinWidth() const {
  return min_width_;
}

void Layer::GetPitch(double &pitch_x, double &pitch_y) const {
  pitch_x = pitchx_;
  pitch_y = pitchy_;
}

double Layer::GetPitchX() const {
  return pitchx_;
}

double Layer::GetPitchY() const {
  return pitchy_;

}

double Layer::GetOffset() const {
  return offset_;
}

double Layer::GetArea() const {
  return area_;
}

double Layer::GetSpacing() const {
  return spacing_;
}

double Layer::GetCPerSqDist() const {
  return capacitance_cpersqdist_;
}

double Layer::GetCapMultiplier() const {
  return capmultiplier_;
}

double Layer::GetEdgeCPerDist() const {
  return edgecapacitance_;
}

double Layer::GetRPerSqUnit() const {
  return resistance_rpersq_;
}

SpacingTable *Layer::SetSpacingTable(SpacingTable &st) {
  spacing_table_ = st;
  return &spacing_table_;
}

SpacingTable *Layer::SetSpacingTable(
    int n_col,
    int n_row,
    const std::vector<double> &v_parallel_run_length,
    const std::vector<double> &v_width,
    const std::vector<double> &v_spacing
) {
  spacing_table_ =
      SpacingTable(n_col, n_row, v_parallel_run_length, v_width, v_spacing);
  return &spacing_table_;
}

SpacingTableInfluence *Layer::AddSpacingTableInfluence(
    double width,
    double within,
    double spacing
) {
  spacing_table_influences_.emplace_back(width, within, spacing);
  return &spacing_table_influences_.back();
}

EolSpacing *Layer::AddEolSpacing(
    double spacing,
    double width,
    double within,
    double par_edge,
    double par_within
) {
  eol_spacings_.emplace_back(spacing, width, within, par_edge, par_within);
  return &eol_spacings_.back();
}

CornerSpacing *Layer::SetCornerSpacing(CornerSpacing &cornerSpacing) {
  corner_spacing_ = cornerSpacing;
  return &corner_spacing_;
}

AdjacentCutSpacing *Layer::SetAdjCutSpacing(
    double spacing,
    int adjacent_cuts,
    int cut_within
) {
  adjacent_cut_spacing_ =
      AdjacentCutSpacing(spacing, adjacent_cuts, cut_within);
  return &adjacent_cut_spacing_;
}

SpacingTable *Layer::GetSpacingTable() {
  return &spacing_table_;
}

std::vector<EolSpacing> *Layer::GetEolSpacings() {
  return &eol_spacings_;
}

std::vector<SpacingTableInfluence> *Layer::GetSpacingTableInfluences() {
  return &spacing_table_influences_;
}

CornerSpacing *Layer::GetCornerSpacing() {
  return &corner_spacing_;
}

AdjacentCutSpacing *Layer::GetAdjCutSpacing() {
  return &adjacent_cut_spacing_;
}

void Layer::InitLayerTechConfig() {
  layer_tech_config_ = new LayerTechConfig;
}

LayerTechConfig *Layer::GetLayerTechConfig() {
  return layer_tech_config_;
}

void Layer::AddTechConfigCorner(int corner_index) {
  if (layer_tech_config_ == nullptr) {
    InitLayerTechConfig();
  }
  layer_tech_config_->AddCorner(corner_index);
}

/****
 * @brief This function computes a optimized/simplified resistance table for
 * resistance extraction.
 *
 * This function is implemented based on `extMain::getResCapTable(...)` and
 * `extMain::calcRes0(...)` in OpenRCX.
 * The basic goal of the above two functions is to construct a simpler resistance
 * table to extract resistance, because the `ResOver` table may contain duplicated
 * or redundant information.
 *
 * @param
 *
 * @return nothing
 */
void Layer::SetResistanceUnitFromTechConfig() {
  PhyDBExpects(layer_tech_config_ != nullptr,
               "Cannot find RC extraction parameters from technology configuration file: "
                   + name_);
  size_t number_of_corners = layer_tech_config_->CornersRef().size();
  unit_res_.assign(number_of_corners, 0);

  for (size_t i = 0; i < number_of_corners; ++i) {
    auto corner = layer_tech_config_->CornersRef()[i];
    double unit_res = corner.GetOverSubstrateNoSurroundingWireRes();
    PhyDBExpects(unit_res >= 0,
                 "Cannot find unit resistance for corner: " + name_ + " "
                     + std::to_string(corner.ModelIndex()));
    unit_res_[i] = unit_res * width_;
  }
}

void Layer::SetResistanceUnitFromLef() {
  PhyDBWarns(resistance_rpersq_ <= 0,
             "resistance_rpersq_ not set: " + name_);
  unit_res_.assign(1, resistance_rpersq_);
}

/****
 * @brief This function computes a optimized/simplified fringe capacitance table
 * for fringe capacitance extraction.
 *
 * This function is implemented based on `extMain::getShapeRC`, `extMain::getResCapTable(...)`,
 * `extMetRCTable::getOverFringeRC(...)`, and `extDistWidthRCTable::getFringeRC`
 * in OpenRCX. In these functions, the capacitance from the area of the metal segment
 * is set to 0, one possible reason is that the metal segment also has the same width,
 * so the area capacitance can be absorb into the fringe capacitance
 *     fringe_capacitance = fringe_capacitance_factor * 2 * length (+width ?)
 * it seems width is ignored from the above functions.
 * And area capacitance is
 *     area_capacitance = area_capacitance_factor * length * width
 * because we assume the above width is fixed, so we can have
 *     area_capacitance = area_capacitance_factor' * length
 * and then we can put the area capacitance into the fringe capacitance.
 *
 * @param
 *
 * @return nothing
 */
void Layer::SetCapacitanceUnitFromTechConfig() {
  PhyDBExpects(layer_tech_config_ != nullptr,
               "Cannot find RC extraction parameters from technology configuration file: "
                   + name_);
  size_t number_of_corners = layer_tech_config_->CornersRef().size();
  unit_area_cap_.assign(number_of_corners, 0);
  unit_edge_cap_.assign(number_of_corners, 0);

  for (size_t i = 0; i < number_of_corners; ++i) {
    auto corner = layer_tech_config_->CornersRef()[i];
    double unit_cap = corner.GetOverSubstrateNoSurroundingWireCap();
    PhyDBExpects(unit_cap >= 0,
                 "Cannot find unit capacitance for corner: " + name_ + " "
                     + std::to_string(corner.ModelIndex()));
    unit_edge_cap_[i] = unit_cap;
  }
}

void Layer::SetCapacitanceUnitFromLef() {
  PhyDBWarns(capacitance_cpersqdist_ <= 0,
             "capacitance_cpersqdist_ not set: " + name_);
  PhyDBWarns(edgecapacitance_ <= 0,
             "edgecapacitance_ not set: " + name_);
  unit_area_cap_.assign(1, capacitance_cpersqdist_ * capmultiplier_);
  unit_edge_cap_.assign(1, edgecapacitance_ * capmultiplier_);
}

/****
 * @brief Returns the resistance of a metal segment.
 *
 * The resistance is computed using the following formula:
 *     r = resistance_factor * length / width
 * where resistance_factor is from the technology configuration file.
 *
 * @param width: width of the metal segment
 * @param length: length of the metal segment
 * @param corner_index: the index of corner
 * @return resistance
 */
double Layer::GetResistance(
    double width,
    double length,
    int corner_index
) {
  assert(corner_index < (int) unit_res_.size());
  //std::cout << length << " " << width << "\n";
  return unit_res_[corner_index] * length / width;
}

/****
 * @brief Returns the area capacitance of a metal segment.
 *
 * The fringe capacitance is computed using the following formula:
 *     c = area_capacitance_factor * width * length
 *
 * Important: this area capacitance from technology configuration file is 0
 *
 * @param width: width of the metal segment, width is not used in this function for now
 * @param length: length of the metal segment
 *  * @param corner_index: the index of corner
 * @return fringe capacitance
 */
double Layer::GetAreaCapacitance(
    double width,
    double length,
    int corner_index
) {
  assert(corner_index < (int) unit_area_cap_.size());
  return unit_area_cap_[corner_index] * width * length;
}

/****
 * @brief Returns the fringe capacitance of a metal segment.
 *
 * The fringe capacitance is computed using the following formula:
 *     c = fringe_capacitance_factor * 2 * length
 * where fringe_capacitance_factor is from the technology configuration file.
 *
 *
 * @param width: width of the metal segment, width is not used in this function for now
 * @param length: length of the metal segment
 * @return fringe capacitance
 */
double Layer::GetFringeCapacitance(
    double width,
    double length,
    int corner_index
) {
  assert(corner_index < (int) unit_edge_cap_.size());
  //std::cout << length << " " << width << "\n";
  return unit_edge_cap_[corner_index] * 2 * (width + length);
}

std::ostream &operator<<(std::ostream &os, const Layer &l) {
  os << l.name_ << " " << LayerTypeStr(l.type_) << " "
     << l.id_ << " " << MetalDirectionStr(l.direction_) << std::endl;
  os << l.pitchx_ << " " << l.pitchy_ << " " << l.width_ << " " << l.area_
     << std::endl;
  os << l.min_width_ << " " << l.offset_ << std::endl;
  os << l.spacing_ << std::endl;
  return os;
}

void Layer::Report() {
  std::cout << "------------------------------" << std::endl;
  std::cout << "Layer: " << name_ << " type: " << LayerTypeStr(type_)
            << " direction: " << MetalDirectionStr(direction_) << " idx_: "
            << id_ << std::endl;
  std::cout << "pitch: " << pitchx_ << " " << pitchy_ << " Width:" << width_
            << " area: " << area_ << std::endl;
  std::cout << "minWidth: " << min_width_ << " offset: " << offset_
            << " GetSpacing: " << spacing_ << std::endl;
  /*
  if (spacingTable.parallelRunLength.size())
      spacingTable.print();
  for (auto spacing : spacings) {
      GetSpacing.print();
  }
  if (cornerSpacing.GetWidth.size())
      cornerSpacing.print();
  */
}

double WellLayer::GetWidth() const {
  return width_;
}

double WellLayer::GetSpacing() const {
  return spacing_;
}

double WellLayer::GetOpSpacing() const {
  return op_spacing_;
}

double WellLayer::GetMaxPlugDist() const {
  return max_plug_dist_;
}

double WellLayer::GetOverhang() const {
  return overhang_;
}

void WellLayer::SetWidth(double width) {
  PhyDBExpects(width >= 0,
               "Negative Width not allowed: WellLayer::SetWidth()");
  width_ = width;
}

void WellLayer::SetSpacing(double spacing) {
  PhyDBExpects(spacing >= 0,
               "Negative GetSpacing not allowed: WellLayer::SetSpacing()");
  spacing_ = spacing;
}

void WellLayer::SetOpSpacing(double op_spacing) {
  PhyDBExpects(op_spacing >= 0,
               "Negative opposite GetSpacing not allowed: Layer::SetOpSpacing()");
  op_spacing_ = op_spacing;
}

void WellLayer::SetMaxPlugDist(double max_plug_dist) {
  PhyDBExpects(max_plug_dist >= 0,
               "Negative max plug distance not allowed: WellLayer::SetMaxPlugDist()");
  max_plug_dist_ = max_plug_dist;
}

void WellLayer::SetOverhang(double overhang) {
  PhyDBExpects(overhang >= 0,
               "Negative well/diffusion GetOverhang not allowed: WellLayer::SetOverhang()");
  overhang_ = overhang;
}

void WellLayer::SetParams(
    double width,
    double spacing,
    double op_spacing,
    double max_plug_dist,
    double overhang
) {
  SetWidth(width);
  SetSpacing(spacing);
  SetOpSpacing(op_spacing);
  SetMaxPlugDist(max_plug_dist);
  SetOverhang(overhang);
}

void WellLayer::Report() const {
  std::cout
      << "    MINWIDTH:    " << width_ << " um\n"
      << "    SPACING:     " << spacing_ << " um\n"
      << "    OPPOSPACING: " << op_spacing_ << " um\n"
      << "    MAXPLUGDIST: " << max_plug_dist_ << " um\n"
      << "    OVERHANG:    " << overhang_ << " um\n";
}

}
