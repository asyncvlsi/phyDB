#include "macro.h"

namespace phydb {

const string &Macro::GetName() {
    return name_;
}

void Macro::SetName(string &name) {
    name_ = name;
}

void Macro::SetOrigin(Point2D<float> _origin) {
    origin_ = _origin;
}

void Macro::SetOrigin(float x, float y) {
    origin_.x = x;
    origin_.y = y;
}

void Macro::SetSize(Point2D<float> size) {
    size_ = size;
}

void Macro::SetSize(float width, float height) {
    size_.x = width;
    size_.y = height;
}

bool Macro::IsPinExist(std::string pin_name) {
    return pin_2_id_.find(pin_name) != pin_2_id_.end();
}

Pin *Macro::AddPin(std::string &pin_name, SignalDirection direction, SignalUse use) {
    PhyDbExpects(!IsPinExist(pin_name), "Pin " + pin_name + " exists in Macro " + GetName() + ", cannot add it again");
    int id = pins_.size();
    pin_2_id_[pin_name] = id;
    pins_.emplace_back(pin_name, direction, use);
    return &(pins_.back());
}

OBS *Macro::GetObs() {
    return &obs_;
}

void Macro::SetWellPtr(MacroWell *well_ptr) {
    well_ptr_ = well_ptr;
}

Point2D<float> Macro::GetOrigin() const {
    return origin_;
}

Point2D<float> &Macro::GetOriginRef() {
    return origin_;
}

float Macro::GetWidth() const {
    return size_.x;
}

float Macro::GetHeight() const {
    return size_.y;
}

vector<Pin> &Macro::GetPinsRef() {
    return pins_;
}

MacroWell *Macro::GetWellPtr() {
    return well_ptr_;
}

ostream &operator<<(ostream &os, const Macro &macro) {
    os << macro.name_ << endl;
    os << macro.origin_ << endl;
    os << macro.size_ << endl;

    for (auto &p : macro.pins_) {
        os << p << endl;
    }
    os << macro.obs_ << endl;
    return os;

}

// get the pointer to the BlockType this well belongs to
Macro *MacroWell::GetMacroPtr() const {
    return macro_ptr_;
}

// Set the rect_ of N-well
void MacroWell::SetNwellRect(double lx, double ly, double ux, double uy) {
    is_n_set_ = true;
    n_rect_.Set(lx, ly, ux, uy);
    if (is_p_set_) {
        //PhyDbExpects(n_rect_.LLY() == p_rect_.URY(), "N/P-well not abutted");
    } else {
        p_n_edge_ = n_rect_.LLY();
    }
}

// get the pointer to the rect_ of N-well
Rect2D<double> *MacroWell::GetNwellRectPtr() {
    return &(n_rect_);
}

// Set the rect_ of P-well
void MacroWell::SetPwellRect(double lx, double ly, double ux, double uy) {
    is_p_set_ = true;
    p_rect_.Set(lx, ly, ux, uy);
    if (is_n_set_) {
        //PhyDbExpects(n_rect_.LLY() == p_rect_.URY(), "N/P-well not abutted");
    } else {
        p_n_edge_ = p_rect_.URY();
    }
}

// get the pointer to the rect_ of P-well
Rect2D<double> *MacroWell::GetPwellRectPtr() {
    return &(p_rect_);
}

// get the P/N well boundary
double MacroWell::GetPnBoundary() const {
    return p_n_edge_;
}

// get the height of N-well
double MacroWell::GetNheight() const {
    return macro_ptr_->GetHeight() - p_n_edge_;
}

// get the height of P-well
double MacroWell::GetPheight() const {
    return p_n_edge_;
}

// Set the rect_ of N or P well
void MacroWell::SetWellRect(bool is_n, double lx, double ly, double ux, double uy) {
    if (is_n) {
        SetNwellRect(lx, ly, ux, uy);
    } else {
        SetPwellRect(lx, ly, ux, uy);
    }
}

// Set the rect_ of N or P well
void MacroWell::SetWellShape(bool is_n, Rect2D<double> &rect) {
    SetWellRect(is_n, rect.LLX(), rect.LLY(), rect.URX(), rect.URY());
}

// check if N-well is abutted with P-well, if both exist
bool MacroWell::IsNPWellAbutted() const {
    if (is_p_set_ && is_n_set_) {
        return p_rect_.URY() == n_rect_.LLY();
    }
    return true;
}

// report the information of N/P-well for debugging purposes
void MacroWell::Report() const {
    std::cout << "Well of BlockType: " << macro_ptr_->GetName() << "\n"
              << "    Pwell: " << p_rect_.LLX() << "  " << p_rect_.LLY() << "  "
              << p_rect_.URX() << "  " << p_rect_.URY() << "\n"
              << "    Nwell: " << n_rect_.LLX() << "  " << n_rect_.LLY() << "  "
              << n_rect_.URX() << "  " << n_rect_.URY() << "\n";

}

}