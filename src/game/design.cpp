#include "deps.h"

namespace DESIGN {
    int16_t __cdecl Cost_Of_Design_(int16_t player_idx, s_ship_design* ship_design) {
        s_current_design local_design;

        struct_::Clear_Structure_(&local_design, sizeof(local_design));

        _design = &local_design;

        // strcpy(local_design.name, ship_design->name)
        char* src = ship_design->name;
        char* dst = local_design.name;
        while (*src) {
            *dst++ = *src++;
        }
        *dst = '\0';

        local_design.ftl_type = static_cast<int16_t>(ship_design->ftl_type);
        local_design.drive_speed = TECHDATA::_drives[local_design.ftl_type].speed;
        local_design.ship_size = static_cast<int16_t>(ship_design->size);

        local_design.hull_space = INITGAME::Get_Design_Hull_Size_();
        local_design.computer_type = static_cast<int16_t>(ship_design->computer_type);
        local_design.picture_type = static_cast<int16_t>(ship_design->picture_num);
        local_design.armor_type = static_cast<int16_t>(ship_design->armor_type);

        local_design.fuel_type = INITSHIP::Best_Ship_Fuel_(player_idx);

        for (int i = 0; i < 8; ++i) {
            local_design.weapon_type[i] = ship_design->ship_weapon[i].type;
            local_design.weapon_count[i] = static_cast<int16_t>(ship_design->ship_weapon[i].count);
            local_design.weapon_specials[i] = ship_design->ship_weapon[i].specials;
            local_design.weapon_firing_arc[i] = ship_design->ship_weapon[i].firing_arc;
            local_design.weapon_ammo[i] = ship_design->ship_weapon[i].ammo;
        }

        for (int i = 0; i < 8; ++i) {
            local_design.special_devices[i] = 0;
            local_design.field102_0xc5[i] = 0;
            local_design.field103_0xd5[i] = 0;
        }

        int16_t special_idx = 0;
        bool stop_search = false;
        for (uint32_t i = 0; i < 0x27 && !stop_search; ++i) {
            if (struct_::Test_Bit_Field_(ship_design->special_device_flags, static_cast<uint16_t>(i))) {
                local_design.special_devices[special_idx] = static_cast<int16_t>(i);
                special_idx++;
                if (special_idx >= 8) {
                    stop_search = true;
                }
            }
        }

        local_design.base_combat_speed = Get_Base_Combat_Speed_(_design);
        Update_Calculated_Design_Data_(player_idx);

        return local_design.field111_0xf1;
    }

    int8_t __cdecl Get_Base_Combat_Speed_(s_current_design* design) {
        uint32_t result = Current_Design_Base_Combat_Speed_(design);
        int8_t speed = static_cast<int8_t>(result & 0xFF);

        if (Current_Design_Has_Special_System_(design, SPECIAL_AUGMENTED_ENGINES) != 0) {
            if (_use_augmented_engines != 0) {
                speed += 5;
            }
        }

        return speed;
    }
}