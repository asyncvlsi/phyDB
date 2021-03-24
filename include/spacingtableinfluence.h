#ifndef SPACINGTABLEINFLUENCE_H
#define SPACINGTABLEINFLUENCE_H

namespace phydb {

class SpacingTableInfluence {
  private:
    double width_;
    double within_;
    double spacing_;

  public: 
    SpacingTableInfluence() {}
    SpacingTableInfluence(double width, double within, double spacing):
        width_(width), within_(within), spacing_(spacing) {}

    void SetWidth(double);
    void SetWithin(double);
    void SetSpacing(double);

    double GetWidth() const;
    double GetWithin() const;
    double GetSpacing() const;
    
    void Report() const;

};

}


#endif




