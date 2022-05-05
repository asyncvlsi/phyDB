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
#include "snet.h"

namespace phydb {

void Polygon::SetLayerName(std::string layer_name) {
  layer_name_ = layer_name;
}

void Polygon::AddRoutingPoint(Point2D<int> p) {
  routing_points_.push_back(p);
}

void Polygon::AddRoutingPoint(int x, int y) {
  routing_points_.emplace_back(x, y);
}

std::vector<Point2D<int>> &Polygon::GetRoutingPointsRef() {
  return routing_points_;
}

std::string Polygon::GetLayerName() const {
  return layer_name_;
}

void Polygon::Report() const {
  std::cout << "POLYGON " << layer_name_ << " ";
  for (auto &point : routing_points_) {
    std::cout << " ( " << point.x << " " << point.y << " ) ";
  }
  std::cout << "\n";
}

void Path::SetLayerName(std::string &layer_name) {
  layer_name_ = layer_name;
}

void Path::SetWidth(int width) {
  width_ = width;
}

void Path::SetShape(std::string &shape) {
  shape_ = shape;
}

void Path::SetViaName(std::string &via_name) {
  via_name_ = via_name;
}

void Path::SetRect(int lx, int ly, int ux, int uy) {
  via_rect_ = Rect2D<int>(lx, ly, ux, uy);
}

void Path::SetRect(Rect2D<int> rect) {
  via_rect_ = rect;
}

void Path::AddRoutingPoint(Point3D<int> p) {
  routing_points_.push_back(p);
}

void Path::AddRoutingPoint(int x, int y, int ext) {
  routing_points_.emplace_back(x, y, ext);
}

std::string Path::GetLayerName() const {
  return layer_name_;
}

int Path::GetWidth() const {
  return width_;
}

std::string Path::GetShape() const {
  return shape_;
}

std::string Path::GetViaName() const {
  return via_name_;
}

Rect2D<int> Path::GetRect() const {
  return via_rect_;
}

std::vector<Point3D<int>> &Path::GetRoutingPointsRef() {
  return routing_points_;
}

void Path::Report() {
  std::cout << " NEW " << layer_name_ << " " << width_ << " + SHAPE "
            << shape_;

  for (auto &point : routing_points_) {
    if (point.z != -1) {
      std::cout << " ( " << point.x << " " << point.y << " " << point.z
                << " ) ";
    } else {
      std::cout << " ( " << point.x << " " << point.y << " ) ";
    }
  }

  if (!via_rect_.IsEmpty())
    std::cout << " (" << via_rect_.ll.x << " " << via_rect_.ll.y << " "
              << via_rect_.ur.x << " " << via_rect_.ur.y << ")";
  if (!via_name_.empty())
    std::cout << via_name_;

  std::cout << "\n";
}

void SNet::SetName(std::string &name) {
  name_ = name;
}

void SNet::SetUse(SignalUse use) {
  bool e = (use == phydb::SignalUse::GROUND || use == phydb::SignalUse::POWER);
  PhyDBExpects(e, "special net use should be POWER or GROUND");
  use_ = use;
}

Path *SNet::AddPath() {
  int id = (int) paths_.size();
  paths_.emplace_back();
  return &paths_[id];
}

Path *SNet::AddPath(std::string &layer_name, std::string shape, int width) {
  int id = (int) paths_.size();
  paths_.emplace_back(layer_name, shape, width);
  return &paths_[id];
}

Polygon *SNet::AddPolygon(std::string const &layer_name) {
  int id = (int) polygons_.size();
  polygons_.emplace_back(layer_name);
  return &polygons_[id];
}

std::string SNet::GetName() const {
  return name_;
}

SignalUse SNet::GetUse() const {
  return use_;
}

std::vector<Path> &SNet::GetPathsRef() {
  return paths_;
}

std::vector<Polygon> &SNet::GetPolygonsRef() {
  return polygons_;
}

void SNet::Report() {
  std::cout << "SNET: " << name_
            << " use: " << SignalUseStr(use_) << "\n";
  for (auto p : paths_) {
    p.Report();
  }
}

}