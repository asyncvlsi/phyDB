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

#ifndef PHYDB_INCLUDE_IOPIN_H_
#define PHYDB_INCLUDE_IOPIN_H_

#include "datatype.h"
#include "logging.h"
#include "enumtypes.h"

namespace phydb {

class IOPin {
 public:
  int id_;
  std::string name_;
  std::string net_name_;
  SignalDirection direction_;
  SignalUse use_;

  std::string layer_name_;
  Rect2D<int> rect_;

  Point2D<int> location_;
  CompOrient orient_ = N;
  PlaceStatus place_status_ = UNPLACED;

  IOPin() : id_(-1) {}
  IOPin(std::string &name, SignalDirection direction, SignalUse use) :
      name_(name), direction_(direction), use_(use) {}
  IOPin(std::string name,
        std::string netName,
        SignalDirection direction,
        SignalUse use,
        std::string layerName,
        Rect2D<int> rect,
        Point2D<int> location,
        CompOrient orient,
        PlaceStatus status) :
      name_(name),
      net_name_(netName),
      direction_(direction),
      use_(use),
      layer_name_(layerName),
      rect_(rect),
      location_(location),
      orient_(orient),
      place_status_(status) {}

  void SetNetName(std::string const &net_name);
  void SetShape(std::string &layer_name, int lx, int ly, int ux, int uy);
  void SetPlacement(PlaceStatus place_status,
                    int x,
                    int y,
                    CompOrient orient);
  void SetPlacementStatus(PlaceStatus place_status);

  const std::string &GetName();
  const std::string &GetNetName();
  SignalDirection GetDirection();
  SignalUse GetUse();
  const std::string &GetLayerName();
  Rect2D<int> GetRect();
  Point2D<int> GetLocation();
  CompOrient GetOrientation();
  PlaceStatus GetPlacementStatus();

  void Report();
};

std::ostream &operator<<(std::ostream &, const IOPin &);

}

#endif //PHYDB_INCLUDE_IOPIN_H_




