#ifndef DESIGN_H
#define DESIGN_H

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
    string name_;
    double version_ = -1;
    string divider_char_;
    string bus_bit_char_;

    Rect2D<int> die_area_;

    int unit_distance_micron_ = -1;
    vector<Row> rows_;
    vector<Track> tracks_;
    vector<Component> components_;
    vector<IOPin> iopins_;
    vector<SNet> snets_;
    vector<Net> nets_;
    vector<DefVia> vias_;
    vector<ClusterCol> cluster_cols_;
    vector<GcellGrid> gcell_grids_;

    unordered_map<string, int> component_2_id_;
    unordered_map<string, int> iopin_2_id_;
    unordered_map<string, int> def_via_2_id_;
    unordered_map<string, int> layer_name_2_trackid_;
    unordered_map<string, int> net_2_id_;
    unordered_map<string, int> snet_2_id_;
    unordered_map<string, int> via_2_id_;
    set<string> row_set_;

    /****DEF file name****/
    std::string def_name_;

    /****Nplus/Pplus and N/P-well filling****/
    SpecialMacroRectLayout *plus_filling_ = nullptr;
    SpecialMacroRectLayout *well_filling_ = nullptr;

    Design() {}

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

    int GetUnitsDistanceMicrons() { return unit_distance_micron_; }
    Rect2D<int> GetDieArea() const { return die_area_; }

    Track *AddTrack(XYDirection direction,
                    int start,
                    int num_tracks,
                    int step,
                    vector<string> &layer_names);
    vector<Track> &GetTracksRef();

    bool IsRowExist(string &name);
    Row *AddRow(string &name,
                string &site_name,
                string &site_orient,
                int origX,
                int origY,
                int numX,
                int numY,
                int stepX,
                int stepY);
    vector<Row> &GetRowVec();

    void SetComponentCount(int count, double redundancy_factor = 1.4);
    bool IsComponentExisting(std::string &comp_name);
    Component *AddComponent(std::string &comp_name,
                            Macro *macro_ptr,
                            PlaceStatus place_status,
                            int llx,
                            int lly,
                            CompOrient orient);
    Component *GetComponentPtr(std::string &comp_name);
    vector<Component> &GetComponentsRef() { return components_; }

    bool IsDefViaExist(std::string const &name);
    DefVia *AddDefVia(std::string &name);
    DefVia *GetDefViaPtr(std::string const &name);
    vector<DefVia> &GetDefViasRef() { return vias_; }

    void SetIoPinCount(int count);
    bool IsIoPinExist(std::string &iopin_name);
    IOPin *AddIoPin(std::string &iopin_name,
                    SignalDirection signal_direction,
                    SignalUse signal_use);
    IOPin *GetIoPinPtr(std::string &iopin_name);
    vector<IOPin> &GetIoPinsRef() { return iopins_; }

    void SetNetCount(int count, double redundancy_factor = 1.4);
    bool IsNetExist(std::string &net_name);
    Net *AddNet(std::string &net_name, double weight = 1);
    void AddIoPinToNet(std::string &iopin_name, std::string &net_name);
    void AddCompPinToNet(std::string &comp_name,
                         std::string &pin_name,
                         std::string &net_name);
    Net *GetNetPtr(std::string &net_name);
    vector<Net> &GetNetsRef() { return nets_; }

    SNet *AddSNet(string &net_name, SignalUse use);
    SNet *GetSNet(string &net_name);
    vector<SNet> &GetSNetRef();

    ClusterCol *AddClusterCol(string &name, string &bot_signal);
    vector<ClusterCol> &GetClusterColsRef();

    GcellGrid *AddGcellGrid(XYDirection direction,
                            int start,
                            int nBoundaries,
                            int step);
    vector<GcellGrid> &GetGcellGridsRef();
    void InsertRoutingGuide(int netID,
                            int llx,
                            int lly,
                            int urx,
                            int ury,
                            int layerID);

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

#endif
