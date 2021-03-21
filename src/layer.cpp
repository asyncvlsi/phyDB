#include "layer.h"

namespace phydb {

string Layer::GetName() const {
    return name_;
}

void Layer::SetName(string &name) {
    name_ = name;
}

void Layer::SetType(string &type) {
    type_ = type;
}

void Layer::SetDirection(string &direction) {
    direction_ = direction;
}

void Layer::SetWidth(float width) {
    width_ = width;
}

void Layer::SetMinWidth(float min_width) {
    min_width_ = min_width;
}

void Layer::SetPitch(float pitch_x, float pitch_y) {
    pitchx_ = pitch_x;
    pitchy_ = pitch_y;
}

void Layer::SetOffset(float offset) {
    offset_ = offset;
}

void Layer::SetArea(float area) {
    area_ = area;
}

void Layer::SetSpacingTable(SpacingTable &st) {
    spacing_table_ = st;
}

void Layer::SetEOLSpacing(EOLSpacing &eolSpacing) {
    eol_spacing_ = eolSpacing;
}

void Layer::SetCornerSpacing(CornerSpacing &cornerSpacing) {
    corner_spacing_ = cornerSpacing;
}

SpacingTable Layer::GetSpacingTable() {
    return spacing_table_;
}

EOLSpacing Layer::GetEOLSpacing() {
    return eol_spacing_;
}

CornerSpacing Layer::GetCornerSpacing() {
    return corner_spacing_;
}

ostream &operator<<(ostream &os, const Layer &l) {
    os << l.name_ << " " << l.type_ << " " << l.idx_ << " " << l.direction_ << endl;
    os << l.pitchx_ << " " << l.pitchy_ << " " << l.width_ << " " << l.area_ << endl;
    os << l.min_width_ << " " << l.offset_ << endl;
    os << l.spacing_ << endl;
    return os;
}

void Layer::Report() {
    cout << "------------------------------" << endl;
    cout << "Layer: " << name_ << " type: " << type_ << " direction: " << direction_ << " idx_: " << idx_ << endl;
    cout << "pitch: " << pitchx_ << " " << pitchy_ << " GetWidth:" << width_ << " area: " << area_ << endl;
    cout << "minWidth: " << min_width_ << " offset: " << offset_ << " GetSpacing: " << spacing_ << endl;
    /*
    if (spacingTable.parallelRunLength.size())
        spacingTable.print();
    for (auto spacing : spacings) {
        GetSpacing.print();
    }
    if (cornerSpacing.GetWidth.size())
        cornerSpacing.print();
    */
}

double WellLayer::GetWidth() const {
    return width_;
}

double WellLayer::GetSpacing() const {
    return spacing_;
}

double WellLayer::GetOpSpacing() const {
    return op_spacing_;
}

double WellLayer::GetMaxPlugDist() const {
    return max_plug_dist_;
}

double WellLayer::GetOverhang() const {
    return overhang_;
}

void WellLayer::SetWidth(double width) {
    PhyDbExpects(width >= 0, "Negative GetWidth not allowed: WellLayer::SetWidth()");
    width_ = width;
}

void WellLayer::SetSpacing(double spacing) {
    PhyDbExpects(spacing >= 0, "Negative GetSpacing not allowed: WellLayer::SetSpacing()");
    spacing_ = spacing;
}

void WellLayer::SetOpSpacing(double op_spacing) {
    PhyDbExpects(op_spacing >= 0, "Negative opposite GetSpacing not allowed: Layer::SetOpSpacing()");
    op_spacing_ = op_spacing;
}

void WellLayer::SetMaxPlugDist(double max_plug_dist) {
    PhyDbExpects(max_plug_dist >= 0, "Negative max plug distance not allowed: WellLayer::SetMaxPlugDist()");
    max_plug_dist_ = max_plug_dist;
}

void WellLayer::SetOverhang(double overhang) {
    PhyDbExpects(overhang >= 0, "Negative well/diffusion GetOverhang not allowed: WellLayer::SetOverhang()");
    overhang_ = overhang;
}

void WellLayer::SetParams(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
    SetWidth(width);
    SetSpacing(spacing);
    SetOpSpacing(op_spacing);
    SetMaxPlugDist(max_plug_dist);
    SetOverhang(overhang);
}

void WellLayer::Report() {
    std::cout << "    GetWidth:       " << GetWidth() << " um\n"
              << "    GetSpacing:     " << GetSpacing() << " um\n"
              << "    GetOpSpacing:   " << GetOpSpacing() << " um\n"
              << "    GetMaxPlugDist: " << GetMaxPlugDist() << " um\n"
              << "    GetOverhang:    " << GetOverhang() << "um\n";
}

}
