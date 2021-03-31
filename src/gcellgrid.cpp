#include "gcellgrid.h"

namespace phydb {

void GcellGrid::SetDirection(string& direction) {
    bool e = (direction == "X") || (direction == "Y");
    PhyDbExpects(e, "gcellgrid direction is not X or Y");
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

string GcellGrid::GetDirection() const {
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
    cout << "GCELLGRID: " << endl;
    cout << " dir: " << direction_;
    cout << " start: " << start_;
    cout << " numBoundaries: " << numBoundaries_;
    cout << " step: " << step_;
    cout << endl;
}

}