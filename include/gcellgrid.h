#ifndef PHYDB_INCLUDE_GCELLGRID_H_
#define PHYDB_INCLUDE_GCELLGRID_H_

#include "enumtypes.h"
#include "logging.h"

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
        direction_(direction),
        start_(start),
        numBoundaries_(numBoundaries),
        step_(step) {}

    void SetDirection(XYDirection);
    void SetStart(int);
    void SetNBoundaries(int);
    void SetStep(int);

    XYDirection GetDirection() const;
    int GetStart() const;
    int GetNBoundaries() const;
    int GetStep() const;

    void Report() const;

    friend std::ostream &operator<<(std::ostream &, const GcellGrid &);

};

std::ostream &operator<<(std::ostream &, const GcellGrid &);

}

#endif //PHYDB_INCLUDE_GCELLGRID_H_
