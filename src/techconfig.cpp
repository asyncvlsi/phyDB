#include "techconfig.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

#include "logging.h"

namespace phydb {

void CapEntry::Report() const {
    std::cout << distance_ << " "
              << coupling_cap_ << " "
              << fringe_cap_ << " "
              << res_ << "\n";
}

void ResOverTable::AddEntry(
    double distance,
    double coupling_cap,
    double fringe_cap,
    double res
) {
    table_.emplace_back(distance, coupling_cap, fringe_cap, res);
}

void ResOverTable::FixLastEntryIfWrong() {
    if (table_.empty()) return;
    CapEntry &last_entry = table_.back();
    if (last_entry.coupling_cap_ == 0) {
        last_entry.coupling_cap_ = last_entry.fringe_cap_;
        last_entry.fringe_cap_ = 0;
    }
}

void ResOverTable::SetWidth(double width) {
    width_ = width;
}

double ResOverTable::Width() const {
    return width_;
}

int ResOverTable::LayerIndex() const {
    return layer_index_;
}

int ResOverTable::OverIndex() const {
    return over_index_;
}

std::vector<CapEntry> &ResOverTable::GetTable() {
    return table_;
}

void ResOverTable::Report() const {
    std::cout << "Metal " << layer_index_ + 1 << " RESOVER " << over_index_ + 1
              << "\n";
    std::cout << "DIST count " << table_.size() << " width " << width_ << "\n";
    for (auto &entry: table_) {
        entry.Report();
    }
}

void CapOverTable::AddEntry(
    double distance,
    double coupling_cap,
    double fringe_cap,
    double res
) {
    table_.emplace_back(distance, coupling_cap, fringe_cap, res);
}

void CapOverTable::SetWidth(double width) {
    width_ = width;
}

double CapOverTable::Width() const {
    return width_;
}

int CapOverTable::LayerIndex() const {
    return layer_index_;
}

int CapOverTable::OverIndex() const {
    return over_index_;
}

std::vector<CapEntry> &CapOverTable::GetTable() {
    return table_;
}

void CapOverTable::Report() const {
    std::cout << "Metal " << layer_index_ + 1 << " OVER " << over_index_ + 1
              << "\n";
    std::cout << "DIST count " << table_.size() << " width " << width_ << "\n";
    for (auto &entry: table_) {
        entry.Report();
    }
}

void CapUnderTable::AddEntry(
    double distance,
    double coupling_cap,
    double fringe_cap,
    double res
) {
    table_.emplace_back(distance, coupling_cap, fringe_cap, res);
}

void CapUnderTable::SetWidth(double width) {
    width_ = width;
}

double CapUnderTable::Width() const {
    return width_;
}

int CapUnderTable::LayerIndex() const {
    return layer_index_;
}

int CapUnderTable::UnderIndex() const {
    return under_index_;
}

std::vector<CapEntry> &CapUnderTable::GetTable() {
    return table_;
}

void CapUnderTable::Report() {
    std::cout << "Metal " << layer_index_ + 1 << " UNDER " << under_index_ + 1
              << "\n";
    std::cout << "DIST count " << table_.size() << " width " << width_ << "\n";
    for (auto &entry: table_) {
        entry.Report();
    }
}

void CapDiagUnderTable::AddEntry(
    double distance,
    double coupling_cap,
    double fringe_cap,
    double res
) {
    table_.emplace_back(distance, coupling_cap, fringe_cap, res);
}

void CapDiagUnderTable::SetWidth(double width) {
    width_ = width;
}

double CapDiagUnderTable::Width() const {
    return width_;
}

int CapDiagUnderTable::LayerIndex() const {
    return layer_index_;
}

int CapDiagUnderTable::DiagUnderIndex() const {
    return diagunder_index_;
}

std::vector<CapEntry> &CapDiagUnderTable::GetTable() {
    return table_;
}

void CapDiagUnderTable::Report() {
    std::cout << "Metal " << layer_index_ + 1
              << " DIAGUNDER " << diagunder_index_ + 1 << "\n";
    std::cout << "DIST count " << table_.size()
              << " width " << width_ << "\n";
    for (auto &entry: table_) {
        entry.Report();
    }
}

void CapOverUnderTable::AddEntry(
    double distance,
    double coupling_cap,
    double fringe_cap,
    double res
) {
    table_.emplace_back(distance, coupling_cap, fringe_cap, res);
}

void CapOverUnderTable::SetWidth(double width) {
    width_ = width;
}

double CapOverUnderTable::Width() const {
    return width_;
}

int CapOverUnderTable::LayerIndex() const {
    return layer_index_;
}

int CapOverUnderTable::OverIndex() const {
    return over_index_;
}

int CapOverUnderTable::UnderIndex() const {
    return under_index_;
}

std::vector<CapEntry> &CapOverUnderTable::GetTable() {
    return table_;
}

void CapOverUnderTable::Report() {
    std::cout << "Metal " << layer_index_ + 1
              << " OVER " << over_index_ + 1
              << " UNDER " << under_index_ + 1 << "\n";
    std::cout << "DIST count " << table_.size()
              << " width " << width_ << "\n";
    for (auto &entry: table_) {
        entry.Report();
    }
}

void TechConfig::SetDiagModelOn(bool is_diagmodel_on) {
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
}

}
