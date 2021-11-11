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
#include "pin.h"

#include <cfloat>

namespace phydb {

void Pin::SetName(std::string &name) {
  name_ = name;
}

void Pin::SetUse(SignalUse &use) {
  use_ = use;
}

LayerRect *Pin::AddLayerRect(std::string &layer_name) {
  layer_rects_.emplace_back(layer_name);
  return &(layer_rects_.back());
}

const std::string &Pin::GetName() {
  return name_;
}

SignalDirection Pin::GetDirection() {
  return direction_;
}

bool Pin::IsDriverPin() const {
  return direction_ == phydb::SignalDirection::OUTPUT
      || direction_ == phydb::SignalDirection::OUTPUT_TRISTATE;
}

SignalUse Pin::GetUse() {
  return use_;
}

std::vector<LayerRect> &Pin::GetLayerRectRef() {
  return layer_rects_;
}

std::vector<LayerRect> Pin::GetLayerRectCpy() {
  return layer_rects_;
}

Rect2D<double> Pin::GetBoundingBox() {
  PhyDBExpects(!layer_rects_.empty(),
               "Compute bounding box for 0 layer_rects?");
  double min_x = DBL_MAX;
  double min_y = DBL_MAX;
  double max_x = -DBL_MAX;
  double max_y = -DBL_MAX;
  for (auto &layer_rect: layer_rects_) {
    auto rect = layer_rect.GetBoundingBox();
    min_x = std::min(min_x, rect.LLX());
    min_y = std::min(min_y, rect.LLY());
    max_x = std::max(max_x, rect.URX());
    max_y = std::max(max_y, rect.URY());
  }
  return {min_x, min_y, max_x, max_y};
}

std::ostream &operator<<(std::ostream &os, const Pin &p) {
  os << p.name_ << " "
     << SignalDirectionStr(p.direction_) << " "
     << SignalUseStr(p.use_) << " "
     << p.shape_ << std::endl;
  os << p.antenna_diff_area_layer_ << " " << p.antenna_diff_area_ << "\n";

  for (auto &l: p.layer_rects_)
    os << l << std::endl;
  return os;
}

}




