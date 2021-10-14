#ifndef PHYDB_INCLUDE_TECHCONFIGPARSER_H_
#define PHYDB_INCLUDE_TECHCONFIGPARSER_H_

#include "phydb.h"

namespace phydb {

bool ReadTechnologyConfigurationFile(PhyDB *phy_db_ptr, std::string const &tech_config_file_name);

}

#endif //PHYDB_INCLUDE_TECHCONFIGPARSER_H_
