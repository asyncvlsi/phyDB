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
#ifndef PHYDB_PHYDB_H_
#define PHYDB_PHYDB_H_

#include <string>
#include <vector>

#include "datatype.h"
#include "design.h"
#include "phydb/timing/actphydbtimingapi.h"
#include "tech.h"

namespace phydb {

class PhyDB {
 public:
  PhyDB() = default;
  ~PhyDB();

  Tech *GetTechPtr();
  Tech &tech();
  Design *GetDesignPtr();
  Design &design();

  /************************************************
   * The following APIs are for information in LEF
   * ************************************************/
  void SetLefVersion(double version);
  void SetDatabaseMicron(int database_micron);
  void SetManufacturingGrid(double manufacture_grid);
  Site *AddSite(std::string const &name, std::string const &class_name,
                double width, double height);
  std::vector<Site> &GetSitesRef();
  Site &GetSite(int id);
  void SetPlacementGrids(double placement_grid_value_x_,
                         double placement_grid_value_y_);

  bool IsLayerExisting(std::string const &layer_name);
  Layer *AddLayer(std::string const &layer_name, LayerType type,
                  MetalDirection direction = MetalDirection::HORIZONTAL);
  Layer *GetLayerPtr(std::string const &layer_name);
  std::vector<Layer> &GetLayersRef();

  bool IsMacroExisting(std::string const &macro_name);
  Macro *AddMacro(std::string const &macro_name);
  Macro *GetMacroPtr(std::string const &macro_name);

  bool IsLefViaExisting(std::string const &name);
  LefVia *AddLefVia(std::string const &name);
  LefVia *GetLefViaPtr(std::string const &name);

  /************************************************
   * The following APIs are for information in DEF
   * ************************************************/
  void SetDefVersion(double version);
  void SetDefDividerChar(std::string const &divider_char);
  void SetDefBusBitChar(std::string const &bus_bit_chars);
  void SetDefName(std::string const &name);

  double GetDefVersion() const;
  std::string GetDefDividerChar() const;
  std::string GetDefBusBitChar() const;
  std::string GetDefName() const;

  void SetUnitsDistanceMicrons(int distance_microns);
  void SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y);
  Rect2D<int> GetDieArea() const;
  void SetRectilinearPolygonDieArea(
      std::vector<Point2D<int>> &rectilinear_polygon_die_area);
  std::vector<Point2D<int>> &RectilinearPolygonDieAreaRef();

  Row *AddRow(std::string const &name, std::string const &site_name,
              std::string const &row_orient, int origX, int origY, int numX,
              int numY, int stepX, int stepY);
  std::vector<Row> &GetRowVec();

  Track *AddTrack(XYDirection direction, int start, int nTracks, int step,
                  std::vector<std::string> &layer_names);
  std::vector<Track> &GetTracksRef();

  GcellGrid *AddGcellGrid(XYDirection direction, int start, int nBoundaries,
                          int step);
  std::vector<GcellGrid> &GetGcellGridsRef();

  bool IsViaRuleGenerateExisting(std::string const &name);
  ViaRuleGenerate *AddViaRuleGenerate(std::string const &name);
  ViaRuleGenerate *GetViaRuleGeneratePtr(std::string const &name);

  bool IsDefViaExisting(std::string const &name);
  DefVia *AddDefVia(std::string const &name);
  DefVia *GetDefViaPtr(std::string const &name);

  void SetComponentCount(int count);
  bool IsComponentExisting(std::string const &component_name);
  Component *AddComponent(std::string const &comp_name, Macro *macro_ptr,
                          PlaceStatus place_status, int llx, int lly,
                          CompOrient orient,
                          CompSource source = CompSource::NETLIST);
  Component *GetComponentPtr(std::string const &comp_name);
  int GetComponentId(std::string const &comp_name);

  void SetIoPinCount(int count);
  bool IsIoPinExisting(std::string const &iopin_name);
  IOPin *AddIoPin(std::string const &iopin_name,
                  SignalDirection signal_direction, SignalUse signal_use);
  IOPin *GetIoPinPtr(std::string const &iopin_name);
  int GetIoPinId(std::string const &iopin_name);

  void SetBlockageCount(int count);
  Blockage *AddBlockage();
  Blockage *GetBlockage(int index);
  std::vector<Blockage> &GetBlockagesRef();

  void SetNetCount(int count);
  bool IsNetExisting(std::string const &net_name);
  Net *AddNet(std::string const &net_name, double weight = 1,
              void *act_net_ptr = nullptr);
  Net *GetNetPtr(std::string const &net_name);
  int GetNetId(std::string const &net_name);
  void AddIoPinToNet(std::string const &io_pin_name,
                     std::string const &net_name,
                     void *act_io_pin_ptr = nullptr);
  void AddCompPinToNet(std::string const &comp_name,
                       std::string const &pin_name, std::string const &net_name,
                       void *act_comp_pin_ptr = nullptr);

  SNet *AddSNet(std::string const &net_name, SignalUse use);
  SNet *GetSNet(std::string const &net_name);
  std::vector<SNet> &GetSNetRef();

  /************************************************
   * The following APIs are for setting up callback functions for timing-driven
   * flow and pointers of parasitic manager, cell libs, adaptor
   * ************************************************/
  void SetGetNumConstraintsCB(int (*callback_function)());
  void SetSpecifyTopKsCB(void (*callback_function)(
      int top_k));        // top-k applied to all correctness constraints
  void SetSpecifyTopKCB(  // top-k for a specific correctness constraint
      void (*callback_function)(int timing_constraint_id, int top_k));
  void SetUpdateTimingIncrementalCB(void (*callback_function)());
  void SetGetSlackCB(std::vector<double> (*callback_function)(
      const std::vector<int> &timing_constraint_ids));
  void SetGetViolatedTimingConstraintsCB(
      void (*callback_function)(std::vector<int> &timing_constraint_ids));
  /* given a timing fork r : a < b
      The fast end paths are paths from node r to node a.
      The slow end paths are from node r to node b.
  */
  void SetGetSlowWitnessCB(void (*callback_function)(
      int timing_constraint_id, std::vector<ActEdge> &path));
  void SetGetFastWitnessCB(void (*callback_function)(
      int timing_constraint_id, std::vector<ActEdge> &path));
  // the following APIs are for performance constraints
  void SetGetNumPerformanceConstraintsCB(int (*callback_function)());
  void SetSpecifyPerformanceTopKsCB(void (*callback_function)(
      int top_k));  // top-k applied to all performance constraints
  void
  SetSpecifyPerformanceTopKCB(  // top-k for a specific performance constraint
      void (*callback_function)(int performance_id, int top_k));
  void SetGetPerformanceConstraintWeightCB(
      double (*callback_function)(int performance_id));
  void SetGetPerformanceSlack(double (*callback_function)(int performance_id));
  void SetGetViolatedPerformanceConstraintsCB(
      void (*callback_function)(std::vector<int> &));
  void SetGetPerformanceWitnessCB(void (*callback_function)(
      int performance_id, std::vector<ActEdge> &path));
  bool IsDriverPin(PhydbPin &phydb_pin);
  std::string GetFullCompPinName(PhydbPin &phydb_pin, char delimiter = ':');
  ActPhyDBTimingAPI &GetTimingApi();
#if PHYDB_USE_GALOIS
  void SetParaManager(galois::eda::parasitics::Manager *manager);
  void AddCellLib(galois::eda::model::CellLib *lib);
  void SetNetlistAdaptor(galois::eda::utility::ExtNetlistAdaptor *adaptor);
  galois::eda::parasitics::Manager *GetParaManager();
  std::vector<galois::eda::model::CellLib *> &GetCellLibs();
  galois::eda::utility::ExtNetlistAdaptor *GetNetlistAdaptor();

  void CreatePhydbActAdaptor();
  void AddNetsAndCompPinsToSpefManager();
#endif

  /************************************************
   * The following APIs are for information in CELL
   * ************************************************/
  void SetNwellLayer(double width, double spacing, double op_spacing,
                     double max_plug_dist, double overhang);
  void SetPwellLayer(double width, double spacing, double op_spacing,
                     double max_plug_dist, double overhang);
  void SetNpwellSpacing(double same_spacing, double any_spacing);
  MacroWell *AddMacrowell(std::string const &macro_name);
  void AddDummyWell(double p_well_height = -1);

  /************************************************
   * The following APIs are for information in clusters,
   * N/P wells, and nplus/pplus layers
   * ************************************************/

  ClusterCol *AddClusterCol(std::string const &name,
                            std::string const &bot_signal);
  std::vector<ClusterCol> &GetClusterColsRef();
  SpecialMacroRectLayout *CreatePpNpMacroAndComponent(int llx, int lly, int urx,
                                                      int ury);
  void SavePpNpToRectFile(std::string const &file_name);
  SpecialMacroRectLayout *CreateWellLayerMacroAndComponent(int llx, int lly,
                                                           int urx, int ury);
  void SaveWellToRectFile(std::string const &file_name);

  /************************************************
   * The following APIs are for file IO
   * ************************************************/

  void ReadLef(std::string const &lef_file_name);
  void ReadDef(std::string const &def_file_name);
  void OverrideComponentLocsFromDef(std::string const &def_file_name);
  void ReadCell(std::string const &cell_file_name);
  void ReadCluster(std::string const &cluster_file_name);
  bool ReadTechConfigFile(std::string const &tech_config_file_name);
  bool ReadTechConfigFile(int argc, char **argv);

  void WriteDef(std::string const &def_file_name);
  void WriteCluster(std::string const &cluster_file_name);
  void WriteGuide(std::string const &guide_file_name);

 private:
  Tech tech_;
  Design design_;
  ActPhyDBTimingAPI timing_api_;

#if PHYDB_USE_GALOIS
  void BindPhydbPinToActPin_(PhydbPin &phydb_pin);
#endif
};

}  // namespace phydb

#endif  // PHYDB_PHYDB_H_
