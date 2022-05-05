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

#ifndef PHYDB_COMPONENT_H_
#define PHYDB_COMPONENT_H_

#include "datatype.h"
#include "enumtypes.h"
#include "phydb/common/logging.h"
#include "macro.h"

namespace phydb {

class Component {
 public:
  Component() = default;
  Component(
      int id,
      std::string const &comp_name,
      Macro *macro_ptr,
      CompSource source,
      PlaceStatus place_status,
      Point2D<int> location,
      CompOrient orient,
      int weight = 0
  ) : id_(id),
      name_(comp_name),
      macro_ptr_(macro_ptr),
      source_(source),
      place_status_(place_status),
      location_(location),
      orient_(orient),
      weight_(weight) {}
  Component(
      int id,
      std::string const &comp_name,
      Macro *macro_ptr,
      CompSource source,
      PlaceStatus place_status,
      int llx,
      int lly,
      CompOrient orient,
      int weight = 0
  ) : id_(id),
      name_(comp_name),
      macro_ptr_(macro_ptr),
      source_(source),
      place_status_(place_status),
      location_(llx, lly),
      orient_(orient),
      weight_(weight) {}

  void SetPlacementStatus(PlaceStatus status);
  void SetLocation(int lx, int ly);
  void SetOrientation(CompOrient orient);
  void SetSource(CompSource source);

  int GetId();
  std::string const &GetName();
  Macro *GetMacro();
  CompSource GetSource() const;
  std::string GetSourceStr() const;
  PlaceStatus GetPlacementStatus();
  std::string GetPlacementStatusStr() const;
  Point2D<int> GetLocation();
  CompOrient GetOrientation();
  std::string GetOrientationStr() const;
  int GetWeight();

 private:
  int id_{};
  std::string name_;
  Macro *macro_ptr_{};
  CompSource source_;
  PlaceStatus place_status_;
  Point2D<int> location_;
  CompOrient orient_;
  int weight_{};
};

std::ostream &operator<<(std::ostream &, Component &);

}

#endif //PHYDB_COMPONENT_H_
