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
#include "net.h"

namespace phydb {

void Net::AddIoPin(int iopin_id) {
  iopins_.push_back(iopin_id);
}

void Net::AddCompPin(int comp_id, int pin_id) {
  pins_.emplace_back(comp_id, pin_id);
}

void Net::AddRoutingGuide(int llx, int lly, int urx, int ury, int layer_id) {
  guides_.emplace_back(llx, lly, layer_id, urx, ury, layer_id);
}

Path *Net::AddPath() {
  int id = (int) paths_.size();
  paths_.emplace_back();
  return &paths_[id];
}

Path *Net::AddPath(std::string &layer_name, std::string shape, int width) {
  int id = (int) paths_.size();
  paths_.emplace_back(layer_name, shape, width);
  return &paths_[id];
}

const std::string &Net::GetName() const {
  return name_;
}

std::vector<PhydbPin> &Net::GetPinsRef() {
  return pins_;
}

std::vector<int> &Net::GetIoPinIdsRef() {
  return iopins_;
}

std::vector<Rect3D<int>> &Net::GetRoutingGuidesRef() {
  return guides_;
}

void Net::SetDriverPin(bool is_driver_io_pin, int pin_id) {
  is_driver_io_pin_ = is_driver_io_pin;
  driver_pin_id_ = pin_id;
}

std::vector<Path> &Net::GetPathsRef() {
  return paths_;
}

void Net::Report() {
  std::cout << "NET: " << name_
            << "  weight: " << weight_
            << " size: " << pins_.size() << "\n";
  for (auto &iopin_id : iopins_) {
    std::cout << "  (PIN " << iopin_id << ") ";
  }

  for (auto &comp_pin : pins_) {
    std::cout << "  (" << comp_pin.InstanceId()
              << " " << comp_pin.PinId() << ") ";
  }
  std::cout << "\n";
}

}