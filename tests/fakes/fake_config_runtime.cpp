#include "pch.h"

namespace MOX {
    s_settings _settings = {};
    int16_t _ranged_to_hit_penalty[9] = {};
    int16_t _ranged_damage_penalty[9] = {};
}

namespace MAPGEN {
    int16_t _maximum_star_count = 99;
}

namespace SHIPMOVE {
    uint8_t _jump_gate_speed_bonus = 3;
    bool _jump_gate_pass_nebula;
}

namespace CMBTSPEC {
    uint8_t _leader_dies_with_no;
    uint8_t _compressor_per_size_class_damage;
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

namespace CMBTFIR2 {
    uint16_t _systems_repair_percent = 50;
}

namespace TECHDATA {
    s_tech_shield_data _shields[SHIELD_COUNT] = {};
    uint16_t _shield_hull_multiplier[HULL_CLASS_COUNT] = {};
    uint8_t _hard_shields_damage_reduction;
}

namespace SCIENCE {
    uint8_t _finds_presented_by_scientist;
}

namespace OFFICER {
    uint8_t _leader_warlord_bonus;
}