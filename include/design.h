#ifndef DESIGN_H
#define DESIGN_H

#include <set>

#include <defrReader.hpp>
#include <defwWriter.hpp>
#include <defwWriterCalls.hpp>

#include "header.h"
#include "row.h"
#include "track.h"
#include "component.h"
#include "iopin.h"
#include "snet.h"
#include "net.h"
#include "tech.h"
#include "defvia.h"

namespace phydb {

class Design {
  public:
    string name_;
    double version_;
    string divider_char_;
    string bus_bit_char_;

    Rect2D<int> die_area_;

    int unit_distance_micron_;
    vector<Row> rows_;
    vector<Track> tracks_;
    vector<Component> components_;
    vector<IOPin> iopins_;
    vector<SNet> snets_;
    vector<Net> nets_;
    vector<DefVia> vias_;

    map<string, int> component_2_id_;
    map<string, int> iopin_2_id_;
    map<string, int> def_via_2_id_;
    map<string, int> layer_name_2_trackid_;
    map<string, int> net_2_id_;
    map<string, int> via_2_id_;
    set<string> row_set_;

    Design() {}

    void SetName(std::string &name);
    void SetVersion(double version);
    void SetDividerChar(std::string &divider_char);
    void SetBusBitChars(std::string &bus_bit_chars);

    void SetUnitsDistanceMicrons(int distance_microns);
    void SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y);

    Track *AddTrack(string &direction, int start, int num_tracks, int step, vector<string> &layer_names);
    vector<Track> &GetTrackVec();

    bool IsRowExist(string &name);
    Row *AddRow(string &name, string &site_name, string &site_orient, int origX, int origY, int numX,
                int numY, int stepX, int stepY);
    vector<Row> &GetRowVec();

    void SetComponentCount(int count);
    bool IsComponentExist(std::string &comp_name);
    Component *AddComponent(std::string &comp_name, std::string &macro_name, PlaceStatus place_status,
                            int llx, int lly, CompOrient orient);
    Component *GetComponentPtr(std::string &comp_name);

    bool IsDefViaExist(std::string const &name);
    DefVia *AddDefVia(std::string &name);
    DefVia *GetDefViaPtr(std::string const &name);

    void SetIoPinCount(int count);
    bool IsIoPinExist(std::string &iopin_name);
    IOPin *AddIoPin(std::string &iopin_name, SignalDirection signal_direction, SignalUse signal_use);
    IOPin *GetIoPinPtr(std::string &iopin_name);

    void SetNetCount(int count);
    bool IsNetExist(std::string &net_name);
    Net *AddNet(std::string &net_name, double weight = 1);
    void AddIoPinToNet(std::string &iopin_name, std::string &net_name);
    void AddCompPinToNet(std::string &comp_name, std::string &pin_name, std::string &net_name);
    Net *GetNetPtr(std::string &net_name);

    void ReportTracks();
    void ReportRows();
    void ReportComponents();
    void ReportIoPins();
    void ReportNets();
    void Report();
};

}

#endif
