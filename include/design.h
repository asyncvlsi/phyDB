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

#ifndef PHYDB_INCLUDE_DESIGN_H_
#define PHYDB_INCLUDE_DESIGN_H_

#include <set>
#include <unordered_map>

#include "clustercol.h"
#include "component.h"
#include "defvia.h"
#include "gcellgrid.h"
#include "iopin.h"
#include "logging.h"
#include "net.h"
#include "row.h"
#include "snet.h"
#include "specialmacrorectlayout.h"
#include "tech.h"
#include "track.h"

namespace phydb {

class Design {
 public:
  std::string name_;
  double version_ = -1;
  std::string divider_char_;
  std::string bus_bit_char_;

  Rect2D<int> die_area_;

  int unit_distance_micron_ = -1;
  std::vector<Row> rows_;
  std::vector<Track> tracks_;
  std::vector<Component> components_;
  std::vector<IOPin> iopins_;
  std::vector<SNet> snets_;
  std::vector<Net> nets_;
  std::vector<DefVia> vias_;
  std::vector<ClusterCol> cluster_cols_;
  std::vector<GcellGrid> gcell_grids_;

  std::unordered_map<std::string, int> component_2_id_;
  std::unordered_map<std::string, int> iopin_2_id_;
  std::unordered_map<std::string, int> def_via_2_id_;
  std::unordered_map<std::string, int> layer_name_2_trackid_;
  std::unordered_map<std::string, int> net_2_id_;
  std::unordered_map<std::string, int> snet_2_id_;
  std::unordered_map<std::string, int> via_2_id_;
  std::set<std::string> row_set_;

  /****DEF file name****/
  std::string def_name_;

  /****Nplus/Pplus and N/P-well filling****/
  SpecialMacroRectLayout *plus_filling_ = nullptr;
  SpecialMacroRectLayout *well_filling_ = nullptr;

  Design() = default;
  ~Design();

  void SetName(std::string &name);
  void SetVersion(double version);
  void SetDividerChar(std::string &divider_char);
  void SetBusBitChar(std::string &bus_bit_chars);

  std::string GetName() const;
  double GetVersion() const;
  std::string GetDividerChar() const;
  std::string GetBusBitChar() const;

  void SetUnitsDistanceMicrons(int distance_microns);
  void SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y);

  int GetUnitsDistanceMicrons() const { return unit_distance_micron_; }
  Rect2D<int> GetDieArea() const { return die_area_; }

  Track *AddTrack(
      XYDirection direction,
      int start,
      int num_tracks,
      int step,
      std::vector<std::string> &layer_names
  );
  std::vector<Track> &GetTracksRef();

  bool IsRowExist(std::string &name);
  Row *AddRow(
      std::string &name,
      std::string &site_name,
      std::string &site_orient,
      int origX,
      int origY,
      int numX,
      int numY,
      int stepX,
      int stepY
  );
  std::vector<Row> &GetRowVec();

  void SetComponentCount(int count, double redundancy_factor = 1.4);
  bool IsComponentExisting(std::string &comp_name);
  Component *AddComponent(
      std::string &comp_name,
      Macro *macro_ptr,
      PlaceStatus place_status,
      int llx,
      int lly,
      CompOrient orient
  );
  Component *GetComponentPtr(std::string &comp_name);
  std::vector<Component> &GetComponentsRef() { return components_; }

  bool IsDefViaExist(std::string const &name);
  DefVia *AddDefVia(std::string &name);
  DefVia *GetDefViaPtr(std::string const &name);
  std::vector<DefVia> &GetDefViasRef() { return vias_; }

  void SetIoPinCount(int count);
  bool IsIoPinExist(std::string &iopin_name);
  IOPin *AddIoPin(
      std::string &iopin_name,
      SignalDirection signal_direction,
      SignalUse signal_use
  );
  IOPin *GetIoPinPtr(std::string &iopin_name);
  std::vector<IOPin> &GetIoPinsRef() { return iopins_; }

  void SetNetCount(int count, double redundancy_factor = 1.4);
  bool IsNetExist(std::string &net_name);
  Net *AddNet(std::string &net_name, double weight = 1);
  void AddIoPinToNet(std::string &iopin_name, std::string &net_name);
  void AddCompPinToNet(int comp_id, int pin_id, int net_id);
  Net *GetNetPtr(std::string &net_name);
  std::vector<Net> &GetNetsRef() { return nets_; }

  SNet *AddSNet(std::string &net_name, SignalUse use);
  SNet *GetSNet(std::string &net_name);
  std::vector<SNet> &GetSNetRef();

  ClusterCol *AddClusterCol(std::string &name, std::string &bot_signal);
  std::vector<ClusterCol> &GetClusterColsRef();

  GcellGrid *AddGcellGrid(
      XYDirection direction,
      int start,
      int nBoundaries,
      int step
  );
  std::vector<GcellGrid> &GetGcellGridsRef();
  void InsertRoutingGuide(
      int netID,
      int llx,
      int lly,
      int urx,
      int ury,
      int layerID
  );

  std::string GetDefName() const;
  void SetDefName(std::string const &def_file_name);

  SpecialMacroRectLayout *CreatePpNpMacroAndComponent(
      Macro *macro_ptr,
      int llx,
      int lly,
      int urx,
      int ury
  );
  void SavePpNpToRectFile(std::string &file_name);
  SpecialMacroRectLayout *CreateWellLayerMacroAndComponent(
      Macro *macro_ptr,
      int llx,
      int lly,
      int urx,
      int ury
  );
  void SaveWellToRectFile(std::string &file_name);

  void ReportTracks();
  void ReportRows();
  void ReportComponents();
  void ReportIoPins();
  void ReportNets();
  void ReportSNets();
  void ReportClusterCols();
  void ReportGcellGrids();
  void Report();
};

}

#endif //PHYDB_INCLUDE_DESIGN_H_
