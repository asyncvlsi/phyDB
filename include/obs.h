#ifndef OBS_H
#define OBS_H

#include "DataType.h"

namespace phydb {

class OBS {
    friend class Macro;
  private:
    vector<LayerRect> layer_rects_;

  public:
    OBS() {}
    OBS(vector<LayerRect> layerRects) : layer_rects_(layerRects) {}

    void SetLayerRect(vector<LayerRect>);

    vector<LayerRect> GetLayerRect() const;

    friend ostream &operator<<(ostream &, const OBS &);
};

ostream &operator<<(ostream &, const OBS &);

}

#endif

