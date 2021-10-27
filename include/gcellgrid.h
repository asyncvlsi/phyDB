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

#ifndef PHYDB_INCLUDE_GCELLGRID_H_
#define PHYDB_INCLUDE_GCELLGRID_H_

#include "enumtypes.h"
#include "logging.h"

namespace phydb {

class GcellGrid {
 private:
  XYDirection direction_;
  int start_;
  int numBoundaries_;
  int step_;

 public:
  GcellGrid() : start_(0), numBoundaries_(0), step_(0) {}
  GcellGrid(XYDirection direction, int start, int numBoundaries, int step) :
      direction_(direction),
      start_(start),
      numBoundaries_(numBoundaries),
      step_(step) {}

  void SetDirection(XYDirection);
  void SetStart(int);
  void SetNBoundaries(int);
  void SetStep(int);

  XYDirection GetDirection() const;
  int GetStart() const;
  int GetNBoundaries() const;
  int GetStep() const;

  void Report() const;

  friend std::ostream &operator<<(std::ostream &, const GcellGrid &);

};

std::ostream &operator<<(std::ostream &, const GcellGrid &);

}

#endif //PHYDB_INCLUDE_GCELLGRID_H_
