#ifndef SNET_H
#define SNET_H

#include "phydb_header.h"
#include "DataType.h"
#include "enumtypes.h"

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
    Path() : width_(0), begin_ext_(0), end_ext_(0) { }
    Path(string& layer_name, int width, string& shape) :
        layer_name_(layer_name), width_(width), shape_(shape), begin_ext_(0), end_ext_(0) { }
    
    void SetLayerName(string&);
    void SetWidth(int );
    void SetShape(string& );
    void SetViaName(string& );
    void SetBeginExt(int );
    void SetEndExt(int );
    void SetRect(int lx, int ly, int ux, int uy);
    void SetRect(Rect2D<int> rect);
    void SetBegin(int x, int y);
    void SetEnd(int x, int y);

    string GetLayerName() const;
    int GetWidth() const;
    string GetShape() const;
    string GetViaName() const;
    int GetBeginExt() const;
    int GetEndExt() const;
    Rect2D<int> GetRect() const;
    Point2D<int> GetBegin() const;
    Point2D<int> GetEnd() const;

    void Report();
};

class SNet {
  private:
    string name_;
    SignalUse use_; // "POWER" or "GROUND"
    vector<Path> paths_;
  public:
    SNet() {}
    SNet(string& name) : name_(name) {}
    SNet(string& name, SignalUse use) : name_(name), use_(use) {}

    void SetName(string&);
    void SetUse(SignalUse);
    Path* AddPath();
    Path* AddPath(string& layer_name, int width, string shape);

    string GetName() const;
    SignalUse GetUse() const;
    vector<Path>& GetPathRef();

    void Report();
};



}

#endif
