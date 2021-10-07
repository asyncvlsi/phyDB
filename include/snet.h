#ifndef PHYDB_INCLUDE_SNET_H_
#define PHYDB_INCLUDE_SNET_H_

#include "datatype.h"
#include "enumtypes.h"
#include "logging.h"

namespace phydb {

class Path {
  private:
    std::string layer_name_;
    int width_;
    std::string shape_;
    std::string via_name_;

    int begin_ext_;
    int end_ext_;
    Rect2D<int> rect_;
    Point2D<int> begin_;
    Point2D<int> end_;
    bool has_end_point_ = false;
  public:
    Path() : width_(0), begin_ext_(0), end_ext_(0), has_end_point_(false) {}
    Path(std::string &layer_name, int width, std::string &shape) :
        layer_name_(layer_name),
        width_(width),
        shape_(shape),
        begin_ext_(0),
        end_ext_(0) {}

    void SetLayerName(std::string &);
    void SetWidth(int);
    void SetShape(std::string &);
    void SetViaName(std::string &);
    void SetBeginExt(int);
    void SetEndExt(int);
    void SetRect(int lx, int ly, int ux, int uy);
    void SetRect(Rect2D<int> rect);
    void SetBegin(int x, int y);
    void SetEnd(int x, int y);

    std::string GetLayerName() const;
    int GetWidth() const;
    std::string GetShape() const;
    std::string GetViaName() const;
    int GetBeginExt() const;
    int GetEndExt() const;
    Rect2D<int> GetRect() const;
    Point2D<int> GetBegin() const;
    Point2D<int> GetEnd() const;
    bool HasEndPoint() const;

    void Report();
};

class SNet {
  private:
    std::string name_;
    SignalUse use_; // POWER or GROUND
    std::vector<Path> paths_;
  public:
    SNet() {}
    SNet(std::string &name) : name_(name) {}
    SNet(std::string &name, SignalUse use) : name_(name), use_(use) {}

    void SetName(std::string &);
    void SetUse(SignalUse);
    Path *AddPath();
    Path *AddPath(std::string &layer_name, int width, std::string shape);

    std::string GetName() const;
    SignalUse GetUse() const;
    std::vector<Path> &GetPathRef();

    void Report();
};

}

#endif //PHYDB_INCLUDE_SNET_H_
