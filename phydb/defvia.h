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
#ifndef PHYDB_DEFVIA_H_
#define PHYDB_DEFVIA_H_

#include "datatype.h"

namespace phydb {

class DefVia {
  // TODO: This can better be private, needs APIs
 public:
  std::string name_;
  // int idx_;
  std::string via_rule_name_;
  Size2D<int> cut_size_;
  std::string layers_[3];
  Size2D<int> cut_spacing_;
  Size2D<int> bot_enc_;
  Size2D<int> top_enc_;
  int num_cut_rows_;
  int num_cut_cols_;
  Point2D<int> origin_;
  Size2D<int> bot_offset_;
  Size2D<int> top_offset_;

  std::vector<Rect2DLayer<int>> rect2d_layers;
  std::string pattern_;

  DefVia() {}
  DefVia(std::string const &name) : name_(name) {}

  void Reset() {
    name_ = "";
    via_rule_name_ = "";
    cut_size_.Clear();
    layers_[0] = "";
    layers_[1] = "";
    layers_[2] = "";
    cut_spacing_.Clear();
    bot_enc_.Clear();
    top_enc_.Clear();
    num_cut_rows_ = 0;
    num_cut_cols_ = 0;
    origin_.Clear();
    bot_offset_.Clear();
    top_offset_.Clear();
  }

  void Report() {
    std::cout << "Via: " << name_ << " VIARule: " << via_rule_name_ << "\n"
              << "CUT: " << cut_size_.x << " " << cut_size_.y << "\n"
              << "Layer: " << layers_[0] << " " << layers_[1] << " "
              << layers_[2] << "\n"
              << "CUTSPACING: " << cut_spacing_.x << " " << cut_spacing_.y
              << "\n"
              << "bot_enc_: " << bot_enc_.x << " " << bot_enc_.y << "\n"
              << "top_enc_: " << top_enc_.x << " " << top_enc_.y << "\n"
              << " row: " << num_cut_rows_ << " col: " << num_cut_cols_ << "\n"
              << " origin_: " << origin_.x << " " << origin_.y << "\n"
              << " bot_offset_: " << bot_offset_.x << " " << bot_offset_.y
              << "\n"
              << " top_offset_: " << top_offset_.x << " " << top_offset_.y
              << "\n";
  }
};

}  // namespace phydb

#endif  // PHYDB_DEFVIA_H_
