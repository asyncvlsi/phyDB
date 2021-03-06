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

#include "spacingtable.h"

namespace phydb {

void SpacingTable::SetSize(int nC, int nR) {
  Reset();
  n_col_ = nC;
  n_row_ = nR;
  parallel_run_length_.resize(nC);
  width_.resize(nR);
  spacing_.resize(nC * nR);
}

void SpacingTable::Reset() {
  n_col_ = 0;
  n_row_ = 0;
  parallel_run_length_.clear();
  width_.clear();
  spacing_.clear();
}

int SpacingTable::GetNCol() const {
  return n_col_;
}

int SpacingTable::GetNRow() const {
  return n_row_;
}

void SpacingTable::SetParallelRunLengthVec(const std::vector<double> &v) {
  assert(v.size() == static_cast<size_t>(n_col_));
  parallel_run_length_ = v;
}

void SpacingTable::SetWidthVec(const std::vector<double> &v) {
  assert(v.size() == static_cast<size_t>(n_row_));
  width_ = v;
}

void SpacingTable::SetSpacingVec(const std::vector<double> &v) {
  assert(v.size() == static_cast<size_t>(n_row_ * n_col_));
  spacing_ = v;
}

std::vector<double> &SpacingTable::GetParallelRunLengthVec() {
  return parallel_run_length_;
}

std::vector<double> &SpacingTable::GetWidthVec() {
  return width_;
}

std::vector<double> &SpacingTable::GetSpacingVec() {
  return spacing_;
}

void SpacingTable::SetParallelRunLengthAt(int col, double val) {
  assert(col < n_col_);
  parallel_run_length_[col] = val;
}

void SpacingTable::SetWidthAt(int row, double val) {
  assert(row < n_row_);
  width_[row] = val;
}

void SpacingTable::SetSpacingAt(int col, int row, double val) {
  assert(col + n_col_ * row < n_col_ * n_row_);
  spacing_[col + n_col_ * row] = val;
}

double SpacingTable::GetParallelRunLengthAt(int col) const {
  assert(col < n_col_);
  return parallel_run_length_[col];
}

double SpacingTable::GetWidthAt(int row) const {
  assert(row < n_row_);
  return width_[row];
}

double SpacingTable::GetSpacingAt(int col, int row) const {
  assert(col + n_col_ * row < n_col_ * n_row_);
  return spacing_[col + n_col_ * row];
}

double SpacingTable::GetSpacingForWidth(double width) const {
  assert(width > 0);
  int i = 0;
  for (; i < n_row_ - 1; i++) {
    if (width >= width_[i] && width < width_[i + 1])
      break;
  }
  if (width >= width_[n_row_ - 1])
    i = n_row_ - 1;
  return GetSpacingAt(n_col_ - 1, i);//for wire of infinite length
}

double SpacingTable::GetSpacingFor(double w, double l) const {
  assert(w > 0 && l > 0);
  int col = 0, row = 0;
  for (; row < n_row_ - 1; row++) {
    if (w >= width_[row] && w < width_[row + 1])
      break;
  }
  if (w >= width_[n_row_ - 1])
    row = n_row_ - 1;

  for (; col < n_col_ - 1; col++) {
    if (l >= parallel_run_length_[col] && l < parallel_run_length_[col + 1])
      break;
  }
  if (l >= parallel_run_length_[n_col_ - 1])
    col = n_col_ - 1;

  return GetSpacingAt(col, row);

}

std::ostream &operator<<(std::ostream &os, const SpacingTable &st) {
  int nCol = st.GetNCol();
  int nRow = st.GetNRow();
  os << "nCol: " << nCol << " nRow: " << nRow << "\n";

  os << "\t";
  for (int c = 0; c < nCol; c++)
    os << st.GetParallelRunLengthAt(c) << "\t";
  os << "\n";
  for (int r = 0; r < nRow; r++) {
    os << st.GetWidthAt(r) << "\t";
    for (int c = 0; c < nCol; c++) {
      os << st.GetSpacingAt(c, r) << "\t";
    }
    os << "\n";
  }
  return os;
}

}
