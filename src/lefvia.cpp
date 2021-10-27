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

#include "lefvia.h"

namespace phydb {

void LefVia::SetDefault() {
  is_default_ = true;
}

void LefVia::UnsetDefault() {
  is_default_ = false;
}

void LefVia::SetLayerRect(
    const std::string &layer_name0,
    const std::vector<Rect2D<double>> &rects0,
    const std::string &layer_name1,
    const std::vector<Rect2D<double>> &rects1,
    const std::string &layer_name2,
    const std::vector<Rect2D<double>> &rects2
) {
  layer_rects_.resize(3);
  layer_rects_[0] = LayerRect(layer_name0, rects0);
  layer_rects_[1] = LayerRect(layer_name1, rects1);
  layer_rects_[2] = LayerRect(layer_name2, rects2);
}

std::string LefVia::GetName() const {
  return name_;
}

std::vector<LayerRect> &LefVia::GetLayerRectsRef() {
  return layer_rects_;
}

void LefVia::Report() {
  std::cout << "LefVia name: " << name_ << "\n";
  for (auto &layer_rect: layer_rects_) {
    layer_rect.Report();
  }
}

}
