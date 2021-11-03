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
#ifndef PHYDB_INCLUDE_SNET_H_
#define PHYDB_INCLUDE_SNET_H_

#include "datatype.h"
#include "enumtypes.h"
#include "logging.h"

namespace phydb {

class Polygon {
 private:
  std::string layer_name_;
  std::vector<Point2D < int>> routing_points_;

 public:
  Polygon() {}
  Polygon(std::string layer_name) : layer_name_(layer_name) {}

  void SetLayerName(std::string layer_name);
  void AddRoutingPoint(Point2D<int> p);
  void AddRoutingPoint(int x, int y);

  std::string GetLayerName() const;
  std::vector<Point2D < int>>&
  GetRoutingPointsRef();

  void Report() const;

};

class Path {
 private:
  std::string layer_name_;
  int width_;
  std::string shape_;
  std::string via_name_;

  Rect2D<int> via_rect_;
  std::vector<Point3D < int>> routing_points_;

 public:
  Path() : width_(0) {}
  Path(std::string &layer_name, std::string &shape, int width) :
      layer_name_(layer_name),
      width_(width),
      shape_(shape) {}

  void SetLayerName(std::string &);
  void SetWidth(int);
  void SetShape(std::string &);
  void SetViaName(std::string &);
  void SetRect(int lx, int ly, int ux, int uy);
  void SetRect(Rect2D<int> rect);
  void AddRoutingPoint(Point3D<int> p);
  void AddRoutingPoint(int x, int y, int ext = -1);

  std::string GetLayerName() const;
  int GetWidth() const;
  std::string GetShape() const;
  std::string GetViaName() const;

  Rect2D<int> GetRect() const;
  std::vector<Point3D < int>>&
  GetRoutingPointsRef();

  void Report();
};

class SNet {
 private:
  std::string name_;
  SignalUse use_; // POWER or GROUND
  std::vector<Path> paths_;
  std::vector<Polygon> polygons_;
 public:
  SNet() {}
  SNet(std::string const &name) : name_(name) {}
  SNet(std::string const &name, SignalUse use) : name_(name), use_(use) {}

  void SetName(std::string &);
  void SetUse(SignalUse);
  Path *AddPath();
  Path *AddPath(std::string &layer_name, std::string shape, int width);
  Polygon *AddPolygon(std::string layer_name);

  std::string GetName() const;
  SignalUse GetUse() const;
  std::vector<Path> &GetPathsRef();
  std::vector<Polygon> &GetPolygonsRef();

  void Report();
};

}

#endif //PHYDB_INCLUDE_SNET_H_
