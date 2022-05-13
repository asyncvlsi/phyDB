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

#include "site.h"

namespace phydb {

void Site::SetName(std::string const &name) {
  name_ = name;
}

void Site::SetClass(SiteClass site_class) {
  site_class_ = site_class;
}

void Site::SetWidth(double width) {
  width_ = width;
}

void Site::SetHeight(double height) {
  height_ = height;
}

void Site::SetSymmetry(bool x, bool y, bool r90) {
  symmetry_.Set(x, y, r90);
}

const std::string &Site::GetName() const {
  return name_;
}

SiteClass Site::GetClass() const {
  return site_class_;
}

double Site::GetWidth() const {
  return width_;
}

double Site::GetHeight() const {
  return height_;
}

Symmetry Site::GetSymmetry() const {
  return symmetry_;
}

std::ostream &operator<<(std::ostream &os, const Site &s) {
  os << s.GetName() << " "
     << SiteClassStr(s.GetClass()) << "\n"
     << s.GetSymmetry().Str() << "\n"
     << s.GetWidth() << " "
     << s.GetHeight() << "\n";
  return os;

}

}
