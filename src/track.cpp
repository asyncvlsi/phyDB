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

#include "track.h"

namespace phydb {

XYDirection Track::GetDirection() const {
  return direction_;
}

int Track::GetStart() const {
  return start_;
}

int Track::GetNTracks() const {
  return n_tracks_;
}

int Track::GetStep() const {
  return step_;
}

std::vector<std::string> &Track::GetLayerNames() {
  return layer_names_;
}

std::ostream &operator<<(std::ostream &os, const Track &t) {
  os << "direction: " << XYDirectionStr(t.direction_);
  os << " start: " << t.start_ << " numTracks:" << t.n_tracks_ << " step: "
     << t.step_;

  return os;
}

}

