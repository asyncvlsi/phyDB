#include "lefdefwriter.h"

namespace phydb {

int writeLefManufacturingGrid(lefwCallbackType_e c, lefiUserData ud);
/*int writeLefSite(lefwCallbackType_e c, lefiUserData ud);
int writeLefMacrosBegin(lefwCallbackType_e c, lefiUserData ud);
int writeLefMacros(lefwCallbackType_e c, lefiUserData ud);
int writeLefMacrosEnd(lefwCallbackType_e c, lefiUserData ud);
int writeLefString(lefwCallbackType_e c, lefiUserData ud);
int writeLefUnits(lefwCallbackType_e c, lefiUserData ud);

int writeLefPins(lefwCallbackType_e c, lefiUserData ud);
int writeLefObs(lefwCallbackType_e c, lefiUserData ud);
int writeLefLayers(lefwCallbackType_e c, lefiUserData ud);
int writeLefVias(lefwCallbackType_e c, lefiUserData ud);
int writeLefViaRuleGenerates(lefwCallbackType_e c, lefiUserData ud);

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
int writeDefRow(defwCallbackType_e c, defiUserData ud);*/

void Si2WriteLef(PhyDB *phy_db_ptr, string const &lefFileName) {
    FILE *f;
    int res;

    if ((f = fopen(lefFileName.c_str(),"w")) == 0) {
        cout <<"Couldn't open write lef file" <<endl;
        exit(2);
    }

    int status = lefwInitCbk(f);

    /*lefwSetManufacturingCbk(writeLefManufacturingGrid);
    lefwSetUnitsCbk(writeLefUnits);

    lefwSetMacroCbk(writeLefMacros);
    lefwSetMacroBeginCbk(writeLefMacrosBegin);
    lefwSetMacroEndCbk(writeLefMacrosEnd);
    lefwSetSiteCbk(writeLefSite);
    lefwSetPinCbk(writeLefPins);
    lefwSetObstructionCbk(writeLefObs);
    lefwSetLayerCbk(writeLefLayers);
    lefwSetViaCbk(writeLefVias);
    lefwSetViaRuleCbk(writeLefViaRuleGenerates);*/

    if ((f = fopen(lefFileName.c_str(), "w")) == 0) {
        cout << "Couldn't open lef file" << endl;
        exit(2);
    }

    res = lefwWrite(f, lefFileName.c_str(), (lefiUserData) phy_db_ptr);
    if (res != 0) {
        cout << "LEF writer returns an error!" << endl;
        exit(2);
    }
    fclose(f);
}

void Si2WriteDef(PhyDB *phy_db_ptr, string const &defFileName) {
    FILE* f;
    int res;

    if ((f = fopen(defFileName.c_str(),"w")) == 0) {
        cout <<"Couldn't open write def file" <<endl;
        exit(2);
    }

    int status = defwInitCbk(f);

    /*defwSetVersionCbk (writeVersion);
    defwSetBusBitCbk (writeBusBit);
    defwSetDividerCbk (writeDivider);

    defwSetDesignCbk (writeDesignName);
    defwSetDesignEndCbk (writeDesignEnd);

    defwSetUnitsCbk (writeUnits);
    defwSetDieAreaCbk (writeDieArea);
    defwSetRowCbk (writeRows);
    defwSetTrackCbk (writeTracks);
    defwSetComponentCbk (writeComponents);
    defwSetPinCbk (writeIOPins);
    defwSetNetCbk (writeNets);

    defwSetSNetCbk (writeSNet);*/

    res = defwWrite(f, defFileName.c_str(), (defiUserData) phy_db_ptr);
    if (res != 0) {
        cout << "DEF writer returns an error!" << endl;
        exit(2);
    }

    fclose(f);
}





}

