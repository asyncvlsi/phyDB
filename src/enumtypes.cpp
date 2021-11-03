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

#include "enumtypes.h"

#include "iostream"

namespace phydb {

LayerType StrToLayerType(std::string const &str_layer_type) {
  LayerType layer_type = ROUTING;
  if (str_layer_type == "ROUTING") {
    layer_type = ROUTING;
  } else if (str_layer_type == "CUT") {
    layer_type = CUT;
  } else {
    std::cout << "Unknown LayerType: " << str_layer_type << std::endl;
    exit(1);
  }
  return layer_type;
}

std::string LayerTypeStr(LayerType layer_type) {
  std::string s;
  switch (layer_type) {
    case ROUTING: { s = "ROUTING"; }
      break;
    case CUT: { s = "CUT"; }
      break;
    default: {
      std::cout << "LayerType error! This should never happen!" << std::endl;
      exit(1);
    }
  }
  return s;
}

MetalDirection StrToMetalDirection(std::string const &str_metal_direction) {
  MetalDirection metal_direction = HORIZONTAL;
  if (str_metal_direction == "HORIZONTAL") {
    metal_direction = HORIZONTAL;
  } else if (str_metal_direction == "VERTICAL") {
    metal_direction = VERTICAL;
  } else if (str_metal_direction == "DIAG45") {
    metal_direction = DIAG45;
  } else if (str_metal_direction == "DIAG135") {
    metal_direction = DIAG135;
  } else {
    std::cout << "Unknown MetalLayer direction: " << str_metal_direction
              << std::endl;
    exit(1);
  }
  return metal_direction;
}

std::string MetalDirectionStr(MetalDirection metal_direction) {
  std::string s;
  switch (metal_direction) {
    case HORIZONTAL: { s = "HORIZONTAL"; }
      break;
    case VERTICAL: { s = "VERTICAL"; }
      break;
    case DIAG45: { s = "DIAG45"; }
      break;
    case DIAG135: { s = "DIAG135"; }
      break;
    default: {
      std::cout << "MetalLayer direction error! This should never happen!"
                << std::endl;
      exit(1);
    }
  }
  return s;
}

XYDirection StrToXYDirection(std::string const &direction) {
  XYDirection dir = XYDirection::X;
  if (direction == "X")
    dir = XYDirection::X;
  else if (direction == "Y")
    dir = XYDirection::Y;
  else {
    std::cout << "unknown XYdirection: " << direction << std::endl;
    exit(1);
  }
  return dir;
}

std::string XYDirectionToStr(XYDirection dir) {
  std::string direction = "X";
  switch (dir) {
    case X: {
      direction = "X";
      break;
    }
    case Y: {
      direction = "Y";
      break;
    }
    default: {
      std::cout << "unknown XYdirection " << std::endl;
      exit(1);
    }
  }
  return direction;
}

std::string XYDirectionStr(XYDirection dir) {
  return XYDirectionToStr(dir);
}

CompOrient StrToCompOrient(std::string const &str_orient) {
  CompOrient orient = N;
  if (str_orient == "N" || str_orient == "R0") {
    orient = N;
  } else if (str_orient == "S" || str_orient == "R180") {
    orient = S;
  } else if (str_orient == "W" || str_orient == "R90") {
    orient = W;
  } else if (str_orient == "E" || str_orient == "R270") {
    orient = E;
  } else if (str_orient == "FN" || str_orient == "MY") {
    orient = FN;
  } else if (str_orient == "FS" || str_orient == "MX") {
    orient = FS;
  } else if (str_orient == "FW" || str_orient == "MX90"
      || str_orient == "MXR90") {
    orient = FW;
  } else if (str_orient == "FE" || str_orient == "MY90"
      || str_orient == "MYR90") {
    orient = FE;
  } else {
    std::cout << "Unknown Block orientation: " << str_orient << std::endl;
    exit(1);
  }
  return orient;
}

std::string CompOrientStr(CompOrient orient) {
  std::string s;
  switch (orient) {
    case N: { s = "N"; }
      break;
    case S: { s = "S"; }
      break;
    case W: { s = "W"; }
      break;
    case E: { s = "E"; }
      break;
    case FN: { s = "FN"; }
      break;
    case FS: { s = "FS"; }
      break;
    case FW: { s = "FW"; }
      break;
    case FE: { s = "FE"; }
      break;
    default: {
      std::cout << "Block orientation error! This should never happen!"
                << std::endl;
      exit(1);
    }
  }
  return s;
}

PlaceStatus StrToPlaceStatus(std::string const &str_place_status) {
  PlaceStatus place_status = UNPLACED;
  if (str_place_status == "COVER") {
    place_status = COVER;
  } else if (str_place_status == "FIXED" || str_place_status == "LOCKED"
      || str_place_status == "FIRM") {
    place_status = FIXED;
  } else if (str_place_status == "PLACED" || str_place_status == "SUGGESTED") {
    place_status = PLACED;
  } else if (str_place_status == "UNPLACED" || str_place_status == "NONE") {
    place_status = UNPLACED;
  } else {
    std::cout << "Unknown placement status: " << str_place_status << std::endl;
    exit(1);
  }
  return place_status;
}

std::string PlaceStatusStr(PlaceStatus place_status) {
  std::string s;
  switch (place_status) {
    case COVER: { s = "COVER"; }
      break;
    case FIXED: { s = "FIXED"; }
      break;
    case PLACED: { s = "PLACED"; }
      break;
    case UNPLACED: { s = "UNPLACED"; }
      break;
    default: {
      std::cout << "Unit placement state error! This should never happen!"
                << std::endl;
      exit(1);
    }
  }
  return s;
}

SignalDirection StrToSignalDirection(std::string const &str_signal_direction) {
  SignalDirection signal_direction = INPUT;
  if (str_signal_direction == "INPUT") {
    signal_direction = INPUT;
  } else if (str_signal_direction == "OUTPUT") {
    signal_direction = OUTPUT;
  } else if (str_signal_direction == "INOUT") {
    signal_direction = INOUT;
  } else if (str_signal_direction == "FEEDTHRU") {
    signal_direction = FEEDTHRU;
  } else if (str_signal_direction == "OUTPUT TRISTATE") {
    signal_direction = OUTPUT_TRISTATE;
  } else {
    std::cout << "Unknown SignalDirection: " << str_signal_direction
              << std::endl;
    exit(0);
  }
  return signal_direction;
}

std::string SignalDirectionStr(SignalDirection signal_direction) {
  std::string s;
  switch (signal_direction) {
    case INPUT: { s = "INPUT"; }
      break;
    case OUTPUT: { s = "OUTPUT"; }
      break;
    case INOUT: { s = "INOUT"; }
      break;
    case FEEDTHRU: { s = "FEEDTHRU"; }
      break;
    case OUTPUT_TRISTATE: { s = "OUTPUT TRISTATE"; }
      break;
    default: {
      std::cout << "IOPIN signal direction error! This should never happen!"
                << std::endl;
      exit(1);
    }
  }
  return s;
}

SignalUse StrToSignalUse(std::string const &str_signal_use) {
  SignalUse signal_use = SIGNAL;
  if (str_signal_use == "SIGNAL") {
    signal_use = SIGNAL;
  } else if (str_signal_use == "POWER") {
    signal_use = POWER;
  } else if (str_signal_use == "GROUND") {
    signal_use = GROUND;
  } else if (str_signal_use == "CLOCK") {
    signal_use = CLOCK;
  } else if (str_signal_use == "TIEOFF") {
    signal_use = TIEOFF;
  } else if (str_signal_use == "ANALOG") {
    signal_use = ANALOG;
  } else if (str_signal_use == "SCAN") {
    signal_use = SCAN;
  } else if (str_signal_use == "RESET") {
    signal_use = RESET;
  } else {
    std::cout << "Unknown SignalUse: " << str_signal_use << std::endl;
    exit(0);
  }
  return signal_use;
}

std::string SignalUseStr(SignalUse signal_use) {
  std::string s;
  switch (signal_use) {
    case SIGNAL: { s = "SIGNAL"; }
      break;
    case POWER: { s = "POWER"; }
      break;
    case GROUND: { s = "GROUND"; }
      break;
    case CLOCK: { s = "CLOCK"; }
      break;
    case TIEOFF: { s = "TIEOFF"; }
      break;
    case ANALOG: { s = "ANALOG"; }
      break;
    case SCAN: { s = "SCAN"; }
      break;
    case RESET: { s = "RESET"; }
      break;
    default: {
      std::cout << "IOPIN signal use error! This should never happen!"
                << std::endl;
      exit(1);
    }
  }
  return s;
}

MacroClass StrToMacroClass(std::string const &str_macro_class) {
  MacroClass macro_class;
  if (str_macro_class == "CORE") {
    macro_class = CORE_;
  } else if (str_macro_class == "CORE WELLTAP") {
    macro_class = CORE_WELLTAP;
  } else if (str_macro_class == "CORE FEEDTHRU") {
    macro_class = CORE_FEEDTHRU;
  } else if (str_macro_class == "CORE TIEHIGH") {
    macro_class = CORE_TIEHIGH;
  } else if (str_macro_class == "CORE TIELOW") {
    macro_class = CORE_TIELOW;
  } else if (str_macro_class == "CORE SPACER") {
    macro_class = CORE_SPACER;
  } else if (str_macro_class == "CORE ANTENNACELL") {
    macro_class = CORE_ANTENNACELL;
  } else if (str_macro_class == "COVER") {
    macro_class = COVER_;
  } else if (str_macro_class == "COVER BUMP") {
    macro_class = COVER_BUMP;
  } else if (str_macro_class == "RING") {
    macro_class = RING_;
  } else if (str_macro_class == "BLOCK") {
    macro_class = BLOCK_;
  } else if (str_macro_class == "BLOCK BLACKBOX") {
    macro_class = BLOCK_BLACKBOX;
  } else if (str_macro_class == "BLOCK SOFT") {
    macro_class = BLOCK_SOFT;
  } else if (str_macro_class == "PAD") {
    macro_class = PAD_;
  } else if (str_macro_class == "PAD INPUT") {
    macro_class = PAD_INPUT;
  } else if (str_macro_class == "PAD OUTPUT") {
    macro_class = PAD_OUTPUT;
  } else if (str_macro_class == "PAD INOUT") {
    macro_class = PAD_INOUT;
  } else if (str_macro_class == "PAD POWER") {
    macro_class = PAD_POWER;
  } else if (str_macro_class == "PAD SPACER") {
    macro_class = PAD_SPACER;
  } else if (str_macro_class == "PAD AREAIO") {
    macro_class = PAD_AREAIO;
  } else if (str_macro_class == "ENDCAP PRE") {
    macro_class = ENDCAP_PRE;
  } else if (str_macro_class == "ENDCAP POST") {
    macro_class = ENDCAP_POST;
  } else if (str_macro_class == "ENDCAP TOPLEFT") {
    macro_class = ENDCAP_TOPLEFT;
  } else if (str_macro_class == "ENDCAP TOPRIGHT") {
    macro_class = ENDCAP_TOPRIGHT;
  } else if (str_macro_class == "ENDCAP BOTTOMLEFT") {
    macro_class = ENDCAP_BOTTOMLEFT;
  } else if (str_macro_class == "ENDCAP BOTTOMRIGHT") {
    macro_class = ENDCAP_BOTTOMRIGHT;
  } else {
    std::cout << "Unknown MacroClass: " << str_macro_class << std::endl;
    exit(0);
  }
  return macro_class;
}

std::string MacroClassStr(MacroClass macro_class) {
  std::string s;
  switch (macro_class) {
    case COVER_: { s = "COVER"; }
      break;
    case COVER_BUMP: { s = "COVER BUMP"; }
      break;
    case RING_: { s = "RING"; }
      break;
    case BLOCK_: { s = "BLOCK"; }
      break;
    case BLOCK_BLACKBOX: { s = "BLOCK BLACKBOX"; }
      break;
    case BLOCK_SOFT: { s = "BLOCK SOFT"; }
      break;
    case PAD_: { s = "PAD"; }
      break;
    case PAD_INPUT: { s = "PAD INPUT"; }
      break;
    case PAD_OUTPUT: { s = "PAD OUTPUT"; }
      break;
    case PAD_INOUT: { s = "PAD INOUT"; }
      break;
    case PAD_POWER: { s = "PAD POWER"; }
      break;
    case PAD_SPACER: { s = "PAD SPACER"; }
      break;
    case PAD_AREAIO: { s = "PAD AREAIO"; }
      break;
    case CORE_: { s = "CORE"; }
      break;
    case CORE_FEEDTHRU: { s = "CORE FEEDTHRU"; }
      break;
    case CORE_TIEHIGH: { s = "CORE TIEHIGH"; }
      break;
    case CORE_TIELOW: { s = "CORE TIELOW"; }
      break;
    case CORE_SPACER: { s = "CORE SPACER"; }
      break;
    case CORE_ANTENNACELL: { s = "CORE ANTENNACELL"; }
      break;
    case CORE_WELLTAP: { s = "CORE WELLTAP"; }
      break;
    case ENDCAP_PRE: { s = "ENDCAP PRE"; }
      break;
    case ENDCAP_POST: { s = "ENDCAP POST"; }
      break;
    case ENDCAP_TOPLEFT: { s = "ENDCAP TOPLEFT"; }
      break;
    case ENDCAP_TOPRIGHT: { s = "ENDCAP TOPRIGHT"; }
      break;
    case ENDCAP_BOTTOMLEFT: { s = "ENDCAP BOTTOMLEFT"; }
      break;
    case ENDCAP_BOTTOMRIGHT: { s = "ENDCAP BOTTOMRIGHT"; }
      break;
    default: {
      std::cout << "IOPIN signal use error! This should never happen!"
                << std::endl;
      exit(1);
    }
  }
  return s;
}

}

