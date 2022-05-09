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
#ifndef PHYDB_IOPIN_H_
#define PHYDB_IOPIN_H_

#include "datatype.h"
#include "phydb/common/logging.h"
#include "enumtypes.h"

namespace phydb {

class IOPin {
 public:
  IOPin() : id_(-1) {}
  IOPin(
      std::string const &name,
      SignalDirection direction,
      SignalUse use
  ) :
      name_(name),
      direction_(direction),
      use_(use) {}
  IOPin(
      std::string const &name,
      int &net_id,
      SignalDirection direction,
      SignalUse use,
      std::string const &layerName,
      Rect2D<int> rect,
      Point2D<int> location,
      CompOrient orient,
      PlaceStatus status
  ) :
      name_(name),
      net_id_(net_id),
      direction_(direction),
      use_(use),
      layer_name_(layerName),
      rect_(rect),
      location_(location),
      orient_(orient),
      place_status_(status) {}

  void SetNetId(int net_id);
  void SetShape(std::string const &layer_name, int lx, int ly, int ux, int uy);
  void SetPlacement(
      PlaceStatus place_status,
      int x,
      int y,
      CompOrient orient
  );
  void SetPlacementStatus(PlaceStatus place_status);

  int GetId() const { return id_; }
  const std::string &GetName();
  int GetNetId();
  SignalDirection GetDirection();
  SignalUse GetUse();
  const std::string &GetLayerName();
  Rect2D<int> GetRect();
  Point2D<int> GetLocation();
  CompOrient GetOrientation();
  PlaceStatus GetPlacementStatus();

  Rect2D<int> GetBoundingBox();

  void Report();
 private:
  int id_;
  std::string name_;
  int net_id_;
  SignalDirection direction_;
  SignalUse use_;

  std::string layer_name_;
  Rect2D<int> rect_;

  Point2D<int> location_;
  CompOrient orient_ = CompOrient::N;
  PlaceStatus place_status_ = PlaceStatus::UNPLACED;
};

std::ostream &operator<<(std::ostream &, const IOPin &);

}

#endif //PHYDB_IOPIN_H_




