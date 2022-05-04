/*******************************************************************************
 *
 * Copyright (c) 2022 Jiayuan He, Yihang Yang
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
#ifndef PHYDB_BLOCKAGE_H_
#define PHYDB_BLOCKAGE_H_

#include "component.h"
#include "datatype.h"
#include "layer.h"

namespace phydb {

class Blockage {
 public:
  Blockage();
  void SetLayer(Layer *layer_ptr);
  void SetSlots(bool is_slots = true);
  void SetFills(bool is_fills = true);
  void SetPushdown(bool is_pulldown = true);
  void SetExceptpgnet(bool is_exceptpgnet = true);
  void SetComponent(Component *component_ptr);
  void SetSpacing(int min_spacing);
  void SetDesignRuleWidth(int effective_width);
  void SetMaskNum(int mask_num);
  void SetPlacement(bool is_placement = true);
  void SetSoft(bool is_soft = true);
  void SetPartial(double max_density);
  void AddRect(int lx, int ly, int ux, int uy);
  Points2D<int> &AddPolygon();

  Layer *GetLayer();
  bool IsSlots() const;
  bool IsFills() const;
  bool IsPushdown() const;
  bool IsExceptpgnet() const;
  Component *GetComponent();
  int GetSpacing() const;
  int GetDesignRuleWidth() const;
  int GetMaskNum() const;
  bool IsPlacement() const;
  bool IsSoft() const;
  double GetMaxPlacementDensity() const;
  std::vector<Rect2D<int>> &GetRectsRef();
  std::vector<Points2D<int>> &GetPolygonRef();

  void Report();

 private:
  Layer *layer_ptr_;
  bool is_slots_;
  bool is_fills_;
  bool is_pushdown_;
  bool is_exceptpgnet_;
  Component *component_ptr_;
  int min_spacing_;
  int effective_width_;
  int mask_num_;
  bool is_placement_;
  bool is_soft_;
  double max_density_;
  std::vector<Rect2D<int>> rects_;
  std::vector<Points2D<int>> polygons_;
};

}

#endif //PHYDB_BLOCKAGE_H_
