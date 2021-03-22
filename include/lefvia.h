#ifndef LEFVIA_H
#define LEFVIA_H

#include <string>

#include "DataType.h"

namespace phydb {

class LefVia {
  private:
    string name_;
    bool is_default_;
    std::vector<LayerRect> layer_rects_;
  public:
    LefVia() {}
    explicit LefVia(std::string &name) : name_(name) {}

    void Report();
};

}

#endif
