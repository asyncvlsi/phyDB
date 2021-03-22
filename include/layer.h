#ifndef LAYER_H
#define LAYER_H

#include "header.h"
#include "spacingtable.h"
#include "eolspacing.h"
#include "cornerspacing.h"

namespace phydb {

class Layer {
  private:
    string name_;
    string type_;
    int idx_;

    //metal layer
    string direction_;
    float pitchx_;
    float pitchy_;
    float width_;
    float area_;
    float min_width_;
    float offset_;

    SpacingTable spacing_table_;
    EOLSpacing eol_spacing_;
    CornerSpacing corner_spacing_;

    //cut layer
    float spacing_;

  public:
    Layer() : name_(""), type_(""), idx_(-1), direction_(""), pitchx_(0),
              pitchy_(0), width_(0), area_(0), min_width_(0), offset_(0), spacing_(0) {}
    explicit Layer(std::string &name) : name_(name) {}

    //constructor for metal layer
    Layer(string name, string type, string direction, float pitchx, float pitchy,
          float width, float area, float minWidth, float offset) :
        name_(name),
        type_(type),
        idx_(-1),
        direction_(direction),
        pitchx_(pitchx),
        pitchy_(pitchy),
        width_(width),
        area_(area),
        min_width_(minWidth),
        offset_(offset) {}

    //constructor for cut layer
    Layer(string name, string type, float spacing) :
        name_(name),
        type_(type),
        idx_(-1),
        spacing_(spacing) {}

    string GetName() const;

    void SetName(string &name);
    void SetType(string &type);
    void SetDirection(string &direction);
    void SetWidth(float width);
    void SetMinWidth(float min_width);
    void SetPitch(float pitch_x, float pitch_y);
    void SetOffset(float offset);
    void SetArea(float area);

    void SetSpacingTable(SpacingTable &);
    void SetEOLSpacing(EOLSpacing &);
    void SetCornerSpacing(CornerSpacing &);

    SpacingTable GetSpacingTable();
    EOLSpacing GetEOLSpacing();
    CornerSpacing GetCornerSpacing();

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
    void Report();
};

}

#endif
