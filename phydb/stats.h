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
#ifndef PHYDB_STATS_H_
#define PHYDB_STATS_H_

#include "phydb.h"

namespace phydb {

class StatsPin {
  friend class Stats;
  friend class StatsComponent;

 public:
  StatsPin(phydb::Pin &pin, int dbuPerMicron) {
    name_ = pin.GetName();
    use_ = pin.GetUse();
    layer_rects_ = pin.GetLayerRectCpy();

    for (auto &layerRect : layer_rects_) {
      for (auto &rect : layerRect.rects_) {
        rect.ll.x *= dbuPerMicron;
        rect.ll.y *= dbuPerMicron;
        rect.ur.x *= dbuPerMicron;
        rect.ur.y *= dbuPerMicron;
      }
    }
  }

 private:
  std::string name_;
  SignalUse use_;
  std::vector<LayerRect> layer_rects_;
};

class StatsComponent {
  friend class Stats;

 public:
  void ComputeLocation(Rect2D<double> &rect, CompOrient orient,
                       Point2D<int> location, Point2D<int> orig,
                       Point2D<int> size);

  StatsComponent(Component &, int);

 private:
  std::string name_;
  phydb::Point2D<int> location_;
  phydb::Point2D<int> size_;
  std::vector<StatsPin> pins_;
  phydb::OBS obs_;
};

class Stats {
 public:
  Stats() {}
  Stats(PhyDB *db_ptr) : db_ptr_(db_ptr) {}

  ~Stats() {
    delete[] RUDY_;
    delete[] pin_density_;
  }

  void SetDbPtr(PhyDB *);
  PhyDB *GetDbPtr();
  void SetGcellSize(int);
  int GetGcellSize() const;
  void AddStatsComponent(Component &);
  std::vector<StatsComponent> &GetStatsComponentsRef();

  StatsPin &GetStatsPin(int component_id, int pin_id);

  void ComputeRUDY();
  void ComputePinDensity();

 private:
  PhyDB *db_ptr_ = nullptr;
  int Gcell_size_ = 15;  // in the unit of the min pitch of all layers
  std::vector<StatsComponent> stats_components_;
  double *RUDY_;
  double *pin_density_;
};

}  // namespace phydb

#endif
