#ifndef PHYDB_INCLUDE_DEFVIA_H_
#define PHYDB_INCLUDE_DEFVIA_H_

#include "datatype.h"

namespace phydb {

class DefVia {
  private:
    std::string name_;
    //int idx_;
    std::string via_rule_name_;
    Size2D<int> cut_size_;
    std::string layers_[3];
    Size2D<int> cut_spacing_;
    Size2D<int> bot_enc_;
    Size2D<int> top_enc_;
    int num_cut_rows_;
    int num_cut_cols_;
    Size2D<int> origin_;
    Size2D<int> bot_offset_;
    Size2D<int> top_offset_;

    std::vector<Rect2DLayer < int>> rect2d_layers;
    std::string pattern_;
  public:
    DefVia() {}
    DefVia(std::string &name) : name_(name) {}

    void Reset() {
        name_ = "";
        via_rule_name_ = "";
        cut_size_.Clear();
        layers_[0] = "";
        layers_[1] = "";
        layers_[2] = "";
        cut_spacing_.Clear();
        bot_enc_.Clear();
        top_enc_.Clear();
        num_cut_rows_ = 0;
        num_cut_cols_ = 0;
        origin_.Clear();
        bot_offset_.Clear();
        top_offset_.Clear();
    }

    void Report() {
        std::cout
            << "Via: " << name_ << " VIARule: " << via_rule_name_ << "\n"
            << "CUT: " << cut_size_.x << " " << cut_size_.y << "\n"
            << "Layer: " << layers_[0] << " " << layers_[1] << " " << layers_[2] << "\n"
            << "CUTSPACING: " << cut_spacing_.x << " " << cut_spacing_.y << "\n"
            << "bot_enc_: " << bot_enc_.x << " " << bot_enc_.y << "\n"
            << "top_enc_: " << top_enc_.x << " " << top_enc_.y << "\n"
            << " row: " << num_cut_rows_ << " col: " << num_cut_cols_ << "\n"
            << " origin_: " << origin_.x << " " << origin_.y << "\n"
            << " bot_offset_: " << bot_offset_.x << " " << bot_offset_.y << "\n"
            << " top_offset_: " << top_offset_.x << " " << top_offset_.y << "\n";
    }
};

}

#endif //PHYDB_INCLUDE_DEFVIA_H_
