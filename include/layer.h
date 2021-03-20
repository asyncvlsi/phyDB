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

    void print();
};

ostream &operator<<(ostream &, const Layer &);

}

#endif
