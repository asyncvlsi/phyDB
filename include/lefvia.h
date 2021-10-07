#ifndef PHYDB_INCLUDE_LEFVIA_H_
#define PHYDB_INCLUDE_LEFVIA_H_

#include <string>

#include "datatype.h"

namespace phydb {

class LefVia {
  private:
    std::string name_;
    bool is_default_;
    std::vector<LayerRect> layer_rects_;
  public:
    LefVia() : is_default_(false) {}
    explicit LefVia(std::string &name) : name_(name) {}

    std::string GetName() const;
    void SetDefault();
    void UnsetDefault();
    void SetLayerRect(
        const std::string &layer_name0,
        const std::vector<Rect2D<double>> &rect0,
        const std::string &layer_name1,
        const std::vector<Rect2D<double>> &rect1,
        const std::string &layer_name2,
        const std::vector<Rect2D<double>> &rect2
    );

    std::vector<LayerRect> &GetLayerRectsRef();

    void Report();
};

}

#endif //PHYDB_INCLUDE_LEFVIA_H_
