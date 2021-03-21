#ifndef PHYDB_H
#define PHYDB_H

#include "design.h"
#include "tech.h"

namespace phydb {

class PhyDB {
  private:
    Tech _tech;
    Design _design;

  public:
    PhyDB() {}

    /************************************************
    * The following APIs are for information in LEF
    * ************************************************/
    void setDatabaseMicron(int database_micron);
    void setManufacturingGrid(double manufacture_grid);
    void addSite(std::string const &name, std::string const &class_name, double width, double height);
    void setPlacementGrids(double placement_grid_value_x_, double placement_grid_value_y_);

    bool isLayerExist(std::string const &layer_name);
    Layer *addLayer(std::string &layer_name);
    Layer *getLayerPtr(std::string const &layer_name);

    //TODO: APIs for adding VIAs

    bool isMacroExist(std::string const &macro_name);
    Macro *addMacro(std::string &macro_name);
    Macro *getMacroPtr(std::string const &macro_name);

    /************************************************
    * The following APIs are for information in DEF
    * ************************************************/

    /************************************************
    * The following APIs are for information in CELL
    * ************************************************/
    void setNwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang);
    void setPwellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang);
    void setNpwellSpacing(double same_spacing, double any_spacing);
    MacroWell *addMacrowell(std::string &macro_name);
    void setMacrowellShape(std::string &macro_name, bool is_N, double lx, double ly, double ux, double uy);

    /************************************************
    * The following APIs are for file IO
    * ************************************************/

    void readLef(string const &lefFileName);
    void readDef(string const &defFileName);
    void readCell(string const &cellFileName);

    /****helper functions****/
    // splits a line into many word tokens
    void StrSplit(std::string &line, std::vector<std::string> &res);
};

}

#endif
