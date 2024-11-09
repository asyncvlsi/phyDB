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
#ifndef PHYDB_CLUSTERCOL_H_
#define PHYDB_CLUSTERCOL_H_

#include <string>
#include <vector>

namespace phydb {

class ClusterCol {
 public:
  ClusterCol() : lx_(0), ux_(0) {}
  ClusterCol(std::string const &name, std::string const &bot_signal)
      : name_(name), bot_signal_(bot_signal) {}
  ClusterCol(std::string const &name, std::string const &bot_signal, int lx,
             int ux)
      : name_(name), bot_signal_(bot_signal), lx_(lx), ux_(ux) {}
  ClusterCol(std::string const &name, std::string const &bot_signal, int lx,
             int ux, std::vector<int> &ly, std::vector<int> &uy)
      : name_(name),
        bot_signal_(bot_signal),
        lx_(lx),
        ux_(ux),
        ly_(ly),
        uy_(uy) {}

  void SetName(std::string &);
  void SetBotSignal(std::string &);
  void SetXRange(int lx, int ux);
  void SetLX(int lx);
  void SetUX(int ux);
  void AddRow(int ly, int uy);

  std::string GetName() const;
  std::string GetBotSignal() const;
  int GetLX() const;
  int GetUX() const;
  std::vector<int> &GetLY();
  std::vector<int> &GetUY();

  void Report();

 private:
  std::string name_;
  std::string bot_signal_;
  int lx_;
  int ux_;
  std::vector<int> ly_;
  std::vector<int> uy_;
};

}  // namespace phydb

#endif  // PHYDB_CLUSTERCOL_H_
