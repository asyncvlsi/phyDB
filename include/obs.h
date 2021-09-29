#ifndef PHYDB_INCLUDE_OBS_H_
#define PHYDB_INCLUDE_OBS_H_

#include "datatype.h"

namespace phydb {

class OBS {
  private:
    vector<LayerRect> layer_rects_;

  public:
    OBS() {}
    OBS(vector<LayerRect> layerRects) : layer_rects_(layerRects) {}

    void SetLayerRects(vector<LayerRect>);

    vector<LayerRect> GetLayerRects() const;
    vector<LayerRect>& GetLayerRectsRef();

    // API to add LayerRect
    LayerRect *AddLayerRect(std::string &layer_name);

    friend ostream &operator<<(ostream &, const OBS &);
};

ostream &operator<<(ostream &, const OBS &);

}

#endif //PHYDB_INCLUDE_OBS_H_

