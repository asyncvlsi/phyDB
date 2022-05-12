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
#include "viarulegenerate.h"

namespace phydb {

void ViaRuleGenerateLayer::SetLayerName(const std::string &layer_name) {
  layer_name_ = layer_name;
}

void ViaRuleGenerateLayer::SetRect(double lx, double ly, double ux, double uy) {
  rect_.Set(lx, ly, ux, uy);
}

void ViaRuleGenerateLayer::SetSpacing(double x, double y) {
  spacing_.Set(x, y);
}

void ViaRuleGenerateLayer::SetEnclosure(double x, double y) {
  enclosure_.Set(x, y);
}

std::string ViaRuleGenerateLayer::GetLayerName() const {
  return layer_name_;
}

Rect2D<double> ViaRuleGenerateLayer::GetRect() const {
  return rect_;
}

Size2D<double> ViaRuleGenerateLayer::GetSpacing() const {
  return spacing_;
}

Size2D<double> ViaRuleGenerateLayer::GetEnclosure() const {
  return enclosure_;
}

void ViaRuleGenerate::SetDefault() {
  is_default_ = true;
}

void ViaRuleGenerate::UnsetDefault() {
  is_default_ = false;
}

void ViaRuleGenerate::SetLayers(
    ViaRuleGenerateLayer &layer0,
    ViaRuleGenerateLayer &layer1,
    ViaRuleGenerateLayer &layer2
) {
  layers_[0] = layer0;
  layers_[1] = layer1;
  layers_[2] = layer2;
}

}



