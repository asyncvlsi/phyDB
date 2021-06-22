#ifndef PHYDB_ENUMTYPES_H_
#define PHYDB_ENUMTYPES_H_

#include "string"

namespace phydb {

enum LayerType {
    ROUTING = 0,
    CUT = 1
};

LayerType StrToLayerType(std::string &str_layer_type);
std::string LayerTypeStr(LayerType layer_type);

enum XYDirection {
    X = 0,
    Y = 1,
};

XYDirection StrToXYDirection(std::string &direction);
std::string XYDirectionToStr(XYDirection);

enum MetalDirection {
    HORIZONTAL = 0,
    VERTICAL = 1,
    DIAG45 = 2,
    DIAG135 = 3
};

MetalDirection StrToMetalDirection(std::string &str_metal_direction);
std::string MetalDirectionStr(MetalDirection metal_direction);

enum CompOrient {
    N = 0,
    S = 1,
    W = 2,
    E = 3,
    FN = 4,
    FS = 5,
    FW = 6,
    FE = 7
};

CompOrient StrToCompOrient(std::string &str_orient);
CompOrient StrToCompOrient(const char *str_orient);
std::string CompOrientStr(CompOrient orient);

enum PlaceStatus {
    COVER = 0,
    FIXED = 1,
    PLACED = 2,
    UNPLACED = 3
};

PlaceStatus StrToPlaceStatus(std::string &str_place_status);
std::string PlaceStatusStr(PlaceStatus place_status);

enum SignalDirection {
    INPUT = 0,
    OUTPUT = 1,
    INOUT = 2,
    FEEDTHRU = 3,
    OUTPUT_TRISTATE = 4
};

SignalDirection StrToSignalDirection(std::string &str_signal_direction);
std::string SignalDirectionStr(SignalDirection signal_direction);

enum SignalUse {
    SIGNAL = 0,
    POWER = 1,
    GROUND = 2,
    CLOCK = 3,
    TIEOFF = 4,
    ANALOG = 5,
    SCAN = 6,
    RESET = 7
};

SignalUse StrToSignalUse(std::string &str_signal_use);
std::string SignalUseStr(SignalUse signal_use);

enum MacroClass {
    COVER_ = 0,
    COVER_BUMP = 1,

    RING_ = 2,

    BLOCK_ = 3,
    BLOCK_BLACKBOX = 4,
    BLOCK_SOFT = 5,

    PAD_ = 6,
    PAD_INPUT = 7,
    PAD_OUTPUT = 8,
    PAD_INOUT = 9,
    PAD_POWER = 10,
    PAD_SPACER = 11,
    PAD_AREAIO = 12,

    CORE_ = 13,
    CORE_FEEDTHRU = 14,
    CORE_TIEHIGH = 15,
    CORE_TIELOW = 16,
    CORE_SPACER = 17,
    CORE_ANTENNACELL = 18,
    CORE_WELLTAP = 19,

    ENDCAP_PRE = 20,
    ENDCAP_POST = 21,
    ENDCAP_TOPLEFT = 22,
    ENDCAP_TOPRIGHT = 23,
    ENDCAP_BOTTOMLEFT = 24,
    ENDCAP_BOTTOMRIGHT = 25
};

MacroClass StrToMacroClass(std::string &str_macro_class);
std::string MacroClassStr(MacroClass macro_class);

}

#endif //PHYDB_ENUMTYPES_H_
