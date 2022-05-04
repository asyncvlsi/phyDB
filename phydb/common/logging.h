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
#ifndef PHYDB_COMMON_LOGGING_H_
#define PHYDB_COMMON_LOGGING_H_

#include <cassert>
#include <cstring>

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace phydb {

#define PhyDBExpects(e, error_message) do{ \
  if(!(e)) { \
    std::cout \
      << "\033[0;31m" \
      << "FATAL ERROR:" << "\n" \
      << "    " << error_message << "\n" \
      << __FILE__ << " : " << __LINE__ << " : " << __FUNCTION__ \
      << "\033[0m" << std::endl; \
      exit(1); \
  } \
} while(0)

#define PhyDBWarns(e, warning_message) do{ \
  if((e)) { \
    std::cout \
      << "\033[0;34m" \
      << "WARNING:" << "\n" \
      << "    " << warning_message << "\n" \
      << "\033[0m" << std::endl; \
  } \
} while(0)

}

#endif //PHYDB_COMMON_LOGGING_H_
