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

#include "iopin.h"

namespace phydb {

void IOPin::SetNetId(int net_id) {
  net_id_ = net_id;
}

void IOPin::SetShape(std::string const &layer_name, int lx, int ly, int ux, int uy) {
  layer_name_ = layer_name;
  rect_.Set(lx, ly, ux, uy);
}

void IOPin::SetPlacement(
    PlaceStatus place_status,
    int x,
    int y,
    CompOrient orient
) {
  place_status_ = place_status;
  location_.x = x;
  location_.y = y;
  orient_ = orient;
}

void IOPin::SetPlacementStatus(PlaceStatus place_status) {
  place_status_ = place_status;
}

const std::string &IOPin::GetName() {
  return name_;
}

int IOPin::GetNetId() {
  return net_id_;
}

SignalDirection IOPin::GetDirection() {
  return direction_;
}

SignalUse IOPin::GetUse() {
  return use_;
}

const std::string &IOPin::GetLayerName() {
  return layer_name_;
}

Rect2D<int> IOPin::GetRect() {
  return rect_;
}

Point2D<int> IOPin::GetLocation() {
  return location_;
}

CompOrient IOPin::GetOrientation() {
  return orient_;
}

PlaceStatus IOPin::GetPlacementStatus() {
  return place_status_;
}

void IOPin::Report() {
  std::cout << "IOPIN name: " << name_ << "  Net: " << net_id_ << " "
            << " DIRECTION: " << SignalDirectionStr(direction_) << " "
            << " USE: " << SignalUseStr(use_) << "\n";
  std::cout << "LAYER: " << layer_name_ << " " << rect_.Str() << "\n";
  std::cout << PlaceStatusStr(place_status_) << " "
            << location_.Str() << " "
            << CompOrientStr(orient_) << "\n";
}

}
