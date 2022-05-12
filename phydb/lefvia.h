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
#ifndef PHYDB_LEFVIA_H_
#define PHYDB_LEFVIA_H_

#include <string>
#include <vector>

#include "datatype.h"

namespace phydb {

class LefVia {
 public:
  LefVia() : is_default_(false) {}
  explicit LefVia(std::string const &name) : name_(name) {}

  std::string GetName() const;
  void SetDefault();
  void UnsetDefault();
  void SetLayerRect(
      const std::string &layer_name0,
      const std::vector<Rect2D<double>> &rects0,
      const std::string &layer_name1,
      const std::vector<Rect2D<double>> &rects1,
      const std::string &layer_name2,
      const std::vector<Rect2D<double>> &rects2
  );

  std::vector<LayerRect> &GetLayerRectsRef();

  void Report();
 private:
  std::string name_;
  bool is_default_;
  std::vector<LayerRect> layer_rects_;
};

}

#endif //PHYDB_LEFVIA_H_
