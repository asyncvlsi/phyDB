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

#ifndef PHYDB_INCLUDE_SPACINGTABLEINFLUENCE_H
#define PHYDB_INCLUDE_SPACINGTABLEINFLUENCE_H

namespace phydb {

class SpacingTableInfluence {
 private:
  double width_;
  double within_;
  double spacing_;

 public:
  SpacingTableInfluence() {}
  SpacingTableInfluence(double width, double within, double spacing) :
      width_(width), within_(within), spacing_(spacing) {}

  void SetWidth(double);
  void SetWithin(double);
  void SetSpacing(double);

  double GetWidth() const;
  double GetWithin() const;
  double GetSpacing() const;

  void Report() const;

};

}

#endif //PHYDB_INCLUDE_SPACINGTABLEINFLUENCE_H
