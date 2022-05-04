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

#ifndef PHYDB_CORNERSPACING_H_
#define PHYDB_CORNERSPACING_H_

#include "phydb/common/logging.h"

namespace phydb {

class CornerSpacing {
 private:
  double eol_width_;
  std::vector<double> width_;
  std::vector<double> spacing_;

 public:
  CornerSpacing() : eol_width_(0) {}

  //constructor for metal layer
  CornerSpacing(
      double eolWidth,
      std::vector<double> width,
      std::vector<double> spacing
  ) : eol_width_(eolWidth),
      width_(width),
      spacing_(spacing) {}

  void SetEOLWidth(double eol_width) { eol_width_ = eol_width; }
  void AddWidth(double width) { width_.push_back(width); }
  void AddSpacing(double spacing) { spacing_.push_back(spacing); }


  double GetEOLWidth() { return eol_width_;}
  std::vector<double> GetWidth() {return width_;}
  std::vector<double> GetSpacing() { return spacing_;}

  void Reset();
};

std::ostream &operator<<(std::ostream &, const CornerSpacing &);

}

#endif //PHYDB_CORNERSPACING_H_
