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

void Site::SetName(std::string name) {
  name_ = name;
}

void Site::SetClassName(std::string className) {
  class_name_ = className;
}

void Site::SetWidth(double width) {
  width_ = width;
}

void Site::SetHeight(double height) {
  height_ = height;
}

std::string Site::GetName() const {
  return name_;
}

std::string Site::GetClassName() const {
  return class_name_;
}

double Site::GetWidth() const {
  return width_;
}

double Site::GetHeight() const {
  return height_;
}

std::ostream &operator<<(std::ostream &os, const Site &s) {
  os << s.GetName() << " " << s.GetClassName() << "\n";
  os << s.GetWidth() << " " << s.GetHeight() << "\n";
  return os;

}

}
