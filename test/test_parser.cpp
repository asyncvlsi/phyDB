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
