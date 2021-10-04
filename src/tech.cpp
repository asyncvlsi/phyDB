#include "tech.h"

#include <cstring>
#include <unordered_set>

namespace phydb {

Tech::~Tech() {
    delete n_layer_ptr_;
    n_layer_ptr_ = nullptr;
    delete p_layer_ptr_;
    p_layer_ptr_ = nullptr;
}

void Tech::SetDatabaseMicron(int database_micron) {
    PhyDBExpects(database_micron > 0,
                 "Cannot Set negative database microns: "
                     + std::to_string(database_micron));

    std::unordered_set<int>
        legal_database_units({100, 200, 1000, 2000, 10000, 20000});
    PhyDBExpects(legal_database_units.find(database_micron)
                     != legal_database_units.end(),
                 "Unsupported DATABASE MICRONS "
                     + std::to_string(database_micron));

    if (database_micron_ > -1 && database_micron != database_micron_) {
        std::cout << "Warning: changing DATABASE MICRONS from "
                  << database_micron_ << " to " << database_micron
                  << "\n";
    }

    database_micron_ = database_micron;
}

int Tech::GetDatabaseMicron() const {
    return database_micron_;
}

void Tech::SetManufacturingGrid(double manufacture_grid) {
    PhyDBExpects(manufacture_grid > 0,
                 "Cannot Set negative manufacturing grid: "
                     + std::to_string(manufacture_grid));

    if (manufacturing_grid_ > -1 && manufacture_grid != manufacturing_grid_) {
        std::cout << "Warning: changing MANUFACTURINGGRID from "
                  << manufacturing_grid_ << " to " << manufacture_grid
                  << "\n";
    }

    manufacturing_grid_ = manufacture_grid;
}

double Tech::GetManufacturingGrid() const {
    return manufacturing_grid_;
}

void Tech::AddSite(std::string const &name,
                   std::string const &class_name,
                   double width,
                   double height) {
    sites_.emplace_back(name, class_name, width, height);
}

std::vector<Site> &Tech::GetSitesRef() {
    return sites_;
}

void Tech::SetPlacementGrids(double placement_grid_value_x,
                             double placement_grid_value_y) {
    PhyDBExpects(placement_grid_value_x > 0 && placement_grid_value_y > 0,
                 "negative placement grid value not allowed");
    placement_grid_value_x_ = placement_grid_value_x;
    placement_grid_value_y_ = placement_grid_value_y;
    is_placement_grid_set_ = true;
}

bool Tech::GetPlacementGrids(double &placement_grid_value_x,
                             double &placement_grid_value_y) {
    placement_grid_value_x = placement_grid_value_x_;
    placement_grid_value_y = placement_grid_value_y_;
    return is_placement_grid_set_;
}

bool Tech::IsLayerExisting(std::string const &layer_name) {
    return layer_2_id_.find(layer_name) != layer_2_id_.end();
}

Layer *Tech::AddLayer(std::string &layer_name,
                      LayerType type,
                      MetalDirection direction) {
    PhyDBExpects(!IsLayerExisting(layer_name),
                 "LAYER name_ exists, cannot use again: " + layer_name);
    int id = layers_.size();
    layers_.emplace_back(layer_name, type, direction);
    layer_2_id_[layer_name] = id;
    layers_[id].SetID(id);
    return &(layers_[id]);
}

Layer *Tech::GetLayerPtr(std::string const &layer_name) {
    if (!IsLayerExisting(layer_name)) {
        return nullptr;
    }
    int id = layer_2_id_[layer_name];
    return &(layers_[id]);
}

int Tech::GetLayerId(string const &layer_name) {
    if (!IsLayerExisting(layer_name)) {
        return -1;
    }
    int id = layer_2_id_[layer_name];
    return id;
}

const std::string &Tech::GetLayerName(int layerID) {
    if (layerID >= layers_.size()) {
        std::cout << "accessing layerID > num. of layers" << endl;
        exit(1);
    }
    return layers_[layerID].GetName();
}

std::vector<Layer> &Tech::GetLayersRef() {
    return layers_;
}

bool Tech::IsMacroExisting(std::string const &macro_name) {
    return macro_2_id_.find(macro_name) != macro_2_id_.end();
}

Macro *Tech::AddMacro(std::string &macro_name) {
    PhyDBExpects(!IsMacroExisting(macro_name),
                 "Macro name_ exists, cannot use it again: " + macro_name);
    int id = (int) macros_.size();
    macros_.emplace_back(macro_name);
    macro_2_id_[macro_name] = id;
    return &(macros_[id]);
}

Macro *Tech::GetMacroPtr(std::string const &macro_name) {
    if (!IsMacroExisting(macro_name)) {
        return nullptr;
    }
    int id = macro_2_id_[macro_name];
    return &(macros_[id]);
}

std::vector<Macro> &Tech::GetMacrosRef() {
    return macros_;
}

bool Tech::IsLefViaExisting(std::string const &via_name) {
    return via_2_id_.find(via_name) != via_2_id_.end();
}

LefVia *Tech::AddLefVia(std::string &via_name) {
    PhyDBExpects(!IsLefViaExisting(via_name),
                 "VIA name_ exists, cannot use it again: " + via_name);
    int id = vias_.size();
    vias_.emplace_back(via_name);
    via_2_id_[via_name] = id;
    return &(vias_[id]);
}

LefVia *Tech::GetLefViaPtr(std::string const &via_name) {
    if (!IsLefViaExisting(via_name)) {
        return nullptr;
    }
    int id = via_2_id_[via_name];
    return &(vias_[id]);
}

vector<LefVia> &Tech::GetLefViasRef() {
    return vias_;
}

bool Tech::IsViaRuleGenerateExisting(std::string const &name) {
    return via_rule_generate_2_id_.find(name) != via_rule_generate_2_id_.end();
}

ViaRuleGenerate *Tech::AddViaRuleGenerate(std::string &name) {
    PhyDBExpects(!IsViaRuleGenerateExisting(name),
                 "Macro name_ exists, cannot use it again");
    int id = via_rule_generates_.size();
    via_rule_generates_.emplace_back(name);
    via_rule_generate_2_id_[name] = id;
    return &(via_rule_generates_[id]);
}

ViaRuleGenerate *Tech::GetViaRuleGeneratePtr(std::string const &name) {
    if (!IsViaRuleGenerateExisting(name)) {
        return nullptr;
    }
    int id = via_rule_generate_2_id_[name];
    return &(via_rule_generates_[id]);
}

vector<ViaRuleGenerate> &Tech::GetViaRuleGeneratesRef() {
    return via_rule_generates_;
}

void Tech::SetNwellLayer(double width,
                         double spacing,
                         double op_spacing,
                         double max_plug_dist,
                         double overhang) {
    if (is_n_well_layer_set_) {
        n_layer_ptr_->SetParams(width,
                                spacing,
                                op_spacing,
                                max_plug_dist,
                                overhang);
    } else {
        n_layer_ptr_ =
            new WellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
        is_n_well_layer_set_ = true;
    }
}

void Tech::SetPwellLayer(double width,
                         double spacing,
                         double op_spacing,
                         double max_plug_dist,
                         double overhang) {
    if (is_p_well_layer_set_) {
        p_layer_ptr_->SetParams(width,
                                spacing,
                                op_spacing,
                                max_plug_dist,
                                overhang);
    } else {
        p_layer_ptr_ =
            new WellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
        is_p_well_layer_set_ = true;
    }
}

void Tech::SetNpwellSpacing(double same_diff, double any_diff) {
    PhyDBExpects(same_diff >= 0 && any_diff >= 0,
                 "Negative values not allowed: Tech::SetNpwellSpacing()");
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

void Tech::GetDiffWellSpacing(double &same_diff_spacing,
                              double any_diff_spacing) {
    same_diff_spacing = same_diff_spacing_;
    any_diff_spacing = any_diff_spacing_;
}

void Tech::ReportWellShape() {
    for (auto &well: wells_) {
        well.Report();
    }
}

std::string Tech::GetLefName() const {
    return lef_name_;
}

void Tech::SetLefName(std::string const &lef_file_name) {
    lef_name_ = lef_file_name;
}

void Tech::FindAllMetalLayers() {
    PhyDBExpects(!layers_.empty(),
                 "It seems LEF file is not loaded, cannot load technology configuration file");
    for (auto &layer: layers_) {
        if (layer.GetType() == ROUTING) {
            metal_layers_.emplace_back(std::make_shared<Layer>(layer));
        }
    }
}

TechConfig &Tech::GetTechConfigRef() {
    return tech_config_;
}

void Tech::AddTechConfigCorner(int corner_index) {
    if (metal_layers_.empty()) {
        FindAllMetalLayers();
    }

    for (auto &metal_ptr: metal_layers_) {
        metal_ptr->AddTechConfigCorner(corner_index);
    }
}

void Tech::AddResOverTable(ResOverTable *res_over_table) {
    if (res_over_table == nullptr) return;
    int layer_index = res_over_table->LayerIndex();
    auto metal_ptr = metal_layers_[layer_index];
    auto corner = metal_ptr->GetLayerTechConfig()->GetLastCorner();
    corner->AddResOverTable(res_over_table);
}

void Tech::AddCapOverTable(CapOverTable *cap_over_table) {
    if (cap_over_table == nullptr) return;
    int layer_index = cap_over_table->LayerIndex();
    auto metal_ptr = metal_layers_[layer_index];
    auto corner = metal_ptr->GetLayerTechConfig()->GetLastCorner();
    corner->AddCapOverTable(cap_over_table);
}

void Tech::AddCapUnderTable(CapUnderTable *cap_under_table) {
    if (cap_under_table == nullptr) return;
    int layer_index = cap_under_table->LayerIndex();
    auto metal_ptr = metal_layers_[layer_index];
    auto corner = metal_ptr->GetLayerTechConfig()->GetLastCorner();
    corner->AddCapUnderTable(cap_under_table);
}

void Tech::AddCapDiagUnderTable(CapDiagUnderTable *cap_diag_under_table) {
    if (cap_diag_under_table == nullptr) return;
    int layer_index = cap_diag_under_table->LayerIndex();
    auto metal_ptr = metal_layers_[layer_index];
    auto corner = metal_ptr->GetLayerTechConfig()->GetLastCorner();
    corner->AddCapDiagUnderTable(cap_diag_under_table);
}

void Tech::AddCapOverUnderTable(CapOverUnderTable *cap_over_under_table) {
    if (cap_over_under_table == nullptr) return;
    int layer_index = cap_over_under_table->LayerIndex();
    auto metal_ptr = metal_layers_[layer_index];
    auto corner = metal_ptr->GetLayerTechConfig()->GetLastCorner();
    corner->AddCapOverUnderTable(cap_over_under_table);
}

void Tech::FixResOverTable() {
    for (auto &metal_ptr: metal_layers_) {
        auto layer_tech_config = metal_ptr->GetLayerTechConfig();
        layer_tech_config->FixResOverTable();
    }
}

void Tech::SetResistanceUnit(bool from_tech_config, bool is_report) {
    if (from_tech_config) {
        for (auto &metal_ptr: metal_layers_) {
            metal_ptr->SetResistanceUnitFromTechConfig();
        }
    } else {
        for (auto &metal_ptr: metal_layers_) {
            metal_ptr->SetResistanceUnitFromLef();
        }
    }

    if (is_report) {
        int num_of_corners_ = tech_config_.model_count_;
        for (int i = 0; i < num_of_corners_; ++i) {
            std::cout << "model index: " << i << "\nres, ";
            size_t sz = metal_layers_.size();
            for (size_t j = 0; j < sz; ++j) {
                std::cout << j + 1 << ": " << metal_layers_[j]->unit_res_[i] << ", ";
            }
            std::cout << "\n";
        }
    }
}

void Tech::SetCapacitanceUnit(bool from_tech_config, bool is_report) {
    if (from_tech_config) {
        for (auto &metal_ptr: metal_layers_) {
            metal_ptr->SetCapacitanceUnitFromTechConfig();
        }
    } else {
        for (auto &metal_ptr: metal_layers_) {
            metal_ptr->SetCapacitanceUnitFromLef();
        }
    }

    if (is_report) {
        int num_of_corners_ = tech_config_.model_count_;
        for (int i = 0; i < num_of_corners_; ++i) {
            std::cout << "model index: " << i << "\nedge cap, ";
            size_t sz = metal_layers_.size();
            for (size_t j = 0; j < sz; ++j) {
                std::cout << j + 1 << ": " << metal_layers_[j]->unit_edge_cap_[i] << ", ";
            }
            std::cout << "\narea cap, ";
            for (size_t j = 0; j < sz; ++j) {
                std::cout << j + 1 << ": " << metal_layers_[j]->unit_area_cap_[i] << ", ";
            }
            std::cout << "\n";
        }
    }
}

void Tech::ReportLayersTechConfig() {
    for (auto &metal_ptr: metal_layers_) {
        auto tech_config = metal_ptr->GetLayerTechConfig();
        tech_config->Report();
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

    if (p_layer_ptr_ != nullptr) {
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
    std::cout << "DATABASE MICRONS " << database_micron_ << ";\n";
    std::cout << "MANUFACTURINGGRID " << manufacturing_grid_ << ";\n";
    std::cout << "CLEARANCEMEASURE " << clearance_measure_ << ";\n\n";

    //ReportSites();
    //ReportLayers();
    ReportVias(); // TODO : Vias not reported, maybe they are not added using callback functions
    //ReportMacros();
    //ReportMacroWell();
}

}