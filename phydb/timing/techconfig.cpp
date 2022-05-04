/*******************************************************************************
 *
 * Copyright (c) 2021 Jiayuan He, Yihang Yang
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 ******************************************************************************/

#include "techconfig.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

#include "phydb/common/logging.h"

namespace phydb {

void TableEntry::Report() const {
  std::cout << distance_ << " "
            << coupling_cap_ << " "
            << fringe_cap_ << " "
            << res_ << "\n";
}

void ConfigTable::AddEntry(
    double distance,
    double coupling_cap,
    double fringe_cap,
    double res
) {
  table_.emplace_back(distance, coupling_cap, fringe_cap, res);
}

void ConfigTable::FixLastEntryIfWrong() {
  if (table_.empty()) return;
  TableEntry &last_entry = table_.back();
  if (last_entry.coupling_cap_ == 0) {
    last_entry.coupling_cap_ = last_entry.fringe_cap_;
    last_entry.fringe_cap_ = 0;
  }
}

void ConfigTable::SetWidth(double width) {
  width_ = width;
}

TableType ConfigTable::Type() const {
  return type_;
}

double ConfigTable::Width() const {
  return width_;
}

int ConfigTable::LayerIndex() const {
  return layer_index_;
}

int ConfigTable::Index0() const {
  return index0_;
}

int ConfigTable::Index1() const {
  return index1_;
}

std::vector<TableEntry> &ConfigTable::GetTable() {
  return table_;
}

void ConfigTable::Report() const {
  std::cout << "Metal " << layer_index_ + 1 << " RESOVER " << index0_ + 1
            << "\n";
  std::cout << "DIST count " << table_.size() << " width " << width_ << "\n";
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
  corner_count_ = model_count;
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

  std::cout << "DensityRate " << corner_count_ << " ";
  for (auto &data_rate: data_rate_table_) {
    std::cout << data_rate << " ";
  }
  std::cout << "\n";
}

}
