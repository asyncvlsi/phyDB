#ifndef PHYDB_INCLUDE_CORNERSPACING_H_
#define PHYDB_INCLUDE_CORNERSPACING_H_

#include "logging.h"

namespace phydb {

class CornerSpacing {
  private:
    double eol_width_;
    vector<double> width_;
    vector<double> spacing_;

  public:
    CornerSpacing() : eol_width_(0) {}

    //constructor for metal layer
    CornerSpacing(double eolWidth, vector<double> width, vector<double> spacing) :
        eol_width_(eolWidth), width_(width), spacing_(spacing) {}

    void Reset();
};

ostream &operator<<(ostream &, const CornerSpacing &);

}

#endif //PHYDB_INCLUDE_CORNERSPACING_H_
