#include "pch.h"

namespace {
    s_player players[MAX_PLAYERS];
}

namespace TECHDATA {
    s_tech_hull_data _hull_data[HULL_CLASS_COUNT];
    s_tech_drive_data _drives[DRIVE_COUNT];
    s_tech_specials_data _specials[SPECIAL_COUNT];
}

namespace MOX {
    s_player* _player = players;
}