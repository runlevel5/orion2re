#include "pch.h"

namespace {
    s_combat_data combat_data[2];
    s_current_design current_design;
}

namespace TEST_SHIELD_TABLE {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(&current_design, 0, sizeof(current_design));
        MOX::_combat_data = combat_data;
        MOX::_design = &current_design;
        MOX::_in_nebula_flag = 0;
        COMBINIT::_satellite_size_class_plus_one = 0;

        const uint16_t multipliers[HULL_CLASS_COUNT] = {
            5, 10, 15, 20, 25, 35, 15, 20, 25
        };
        memcpy(TECHDATA::_shield_hull_multiplier, multipliers,
               sizeof(multipliers));
        const uint16_t shield_classes[SHIELD_COUNT] = {0, 1, 3, 5, 7, 10};
        for (int i = 0; i < SHIELD_COUNT; ++i) {
            TECHDATA::_shields[i].shield_class = shield_classes[i];
        }
        TECHDATA::_hard_shields_damage_reduction = 3;
    }

    s_combat_data* Combat_Ship_(int16_t ship_idx) {
        return &combat_data[ship_idx];
    }

    s_current_design* Design_() {
        return &current_design;
    }

    void Set_Combat_Special_(int16_t special_id) {
        combat_data[0].special_device_flags[special_id / 8] |=
            (uint8_t)(1u << (special_id % 8));
    }

    void Set_Design_Special_(int16_t special_id) {
        for (int i = 0; i < 8; ++i) {
            if (current_design.special_devices[i] == 0) {
                current_design.special_devices[i] = special_id;
                return;
            }
        }
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    s_current_design* _design = &current_design;
    int16_t _in_nebula_flag;
}

namespace struct_ {
    int16_t __cdecl Test_Bit_Field_(void* bit_field, uint16_t bit_index) {
        const uint8_t* bytes = static_cast<const uint8_t*>(bit_field);
        return (bytes[bit_index / 8] >> (bit_index % 8)) & 1;
    }
}