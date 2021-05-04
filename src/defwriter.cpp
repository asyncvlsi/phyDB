#include <time.h>

#include "defwriter.h"

namespace phydb {

int WriteVersion(defwCallbackType_e c, defiUserData ud) {
    int status;
    double version = ((PhyDB*) ud)->GetDefVersion();

    int int_version = version * 10;
    status = defwVersion(int_version / 10, int_version % 10);
    defwNewLine();
    return 0;
}

int WriteBusBit(defwCallbackType_e c, defiUserData ud) {
    int status;
    status = defwBusBitChars(((PhyDB*) ud)->GetDefBusBitChar().c_str());
    defwNewLine();
    return 0;
}

int WriteDivider(defwCallbackType_e c, defiUserData ud) {
    int status;
    status = defwDividerChar(((PhyDB*) ud)->GetDefDividerChar().c_str());
    defwNewLine();
    return 0;
}

int WriteDesignName(defwCallbackType_e c, defiUserData ud) {
  int status;

  status = defwDesignName(((PhyDB*) ud)->GetDefName().c_str());
  defwNewLine();
  return 0;
}

int WriteDesignEnd(defwCallbackType_e c, defiUserData ud) {
    defwNewLine();
    defwEnd();
    return 0;
}

int WriteUnitsDistanceMicrons(defwCallbackType_e c, defiUserData ud) {
    int status;

    status = defwUnits(((PhyDB*) ud)->GetDesignPtr()->GetUnitsDistanceMicrons());
    defwNewLine();
    return 0;
}

int WriteDieArea(defwCallbackType_e c, defiUserData ud) {
    int status;
    Rect2D<int> dieArea = ((PhyDB*) ud)->GetDieArea(); 
    status = defwDieArea(dieArea.LLX(), dieArea.LLY(), dieArea.URX(), dieArea.URY());
    defwNewLine();
    return 0;
}

int WriteRows(defwCallbackType_e type, defiUserData data) {
    int status;
    auto rows = ((PhyDB*) data)->GetRowVec();
    for(auto row : rows) {
        status = defwRowStr(row.name_.c_str(), row.site_name_.c_str(), row.orig_x_, row.orig_y_,
            row.site_orient_.c_str(), row.num_x_, row.num_y_, row.step_x_, row.step_y_); 
    }

    return 0;
}

/*int WriteDefString(defwCallbackType_e c, defiUserData ud);
int WriteDefVoid(defwCallbackType_e c, defiUserData ud);
int WriteDefDieArea(defwCallbackType_e c, defiUserData ud);
int WriteDefUnits(defwCallbackType_e c, defiUserData ud);
int WriteDefTracks(defwCallbackType_e c, defiUserData ud);
int WriteDefComponents(defwCallbackType_e c, defiUserData ud);
int WriteDefIOPins(defwCallbackType_e c, defiUserData ud);
int WriteDefNets(defwCallbackType_e c, defiUserData ud);
int WriteDefSNets(defwCallbackType_e c, defiUserData ud);
int WriteDefVias(defwCallbackType_e c, defiUserData ud);
int WriteDefGcellGrid(defwCallbackType_e c, defiUserData ud);
int WriteDefRow(defwCallbackType_e c, defiUserData ud);*/

std::string GetCurrentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}


void Si2WriteDef(PhyDB *phy_db_ptr, string const &defFileName) {
    FILE* f;
    int res;

    if ((f = fopen(defFileName.c_str(),"w")) == 0) {
        cout <<"Couldn't open Write def file" <<endl;
        exit(2);
    }
    std::cout << "Writing def to " << defFileName << std::endl;
    int status = defwInitCbk(f);

    defwSetVersionCbk (WriteVersion);
    defwSetBusBitCbk (WriteBusBit);
    defwSetDividerCbk (WriteDivider);

    defwSetDesignCbk (WriteDesignName);
    defwSetDesignEndCbk (WriteDesignEnd);

    defwSetUnitsCbk (WriteUnitsDistanceMicrons);
    defwSetDieAreaCbk (WriteDieArea);
    defwSetRowCbk (WriteRows);

    /*defwSetTrackCbk (WriteTracks);
    defwSetComponentCbk (WriteComponents);
    defwSetPinCbk (WriteIOPins);
    defwSetNetCbk (WriteNets);

    defwSetSNetCbk (WriteSNet);*/


    fprintf(f, "###########################\n");
    fprintf(f, "# Written by PhyDB at ");
    fprintf(f, GetCurrentDateTime().c_str());
    fprintf(f, "\n");
    fprintf(f, "###########################\n");

    res = defwWrite(f, defFileName.c_str(), (defiUserData) phy_db_ptr);
    if (res != 0) {
        cout << "DEF Writer returns an error!" << endl;
        exit(2);
    }
    fclose(f);
    std::cout << "def writing completes" << std::endl;
}

}

