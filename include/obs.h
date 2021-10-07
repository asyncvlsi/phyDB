#ifndef PHYDB_INCLUDE_OBS_H_
#define PHYDB_INCLUDE_OBS_H_

#include "datatype.h"

namespace phydb {

class OBS {
  private:
    std::vector<LayerRect> layer_rects_;

  public:
    OBS() {}
    OBS(std::vector<LayerRect> layerRects) : layer_rects_(layerRects) {}

    void SetLayerRects(std::vector<LayerRect>);

    std::vector<LayerRect> GetLayerRects() const;
    std::vector<LayerRect> &GetLayerRectsRef();

    // API to add LayerRect
    LayerRect *AddLayerRect(std::string &layer_name);

    friend std::ostream &operator<<(std::ostream &, const OBS &);
};

std::ostream &operator<<(std::ostream &, const OBS &);

}

#endif //PHYDB_INCLUDE_OBS_H_

