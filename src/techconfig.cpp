#include "techconfig.h"

#include <iostream>

namespace phydb{

void CapEntry::Report() {
    std::cout << distance_ << " " << coupling_cap_ << " " << fringe_cap_ << " " << res_ << "\n";
}

void ResOverTable::AddEntry(double distance, double coupling_cap, double fringe_cap, double res) {
    table_.emplace_back(distance, coupling_cap,fringe_cap, res);
}

void ResOverTable::Report() {
    std::cout << "Metal " << "?" << " RESOVER " << layer_index_ << "\n";
    std::cout << "DIST count " << table_.size() << " width " << width_ << "\n";
    for (auto &entry: table_) {
        entry.Report();
    }
}

void CapOverTable::AddEntry(double distance, double coupling_cap, double fringe_cap, double res) {
    table_.emplace_back(distance, coupling_cap,fringe_cap, res);
}

void CapOverTable::Report() {
    std::cout << "Metal " << "?" << " OVER " << layer_index_ << "\n";
    std::cout << "DIST count " << table_.size() << " width " << width_ << "\n";
    for (auto &entry: table_) {
        entry.Report();
    }
}

void CapUnderTable::AddEntry(double distance, double coupling_cap, double fringe_cap, double res) {
    table_.emplace_back(distance, coupling_cap,fringe_cap, res);
}

void CapUnderTable::Report() {
    std::cout << "Metal " << "?" << " UNDER " << layer_index_ << "\n";
    std::cout << "DIST count " << table_.size() << " width " << width_ << "\n";
    for (auto &entry: table_) {
        entry.Report();
    }
}

void CapDiagUnderTable::AddEntry(double distance, double coupling_cap, double fringe_cap, double res) {
    table_.emplace_back(distance, coupling_cap,fringe_cap, res);
}

void CapDiagUnderTable::Report() {
    std::cout << "Metal " << "?" << " DIAGUNDER " << layer_index_ << "\n";
    std::cout << "DIST count " << table_.size() << " width " << width_ << "\n";
    for (auto &entry: table_) {
        entry.Report();
    }
}

void CapOverUnderTable::AddEntry(double distance, double coupling_cap, double fringe_cap, double res) {
    table_.emplace_back(distance, coupling_cap,fringe_cap, res);
}

void CapOverUnderTable::Report() {
    std::cout << "Metal " << "?" << " OVER " << over_index_ << " UNDER " << under_index_ << "\n";
    std::cout << "DIST count " << table_.size() << " width " << width_ << "\n";
    for (auto &entry: table_) {
        entry.Report();
    }
}

void CornerModel::MarkResOver() {
    tmp_res_over_ = &res_over_.back();
    tmp_cap_over_ = nullptr;
    tmp_cap_under_= nullptr;
    tmp_cap_diagunder_ = nullptr;
    tmp_cap_overunder_ = nullptr;
}

void CornerModel::MarkCapOver() {
    tmp_res_over_ = nullptr;
    tmp_cap_over_ = &cap_over_.back();
    tmp_cap_under_= nullptr;
    tmp_cap_diagunder_ = nullptr;
    tmp_cap_overunder_ = nullptr;
}

void CornerModel::MarkCapUnder() {
    tmp_res_over_ = nullptr;
    tmp_cap_over_ = nullptr;
    tmp_cap_under_ = &cap_under_.back();
    tmp_cap_diagunder_ = nullptr;
    tmp_cap_overunder_ = nullptr;
}

void CornerModel::MarkCapDiagUnder() {
    tmp_res_over_ = nullptr;
    tmp_cap_over_ = nullptr;
    tmp_cap_under_= nullptr;
    tmp_cap_diagunder_ = &cap_diagunder_.back();
    tmp_cap_overunder_ = nullptr;
}

void CornerModel::MarkCapOverUnder() {
    tmp_res_over_ = nullptr;
    tmp_cap_over_ = nullptr;
    tmp_cap_under_= nullptr;
    tmp_cap_diagunder_ = nullptr;
    tmp_cap_overunder_ = &cap_overunder_.back();
}

void CornerModel::MarkNothing() {
    tmp_res_over_ = nullptr;
    tmp_cap_over_ = nullptr;
    tmp_cap_under_= nullptr;
    tmp_cap_diagunder_ = nullptr;
    tmp_cap_overunder_ = nullptr;
}

void CornerModel::Report() {
    std::cout << "DensityModel " << model_index_ << "\n";

    for (auto &table: res_over_) {
        table.Report();
    }
    for (auto &table: cap_over_) {
        table.Report();
    }
    for (auto &table: cap_under_) {
        table.Report();
    }
    for (auto &table: cap_diagunder_) {
        table.Report();
    }
    for (auto &table: cap_overunder_) {
        table.Report();
    }
}

void TechConfig::SetDiagmodelOn(bool is_diagmodel_on) {
    is_diagmodel_on_ = is_diagmodel_on;
}

void TechConfig::SetLayerCount(int layer_count) {
    layer_count_ = layer_count;
}

void TechConfig::SetModelCount(int model_count) {
    model_count_ = model_count;
}

void TechConfig::AddDataRate(double data_rate) {
    data_rate_table_.push_back(data_rate);
}

void TechConfig::AddModel(int model_index) {
    model_table_.emplace_back(model_index);
}

CornerModel *TechConfig::GetLastModel() {
    if (model_table_.empty()) {
        return nullptr;
    }
    return &model_table_.back();
}

void TechConfig::Report() {
    std::cout << "DIAGMODEL ";
    if (is_diagmodel_on_) {
        std::cout << "ON";
    } else {
        std::cout << "OFF";
    }
    std::cout << "\n";

    std::cout << "LayerCount " << layer_count_ << "\n";

    std::cout << "DensityRate " << model_count_ << " ";
    for (auto &data_rate: data_rate_table_) {
        std::cout << data_rate << " ";
    }
    std::cout << "\n";

    for (auto &model: model_table_) {
        model.Report();
    }
}

}
