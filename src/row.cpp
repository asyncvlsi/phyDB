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

#include "row.h"

namespace phydb {

std::ostream &operator<<(std::ostream &os, const Row &r) {
  os << r.name_ << " " << r.site_name_ << " " << r.site_orient_ << "\n";
  os << "orig: " << r.orig_x_ << ", " << r.orig_y_ << "\n";
  os << "num: " << r.num_x_ << ", " << r.num_y_ << "\n";
  os << "step: " << r.step_x_ << ", " << r.step_y_ << "\n";
  return os;
}

}
