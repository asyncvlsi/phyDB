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
    string _name;

    Point2D<float> _origin;
    Point2D<float> _size;

    vector<Pin> _pins;
    OBS _obs;

    MacroWell *well_ptr_ = nullptr;

    // ACT pointer
    // ACTMacro *act_ptr_ = nullptr;

  public:
    Macro() : _name("") {}
    explicit Macro(string &name) : _name(name) {
        _size.x = 0;
        _size.y = 0;
        _origin.x = 0;
        _origin.y = 0;
    }
    Macro(string name, Point2D<float> origin, Point2D<float> size, vector<Pin> pins, OBS obs) :
        _name(name),
        _origin(origin),
        _size(size),
        _pins(pins),
        _obs(obs) {}

    void setName(string &name);
    void setOrigin(Point2D<float> _origin);
    void setOrigin(float x, float y);
    void setSize(Point2D<float> size);
    void setSize(float width, float height);
    void setPins(vector<Pin>);
    void addPin(Pin &pin) { _pins.push_back(pin); }
    void setOBS(OBS obs) { _obs = obs; }
    void addObsLayerRect(LayerRect &layer_rect) { _obs._layerRects.push_back(layer_rect); }
    void set_well_ptr(MacroWell *well_ptr) { well_ptr_ = well_ptr; }

    string getName() const;
    Point2D<float> getOrigin() const;
    Point2D<float> &getOriginRef();
    float getOriginX() const;
    float getOriginY() const;
    Point2D<float> getSize() const;
    float getWidth() const;
    float getHeight() const;
    vector<Pin> getPinVec() const;
    vector<Pin> &getPinVecRef() const;
    bool getPin(string pinName, Pin &pin) const;
    bool getOBS(OBS &) const;
    MacroWell *well_ptr() { return well_ptr_; }

    friend ostream &operator<<(ostream &, const Macro &);
};

ostream &operator<<(ostream &, const Macro &);

struct MacroWell {
    Macro *macro_ptr_; // pointer to BlockType
    bool is_n_set_ = false; // whether N-well shape is set or not
    bool is_p_set_ = false; // whether P-well shape is set or not
    Rect2D<double> n_rect_; // N-well rect
    Rect2D<double> p_rect_; // P-well rect
    double p_n_edge_ = 0; // cached N/P-well boundary

    explicit MacroWell(Macro *type_ptr) : macro_ptr_(type_ptr) {}

    // get the pointer to the BlockType this well belongs to
    Macro *BlkTypePtr() const { return macro_ptr_; }

    // set the rect of N-well
    void setNWellRect(double lx, double ly, double ux, double uy) {
        is_n_set_ = true;
        n_rect_.set(lx, ly, ux, uy);
        if (is_p_set_) {
            //PhyDbExpects(n_rect_.LLY() == p_rect_.URY(), "N/P-well not abutted");
        } else {
            p_n_edge_ = n_rect_.LLY();
        }
    }

    // get the pointer to the rect of N-well
    Rect2D<double> *NWellRectPtr() { return &(n_rect_); }

    // set the rect of P-well
    void setPWellRect(double lx, double ly, double ux, double uy) {
        is_p_set_ = true;
        p_rect_.set(lx, ly, ux, uy);
        if (is_n_set_) {
            //PhyDbExpects(n_rect_.LLY() == p_rect_.URY(), "N/P-well not abutted");
        } else {
            p_n_edge_ = p_rect_.URY();
        }
    }

    // get the pointer to the rect of P-well
    Rect2D<double> *PWellRectPtr() { return &(p_rect_); }

    // get the P/N well boundary
    double PNBoundary() const { return p_n_edge_; }

    // get the height of N-well
    double NHeight() const { return macro_ptr_->getHeight() - p_n_edge_; }

    // get the height of P-well
    double PHeight() const { return p_n_edge_; }

    // set the rect of N or P well
    void setWellRect(bool is_n, double lx, double ly, double ux, double uy) {
        if (is_n) {
            setNWellRect(lx, ly, ux, uy);
        } else {
            setPWellRect(lx, ly, ux, uy);
        }
    }

    // set the rect of N or P well
    void SetWellShape(bool is_n, Rect2D<double> &rect) {
        setWellRect(is_n, rect.LLX(), rect.LLY(), rect.URX(), rect.URY());
    }

    // check if N-well is abutted with P-well, if both exist
    bool IsNPWellAbutted() const {
        if (is_p_set_ && is_n_set_) {
            return p_rect_.URY() == n_rect_.LLY();
        }
        return true;
    }

    // report the information of N/P-well for debugging purposes
    void Report() const {
        std::cout
            << "  Well of BlockType: " << macro_ptr_->getName() << "\n"
            << "    Nwell: " << n_rect_.LLX() << "  " << n_rect_.LLY() << "  " << n_rect_.URX() << "  " << n_rect_.URY()
            << "\n"
            << "    Pwell: " << p_rect_.LLX() << "  " << p_rect_.LLY() << "  " << p_rect_.URX() << "  " << p_rect_.URY()
            << "\n";

    }
};

}

#endif



