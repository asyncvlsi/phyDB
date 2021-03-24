#ifndef ADJACENTCUTSPACING_H
#define ADJACENTCUTSPACING_H

namespace phydb {

class AdjacentCutSpacing {
  private:
    double spacing_;
    int adjacent_cuts_;
    int cut_within_;

  public:
    AdjacentCutSpacing(): spacing_(0), adjacent_cuts_(0), cut_within_(0) {}
    AdjacentCutSpacing(double spacing, int adjacent_cuts, int cut_within):
        spacing_(spacing), adjacent_cuts_(adjacent_cuts), cut_within_(cut_within) {}

    void Report();
};

}

#endif


