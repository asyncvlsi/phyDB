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

#include "clustercol.h"

namespace phydb {

void ClusterCol::SetName(std::string &name) {
  name_ = name;
}

void ClusterCol::SetBotSignal(std::string &bot_signal) {
  bool e = (bot_signal == "GND") || (bot_signal == "VDD");
  PhyDBExpects(e, "ERROR: bot signal in cluster col is not GND or VDD");
  bot_signal_ = bot_signal;
}
void ClusterCol::SetXRange(int lx, int ux) {
  PhyDBExpects(lx < ux, "ERROR: in cluster col lx >= ux");
  lx_ = lx;
  ux_ = ux;
}
void ClusterCol::SetLX(int lx) {
  lx_ = lx;
}
void ClusterCol::SetUX(int ux) {
  ux_ = ux;
}
void ClusterCol::AddRow(int ly, int uy) {
  PhyDBExpects(ly < uy, "ERROR: in cluster col lx >= ux");
  ly_.push_back(ly);
  uy_.push_back(uy);
}

std::string ClusterCol::GetName() const {
  return name_;
}

std::string ClusterCol::GetBotSignal() const {
  return bot_signal_;
}

int ClusterCol::GetLX() const {
  return lx_;
}

int ClusterCol::GetUX() const {
  return ux_;
}

std::vector<int> &ClusterCol::GetLY() {
  return ly_;
}

std::vector<int> &ClusterCol::GetUY() {
  return uy_;
}

void ClusterCol::Report() {
  std::cout << "signal: " << bot_signal_ << " " << lx_ << " " << ux_
            << std::endl;
  for (size_t i = 0; i < ly_.size(); i++) {
    std::cout << ly_[i] << " " << uy_[i] << std::endl;
  }

}

}
