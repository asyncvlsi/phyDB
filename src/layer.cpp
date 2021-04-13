#include "layer.h"

namespace phydb {

void Layer::SetName(string &name) {
    name_ = name;
}

void Layer::SetType(LayerType type) {
    type_ = type;
}

void Layer::SetID(int id) {
    id_ = id;
}

void Layer::SetDirection(MetalDirection direction) {
    direction_ = direction;
}

void Layer::SetWidth(double width) {
    width_ = width;
}

void Layer::SetMinWidth(double min_width) {
    min_width_ = min_width;
}

void Layer::SetPitch(double pitch_x, double pitch_y) {
    pitchx_ = pitch_x;
    pitchy_ = pitch_y;
}

void Layer::SetOffset(double offset) {
    offset_ = offset;
}

void Layer::SetArea(double area) {
    area_ = area;
}

void Layer::SetSpacing(double spacing) {
    spacing_ = spacing;
}

const std::string &Layer::GetName() {
    return name_;
}

int Layer::GetID() const {
    return id_;
}

LayerType Layer::GetType() const {
    return type_;
}

MetalDirection Layer::GetDirection() const {
    return direction_;
}

double Layer::GetWidth() const {
    return width_;
}

double Layer::GetMinWidth() const {
    return min_width_;
}

void Layer::GetPitch(double &pitch_x, double &pitch_y) const {
    pitch_x = pitchx_;
    pitch_y = pitchy_;
}

double Layer::GetPitchX() const {
    return pitchx_;
}

double Layer::GetPitchY() const {
    return pitchy_;

}

double Layer::GetOffset() const {
    return offset_;
}

double Layer::GetArea() const {
    return area_;
}

double Layer::GetSpacing() const {
    return spacing_;
}

SpacingTable *Layer::SetSpacingTable(SpacingTable &st) {
    spacing_table_ = st;
    return &spacing_table_;
}

SpacingTable *Layer::SetSpacingTable(int n_col, int n_row, const vector<double> &v_parallel_run_length,
                                     const vector<double> &v_width, const vector<double> &v_spacing) {
    spacing_table_ = SpacingTable(n_col, n_row, v_parallel_run_length, v_width, v_spacing);
    return &spacing_table_;
}

SpacingTableInfluence *Layer::AddSpacingTableInfluence(double width, double within, double spacing) {
    spacing_table_influences_.emplace_back(width, within, spacing);
    return &spacing_table_influences_.back();
}

EolSpacing *Layer::AddEolSpacing(double spacing, double width, double within, double par_edge, double par_within) {
    eol_spacings_.emplace_back(spacing, width, within, par_edge, par_within);
    return &eol_spacings_.back();
}

CornerSpacing *Layer::SetCornerSpacing(CornerSpacing &cornerSpacing) {
    corner_spacing_ = cornerSpacing;
    return &corner_spacing_;
}

AdjacentCutSpacing *Layer::SetAdjCutSpacing(double spacing, int adjacent_cuts, int cut_within) {
    adjacent_cut_spacing_ = AdjacentCutSpacing(spacing, adjacent_cuts, cut_within);
    return &adjacent_cut_spacing_;
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
       << l.id_ << " " << MetalDirectionStr(l.direction_) << endl;
    os << l.pitchx_ << " " << l.pitchy_ << " " << l.width_ << " " << l.area_ << endl;
    os << l.min_width_ << " " << l.offset_ << endl;
    os << l.spacing_ << endl;
    return os;
}

void Layer::Report() {
    cout << "------------------------------" << endl;
    cout << "Layer: " << name_ << " type: " << LayerTypeStr(type_)
         << " direction: " << MetalDirectionStr(direction_) << " idx_: " << id_ << endl;
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
