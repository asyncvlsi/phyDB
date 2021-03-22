#ifndef SPACINGTABLEINFLUENCE_H
#define SPACINGTABLEINFLUENCE_H

namespace phydb {

class SpacingTableInfluence {
  private:
    float width_;
    float within_;
    float spacing_;

  public: 
    SpacingTableInfluence() {}
    SpacingTableInfluence(float width, float within, float spacing): 
        width_(width), within_(within), spacing_(spacing) {}

    void SetWidth(float);
    void SetWithin(float);
    void SetSpacing(float);

    float GetWidth() const;
    float GetWithin() const;
    float GetSpacing() const;
    
    void Report() const;

};

}


#endif




