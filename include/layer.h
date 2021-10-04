#ifndef PHYDB_INCLUDE_LAYER_H_
#define PHYDB_INCLUDE_LAYER_H_

#include <string>
#include <vector>

#include "adjacentcutspacing.h"
#include "cornerspacing.h"
#include "enumtypes.h"
#include "eolspacing.h"
#include "logging.h"
#include "spacingtable.h"
#include "spacingtableinfluence.h"
#include "techconfig.h"

namespace phydb {

class LayerTechConfigCorner {
  public:
    explicit LayerTechConfigCorner(int model_index)
        : model_index_(model_index) {}

    void AddResOverTable(ResOverTable *res_over_table);
    void AddCapOverTable(CapOverTable *cap_over_table);
    void AddCapUnderTable(CapUnderTable *cap_under_table);
    void AddCapDiagUnderTable(CapDiagUnderTable *cap_diag_under_table);
    void AddCapOverUnderTable(CapOverUnderTable *cap_over_under_table);

    /**** getters for raw data ****/
    std::vector<ResOverTable> &GetResOverRef();
    std::vector<CapOverTable> &GetCapOverRef();
    std::vector<CapUnderTable> &GetCapUnderRef();
    std::vector<CapDiagUnderTable> &GetCapDiagUnderRef();
    std::vector<CapOverUnderTable> &GetCapOverUnderRef();

    int ModelIndex() const;
    void FixResOverTableLastEntry();
    double GetOverSubstrateNoSurroundingWireRes();
    double GetOverSubstrateNoSurroundingWireCap();
    void Report();

  private:
    int model_index_;
    /**** raw data from technology configuration file ****/
    std::vector<ResOverTable> res_over_;
    std::vector<CapOverTable> cap_over_;
    std::vector<CapUnderTable> cap_under_;
    std::vector<CapDiagUnderTable> cap_diagunder_;
    std::vector<CapOverUnderTable> cap_overunder_;
};

class LayerTechConfig {
  private:
    std::vector<LayerTechConfigCorner> corners_;
  public:
    void AddCorner(int corner_index);
    std::vector<LayerTechConfigCorner> &CornersRef();
    LayerTechConfigCorner *GetLastCorner();
    void FixResOverTable();

    void Report();
};

class Layer {
    friend class Tech;
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

    ~Layer();

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

    void InitLayerTechConfig();
    LayerTechConfig *GetLayerTechConfig();
    void AddTechConfigCorner(int corner_index);
    void SetResistanceUnitFromTechConfig();
    void SetResistanceUnitFromLef();
    void SetCapacitanceUnitFromTechConfig();
    void SetCapacitanceUnitFromLef();
    double GetResistance(
        double width,
        double length,
        int corner_index
    );
    double GetAreaCapacitance(
        double width,
        double length,
        int corner_index
    );
    double GetFringeCapacitance(
        double width,
        double length,
        int corner_index
    );

    friend ostream &operator<<(ostream &, const Layer &);

    void Report();

  private:
    std::string name_;
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

    /**** RC estimation (multiple corners) ****/
    std::vector<double> unit_area_cap_;
    std::vector<double> unit_edge_cap_;
    std::vector<double> unit_res_;
    /**** Part 1. parameters from LEF (only one corner?) ****/
    // capacitance for each square unit, in picofarads per square micron. This is used to model wire-to-ground capacitance.
    double capacitance_cpersqdist_ = -1;
    // Specifies the multiplier for interconnect capacitance to account for increases in capacitance caused by nearby wires.
    double capmultiplier_ = 1;
    // segment capacitance = (layer_capacitance_per_square x segment_width x segment_length) + (peripheral_capacitance x 2 (segment_width + segment_length))
    double edgecapacitance_ = -1;
    // Specifies the resistance for a square of wire, in ohms per square. The resistance of a wire can be defined as
    // RPERSQU x wire_length/wire_width
    double resistance_rpersq_ = -1;
    /**** Part 2. parameters from OpenRCX technology configuration file (multiple corners) ****/
    LayerTechConfig *layer_tech_config_ = nullptr;
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
    ) :
        width_(width),
        spacing_(spacing),
        op_spacing_(op_spacing),
        max_plug_dist_(max_plug_dist),
        overhang_(overhang) {}

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
