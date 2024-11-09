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
#ifndef PHYDB_EOLSPACING_H_
#define PHYDB_EOLSPACING_H_

#include "phydb/common/logging.h"

namespace phydb {

class EolSpacing {
 public:
  EolSpacing()
      : spacing_(0),
        eol_width_(0),
        eol_within_(0),
        par_edge_(0),
        par_within_(0) {}

  // constructor for metal layer
  EolSpacing(double spacing, double eolWidth, double eolWithin, double parEdge,
             double parWithin)
      : spacing_(spacing),
        eol_width_(eolWidth),
        eol_within_(eolWithin),
        par_edge_(parEdge),
        par_within_(parWithin) {}

  double GetSpacing() { return spacing_; }
  double GetEOLWidth() { return eol_width_; }
  double GetEOLWithin() { return eol_within_; }
  double GetParEdge() { return par_edge_; }
  double GetParWithin() { return par_within_; }

  void Reset();

 private:
  // metal layer
  double spacing_;
  double eol_width_;
  double eol_within_;
  double par_edge_;
  double par_within_;
};

std::ostream &operator<<(std::ostream &, const EolSpacing &);

}  // namespace phydb

#endif  // PHYDB_EOLSPACING_H_
