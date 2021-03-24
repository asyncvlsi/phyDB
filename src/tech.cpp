#include "tech.h"

#include <cstring>

namespace phydb {

void Tech::SetDatabaseMicron(int database_micron) {
    PhyDbExpects(database_micron > 0, "Cannot Set negative database microns: Tech::SetDatabaseMicron()");
    dbu_per_micron_ = database_micron;
}

int Tech::GetDatabaseMicron() {
    return dbu_per_micron_;
}

void Tech::SetManufacturingGrid(double manufacture_grid) {
    PhyDbExpects(manufacture_grid > 0, "Cannot Set negative manufacturing grid: Tech::SetManufacturingGrid()");
    manufacturing_grid_ = manufacture_grid;
}

double Tech::GetManufacturingGrid() {
    return manufacturing_grid_;
}

void Tech::AddSite(std::string const &name, std::string const &class_name, double width, double height) {
    sites_.emplace_back(name, class_name, width, height);
}

std::vector<Site> &Tech::GetSitesRef() {
    return sites_;
}

void Tech::SetPlacementGrids(double placement_grid_value_x, double placement_grid_value_y) {
    PhyDbExpects(placement_grid_value_x > 0 && placement_grid_value_y > 0, "negative placement grid value not allowed");
    placement_grid_value_x_ = placement_grid_value_x;
    placement_grid_value_y_ = placement_grid_value_y;
    is_placement_grid_set_ = true;
}

bool Tech::GetPlacementGrids(double &placement_grid_value_x, double &placement_grid_value_y) {
    placement_grid_value_x = placement_grid_value_x_;
    placement_grid_value_y = placement_grid_value_y_;
    return is_placement_grid_set_;
}

bool Tech::IsLayerExist(std::string const &layer_name) {
    return layer_2_id_.find(layer_name) != layer_2_id_.end();
}

Layer *Tech::AddLayer(std::string &layer_name, LayerType type, MetalDirection direction) {
    PhyDbExpects(!IsLayerExist(layer_name), "LAYER name_ exists, cannot use again: " + layer_name);
    int id = layers_.size();
    layers_.emplace_back(layer_name, type, direction);
    layer_2_id_[layer_name] = id;
    return &(layers_[id]);
}

Layer *Tech::GetLayerPtr(std::string const &layer_name) {
    if (!IsLayerExist(layer_name)) {
        return nullptr;
    }
    int id = layer_2_id_[layer_name];
    return &(layers_[id]);
}

int Tech::GetLayerId(string const &layer_name) {
    if (!IsLayerExist(layer_name)) {
        return -1;
    }
    int id = layer_2_id_[layer_name];
    return id;
}

std::vector<Layer> &Tech::GetLayersRef() {
    return layers_;
}

bool Tech::IsMacroExist(std::string const &macro_name) {
    return macro_2_id_.find(macro_name) != macro_2_id_.end();
}

Macro *Tech::AddMacro(std::string &macro_name) {
    PhyDbExpects(!IsMacroExist(macro_name), "Macro name_ exists, cannot use it again: " + macro_name);
    int id = macros_.size();
    macros_.emplace_back(macro_name);
    macro_2_id_[macro_name] = id;
    return &(macros_[id]);
}

Macro *Tech::GetMacroPtr(std::string const &macro_name) {
    if (!IsMacroExist(macro_name)) {
        return nullptr;
    }
    int id = macro_2_id_[macro_name];
    return &(macros_[id]);
}

std::vector<Macro> &Tech::GetMacrosRef() {
    return macros_;
}

bool Tech::IsLefViaExist(std::string const &via_name) {
    return via_2_id_.find(via_name) != via_2_id_.end();
}

LefVia *Tech::AddLefVia(std::string &via_name) {
    PhyDbExpects(!IsLefViaExist(via_name), "VIA name_ exists, cannot use it again: " + via_name);
    int id = vias_.size();
    vias_.emplace_back(via_name);
    via_2_id_[via_name] = id;
    return &(vias_[id]);
}

LefVia *Tech::GetLefViaPtr(std::string const &via_name) {
    if (!IsLefViaExist(via_name)) {
        return nullptr;
    }
    int id = via_2_id_[via_name];
    return &(vias_[id]);
}

bool Tech::IsViaRuleGenerateExist(std::string const &name) {
    return via_rule_generate_2_id_.find(name) != via_rule_generate_2_id_.end();
}

ViaRuleGenerate *Tech::AddViaRuleGenerate(std::string &name) {
    PhyDbExpects(!IsViaRuleGenerateExist(name), "Macro name_ exists, cannot use it again");
    int id = via_rule_generates_.size();
    via_rule_generates_.emplace_back(name);
    via_rule_generate_2_id_[name] = id;
    return &(via_rule_generates_[id]);
}

ViaRuleGenerate *Tech::GetViaRuleGeneratePtr(std::string const &name) {
    if (!IsViaRuleGenerateExist(name)) {
        return nullptr;
    }
    int id = via_rule_generate_2_id_[name];
    return &(via_rule_generates_[id]);
}

void Tech::SetNwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
    if (is_n_well_layer_set_) {
        n_layer_ptr_->SetParams(width, spacing, op_spacing, max_plug_dist, overhang);
    } else {
        n_layer_ptr_ = new WellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
        is_n_well_layer_set_ = true;
    }
}

void Tech::SetPwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
    if (is_p_well_layer_set_) {
        p_layer_ptr_->SetParams(width, spacing, op_spacing, max_plug_dist, overhang);
    } else {
        p_layer_ptr_ = new WellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
        is_p_well_layer_set_ = true;
    }
}

void Tech::SetNpwellSpacing(double same_diff, double any_diff) {
    PhyDbExpects(same_diff >= 0 && any_diff >= 0, "Negative values not allowed: Tech::SetNpwellSpacing()");
    same_diff_spacing_ = same_diff;
    any_diff_spacing_ = any_diff;
}

bool Tech::IsWellInfoSet() const {
    return (n_layer_ptr_ != nullptr) || (p_layer_ptr_ != nullptr);
}

WellLayer *Tech::GetNwellLayerPtr() {
    return n_layer_ptr_;
}

WellLayer *Tech::GetPwellLayerPtr() {
    return p_layer_ptr_;
}

void Tech::GetDiffWellSpacing(double &same_diff_spacing, double any_diff_spacing) {
    same_diff_spacing = same_diff_spacing_;
    any_diff_spacing = any_diff_spacing_;
}

void Tech::ReportWellShape() {
    for (auto &well: wells_) {
        well.Report();
    }
}

void Tech::ReportSites() {
    for (auto &site: sites_) {
        std::cout << site << "\n";
    }
}

void Tech::ReportLayers() {
    std::cout << "Total number of layer: " << layers_.size() << "\n";
    for (auto &layer: layers_) {
        layer.Report();
    }
    std::cout << "\n";
}

void Tech::ReportVias() {
    std::cout << "Total number of via: " << vias_.size() << "\n";
    for (auto &via: vias_) {
        via.Report();
    }
    std::cout << "\n";
}

void Tech::ReportMacros() {
    std::cout << "Total number of macro: " << macros_.size() << "\n";
    for (auto &macro: macros_) {
        std::cout << macro << "\n";
    }
    std::cout << "\n";
}

void Tech::ReportMacroWell() {
    std::cout << "SAME_DIFF_SPACING " << same_diff_spacing_ << "\n";
    std::cout << "ANY_DIFF_SPACING " << any_diff_spacing_ << "\n";

    if (p_layer_ptr_!= nullptr) {
        std::cout << "LAYER pwell\n";
        p_layer_ptr_->Report();
    }
    if (n_layer_ptr_ != nullptr) {
        std::cout << "LAYER nwell\n";
        n_layer_ptr_->Report();
    }

    std::cout << "Total number of  macro wells: " << wells_.size() << "\n";
    for (auto &macro_well: wells_) {
        macro_well.Report();
    }
    std::cout << "\n";
}

void Tech::Report() {
    std::cout << "VERSION " << version_ << ";\n";
    std::cout << "BUSBITCHARS " << bus_bit_char_ << ";\n";
    std::cout << "DIVIDERCHAR " << divier_char_ << ";\n";
    std::cout << "DATABASE MICRONS " << dbu_per_micron_ << ";\n";
    std::cout << "MANUFACTURINGGRID " << manufacturing_grid_ << ";\n";
    std::cout << "CLEARANCEMEASURE " << clearance_measure_ << ";\n\n";

    //ReportSites();
    //ReportLayers();
    //ReportVias(); // TODO : Vias not reported, maybe they are not added using callback functions
    ReportMacros();
    //ReportMacroWell();
}

}