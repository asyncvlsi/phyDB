#ifndef MACRO_H
#define MACRO_H

#include "DataType.h"
#include "pin.h"
#include "obs.h"
#include <unordered_map>

namespace phydb {

class Macro;
struct MacroWell;

class Macro {
  private:
    string name_;

    Point2D<double> origin_;
    Point2D<double> size_;

    vector<Pin> pins_;
    OBS obs_;

    unordered_map<string, int> pin_2_id_;
    MacroWell *well_ptr_ = nullptr;

  public:
    Macro() : name_("") {}
    Macro(string &name) : name_(name){
        size_.x = 0;
        size_.y = 0;
        origin_.x = 0;
        origin_.y = 0;
    }
    Macro(string name, Point2D<double> origin, Point2D<double> size, vector<Pin> pins, OBS obs) :
        name_(name),
        origin_(origin),
        size_(size),
        pins_(pins),
        obs_(obs) {}

    void SetName(string &name);
    void SetOrigin(Point2D<double> _origin);
    void SetOrigin(double x, double y);
    void SetSize(Point2D<double> size);
    void SetSize(double width, double height);

    // APIs for adding PINs to this MACRO
    bool IsPinExist(std::string pin_name);
    Pin *AddPin(std::string &pin_name, SignalDirection direction, SignalUse use);
    int GetPinId(std::string &pin_name);

    // APIs for adding OBS to this MACRO
    //void SetObs(OBS &obs); // TODO: change this API to return a pointer
    //void AddObsLayerRect(LayerRect &layer_rect);
    

    void SetWellPtr(MacroWell *well_ptr);

    const string &GetName();
    Point2D<double> GetOrigin() const;
    Point2D<double> &GetOriginRef();
    double GetOriginX() const;
    double GetOriginY() const;
    Point2D<double> GetSize() const;
    double GetWidth() const;
    double GetHeight() const;
    vector<Pin> GetPinVec() const;
    vector<Pin> &GetPinsRef();
    bool GetPin(string pinName, Pin &pin) const;
    //bool GetObs(OBS &) const;
    OBS *GetObs();
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
    void SetWellRect(bool is_n, double lx, double ly, double ux, double uy);
    void SetWellShape(bool is_n, Rect2D<double> &rect);
    bool IsNPWellAbutted() const;
    void Report() const;
};

}

#endif



