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

#include "component.h"

namespace phydb {

void Component::SetPlacementStatus(PlaceStatus status) {
  place_status_ = status;
}

void Component::SetOrientation(CompOrient orient) {
  orient_ = orient;
}

void Component::SetLocation(int lx, int ly) {
  location_.x = lx;
  location_.y = ly;
}

const std::string &Component::GetName() {
  return name_;
}

Macro *Component::GetMacro() {
  return macro_ptr_;
}

PlaceStatus Component::GetPlacementStatus() {
  return place_status_;
}

CompOrient Component::GetOrientation() {
  return orient_;
}

Point2D<int> Component::GetLocation() {
  return location_;
}

std::ostream &operator<<(std::ostream &os, const Component &c) {
  os << c.name_ << " " << c.macro_ptr_ << " "
     << c.source_ << " " << PlaceStatusStr(c.place_status_) << " "
     << CompOrientStr(c.orient_) << std::endl;
  os << "weight: " << c.weight_ << " location: " << c.location_ << std::endl;
  return os;
}

}

