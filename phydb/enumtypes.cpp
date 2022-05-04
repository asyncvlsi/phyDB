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

#include <iostream>

#include "phydb/common/logging.h"

namespace phydb {

LayerType StrToLayerType(std::string const &str_layer_type) {
  LayerType layer_type = phydb::LayerType::ROUTING;
  if (str_layer_type == "ROUTING") {
    layer_type = phydb::LayerType::ROUTING;
  } else if (str_layer_type == "CUT") {
    layer_type = phydb::LayerType::CUT;
  } else {
    std::cout << "Unknown LayerType: " << str_layer_type << std::endl;
    exit(1);
  }
  return layer_type;
}

std::string LayerTypeStr(LayerType layer_type) {
  std::string s;
  switch (layer_type) {
    case phydb::LayerType::ROUTING: { s = "ROUTING"; }
      break;
    case phydb::LayerType::CUT: { s = "CUT"; }
      break;
    default: {
      std::cout << "LayerType error! This should never happen!" << std::endl;
      exit(1);
    }
  }
  return s;
}

MetalDirection StrToMetalDirection(std::string const &str_metal_direction) {
  MetalDirection metal_direction = phydb::MetalDirection::HORIZONTAL;
  if (str_metal_direction == "HORIZONTAL") {
    metal_direction = phydb::MetalDirection::HORIZONTAL;
  } else if (str_metal_direction == "VERTICAL") {
    metal_direction = phydb::MetalDirection::VERTICAL;
  } else if (str_metal_direction == "DIAG45") {
    metal_direction = phydb::MetalDirection::DIAG45;
  } else if (str_metal_direction == "DIAG135") {
    metal_direction = phydb::MetalDirection::DIAG135;
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
    case phydb::MetalDirection::HORIZONTAL: { s = "HORIZONTAL"; }
      break;
    case phydb::MetalDirection::VERTICAL: { s = "VERTICAL"; }
      break;
    case phydb::MetalDirection::DIAG45: { s = "DIAG45"; }
      break;
    case phydb::MetalDirection::DIAG135: { s = "DIAG135"; }
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

std::string XYDirectionStr(XYDirection dir) {
  std::string direction = "X";
  switch (dir) {
    case XYDirection::X: {
      direction = "X";
      break;
    }
    case XYDirection::Y: {
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

CompOrient StrToCompOrient(std::string const &str_orient) {
  CompOrient orient = CompOrient::N;
  if (str_orient == "N" || str_orient == "R0") {
    orient = CompOrient::N;
  } else if (str_orient == "S" || str_orient == "R180") {
    orient = CompOrient::S;
  } else if (str_orient == "W" || str_orient == "R90") {
    orient = CompOrient::W;
  } else if (str_orient == "E" || str_orient == "R270") {
    orient = CompOrient::E;
  } else if (str_orient == "FN" || str_orient == "MY") {
    orient = CompOrient::FN;
  } else if (str_orient == "FS" || str_orient == "MX") {
    orient = CompOrient::FS;
  } else if (str_orient == "FW" || str_orient == "MX90"
      || str_orient == "MXR90") {
    orient = CompOrient::FW;
  } else if (str_orient == "FE" || str_orient == "MY90"
      || str_orient == "MYR90") {
    orient = CompOrient::FE;
  } else {
    std::cout << "Unknown Block orientation: " << str_orient << std::endl;
    exit(1);
  }
  return orient;
}

std::string CompOrientStr(CompOrient orient) {
  std::string s;
  switch (orient) {
    case CompOrient::N: { s = "N"; }
      break;
    case CompOrient::S: { s = "S"; }
      break;
    case CompOrient::W: { s = "W"; }
      break;
    case CompOrient::E: { s = "E"; }
      break;
    case CompOrient::FN: { s = "FN"; }
      break;
    case CompOrient::FS: { s = "FS"; }
      break;
    case CompOrient::FW: { s = "FW"; }
      break;
    case CompOrient::FE: { s = "FE"; }
      break;
    default: {
      std::cout << "Block orientation error! This should never happen!"
                << std::endl;
      exit(1);
    }
  }
  return s;
}

CompSource StrToCompSource(std::string const &str_comp_source) {
  if (str_comp_source == "NETLIST") {
    return CompSource::NETLIST;
  } else if (str_comp_source == "DIST") {
    return CompSource::DIST;
  } else if (str_comp_source == "USER") {
    return CompSource::USER;
  } else if (str_comp_source == "TIMING") {
    return CompSource::TIMING;
  } else {
    PhyDBExpects(false, "Unknown component source");
  }
  return CompSource::NETLIST;
}

std::string CompSourceStr(CompSource comp_source) {
  std::string res;
  switch (comp_source) {
    case CompSource::NETLIST: { res = "NETLIST"; }
      break;
    case CompSource::DIST: { res = "DIST"; }
      break;
    case CompSource::USER: { res = "USER"; }
      break;
    case CompSource::TIMING: { res = "TIMING"; }
      break;
    default: {
      PhyDBExpects(false, "This should never happen");
    }
  }
  return res;
}

PlaceStatus StrToPlaceStatus(std::string const &str_place_status) {
  PlaceStatus place_status = PlaceStatus::UNPLACED;
  if (str_place_status == "COVER") {
    place_status = PlaceStatus::COVER;
  } else if (str_place_status == "FIXED" || str_place_status == "LOCKED"
      || str_place_status == "FIRM") {
    place_status = PlaceStatus::FIXED;
  } else if (str_place_status == "PLACED" || str_place_status == "SUGGESTED") {
    place_status = PlaceStatus::PLACED;
  } else if (str_place_status == "UNPLACED" || str_place_status == "NONE") {
    place_status = PlaceStatus::UNPLACED;
  } else {
    std::cout << "Unknown placement status: " << str_place_status << std::endl;
    exit(1);
  }
  return place_status;
}

std::string PlaceStatusStr(PlaceStatus place_status) {
  std::string s;
  switch (place_status) {
    case PlaceStatus::COVER: { s = "COVER"; }
      break;
    case PlaceStatus::FIXED: { s = "FIXED"; }
      break;
    case PlaceStatus::PLACED: { s = "PLACED"; }
      break;
    case PlaceStatus::UNPLACED: { s = "UNPLACED"; }
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
  SignalDirection signal_direction = SignalDirection::INPUT;
  if (str_signal_direction == "INPUT") {
    signal_direction = SignalDirection::INPUT;
  } else if (str_signal_direction == "OUTPUT") {
    signal_direction = SignalDirection::OUTPUT;
  } else if (str_signal_direction == "INOUT") {
    signal_direction = SignalDirection::INOUT;
  } else if (str_signal_direction == "FEEDTHRU") {
    signal_direction = SignalDirection::FEEDTHRU;
  } else if (str_signal_direction == "OUTPUT TRISTATE") {
    signal_direction = SignalDirection::OUTPUT_TRISTATE;
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
    case SignalDirection::INPUT: { s = "INPUT"; }
      break;
    case SignalDirection::OUTPUT: { s = "OUTPUT"; }
      break;
    case SignalDirection::INOUT: { s = "INOUT"; }
      break;
    case SignalDirection::FEEDTHRU: { s = "FEEDTHRU"; }
      break;
    case SignalDirection::OUTPUT_TRISTATE: { s = "OUTPUT TRISTATE"; }
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
  SignalUse signal_use = SignalUse::SIGNAL;
  if (str_signal_use == "SIGNAL") {
    signal_use = SignalUse::SIGNAL;
  } else if (str_signal_use == "POWER") {
    signal_use = SignalUse::POWER;
  } else if (str_signal_use == "GROUND") {
    signal_use = SignalUse::GROUND;
  } else if (str_signal_use == "CLOCK") {
    signal_use = SignalUse::CLOCK;
  } else if (str_signal_use == "TIEOFF") {
    signal_use = SignalUse::TIEOFF;
  } else if (str_signal_use == "ANALOG") {
    signal_use = SignalUse::ANALOG;
  } else if (str_signal_use == "SCAN") {
    signal_use = SignalUse::SCAN;
  } else if (str_signal_use == "RESET") {
    signal_use = SignalUse::RESET;
  } else {
    std::cout << "Unknown SignalUse: " << str_signal_use << std::endl;
    exit(0);
  }
  return signal_use;
}

std::string SignalUseStr(SignalUse signal_use) {
  std::string s;
  switch (signal_use) {
    case SignalUse::SIGNAL: { s = "SIGNAL"; }
      break;
    case SignalUse::POWER: { s = "POWER"; }
      break;
    case SignalUse::GROUND: { s = "GROUND"; }
      break;
    case SignalUse::CLOCK: { s = "CLOCK"; }
      break;
    case SignalUse::TIEOFF: { s = "TIEOFF"; }
      break;
    case SignalUse::ANALOG: { s = "ANALOG"; }
      break;
    case SignalUse::SCAN: { s = "SCAN"; }
      break;
    case SignalUse::RESET: { s = "RESET"; }
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
    macro_class = MacroClass::CORE;
  } else if (str_macro_class == "CORE WELLTAP") {
    macro_class = MacroClass::CORE_WELLTAP;
  } else if (str_macro_class == "CORE FEEDTHRU") {
    macro_class = MacroClass::CORE_FEEDTHRU;
  } else if (str_macro_class == "CORE TIEHIGH") {
    macro_class = MacroClass::CORE_TIEHIGH;
  } else if (str_macro_class == "CORE TIELOW") {
    macro_class = MacroClass::CORE_TIELOW;
  } else if (str_macro_class == "CORE SPACER") {
    macro_class = MacroClass::CORE_SPACER;
  } else if (str_macro_class == "CORE ANTENNACELL") {
    macro_class = MacroClass::CORE_ANTENNACELL;
  } else if (str_macro_class == "COVER") {
    macro_class = MacroClass::COVER;
  } else if (str_macro_class == "COVER BUMP") {
    macro_class = MacroClass::COVER_BUMP;
  } else if (str_macro_class == "RING") {
    macro_class = MacroClass::RING;
  } else if (str_macro_class == "BLOCK") {
    macro_class = MacroClass::BLOCK;
  } else if (str_macro_class == "BLOCK BLACKBOX") {
    macro_class = MacroClass::BLOCK_BLACKBOX;
  } else if (str_macro_class == "BLOCK SOFT") {
    macro_class = MacroClass::BLOCK_SOFT;
  } else if (str_macro_class == "PAD") {
    macro_class = MacroClass::PAD;
  } else if (str_macro_class == "PAD INPUT") {
    macro_class = MacroClass::PAD_INPUT;
  } else if (str_macro_class == "PAD OUTPUT") {
    macro_class = MacroClass::PAD_OUTPUT;
  } else if (str_macro_class == "PAD INOUT") {
    macro_class = MacroClass::PAD_INOUT;
  } else if (str_macro_class == "PAD POWER") {
    macro_class = MacroClass::PAD_POWER;
  } else if (str_macro_class == "PAD SPACER") {
    macro_class = MacroClass::PAD_SPACER;
  } else if (str_macro_class == "PAD AREAIO") {
    macro_class = MacroClass::PAD_AREAIO;
  } else if (str_macro_class == "ENDCAP PRE") {
    macro_class = MacroClass::ENDCAP_PRE;
  } else if (str_macro_class == "ENDCAP POST") {
    macro_class = MacroClass::ENDCAP_POST;
  } else if (str_macro_class == "ENDCAP TOPLEFT") {
    macro_class = MacroClass::ENDCAP_TOPLEFT;
  } else if (str_macro_class == "ENDCAP TOPRIGHT") {
    macro_class = MacroClass::ENDCAP_TOPRIGHT;
  } else if (str_macro_class == "ENDCAP BOTTOMLEFT") {
    macro_class = MacroClass::ENDCAP_BOTTOMLEFT;
  } else if (str_macro_class == "ENDCAP BOTTOMRIGHT") {
    macro_class = MacroClass::ENDCAP_BOTTOMRIGHT;
  } else {
    std::cout << "Unknown MacroClass: " << str_macro_class << std::endl;
    exit(0);
  }
  return macro_class;
}

std::string MacroClassStr(MacroClass macro_class) {
  std::string s;
  switch (macro_class) {
    case MacroClass::COVER: { s = "COVER"; }
      break;
    case MacroClass::COVER_BUMP: { s = "COVER BUMP"; }
      break;
    case MacroClass::RING: { s = "RING"; }
      break;
    case MacroClass::BLOCK: { s = "BLOCK"; }
      break;
    case MacroClass::BLOCK_BLACKBOX: { s = "BLOCK BLACKBOX"; }
      break;
    case MacroClass::BLOCK_SOFT: { s = "BLOCK SOFT"; }
      break;
    case MacroClass::PAD: { s = "PAD"; }
      break;
    case MacroClass::PAD_INPUT: { s = "PAD INPUT"; }
      break;
    case MacroClass::PAD_OUTPUT: { s = "PAD OUTPUT"; }
      break;
    case MacroClass::PAD_INOUT: { s = "PAD INOUT"; }
      break;
    case MacroClass::PAD_POWER: { s = "PAD POWER"; }
      break;
    case MacroClass::PAD_SPACER: { s = "PAD SPACER"; }
      break;
    case MacroClass::PAD_AREAIO: { s = "PAD AREAIO"; }
      break;
    case MacroClass::CORE: { s = "CORE"; }
      break;
    case MacroClass::CORE_FEEDTHRU: { s = "CORE FEEDTHRU"; }
      break;
    case MacroClass::CORE_TIEHIGH: { s = "CORE TIEHIGH"; }
      break;
    case MacroClass::CORE_TIELOW: { s = "CORE TIELOW"; }
      break;
    case MacroClass::CORE_SPACER: { s = "CORE SPACER"; }
      break;
    case MacroClass::CORE_ANTENNACELL: { s = "CORE ANTENNACELL"; }
      break;
    case MacroClass::CORE_WELLTAP: { s = "CORE WELLTAP"; }
      break;
    case MacroClass::ENDCAP_PRE: { s = "ENDCAP PRE"; }
      break;
    case MacroClass::ENDCAP_POST: { s = "ENDCAP POST"; }
      break;
    case MacroClass::ENDCAP_TOPLEFT: { s = "ENDCAP TOPLEFT"; }
      break;
    case MacroClass::ENDCAP_TOPRIGHT: { s = "ENDCAP TOPRIGHT"; }
      break;
    case MacroClass::ENDCAP_BOTTOMLEFT: { s = "ENDCAP BOTTOMLEFT"; }
      break;
    case MacroClass::ENDCAP_BOTTOMRIGHT: { s = "ENDCAP BOTTOMRIGHT"; }
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

