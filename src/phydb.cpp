#include "phydb.h"

#include <cmath>

#include <fstream>

namespace phydb {

void PhyDB::setDatabaseMicron(int database_micron) {
    _tech.setDatabaseMicron(database_micron);
}

void PhyDB::setManufacturingGrid(double manufacture_grid) {
    _tech.setManufacturingGrid(manufacture_grid);
}

void PhyDB::addSite(std::string const &name, std::string const &class_name, double width, double height) {
    _tech.addSite(name, class_name, width, height);
}

bool PhyDB::isMacroExist(std::string const &macro_name) {
    return _tech.isMacroExist(macro_name);
}

Macro *PhyDB::addMacro(std::string &macro_name) {
    return _tech.addMacro(macro_name);
}

Macro *PhyDB::getMacroPtr(std::string const &macro_name) {
    return _tech.getMacroPtr(macro_name);
}

void PhyDB::setPlacementGrids(double placement_grid_value_x_, double placement_grid_value_y_) {
    _tech.setPlacementGrids(placement_grid_value_x_, placement_grid_value_y_);
}

bool PhyDB::isLayerExist(std::string const &layer_name) {
    return _tech.isLayerExist(layer_name);
}

Layer *PhyDB::addLayer(std::string &layer_name) {
    return _tech.addLayer(layer_name);
}

Layer *PhyDB::getLayerPtr(std::string const &layer_name) {
    return _tech.getLayerPtr(layer_name);
}

void PhyDB::setNwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
    _tech.setNwellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
}

void PhyDB::setPwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
    _tech.setPwellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
}

void PhyDB::setNpwellSpacing(double same_spacing, double any_spacing) {
    _tech.setNpwellSpacing(same_spacing, any_spacing);
}

MacroWell *PhyDB::addMacrowell(std::string &macro_name) {
    Macro *macro_ptr = getMacroPtr(macro_name);
    PhyDbExpects(macro_ptr != nullptr, "Macro does not exist, cannot add well info: " + macro_name);
    _tech.wells_.emplace_back(macro_ptr);
    macro_ptr->set_well_ptr(&(_tech.wells_.back()));
    return macro_ptr->well_ptr();
}

void PhyDB::setMacrowellShape(std::string &macro_name, bool is_N, double lx, double ly, double ux, double uy) {
    Macro *macro_ptr = getMacroPtr(macro_name);
    PhyDbExpects(macro_ptr != nullptr, "Macro does not exist, cannot add well info: " + macro_name);
    MacroWell *well = macro_ptr->well_ptr();

    if (well == nullptr) {
        well = addMacrowell(macro_name);
    }
    well->setWellRect(is_N, lx, ly, ux, uy);
}

void PhyDB::readLef(string const &lefFileName) {
    FILE *f;
    int res;

    lefrInitSession(1);

    lefrSetUserData((lefiUserData) &_tech);

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

    res = lefrRead(f, lefFileName.c_str(), (lefiUserData) &_tech);
    if (res != 0) {
        cout << "LEF parser returns an error!" << endl;
        exit(2);
    }
    fclose(f);

    lefrClear();
}

void PhyDB::readDef(string const &defFileName) {
    FILE *f;
    int res;

    defrInit();
    defrReset();

    defrInitSession(1);

    defrSetUserData((defiUserData) &_design);

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

    res = defrRead(f, defFileName.c_str(), (defiUserData) &_design, 1);
    if (res != 0) {
        cout << "DEF parser returns an error!" << endl;
        exit(2);
    }
    fclose(f);

    //numPins = readPinCnt;

    defrClear();
}

void PhyDB::readCell(string const &cellFileName) {
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
                //BOOST_LOG_TRIVIAL(info)   << "same diff spacing: " << same_diff_spacing << "\n any diff spacing: " << any_diff_spacing << "\n";
                setNpwellSpacing(same_diff_spacing, any_diff_spacing);
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
                    setNwellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
                } else {
                    setPwellLayer(width, spacing, op_spacing, max_plug_dist, overhang);
                }
            }
        }

        if (line.find("MACRO") != std::string::npos) {
            //BOOST_LOG_TRIVIAL(info)   << line << "\n";
            std::vector<std::string> macro_fields;
            StrSplit(line, macro_fields);
            std::string end_macro_flag = "END " + macro_fields[1];
            MacroWell *well = addMacrowell(macro_fields[1]);
            auto blk_type = getMacroPtr(macro_fields[1]);
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
                            setMacrowellShape(macro_fields[1], is_n, lx, ly, ux, uy);
                        }
                        getline(ist, line);
                    } while (line.find("END VERSION") == std::string::npos && !ist.eof());
                }
            } while (line.find(end_macro_flag) == std::string::npos && !ist.eof());
        }
    }
    if (!_tech.IsWellInfoSet()) {
        std::cout << "N/P well technology information not found!" << std::endl;
        exit(1);
    }
    //_tech.ReportWellShape();

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
