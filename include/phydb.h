#ifndef PHYDB_H
#define PHYDB_H

#include "design.h"
#include "tech.h"
#include "actphydbtimingapi.h"

namespace phydb {

class PhyDB {
  public:
    PhyDB() {}

    Tech *GetTechPtr();
    Design *GetDesignPtr();

    /************************************************
    * The following APIs are for information in LEF
    * ************************************************/
    void SetDatabaseMicron(int database_micron);
    void SetManufacturingGrid(double manufacture_grid);
    void AddSite(std::string const &name, std::string const &class_name, double width, double height);
    void SetPlacementGrids(double placement_grid_value_x_, double placement_grid_value_y_);

    bool IsLayerExisting(std::string const &layer_name);
    Layer *AddLayer(std::string &layer_name, LayerType type, MetalDirection direction = HORIZONTAL);
    Layer *GetLayerPtr(std::string const &layer_name);
    vector<Layer> &GetLayersRef();

    bool IsMacroExisting(std::string const &macro_name);
    Macro *AddMacro(std::string &macro_name);
    Macro *GetMacroPtr(std::string const &macro_name);

    bool IsLefViaExisting(std::string const &name);
    LefVia *AddLefVia(std::string &name);
    LefVia *GetLefViaPtr(std::string const &name);

    /************************************************
    * The following APIs are for information in DEF
    * ************************************************/
    void SetDefName(std::string &name);
    void SetDefVersion(double version);
    void SetDefDividerChar(std::string &divider_char);
    void SetDefBusBitChars(std::string &bus_bit_chars);
    void SetUnitsDistanceMicrons(int distance_microns);
    void SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y);

    Track *AddTrack(XYDirection direction, int start, int nTracks, int step, vector<string> &layer_names);
    vector<Track> &GetTracksRef();

    Row *AddRow(string &name, string &site_name, string &site_orient, int origX, int origY, int numX,
                int numY, int stepX, int stepY);
    vector<Row> &GetRowVec();

    bool IsViaRuleGenerateExisting(std::string const &name);
    ViaRuleGenerate *AddViaRuleGenerate(std::string &name);
    ViaRuleGenerate *GetViaRuleGeneratePtr(std::string const &name);

    bool IsDefViaExisting(std::string const &name);
    DefVia *AddDefVia(std::string &name);
    DefVia *GetDefViaPtr(std::string const &name);

    void SetComponentCount(int count);
    bool IsComponentExisting(std::string &component_name);
    Component *AddComponent(std::string &comp_name, std::string &macro_name, PlaceStatus place_status,
                            int llx, int lly, CompOrient orient);
    Component *GetComponentPtr(std::string &comp_name);

    void SetIoPinCount(int count);
    bool IsIoPinExisting(std::string &iopin_name);
    IOPin *AddIoPin(std::string &iopin_name, SignalDirection signal_direction, SignalUse signal_use);
    IOPin *GetIoPinPtr(std::string &iopin_name);

    void SetNetCount(int count);
    bool IsNetExisting(std::string &net_name);
    Net *AddNet(std::string &net_name, double weight = 1, void *act_net_ptr = nullptr);
    void AddIoPinToNet(std::string &iopin_name, std::string &net_name); // this API does a sanity check
    void AddCompPinToNet(std::string &comp_name,
                         std::string &pin_name,
                         std::string &net_name,
                         void *act_comp_pin_ptr = nullptr); // this API will check sanity
    Net *GetNetPtr(std::string &net_name);

    SNet *AddSNet(std::string &net_name, SignalUse use);
    SNet *GetSNet(std::string &net_name);
    vector<SNet> &GetSNetRef();

    GcellGrid *AddGcellGrid(XYDirection direction, int start, int nBoundaries, int step);
    vector<GcellGrid> &GetGcellGridsRef();

    /************************************************
    * The following APIs are for setting up callback functions for timing-driven flow
    * ************************************************/
    void SetGetNumConstraintsCB(int (*callback_function)());
    void SetUpdateTimingIncremental(void (*callback_function)());
    void SetGetSlackCB(double (*callback_function)(int));
    void SetGetWitnessCB(void (*callback_function)(int, std::vector<ActEdge> &, std::vector<ActEdge> &));
    void SetGetViolatedTimingConstraintsCB(void (*callback_function)(std::vector<int> &));

    /************************************************
    * The following APIs are for information in CELL
    * ************************************************/
    void SetNwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang);
    void SetPwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang);
    void SetNpwellSpacing(double same_spacing, double any_spacing);
    MacroWell *AddMacrowell(std::string &macro_name);

    /************************************************
    * The following APIs are for information in cluster
    * ************************************************/

    ClusterCol *AddClusterCol(string &name, string &bot_signal);
    vector<ClusterCol> &GetClusterColsRef();

    /************************************************
    * The following APIs are for file IO
    * ************************************************/

    void ReadLef(string const &lefFileName);
    void ReadDef(string const &defFileName);
    void ReadCell(string const &cellFileName);
    void ReadCluster(string const &clusterFileName);

    void WriteLef(string const &lefFileName);
    void WriteDef(string const &defFileName);
    void WriteCluster(string const &clusterFileName);

  private:
    Tech tech_;
    Design design_;
    ActPhyDBTimingAPI act_phy_db_timing_api_;

    /****helper functions****/
    // splits a line into many word tokens
    void StrSplit(std::string &line, std::vector<std::string> &res);
};

}

#endif
