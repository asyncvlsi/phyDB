#ifndef PHYDB_INCLUDE_SPECIALMACRORECTLAYOUT_H_
#define PHYDB_INCLUDE_SPECIALMACRORECTLAYOUT_H_

#include <vector>

#include "datatype.h"
#include "macro.h"

namespace phydb {

struct RectSignalLayer {
  private:
    std::string signal_name_;
    std::string layer_name_;
    Rect2D<int> rect_;
  public:
    RectSignalLayer(
        std::string &signal_name,
        std::string &layer_name,
        int llx,
        int lly,
        int urx,
        int ury
    );
    std::string &SignalName();
    std::string &LayerName();
    Rect2D<int> &Rect();

    friend ostream &operator<<(
        ostream &os,
        const RectSignalLayer &rect_signal_layer
    );
};

struct SpecialMacroRectLayout {
  private:
    Macro *macro_ptr_;
    Rect2D<int> bbox_;
    std::vector<RectSignalLayer> rects_;
  public:
    explicit SpecialMacroRectLayout(
        Macro *macro_ptr,
        int llx,
        int lly,
        int urx,
        int ury
    );
    void SetBoundingBox(
        int llx,
        int lly,
        int urx,
        int ury
    );
    void AddRectSignalLayer(
        std::string &signal_name,
        std::string &layer_name,
        int llx,
        int lly,
        int urx,
        int ury
    );
    void SaveToRectFile(std::string &file_name) const;
};

}

#endif //PHYDB_INCLUDE_SPECIALMACRORECTLAYOUT_H_
