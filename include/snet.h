#ifndef SNET_H
#define SNET_H

#include "header.h"

namespace phydb {

class Path {
  private:
    string layer_name_;
    int width_;
    string shape_;
    string via_name_;

    int begin_ext_;
    int end_ext_;
    Rect2D<int> rect_;
    Point2D<int> begin_;
    Point2D<int> end_;
  public:
    Path() {
        layer_name_ = "";
        width_ = 0;
        shape_ = "";
        via_name_ = "";
        begin_ext_ = 0;
        end_ext_ = 0;
    }
    void Report() {
        cout << " NEW " << layer_name_ << " " << width_ << " + SHAPE " << shape_;
        if (!rect_.IsEmpty())
            cout << " (" << rect_.ll.x << " " << rect_.ll.y << " " << rect_.ur.x << " " << rect_.ur.y << ")";
        if (!begin_.IsEmpty())
            cout << " (" << begin_.x << " " << begin_.y << " ) ";
        if (!end_.IsEmpty())
            cout << " (" << end_.x << " " << end_.y << " ) ";
        if (begin_ext_ != 0 || end_ext_ != 0)
            cout << " EXT " << begin_ext_ << " " << end_ext_;
        if (via_name_ != "")
            cout << via_name_;
        cout << endl;
    }
};

class SNet {
  private:
    string name_;
    vector<Path> paths_;
  public:
    SNet() {}

    void SetName(string);

    string GetName() const;
};

ostream &operator<<(ostream &, const SNet &);

}

#endif
