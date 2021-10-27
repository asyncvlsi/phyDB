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

#include <iostream>

#include "spacingtableinfluence.h"

namespace phydb {

void SpacingTableInfluence::SetWidth(double width) {
  width_ = width;
}

void SpacingTableInfluence::SetWithin(double within) {
  within_ = within;
}

void SpacingTableInfluence::SetSpacing(double spacing) {
  spacing_ = spacing;
}

double SpacingTableInfluence::GetWidth() const {
  return width_;
}

double SpacingTableInfluence::GetWithin() const {
  return within_;
}

double SpacingTableInfluence::GetSpacing() const {
  return spacing_;
}

void SpacingTableInfluence::Report() const {
  std::cout << "width: " << width_;
  std::cout << "within: " << within_;
  std::cout << "spacing: " << spacing_;
  std::cout << std::endl;

}

}




