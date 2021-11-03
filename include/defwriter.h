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
#ifndef PHYDB_INCLUDE_DEFWRITER_H_
#define PHYDB_INCLUDE_DEFWRITER_H_

#include "phydb.h"

namespace phydb {

void Si2WriteDef(PhyDB *phy_db_ptr, std::string const &def_file_name);
void WriteCluter(PhyDB *phy_db_ptr, std::string const &cluster_file_name);

}

#endif //PHYDB_INCLUDE_DEFWRITER_H_
