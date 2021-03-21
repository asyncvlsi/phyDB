#ifndef EOLSPACING_H
#define EOLSPACING_H

#include "header.h"

namespace phydb {

class EOLSpacing {
  private:
    //metal layer
    float spacing_;
    float eol_width_;
    float eol_within_;
    float par_edge_;
    float par_within_;

    //cut layer
    int adjacent_cuts_;
    float cut_within_;

  public:
    EOLSpacing() : spacing_(0), eol_width_(0), eol_within_(0),
                   par_edge_(0), par_within_(0), adjacent_cuts_(0), cut_within_(0) {}

    //constructor for metal layer
    EOLSpacing(float spacing, float eolWidth, float eolWithin,
               float parEdge, float parWithin) : spacing_(spacing), eol_width_(eolWidth),
                                                 eol_within_(eolWithin), par_edge_(parEdge), par_within_(parWithin) {}

    //constructor for cut layer
    EOLSpacing(int adjacentCuts, float cutWithin) : adjacent_cuts_(adjacentCuts),
                                                    cut_within_(cutWithin) {}

    void Reset();
};

ostream &operator<<(ostream &, const EOLSpacing &);

}

#endif
