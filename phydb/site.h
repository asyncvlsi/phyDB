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

#include "phydb/common/logging.h"

namespace phydb {

class Site {
 private:
  std::string name_;
  std::string class_name_;
  double width_;
  double height_;

 public:
  Site() : name_(""), class_name_(""), width_(0), height_(0) {}
  Site(std::string name, std::string className, double width, double height) :
      name_(name), class_name_(className), width_(width), height_(height) {}

  void SetName(std::string);
  void SetClassName(std::string);
  void SetWidth(double);
  void SetHeight(double);

  std::string GetName() const;
  std::string GetClassName() const;
  double GetWidth() const;
  double GetHeight() const;

};

std::ostream &operator<<(std::ostream &, const Site &);

}

#endif //PHYDB_SITE_H_
