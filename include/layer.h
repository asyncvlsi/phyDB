#ifndef PHYDB_INCLUDE_LAYER_H_
#define PHYDB_INCLUDE_LAYER_H_

#include "adjacentcutspacing.h"
#include "cornerspacing.h"
#include "enumtypes.h"
#include "eolspacing.h"
#include "logging.h"
#include "spacingtable.h"
#include "spacingtableinfluence.h"

namespace phydb {

class Layer {
  private:
    string name_;
    LayerType type_;
    int id_ = -1;

    //metal layer
    MetalDirection direction_ = VERTICAL;
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

    // RC estimation
    // capacitance for each square unit, in picofarads per square micron. This is used to model wire-to-ground capacitance.
    double capacitance_cpersqdist_ = -1;
    // Specifies the multiplier for interconnect capacitance to account for increases in capacitance caused by nearby wires.
    double capmultiplier_ = 1;
    // segment capacitance = (layer capacitance per square x segment width x segment length) + (peripheral capacitance x 2 (segment width + segment length))
    double edgecapacitance_ = -1; // not used yet
    // Specifies the resistance for a square of wire, in ohms per square. The resistance of a wire can be defined as
    // RPERSQU x wire_length/wire_width
    double resistance_rpersq_ = -1;

  public:
    Layer(std::string &name, LayerType type, MetalDirection direction) :
        name_(name), type_(type), direction_(direction) {}

    //constructor for metal layer
    Layer(
        string &name,
        LayerType type,
        MetalDirection direction,
        double pitch_x,
        double pitch_y,
        double width,
        double area,
        double min_width,
        double offset
    ) :
        name_(name),
        type_(type),
        id_(-1),
        direction_(direction),
        pitchx_(pitch_x),
        pitchy_(pitch_y),
        width_(width),
        area_(area),
        min_width_(min_width),
        offset_(offset) {}

    Layer(
        string name,
        LayerType type,
        MetalDirection direction,
        double pitch_x,
        double pitch_y,
        double width,
        double area,
        double min_width,
        double offset
    ) :
        name_(std::move(name)),
        type_(type),
        id_(-1),
        direction_(direction),
        pitchx_(pitch_x),
        pitchy_(pitch_y),
        width_(width),
        area_(area),
        min_width_(min_width),
        offset_(offset) {}

    //constructor for cut layer
    Layer(string &name, LayerType type, double spacing) :
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
    void SetCPerSqDist(double cpersqdist);
    void SetCapMultiplier(double capmultiplier);
    void SetEdgeCPerDist(double edgecapacitance);
    void SetRPerSqUnit(double rpersq);

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
    double GetCPerSqDist() const;
    double GetCapMultiplier() const;
    double GetEdgeCPerDist() const;
    double GetRPerSqUnit() const;

    SpacingTable *SetSpacingTable(SpacingTable &);
    SpacingTable *SetSpacingTable(
        int n_col,
        int n_row,
        const vector<double> &v_parallel_run_length,
        const vector<double> &v_width,
        const vector<double> &v_spacing
    );
    SpacingTableInfluence *AddSpacingTableInfluence(
        double width,
        double within,
        double spacing
    );

    EolSpacing *AddEolSpacing(
        double spacing,
        double eol_width,
        double eol_within,
        double par_edge,
        double par_within
    );
    CornerSpacing *SetCornerSpacing(CornerSpacing &);
    AdjacentCutSpacing *SetAdjCutSpacing(
        double spacing,
        int adjacent_cuts,
        int cut_within
    );

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
    WellLayer(
        double width,
        double spacing,
        double op_spacing,
        double max_plug_dist,
        double overhang
    ) {
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
    void SetParams(
        double width,
        double spacing,
        double op_spacing,
        double max_plug_dist,
        double overhang
    );
    void Report() const;
};

}

#endif //PHYDB_INCLUDE_LAYER_H_
