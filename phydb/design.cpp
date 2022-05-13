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
#include "design.h"

#include <cmath>

namespace phydb {

Design::~Design() {
  delete plus_filling_;
  delete well_filling_;
}

void Design::SetName(std::string const &name) {
  name_ = name;
}

void Design::SetVersion(double version) {
  version_ = version;
}
void Design::SetDividerChar(std::string const &divider_char) {
  divider_char_ = divider_char;
}

void Design::SetBusBitChar(std::string const &bus_bit_chars) {
  bus_bit_char_ = bus_bit_chars;
}

std::string Design::GetName() const {
  return name_;
}

double Design::GetVersion() const {
  return version_;
}

std::string Design::GetDividerChar() const {
  return divider_char_;
}

std::string Design::GetBusBitChar() const {
  return bus_bit_char_;
}

void Design::SetUnitsDistanceMicrons(int distance_microns) {
  PhyDBExpects(distance_microns > 0, "Negative distance micron?");
  unit_distance_micron_ = distance_microns;
}

void Design::SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y) {
  PhyDBExpects(upper_x > lower_x,
               "Right boundary is not larger than Left boundary?");
  PhyDBExpects(upper_y > lower_y,
               "Top boundary is not larger than Bottom boundary?");
  die_area_.ll.x = lower_x;
  die_area_.ll.y = lower_y;
  die_area_.ur.x = upper_x;
  die_area_.ur.y = upper_y;
}

Track *Design::AddTrack(
    XYDirection direction,
    int start,
    int num_tracks,
    int step,
    std::vector<std::string> &layer_names
) {
  tracks_.emplace_back(direction, start, num_tracks, step, layer_names);
  return &(tracks_.back());
}

std::vector<Track> &Design::GetTracksRef() {
  return tracks_;
}

bool Design::IsRowExisting(std::string const &row_name) {
  return (row_set_.find(row_name) != row_set_.end());
}

Row *Design::AddRow(
    std::string const &name,
    int site_id,
    CompOrient orient,
    int origX,
    int origY,
    int numX,
    int numY,
    int stepX,
    int stepY
) {
  size_t id = rows_.size();
  PhyDBExpects(
      !IsRowExisting(name),
      name + " row name_ exists, cannot use it again"
  );
  rows_.emplace_back(
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
  return &(rows_[id]);
}

std::vector<Row> &Design::GetRowVec() {
  return rows_;
}

void Design::SetComponentCount(int count, double redundancy_factor) {
  // redundancy_factor for well tap cells and buffer insertion
  if (redundancy_factor < 1) redundancy_factor = 1;
  int actual_count = (int) std::ceil(count * redundancy_factor);
  components_.reserve(actual_count);
}

bool Design::IsComponentExisting(std::string const &comp_name) {
  return component_2_id_.find(comp_name) != component_2_id_.end();
}

Component *Design::AddComponent(
    std::string const &comp_name,
    Macro *macro_ptr,
    PlaceStatus place_status,
    int llx,
    int lly,
    CompOrient orient,
    CompSource source
) {
  PhyDBExpects(
      !IsComponentExisting(comp_name),
      "Component name_ exists, cannot use it again"
  );
  int id = static_cast<int>(components_.size());
  components_.emplace_back(
      id, comp_name,
      macro_ptr,
      source,
      place_status,
      llx,
      lly,
      orient
  );
  component_2_id_[comp_name] = id;
  return &(components_[id]);
}

Component *Design::GetComponentPtr(std::string const &comp_name) {
  if (!IsComponentExisting(comp_name)) {
    return nullptr;
  }
  int id = component_2_id_[comp_name];
  return &(components_[id]);
}

int Design::GetComponentId(std::string const &comp_name) {
  auto res = component_2_id_.find(comp_name);
  PhyDBExpects(
      res != component_2_id_.end(),
      "Component does not exist: " << comp_name
  );
  return res->second;
}

bool Design::IsDefViaExisting(std::string const &name) {
  return def_via_2_id_.find(name) != def_via_2_id_.end();
}

DefVia *Design::AddDefVia(std::string const &via_name) {
  PhyDBExpects(!IsDefViaExisting(via_name),
               "Macro name_ exists, cannot use it again");
  int id = (int) vias_.size();
  vias_.emplace_back(via_name);
  via_2_id_[via_name] = id;
  return &(vias_[id]);
}

DefVia *Design::GetDefViaPtr(std::string const &via_name) {
  if (!IsDefViaExisting(via_name)) {
    return nullptr;
  }
  int id = via_2_id_[via_name];
  return &(vias_[id]);
}

void Design::SetIoPinCount(int count) {
  iopins_.reserve(count);
}

bool Design::IsIoPinExisting(std::string const &iopin_name) {
  return iopin_2_id_.find(iopin_name) != iopin_2_id_.end();
}

IOPin *Design::AddIoPin(
    std::string const &iopin_name,
    SignalDirection signal_direction,
    SignalUse signal_use
) {
  PhyDBExpects(!IsIoPinExisting(iopin_name),
               "IOPin name_ exists, cannot use it again");
  int id = (int) iopins_.size();
  iopins_.emplace_back(iopin_name, signal_direction, signal_use);
  iopin_2_id_[iopin_name] = id;
  return &(iopins_[id]);
}

IOPin *Design::GetIoPinPtr(std::string const &iopin_name) {
  if (!IsIoPinExisting(iopin_name)) {
    return nullptr;
  }
  int id = iopin_2_id_[iopin_name];
  return &(iopins_[id]);
}

int Design::GetIoPinId(std::string const &iopin_name) {
  auto res = iopin_2_id_.find(iopin_name);
  PhyDBExpects(
      res != iopin_2_id_.end(),
      "IO pin does not exist: " << iopin_name
  );
  return res->second;
}

void Design::SetBlockageCount(int count) {
  blockages_.reserve(count);
}

Blockage *Design::AddBlockage() {
  blockages_.emplace_back();
  return &(blockages_.back());
}

Blockage *Design::GetBlockage(int index) {
  if (static_cast<int>(blockages_.size()) > index) {
    return nullptr;
  }
  return &blockages_[index];
}

std::vector<Blockage> &Design::GetBlockagesRef() {
  return blockages_;
}

void Design::SetNetCount(
    int count,
    double redundancy_factor
) { // redundancy_factor for buffer insertion
  if (redundancy_factor < 1) redundancy_factor = 1;
  int actual_count = (int) std::ceil(count * redundancy_factor);
  nets_.reserve(actual_count);
}

bool Design::IsNetExisting(std::string const &net_name) {
  return net_2_id_.find(net_name) != net_2_id_.end();
}

Net *Design::AddNet(std::string const &net_name, double weight) {
  PhyDBExpects(!IsNetExisting(net_name),
               "Net name exists, cannot use it again");
  int id = (int) nets_.size();
  nets_.emplace_back(net_name, weight);
  net_2_id_[net_name] = id;
  return &(nets_[id]);
}

void Design::AddIoPinToNet(int iopin_id, int net_id) {
  PhyDBExpects(
      (iopin_id < static_cast<int>(iopins_.size())) && (iopin_id >= 0),
      "iopin id out of bound: " << iopin_id
  );
  PhyDBExpects(
      (net_id < static_cast<int>(nets_.size())) && (net_id >= 0),
      "net id out of bound: " << net_id
  );
  iopins_[iopin_id].SetNetId(net_id);
  nets_[net_id].AddIoPin(iopin_id);
}

void Design::AddCompPinToNet(int comp_id, int pin_id, int net_id) {
  PhyDBExpects(
      (net_id < static_cast<int>(nets_.size())) && (net_id >= 0),
      "net id out of bound: " << net_id
  );
  nets_[net_id].AddCompPin(comp_id, pin_id);
}

Net *Design::GetNetPtr(std::string const &net_name) {
  if (!IsNetExisting(net_name)) {
    return nullptr;
  }
  int id = net_2_id_[net_name];
  return &(nets_[id]);
}

int Design::GetNetId(std::string const &net_name) {
  auto res = net_2_id_.find(net_name);
  if (res == net_2_id_.end()) {
    PhyDBExpects(false, "Net does not exist: " << net_name);
  }
  return res->second;
}

SNet *Design::AddSNet(std::string const &net_name, SignalUse use) {
  bool e = (use == phydb::SignalUse::GROUND || use == phydb::SignalUse::POWER);
  PhyDBExpects(e, "special net use should be POWER or GROUND");
  int id = (int) snets_.size();
  snets_.emplace_back(net_name, use);
  snet_2_id_[net_name] = id;
  return &snets_[id];
}

SNet *Design::GetSNet(std::string const &net_name) {
  bool e = (snet_2_id_.find(net_name) != snet_2_id_.end());
  PhyDBExpects(e, "snet is not found");
  return &snets_[snet_2_id_[net_name]];
}

std::vector<SNet> &Design::GetSNetRef() {
  return snets_;
}

ClusterCol *Design::AddClusterCol(
    std::string const &name,
    std::string const &bot_signal
) {
  int id = (int) cluster_cols_.size();
  cluster_cols_.emplace_back(name, bot_signal);
  return &cluster_cols_[id];
}

std::vector<ClusterCol> &Design::GetClusterColsRef() {
  return cluster_cols_;
}

GcellGrid *Design::AddGcellGrid(
    XYDirection direction,
    int start,
    int nBoundaries,
    int step
) {
  int id = (int) gcell_grids_.size();
  gcell_grids_.emplace_back(direction, start, nBoundaries, step);
  return &gcell_grids_[id];
}

std::vector<GcellGrid> &Design::GetGcellGridsRef() {
  return gcell_grids_;
}

void Design::InsertRoutingGuide(
    int netID,
    int llx,
    int lly,
    int urx,
    int ury,
    int layerID
) {
  this->nets_[netID].AddRoutingGuide(llx, lly, urx, ury, layerID);
}

std::string Design::GetDefName() const {
  return def_name_;
}

void Design::SetDefName(std::string const &def_file_name) {
  def_name_ = def_file_name;
}

SpecialMacroRectLayout *Design::CreatePpNpMacroAndComponent(
    Macro *macro_ptr,
    int llx,
    int lly,
    int urx,
    int ury
) {
  delete plus_filling_;
  plus_filling_ = new SpecialMacroRectLayout(
      macro_ptr,
      llx,
      lly,
      urx,
      ury
  );
  return plus_filling_;
}

void Design::SavePpNpToRectFile(std::string const &file_name) const {
  if (plus_filling_ != nullptr) {
    plus_filling_->SaveToRectFile(file_name);
  }
}

SpecialMacroRectLayout *Design::CreateWellLayerMacroAndComponent(
    Macro *macro_ptr,
    int llx,
    int lly,
    int urx,
    int ury
) {
  delete well_filling_;
  well_filling_ = new SpecialMacroRectLayout(
      macro_ptr,
      llx,
      lly,
      urx,
      ury
  );
  return well_filling_;
}

void Design::SaveWellToRectFile(std::string const &file_name) const {
  if (well_filling_ != nullptr) {
    well_filling_->SaveToRectFile(file_name);
  }
}

/****
 * @brief Get the center of the bounding box of the component pin.
 * Every component pin consists of metal segments, whose shape may or may not be
 * regular. This method returns the center of the bounding box as its location
 * as an approximation.
 *
 * @param comp_id: index of the component
 * @param pin_id: index of the pin
 * @return the location of this component pin, in manufacturing grid unit.
 */
Point2D<int> Design::GetComponentPinLocation(int comp_id, int pin_id) {
  Component &comp = components_[comp_id];
  Macro &macro = *(comp.GetMacro());
  Pin &pin = macro.GetPinsRef()[pin_id];

  Rect2D<double> bbox = pin.GetBoundingBox();
  Point2D<double> bb_center(
      (bbox.LLX() + bbox.URX()) / 2.0,
      (bbox.LLY() + bbox.URY()) / 2.0
  );
  bb_center.Rotate(comp.GetOrientation(), macro.GetWidth(), macro.GetHeight());

  Point2D<int> comp_loc = comp.GetLocation();
  Point2D<int> res;
  res.x = comp_loc.x
      + static_cast<int>(std::round(bb_center.x * GetUnitsDistanceMicrons()));
  res.y = comp_loc.y
      + static_cast<int>(std::round(bb_center.y * GetUnitsDistanceMicrons()));
  return res;
}

/****
 * @brief Get the location of an I/O pin.
 *
 * @param iopin_id: index of this IO pin
 * @return the location of this component pin, in manufacturing grid unit.
 */
Point2D<int> Design::GetIoPinLocation(int iopin_id) {
  IOPin &iopin = iopins_[iopin_id];
  return iopin.GetLocation();
}

void Design::ReportTracks() {
  std::cout << "Total number of track: " << tracks_.size() << "\n";
  for (auto &track : tracks_) {
    std::cout << track << "\n";
  }
  std::cout << "\n";
}

void Design::ReportRows() {
  std::cout << "Total number of row: " << rows_.size() << "\n";
  for (auto &row : rows_) {
    std::cout << row << "\n";
  }
  std::cout << "\n";
}

void Design::ReportComponents() {
  std::cout << "Total component: " << components_.size() << "\n";
  for (auto &component : components_) {
    std::cout << component << "\n";
  }
  std::cout << "\n";
}

void Design::ReportIoPins() {
  std::cout << "Total iopin: " << iopins_.size() << "\n";
  for (auto &iopin : iopins_) {
    iopin.Report();
  }
  std::cout << "\n";
}

void Design::ReportBlockages() {
  std::cout << "Total blockages: " << blockages_.size() << "\n";
  for (auto &blockage : blockages_) {
    blockage.Report();
  }
  std::cout << "\n";
}

void Design::ReportNets() {
  std::cout << "Total net: " << nets_.size() << "\n";
  for (auto &net : nets_) {
    net.Report();
  }
  std::cout << "\n";
}

void Design::ReportClusterCols() {
  std::cout << "Total cluster cols: " << cluster_cols_.size() << "\n";
  for (auto &c : cluster_cols_) {
    c.Report();
  }
  std::cout << "\n";
}

void Design::ReportGcellGrids() {
  std::cout << "Total GcellGrids: " << gcell_grids_.size() << "\n";
  for (auto &g : gcell_grids_) {
    g.Report();
  }
  std::cout << "\n";
}

void Design::ReportSNets() {
  std::cout << "Total SNets: " << snets_.size() << "\n";
  for (auto &n : snets_) {
    n.Report();
  }
  std::cout << "\n";
}

void Design::Report() {
  std::cout << "VERSION " << version_ << ";\n";
  std::cout << "BUSBITCHARS " << bus_bit_char_ << ";\n";
  std::cout << "DIVIDERCHAR " << divider_char_ << ";\n";
  std::cout << "UNITS DISTANCE MICRONS " << unit_distance_micron_ << ";\n";
  std::cout << "DIE AREA " << die_area_.Str() << "\n";

  //ReportTracks();
  //ReportRows(); // TODO : rows not loaded
  //ReportComponents();
  //ReportIoPins();
  ReportBlockages();
  //ReportNets();
  //ReportClusterCols();
  //ReportGcellGrids();
  //ReportSNets();
}

}
