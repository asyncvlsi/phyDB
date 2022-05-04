/*******************************************************************************
 *
 * Copyright (c) 2021 Jiayuan He, Yihang Yang
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 ******************************************************************************/

#include "techconfigparser.h"

#include <fstream>

#include "phydb/common/helper.h"

namespace phydb {

int corner_index = -1;

void ReadHeader(std::ifstream &ist) {
  std::string line;
  do {
    getline(ist, line);
  } while (line.empty());
  std::string header("Extraction Rules for OpenRCX");
  if (line != header) {
    PhyDBExpects(false, "Expect: " << header);
  }
}

void ReadDiagmodelOnOff(PhyDB *phy_db_ptr, std::ifstream &ist) {
  std::string line;
  std::vector<std::string> words;
  auto &tech_config = phy_db_ptr->tech().GetTechConfigRef();
  do {
    getline(ist, line);
  } while (line.empty());
  if (line.find("DIAGMODEL") != std::string::npos) {
    StrTokenize(line, words);
    PhyDBExpects(words.size() == 2, "Expect: DIAGMODEL ON/OFF");
    if (words[1] == "ON") {
      tech_config.SetDiagModelOn(true);
    } else if (words[1] == "OFF") {
      tech_config.SetDiagModelOn(false);
    } else {
      PhyDBExpects(false, "Incorrect option, expect ON or OFF");
    }
  } else {
    PhyDBExpects(false, "Couldn't find: DIAGMODEL ON/OFF");
  }
}

void ReadLayerCount(PhyDB *phy_db_ptr, std::ifstream &ist) {
  std::string line;
  std::vector<std::string> words;
  auto &tech = phy_db_ptr->tech();
  do {
    getline(ist, line);
  } while (line.empty());
  if (line.find("LayerCount") != std::string::npos) {
    StrTokenize(line, words);
    PhyDBExpects(words.size() == 2, "Expect: LayerCount num");
    int number_of_layers = -1;
    try {
      number_of_layers = std::stoi(words[1]);
    } catch (...) {
      PhyDBExpects(false,
                   "Unable to convert string to int: " << words[1]);
    }
    tech.SetTechConfigLayerCount(number_of_layers);
  } else {
    PhyDBExpects(false, "Couldn't find: DIAGMODEL ON/OFF");
  }
}

void ReadDensityRate(PhyDB *phy_db_ptr, std::ifstream &ist) {
  std::string line;
  std::vector<std::string> words;
  auto &tech_config = phy_db_ptr->tech().GetTechConfigRef();
  do {
    getline(ist, line);
  } while (line.empty());
  if (line.find("DensityRate") != std::string::npos) {
    StrTokenize(line, words);
    PhyDBExpects(words.size() >= 3,
                 "Expect: DensityRate model_numbers rate ...");
    int model_numbers = -1;
    try {
      model_numbers = std::stoi(words[1]);
    } catch (...) {
      PhyDBExpects(false,
                   "Unable to convert string to int: " << words[1]);
    }
    tech_config.SetModelCount(model_numbers);
    PhyDBExpects((int) words.size() == model_numbers + 2,
                 "the number of rates does not match the expected number");
    for (int i = 0; i < model_numbers; ++i) {
      try {
        tech_config.AddDataRate(std::stod(words[i + 2]));
      } catch (...) {
        PhyDBExpects(false, "Unable to convert string to int: " << words[i + 2]);
      }
    }
  } else {
    PhyDBExpects(false, "Couldn't find: DensityRate ...");
  }
}

void InitializeDensityModel(PhyDB *phy_db_ptr, std::ifstream &ist) {
  std::string line;
  std::vector<std::string> words;
  auto &tech = phy_db_ptr->tech();
  do {
    getline(ist, line);
  } while (line.empty() && !ist.eof());
  if (line.find("DensityModel") != std::string::npos) {
    StrTokenize(line, words);
    PhyDBExpects(words.size() == 2, "Expect: DensityModel num");
    try {
      corner_index = std::stoi(words[1]);
    } catch (...) {
      PhyDBExpects(false,
                   "Unable to convert string to int: " << words[1]);
    }
    tech.AddTechConfigCorner(corner_index);
  } else {
    if (ist.eof()) return;
    PhyDBExpects(false, "Couldn't find: DensityModel model_index");
  }
}

void ReadConfigTable(ConfigTable &config_table, std::ifstream &ist) {
  std::string line;
  std::vector<std::string> words;
  do {
    getline(ist, line);
  } while (line.empty());
  int pre_sz = -1;
  if (line.find("DIST count") != std::string::npos) {
    StrTokenize(line, words);
    try {
      pre_sz = std::stoi(words[2]);
      PhyDBExpects(pre_sz >= 0, "Negative table size?");
      config_table.GetTable().reserve(pre_sz);
    } catch (...) {
      PhyDBExpects(false,
                   "Unable to convert string to int: " << words[2]);
    }
    double width = -1;
    try {
      width = std::stod(words[4]);
      config_table.SetWidth(width);
    } catch (...) {
      PhyDBExpects(false,
                   "Unable to convert string to double: " << words[4]);
    }
  } else {
    PhyDBExpects(false, "Couldn't find: DIST count ...");
  }

  while (!ist.eof()) {
    getline(ist, line);
    if (line.empty()) continue;
    if (line.find("END DIST") != std::string::npos) break;
    StrTokenize(line, words);
    double distance = -1;
    double coupling_cap = -1;
    double fringe_cap = -1;
    double res = -1;
    try {
      distance = std::stod(words[0]);
      coupling_cap = std::stod(words[1]);
      fringe_cap = std::stod(words[2]);
      res = std::stod(words[3]);
    } catch (...) {
      PhyDBExpects(false,
                   "Unable to convert string to double: " << line);
    }
    config_table.AddEntry(distance, coupling_cap, fringe_cap, res);
  }
}

void ReadDensityModel(PhyDB *phy_db_ptr, std::ifstream &ist) {
  std::string line;
  std::vector<std::string> words;
  auto &tech = phy_db_ptr->tech();
  while (!ist.eof()) {
    getline(ist, line);
    if (line.empty()) continue;
    if (line.find("END DensityModel") != std::string::npos) {
      std::cout << line << "\n";
      return;
    }

    StrTokenize(line, words);
    bool is_table_start = (words[0] == "Metal") &&
        ((words.size() == 4) || (words.size() == 6));
    if (is_table_start) {
      TableType type = BAD_TABLE_TYPE;
      int layer_index = -1;
      int index0 = -1;
      int index1 = -1;
      if (words.size() == 4) {
        if (words[2] == "RESOVER") {
          type = RES_OVER;
        } else if (words[2] == "OVER") {
          type = CAP_OVER;
        } else if (words[2] == "UNDER") {
          type = CAP_UNDER;
        } else if (words[2] == "DIAGUNDER") {
          type = CAP_DIAGUNDER;
        } else {
          PhyDBExpects(false, "Unknown table type: " << words[2]);
        }
      } else if (words.size() == 6) {
        PhyDBExpects(
            words[2] == "OVER" && words[4] == "UNDER",
            "Unknown table type: " << words[2] << " " << words[4]
        );
        type = CAP_OVERUNDER;
      } else {
        PhyDBExpects(false, "You cannot reach here");
      }
      try {
        layer_index = std::stoi(words[1]) - 1;
        index0 = std::stoi(words[3]) - 1;
        if (words.size() == 6) {
          index0 = std::stoi(words[5]) - 1;
        }
      } catch (...) {
        PhyDBExpects(false,
                     "Unable to convert string to int: " << line);
      }
      PhyDBExpects(type != BAD_TABLE_TYPE, "Bad table type?");
      auto &config_table = tech.InitConfigTable(
          type,
          layer_index,
          index0,
          index1,
          corner_index
      );
      ReadConfigTable(config_table, ist);
    }
  }
}

void ReadDensityModels(PhyDB *phy_db_ptr, std::ifstream &ist) {
  while (!ist.eof()) {
    InitializeDensityModel(phy_db_ptr, ist);
    ReadDensityModel(phy_db_ptr, ist);
  }
}

bool ReadTechnologyConfigurationFile(
    PhyDB *phy_db_ptr,
    std::string const &tech_config_file_name
) {
  PhyDBExpects(phy_db_ptr != nullptr, "PhyDB pointer is a nullptr");
  std::ifstream ist(tech_config_file_name.c_str());
  PhyDBExpects(ist.is_open(),
               "Cannot open input file " << tech_config_file_name);

  ReadHeader(ist);
  ReadDiagmodelOnOff(phy_db_ptr, ist);
  ReadLayerCount(phy_db_ptr, ist);
  ReadDensityRate(phy_db_ptr, ist);
  ReadDensityModels(phy_db_ptr, ist);

  return true;
}

}