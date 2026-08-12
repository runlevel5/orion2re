#include "pch.h"

namespace {
    s_combat_data combat_data[2];
    int warning_count;
    int combat_refresh_count;
    std::string last_warning;
}

namespace TEST_ABSORBER_CLEANUP {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(TECHDATA::_hull_data, 0, sizeof(TECHDATA::_hull_data));
        memset(TECHDATA::_specials, 0, sizeof(TECHDATA::_specials));
        combat_data[1].structure_max = 100;
        combat_data[1].drive_current_hits = 1;
        combat_data[1].ftl_type = 1;
        combat_data[1].size_class = HULL_CRUISER;
        TECHDATA::_hull_data[HULL_CRUISER].drive_hp = 30;
        CMBTFIR2::_systems_repair_percent = 50;
        MOX::_apply_damage_to_planet = 0;
        warning_count = 0;
        combat_refresh_count = 0;
        last_warning.clear();
    }

    void Set_Special_(int16_t ship_idx, int16_t special_id) {
        combat_data[ship_idx].special_device_flags[special_id / 8] |=
            (uint8_t)(1u << (special_id % 8));
    }

    int Warning_Count_() {
        return warning_count;
    }

    const char* Last_Warning_() {
        return last_warning.c_str();
    }

    int Combat_Refresh_Count_() {
        return combat_refresh_count;
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    uint16_t _apply_damage_to_planet;
}

namespace TECHDATA {
    s_tech_hull_data _hull_data[HULL_CLASS_COUNT];
    s_tech_armor_data _armor[ARMOR_COUNT];
    s_tech_specials_data _specials[SPECIAL_COUNT];
}

namespace DESIGN {
    int16_t __cdecl One_Weapon_Space_Without_M_Level_(
        int16_t, int16_t, int16_t, int16_t) {
        return 0;
    }
}

namespace struct_ {
    int16_t __cdecl Test_Bit_Field_(void* bit_field, uint16_t bit_index) {
        const uint8_t* bytes = static_cast<const uint8_t*>(bit_field);
        return (bytes[bit_index / 8] >> (bit_index % 8)) & 1;
    }

    void __cdecl Clear_Bit_Field_(void* bit_field, uint16_t bit_index) {
        uint8_t* bytes = static_cast<uint8_t*>(bit_field);
        bytes[bit_index / 8] &= (uint8_t)~(1u << (bit_index % 8));
    }
}

namespace COMBINIT {
    int16_t __cdecl Does_Combat_Ship_Have_Special_(
        int16_t ship_idx, int16_t special_id) {
        uint8_t mask = (uint8_t)(1u << (special_id % 8));
        s_combat_data* ship = &combat_data[ship_idx];
        return (ship->special_device_flags[special_id / 8] & mask) != 0 &&
               (ship->special_device_damage_flags[special_id / 8] & mask) == 0;
    }

    int16_t __cdecl Calc_Combat_Shield_Max_(int16_t) {
        return 0;
    }
}

namespace INITSHIP {
    int16_t __cdecl Get_Ship_Armor_Hits_(int16_t) {
        return 0;
    }
}

namespace palette {
    void __cdecl Refresh_Palette_() {
    }
}

namespace GENDRAW {
    void __cdecl Warning_Box_(const char* message) {
        warning_count++;
        last_warning = message;
    }
}

namespace CMBTDRW1 {
    void __cdecl Refresh_Combat_Screen_Full_() {
        combat_refresh_count++;
    }
}