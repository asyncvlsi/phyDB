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

#include <cmath>
#include "stats.h"

namespace phydb {

void Stats::SetGcellSize(int size) {
  PhyDBExpects(size > 0, "Need positive gcell size in stats");
  Gcell_size_ = size;
}

int Stats::GetGcellSize() const {
  return Gcell_size_;
}

void Stats::SetDbPtr(PhyDB *db_ptr) {
  db_ptr_ = db_ptr;
}

PhyDB *Stats::GetDbPtr() {
  return db_ptr_;
}

void Stats::AddStatsComponent(Component &component) {
  int dbuPerMicron = GetDbPtr()->design().GetUnitsDistanceMicrons();
  stats_components_.emplace_back(component, dbuPerMicron);
}

std::vector<StatsComponent> &Stats::GetStatsComponentsRef() {
  return stats_components_;
}

StatsPin &Stats::GetStatsPin(int component_id, int pin_id) {
  return stats_components_[component_id].pins_[pin_id];
}

StatsComponent::StatsComponent(Component &component, int dbuPerMicron) {
  this->name_ = component.GetName();

  //auto location = component.GetLocation();

  Macro *macro_ptr = component.GetMacro();

  Point2D<int> orig;
  orig.x = macro_ptr->GetOriginX() * dbuPerMicron;
  orig.y = macro_ptr->GetOriginY() * dbuPerMicron;

  size_.x = macro_ptr->GetWidth() * dbuPerMicron;
  size_.y = macro_ptr->GetHeight() * dbuPerMicron;

  location_ = component.GetLocation();
  //enlarge by dbuPermicron
  for (auto &pin : macro_ptr->GetPinsRef()) {
    pins_.emplace_back(pin, dbuPerMicron);
  }
  phydb::OBS obs;
  auto obs_ptr = macro_ptr->GetObs();

  obs_ = *obs_ptr; // make a copy of obs

  for (auto &layerRect : obs_.GetLayerRectsRef()) {
    for (auto &rect : layerRect.rects_) {
      rect.ll.x *= dbuPerMicron;
      rect.ll.y *= dbuPerMicron;
      rect.ur.x *= dbuPerMicron;
      rect.ur.y *= dbuPerMicron;
    }
  }

  CompOrient orient = component.GetOrientation();
  //compute final coordinate
  for (auto &p : pins_) {
    for (auto &layer_rect : p.layer_rects_) {
      for (auto &rect : layer_rect.rects_) {
        ComputeLocation(rect, orient, location_, orig, size_);
      }
    }
  }

  for (auto &layer_rect : obs_.GetLayerRectsRef()) {
    for (auto &rect : layer_rect.rects_) {
      ComputeLocation(rect, orient, location_, orig, size_);
    }
  }
}

void StatsComponent::ComputeLocation(
    Rect2D<double> &rect,
    CompOrient orient,
    Point2D<int> location,
    Point2D<int> origin,
    Point2D<int> size
) {
  int llx, lly, urx, ury;
  switch (orient) {
    case phydb::CompOrient::N: { // "N"
      llx = location.x + origin.x + rect.ll.x;
      lly = location.y + origin.y + rect.ll.y;
      urx = location.x + origin.x + rect.ur.x;
      ury = location.y + origin.y + rect.ur.y;
      rect.Set(llx, lly, urx, ury);
      break;
    }
    case phydb::CompOrient::S: { // "S"
      llx = location.x + origin.x + size.x - rect.ur.x;
      lly = location.y + origin.y + size.y - rect.ur.y;
      urx = location.x + origin.x + size.x - rect.ll.x;
      ury = location.y + origin.y + size.y - rect.ll.y;
      rect.Set(llx, lly, urx, ury);
      break;
    }
    case phydb::CompOrient::W: { // "W"
      float centerx = (rect.ll.x + rect.ur.x) / 2.0;
      float centery = (rect.ll.y + rect.ur.y) / 2.0;
      int rectwidth = rect.ur.x - rect.ll.x;
      int rectheight = rect.ur.y - rect.ll.y;
      float centerxprime = -centery;
      // assumption origin.x and origin.y is always 0
      // cos(90deg)centerx - sin(90deg)centery
      float centeryprime = centerx;
      // sin(90deg)centerx + cos(90deg)centery
      int tmplength;
      tmplength = rectwidth;
      // 90deg rotation, width and height swap
      rectwidth = rectheight;
      rectheight = tmplength;
      // macro width and height should also swap, but we cannot do it here, because we are at pin level now
      // but remember, when we need macro width, we should call its height, and vice versa
      llx = std::round(centerxprime - rectwidth / 2.0);
      lly = std::round(centeryprime - rectheight / 2.0);
      urx = std::round(centerxprime + rectwidth / 2.0);
      ury = std::round(centeryprime + rectheight / 2.0);
      // shift rightward for a macro width, now we should use macro.size.y, after shift move to (comp.placeLoc.x, comp.placeLoc.y)
      llx += size.y + location.x;
      lly += location.y;
      urx += size.y + location.x;
      ury += location.y;

      llx += origin.x;
      lly += origin.y;
      urx += origin.x;
      ury += origin.y;
      rect.Set(llx, lly, urx, ury);
      break;
    }

    case phydb::CompOrient::E : {   //"E"
      float centerx = (rect.ll.x + rect.ur.x) / 2.0;
      float centery = (rect.ll.y + rect.ur.y) / 2.0;
      int rectwidth = rect.ur.x - rect.ll.x;
      int rectheight = rect.ur.y - rect.ll.y;
      float centerxprime = centery;
      // assumption origin.x and origin.y is always 0
      // cos(270deg)centerx - sin(270deg)centery
      float centeryprime = -centerx;
      // sin(270deg)centerx + cos(270deg)centery
      int tmplength;
      tmplength = rectwidth;
      // 270deg rotation, width and height swap
      rectwidth = rectheight;
      rectheight = tmplength;
      // macro width and height should also swap, but we cannot do it here, because we are at pin level now
      // but remember, when we need macro width, we should call its height, and vice versa
      llx = centerxprime - rectwidth / 2.0;
      lly = centeryprime - rectheight / 2.0;
      urx = centerxprime + rectwidth / 2.0;
      ury = centeryprime + rectheight / 2.0;
      // shift upward for a macro height, now we should use macro.size.x, after shift move to (comp.placeLoc.x, comp.placeLoc.y)
      llx += location.x;
      lly += size.x + location.y;
      urx += location.x;
      ury += size.x + location.y;

      llx += origin.x;
      lly += origin.y;
      urx += origin.x;
      ury += origin.y;
      rect.Set(llx, lly, urx, ury);
      break;
    }

    case phydb::CompOrient::FN: { // "FN"
      llx = location.x + size.x - rect.ur.x;
      lly = location.y + rect.ll.y;
      urx = location.x + size.x - rect.ll.x;
      ury = location.y + rect.ur.y;

      llx += origin.x;
      lly += origin.y;
      urx += origin.x;
      ury += origin.y;
      rect.Set(llx, lly, urx, ury);
      break;
    }
    case phydb::CompOrient::FS: { //"FS"
      llx = location.x + rect.ll.x;
      lly = location.y + size.y - rect.ur.y;
      urx = location.x + rect.ur.x;
      ury = location.y + size.y - rect.ll.y;

      llx += origin.x;
      lly += origin.y;
      urx += origin.x;
      ury += origin.y;
      rect.Set(llx, lly, urx, ury);
      break;
    }
    case phydb::CompOrient::FW: { //"FW"
      float centerx = (rect.ll.x + rect.ur.x) / 2.0;
      float centery = (rect.ll.y + rect.ur.y) / 2.0;
      int rectwidth = rect.ur.x - rect.ll.x;
      int rectheight = rect.ur.y - rect.ll.y;
      float centerxprime = -centery;
      // assumption origin.x and origin.y is always 0
      // cos(90deg)centerx - sin(90deg)centery
      float centeryprime = centerx;
      // sin(90deg)centerx + cos(90deg)centery
      int tmplength;
      tmplength = rectwidth;
      // 90deg rotation, width and height swap
      rectwidth = rectheight;
      rectheight = tmplength;
      // macro width and height should also swap, but we cannot do it here, because we are at pin level now
      // but remember, when we need macro width, we should call its height, and vice versa
      llx = std::round(centerxprime - rectwidth / 2.0);
      lly = std::round(centeryprime - rectheight / 2.0);
      urx = std::round(centerxprime + rectwidth / 2.0);
      ury = std::round(centeryprime + rectheight / 2.0);
      // shift rightward for a macro width, now we should use macro.size.y
      llx += size.y;
      urx += size.y;
      // flip, this is done using the right boundary location, which is macro.size.y
      int axis = size.y;
      centerx = (llx + urx) / 2.0;
      centery = (lly + ury) / 2.0;
      rectwidth = urx - llx;
      rectheight = ury - lly;

      centerx = axis - centerx;

      llx = std::round(centerx - rectwidth / 2.0) + location.x;
      lly = std::round(centery - rectheight / 2.0) + location.y;
      urx = std::round(centerx + rectwidth / 2.0) + location.x;
      ury = std::round(centery + rectheight / 2.0) + location.y;

      llx += origin.x;
      lly += origin.y;
      urx += origin.x;
      ury += origin.y;

      rect.Set(llx, lly, urx, ury);
      break;
    }
    case phydb::CompOrient::FE: { //" FE"
      float centerx = (rect.ll.x + rect.ur.x) / 2.0;
      float centery = (rect.ll.y + rect.ur.y) / 2.0;
      int rectwidth = rect.ur.x - rect.ll.x;
      int rectheight = rect.ur.y - rect.ll.y;
      float centerxprime = centery;
      // assumption origin.x and origin.y is always 0
      // cos(270deg)centerx - sin(270deg)centery
      float centeryprime = -centerx;
      // sin(270deg)centerx + cos(270deg)centery
      int tmplength;
      tmplength = rectwidth;
      // 270deg rotation, width and height swap
      rectwidth = rectheight;
      rectheight = tmplength;
      // macro width and height should also swap, but we cannot do it here, because we are at pin level now
      // but remember, when we need macro width, we should call its height, and vice versa
      llx = centerxprime - rectwidth / 2.0;
      lly = centeryprime - rectheight / 2.0;
      urx = centerxprime + rectwidth / 2.0;
      ury = centeryprime + rectheight / 2.0;
      // shift upward for a macro height, now we should use macro.size.x
      lly += size.x;
      ury += size.x;

      int axis = size.y;
      centerx = (llx + urx) / 2.0;
      centery = (lly + ury) / 2.0;
      rectwidth = urx - llx;
      rectheight = ury - lly;

      centerx = axis - centerx;

      llx = std::round(centerx - rectwidth / 2.0) + location.x;
      lly = std::round(centery - rectheight / 2.0) + location.y;
      urx = std::round(centerx + rectwidth / 2.0) + location.x;
      ury = std::round(centery + rectheight / 2.0) + location.y;

      llx += origin.x;
      lly += origin.y;
      urx += origin.x;
      ury += origin.y;

      rect.Set(llx, lly, urx, ury);
      break;
    }
    default: {
      std::cout << "unknown orientation for component: "
                << phydb::CompOrientStr(orient) << std::endl;
      break;
    }
  }
}

void Stats::ComputeRUDY() {
  if (GetDbPtr() == nullptr) {
    std::cout << "Error: please initialize db_ptr in Stats" << std::endl;
    return;
  }
  int dbuPerMicron = GetDbPtr()->design().GetUnitsDistanceMicrons();
  if (GetStatsComponentsRef().size() == 0) {
    auto components = GetDbPtr()->design().GetComponentsRef();

    for (auto comp : components) {
      AddStatsComponent(comp);
    }
  }

  double min_pitch = 100.0;
  auto layers = GetDbPtr()->tech().GetLayersRef();
  for (auto layer : layers) {
    if (layer.GetType() == phydb::LayerType::ROUTING) {
      double tmp_min = std::min(layer.GetPitchX(), layer.GetPitchY());
      min_pitch = std::min(min_pitch, tmp_min);
    }
  }
  double Gcell_size = min_pitch * dbuPerMicron;
  auto die_area = GetDbPtr()->design().GetDieArea();
  int grid_x = (int) std::ceil(
      (double) (die_area.URX() - die_area.LLX()) / (double) Gcell_size);
  int grid_y = (int) std::ceil(
      (double) (die_area.URY() - die_area.LLY()) / (double) Gcell_size);

  RUDY = new double[grid_x * grid_y];

  for (int i = 0; i < grid_x * grid_y; i++) {
    RUDY[i] = 0;
  }

  auto nets = GetDbPtr()->design().GetNetsRef();

  for (auto net : nets) {
    int net_min_x = grid_x - 1, net_min_y = grid_y - 1;
    int net_max_x = 0, net_max_y = 0;

    auto phydb_pins = net.GetPinsRef();
    for (auto phydb_pin : phydb_pins) {
      int pin_min_x = grid_x - 1, pin_min_y = grid_y - 1;
      int pin_max_x = 0, pin_max_y = 0;
      auto stats_pin = GetStatsPin(phydb_pin.InstanceId(), phydb_pin.PinId());

      for (auto layer_rect : stats_pin.layer_rects_) {
        for (auto rect : layer_rect.rects_) {
          pin_min_x = std::min((int) rect.LLX(), pin_min_x);
          pin_min_y = std::min((int) rect.LLY(), pin_min_y);
          pin_max_x = std::max((int) rect.URX(), pin_max_x);
          pin_max_y = std::max((int) rect.URY(), pin_max_y);
        }
      }

      net_min_x = std::min(net_min_x, pin_min_x / grid_x);
      net_min_y = std::min(net_min_y, pin_min_y / grid_y);
      net_max_x = std::max(net_max_x, pin_max_x / grid_x);
      net_max_y = std::max(net_max_y, pin_max_y / grid_y);
    }
    double h = net_max_x - net_min_x + 1;
    double v = net_max_y - net_min_y + 1;
    double net_rudy = (h + v) / (h * v);

    for (int x = net_min_x; x <= net_max_x; x++) {
      for (int y = net_min_y; y <= net_max_y; y++) {
        RUDY[y * grid_x + x] += net_rudy;
      }
    }
  }
}

void Stats::ComputePinDensity() {
  if (GetDbPtr() == nullptr) {
    std::cout << "Error: please initialize db_ptr in Stats" << std::endl;
    return;
  }
  int dbuPerMicron = GetDbPtr()->design().GetUnitsDistanceMicrons();
  if (GetStatsComponentsRef().size() == 0) {
    auto components = GetDbPtr()->design().GetComponentsRef();

    for (auto comp : components) {
      AddStatsComponent(comp);
    }
  }

  double min_pitch = 100.0;
  auto layers = GetDbPtr()->tech().GetLayersRef();
  for (auto layer : layers) {
    if (layer.GetType() == phydb::LayerType::ROUTING) {
      double tmp_min = std::min(layer.GetPitchX(), layer.GetPitchY());
      min_pitch = std::min(min_pitch, tmp_min);
    }
  }
  double Gcell_size = min_pitch * dbuPerMicron;
  auto die_area = GetDbPtr()->design().GetDieArea();
  int grid_x = (int) std::ceil(
      (double) (die_area.URX() - die_area.LLX()) / (double) Gcell_size);
  int grid_y = (int) std::ceil(
      (double) (die_area.URY() - die_area.LLY()) / (double) Gcell_size);

  pin_density = new double[grid_x * grid_y];

  for (int i = 0; i < grid_x * grid_y; i++) {
    pin_density[i] = 0;
  }

  auto nets = GetDbPtr()->design().GetNetsRef();

  for (auto net : nets) {

    auto phydb_pins = net.GetPinsRef();
    for (auto phydb_pin : phydb_pins) {
      int pin_min_x = grid_x - 1, pin_min_y = grid_y - 1;
      int pin_max_x = 0, pin_max_y = 0;
      auto stats_pin = GetStatsPin(phydb_pin.InstanceId(), phydb_pin.PinId());

      for (auto layer_rect : stats_pin.layer_rects_) {
        for (auto rect : layer_rect.rects_) {
          pin_min_x = std::min((int) rect.LLX(), pin_min_x);
          pin_min_y = std::min((int) rect.LLY(), pin_min_y);
          pin_max_x = std::max((int) rect.URX(), pin_max_x);
          pin_max_y = std::max((int) rect.URY(), pin_max_y);
        }
      }

      for (int x = pin_min_x; x <= pin_max_x; x++) {
        for (int y = pin_min_y; y <= pin_max_y; y++) {
          pin_density[y * grid_x + x] += 1;
        }
      }
    }
  }
}

} //namespace phydb
