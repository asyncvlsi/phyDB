#ifndef DEFVIA_H
#define DEFVIA_H

#include "DataType.h"

namespace phydb {

class DefVia {
  private:
    string name_;
    int idx_;
    string via_rule_name_;
    Size2D<int> cut_size_;
    string layers_[3];
    Size2D<int> cut_spacing_;
    Size2D<int> bot_enc_;
    Size2D<int> top_enc_;
    int num_cut_rows_;
    int num_cut_cols_;
    Size2D<int> origin_;
    Size2D<int> bot_offset_;
    Size2D<int> top_offset_;

    vector<Rect2DLayer < int>> rect2d_layers;
    string pattern_;
  public:
    DefVia() {}
    DefVia(string &name) : name_(name) {}

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
        cout << "Via: " << name_ << " VIARule: " << via_rule_name_ << endl;
        cout << "CUT: " << cut_size_.x << " " << cut_size_.y << endl;
        cout << "Layer: " << layers_[0] << " " << layers_[1] << " " << layers_[2] << endl;
        cout << "CUTSPACING: " << cut_spacing_.x << " " << cut_spacing_.y << endl;
        cout << "bot_enc_: " << bot_enc_.x << " " << bot_enc_.y << endl;
        cout << "top_enc_: " << top_enc_.x << " " << top_enc_.y << endl;
        cout << " row: " << num_cut_rows_ << " col: " << num_cut_cols_ << endl;
        cout << " origin_: " << origin_.x << " " << origin_.y << endl;
        cout << " bot_offset_: " << bot_offset_.x << " " << bot_offset_.y << endl;
        cout << " top_offset_: " << top_offset_.x << " " << top_offset_.y << endl;
    }
};

}

#endif
