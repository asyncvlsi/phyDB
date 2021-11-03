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

#ifndef PHYDB_INCLUDE_COMPONENT_H_
#define PHYDB_INCLUDE_COMPONENT_H_

#include "datatype.h"
#include "enumtypes.h"
#include "logging.h"
#include "macro.h"

namespace phydb {

class Component {
 public:
  int id_;
  std::string name_;
  Macro *macro_ptr_;
  std::string source_;
  PlaceStatus place_status_;
  CompOrient orient_;

  int weight_;
  Point2D<int> location_;

  Component() {}
  Component(
      std::string const &name,
      Macro *macroName,
      std::string const &source,
      PlaceStatus place_status,
      CompOrient orient,
      int weight,
      Point2D<int> location
  ) : name_(name),
      macro_ptr_(macroName),
      source_(source),
      place_status_(place_status),
      orient_(orient),
      weight_(weight),
      location_(location) {}
  Component(
      std::string const &comp_name,
      Macro *macro_name,
      PlaceStatus place_status,
      int llx,
      int lly,
      CompOrient orient
  ) : name_(comp_name),
      macro_ptr_(macro_name),
      place_status_(place_status),
      orient_(orient) {
    location_.x = llx;
    location_.y = lly;
  }

  void SetPlacementStatus(PlaceStatus status);
  void SetOrientation(CompOrient orient);
  void SetLocation(int lx, int ly);

  const std::string &GetName();
  Macro *GetMacro();
  PlaceStatus GetPlacementStatus();
  CompOrient GetOrientation();
  Point2D<int> GetLocation();
  int GetWeight() { return weight_; }
};

std::ostream &operator<<(std::ostream &, const Component &);

}

#endif //PHYDB_INCLUDE_COMPONENT_H_
