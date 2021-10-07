#ifndef PHYDB_INCLUDE_DEFWRITER_H_
#define PHYDB_INCLUDE_DEFWRITER_H_

#include <defrReader.hpp>
#include <defwWriter.hpp>
#include <defwWriterCalls.hpp>
#include <lefrReader.hpp>
#include <lefwWriter.hpp>
#include <lefwWriterCalls.hpp>

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
