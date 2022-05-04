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

#ifndef PHYDB_OBS_H_
#define PHYDB_OBS_H_

#include "datatype.h"

namespace phydb {

class OBS {
 private:
  std::vector<LayerRect> layer_rects_;

 public:
  OBS() {}
  OBS(std::vector<LayerRect> layerRects) : layer_rects_(layerRects) {}

  void SetLayerRects(std::vector<LayerRect>);

  std::vector<LayerRect> GetLayerRects() const;
  std::vector<LayerRect> &GetLayerRectsRef();

  // API to add LayerRect
  LayerRect *AddLayerRect(std::string &layer_name);

  friend std::ostream &operator<<(std::ostream &, const OBS &);
};

std::ostream &operator<<(std::ostream &, const OBS &);

}

#endif //PHYDB_OBS_H_

