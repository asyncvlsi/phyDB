#ifndef ROW_H
#define ROW_H

#include "phydb_header.h"

namespace phydb {

class Row {
  public:
    string name_;
    string site_name_;
    string site_orient_;
    int orig_x_;
    int orig_y_;
    int num_x_;
    int num_y_;
    int step_x_;
    int step_y_;

    Row() {}
    Row(string name, string siteName, string siteOrient,
        int origX, int origY, int numX, int numY, int stepX, int stepY) :
        name_(name),
        site_name_(siteName),
        site_orient_(siteOrient),
        orig_x_(origX),
        orig_y_(origY),
        num_x_(numX),
        num_y_(numY),
        step_x_(stepX),
        step_y_(stepY) {}

};

ostream &operator<<(ostream &, const Row &);

}

#endif

