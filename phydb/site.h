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
#ifndef PHYDB_SITE_H_
#define PHYDB_SITE_H_

#include <string>

#include "enumtypes.h"
#include "phydb/common/logging.h"

namespace phydb {

class Site {
 public:
  Site() = default;
  Site(std::string const &name, SiteClass site_class, double width,
       double height)
      : name_(name), site_class_(site_class), width_(width), height_(height) {}

  void SetName(std::string const &);
  void SetClass(SiteClass site_class);
  void SetWidth(double);
  void SetHeight(double);
  void SetSymmetry(bool x, bool y, bool r90);

  const std::string &GetName() const;
  SiteClass GetClass() const;
  double GetWidth() const;
  double GetHeight() const;
  Symmetry GetSymmetry() const;

 private:
  std::string name_;
  SiteClass site_class_;
  double width_ = 0;
  double height_ = 0;
  Symmetry symmetry_;
};

std::ostream &operator<<(std::ostream &, const Site &);

}  // namespace phydb

#endif  // PHYDB_SITE_H_
