#ifndef GCELLGRID_H
#define GCELLGRID_H

#include "logging.h"
#include "enumtypes.h"

namespace phydb {

class GcellGrid {
  private:
    XYDirection direction_;
    int start_;
    int numBoundaries_;
    int step_;

  public:
    GcellGrid() : start_(0), numBoundaries_(0), step_(0) {}
    GcellGrid(XYDirection direction, int start, int numBoundaries, int step) : 
      direction_(direction), start_(start), numBoundaries_(numBoundaries), step_(step) {}

    void SetDirection(XYDirection );
    void SetStart(int );
    void SetNBoundaries(int );
    void SetStep(int );
    
    XYDirection GetDirection() const;
    int GetStart() const;
    int GetNBoundaries() const;
    int GetStep() const;

    void Report() const;

    friend ostream &operator<<(ostream &, const GcellGrid &);

};

ostream &operator<<(ostream &, const GcellGrid &);

}

#endif
