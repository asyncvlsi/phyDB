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

#include <fstream>
#include <iostream>

#include "logging.h"
#include "phydb.h"

using namespace phydb;

int main(int argc, char **argv) {
    PhyDBExpects(argc == 3,
                 "Please provide a LEF file and a technology configuration file");
    std::string lef_file_name(argv[1]);
    std::string tech_config_file_name(argv[2]);

    PhyDB phy_db;
    phy_db.ReadLef(lef_file_name);
    phy_db.ReadTechConfigFile(tech_config_file_name);

    phy_db.GetTechPtr()->ReportLayersTechConfig();
    phy_db.GetTechPtr()->SetResistanceUnit(true, true);
    phy_db.GetTechPtr()->SetCapacitanceUnit(true, true);

    return 0;
}
