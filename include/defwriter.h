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

#include <def/defrReader.hpp>
#include <def/defwWriter.hpp>
#include <def/defwWriterCalls.hpp>
#include <lef/lefrReader.hpp>
#include <lef/lefwWriter.hpp>
#include <lef/lefwWriterCalls.hpp>

#include "phydb.h"

namespace phydb {

int writeDefVersion(defwCallbackType_e c, defiUserData ud);
int writeDefBusBit(defwCallbackType_e c, defiUserData ud);
int writeDefDivider(defwCallbackType_e c, defiUserData ud);
int writeDefString(defwCallbackType_e c, defiUserData ud);
int writeDefVoid(defwCallbackType_e c, defiUserData ud);
int writeDefDieArea(defwCallbackType_e c, defiUserData ud);
int writeDefUnits(defwCallbackType_e c, defiUserData ud);
int writeDefTracks(defwCallbackType_e c, defiUserData ud);
int writeDefComponents(defwCallbackType_e c, defiUserData ud);
int writeDefIOPins(defwCallbackType_e c, defiUserData ud);
int writeDefNets(defwCallbackType_e c, defiUserData ud);
int writeDefSNets(defwCallbackType_e c, defiUserData ud);
int writeDefVias(defwCallbackType_e c, defiUserData ud);
int writeDefGcellGrid(defwCallbackType_e c, defiUserData ud);
int writeDefRow(defwCallbackType_e c, defiUserData ud);

void Si2WriteDef(PhyDB *phy_db_ptr, std::string const &defFileName);
void WriteCluter(PhyDB *phy_db_ptr, std::string const &clusterFileName);

}

#endif //PHYDB_INCLUDE_DEFWRITER_H_
