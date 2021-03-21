#ifndef COMPONENT_H
#define COMPONENT_H

#include "header.h"
#include "DataType.h"

namespace phydb {

class Component {
  public:
    int id_;
    string name_;
    string macro_name_;
    string source_;
    string location_type_;
    string orient_;

    int weight_;
    Point2D<int> location_;

    Component() {}
    Component(string name, string macroName, string source, string locationType,
              string orient, int weight, Point2D<int> location) :
        name_(name),
        macro_name_(macroName),
        source_(source),
        location_type_(locationType),
        orient_(orient),
        weight_(weight),
        location_(location) {}
    Component(std::string &comp_name, std::string &macro_name, std::string &place_status,
              int llx, int lly, std::string &orient)
        : name_(comp_name), macro_name_(macro_name), location_type_(place_status), orient_(orient) {
        location_.x = llx;
        location_.y = lly;
    }

    string GetName();
    string GetMacroName();

};

ostream &operator<<(ostream &, const Component &);

}

#endif

