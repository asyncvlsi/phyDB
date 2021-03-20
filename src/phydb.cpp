#include "phydb.h"

namespace phydb {

void PhyDB::readLef(string lefFileName) {
    FILE* f;
    int res;

    lefrInitSession(1);

    lefrSetUserData ((lefiUserData)&tech);

    lefrSetMacroCbk(getLefMacros);
    lefrSetMacroBeginCbk(getLefMacroBegin);
    lefrSetMacroEndCbk(getLefMacroEnd);
    lefrSetUnitsCbk(getLefUnits);
    lefrSetManufacturingCbk(getLefManufacturingGrid);
    lefrSetPinCbk(getLefPins);
    lefrSetObstructionCbk(getLefObs);
    lefrSetLayerCbk(getLefLayers);
    lefrSetViaCbk(getLefVias);
    lefrSetViaRuleCbk(getLefViaGenerateRules);

    if ((f = fopen(lefFileName.c_str(),"r")) == 0) {
        cout <<"Couldn't open lef file" <<endl;
        exit(2);
    }

    res = lefrRead(f, lefFileName.c_str(), (lefiUserData)&lefDB);
    if (res != 0) {
        cout <<"LEF parser returns an error!" <<endl;
        exit(2);
    }
    fclose(f);

    lefrClear();
}

void PhyDB::readDef(string defFileName) {
    FILE* f;
    int res;

    defrInit();
    defrReset();

    defrInitSession(1);

    defrSetUserData((defiUserData)&_design);

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


    if ((f = fopen(defFileName.c_str(),"r")) == 0) {
        cout <<"Couldn't open def file" <<endl;
        exit(2);
    }

    res = defrRead(f, defFileName.c_str(), (defiUserData)&_design, 1);
    if (res != 0) {
        cout <<"DEF parser returns an error!" <<endl;
        exit(2);
    }
    fclose(f);

    //numPins = readPinCnt;

    defrClear();
}





}



