#ifndef GCELLGRID_H
#define GCELLGRID_H

#include "header.h"

namespace phydb {

class GcellGrid {
  public:
    string direction_;
    int start_;
    int numBoundaries_;
    int step_;

    GcellGrid() : start_(0), numBoundaries_(0), step_(0) {}

    friend ostream &operator<<(ostream &, const GcellGrid &);

};

ostream &operator<<(ostream &, const GcellGrid &);

}

#endif
