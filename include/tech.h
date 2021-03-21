#ifndef TECH_H
#define TECH_H

#include <list>

#include <lefrReader.hpp>

#include "layer.h"
#include "site.h"
#include "macro.h"
#include "lefVia.h"

namespace phydb {

class Tech {
    friend class PhyDB;
  private:
    string _version;
    string _busBitChar;
    string _divierChar;
    string _clearanceMeasure;
    double _manufacturingGrid;

    int _dbuPerMicron;

    vector<Site> _sites;
    vector<Layer> _layers;
    vector<Macro> _macros;
    vector<LefVia> _vias;
    vector<ViaRuleGenerate> _via_rule_generates;

    map<string, int> _layer2id;
    map<string, int> _macro2id;
    map<string, int> _via_rule_generate2id;

    /****placement grid parameters****/
    bool is_placement_grid_set_ = false;
    double placement_grid_value_x_ = 0;
    double placement_grid_value_y_ = 0;

    /****N/P-well layers****/
    bool is_n_well_layer_set_ = false;
    bool is_p_well_layer_set_ = false;
    WellLayer *n_layer_ptr_ = nullptr;
    WellLayer *p_layer_ptr_ = nullptr;
    double same_diff_spacing_ = -1;
    double any_diff_spacing_ = -1;
    std::list<MacroWell> wells_;

  public:
    Tech() : _manufacturingGrid(0), _dbuPerMicron(0) {}

    void setDatabaseMicron(int database_micron);
    void setManufacturingGrid(double manufacture_grid);
    void addSite(std::string const &name, std::string const &class_name, double width, double height);
    void setPlacementGrids(double placement_grid_value_x, double placement_grid_value_y);

    bool isLayerExist(std::string const &layer_name);
    Layer *addLayer(std::string &layer_name);
    Layer *getLayerPtr(std::string const &layer_name);
    int GetLayerId(const string& layerName);
    void ReportLayers();

    bool isMacroExist(std::string const &macro_name);
    Macro *addMacro(std::string &macro_name);
    Macro *getMacroPtr(std::string const &macro_name);

    bool IsLefViaExist(std::string const &via_name);
    LefVia *AddLefVia(std::string &via_name);
    LefVia *GetLefViaPtr(std::string const &via_name);

    bool IsViaRuleGenerateExist(std::string const &name);
    ViaRuleGenerate* AddViaRuleGenerate(std::string &name);
    ViaRuleGenerate* GetViaRuleGeneratePtr(std::string const &name)
    
    void setNwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang);
    void setPwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang);
    void setNpwellSpacing(double same_diff, double any_diff);
    bool IsWellInfoSet() const;
    void ReportWellShape(); // report the well shape for each BlockType for debugging purposes.


    friend ostream &operator<<(ostream &, const Tech &);

    friend int getLefSite(lefrCallbackType_e, lefiSite *, lefiUserData);
    friend int getLefMacros(lefrCallbackType_e, lefiMacro *, lefiUserData);
    friend int getLefString(lefrCallbackType_e, const char *, lefiUserData);
    friend int getLefUnits(lefrCallbackType_e, lefiUnits *, lefiUserData);
    friend int getLefManufacturingGrid(lefrCallbackType_e, double, lefiUserData);
    friend int getLefPins(lefrCallbackType_e, lefiPin *, lefiUserData);
    friend int getLefObs(lefrCallbackType_e, lefiObstruction *, lefiUserData);
    friend int getLefLayers(lefrCallbackType_e, lefiLayer *, lefiUserData);
    friend int getLefVias(lefrCallbackType_e, lefiVia *, lefiUserData);
    friend int getLefViaGenerateRules(lefrCallbackType_e, lefiViaRule *, lefiUserData);

};

int getLefSite(lefrCallbackType_e type, lefiSite *site, lefiUserData data);
int getLefMacrosBegin(lefrCallbackType_e type, const char *str, lefiUserData data);
int getLefMacros(lefrCallbackType_e, lefiMacro *, lefiUserData);
int getLefMacrosEnd(lefrCallbackType_e type, const char *str, lefiUserData data);
int getLefString(lefrCallbackType_e, const char *, lefiUserData);
int getLefUnits(lefrCallbackType_e, lefiUnits *, lefiUserData);
int getLefManufacturingGrid(lefrCallbackType_e, double, lefiUserData);
int getLefPins(lefrCallbackType_e, lefiPin *, lefiUserData);
int getLefObs(lefrCallbackType_e, lefiObstruction *, lefiUserData);
int getLefLayers(lefrCallbackType_e, lefiLayer *, lefiUserData);
int getLefVias(lefrCallbackType_e, lefiVia *, lefiUserData);
int getLefViaGenerateRules(lefrCallbackType_e, lefiViaRule *, lefiUserData);

ostream &operator<<(ostream &, const Tech &);

}

#endif

