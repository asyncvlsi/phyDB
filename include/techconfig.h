//
// Created by yihang on 9/16/21.
//

#ifndef PHYDB_INCLUDE_TECHCONFIG_H_
#define PHYDB_INCLUDE_TECHCONFIG_H_

#include <vector>

#include "scanner.h"
#include "parser.hpp"

namespace phydb{

class CapEntry {
public:
    CapEntry(double distance, double coupling_cap, double fringe_cap, double res):
        distance_(distance), coupling_cap_(coupling_cap), fringe_cap_(fringe_cap), res_(res) {}
    double distance_;
    double coupling_cap_;
    double fringe_cap_;
    double res_;

    void Report();
};

class ResOverTable {
public:
    explicit ResOverTable(int layer_index): layer_index_(layer_index) {}
    double width_;
    int layer_index_;
    std::vector<CapEntry> table_;

    void AddEntry(double distance, double coupling_cap, double fringe_cap, double res);
    void Report();
};

class CapOverTable {
public:
    explicit CapOverTable(int layer_index): layer_index_(layer_index) {}
    double width_;
    int layer_index_;
    std::vector<CapEntry> table_;

    void AddEntry(double distance, double coupling_cap, double fringe_cap, double res);
    void Report();
};

class CapUnderTable {
public:
    explicit CapUnderTable(int layer_index): layer_index_(layer_index) {}
    double width_;
    int layer_index_;
    std::vector<CapEntry> table_;

    void AddEntry(double distance, double coupling_cap, double fringe_cap, double res);
    void Report();
};

class CapDiagUnderTable {
public:
    explicit CapDiagUnderTable(int layer_index): layer_index_(layer_index) {}
    double width_;
    int layer_index_;
    std::vector<CapEntry> table_;

    void AddEntry(double distance, double coupling_cap, double fringe_cap, double res);
    void Report();
};

class CapOverUnderTable {
public:
    CapOverUnderTable(int over_index, int under_index): over_index_(over_index), under_index_(under_index) {}
    double width_;
    int over_index_;
    int under_index_;
    std::vector<CapEntry> table_;

    void AddEntry(double distance, double coupling_cap, double fringe_cap, double res);
    void Report();
};

class CornerModel {
public:
    explicit CornerModel(int model_index): model_index_(model_index) {}
    int model_index_;
    std::vector<ResOverTable> res_over_;
    std::vector<CapOverTable> cap_over_;
    std::vector<CapUnderTable> cap_under_;
    std::vector<CapDiagUnderTable> cap_diagunder_;
    std::vector<CapOverUnderTable> cap_overunder_;

    ResOverTable *tmp_res_over_ = nullptr;
    CapOverTable *tmp_cap_over_ = nullptr;
    CapUnderTable *tmp_cap_under_ = nullptr;
    CapDiagUnderTable *tmp_cap_diagunder_ = nullptr;
    CapOverUnderTable *tmp_cap_overunder_ = nullptr;

    void MarkResOver();
    void MarkCapOver();
    void MarkCapUnder();
    void MarkCapDiagUnder();
    void MarkCapOverUnder();
    void MarkNothing();

    void Report();
};

class TechConfig {
public:
    bool is_diagmodel_on_ = false;
    int layer_count_ = 0;
    // DensityRate : model_count data_rate_table
    int model_count_;
    std::vector<double> data_rate_table_;
    // DensityModel
    std::vector<CornerModel> model_table_;

    void SetDiagmodelOn(bool is_diagmodel_on);
    void SetLayerCount(int layer_count);
    void SetModelCount(int model_count);
    void AddDataRate(double data_rate);
    void AddModel(int model_index);
    CornerModel *GetLastModel();

    void Report();
};

class Interpreter {
  public:
    explicit Interpreter(TechConfig *tech_config);
    int parse();
    void clear();
    void switchInputStream(std::istream *is);
    TechConfig *UserData();
    friend class Parser;
    friend class Scanner;

  private:
    void increaseLocation(unsigned int loc);
    unsigned int location() const;

    Scanner scanner_;
    Parser parser_;
    unsigned int location_;
    TechConfig *tech_config_ = nullptr;
};

}

#endif //PHYDB_INCLUDE_TECHCONFIG_H_
