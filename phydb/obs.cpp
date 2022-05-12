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
#include "obs.h"

namespace phydb {

LayerRect *OBS::AddLayerRect(std::string &layer_name) {
  layer_rects_.emplace_back(layer_name);
  return &(layer_rects_.back());
}

std::ostream &operator<<(std::ostream &os, const OBS &obs) {
  if (!obs.layer_rects_.empty()) {
    os << "OBS\n";
  }
  for (auto &l : obs.layer_rects_) {
    os << l << "\n";
  }
  return os;
}

std::vector<LayerRect> OBS::GetLayerRects() const {
  return layer_rects_;
}

std::vector<LayerRect> &OBS::GetLayerRectsRef() {
  return layer_rects_;
}

}


