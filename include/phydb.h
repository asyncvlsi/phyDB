#ifndef PHYDB_INCLUDE_PHYDB_H_
#define PHYDB_INCLUDE_PHYDB_H_

#include "actphydbtimingapi.h"
#include "design.h"
#include "rcestimator.h"
#include "tech.h"

namespace phydb {

class RCEstimator;

class PhyDB {
  public:
    PhyDB() = default;
    ~PhyDB();

    Tech *GetTechPtr();
    Design *GetDesignPtr();

    /************************************************
    * The following APIs are for information in LEF
    * ************************************************/
    void SetDatabaseMicron(int database_micron);
    void SetManufacturingGrid(double manufacture_grid);
    void AddSite(
        std::string const &name,
        std::string const &class_name,
        double width,
        double height
    );
    void SetPlacementGrids(
        double placement_grid_value_x_,
        double placement_grid_value_y_
    );

    bool IsLayerExisting(std::string const &layer_name);
    Layer *AddLayer(
        std::string &layer_name,
        LayerType type,
        MetalDirection direction = HORIZONTAL
    );
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
    void SetDefBusBitChar(std::string &bus_bit_chars);

    std::string GetDefName() const;
    double GetDefVersion() const;
    std::string GetDefDividerChar() const;
    std::string GetDefBusBitChar() const;

    void SetUnitsDistanceMicrons(int distance_microns);
    void SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y);
    Rect2D<int> GetDieArea() const;

    Track *AddTrack(
        XYDirection direction,
        int start,
        int nTracks,
        int step,
        vector<string> &layer_names
    );
    vector<Track> &GetTracksRef();

    Row *AddRow(
        string &name,
        string &site_name,
        string &site_orient,
        int origX,
        int origY,
        int numX,
        int numY,
        int stepX,
        int stepY
    );
    vector<Row> &GetRowVec();

    bool IsViaRuleGenerateExisting(std::string const &name);
    ViaRuleGenerate *AddViaRuleGenerate(std::string &name);
    ViaRuleGenerate *GetViaRuleGeneratePtr(std::string const &name);

    bool IsDefViaExisting(std::string const &name);
    DefVia *AddDefVia(std::string &name);
    DefVia *GetDefViaPtr(std::string const &name);

    void SetComponentCount(int count);
    bool IsComponentExisting(std::string &component_name);
    Component *AddComponent(
        std::string &comp_name,
        Macro *macro_ptr,
        PlaceStatus place_status,
        int llx,
        int lly,
        CompOrient orient
    );
    Component *GetComponentPtr(std::string &comp_name);
    int GetComponentId(std::string &comp_name);

    void SetIoPinCount(int count);
    bool IsIoPinExisting(std::string &iopin_name);
    IOPin *AddIoPin(
        std::string &iopin_name,
        SignalDirection signal_direction,
        SignalUse signal_use
    );
    IOPin *GetIoPinPtr(std::string &iopin_name);

    void SetNetCount(int count);
    bool IsNetExisting(std::string &net_name);
    Net *AddNet(
        std::string &net_name,
        double weight = 1,
        void *act_net_ptr = nullptr
    );
    Net *GetNetPtr(std::string &net_name);
    int GetNetId(std::string &net_name);
    void AddIoPinToNet(
        std::string &iopin_name,
        std::string &net_name,
        void *act_io_pin_ptr = nullptr
    );
    void AddCompPinToNet(
        std::string &comp_name,
        std::string &pin_name,
        std::string &net_name
    );
    void BindPhydbPinToActPin(
        std::string &comp_name,
        std::string &pin_name,
        void *act_comp_pin_ptr
    );
    void AddCompPinToNetWithActPtr(
        std::string &comp_name,
        std::string &pin_name,
        std::string &net_name,
        void *act_comp_pin_ptr = nullptr
    );

    SNet *AddSNet(std::string &net_name, SignalUse use);
    SNet *GetSNet(std::string &net_name);
    vector<SNet> &GetSNetRef();

    GcellGrid *AddGcellGrid(
        XYDirection direction,
        int start,
        int nBoundaries,
        int step
    );
    vector<GcellGrid> &GetGcellGridsRef();

    /************************************************
    * The following APIs are for setting up callback functions for timing-driven flow
    * and pointers of parasitic manager, cell libs, adaptor
    * ************************************************/
    void SetGetNumConstraintsCB(int (*callback_function)());
    void SetUpdateTimingIncrementalCB(void (*callback_function)());
    void SetGetSlackCB(
        std::vector<double> (*callback_function)(const std::vector<int> &)
    );
    void SetGetWitnessCB(
        void (*callback_function)(
            int,
            std::vector<ActEdge> &,
            std::vector<ActEdge> &
        )
    );
    void SetGetViolatedTimingConstraintsCB(
        void (*callback_function)(std::vector<int> &)
    );
    bool IsDriverPin(PhydbPin &phydb_pin);
    ActPhyDBTimingAPI &GetTimingApi();
#if PHYDB_USE_GALOIS
    void SetParaManager(galois::eda::parasitics::Manager *manager);
    void AddCellLib(galois::eda::liberty::CellLib *lib);
    void SetNetlistAdaptor(galois::eda::utility::ExtNetlistAdaptor *adaptor);
    galois::eda::parasitics::Manager *GetParaManager();
    std::vector<galois::eda::liberty::CellLib *> GetCellLibs();
    galois::eda::utility::ExtNetlistAdaptor *GetNetlistAdaptor();

    void CreatePhydbActAdaptor();
    void AddNetsAndCompPinsToSpefManager();
#endif

    /************************************************
    * The following APIs are for information in CELL
    * ************************************************/
    void SetNwellLayer(
        double width,
        double spacing,
        double op_spacing,
        double max_plug_dist,
        double overhang
    );
    void SetPwellLayer(
        double width,
        double spacing,
        double op_spacing,
        double max_plug_dist,
        double overhang
    );
    void SetNpwellSpacing(double same_spacing, double any_spacing);
    MacroWell *AddMacrowell(std::string &macro_name);

    /************************************************
    * The following APIs are for information in clusters,
    * N/P wells, and nplus/pplus layers
    * ************************************************/

    ClusterCol *AddClusterCol(string &name, string &bot_signal);
    vector<ClusterCol> &GetClusterColsRef();
    SpecialMacroRectLayout *CreatePpNpMacroAndComponent(
        int llx,
        int lly,
        int urx,
        int ury
    );
    void SavePpNpToRectFile(std::string &file_name);
    SpecialMacroRectLayout *CreateWellLayerMacroAndComponent(
        int llx,
        int lly,
        int urx,
        int ury
    );
    void SaveWellToRectFile(std::string &file_name);

    /************************************************
    * The following APIs are for file IO
    * ************************************************/

    void ReadLef(string const &lef_file_name);
    void ReadDef(string const &def_file_name);
    void OverrideComponentLocsFromDef(string const &def_file_name);
    void ReadCell(string const &cell_file_name);
    void ReadCluster(string const &cluster_file_name);
    void LoadFakeTechConfigFile();
    bool ReadTechConfigFile(string const &tech_config_file_name);
    bool ReadTechConfigFile(int argc, char **argv);

    void WriteDef(string const &def_file_name);
    void WriteCluster(string const &cluster_file_name);
    void WriteGuide(string const &guide_file_name);

  private:
    Tech tech_;
    Design design_;
    ActPhyDBTimingAPI timing_api_;

    RCEstimator *rc_estimator_ = nullptr;

    /************************************************
    * The following APIs are for RC extraction and timing analysis
    * ************************************************/
    void InitializeRCEstimator(RCEstimatorType rc_estimator_type);
    void PushNetRCToManager(int net_id);
};

}

#endif //PHYDB_INCLUDE_PHYDB_H_
