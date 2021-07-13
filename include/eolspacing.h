#ifndef EOLSPACING_H
#define EOLSPACING_H

#include "logging.h"

namespace phydb {

class EolSpacing {
  private:
    //metal layer
    double spacing_;
    double eol_width_;
    double eol_within_;
    double par_edge_;
    double par_within_;

  public:
    EolSpacing() : spacing_(0), eol_width_(0), eol_within_(0),
                   par_edge_(0), par_within_(0) {}

    //constructor for metal layer
    EolSpacing(double spacing, double eolWidth, double eolWithin,
               double parEdge, double parWithin) :
        spacing_(spacing), 
        eol_width_(eolWidth),
        eol_within_(eolWithin), 
        par_edge_(parEdge), 
        par_within_(parWithin) {}
    
    double GetSpacing()   {return spacing_;}
    double GetEOLWidth()  {return eol_width_;}
    double GetEOLWithin() {return eol_within_;}
    double GetParEdge()   {return par_edge_;}
    double GetParWithin() {return par_within_;}

    void Reset();
};

ostream &operator<<(ostream &, const EolSpacing &);

}

#endif
