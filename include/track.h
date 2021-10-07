#ifndef PHYDB_INCLUDE_TRACK_H_
#define PHYDB_INCLUDE_TRACK_H_

#include "enumtypes.h"
#include "logging.h"

namespace phydb {

class Track {
  private:
    XYDirection direction_;
    int start_;
    int n_tracks_;
    int step_;

    std::vector<std::string> layer_names_;

  public:
    Track() {}
    Track(
        XYDirection direction,
        int start,
        int nTracks,
        int step,
        std::vector<std::string> &layerNames
    ) : direction_(direction),
        start_(start),
        n_tracks_(nTracks),
        step_(step),
        layer_names_(layerNames) {}

    XYDirection GetDirection() const;
    int GetStart() const;
    int GetNTracks() const;
    int GetStep() const;
    std::vector<std::string> &GetLayerNames();

    friend std::ostream &operator<<(std::ostream &, const Track &);
};

std::ostream &operator<<(std::ostream &, const Track &);

}

#endif //PHYDB_INCLUDE_TRACK_H_
