#include "phydb.h"

#include <cmath>

#include <fstream>

#include "lefdefparser.h"
#include "defwriter.h"

namespace phydb {

Tech *PhyDB::GetTechPtr() {
    return &tech_;
}

Design *PhyDB::GetDesignPtr() {
    return &design_;
}

void PhyDB::SetDatabaseMicron(int database_micron) {
    tech_.SetDatabaseMicron(database_micron);
}

void PhyDB::SetManufacturingGrid(double manufacture_grid) {
    tech_.SetManufacturingGrid(manufacture_grid);
}

void PhyDB::AddSite(std::string const &name, std::string const &class_name, double width, double height) {
    tech_.AddSite(name, class_name, width, height);
}

void PhyDB::SetPlacementGrids(double placement_grid_value_x_, double placement_grid_value_y_) {
    tech_.SetPlacementGrids(placement_grid_value_x_, placement_grid_value_y_);
}

bool PhyDB::IsLayerExisting(std::string const &layer_name) {
    return tech_.IsLayerExist(layer_name);
}

Layer *PhyDB::AddLayer(std::string &layer_name, LayerType type, MetalDirection direction) {
    return tech_.AddLayer(layer_name, type, direction);
}

Layer *PhyDB::GetLayerPtr(std::string const &layer_name) {
    return tech_.GetLayerPtr(layer_name);
}

vector<Layer> &PhyDB::GetLayersRef() {
    return tech_.GetLayersRef();
}

bool PhyDB::IsMacroExisting(std::string const &macro_name) {
    return tech_.IsMacroExist(macro_name);
}

Macro *PhyDB::AddMacro(std::string &macro_name) {
    return tech_.AddMacro(macro_name);
}

Macro *PhyDB::GetMacroPtr(std::string const &macro_name) {
    return tech_.GetMacroPtr(macro_name);
}

bool PhyDB::IsLefViaExisting(std::string const &name) {
    return tech_.IsLefViaExist(name);
}

LefVia *PhyDB::AddLefVia(std::string &via_name) {
    return tech_.AddLefVia(via_name);
}

LefVia *PhyDB::GetLefViaPtr(std::string const &via_name) {
    return tech_.GetLefViaPtr(via_name);
}

bool PhyDB::IsViaRuleGenerateExisting(std::string const &name) {
    return tech_.IsViaRuleGenerateExist(name);
}

ViaRuleGenerate *PhyDB::AddViaRuleGenerate(std::string &name) {
    return tech_.AddViaRuleGenerate(name);
}

ViaRuleGenerate *PhyDB::GetViaRuleGeneratePtr(std::string const &name) {
    return tech_.GetViaRuleGeneratePtr(name);
}

void PhyDB::SetDefName(std::string &name) {
    design_.SetName(name);
}

void PhyDB::SetDefVersion(double version) {
    design_.SetVersion(version);
}

void PhyDB::SetDefDividerChar(std::string &divider_char) {
    design_.SetDividerChar(divider_char);
}

void PhyDB::SetDefBusBitChar(std::string &bus_bit_char) {
    design_.SetBusBitChar(bus_bit_char);
}

std::string PhyDB::GetDefName() const {
    return design_.GetName();
}

double PhyDB::GetDefVersion() const {
    return design_.GetVersion();
}

std::string PhyDB::GetDefDividerChar() const {
    return design_.GetDividerChar();
}

std::string PhyDB::GetDefBusBitChar() const {
    return design_.GetBusBitChar();
}

void PhyDB::SetUnitsDistanceMicrons(int distance_microns) {
    design_.SetUnitsDistanceMicrons(distance_microns);
}

void PhyDB::SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y) {
    design_.SetDieArea(lower_x, lower_y, upper_x, upper_y);
}

Rect2D<int> PhyDB::GetDieArea() const {
    return design_.GetDieArea();
}

void PhyDB::SetComponentCount(int count) {
    design_.SetComponentCount(count);
}

bool PhyDB::IsComponentExisting(std::string &component_name) {
    return design_.IsComponentExist(component_name);
}

Track *PhyDB::AddTrack(XYDirection direction, int start, int nTracks, int step, vector<string> &layer_names) {
    return design_.AddTrack(direction, start, nTracks, step, layer_names);
}

vector<Track> &PhyDB::GetTracksRef() {
    return design_.GetTracksRef();
}

Row *PhyDB::AddRow(string &name, string &site_name, string &site_orient, int origX, int origY, int numX,
                   int numY, int stepX, int stepY) {
    return design_.AddRow(name, site_name, site_orient, origX, origY, numX, numY, stepX, stepY);
}

vector<Row> &PhyDB::GetRowVec() {
    return design_.GetRowVec();
}

Component *PhyDB::AddComponent(std::string &comp_name, std::string &macro_name, PlaceStatus place_status,
                               int llx, int lly, CompOrient orient) {
    return design_.AddComponent(comp_name, macro_name, place_status, llx, lly, orient);
}

Component *PhyDB::GetComponentPtr(std::string &comp_name) {
    return design_.GetComponentPtr(comp_name);
}

void PhyDB::SetIoPinCount(int count) {
    design_.SetIoPinCount(count);
}

bool PhyDB::IsIoPinExisting(std::string &iopin_name) {
    return design_.IsIoPinExist(iopin_name);
}

bool PhyDB::IsDefViaExisting(std::string const &name) {
    return design_.IsDefViaExist(name);
}

DefVia *PhyDB::AddDefVia(std::string &via_name) {
    return design_.AddDefVia(via_name);
}

DefVia *PhyDB::GetDefViaPtr(std::string const &via_name) {
    return design_.GetDefViaPtr(via_name);
}

IOPin *PhyDB::AddIoPin(std::string &iopin_name, SignalDirection signal_direction, SignalUse signal_use) {
    return design_.AddIoPin(iopin_name, signal_direction, signal_use);
}

IOPin *PhyDB::GetIoPinPtr(std::string &iopin_name) {
    return design_.GetIoPinPtr(iopin_name);
}

void PhyDB::SetNetCount(int count) {
    design_.SetNetCount(count);
}

bool PhyDB::IsNetExisting(std::string &net_name) {
    return design_.IsNetExist(net_name);
}

Net *PhyDB::AddNet(std::string &net_name, double weight, void *act_net_ptr) {
    auto *ret = design_.AddNet(net_name, weight);

    if (act_net_ptr != nullptr) {
        if (act_phy_db_timing_api_.IsActNetPtrExisting(act_net_ptr)) {
            int id = act_phy_db_timing_api_.ActNetPtr2Id(act_net_ptr);
            std::string error_msg =
                "Net " + design_.nets_[id].GetName() + " has the same Act pointer as " + net_name;
            PhyDBExpects(false, error_msg);
        }
        int id = (int) design_.nets_.size() - 1;
        act_phy_db_timing_api_.AddActNetPtrIdPair(act_net_ptr, id);
    }

    return ret;
}

void PhyDB::AddIoPinToNet(std::string &iopin_name, std::string &net_name) {
    PhyDBExpects(IsIoPinExisting(iopin_name), "Cannot add a nonexistent iopin to a net: " + iopin_name);
    PhyDBExpects(IsNetExisting(net_name), "Cannot add iopin to a nonexistent Net: " + net_name);
    design_.AddIoPinToNet(iopin_name, net_name);
}

void PhyDB::AddCompPinToNet(std::string &comp_name, std::string &pin_name, std::string &net_name) {
    PhyDBExpects(IsNetExisting(net_name), "Cannot add a component pin to a nonexistent Net: " + net_name);
    PhyDBExpects(IsComponentExisting(comp_name), "Cannot add a nonexistent component to a net: " + comp_name);
    Component *comp_ptr = GetComponentPtr(comp_name);
    std::string macro_name = comp_ptr->GetMacroName();
    Macro *macro_ptr = GetMacroPtr(macro_name);
    PhyDBExpects(macro_ptr->IsPinExist(pin_name),
                 "Macro " + macro_name + " does not contain a pin with name " + pin_name);
    design_.AddCompPinToNet(comp_name, pin_name, net_name);
}

void PhyDB::BindPhydbPinToActPin(std::string &comp_name, std::string &pin_name, void *act_comp_pin_ptr) {
    PhyDBExpects(IsComponentExisting(comp_name), "Cannot find component: " + comp_name);
    Component *comp_ptr = GetComponentPtr(comp_name);
    std::string macro_name = comp_ptr->GetMacroName();
    Macro *macro_ptr = GetMacroPtr(macro_name);
    PhyDBExpects(macro_ptr->IsPinExist(pin_name),
                 "Macro " + macro_name + " does not contain a pin with name " + pin_name);

    int comp_id = design_.component_2_id_[comp_name];
    int pin_id = macro_ptr->GetPinId(pin_name);

    if (act_phy_db_timing_api_.IsActComPinPtrExisting(act_comp_pin_ptr)) {
        auto id_pair = act_phy_db_timing_api_.ActCompPinPtr2Id(act_comp_pin_ptr);
        Component &comp = design_.components_[id_pair.comp_id];
        Macro *tmp_macro_ptr = GetMacroPtr(comp.GetMacroName());
        Pin &pin = tmp_macro_ptr->GetPinsRef()[id_pair.pin_id];
        std::string error_msg =
            "Component pin: " + comp.GetName() + " " + pin.GetName()
                + " has the same Act pointer as " + comp_name + " " + pin_name;
        PhyDBExpects(false, error_msg);
    }
    act_phy_db_timing_api_.AddActCompPinPtrIdPair(act_comp_pin_ptr, comp_id, pin_id);
}

Net *PhyDB::GetNetPtr(std::string &net_name) {
    return design_.GetNetPtr(net_name);
}

SNet *PhyDB::AddSNet(string &net_name, SignalUse use) {
    return design_.AddSNet(net_name, use);
}

SNet *PhyDB::GetSNet(string &net_name) {
    return design_.GetSNet(net_name);
}

vector<SNet> &PhyDB::GetSNetRef() {
    return design_.GetSNetRef();
}

void PhyDB::SetNwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
    tech_.SetNwellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
}

void PhyDB::SetPwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
    tech_.SetPwellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
}

void PhyDB::SetNpwellSpacing(double same_spacing, double any_spacing) {
    tech_.SetNpwellSpacing(same_spacing, any_spacing);
}

MacroWell *PhyDB::AddMacrowell(std::string &macro_name) {
    Macro *macro_ptr = GetMacroPtr(macro_name);
    PhyDBExpects(macro_ptr != nullptr, "Macro does not exist, cannot add well info: " + macro_name);
    tech_.wells_.emplace_back(macro_ptr);
    macro_ptr->SetWellPtr(&(tech_.wells_.back()));
    return macro_ptr->GetWellPtr();
}

ClusterCol *PhyDB::AddClusterCol(string &name, string &bot_signal) {
    return design_.AddClusterCol(name, bot_signal);
}

vector<ClusterCol> &PhyDB::GetClusterColsRef() {
    return design_.GetClusterColsRef();
}

GcellGrid *PhyDB::AddGcellGrid(XYDirection direction, int start, int nBoundaries, int step) {
    return design_.AddGcellGrid(direction, start, nBoundaries, step);
}

vector<GcellGrid> &PhyDB::GetGcellGridsRef() {
    return design_.GetGcellGridsRef();
}

void PhyDB::SetGetNumConstraintsCB(int (*callback_function)()) {
    act_phy_db_timing_api_.SetGetNumConstraintsCB(callback_function);
}

void PhyDB::SetUpdateTimingIncremental(void (*callback_function)()) {
    act_phy_db_timing_api_.SetUpdateTimingIncrementalCB(callback_function);
}

void PhyDB::SetGetSlackCB(double (*callback_function)(int)) {
    act_phy_db_timing_api_.SetGetSlackCB(callback_function);
}

void PhyDB::SetGetWitnessCB(void (*callback_function)(int, std::vector<ActEdge> &, std::vector<ActEdge> &)) {
    act_phy_db_timing_api_.SetGetWitnessCB(callback_function);
}

void PhyDB::SetGetViolatedTimingConstraintsCB(void (*callback_function)(std::vector<int> &)) {
    act_phy_db_timing_api_.SetGetViolatedTimingConstraintsCB(callback_function);
}

void PhyDB::ReadLef(string const &lefFileName) {
    Si2ReadLef(this, lefFileName);
}

void PhyDB::ReadDef(string const &defFileName) {
    Si2ReadDef(this, defFileName);
}

void PhyDB::ReadCell(string const &cellFileName) {
    std::ifstream ist(cellFileName.c_str());
    if (ist.is_open()) {
        std::cout << "Loading CELL file: " << cellFileName << "\n";
    } else {
        std::cout << "ERROR: cannot open input file " << cellFileName << std::endl;
        exit(1);
    }
    std::string line;

    while (!ist.eof()) {
        getline(ist, line);
        if (line.empty()) continue;
        if (line.find("LAYER") != std::string::npos) {
            if (line.find("LEGALIZER") != std::string::npos) {
                std::vector<std::string> legalizer_fields;
                double same_diff_spacing = 0;
                double any_diff_spacing = 0;
                do {
                    getline(ist, line);
                    StrSplit(line, legalizer_fields);
                    if (legalizer_fields.size() != 2) {
                        std::cout << "Expect: SPACING + Value, get: " + line << std::endl;
                        exit(1);
                    }
                    if (legalizer_fields[0] == "SAME_DIFF_SPACING") {
                        try {
                            same_diff_spacing = std::stod(legalizer_fields[1]);
                        } catch (...) {
                            std::cout << "Invalid stod conversion: " + line << std::endl;
                            exit(1);
                        }
                    } else if (legalizer_fields[0] == "ANY_DIFF_SPACING") {
                        try {
                            any_diff_spacing = std::stod(legalizer_fields[1]);
                        } catch (...) {
                            std::cout << "Invalid stod conversion: " + line << std::endl;
                            exit(1);
                        }
                    }
                } while (line.find("END LEGALIZER") == std::string::npos && !ist.eof());
                SetNpwellSpacing(same_diff_spacing, any_diff_spacing);
            } else {
                std::vector<std::string> well_fields;
                StrSplit(line, well_fields);
                bool is_n_well = (well_fields[1] == "nwell");
                if (!is_n_well) {
                    if (well_fields[1] != "pwell") {
                        std::cout << "Unknow N/P well type: " + well_fields[1] << std::endl;
                        exit(1);
                    }
                }
                std::string end_layer_flag = "END " + well_fields[1];
                double width = 0;
                double spacing = 0;
                double op_spacing = 0;
                double max_plug_dist = 0;
                double overhang = 0;
                do {
                    if (line.find("MINWIDTH") != std::string::npos) {
                        StrSplit(line, well_fields);
                        try {
                            width = std::stod(well_fields[1]);
                        } catch (...) {
                            std::cout << "Invalid stod conversion: " + well_fields[1] << std::endl;
                            exit(1);
                        }
                    } else if (line.find("OPPOSPACING") != std::string::npos) {
                        StrSplit(line, well_fields);
                        try {
                            op_spacing = std::stod(well_fields[1]);
                        } catch (...) {
                            std::cout << "Invalid stod conversion: " + well_fields[1] << std::endl;
                            exit(1);
                        }
                    } else if (line.find("SPACING") != std::string::npos) {
                        StrSplit(line, well_fields);
                        try {
                            spacing = std::stod(well_fields[1]);
                        } catch (...) {
                            std::cout << "Invalid stod conversion: " + well_fields[1] << std::endl;
                            exit(1);
                        }
                    } else if (line.find("MAXPLUGDIST") != std::string::npos) {
                        StrSplit(line, well_fields);
                        try {
                            max_plug_dist = std::stod(well_fields[1]);
                        } catch (...) {
                            std::cout << "Invalid stod conversion: " + well_fields[1] << std::endl;
                            exit(1);
                        }
                    } else if (line.find("MAXPLUGDIST") != std::string::npos) {
                        StrSplit(line, well_fields);
                        try {
                            overhang = std::stod(well_fields[1]);
                        } catch (...) {
                            std::cout << "Invalid stod conversion: " + well_fields[1] << std::endl;
                            exit(1);
                        }
                    } else {}
                    getline(ist, line);
                } while (line.find(end_layer_flag) == std::string::npos && !ist.eof());
                if (is_n_well) {
                    SetNwellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
                } else {
                    SetPwellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
                }
            }
        }

        if (line.find("MACRO") != std::string::npos) {
            std::vector<std::string> macro_fields;
            StrSplit(line, macro_fields);
            std::string end_macro_flag = "END " + macro_fields[1];
            MacroWell *well_ptr = AddMacrowell(macro_fields[1]);
            do {
                getline(ist, line);
                bool is_n = false;
                if (line.find("LAYER") != std::string::npos) {
                    do {
                        if (line.find("nwell") != std::string::npos) {
                            is_n = true;
                        }
                        if (line.find("RECT") != std::string::npos) {
                            double lx = 0, ly = 0, ux = 0, uy = 0;
                            std::vector<std::string> shape_fields;
                            StrSplit(line, shape_fields);
                            try {
                                lx = std::stod(shape_fields[1]);
                                ly = std::stod(shape_fields[2]);
                                ux = std::stod(shape_fields[3]);
                                uy = std::stod(shape_fields[4]);
                            } catch (...) {
                                std::cout << "Invalid stod conversion: " + line << std::endl;
                                exit(1);
                            }
                            well_ptr->SetWellRect(is_n, lx, ly, ux, uy);
                        }
                        getline(ist, line);
                    } while (line.find("END VERSION") == std::string::npos && !ist.eof());
                }
            } while (line.find(end_macro_flag) == std::string::npos && !ist.eof());
        }
    }
    if (!tech_.IsWellInfoSet()) {
        std::cout << "N/P well technology information not found!" << std::endl;
        exit(1);
    }
    //tech_.ReportWellShape();

    std::cout << "CELL file loading complete: " << cellFileName << "\n";
}

void PhyDB::ReadCluster(string const &clusterFileName) {
    std::ifstream infile(clusterFileName.c_str());
    if (infile.is_open()) {
        std::cout << "Loading cluster file: " << clusterFileName << "\n";
    } else {
        std::cout << "ERROR: cannot open input file " << clusterFileName << std::endl;
        exit(1);
    }

    string tmp1, tmp2, tmp3;
    int lx, ux, ly, uy;
    int cnt_y = 0, cnt_x = 0;
    ClusterCol *col;
    while (!infile.eof()) {
        infile >> tmp1 >> tmp2;
        if (tmp1 == "STRIP") {
            infile >> lx >> ux >> tmp3;
            col = AddClusterCol(tmp2, tmp3);
            col->SetXRange(lx, ux);
        } else if (tmp1 == "END") {
            assert(tmp2 == col->GetName());
        } else {
            ly = atoi(tmp1.c_str());
            uy = atoi(tmp2.c_str());
            col->AddRow(ly, uy);
        }
    }
}

void PhyDB::StrSplit(std::string &line, std::vector<std::string> &res) {
    static std::vector<char> delimiter_list{' ', ':', ';', '\t', '\r', '\n'};

    res.clear();
    std::string empty_str;
    bool is_delimiter, old_is_delimiter = true;
    int current_field = -1;
    for (auto &c: line) {
        is_delimiter = false;
        for (auto &delimiter: delimiter_list) {
            if (c == delimiter) {
                is_delimiter = true;
                break;
            }
        }
        if (is_delimiter) {
            old_is_delimiter = is_delimiter;
            continue;
        } else {
            if (old_is_delimiter) {
                current_field++;
                res.push_back(empty_str);
            }
            res[current_field] += c;
            old_is_delimiter = is_delimiter;
        }
    }
}

void PhyDB::WriteDef(string const &defFileName) {
    Si2WriteDef(this, defFileName);
}

void PhyDB::WriteCluster(string const &clusterFileName) {
    std::ofstream outfile(clusterFileName.c_str());
    if (outfile.is_open()) {
        std::cout << "writing cluster file: " << clusterFileName << "\n";
    } else {
        std::cout << "ERROR: cannot open output cluster file " << clusterFileName << std::endl;
        exit(1);
    }

    auto cluster_cols = this->GetClusterColsRef();
    for (auto cluster_col : cluster_cols) {
        outfile << "STRIP " << cluster_col.GetName() << std::endl;
        outfile << cluster_col.GetLX() << " " << cluster_col.GetUX() << " " << cluster_col.GetBotSignal() << std::endl;
        auto ly = cluster_col.GetLY();
        auto uy = cluster_col.GetUY();
        for (int i = 0; i < ly.size(); i++) {
            outfile << ly[i] << " " << uy[i] << std::endl;
        }
        outfile << "END " << cluster_col.GetName() << std::endl;
        outfile << std::endl;
    }
}
}
