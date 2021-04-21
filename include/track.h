#ifndef TRACK_H
#define TRACK_H

#include "logging.h"
#include "enumtypes.h"

namespace phydb {

class Track {
  private:
    XYDirection direction_;
    int start_;
    int n_tracks_;
    int step_;

    vector<string> layer_names_;

  public:
    Track() {}
    Track(XYDirection direction, int start, int nTracks, int step, vector<string> &layerNames) :
        direction_(direction),
        start_(start),
        n_tracks_(nTracks),
        step_(step),
        layer_names_(layerNames) {}

    XYDirection GetDirection() const;
    int GetStart() const;
    int GetNTracks() const;
    int GetStep() const;
    vector<string>& GetLayerNames();

    friend ostream &operator<<(ostream &, const Track &);
};

ostream &operator<<(ostream &, const Track &);

}

#endif

