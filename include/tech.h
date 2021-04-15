#ifndef TECH_H
#define TECH_H

#include <list>
#include <unordered_map>

#include "layer.h"
#include "site.h"
#include "macro.h"
#include "lefvia.h"
#include "viarulegenerate.h"

namespace phydb {

class Tech {
    friend class PhyDB;
  private:
    string version_;
    string bus_bit_char_;
    string divier_char_;
    string clearance_measure_;
    double manufacturing_grid_ = -1;

    int dbu_per_micron_ = -1;

    vector<Site> sites_;
    vector<Layer> layers_;
    vector<Macro> macros_;
    vector<LefVia> vias_;
    vector<ViaRuleGenerate> via_rule_generates_;

    unordered_map<string, int> layer_2_id_;
    unordered_map<string, int> macro_2_id_;
    unordered_map<string, int> via_2_id_;
    unordered_map<string, int> via_rule_generate_2_id_;

    /****placement grid parameters****/
    bool is_placement_grid_set_ = false;
    double placement_grid_value_x_ = -1;
    double placement_grid_value_y_ = -1;

    /****N/P-well layers_****/
    bool is_n_well_layer_set_ = false;
    bool is_p_well_layer_set_ = false;
    WellLayer *n_layer_ptr_ = nullptr;
    WellLayer *p_layer_ptr_ = nullptr;
    double same_diff_spacing_ = -1;
    double any_diff_spacing_ = -1;
    std::list<MacroWell> wells_;

  public:
    Tech() : manufacturing_grid_(-1), dbu_per_micron_(-1) {}
    ~Tech();

    void SetDatabaseMicron(int database_micron);
    int GetDatabaseMicron();
    void SetManufacturingGrid(double manufacture_grid);
    double GetManufacturingGrid();
    void AddSite(std::string const &name, std::string const &class_name, double width, double height);
    std::vector<Site> &GetSitesRef();
    void SetPlacementGrids(double placement_grid_value_x, double placement_grid_value_y);
    bool GetPlacementGrids(double &placement_grid_value_x, double &placement_grid_value_y);

    bool IsLayerExist(std::string const &layer_name);
    Layer *AddLayer(std::string &layer_name, LayerType type, MetalDirection direction=HORIZONTAL);
    Layer *GetLayerPtr(std::string const &layer_name);
    int GetLayerId(string const &layer_name);
    std::vector<Layer> &GetLayersRef();

    bool IsMacroExist(std::string const &macro_name);
    Macro *AddMacro(std::string &macro_name);
    Macro *GetMacroPtr(std::string const &macro_name);
    std::vector<Macro> &GetMacrosRef();

    bool IsLefViaExist(std::string const &via_name);
    LefVia *AddLefVia(std::string &via_name);
    LefVia *GetLefViaPtr(std::string const &via_name);
    vector<LefVia>& GetLefViasRef();

    bool IsViaRuleGenerateExist(std::string const &name);
    ViaRuleGenerate* AddViaRuleGenerate(std::string &name);
    ViaRuleGenerate* GetViaRuleGeneratePtr(std::string const &name);
    
    void SetNwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang);
    void SetPwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang);
    void SetNpwellSpacing(double same_diff, double any_diff);
    bool IsWellInfoSet() const;
    WellLayer *GetNwellLayerPtr();
    WellLayer *GetPwellLayerPtr();
    void GetDiffWellSpacing(double &same_diff_spacing, double any_diff_spacing);
    void ReportWellShape(); // report the well shape_ for each BlockType for debugging purposes.

    void ReportSites();
    void ReportLayers();
    void ReportVias();
    void ReportMacros();
    void ReportMacroWell();
    void Report(); // for debugging purposes

    friend ostream &operator<<(ostream &, const Tech &);
};

ostream &operator<<(ostream &, const Tech &);

}

#endif

