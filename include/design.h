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
    double version_;
    string divider_char_;
    string bus_bit_chars_;
    string name_;

    Rect2D<int> die_area_;

    int dbu_per_micro_;
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

    void SetUnitsDistanceMicrons(int distance_microns);
    void SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y);

    Track *AddTrack(string &direction, int start, int num_tracks, int step, vector<string> &layer_names);
    vector<Track> &GetTrackVec();

    bool IsRowExist(string &name);
    Row *AddRow(string &name, string &site_name, string &site_orient, int origX, int origY, int numX,
                int numY, int stepX, int stepY);
    vector<Row> &GetRowVec();

    bool IsComponentExist(std::string &comp_name);
    Component *AddComponent(std::string &comp_name, std::string &macro_name, std::string &place_status,
                            int llx, int lly, std::string &orient);
    Component *GetComponentPtr(std::string &comp_name);

    bool IsDefViaExist(std::string const &name);
    DefVia *AddDefVia(std::string &name);
    DefVia *GetDefViaPtr(std::string const &name);

    void ReportComponent();

    bool IsIoPinExist(std::string &iopin_name);
    IOPin *AddIoPin(std::string &iopin_name, std::string &place_status,
                    std::string &signal_use, std::string &signal_direction,
                    int lx = 0, int ly = 0);
    IOPin *GetIoPinPtr(std::string &iopin_name);

    bool IsNetExist(std::string &net_name);
    Net *AddNet(std::string &net_name, double weight = 1);
    void AddIoPinToNet(std::string &iopin_name, std::string &net_name);
    void AddCompPinToNet(std::string &comp_name, std::string &pin_name, std::string &net_name);
    Net *GetNetPtr(std::string &net_name);

    friend int getDefVersion(defrCallbackType_e, double, defiUserData);
    friend int getDefBusBit(defrCallbackType_e, const char *, defiUserData);
    friend int getDefDivider(defrCallbackType_e, const char *, defiUserData);
    friend int getDefString(defrCallbackType_e, const char *, defiUserData);
    friend int getDefVoid(defrCallbackType_e, void *, defiUserData);
    friend int getDefDieArea(defrCallbackType_e, defiBox *, defiUserData);
    friend int getDefUnits(defrCallbackType_e, double, defiUserData);
    friend int getDefTracks(defrCallbackType_e, defiTrack *, defiUserData);
    friend int getDefComponents(defrCallbackType_e, defiComponent *, defiUserData);
    friend int getDefIOPins(defrCallbackType_e, defiPin *, defiUserData);
    friend int getDefNets(defrCallbackType_e, defiNet *, defiUserData);
    friend int getDefSNets(defrCallbackType_e, defiNet *, defiUserData);
    friend int getDefVias(defrCallbackType_e, defiVia *, defiUserData);
    friend int getDefGcell(defrCallbackType_e, defiGcellGrid *, defiUserData);
    friend int getDefRow(defrCallbackType_e, defiRow *, defiUserData);
};

int getDefVersion(defrCallbackType_e, double, defiUserData);
int getDefBusBit(defrCallbackType_e, const char *, defiUserData);
int getDefDivider(defrCallbackType_e, const char *, defiUserData);
int getDefString(defrCallbackType_e, const char *, defiUserData);
int getDefVoid(defrCallbackType_e, void *, defiUserData);
int getDefDieArea(defrCallbackType_e, defiBox *, defiUserData);
int getDefUnits(defrCallbackType_e, double, defiUserData);
int getDefTracks(defrCallbackType_e, defiTrack *, defiUserData);
int getDefComponents(defrCallbackType_e, defiComponent *, defiUserData);
int getDefIOPins(defrCallbackType_e, defiPin *, defiUserData);
int getDefNets(defrCallbackType_e, defiNet *, defiUserData);
int getDefSNets(defrCallbackType_e, defiNet *, defiUserData);
int getDefVias(defrCallbackType_e, defiVia *, defiUserData);
int getDefGcell(defrCallbackType_e, defiGcellGrid *, defiUserData);
int getDefRow(defrCallbackType_e, defiRow *, defiUserData);

int writeVersion(defwCallbackType_e c, defiUserData ud);
int writeBusBit(defwCallbackType_e c, defiUserData ud);
int writeDivider(defwCallbackType_e c, defiUserData ud);
int writeUnits(defwCallbackType_e c, defiUserData ud);
int writeRows(defwCallbackType_e c, defiUserData ud);
int writeTracks(defwCallbackType_e c, defiUserData ud);
int writeComponents(defwCallbackType_e c, defiUserData ud);
int writeDieArea(defwCallbackType_e c, defiUserData ud);
int writeDesignName(defwCallbackType_e c, defiUserData ud);
int writeDesignEnd(defwCallbackType_e c, defiUserData ud);
int writeSNet(defwCallbackType_e c, defiUserData ud);
int writeIOPins(defwCallbackType_e c, defiUserData ud);
int writeNets(defwCallbackType_e c, defiUserData ud);
int clusterWriteSNet(defwCallbackType_e c, defiUserData ud);

}

#endif
