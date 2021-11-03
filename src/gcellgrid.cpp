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

#include "gcellgrid.h"

namespace phydb {

void GcellGrid::SetDirection(XYDirection direction) {
  bool e = (direction == XYDirection::X) || (direction == XYDirection::Y);
  PhyDBExpects(e, "gcellgrid direction is not X or Y");
  direction_ = direction;
}

void GcellGrid::SetStart(int start) {
  start_ = start;
}

void GcellGrid::SetNBoundaries(int numBoundaries) {
  numBoundaries_ = numBoundaries;
}

void GcellGrid::SetStep(int step) {
  step_ = step;
}

XYDirection GcellGrid::GetDirection() const {
  return direction_;
}

int GcellGrid::GetStart() const {
  return start_;
}

int GcellGrid::GetNBoundaries() const {
  return numBoundaries_;
}

int GcellGrid::GetStep() const {
  return step_;
}

void GcellGrid::Report() const {
  std::cout << "GCELLGRID: "
            << " dir: " << XYDirectionStr(direction_)
            << " start: " << start_
            << " numBoundaries: " << numBoundaries_
            << " step: " << step_
            << std::endl;
}

}