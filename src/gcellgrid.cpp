#include "gcellgrid.h"

namespace phydb {

void GcellGrid::SetDirection(XYDirection direction) {
    bool e = (direction == X) || (direction == Y);
    PhyDBExpects(e, "gcellgrid direction is not X or Y");
    direction_ = direction;
}

void GcellGrid::SetStart(int start) {
    start_ = start;
}

void GcellGrid::SetNBoundaries(int numBoundaries) {
    numBoundaries_ = numBoundaries;
}

void GcellGrid::SetStep(int step) {
    step_ = step;
}

XYDirection GcellGrid::GetDirection() const {
    return direction_;
}

int GcellGrid::GetStart() const {
    return start_;
}

int GcellGrid::GetNBoundaries() const {
    return numBoundaries_;
}

int GcellGrid::GetStep() const {
    return step_;
}

void GcellGrid::Report() const {
    std::cout << "GCELLGRID: "
              << " dir: " << direction_
              << " start: " << start_
              << " numBoundaries: " << numBoundaries_
              << " step: " << step_
              << std::endl;
}

}