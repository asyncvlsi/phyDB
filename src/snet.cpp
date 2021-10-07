#include "snet.h"

namespace phydb {

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

void Path::SetBeginExt(int begin_ext) {
    begin_ext_ = begin_ext;
}

void Path::SetEndExt(int end_ext) {
    end_ext_ = end_ext;
}

void Path::SetRect(int lx, int ly, int ux, int uy) {
    rect_ = Rect2D<int>(lx, ly, ux, uy);
}

void Path::SetRect(Rect2D<int> rect) {
    rect_ = rect;
}

void Path::SetBegin(int x, int y) {
    begin_.x = x;
    begin_.y = y;
}

void Path::SetEnd(int x, int y) {
    has_end_point_ = true;
    end_.x = x;
    end_.y = y;
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

int Path::GetBeginExt() const {
    return begin_ext_;
}

int Path::GetEndExt() const {
    return end_ext_;
}

Rect2D<int> Path::GetRect() const {
    return rect_;
}

Point2D<int> Path::GetBegin() const {
    return begin_;
}

Point2D<int> Path::GetEnd() const {
    return end_;
}

bool Path::HasEndPoint() const {
    return has_end_point_;
}

void Path::Report() {
    std::cout << " NEW " << layer_name_ << " " << width_ << " + SHAPE "
              << shape_;
    if (!rect_.IsEmpty())
        std::cout << " (" << rect_.ll.x << " " << rect_.ll.y << " "
                  << rect_.ur.x << " " << rect_.ur.y << ")";
    if (!begin_.IsEmpty())
        std::cout << " (" << begin_.x << " " << begin_.y << " ) ";
    if (!end_.IsEmpty())
        std::cout << " (" << end_.x << " " << end_.y << " ) ";
    if (begin_ext_ != 0 || end_ext_ != 0)
        std::cout << " EXT " << begin_ext_ << " " << end_ext_;
    if (!via_name_.empty())
        std::cout << via_name_;
    std::cout << "\n";
}

void SNet::SetName(std::string &name) {
    name_ = name;
}

void SNet::SetUse(SignalUse use) {
    bool e = (use == GROUND || use == POWER);
    PhyDBExpects(e, "special net use should be POWER or GROUND");
    use_ = use;
}

Path *SNet::AddPath() {
    int id = (int) paths_.size();
    paths_.emplace_back();
    return &paths_[id];
}

Path *SNet::AddPath(std::string &layer_name, int width, std::string shape) {
    int id = (int) paths_.size();
    paths_.emplace_back(layer_name, width, shape);
    return &paths_[id];
}

std::string SNet::GetName() const {
    return name_;
}

SignalUse SNet::GetUse() const {
    return use_;
}

std::vector<Path> &SNet::GetPathRef() {
    return paths_;
}

void SNet::Report() {
    std::cout << "SNET: " << name_ << " use: " << use_ << "\n";
    for (auto p: paths_) {
        p.Report();
    }
}

}