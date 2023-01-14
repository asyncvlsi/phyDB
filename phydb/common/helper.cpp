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

#include "helper.h"

namespace phydb {

/****
 * @brief Splits a line into word tokens
 *
 * @param line: a string containing many words
 * @return: a list of word tokens
 */
std::vector<std::string> StrTokenize(std::string &line) {
  static std::vector<char> delimiter_list{' ', ':', ';', '\t', '\r', '\n'};

  std::vector<std::string> tokens;
  std::string empty_str;
  bool is_delimiter, old_is_delimiter = true;
  int current_field = -1;
  for (auto &c: line) {
    is_delimiter = false;
    for (auto &delimiter: delimiter_list) {
      if (c == delimiter) {
        is_delimiter = true;
        break;
      }
    }
    if (is_delimiter) {
      old_is_delimiter = is_delimiter;
      continue;
    } else {
      if (old_is_delimiter) {
        current_field++;
        tokens.push_back(empty_str);
      }
      tokens[current_field] += c;
      old_is_delimiter = is_delimiter;
    }
  }

  return tokens; // return value optimization
}

}