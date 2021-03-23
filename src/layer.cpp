#include "layer.h"

namespace phydb {

string Layer::GetName() const {
    return name_;
}

void Layer::SetName(string &name) {
    name_ = name;
}

void Layer::SetType(LayerType type) {
    type_ = type;
}

void Layer::SetDirection(MetalDirection direction) {
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

void Layer::SetSpacing(float spacing) {
    spacing_ = spacing;
}

SpacingTable *Layer::SetSpacingTable(SpacingTable &st) {
    spacing_table_ = st;
    return &spacing_table_;
}

SpacingTable *Layer::SetSpacingTable(int n_col, int n_row, const vector<float> &v_parallel_run_length,
                                     const vector<float> &v_width, const vector<float> &v_spacing) {
    spacing_table_ = SpacingTable(n_col, n_row, v_parallel_run_length, v_width, v_spacing);
    return &spacing_table_;
}

SpacingTableInfluence *Layer::AddSpacingTableInfluence(float width, float within, float spacing) {
    spacing_table_influences_.emplace_back(width, within, spacing);
    return &spacing_table_influences_.back();
}

EolSpacing *Layer::AddEolSpacing(float spacing, float width, float within, float par_edge, float par_within) {
    eol_spacings_.emplace_back(spacing, width, within, par_edge, par_within);
    return &eol_spacings_.back();
}

CornerSpacing *Layer::SetCornerSpacing(CornerSpacing &cornerSpacing) {
    corner_spacing_ = cornerSpacing;
    return &corner_spacing_;
}

AdjacentCutSpacing *Layer::SetAdjCutSpacing(float spacing, int adjacent_cuts, int cut_within) {
    adjacent_cut_spacing_ = AdjacentCutSpacing(spacing, adjacent_cuts, cut_within);
    return &adjacent_cut_spacing_;
}

float Layer::GetSpacing() const {
    return spacing_;
}

SpacingTable *Layer::GetSpacingTable() {
    return &spacing_table_;
}

vector<EolSpacing> *Layer::GetEolSpacings() {
    return &eol_spacings_;
}

vector<SpacingTableInfluence> *Layer::GetSpacingTableInfluences() {
    return &spacing_table_influences_;
}

CornerSpacing *Layer::GetCornerSpacing() {
    return &corner_spacing_;
}

AdjacentCutSpacing *Layer::GetAdjCutSpacing() {
    return &adjacent_cut_spacing_;
}

ostream &operator<<(ostream &os, const Layer &l) {
    os << l.name_ << " " << LayerTypeStr(l.type_) << " "
       << l.idx_ << " " << MetalDirectionStr(l.direction_) << endl;
    os << l.pitchx_ << " " << l.pitchy_ << " " << l.width_ << " " << l.area_ << endl;
    os << l.min_width_ << " " << l.offset_ << endl;
    os << l.spacing_ << endl;
    return os;
}

void Layer::Report() {
    cout << "------------------------------" << endl;
    cout << "Layer: " << name_ << " type: " << LayerTypeStr(type_)
         << " direction: " << MetalDirectionStr(direction_) << " idx_: " << idx_ << endl;
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

void WellLayer::Report() const {
    std::cout << "    MINWIDTH:    " << width_ << " um\n"
              << "    SPACING:     " << spacing_ << " um\n"
              << "    OPPOSPACING: " << op_spacing_ << " um\n"
              << "    MAXPLUGDIST: " << max_plug_dist_ << " um\n"
              << "    OVERHANG:    " << overhang_ << " um\n";
}

}
