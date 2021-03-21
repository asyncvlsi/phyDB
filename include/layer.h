#ifndef LAYER_H
#define LAYER_H

#include "header.h"
#include "spacingtable.h"
#include "eolspacing.h"
#include "cornerspacing.h"

namespace phydb {

class Layer {
  private:
    string _name;
    string _type;
    int _idx;

    //metal layer
    string _direction;
    float _pitchx;
    float _pitchy;
    float _width;
    float _area;
    float _minWidth;
    float _offset;
    float _minLength; //derived by minarea/width

    SpacingTable _spacingTable;
    EOLSpacing _eolSpacing;
    CornerSpacing _cornerSpacing;

    //cut layer
    float _spacing;

  public:
    Layer() : _name(""), _type(""), _idx(-1), _direction(""), _pitchx(0),
              _pitchy(0), _width(0), _area(0), _minWidth(0), _offset(0), _spacing(0) {}
    explicit Layer(std::string &name) : _name(name) {}

    //constructor for metal layer
    Layer(string name, string type, string direction, float pitchx, float pitchy,
          float width, float area, float minWidth, float offset) :
        _name(name),
        _type(type),
        _idx(-1),
        _direction(direction),
        _pitchx(pitchx),
        _pitchy(pitchy),
        _width(width),
        _area(area),
        _minWidth(minWidth),
        _offset(offset) {}

    //constructor for cut layer
    Layer(string name, string type, float spacing) :
        _name(name),
        _type(type),
        _idx(-1),
        _spacing(spacing) {}

    string getName() const { return _name; }

    void setName(string &name) { _name = name; }
    void setType(string &type) { _type = type; }
    void setDirection(string direction) { _direction = direction; }
    void setWidth(float width) { _width = width; }
    void setMinWidth(float min_width) { _minWidth = min_width; }
    void setPitch(float pitch_x, float pitch_y) {
        _pitchx = pitch_x;
        _pitchy = pitch_y;
    }
    void setOffset(float offset) { _offset = offset; }
    void setArea(float area) { _area = area; }

    void setSpacingTable(SpacingTable &);
    void setEOLSpacing(EOLSpacing &);
    void setCornerSpacing(CornerSpacing &);

    SpacingTable getSpacingTable();
    EOLSpacing getEOLSpacing();
    CornerSpacing getCornerSpacing();

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
        set_width(width);
        set_spacing(spacing);
        set_op_spacing(op_spacing);
        set_max_plug_dist(max_plug_dist);
        set_overhang(overhang);
    }

    double width() const { return width_; }
    double spacing() const { return spacing_; }
    double op_spacing() const { return op_spacing_; }
    double max_plug_dist() const { return max_plug_dist_; }
    double overhang() const { return overhang_; }

    void set_width(double width) {
        if (width < 0) {
            std::cout << "Negative width not allowed: WellLayer::set_width()" << std::endl;
            exit(1);
        }
        width_ = width;
    }
    void set_spacing(double spacing) {
        if (spacing < 0) {
            std::cout << "Negative spacing not allowed: WellLayer::set_spacing()" << std::endl;
            exit(1);
        }
        spacing_ = spacing;
    }
    void set_op_spacing(double op_spacing) {
        if (op_spacing < 0) {
            std::cout << "Negative opposite spacing not allowed: Layer::set_op_spacing()" << std::endl;
            exit(1);
        }
        op_spacing_ = op_spacing;
    }
    void set_max_plug_dist(double max_plug_dist) {
        if (max_plug_dist < 0) {
            std::cout << "Negative max plug distance not allowed: WellLayer::set_max_plug_dist()" << std::endl;
            exit(1);
        }
        max_plug_dist_ = max_plug_dist;
    }
    void set_overhang(double overhang) {
        if (overhang < 0) {
            std::cout << "Negative well/diffusion overhang not allowed: WellLayer::set_overhang()" << std::endl;
            exit(1);
        }
        overhang_ = overhang;
    }
    void set_params(double width, double spacing, double op_spacing, double max_plug_dist, double overhang) {
        set_width(width);
        set_spacing(spacing);
        set_op_spacing(op_spacing);
        set_max_plug_dist(max_plug_dist);
        set_overhang(overhang);
    }
    void Report();
};

}

#endif
