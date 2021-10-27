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

#ifndef PHYDB_INCLUDE_PIN_H_
#define PHYDB_INCLUDE_PIN_H_

#include "datatype.h"
#include "enumtypes.h"

namespace phydb {

class Pin {
 private:
  std::string name_;
  SignalDirection direction_;
  SignalUse use_;
  std::string shape_;
  std::string antenna_diff_area_layer_;
  double antenna_diff_area_;

  std::vector<LayerRect> layer_rects_;

 public:
  Pin() : name_(""), direction_(INPUT), use_(SIGNAL), shape_(""),
          antenna_diff_area_layer_(""), antenna_diff_area_(0) {}
  Pin(std::string &name, SignalDirection direction, SignalUse use) :
      name_(name), direction_(direction), use_(use) {}
  Pin(std::string name,
      SignalDirection direction,
      SignalUse use,
      std::string shape,
      std::string antennaDiffAreaLayer,
      double antennaDiffArea,
      std::vector<LayerRect> layerRects) :
      name_(name),
      direction_(direction),
      use_(use),
      shape_(shape),
      antenna_diff_area_layer_(antennaDiffAreaLayer),
      antenna_diff_area_(antennaDiffArea),
      layer_rects_(layerRects) {}

  void SetName(std::string &name);
  void SetUse(SignalUse &use);
  void SetLayerRect(std::vector<LayerRect>);

  LayerRect *AddLayerRect(std::string &layer_name);

  const std::string &GetName();
  SignalDirection GetDirection();
  bool IsDriverPin() const;
  SignalUse GetUse();
  std::vector<LayerRect> &GetLayerRectRef();
  std::vector<LayerRect> GetLayerRectCpy();

  friend std::ostream &operator<<(std::ostream &, const Pin &);
};

std::ostream &operator<<(std::ostream &, const Pin &);

}

#endif //PHYDB_INCLUDE_PIN_H_
