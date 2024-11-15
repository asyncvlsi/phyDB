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
#ifndef PHYDB_MACRO_H_
#define PHYDB_MACRO_H_

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

#include "datatype.h"
#include "enumtypes.h"
#include "obs.h"
#include "pin.h"
#include "site.h"

namespace phydb {

class Macro;
struct MacroWell;

class Macro {
 public:
  Macro() : name_("") {}
  explicit Macro(std::string const &name) : name_(name) {
    size_.x = 0;
    size_.y = 0;
    origin_.x = 0;
    origin_.y = 0;
  }
  Macro(std::string const &name, Point2D<double> origin, Point2D<double> size,
        std::vector<Pin> pins, OBS obs)
      : name_(name), origin_(origin), size_(size), pins_(pins), obs_(obs) {}

  void SetName(std::string const &name);
  void SetClass(MacroClass macro_class);
  void SetOrigin(Point2D<double> _origin);
  void SetOrigin(double x, double y);
  void SetSize(Point2D<double> size);
  void SetSize(double width, double height);
  void SetSymmetry(bool x, bool y, bool r90);
  void SetSite(std::string const &site_name);

  // APIs for adding PINs to this MACRO
  bool IsPinExisting(std::string const &pin_name);
  Pin *AddPin(std::string const &pin_name, SignalDirection direction,
              SignalUse use);
  int GetPinId(std::string const &pin_name);

  // APIs for adding OBS to this MACRO
  // void SetObs(OBS &obs); // TODO: change this API to return a pointer
  // void AddObsLayerRect(LayerRect &layer_rect);

  const std::string &GetName();
  MacroClass GetClass() const;
  Point2D<double> GetOrigin() const;
  Point2D<double> &GetOriginRef();
  double GetOriginX() const;
  double GetOriginY() const;
  Point2D<double> GetSize() const;
  double GetWidth() const;
  double GetHeight() const;
  Symmetry GetSymmetry() const;
  std::string GetSite() const;
  std::vector<Pin> GetPinVec() const;
  std::vector<Pin> &GetPinsRef();
  bool GetPin(std::string const pinName,
              Pin &pin) const;  // TODO: what is this?
  // bool GetObs(OBS &) const;
  OBS *GetObs();
  void InitWellPtr(Macro *macro_ptr);
  std::unique_ptr<MacroWell> &WellPtrRef();
  double GetWellPNEdge() const;

  /****Helper functions****/
  void ExportToFile(std::ofstream &ost);

  friend std::ostream &operator<<(std::ostream &, const Macro &);

 private:
  std::string name_;
  MacroClass class_ = MacroClass::CORE;
  Point2D<double> origin_;
  Point2D<double> size_;
  Symmetry symmetry_;
  std::string site_name_;  // TODO optimize this
  std::vector<Pin> pins_;
  OBS obs_;

  std::unordered_map<std::string, int> pin_2_id_;
  std::unique_ptr<MacroWell> well_ptr_ = nullptr;
};

std::ostream &operator<<(std::ostream &, const Macro &);

struct MacroWell {
 public:
  explicit MacroWell(Macro *macro_ptr) : macro_ptr_(macro_ptr) {}

  Macro *GetMacroPtr() const;
  void SetNwellRect(double lx, double ly, double ux, double uy);
  Rect2D<double> *GetNwellRectPtr();
  void SetPwellRect(double lx, double ly, double ux, double uy);
  Rect2D<double> *GetPwellRectPtr();
  void SetWellRect(bool is_n, double lx, double ly, double ux, double uy);
  void SetWellShape(bool is_n, Rect2D<double> &rect);
  bool IsNPWellAbutted() const;
  void Report() const;
  double GetPNEdge() const;

 private:
  Macro *macro_ptr_;       // pointer to BlockType
  bool is_n_set_ = false;  // whether N-well shape_ is Set or not
  bool is_p_set_ = false;  // whether P-well shape_ is Set or not
  Rect2D<double> n_rect_;  // N-well rect_
  Rect2D<double> p_rect_;  // P-well rect_
  double p_n_edge_ = 0;    // cached N/P-well boundary
};

}  // namespace phydb

#endif  // PHYDB_MACRO_H_
