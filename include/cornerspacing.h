#ifndef CORNERSPACING_H
#define CORNERSPACING_H

#include "header.h"

namespace phydb {

class CornerSpacing {
  private:
    float eol_width_;
    vector<float> width_;
    vector<float> spacing_;

  public:
    CornerSpacing() : eol_width_(0) {}

    //constructor for metal layer
    CornerSpacing(float eolWidth, vector<float> width, vector<float> spacing) :
        eol_width_(eolWidth), width_(width), spacing_(spacing) {}

    void Reset();
};

ostream &operator<<(ostream &, const CornerSpacing &);

}

#endif
