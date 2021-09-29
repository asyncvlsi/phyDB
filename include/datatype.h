#ifndef PHYDB_INCLUDE_DATATYPES_H_
#define PHYDB_INCLUDE_DATATYPES_H_

#include <string>

#include "logging.h"

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
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
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
    Rect2D(T llx, T lly, T urx, T ury) : ll(llx, lly), ur(urx, ury) { SanityCheck(); }

    bool IsLegal() { return (ll.x < ur.x && ll.y < ur.y); }
    void SanityCheck() { PhyDBExpects(this->IsLegal(), "Illegal Rect2D: ll, ur, " + ll.Str() + ur.Str()); }
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

    string Str() {
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
    string layer;
    Rect2DLayer() : Rect2D<T>() {}
    void Set(string &layer_init, T llx, T lly, T urx, T ury) {
        this->layer = layer_init;
        this->lowerLeft.Set(llx, lly);
        this->upperRight.Set(urx, ury);
    }
};

template<typename T>
class Rect3D {
  public:
    Point3D<T> ll;
    Point3D<T> ur;

    Rect3D() : ll(0, 0, 0), ur(0, 0, 0) {}
    Rect3D(Point3D<T> LL, Point3D<T> UR) : ll(LL), ur(UR) {}
    Rect3D(T llx, T lly, T llz, T urx, T ury, T urz) : ll(llx, lly, llz), ur(urx, ury, urz) {}

    bool IsLegal() { //on the same layer is legal
        return (ll.x < ur.x && ll.y < ur.y && ll.z <= ur.z);
    }
    void SanityCheck() { PhyDBExpects(this->IsLegal(), "Illegal Rect2D: ll, ur, " + ll.Str() + ur.Str()); }

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
    string layer_name_;
    vector<Rect2D<double>> rects_;

    LayerRect() : layer_name_("") {}
    explicit LayerRect(std::string &layer_name) : layer_name_(layer_name) {}
    LayerRect(const string &layerName, const vector<Rect2D<double>> &rects) :
        layer_name_(layerName),
        rects_(rects) {}

    // API to add LayerRect
    void AddRect(double llx, double lly, double urx, double ury) {
        rects_.emplace_back(llx, lly, urx, ury);
    }

    vector<Rect2D<double>> &GetRects() { return rects_;}

    void Reset() {
        layer_name_ = "";
        rects_.clear();
    }
    void Report() {
        std::cout << "Name: " << layer_name_ << "\n";
        for (auto &rect_2d: rects_) {
            std::cout << "  " << rect_2d.ll.Str() << " " << rect_2d.ur.Str() << "\n";
        }
    }
};

template<typename T>
ostream &operator<<(ostream &os, const Point2D<T> &p) {
    os << "(" << p.x << ", " << p.y << ") ";
    return os;
}

template<typename T>
ostream &operator<<(ostream &os, const Point3D<T> &p) {
    os << "(" << p.x << ", " << p.y << ", " << p.z << ") ";
    return os;
}

template<typename T>
ostream &operator<<(ostream &os, const Rect2D<T> &p) {
    os << "Rect: " << p.ll << " " << p.ur << " ";
    return os;
}

template<typename T>
ostream &operator<<(ostream &os, const Rect3D<T> &p) {
    os << "Rect: " << p.ll << " " << p.ur << " ";
    return os;
}

template<typename T>
ostream &operator<<(ostream &os, const Range<T> &r) {
    os << "Range [" << r.begin << ", " << r.end << "] ";
    return os;
}

ostream &operator<<(ostream &os, const LayerRect &lr);

}

#endif //PHYDB_INCLUDE_DATATYPES_H_


