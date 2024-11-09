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
#ifndef PHYDB_NET_H_
#define PHYDB_NET_H_

#include "datatype.h"
#include "enumtypes.h"
#include "phydb/common/logging.h"
#include "phydb/timing/actphydbtimingapi.h"
#include "snet.h"

namespace phydb {

class Net {
 public:
  Net() {}
  Net(const std::string &name, double weight) : name_(name), weight_(weight) {}

  void AddIoPin(int iopin_id);
  void AddCompPin(int comp_id, int pin_id);
  void AddRoutingGuide(int llx, int lly, int urx, int ury, int layer_id);

  Path *AddPath();
  // by default, width of signal nets is the standard width
  Path *AddPath(std::string &layer_name, std::string shape, int width = 0);

  const std::string &GetName() const;
  std::vector<PhydbPin> &GetPinsRef();
  std::vector<int> &GetIoPinIdsRef();
  std::vector<Rect3D<int>> &GetRoutingGuidesRef();
  std::vector<Path> &GetPathsRef();

  void SetDriverPin(bool is_driver_io_pin, int pin_id);
  bool IsDriverIoPin() const { return is_driver_io_pin_; }
  int GetDriverPinId() const { return driver_pin_id_; }

  void Report();

 private:
  std::string name_;
  SignalUse use_ = SignalUse::SIGNAL;

  double weight_ = 1.0;

  std::vector<PhydbPin> pins_;
  std::vector<int> iopins_;
  std::vector<Rect3D<int>> guides_;

  std::vector<Path> paths_;

  // cached info
  bool is_driver_io_pin_ = false;
  int driver_pin_id_ = -1;
};

std::ostream &operator<<(std::ostream &, const Net &);

}  // namespace phydb

#endif  // PHYDB_NET_H_
