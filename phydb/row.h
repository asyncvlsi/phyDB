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

#ifndef PHYDB_ROW_H_
#define PHYDB_ROW_H_

#include "phydb/common/logging.h"

namespace phydb {

class Row {
 public:
  std::string name_;
  std::string site_name_;
  std::string site_orient_;
  int orig_x_;
  int orig_y_;
  int num_x_;
  int num_y_;
  int step_x_;
  int step_y_;

  Row() {}
  Row(
      std::string name,
      std::string siteName,
      std::string siteOrient,
      int origX,
      int origY,
      int numX,
      int numY,
      int stepX,
      int stepY
  ) :
      name_(name),
      site_name_(siteName),
      site_orient_(siteOrient),
      orig_x_(origX),
      orig_y_(origY),
      num_x_(numX),
      num_y_(numY),
      step_x_(stepX),
      step_y_(stepY) {}

};

std::ostream &operator<<(std::ostream &, const Row &);

}

#endif //PHYDB_ROW_H_

