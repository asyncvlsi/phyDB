#ifndef SI2PARSER_H_
#define SI2PARSER_H_

#include "phydb.h"
#include <lefrReader.hpp>
#include <defrReader.hpp>
#include <defwWriter.hpp>
#include <defwWriterCalls.hpp>

namespace phydb {

int getLefSite(lefrCallbackType_e type, lefiSite *site, lefiUserData data);
int getLefMacrosBegin(lefrCallbackType_e type, const char *str, lefiUserData data);
int getLefMacros(lefrCallbackType_e, lefiMacro *, lefiUserData);
int getLefMacrosEnd(lefrCallbackType_e type, const char *str, lefiUserData data);
int getLefString(lefrCallbackType_e, const char *, lefiUserData);
int getLefUnits(lefrCallbackType_e, lefiUnits *, lefiUserData);
int getLefManufacturingGrid(lefrCallbackType_e, double, lefiUserData);
int getLefPins(lefrCallbackType_e, lefiPin *, lefiUserData);
int getLefObs(lefrCallbackType_e, lefiObstruction *, lefiUserData);
int getLefLayers(lefrCallbackType_e, lefiLayer *, lefiUserData);
int getLefVias(lefrCallbackType_e, lefiVia *, lefiUserData);
int getLefViaRuleGenerates(lefrCallbackType_e, lefiViaRule *, lefiUserData);

int getDefVersion(defrCallbackType_e, double, defiUserData);
int getDefBusBit(defrCallbackType_e, const char *, defiUserData);
int getDefDivider(defrCallbackType_e, const char *, defiUserData);
int getDefString(defrCallbackType_e, const char *, defiUserData);
int getDefVoid(defrCallbackType_e, void *, defiUserData);
int getDefDieArea(defrCallbackType_e, defiBox *, defiUserData);
int getDefUnits(defrCallbackType_e, double, defiUserData);
int getDefTracks(defrCallbackType_e, defiTrack *, defiUserData);
int getDefComponents(defrCallbackType_e, defiComponent *, defiUserData);
int getDefIOPins(defrCallbackType_e, defiPin *, defiUserData);
int getDefNets(defrCallbackType_e, defiNet *, defiUserData);
int getDefSNets(defrCallbackType_e, defiNet *, defiUserData);
int getDefVias(defrCallbackType_e, defiVia *, defiUserData);
int getDefGcellGrid(defrCallbackType_e, defiGcellGrid *, defiUserData);
int getDefRow(defrCallbackType_e, defiRow *, defiUserData);

void Si2ReadLef(PhyDB *phy_db_ptr, string const &lefFileName);
void Si2ReadDef(PhyDB *phy_db_ptr, string const &defFileName);

}


#endif //SI2PARSER_H_
