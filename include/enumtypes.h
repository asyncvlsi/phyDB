#ifndef PHYDB_ENUMTYPES_H_
#define PHYDB_ENUMTYPES_H_

#include "string"

namespace phydb {

enum MetalDirection {
    HORIZONTAL = 0,
    VERTICAL = 1,
    DIAG45 = 2,
    DIAG135 = 3
};

MetalDirection StrToMetalDirection(std::string &str_metal_direction);
std::string MetalDirectionStr(MetalDirection metal_direction);

enum BlockOrient {
    N = 0,
    S = 1,
    W = 2,
    E = 3,
    FN = 4,
    FS = 5,
    FW = 6,
    FE = 7
};

BlockOrient StrToOrient(std::string &str_orient);
BlockOrient StrToOrient(const char *str_orient);
std::string OrientStr(BlockOrient orient);

enum PlaceStatus {
    COVER = 0,
    FIXED = 1,
    PLACED = 2,
    UNPLACED = 3,
    NULL_STATE = 4
};

PlaceStatus StrToPlaceStatus(std::string &str_place_status);
std::string PlaceStatusStr(PlaceStatus place_status);

enum SignalDirection {
    INPUT = 0,
    OUTPUT = 1,
    INOUT = 2,
    FEEDTHRU = 3
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

}

#endif //PHYDB_ENUMTYPES_H_