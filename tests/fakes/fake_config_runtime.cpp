#include "pch.h"

namespace MOX {
    s_settings _settings = {};
}

namespace MAPGEN {
    int16_t _maximum_star_count = 99;
}

namespace CMBTSPEC {
    uint8_t _leader_dies_with_no;
}

namespace CMBTMIS {
    int8_t _missile_initiative = -1;
}

namespace CMBTFIRE {
    s_mount_damage _mount_dmg[HIGH_ENERGY_FOCUS_PRESENCE_COUNT] = {
        { 50, 100, 150 },
        { 100, 150, 200 },
    };
}

namespace SCIENCE {
    uint8_t _finds_presented_by_scientist;
}