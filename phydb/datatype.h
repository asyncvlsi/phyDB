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
#ifndef PHYDB_DATATYPES_H_
#define PHYDB_DATATYPES_H_

#include <string>
#include <vector>

#include "enumtypes.h"
#include "phydb/common/logging.h"

namespace phydb {

template<typename T>
class Point2D {
 public:
  T x;
  T y;

  Point2D() : x(0), y(0) {}
  Point2D(T xx, T yy) : x(xx), y(yy) {}
  void Set(T xx, T yy) {
    x = xx;
    y = yy;
  }
  void Clear() {
    x = 0;
    y = 0;
  }
  void Reset() {
    x = 0;
    y = 0;
  }
  bool IsEmpty() {
    return (x == 0 && y == 0);
  }
  std::string Str() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }
  //define '<' and '>' for comparison in std Set and map
  bool operator<(const Point2D<T> p) const {
    if (y < p.y) return true;
    else if (y > p.y) return false;
    else return (x < p.x);
  }
  bool operator>(const Point2D<T> p) const {
    if (y > p.y) return true;
    else if (y < p.y) return false;
    else return (x > p.x);
  }
  void Rotate(CompOrient orient, T width, T height) {
    double new_x = x;
    double new_y = y;
    switch (orient) {
      case CompOrient::N: {
        // no rotation is needed
        break;
      }
      case CompOrient::S: {
        new_x = width - x;
        new_y = height - y;
        break;
      }
      case CompOrient::W: {
        new_x = height - y;
        new_y = x;
        break;
      }
      case CompOrient::E: {
        new_x = y;
        new_y = width - x;
        break;
      }
      case CompOrient::FN: {
        new_x = width - x;
        new_y = y;
        break;
      }
      case CompOrient::FS: {
        new_x = x;
        new_y = height - y;
        break;
      }
      case CompOrient::FW: {
        new_x = y;
        new_y = x;
        break;
      }
      case CompOrient::FE: {
        new_x = height - y;
        new_y = width - x;
        break;
      }
      default: {
        PhyDBExpects(false, "Impossible to happen!");
      }
    }
    x = new_x;
    y = new_y;
  }
};

template<typename T>
using Size2D = Point2D<T>;

template<typename T>
class Point3D {
 public:
  T x;
  T y;
  T z;
  Point3D() : x(0), y(0), z(0) {}
  Point3D(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
  void Set(T xx, T yy, T zz) {
    x = xx;
    y = yy;
    z = zz;
  }
  void Reset() {
    x = 0;
    y = 0;
    z = 0;
  }
  bool IsEmpty() {
    return (x == 0 && y == 0 && z == 0);
  }
  std::string Str() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", "
        + std::to_string(z) + ")";
  }

  //define '<' and '>' for comparison in Set amd map
  bool operator<(const Point3D<T> p) const {
    if (z < p.z) return true;
    else if (z > p.z) return false;
    else if (y < p.y) return true;
    else if (y > p.y) return false;
    else return (x < p.x);
  }
  bool operator>(const Point3D<T> p) const {
    if (z > p.z) return true;
    else if (z < p.z) return false;
    else if (y > p.y) return true;
    else if (y < p.y) return false;
    else return (x > p.x);
  }
};

template<typename T>
class Rect2D {
 public:
  Point2D<T> ll; //lower left
  Point2D<T> ur; //upper right

  Rect2D() : ll(0, 0), ur(0, 0) {}
  Rect2D(Point2D<T> LL, Point2D<T> UR) : ll(LL), ur(UR) { SanityCheck(); }
  Rect2D(T llx, T lly, T urx, T ury) :
      ll(llx, lly),
      ur(urx, ury) {
    SanityCheck();
  }

  bool IsLegal() { return (ll.x < ur.x && ll.y < ur.y); }
  void SanityCheck() {
    PhyDBExpects(
        this->IsLegal(),
        "Illegal Rect2D: ll, ur, " + ll.Str() + ur.Str()
    );
  }
  bool IsEmpty() { return ll.IsEmpty() && ur.IsEmpty(); }

  void Set(Point2D<T> LL, Point2D<T> UR) {
    ll = LL;
    ur = UR;
    SanityCheck();
  }
  void Set(T llx, T lly, T urx, T ury) {
    ll.x = llx;
    ll.y = lly;
    ur.x = urx;
    ur.y = ury;
    SanityCheck();
  }

  bool BoundaryExclusiveCover(Point2D<T> p) {
    return (ll.x < p.x && ll.y < p.y && ur.x > p.x && ur.y > p.y);
  }
  bool Cover(Point2D<T> p) {
    return (ll.x <= p.x && ll.y <= p.y && ur.x >= p.x && ur.y >= p.y);
  }

  std::string Str() {
    return ll.Str() + ur.Str();
  }

  T LLX() const { return ll.x; }
  T LLY() const { return ll.y; }
  T URX() const { return ur.x; }
  T URY() const { return ur.y; }
  T GetHeight() const { return ur.y - ll.y; }
  T GetWidth() const { return ur.x - ll.x; }
};

template<typename T>
class Rect2DLayer : public Rect2D<T> {
 public:
  std::string layer;
  Rect2DLayer() : Rect2D<T>() {}
  void Set(std::string &layer_init, T llx, T lly, T urx, T ury) {
    this->layer = layer_init;
    this->ll.Set(llx, lly);
    this->ur.Set(urx, ury);
  }
};

template<typename T>
class Rect3D {
 public:
  Point3D<T> ll;
  Point3D<T> ur;

  Rect3D() : ll(0, 0, 0), ur(0, 0, 0) {}
  Rect3D(Point3D<T> LL, Point3D<T> UR) : ll(LL), ur(UR) {}
  Rect3D(T llx, T lly, T llz, T urx, T ury, T urz) :
      ll(llx, lly, llz),
      ur(urx, ury, urz) {}

  bool IsLegal() { //on the same layer is legal
    return (ll.x < ur.x && ll.y < ur.y && ll.z <= ur.z);
  }
  void SanityCheck() {
    PhyDBExpects(
        this->IsLegal(),
        "Illegal Rect2D: ll, ur, " + ll.Str() + ur.Str()
    );
  }

  void set(Point3D<T> LL, Point3D<T> UR) {
    ll = LL;
    ur = UR;
    SanityCheck();
  }
  void set(T llx, T lly, T llz, T urx, T ury, T urz) {
    ll.Set(llx, lly, llz);
    ur.Set(urx, ury, urz);
    SanityCheck();
  }
};

template<typename T>
class Range {
 public:
  T begin;
  T end;

  Range() : begin(0), end(0) {}
  Range(T b, T e) : begin(b), end(e) {
    PhyDBExpects(b < e, "Illegal Range, " + Str());
  }
  std::string Str() {
    return "(" + std::to_string(begin) + ", " + std::to_string(end) + ")";
  }
};

class LayerRect {
 public:
  std::string layer_name_;
  std::vector<Rect2D<double>> rects_;

  LayerRect() : layer_name_("") {}
  explicit LayerRect(std::string &layer_name) : layer_name_(layer_name) {}
  LayerRect(
      const std::string &layer_name,
      const std::vector<Rect2D<double>> &rects
  ) :
      layer_name_(layer_name),
      rects_(rects) {}

  // API to add LayerRect
  void AddRect(double llx, double lly, double urx, double ury);
  std::vector<Rect2D<double>> &GetRects();
  Rect2D<double> GetBoundingBox();
  void Reset();
  void Report();
};

template<typename T>
class Points2D {
 public:
  void AddPoint(T x, T y) {
    points_.emplace_back(x, y);
  }
  Point2D<T> &GetPoint(int index) { return points_[index]; }
  std::vector<Point2D<T>> &GetPointsRef() { return points_; }
 private:
  std::vector<Point2D<T>> points_;
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const Point2D<T> &p) {
  os << "(" << p.x << ", " << p.y << ") ";
  return os;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Point3D<T> &p) {
  os << "(" << p.x << ", " << p.y << ", " << p.z << ") ";
  return os;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Rect2D<T> &p) {
  os << "Rect: " << p.ll << " " << p.ur << " ";
  return os;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Rect3D<T> &p) {
  os << "Rect: " << p.ll << " " << p.ur << " ";
  return os;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Range<T> &r) {
  os << "Range [" << r.begin << ", " << r.end << "] ";
  return os;
}

std::ostream &operator<<(std::ostream &os, const LayerRect &lr);

}

#endif //PHYDB_DATATYPES_H_


