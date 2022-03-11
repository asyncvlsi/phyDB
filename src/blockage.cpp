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
#include "blockage.h"

namespace phydb {

Blockage::Blockage() :
    layer_ptr_(nullptr),
    is_slots_(false),
    is_fills_(false),
    is_pushdown_(false),
    is_exceptpgnet_(false),
    component_ptr_(nullptr),
    min_spacing_(-1.0),
    effective_width_(-1.0),
    mask_num_(0),
    is_placement_(false),
    is_soft_(false),
    max_density_(-1.0) {}

void Blockage::SetLayer(Layer *layer_ptr) {
  layer_ptr_ = layer_ptr;
}

void Blockage::SetSlots(bool is_slots) {
  is_slots_ = is_slots;
}

void Blockage::SetFills(bool is_fills) {
  is_fills_ = is_fills;
}

void Blockage::SetPushdown(bool is_pulldown) {
  is_pushdown_ = is_pulldown;
}

void Blockage::SetExceptpgnet(bool is_exceptpgnet) {
  is_exceptpgnet_ = is_exceptpgnet;
}

void Blockage::SetComponent(Component *component_ptr) {
  component_ptr_ = component_ptr;
}

void Blockage::SetSpacing(int min_spacing) {
  min_spacing_ = min_spacing;
}

void Blockage::SetDesignRuleWidth(int effective_width) {
  effective_width_ = effective_width;
}

void Blockage::SetMaskNum(int mask_num) {
  mask_num_ = mask_num;
}

void Blockage::SetPlacement(bool is_placement) {
  is_placement_ = is_placement;
}

void Blockage::SetSoft(bool is_soft) {
  is_soft_ = is_soft;
}

void Blockage::SetPartial(double max_density) {
  max_density_ = max_density;
}

void Blockage::AddRect(int lx, int ly, int ux, int uy) {
  rects_.emplace_back(lx, ly, ux, uy);
}

Points2D<int> &Blockage::AddPolygon() {
  polygons_.emplace_back();
  return polygons_.back();
}

Layer *Blockage::GetLayer() {
  return layer_ptr_;
}

bool Blockage::IsSlots() const {
  return is_slots_;
}

bool Blockage::IsFills() const {
  return is_fills_;
}

bool Blockage::IsPushdown() const {
  return is_pushdown_;
}

bool Blockage::IsExceptpgnet() const {
  return is_exceptpgnet_;
}

Component *Blockage::GetComponent() {
  return component_ptr_;
}

int Blockage::GetSpacing() const {
  return min_spacing_;
}

int Blockage::GetDesignRuleWidth() const {
  return effective_width_;
}

int Blockage::GetMaskNum() const {
  return mask_num_;
}

bool Blockage::IsPlacement() const {
  return is_placement_;
}

bool Blockage::IsSoft() const {
  return is_soft_;
}

double Blockage::GetMaxPlacementDensity() const {
  return max_density_;
}

std::vector<Rect2D<int>> &Blockage::GetRectsRef() {
  return rects_;
}

std::vector<Points2D<int>> &Blockage::GetPolygonRef() {
  return polygons_;
}

void Blockage::Report() {
  if (GetLayer() != nullptr) {
    std::cout << "- LAYER " << GetLayer()->GetName();
    if (IsSlots()) {
      std::cout << " + SLOTS";
    }
    if (IsFills()) {
      std::cout << " + FILLS";
    }
    if (IsPushdown()) {
      std::cout << " + PUSHDOWN";
    }
    if (IsExceptpgnet()) {
      std::cout << " + EXCEPTPGNET";
    }
    if (GetSpacing() > 0) {
      std::cout << " + SPACING " << GetSpacing();
    }
    if (GetDesignRuleWidth() > 0) {
      std::cout << " + DESIGNRULEWIDTH " << GetDesignRuleWidth();
    }
    if (GetMaskNum() > 0) {
      std::cout << " + MASK " << GetMaskNum();
    }
    if (GetComponent() != nullptr) {
      std::cout << " + COMPONENT " << GetComponent()->GetName();
    }
    std::cout << "\n";
  }
  if (IsPlacement()) {
    std::cout << "- PLACEMENT";
    if (IsSoft()) {
      std::cout << " + SOFT";
    }
    if (GetMaxPlacementDensity() > 0) {
      std::cout << " + PARTIAL " << GetMaxPlacementDensity();
    }
    if (IsPushdown()) {
      std::cout << " + PUSHDOWN";
    }
    if (GetComponent() != nullptr) {
      std::cout << " + COMPONENT " << GetComponent()->GetName();
    }
    std::cout << "\n";
  }

  for (auto &rect: rects_) {
    std::cout << "   RECT "
              << rect.LLX() << " " << rect.LLY() << " "
              << rect.URX() << " " << rect.URY() << "\n";
  }

  for (auto &polygon: polygons_) {
    std::cout << "   POLYGON ";
    for (auto &point: polygon.GetPointsRef()) {
      std::cout << point.x << " " << point.y << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

}
