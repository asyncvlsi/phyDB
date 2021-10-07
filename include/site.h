#ifndef PHYDB_INCLUDE_SITE_H_
#define PHYDB_INCLUDE_SITE_H_

#include "logging.h"

namespace phydb {

class Site {
  private:
    std::string name_;
    std::string class_name_;
    double width_;
    double height_;

  public:
    Site() : name_(""), class_name_(""), width_(0), height_(0) {}
    Site(std::string name, std::string className, double width, double height) :
        name_(name), class_name_(className), width_(width), height_(height) {}

    void SetName(std::string);
    void SetClassName(std::string);
    void SetWidth(double);
    void SetHeight(double);

    std::string GetName() const;
    std::string GetClassName() const;
    double GetWidth() const;
    double GetHeight() const;

};

std::ostream &operator<<(std::ostream &, const Site &);

}

#endif //PHYDB_INCLUDE_SITE_H_
