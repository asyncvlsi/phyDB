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
#include "structure_json.h"

#include <cctype>
#include <cstdlib>

namespace phydb {

const StructureJsonValue *StructureJsonValue::Find(
    std::string const &key) const {
  if (type != Type::kObject) return nullptr;
  auto it = object.find(key);
  return it == object.end() ? nullptr : &it->second;
}

namespace {

/**
 * Recursive-descent parser over the input string.
 *
 * Each Parse* method consumes one value starting at pos_ and returns false
 * with error_ set on the first malformed token; the caller stops there.
 */
class Parser {
 public:
  explicit Parser(std::string const &text) : text_(text) {}

  bool ParseDocument(StructureJsonValue *root, std::string *error_message) {
    SkipSpace();
    bool ok = ParseValue(root);
    if (ok) {
      SkipSpace();
      if (pos_ != text_.size()) ok = Fail("trailing characters");
    }
    if (!ok) *error_message = error_;
    return ok;
  }

 private:
  bool Fail(std::string const &what) {
    error_ = what + " at byte " + std::to_string(pos_);
    return false;
  }

  void SkipSpace() {
    while (pos_ < text_.size() &&
           std::isspace(static_cast<unsigned char>(text_[pos_]))) {
      ++pos_;
    }
  }

  bool Consume(char c) {
    SkipSpace();
    if (pos_ < text_.size() && text_[pos_] == c) {
      ++pos_;
      return true;
    }
    return false;
  }

  bool ParseValue(StructureJsonValue *value) {
    SkipSpace();
    if (pos_ >= text_.size()) return Fail("unexpected end of input");
    char c = text_[pos_];
    if (c == '{') return ParseObject(value);
    if (c == '[') return ParseArray(value);
    if (c == '"') {
      value->type = StructureJsonValue::Type::kString;
      return ParseString(&value->string);
    }
    if (text_.compare(pos_, 4, "true") == 0) {
      pos_ += 4;
      value->type = StructureJsonValue::Type::kBool;
      value->boolean = true;
      return true;
    }
    if (text_.compare(pos_, 5, "false") == 0) {
      pos_ += 5;
      value->type = StructureJsonValue::Type::kBool;
      return true;
    }
    if (text_.compare(pos_, 4, "null") == 0) {
      pos_ += 4;
      value->type = StructureJsonValue::Type::kNull;
      return true;
    }
    return ParseNumber(value);
  }

  bool ParseNumber(StructureJsonValue *value) {
    char const *begin = text_.c_str() + pos_;
    char *end = nullptr;
    double number = std::strtod(begin, &end);
    if (end == begin) return Fail("expected a value");
    pos_ += static_cast<size_t>(end - begin);
    value->type = StructureJsonValue::Type::kNumber;
    value->number = number;
    return true;
  }

  bool ParseString(std::string *out) {
    if (!Consume('"')) return Fail("expected '\"'");
    out->clear();
    while (pos_ < text_.size()) {
      char c = text_[pos_++];
      if (c == '"') return true;
      if (c != '\\') {
        out->push_back(c);
        continue;
      }
      if (pos_ >= text_.size()) break;
      char e = text_[pos_++];
      switch (e) {
        case '"': out->push_back('"'); break;
        case '\\': out->push_back('\\'); break;
        case '/': out->push_back('/'); break;
        case 'b': out->push_back('\b'); break;
        case 'f': out->push_back('\f'); break;
        case 'n': out->push_back('\n'); break;
        case 'r': out->push_back('\r'); break;
        case 't': out->push_back('\t'); break;
        default: return Fail("unsupported escape in string");
      }
    }
    return Fail("unterminated string");
  }

  bool ParseArray(StructureJsonValue *value) {
    Consume('[');
    value->type = StructureJsonValue::Type::kArray;
    if (Consume(']')) return true;
    do {
      value->array.emplace_back();
      if (!ParseValue(&value->array.back())) return false;
    } while (Consume(','));
    return Consume(']') || Fail("expected ']'");
  }

  bool ParseObject(StructureJsonValue *value) {
    Consume('{');
    value->type = StructureJsonValue::Type::kObject;
    if (Consume('}')) return true;
    do {
      SkipSpace();
      std::string key;
      if (!ParseString(&key)) return false;
      if (!Consume(':')) return Fail("expected ':'");
      if (!ParseValue(&value->object[key])) return false;
    } while (Consume(','));
    return Consume('}') || Fail("expected '}'");
  }

  std::string const &text_;
  size_t pos_ = 0;
  std::string error_;
};

}  // namespace

bool ParseStructureJson(std::string const &text, StructureJsonValue *root,
                        std::string *error_message) {
  Parser parser(text);
  return parser.ParseDocument(root, error_message);
}

}  // namespace phydb
