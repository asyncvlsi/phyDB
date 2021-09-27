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
    std::cout << "Metal " << layer_index_ << " RESOVER " << over_index_ << "\n";
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
    std::cout << "Metal " << layer_index_ << " OVER " << over_index_ << "\n";
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
    std::cout << "Metal " << layer_index_ << " UNDER " << under_index_ << "\n";
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
    std::cout << "Metal " << layer_index_
              << " DIAGUNDER " << diagunder_index_ << "\n";
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
    std::cout << "Metal " << layer_index_
              << " OVER " << over_index_
              << " UNDER " << under_index_ << "\n";
    std::cout << "DIST count " << table_.size()
              << " width " << width_ << "\n";
    for (auto &entry: table_) {
        entry.Report();
    }
}

void CornerModel::MarkResOver() {
    tmp_res_over_ = &res_over_.back();
    tmp_cap_over_ = nullptr;
    tmp_cap_under_ = nullptr;
    tmp_cap_diagunder_ = nullptr;
    tmp_cap_overunder_ = nullptr;
}

void CornerModel::MarkCapOver() {
    tmp_res_over_ = nullptr;
    tmp_cap_over_ = &cap_over_.back();
    tmp_cap_under_ = nullptr;
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
    tmp_cap_under_ = nullptr;
    tmp_cap_diagunder_ = &cap_diagunder_.back();
    tmp_cap_overunder_ = nullptr;
}

void CornerModel::MarkCapOverUnder() {
    tmp_res_over_ = nullptr;
    tmp_cap_over_ = nullptr;
    tmp_cap_under_ = nullptr;
    tmp_cap_diagunder_ = nullptr;
    tmp_cap_overunder_ = &cap_overunder_.back();
}

void CornerModel::MarkNothing() {
    tmp_res_over_ = nullptr;
    tmp_cap_over_ = nullptr;
    tmp_cap_under_ = nullptr;
    tmp_cap_diagunder_ = nullptr;
    tmp_cap_overunder_ = nullptr;
}

void CornerModel::FixResOverTableLastEntry() {
    for (auto &res_over_table: res_over_) {
        res_over_table.FixLastEntryIfWrong();
    }
}

std::vector<ResOverTable> &CornerModel::GetResOverTablesRef() {
    return res_over_;
}

std::vector<CapOverTable> &CornerModel::GetCapOverTablesRef() {
    return cap_over_;
}

std::vector<CapUnderTable> &CornerModel::GetCapUnderTablesRef() {
    return cap_under_;
}
std::vector<CapDiagUnderTable> &CornerModel::GetCapDiagUnderTablesRef() {
    return cap_diagunder_;
}

std::vector<CapOverUnderTable> &CornerModel::GetCapOverUnderTablesRef() {
    return cap_overunder_;
}

/****
 * @brief This function computes a optimized/simplified resistance table for
 * resistance extraction.
 *
 * This function is implemented based on `extMain::getResCapTable(...)` and
 * `extMain::calcRes0(...)` in OpenRCX.
 * The basic goal of the above two functions is to construct a simpler resistance
 * table to extract resistance, because the `ResOver` table may contain duplicated
 * or redundant information.
 *
 * This function calculate the simplified resistance table in the following way:
 * 1. iterate over all ResOver tables
 * 2. find all ResOver tables with over_index 0, and sort them if necessary
 * 3. for each ResOver table, put the res_ in the first entry to the simplified table
 *
 * @param is_report: if true, print out the simplified resistance table.
 *
 * @return
 */
void CornerModel::CalculateSimplifiedResistanceTable(bool is_report) {
    std::vector<std::pair<size_t, size_t>> candidate_table_index;
    std::unordered_set<size_t> index_in_table;

    size_t sz = res_over_.size();
    for (size_t i = 0; i < sz; ++i) {
        auto &res_over_table = res_over_[i];
        int layer_index = res_over_table.LayerIndex();
        int over_index = res_over_table.OverIndex();
        if (over_index == 0) {
            if (index_in_table.find(layer_index) == index_in_table.end()) {
                candidate_table_index.emplace_back(layer_index, i);
                index_in_table.insert(layer_index);
            }
        }
    }

    std::sort(
        candidate_table_index.begin(),
        candidate_table_index.end(),
        [](const std::pair<size_t, size_t> &lhs,
           const std::pair<size_t, size_t> &rhs) {
            return lhs.first < rhs.first;
        }
    );

    resistance_table_.reserve(candidate_table_index.size());
    for (auto &pair: candidate_table_index) {
        size_t layer_index = pair.first;
        size_t res_over_table_index = pair.second;
        PhyDBExpects(layer_index - 1 == resistance_table_.size(),
                     "Metal layer index and resistance_table_ size do not match during computation");
        auto &table = res_over_[res_over_table_index].GetTable();
        PhyDBExpects(!table.empty(),
                     "Cannot find a valid resistance table for metal index: "
                         + std::to_string(layer_index));
        resistance_table_.push_back(table[0].res_);
    }

    if (is_report) {
        std::cout << "model index: " << model_index_ << "\n";
        sz = resistance_table_.size();
        for (size_t i = 0; i < sz; ++i) {
            std::cout << i + 1 << ": " << resistance_table_[i] << ", ";
        }
        std::cout << "\n";
    }
}

double CornerModel::GetResistance(
    int metal_index,
    double width,
    double length
) {
    PhyDBExpects(metal_index >= 0, "Negative metal index is not allowed!");
    PhyDBExpects(metal_index < (int) resistance_table_.size(), "Metal index out of bound!");

    return length / width * resistance_table_[metal_index];
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

void TechConfig::AddModel(int model_index) {
    model_table_.emplace_back(model_index);
}

CornerModel *TechConfig::GetLastModel() {
    if (model_table_.empty()) {
        return nullptr;
    }
    return &model_table_.back();
}

void TechConfig::FixResOverTable() {
    for (auto &model: model_table_) {
        model.FixResOverTableLastEntry();
    }
}

void TechConfig::CalculateSimplifiedResistanceTable(bool is_report) {
    for (auto &model: model_table_) {
        model.CalculateSimplifiedResistanceTable(is_report);
    }
}

CornerModel &TechConfig::GetModel(int model_index) {
    PhyDBExpects(model_index >= 0, "Negative model index is not allowed!");
    PhyDBExpects(model_index < (int) model_table_.size(), "Model index out of bound!");
    return model_table_[model_index];
}

double TechConfig::GetResistance(
    int metal_index,
    double width,
    double length,
    int model_index
) {
    auto &model = GetModel(model_index);
    return model.GetResistance(metal_index, width, length);
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

Interpreter::Interpreter(TechConfig *tech_config) :
    scanner_(*this),
    parser_(scanner_, *this),
    tech_config_(tech_config) {}

int Interpreter::Parse() {
    int res = parser_.parse();
    tech_config_->FixResOverTable();
    return res;
}

void Interpreter::SetInputStream(std::istream *is) {
    scanner_.switch_streams(is, nullptr);
}

TechConfig *Interpreter::UserData() {
    return tech_config_;
}

}
