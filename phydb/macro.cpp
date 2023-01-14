/*******************************************************************************
 *
 * Copyright (c) 2021 Jiayuan He, Yihang Yang
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 ******************************************************************************/
#include "macro.h"

namespace phydb {

const std::string &Macro::GetName() {
  return name_;
}

void Macro::SetName(std::string const &name) {
  name_ = name;
}

void Macro::SetClass(MacroClass macro_class) {
  class_ = macro_class;
}

void Macro::SetOrigin(Point2D<double> _origin) {
  origin_ = _origin;
}

void Macro::SetOrigin(double x, double y) {
  origin_.x = x;
  origin_.y = y;
}

void Macro::SetSize(Point2D<double> size) {
  size_ = size;
}

void Macro::SetSize(double width, double height) {
  size_.x = width;
  size_.y = height;
}

void Macro::SetSymmetry(bool x, bool y, bool r90) {
  symmetry_.Set(x, y, r90);
}

void Macro::SetSite(std::string const &site_name) {
  site_name_ = site_name;
}

bool Macro::IsPinExisting(std::string const &pin_name) {
  return pin_2_id_.find(pin_name) != pin_2_id_.end();
}

Pin *Macro::AddPin(
    std::string const &pin_name,
    SignalDirection direction,
    SignalUse use
) {
  PhyDBExpects(
      !IsPinExisting(pin_name),
      "Pin " << pin_name << " exists in Macro " << name_
             << ", cannot add it again"
  );
  int id = (int) pins_.size();
  pin_2_id_[pin_name] = id;
  pins_.emplace_back(pin_name, direction, use);
  return &(pins_.back());
}

int Macro::GetPinId(std::string const &pin_name) {
  if (pin_2_id_.find(pin_name) != pin_2_id_.end()) {
    return pin_2_id_[pin_name];
  }
  return -1;
}

OBS *Macro::GetObs() {
  return &obs_;
}

MacroClass Macro::GetClass() const {
  return class_;
}

Point2D<double> Macro::GetOrigin() const {
  return origin_;
}

double Macro::GetOriginX() const {
  return origin_.x;
}

double Macro::GetOriginY() const {
  return origin_.y;
}

Point2D<double> &Macro::GetOriginRef() {
  return origin_;
}

double Macro::GetWidth() const {
  return size_.x;
}

double Macro::GetHeight() const {
  return size_.y;
}

Symmetry Macro::GetSymmetry() const {
  return symmetry_;
}

std::string Macro::GetSite() const {
  return site_name_;
}

Point2D<double> Macro::GetSize() const {
  return size_;
}

std::vector<Pin> Macro::GetPinVec() const {
  return pins_;
}

std::vector<Pin> &Macro::GetPinsRef() {
  return pins_;
}

void Macro::InitWellPtr(Macro *macro_ptr) {
  well_ptr_ = std::make_unique<MacroWell>(macro_ptr);
}

std::unique_ptr<MacroWell> &Macro::WellPtrRef() {
  return well_ptr_;
}

void Macro::ExportToFile(std::ofstream &ost) {
  ost << "MACRO " << name_ << "\n"
      << "    CLASS " << MacroClassStr(class_) << " ;\n"
      << "    FOREIGN " << name_ << " 0.000000 0.000000 ;\n"
      << "    ORIGIN " << origin_.x << " " << origin_.y << " ;\n"
      << "    SIZE " << size_.x << " BY " << size_.y << " ;\n"
      << "    SYMMETRY " << symmetry_.Str() << " ;\n"
      << "    SITE " <<site_name_ << " ;\n";

  for (auto &pin: pins_) {
    pin.ExportToFile(ost);
  }

  ost << "END " << name_ << "\n";
  ost << "\n";
}

std::ostream &operator<<(std::ostream &os, const Macro &macro) {
  os << macro.name_ << std::endl;
  os << macro.origin_ << std::endl;
  os << macro.size_ << std::endl;

  for (auto &p : macro.pins_) {
    os << p << std::endl;
  }
  os << macro.obs_ << std::endl;
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
    //PhyDBExpects(n_rect_.LLY() == p_rect_.URY(), "N/P-well not abutted");
  } else {
    p_n_edge_ = n_rect_.LLY();
  }
}

// get the pointer to the rect_ of N-well
Rect2D<double> *MacroWell::GetNwellRectPtr() {
  if (is_n_set_) return &(n_rect_);
  return nullptr;
}

// Set the rect_ of P-well
void MacroWell::SetPwellRect(double lx, double ly, double ux, double uy) {
  is_p_set_ = true;
  p_rect_.Set(lx, ly, ux, uy);
  if (is_n_set_) {
    //PhyDBExpects(n_rect_.LLY() == p_rect_.URY(), "N/P-well not abutted");
  } else {
    p_n_edge_ = p_rect_.URY();
  }
}

// get the pointer to the rect_ of P-well
Rect2D<double> *MacroWell::GetPwellRectPtr() {
  if (is_p_set_) return &(p_rect_);
  return nullptr;
}

// Set the rect_ of N or P well
void MacroWell::SetWellRect(
    bool is_n,
    double lx,
    double ly,
    double ux,
    double uy
) {
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