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

#ifndef PHYDB_SPECIALMACRORECTLAYOUT_H_
#define PHYDB_SPECIALMACRORECTLAYOUT_H_

#include <vector>

#include "datatype.h"
#include "macro.h"

namespace phydb {

struct RectSignalLayer {
 private:
  std::string signal_name_;
  std::string layer_name_;
  Rect2D<int> rect_;
 public:
  RectSignalLayer(
      std::string &signal_name,
      std::string &layer_name,
      int llx,
      int lly,
      int urx,
      int ury
  );
  std::string &SignalName();
  std::string &LayerName();
  Rect2D<int> &Rect();

  friend std::ostream &operator<<(
      std::ostream &os,
      const RectSignalLayer &rect_signal_layer
  );
};

struct SpecialMacroRectLayout {
 private:
  Macro *macro_ptr_;
  Rect2D<int> bbox_;
  std::vector<RectSignalLayer> rects_;
 public:
  explicit SpecialMacroRectLayout(
      Macro *macro_ptr,
      int llx,
      int lly,
      int urx,
      int ury
  );
  void SetBoundingBox(
      int llx,
      int lly,
      int urx,
      int ury
  );
  void AddRectSignalLayer(
      std::string &signal_name,
      std::string &layer_name,
      int llx,
      int lly,
      int urx,
      int ury
  );
  void SaveToRectFile(std::string const &file_name) const;
};

}

#endif //PHYDB_SPECIALMACRORECTLAYOUT_H_
