#include "deps.h"

namespace INITSHIP {
    void __cdecl Init_Ship_Designs_(int16_t player_idx) {
        s_player* player = &MOX::_player[player_idx];
        int16_t skip_offset;

        if (MOX::_settings.starting_civilization_level == 2 || player->objectives != 100 /* 'd' */) {
            skip_offset = 0;
        } else {
            skip_offset = 1;
        }

        if (MOX::_settings.strategic_combat_flag == 1) {
            for (int16_t i = 0; i < 6; ++i) {
                struct_::Clear_Structure_(&player->ship_designs[i], 99);
                aidesign::Auto_Strategic_Design_Ship_(player_idx, i, player->ship_designs[i].name);
            }
            return;
        }

        MOX::_cur_combat_x = -1000;
        aidesign::Auto_Design_Ship_(player_idx, player->ship_designs[0].name, 0);
        MOX::_cur_combat_x = 0;

        for (int16_t i = 1; i < 5; ++i) {
            aidesign::Auto_Design_Ship_(player_idx, player->ship_designs[i].name, i - skip_offset);
        }

        if (player->objectives == 100 /* 'd' */) {
            struct_::Clear_Structure_(&player->ship_designs[5], 99);
        } else {
            aidesign::Auto_Design_Ship_(player_idx, player->ship_designs[5].name, 5);

            int16_t picture_mod = (int16_t)(player->ship_designs[5].picture_num % 8);
            Set_Default_Ship_Name((uint16_t)(uint8_t)player->race,
                         (int16_t)player->ship_designs[5].size,
                         picture_mod,
                         player->ship_designs[5].name);
        }
    }

    void __cdecl Update_Player_Ship_Designs_(int16_t player_idx) {
        if (MOX::_settings.strategic_combat_flag == 1) {
            Update_Player_Strategic_Ship_Designs_(player_idx);
            return;
        }

        if (MOX::_player[player_idx].objectives == 100 /* 'd' */) {
            Update_Automatic_Design_Equipment_(player_idx);
            Update_Equipment_On_Ships_Being_Built_(player_idx);
            return;
        }

        Update_Equipment_On_Ships_Being_Built_(player_idx);

        for (int16_t i = 0; i < 5; ++i) {
            aidesign::Auto_Strategic_Design_Ship_(player_idx, i, MOX::_player[player_idx].ship_designs[i].name);
        }
    }

    int16_t __cdecl Best_Ship_Fuel_(int16_t player_idx) {
        if (player_idx >= 8) {
            return 5;
        }

        int16_t best_fuel_idx = 0;
        int16_t best_range = TECHDATA::_fuel[0].ship_range;

        for (int16_t i = 0; i < 6; ++i) {
            // Check if player has researched this fuel technology (application value 3)
            // and if it provides a better range than the current best.
            if (MOX::_player[player_idx].tech_applications[TECHDATA::_fuel[i].tech_app] == 3) {
                if (TECHDATA::_fuel[i].ship_range > best_range) {
                    best_range = TECHDATA::_fuel[i].ship_range;
                    best_fuel_idx = i;
                }
            }
        }

        return best_fuel_idx;
    }

    int16_t __cdecl Best_Warp_Drive_(int16_t player_idx) {
        if (player_idx > 7) {
            // Handle special players (Guardians, Antarans, etc.)
            switch (player_idx) {
                case 8:  // Antarans
                case 12:
                case 13:
                    return 6;
                case 9:
                case 11:
                    return 4;
                case 10:
                case 14:
                    return 2;
                default:
                    return 0;
            }
        }

        int16_t best_warp_index = 0;
        // Search through warp drive techs 1-6 (Nuclear to Interdimensional)
        for (int16_t i = 1; i < 7; ++i) {
            int16_t app_idx = TECHDATA::_drives[i].tech_app;
            // Status 3 indicates that the technology has been researched
            if (MOX::_player[player_idx].tech_applications[app_idx] == 3) {
                best_warp_index = i;
            }
        }

        return best_warp_index;
    }
}