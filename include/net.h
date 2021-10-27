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
#ifndef PHYDB_INCLUDE_NET_H_
#define PHYDB_INCLUDE_NET_H_

#include "actphydbtimingapi.h"
#include "datatype.h"
#include "enumtypes.h"
#include "logging.h"
#include "snet.h"

namespace phydb {

class Net {
 public:
  std::string name_;
  SignalUse use_;

  double weight_;

  std::vector<PhydbPin> pins_;
  std::vector<std::string> iopin_names_;
  int driver_pin_id_ = 0;
  std::vector<Rect3D<int>> guides_;

  std::vector<Path> paths_;

  Net() {}
  Net(const std::string &name, double weight)
      : name_(name), weight_(weight) {}

  void AddIoPin(std::string const &iopin_name);
  void AddCompPin(int comp_id, int pin_id);
  void AddRoutingGuide(int llx, int lly, int urx, int ury, int layerID);

  Path *AddPath();
  Path *AddPath(std::string &layer_name,
                std::string shape,
                int width = 0); //by default, width of signal nets is the standard width

  const std::string &GetName() const;
  std::vector<PhydbPin> &GetPinsRef();
  std::vector<std::string> &GetIoPinNamesRef();
  std::vector<Rect3D<int>> &GetRoutingGuidesRef();
  std::vector<Path> &GetPathsRef();

  void Report();
};

std::ostream &operator<<(std::ostream &, const Net &);

}

#endif //PHYDB_INCLUDE_NET_H_
