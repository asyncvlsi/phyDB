#include "phydb.h"

#include <cmath>

#include <fstream>

namespace phydb {

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

bool PhyDB::IsLayerExist(std::string const &layer_name) {
    return tech_.IsLayerExist(layer_name);
}

Layer *PhyDB::AddLayer(std::string &layer_name) {
    return tech_.AddLayer(layer_name);
}

Layer *PhyDB::GetLayerPtr(std::string const &layer_name) {
    return tech_.GetLayerPtr(layer_name);
}

bool PhyDB::IsMacroExist(std::string const &macro_name) {
    return tech_.IsMacroExist(macro_name);
}

Macro *PhyDB::AddMacro(std::string &macro_name) {
    return tech_.AddMacro(macro_name);
}

Macro *PhyDB::GetMacroPtr(std::string const &macro_name) {
    return tech_.GetMacroPtr(macro_name);
}

bool PhyDB::IsLefViaExist(std::string const &name) {
    return tech_.IsLefViaExist(name);
}

LefVia *PhyDB::AddLefVia(std::string &via_name) {
    return tech_.AddLefVia(via_name);
}

LefVia *PhyDB::GetLefViaPtr(std::string const &via_name) {
    return tech_.GetLefViaPtr(via_name);
}

bool PhyDB::IsViaRuleGenerateExist(std::string const &name) {
    return tech_.IsViaRuleGenerateExist(name);
}

ViaRuleGenerate* PhyDB::AddViaRuleGenerate(std::string &name) {
    return tech_.AddViaRuleGenerate(name);
}

ViaRuleGenerate* PhyDB::GetViaRuleGeneratePtr(std::string const &name) {
    return tech_.GetViaRuleGeneratePtr(name);
}

void PhyDB::SetUnitsDistanceMicrons(int distance_microns) {
    design_.SetUnitsDistanceMicrons(distance_microns);
}
void PhyDB::SetDieArea(int lower_x, int lower_y, int upper_x, int upper_y) {
    design_.SetDieArea(lower_x, lower_y, upper_x, upper_y);
}

bool PhyDB::IsComponentExist(std::string &component_name) {
    return design_.IsComponentExist(component_name);
}

Track* PhyDB::AddTrack(string& direction, int start, int nTracks, int step, vector<string>& layer_names) {
    design_.AddTrack(direction, start, nTracks, step, layer_names);
}

vector<Track>& PhyDB::GetTrackVec() {
    return design_.GetTrackVec();
}

Row* PhyDB::AddRow(string& name, string& site_name, string& site_orient, int origX, int origY, int numX, 
            int numY, int stepX, int stepY) {
    design_.AddRow(name, site_name, site_orient, origX, origY, numX, numY, stepX, stepY);
}

vector<Row>& PhyDB::GetRowVec() {
    return design_.GetRowVec();
}

Component *PhyDB::AddComponent(std::string &comp_name, std::string &macro_name, std::string &place_status,
                               int llx, int lly, std::string &orient) {
    return design_.AddComponent(comp_name, macro_name, place_status, llx, lly, orient);
}

Component *PhyDB::GetComponentPtr(std::string &comp_name) {
    return design_.GetComponentPtr(comp_name);
}

bool PhyDB::IsIoPinExist(std::string &iopin_name) {
    return design_.IsIoPinExist(iopin_name);
}

bool PhyDB::IsDefViaExist(std::string const &name) {
    return design_.IsDefViaExist(name);
}

DefVia *PhyDB::AddDefVia(std::string &via_name) {
    return design_.AddDefVia(via_name);
}

DefVia *PhyDB::GetDefViaPtr(std::string const &via_name) {
    return design_.GetDefViaPtr(via_name);
}

IOPin *PhyDB::AddIoPin(std::string &iopin_name, std::string &place_status,
                       std::string &signal_use, std::string &signal_direction,
                       int lx, int ly) {
    return design_.AddIoPin(iopin_name, place_status, signal_use, signal_direction, lx, ly);
}

IOPin *PhyDB::GetIoPinPtr(std::string &iopin_name) {
    return design_.GetIoPinPtr(iopin_name);
}

bool PhyDB::IsNetExist(std::string &net_name) {
    return design_.IsNetExist(net_name);
}

Net *PhyDB::AddNet(std::string &net_name, double weight) {
    return  design_.AddNet(net_name, weight);
}

void PhyDB::AddIoPinToNet(std::string &iopin_name, std::string &net_name) {
    PhyDbExpects(IsIoPinExist(iopin_name), "Cannot add a nonexistent iopin to a net: " + iopin_name);
    PhyDbExpects(IsNetExist(net_name), "Cannot add iopin to a nonexistent Net: " + net_name);
    design_.AddIoPinToNet(iopin_name, net_name);
}

void PhyDB::AddCompPinToNet(std::string &comp_name, std::string &pin_name, std::string &net_name) {
    PhyDbExpects(IsNetExist(net_name), "Cannot add a component pin to a nonexistent Net: " + net_name);
    PhyDbExpects(IsComponentExist(comp_name), "Cannot add a nonexistent component to a net: " + comp_name);

    Component *comp_ptr = GetComponentPtr(comp_name);
    std::string macro_name = comp_ptr->GetMacroName();
    PhyDbExpects(IsMacroExist(macro_name), "Macro of this component does not exist: " + comp_name + ", macro name:" + macro_name);

    Macro *macro_ptr = GetMacroPtr(macro_name);
    PhyDbExpects(macro_ptr->IsPinExist(pin_name), "Macro " + macro_name + " does not contain a pin with name " + pin_name);
    design_.AddCompPinToNet(comp_name, pin_name, net_name);
}

Net *PhyDB::GetNetPtr(std::string &net_name) {
    return design_.GetNetPtr(net_name);
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
    PhyDbExpects(macro_ptr != nullptr, "Macro does not exist, cannot add well info: " + macro_name);
    tech_.wells_.emplace_back(macro_ptr);
    macro_ptr->SetWellPtr(&(tech_.wells_.back()));
    return macro_ptr->GetWellPtr();
}

void PhyDB::SetMacrowellShape(std::string &macro_name, bool is_N, double lx, double ly, double ux, double uy) {
    Macro *macro_ptr = GetMacroPtr(macro_name);
    PhyDbExpects(macro_ptr != nullptr, "Macro does not exist, cannot add well info: " + macro_name);
    MacroWell *well = macro_ptr->GetWellPtr();

    if (well == nullptr) {
        well = AddMacrowell(macro_name);
    }
    well->SetWellRect(is_N, lx, ly, ux, uy);
}

void PhyDB::ReadLef(string const &lefFileName) {
    FILE *f;
    int res;

    lefrInitSession(1);

    lefrSetUserData((lefiUserData) &tech_);

    lefrSetMacroCbk(getLefMacros);
    lefrSetMacroBeginCbk(getLefMacrosBegin);
    lefrSetMacroEndCbk(getLefMacrosEnd);
    lefrSetUnitsCbk(getLefUnits);
    lefrSetManufacturingCbk(getLefManufacturingGrid);
    lefrSetPinCbk(getLefPins);
    lefrSetObstructionCbk(getLefObs);
    lefrSetLayerCbk(getLefLayers);
    lefrSetViaCbk(getLefVias);
    lefrSetViaRuleCbk(getLefViaGenerateRules);

    if ((f = fopen(lefFileName.c_str(), "r")) == 0) {
        cout << "Couldn't open lef file" << endl;
        exit(2);
    }

    res = lefrRead(f, lefFileName.c_str(), (lefiUserData) &tech_);
    if (res != 0) {
        cout << "LEF parser returns an error!" << endl;
        exit(2);
    }
    fclose(f);

    lefrClear();
}

void PhyDB::ReadDef(string const &defFileName) {
    FILE *f;
    int res;

    defrInit();
    defrReset();

    defrInitSession(1);

    defrSetUserData((defiUserData) &design_);

    defrSetVersionCbk(getDefVersion);
    defrSetBusBitCbk(getDefBusBit);
    defrSetDividerCbk(getDefDivider);

    defrSetDesignCbk(getDefString);
    defrSetDesignEndCbk(getDefVoid);
    defrSetDieAreaCbk(getDefDieArea);
    defrSetUnitsCbk(getDefUnits);
    defrSetRowCbk(getDefRow);
    defrSetTrackCbk(getDefTracks);
    defrSetComponentCbk(getDefComponents);
    defrSetPinCbk(getDefIOPins);

    defrSetSNetCbk(getDefSNets);
    defrSetAddPathToNet();
    defrSetNetCbk(getDefNets);

    defrSetViaCbk(getDefVias);
    defrSetGcellGridCbk(getDefGcell);

    if ((f = fopen(defFileName.c_str(), "r")) == 0) {
        cout << "Couldn't open def file" << endl;
        exit(2);
    }

    res = defrRead(f, defFileName.c_str(), (defiUserData) &design_, 1);
    if (res != 0) {
        cout << "DEF parser returns an error!" << endl;
        exit(2);
    }
    fclose(f);

    //numPins = readPinCnt;

    defrClear();
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
                //BOOST_LOG_TRIVIAL(info)   << "same diff spacing: " << same_diff_spacing << "\n any diff GetSpacing: " << any_diff_spacing << "\n";
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
            //BOOST_LOG_TRIVIAL(info)   << line << "\n";
            std::vector<std::string> macro_fields;
            StrSplit(line, macro_fields);
            std::string end_macro_flag = "END " + macro_fields[1];
            MacroWell *well = AddMacrowell(macro_fields[1]);
            auto blk_type = GetMacroPtr(macro_fields[1]);
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
                            SetMacrowellShape(macro_fields[1], is_n, lx, ly, ux, uy);
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

}
