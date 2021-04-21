#ifndef LAYER_H
#define LAYER_H

#include "logging.h"
#include "spacingtable.h"
#include "eolspacing.h"
#include "cornerspacing.h"
#include "adjacentcutspacing.h"
#include "spacingtableinfluence.h"
#include "enumtypes.h"

namespace phydb {

class Layer {
  private:
    string name_;
    LayerType type_;
    int id_ = -1;

    //metal layer
    MetalDirection direction_;
    double pitchx_ = -1;
    double pitchy_ = -1;
    double width_ = -1;
    double area_ = 0;
    double min_width_ = -1;
    double offset_ = 0;

    SpacingTable spacing_table_;
    vector<SpacingTableInfluence> spacing_table_influences_;
    vector<EolSpacing> eol_spacings_;
    CornerSpacing corner_spacing_;

    //cut layer
    double spacing_ = -1;
    AdjacentCutSpacing adjacent_cut_spacing_;

  public:
    Layer(std::string &name, LayerType type, MetalDirection direction) :
        name_(name), type_(type), direction_(direction) {}

    //constructor for metal layer
    Layer(string name, LayerType type, MetalDirection direction, double pitchx, double pitchy,
          double width, double area, double minWidth, double offset) :
        name_(name),
        type_(type),
        id_(-1),
        direction_(direction),
        pitchx_(pitchx),
        pitchy_(pitchy),
        width_(width),
        area_(area),
        min_width_(minWidth),
        offset_(offset) {}

    //constructor for cut layer
    Layer(string name, LayerType type, double spacing) :
        name_(name),
        type_(type),
        id_(-1),
        spacing_(spacing) {}

    void SetName(string &name);
    void SetType(LayerType type);
    void SetID(int id);
    void SetDirection(MetalDirection direction);
    void SetWidth(double width);
    void SetMinWidth(double min_width);
    void SetPitch(double pitch_x, double pitch_y);
    void SetOffset(double offset);
    void SetArea(double area);
    void SetSpacing(double spacing);

    const std::string &GetName();
    int GetID() const;
    LayerType GetType() const;
    MetalDirection GetDirection() const;
    double GetWidth() const;
    double GetMinWidth() const;
    double GetPitchX() const;
    double GetPitchY() const;
    void GetPitch(double &pitch_x, double &pitch_y) const;
    double GetOffset() const;
    double GetArea() const;
    double GetSpacing() const;

    SpacingTable *SetSpacingTable(SpacingTable &);
    SpacingTable *SetSpacingTable(int n_col, int n_row, const vector<double> &v_parallel_run_length,
                                  const vector<double> &v_width, const vector<double> &v_spacing);
    SpacingTableInfluence *AddSpacingTableInfluence(double width, double within, double spacing);

    EolSpacing *AddEolSpacing(double spacing, double eol_width, double eol_within,
                              double par_edge, double par_within);
    CornerSpacing *SetCornerSpacing(CornerSpacing &);
    AdjacentCutSpacing *SetAdjCutSpacing(double spacing, int adjacent_cuts, int cut_within);

    SpacingTable *GetSpacingTable();
    vector<SpacingTableInfluence> *GetSpacingTableInfluences();
    vector<EolSpacing> *GetEolSpacings();
    CornerSpacing *GetCornerSpacing();
    AdjacentCutSpacing *GetAdjCutSpacing();

    friend ostream &operator<<(ostream &, const Layer &);

    void Report();
};

ostream &operator<<(ostream &, const Layer &);

class WellLayer {
  private:
    double width_;
    double spacing_;
    double op_spacing_;
    double max_plug_dist_;
    double overhang_;
  public:
    WellLayer(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
        SetWidth(width);
        SetSpacing(spacing);
        SetOpSpacing(op_spacing);
        SetMaxPlugDist(max_plug_dist);
        SetOverhang(overhang);
    }

    double GetWidth() const;
    double GetSpacing() const;
    double GetOpSpacing() const;
    double GetMaxPlugDist() const;
    double GetOverhang() const;

    void SetWidth(double width);
    void SetSpacing(double spacing);
    void SetOpSpacing(double op_spacing);
    void SetMaxPlugDist(double max_plug_dist);
    void SetOverhang(double overhang);
    void SetParams(double width, double spacing, double op_spacing, double max_plug_dist, double overhang);
    void Report() const;
};

}

#endif
