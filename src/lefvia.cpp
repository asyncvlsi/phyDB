#include "lefvia.h"

namespace phydb {

void LefVia::Report() {
    std::cout << "LefVia name: " << name_ << "\n";
    for (auto &layer_rect : layer_rects_) {
        layer_rect.Report();
    }
}

}