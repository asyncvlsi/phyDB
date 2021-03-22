#ifndef EOLSPACING_H
#define EOLSPACING_H

#include "header.h"

namespace phydb {

class EolSpacing {
  private:
    //metal layer
    float spacing_;
    float eol_width_;
    float eol_within_;
    float par_edge_;
    float par_within_;

  public:
    EolSpacing() : spacing_(0), eol_width_(0), eol_within_(0),
                   par_edge_(0), par_within_(0) {}

    //constructor for metal layer
    EolSpacing(float spacing, float eolWidth, float eolWithin,
               float parEdge, float parWithin) : 
        spacing_(spacing), 
        eol_width_(eolWidth),
        eol_within_(eolWithin), 
        par_edge_(parEdge), 
        par_within_(parWithin) {}
    
    void Reset();
};

ostream &operator<<(ostream &, const EolSpacing &);

}

#endif
