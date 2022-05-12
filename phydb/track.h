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
#ifndef PHYDB_TRACK_H_
#define PHYDB_TRACK_H_

#include <string>
#include <vector>

#include "enumtypes.h"
#include "phydb/common/logging.h"

namespace phydb {

class Track {
 public:
  Track() {}
  Track(
      XYDirection direction,
      int start,
      int nTracks,
      int step,
      std::vector<std::string> &layerNames
  ) : direction_(direction),
      start_(start),
      n_tracks_(nTracks),
      step_(step),
      layer_names_(layerNames) {}

  XYDirection GetDirection() const;
  int GetStart() const;
  int GetNTracks() const;
  int GetStep() const;
  std::vector<std::string> &GetLayerNames();

  friend std::ostream &operator<<(std::ostream &, const Track &);
 private:
  XYDirection direction_;
  int start_;
  int n_tracks_;
  int step_;

  std::vector<std::string> layer_names_;
};

std::ostream &operator<<(std::ostream &, const Track &);

}

#endif //PHYDB_TRACK_H_
