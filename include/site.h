#ifndef SITE_H
#define SITE_H

#include "header.h"

namespace phydb {

class Site {
  private:
    string name_;
    string class_name_;
    float width_;
    float height_;

  public:
    Site() : name_(""), class_name_(""), width_(0), height_(0) {}
    Site(string name, string className, float width, float height) :
        name_(name), class_name_(className), width_(width), height_(height) {}

    void SetName(string);
    void SetClassName(string);
    void SetWidth(float);
    void SetHeight(float);

    string GetName() const;
    string GetClassName() const;
    float GetWidth() const;
    float GetHeight() const;

};

ostream &operator<<(ostream &, const Site &);

}

#endif
