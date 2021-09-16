//
// Created by yihang on 9/16/21.
//

#ifndef PHYDB_INCLUDE_TECHCONFIG_H_
#define PHYDB_INCLUDE_TECHCONFIG_H_

#include <vector>

class ResOverTable {
    double width_;
    std::vector<double> space1_;
    std::vector<double> space2_;
    // store the upper triangular matrix as a list
    std::vector<double> cap_;
    std::vector<double> res_;
};

class CapEntry {
    double distance_;
    double coupling_cap_;
    double fringe_cap_;
    double res_;
};

enum CapTableType{
    OVER = 0,
    UNDER =1 ,
    DIAGUNDER = 2,
    OVERUNDER = 3
};

class CapTable {
    double width_;
    CapTableType type_;
    std::vector<CapEntry> table_;
};


class CornerModel {
  private:
    int model_index_;
    std::vector<ResOverTable> res_over_;
    std::vector<CapTable> cap_over_;
};

class TechConfig {
  private:
    bool is_diagmodel_on_;
    int layer_count_;
    // DensityRate : model_count data_rate_table
    int model_count_;
    std::vector<double> data_rate_table_;
    // DensityModel
    std::vector<CornerModel> model_table_;
};

#endif //PHYDB_INCLUDE_TECHCONFIG_H_
