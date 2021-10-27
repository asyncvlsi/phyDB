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

#ifndef PHYDB_INCLUDE_VIARULEGENERATE_H
#define PHYDB_INCLUDE_VIARULEGENERATE_H

#include "datatype.h"

namespace phydb {

class ViaRuleGenerateLayer {
 public:
  std::string layer_name_;
  Rect2D<double> rect_;
  Size2D<double> spacing_;
  Size2D<double> enclosure_;

  ViaRuleGenerateLayer() {}
  ViaRuleGenerateLayer(std::string &layerName) : layer_name_(layerName) {}

  void SetLayerName(const std::string &layer_name);
  void SetRect(double lx, double ly, double ux, double uy);
  void SetSpacing(double x, double y);
  void SetEnclosure(double x, double y);

  std::string GetLayerName() const;
  Rect2D<double> GetRect() const;
  Size2D<double> GetSpacing() const;
  Size2D<double> GetEnclosure() const;
};

class ViaRuleGenerate {
 public:
  std::string name_;
  bool is_default_;
  ViaRuleGenerateLayer layers_[3];

  ViaRuleGenerate() : is_default_(false) {}
  ViaRuleGenerate(std::string &name) : name_(name), is_default_(false) {}

  void SetDefault();
  void UnsetDefault();
  void SetLayers(
      ViaRuleGenerateLayer &,
      ViaRuleGenerateLayer &,
      ViaRuleGenerateLayer &
  );
};

}

#endif //PHYDB_INCLUDE_VIARULEGENERATE_H
