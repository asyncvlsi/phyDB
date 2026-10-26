/*******************************************************************************
 *
 * Copyright (c) 2026 Yihang Yang
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
/**
 * A minimal JSON reader for design-structure metadata.
 *
 * phyDB links no JSON library, and the structure file written by synthesis
 * uses only objects, arrays, strings, integers and booleans. This reader
 * covers exactly that subset (plus null and real numbers, so a
 * well-formed file never fails for a value type it does not use) and
 * reports the byte offset of the first syntax error.
 */
#ifndef PHYDB_STRUCTURE_STRUCTURE_JSON_H_
#define PHYDB_STRUCTURE_STRUCTURE_JSON_H_

#include <map>
#include <string>
#include <vector>

namespace phydb {

struct StructureJsonValue {
  enum class Type { kNull, kBool, kNumber, kString, kArray, kObject };
  Type type = Type::kNull;
  bool boolean = false;
  double number = 0.0;
  std::string string;
  std::vector<StructureJsonValue> array;
  std::map<std::string, StructureJsonValue> object;

  /** Member `key` of an object, or nullptr when absent or not an object. */
  const StructureJsonValue *Find(std::string const &key) const;
};

/** Parse `text`; on failure return false and describe the error. */
bool ParseStructureJson(std::string const &text, StructureJsonValue *root,
                        std::string *error_message);

}  // namespace phydb

#endif  // PHYDB_STRUCTURE_STRUCTURE_JSON_H_
