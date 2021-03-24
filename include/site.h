#ifndef SITE_H
#define SITE_H

#include "header.h"

namespace phydb {

class Site {
  private:
    string name_;
    string class_name_;
    double width_;
    double height_;

  public:
    Site() : name_(""), class_name_(""), width_(0), height_(0) {}
    Site(string name, string className, double width, double height) :
        name_(name), class_name_(className), width_(width), height_(height) {}

    void SetName(string);
    void SetClassName(string);
    void SetWidth(double);
    void SetHeight(double);

    string GetName() const;
    string GetClassName() const;
    double GetWidth() const;
    double GetHeight() const;

};

ostream &operator<<(ostream &, const Site &);

}

#endif
