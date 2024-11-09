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
#ifndef PHYDB_TIMING_TECHCONFIG_H_
#define PHYDB_TIMING_TECHCONFIG_H_

#include <vector>

namespace phydb {

/****
 * @brief This structure presents an entry in tables in a technology
 * configuration file from OpenRCX.
 *
 * This structure contains four attributes:
 * 1. distance_: distance of two metal segments.
 * 2. coupling_cap_: coupling capacitance for a unit length.
 * 3. fringe_cap_: capacitance to the substrate for a unit length.
 * 4. res_: resistance for a unit length.
 *
 * A unit length is usually 1 micrometer.
 */
struct TableEntry {
 public:
  TableEntry(double distance, double coupling_cap, double fringe_cap,
             double res)
      : distance_(distance),
        coupling_cap_(coupling_cap),
        fringe_cap_(fringe_cap),
        res_(res) {}
  double distance_;
  double coupling_cap_;
  double fringe_cap_;
  double res_;

  void Report() const;
};

enum TableType {
  RES_OVER = 0,
  CAP_OVER = 1,
  CAP_UNDER = 2,
  CAP_DIAGUNDER = 3,
  CAP_OVERUNDER = 4,
  BAD_TABLE_TYPE = 5
};

/****
 * @brief A structure containing a resistance table.
 *
 * This table contains the following for columns in a technology configuration
 * file: distance_, coupling_cap_, fringe_cap_, res_.
 * However, the fringe_cap_ is 0 for most of the time, and the coupling_cap_
 * refers to another distance instead of a capacitance. Here is what an entry
 * in this table really is:
 * distance_1, distance_2, 0, resistance
 */
struct ConfigTable {
 public:
  ConfigTable(TableType type, int layer_index, int index0, int index1 = 0)
      : type_(type),
        layer_index_(layer_index),
        index0_(index0),
        index1_(index1) {}

  void AddEntry(double distance, double coupling_cap, double fringe_cap,
                double res);

  void FixLastEntryIfWrong();
  void SetWidth(double width);
  TableType Type() const;
  double Width() const;
  int LayerIndex() const;
  int Index0() const;
  int Index1() const;
  std::vector<TableEntry> &GetTable();

  void Report() const;

 private:
  TableType type_;
  double width_ = -1;
  int layer_index_ = -1;
  int index0_ = -1;
  int index1_ = -1;
  std::vector<TableEntry> table_;
};

class TechConfig {
 public:
  bool is_diagmodel_on_ = false;
  int layer_count_ = 0;
  int corner_count_;  // DensityRate : model_count data_rate_table
  std::vector<double> data_rate_table_;

  void SetDiagModelOn(bool is_diagmodel_on);
  void SetLayerCount(int layer_count);
  void SetModelCount(int model_count);
  void AddDataRate(double data_rate);

  void Report();
};

}  // namespace phydb

#endif  // PHYDB_TIMING_TECHCONFIG_H_
