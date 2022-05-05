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

void Component::SetLocation(int lx, int ly) {
  location_.x = lx;
  location_.y = ly;
}

void Component::SetOrientation(CompOrient orient) {
  orient_ = orient;
}

void Component::SetSource(CompSource source) {
  source_ = source;
}

int Component::GetId() {
  return id_;
}

std::string const &Component::GetName() {
  return name_;
}

Macro *Component::GetMacro() {
  return macro_ptr_;
}

CompSource Component::GetSource() const {
  return source_;
}

std::string Component::GetSourceStr() const {
  return CompSourceStr(source_);
}

PlaceStatus Component::GetPlacementStatus() {
  return place_status_;
}

std::string Component::GetPlacementStatusStr() const {
  return PlaceStatusStr(place_status_);
}

Point2D<int> Component::GetLocation() {
  return location_;
}

CompOrient Component::GetOrientation() {
  return orient_;
}

std::string Component::GetOrientationStr() const {
  return CompOrientStr(orient_);
}

int Component::GetWeight() {
  return weight_;
}

std::ostream &operator<<(std::ostream &os, Component &c) {
  os << c.GetId() << " "
     << c.GetName() << " "
     << c.GetMacro()->GetName() << " "
     << c.GetSourceStr() << " "
     << c.GetPlacementStatusStr() << " "
     << c.GetOrientationStr() << "\n"
     << "weight: " << c.GetWeight()
     << " location: " << c.GetLocation() << "\n";
  return os;
}

}

