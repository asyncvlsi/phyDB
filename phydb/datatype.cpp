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
#include "datatype.h"

#include <cfloat>

namespace phydb {

std::ostream &operator<<(std::ostream &os, const LayerRect &lr) {
  os << lr.layer_name_ << "\n";
  for (auto rect : lr.rects_) {
    os << rect << "\n";
  }
  return os;
}

void LayerRect::AddRect(double llx, double lly, double urx, double ury) {
  rects_.emplace_back(llx, lly, urx, ury);
}

std::vector<Rect2D<double>> &LayerRect::GetRects() {
  return rects_;
}

Rect2D<double> LayerRect::GetBoundingBox() {
  PhyDBExpects(
      !rects_.empty(),
      "Compute bounding box for 0 rects?"
  );
  double min_x = DBL_MAX;
  double min_y = DBL_MAX;
  double max_x = -DBL_MAX;
  double max_y = -DBL_MAX;
  for (auto &rect : rects_) {
    min_x = std::min(min_x, rect.LLX());
    min_y = std::min(min_y, rect.LLY());
    max_x = std::max(max_x, rect.URX());
    max_y = std::max(max_y, rect.URY());
  }

  return {min_x, min_y, max_x, max_y};
}

void LayerRect::Reset() {
  layer_name_ = "";
  rects_.clear();
}

void LayerRect::Report() {
  std::cout << "Name: " << layer_name_ << "\n";
  for (auto &rect_2d : rects_) {
    std::cout << "  " << rect_2d.ll.Str() << " " << rect_2d.ur.Str() << "\n";
  }
}

}

