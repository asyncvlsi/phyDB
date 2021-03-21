#ifndef DESIGN_H
#define DESIGN_H

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

namespace phydb {

class Design {
  public:
    double _version;
    string _dividerChar;
    string _busBitChars;
    string _name;

    Rect2D<int> _dieArea;

    Point3D<int> _gcellGridDim;
    Point2D<int> _commonGcell;

    int _dbuPerMicro;
    vector<Row> _rows;
    vector<Track> _tracks;
    vector<Component> _components;
    vector<IOPin> _iopins;
    vector<SNet> _snets;
    vector<Net> _nets;

    map<string, int> _component2idx;
    map<string, int> _iopin2idx;
    map<string, int> _defVia2idx;
    map<string, int> _layerName2trackidx;
    map<string, int> _net2idx;
    map<string, int> _powerNet2idx;
    map<string, int> _gndNet2idx;

    map<int, int> layeridx2trackidx;

    Design() {}

    void readDef(string);

    void SetUnitsDistanceMicrons(int distance_microns);
    void SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y);

    // TODO: APIs for tracks
    Track &addTrack(Track &track);

    //TODO: APIs for rows
    Row &addRow(Row &row);

    bool IsComponentExist(std::string &comp_name);
    Component *AddComponent(std::string &comp_name, std::string &macro_name, std::string &place_status,
                            int llx, int lly, std::string &orient);
    Component *GetComponentPtr(std::string &comp_name);
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

    Net &addNet(Net);

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
