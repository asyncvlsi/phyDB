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

//
// Created by yihang on 9/3/21.
//

#include "specialmacrorectlayout.h"

#include <fstream>

namespace phydb {

RectSignalLayer::RectSignalLayer(
    std::string &signal_name,
    std::string &layer_name,
    int llx,
    int lly,
    int urx,
    int ury
) : signal_name_(signal_name), layer_name_(layer_name) {
  rect_.Set(llx, lly, urx, ury);
}

std::string &RectSignalLayer::SignalName() {
  return signal_name_;
}

std::string &RectSignalLayer::LayerName() {
  return layer_name_;
}

Rect2D<int> &RectSignalLayer::Rect() {
  return rect_;
}

std::ostream &operator<<(
    std::ostream &os,
    const RectSignalLayer &rect_signal_layer
) {
  os << "rect " << rect_signal_layer.signal_name_ << " "
     << rect_signal_layer.layer_name_ << " "
     << rect_signal_layer.rect_.LLX() << "\t"
     << rect_signal_layer.rect_.LLY() << "\t"
     << rect_signal_layer.rect_.URX() << "\t"
     << rect_signal_layer.rect_.URY();
  return os;
}

SpecialMacroRectLayout::SpecialMacroRectLayout(
    Macro *macro_ptr,
    int llx,
    int lly,
    int urx,
    int ury
) : macro_ptr_(macro_ptr) {
  bbox_.Set(llx, lly, urx, ury);
}

void SpecialMacroRectLayout::SetBoundingBox(
    int llx,
    int lly,
    int urx,
    int ury
) {
  bbox_.Set(llx, lly, urx, ury);
}

void SpecialMacroRectLayout::AddRectSignalLayer(
    std::string &signal_name,
    std::string &layer_name,
    int llx,
    int lly,
    int urx,
    int ury
) {
  rects_.emplace_back(signal_name, layer_name, llx, lly, urx, ury);
}

void SpecialMacroRectLayout::SaveToRectFile(std::string &file_name) const {
  std::ofstream ost(file_name.c_str());
  PhyDBExpects(ost.is_open(), "Cannot open output file: " + file_name);

  ost << "bbox "
      << bbox_.LLX() << " "
      << bbox_.LLY() << " "
      << bbox_.URX() << " "
      << bbox_.URY() << "\n";

  for (auto &rect: rects_) {
    ost << rect << "\n";
  }
}

}