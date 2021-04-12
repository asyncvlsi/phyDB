#include "snet.h"

namespace phydb {

void Path::SetLayerName(string& layer_name) {
    layer_name_ = layer_name;
}

void Path::SetWidth(int width) {
    width_ = width;
}

void Path::SetShape(string& shape) {
    shape_ = shape;
}

void Path::SetViaName(string& via_name) {
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
    end_.x = x;
    end_.y = y;
}

string Path::GetLayerName() const {
    return layer_name_;
}

int Path::GetWidth() const {
    return width_;
}

string Path::GetShape() const {
    return shape_;
}

string Path::GetViaName() const {
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

void Path::Report() {
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

void SNet::SetName(string& name) {
    name_ = name;
}

void SNet::SetUse(SignalUse use) {
    bool e = (use == GROUND || use == POWER);
    PhyDbExpects(e, "special net use should be POWER or GROUND");
    use_ = use;
}

Path* SNet::AddPath() {
    int id = paths_.size();
    paths_.emplace_back();
    return &paths_[id];
}

Path* SNet::AddPath(string& layer_name, int width, string shape) {
    int id = paths_.size();
    paths_.emplace_back(layer_name, width, shape);
    return &paths_[id];
}

string SNet::GetName() const {
    return name_;
}

SignalUse SNet::GetUse() const {
    return use_;
}

vector<Path>& SNet::GetPathRef() {
    return paths_;
}

void SNet::Report() {
    cout << "SNET: " << name_ << " use: " << use_ << endl;
    for(auto p : paths_) {
        p.Report();
    }
}


}