#ifndef MACRO_H
#define MACRO_h

#include "DataType.h"
#include "pin.h"
#include "obs.h"

namespace phydb {

class Macro;
struct MacroWell;

class Macro {
  private:
    string name_;

    Point2D<float> origin_;
    Point2D<float> size_;

    vector<Pin> pins_;
    OBS obs_;

    map<string, int> pin_2_id_;
    MacroWell *well_ptr_ = nullptr;

    // ACT pointer
    // ACTMacro *act_ptr_ = nullptr;

  public:
    Macro() : name_("") {}
    explicit Macro(string &name) : name_(name) {
        size_.x = 0;
        size_.y = 0;
        origin_.x = 0;
        origin_.y = 0;
    }
    Macro(string name, Point2D<float> origin, Point2D<float> size, vector<Pin> pins, OBS obs) :
        name_(name),
        origin_(origin),
        size_(size),
        pins_(pins),
        obs_(obs) {}

    void SetName(string &name);
    void SetOrigin(Point2D<float> _origin);
    void SetOrigin(float x, float y);
    void SetSize(Point2D<float> size);
    void SetSize(float width, float height);

    // APIs for adding PINs to this MACRO
    bool IsPinExist(std::string pin_name);
    Pin *AddPin(std::string &pin_name, SignalDirection direction, SignalUse use);

    // APIs for adding OBS to this MACRO
    //void SetObs(OBS &obs); // TODO: change this API to return a pointer
    //void AddObsLayerRect(LayerRect &layer_rect);
    OBS *GetObs();


    void SetWellPtr(MacroWell *well_ptr);

    string GetName() const;
    Point2D<float> GetOrigin() const;
    Point2D<float> &GetOriginRef();
    float GetOriginX() const;
    float GetOriginY() const;
    Point2D<float> GetSize() const;
    float GetWidth() const;
    float GetHeight() const;
    vector<Pin> GetPinVec() const;
    vector<Pin> &GetPinVecRef() const;
    bool GetPin(string pinName, Pin &pin) const;
    bool GetObs(OBS &) const;
    MacroWell *GetWellPtr();

    friend ostream &operator<<(ostream &, const Macro &);
};

ostream &operator<<(ostream &, const Macro &);

struct MacroWell {
  private:
    Macro *macro_ptr_; // pointer to BlockType
    bool is_n_set_ = false; // whether N-well shape_ is Set or not
    bool is_p_set_ = false; // whether P-well shape_ is Set or not
    Rect2D<double> n_rect_; // N-well rect_
    Rect2D<double> p_rect_; // P-well rect_
    double p_n_edge_ = 0; // cached N/P-well boundary
  public:
    explicit MacroWell(Macro *type_ptr) : macro_ptr_(type_ptr) {}

    Macro *GetMacroPtr() const;
    void SetNwellRect(double lx, double ly, double ux, double uy);
    Rect2D<double> *GetNwellRectPtr();
    void SetPwellRect(double lx, double ly, double ux, double uy);
    Rect2D<double> *GetPwellRectPtr();
    double GetPnBoundary() const;
    double GetNheight() const;
    double GetPheight() const;
    void SetWellRect(bool is_n, double lx, double ly, double ux, double uy);
    void SetWellShape(bool is_n, Rect2D<double> &rect);
    bool IsNPWellAbutted() const;
    void Report() const;
};

}

#endif



