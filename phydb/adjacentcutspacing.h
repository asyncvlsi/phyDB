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

#ifndef PHYDB_ADJACENTCUTSPACING_H_
#define PHYDB_ADJACENTCUTSPACING_H_

namespace phydb {

class AdjacentCutSpacing {
 private:
  double spacing_;
  int adjacent_cuts_;
  int cut_within_;

 public:
  AdjacentCutSpacing() : spacing_(0), adjacent_cuts_(0), cut_within_(0) {}
  AdjacentCutSpacing(double spacing, int adjacent_cuts, int cut_within) :
      spacing_(spacing),
      adjacent_cuts_(adjacent_cuts),
      cut_within_(cut_within) {}

  void Report();
};

}

#endif //PHYDB_ADJACENTCUTSPACING_H_


