#include "pch.h"

namespace KEN {
    void __cdecl Ken_Init_() {
    }

    uint16_t __cdecl Get_Weighted_Choice_(uint16_t* weights, int16_t count) {
        uint32_t sum;

        while (true) {
            sum = 0;

            if (count <= 0) {
                break;
            }

            for (int32_t i = 0; i < count; ++i) {
                sum += weights[i];
                if (sum >= 0x200) {
                    break;
                }
            }

            if (sum >= 0x200) {
                for (int32_t j = 0; j < count; ++j) {
                    weights[j] >>= 1;
                }
                continue;
            }

            break;
        }

        if (sum == 0) {
            return 0;
        }

        int32_t rem = random::Random_(sum) - weights[0];
        uint16_t idx = 0;

        if (rem > 0) {
            while (idx < count - 1) {
                idx++;
                rem -= weights[idx];
                if (rem <= 0) {
                    break;
                }
            }
        }

        return idx;
    }

    void __cdecl Load_Player_Ship_Palette_(int16_t player_idx) {
        s_animation_header* palette_anim; // BUG: uninitialized pointer used for player_idx > 14

        if (player_idx < 8) {
            palette_anim = (s_animation_header*)buffer::Buffer_Reload_(
                s_ships_lbx_005597b4,
                static_cast<uint8_t>(MOX::_player[player_idx].color) * 50 + 49,
                MOX::_global_cache_seg
            );
        } else {
            switch (player_idx) {
                case 8:
                    palette_anim = (s_animation_header*)buffer::Buffer_Reload_(s_ships_lbx_005597b4, 413, MOX::_global_cache_seg);
                    break;
                case 9:
                    palette_anim = (s_animation_header*)buffer::Buffer_Reload_(s_ships_lbx_005597b4, 419, MOX::_global_cache_seg);
                    break;
                case 10:
                    palette_anim = (s_animation_header*)buffer::Buffer_Reload_(s_ships_lbx_005597b4, 417, MOX::_global_cache_seg);
                    break;
                case 11:
                    palette_anim = (s_animation_header*)buffer::Buffer_Reload_(s_ships_lbx_005597b4, 416, MOX::_global_cache_seg);
                    break;
                case 12:
                    palette_anim = (s_animation_header*)buffer::Buffer_Reload_(s_ships_lbx_005597b4, 414, MOX::_global_cache_seg);
                    break;
                case 13:
                    palette_anim = (s_animation_header*)buffer::Buffer_Reload_(s_ships_lbx_005597b4, 415, MOX::_global_cache_seg);
                    break;
                case 14:
                    palette_anim = (s_animation_header*)buffer::Buffer_Reload_(s_ships_lbx_005597b4, 418, MOX::_global_cache_seg);
                    break;
            }
        }

        animate::Draw_Palette_(palette_anim);
        palette::Refresh_Palette_();
        MOX::_reloaded_ship_palette = 1;
    }

    void __cdecl Ken_Init2_() {
    }

    void __cdecl Ken_Get_Text_Message_(int32_t msg_index, char* buffer, int16_t param_3) {
        if (msg_index >= 0x50) {
            JIM::Get_Text_Message_(s_KENTEXT1_LBX_005597cc, static_cast<int16_t>(msg_index - 0x50), buffer, param_3);
        } else {
            JIM::Get_Text_Message_(s_KENTEXT_LBX_005597c0, static_cast<int16_t>(msg_index), buffer, param_3);
        }
    }

    void __cdecl Init_NPC_Personalities_Objectives_Themes_() {
        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int i = 0; i < MOX::_NUM_PLAYERS; i++) {
            int difficulty_val = random::Random_(10) - MOX::_settings.game_difficulty + 1;
            if (difficulty_val < 0) {
                difficulty_val = 0;
            } else if (difficulty_val > 9) {
                difficulty_val = 9;
            }

            MOX::_player[i].personality = _personalities[MOX::_player[i].race * 10 + difficulty_val];

            int obj_w[6];
            int des_w[4];
            int spec_w[7];

            obj_w[0] = 1;
            obj_w[2] = 1;
            obj_w[5] = 1;
            des_w[1] = 1;

            obj_w[1] = 2;
            obj_w[3] = 2;
            obj_w[4] = 2;
            des_w[0] = 2;
            des_w[2] = 2;
            spec_w[0] = 2;
            spec_w[1] = 2;
            spec_w[3] = 2;

            des_w[3] = 1;
            spec_w[5] = 2;
            spec_w[2] = 1;
            spec_w[4] = 1;
            spec_w[6] = 3;

            int8_t* traits = MOX::_player[i].traits;

            if (traits[1] == 50) {
                obj_w[5] = 11;
                obj_w[2] = 11;
            }
            if (traits[1] == 100) {
                obj_w[5] += 100;
                obj_w[2] += 100;
            }
            if (traits[2] == 1) {
                obj_w[5] += 3;
                obj_w[4] = 5;
                obj_w[3] = 5;
            }
            if (traits[2] == 2) {
                obj_w[5] += 100;
                obj_w[4] += 100;
                obj_w[3] += 100;
            }
            if (traits[3] == 1) {
                obj_w[5] += 3;
                obj_w[4] += 3;
                obj_w[3] += 3;
            }
            if (traits[3] == 2) {
                obj_w[5] += 10;
                obj_w[4] += 10;
                obj_w[3] += 10;
            }
            if (traits[4] == 1) {
                obj_w[4] += 3;
                obj_w[3] += 10;
            }
            if (traits[4] == 2) {
                obj_w[4] += 10;
                obj_w[3] += 100;
            }
            if (traits[6] == 20) {
                obj_w[1] = 12;
                spec_w[3] = 102;
                des_w[1] = 11;
            }
            if (traits[6] == 40) {
                obj_w[1] += 100;
                spec_w[3] += 1000;
                des_w[1] += 100;
            }
            if (traits[7] == 25) {
                spec_w[4] = 4;
                des_w[0] = 102;
                obj_w[1] += 10;
            }
            if (traits[7] == 50) {
                obj_w[1] += 100;
                spec_w[4] += 10;
                des_w[0] += 1000;
            }
            if (traits[8] == 10) {
                spec_w[1] = 102;
                obj_w[2] += 10;
            }
            if (traits[8] == 20) {
                obj_w[2] += 100;
                spec_w[1] += 1000;
            }
            if (traits[9] == 10) {
                obj_w[0] = 4;
            }
            if (traits[9] == 20) {
                obj_w[0] += 10;
            }
            if (traits[10] == 1) {
                obj_w[4] += 10;
                obj_w[5] += 3;
                obj_w[3] += 3;
            }
            if (traits[11] == 1) {
                obj_w[2] += 10;
                spec_w[1] += 1000;
            }
            if (traits[12] == 1) {
                obj_w[4] += 3;
                obj_w[5] += 3;
                obj_w[3] += 3;
            }
            if (traits[13] == 1) {
                obj_w[4] += 10;
                obj_w[5] += 3;
                obj_w[3] += 3;
            }
            if (traits[14] == 1) {
                obj_w[4] += 3;
                obj_w[5] += 10;
            }
            if (traits[15] == 1) {
                obj_w[4] += 10;
            }
            if (traits[17] == 1) {
                obj_w[4] += 3;
                des_w[0] += 10;
                spec_w[5] = 1002;
            }
            if (traits[18] == 1) {
                obj_w[4] += 3;
                obj_w[2] += 10;
                obj_w[3] += 3;
            }
            if (traits[20] == 1) {
                obj_w[0] += 100;
            }
            if (traits[22] == 1) {
                obj_w[3] += 1000;
            }
            if (traits[23] == 1) {
                obj_w[4] += 100;
            }
            if (traits[24] == 1) {
                obj_w[0] += 100;
            }
            if (traits[28] == 1) {
                obj_w[1] += 100;
            }
            if (traits[30] == 1) {
                obj_w[2] += 10;
            }
            if (traits[29] == 1) {
                obj_w[2] += 10;
                obj_w[1] += 13;
                spec_w[3] += 100;
            }

            if (MOX::_settings.game_difficulty == 3) {
                obj_w[4] += 3;
                obj_w[2] += 3;
                obj_w[3] += 3;
            }
            if (MOX::_settings.game_difficulty == 4) {
                obj_w[4] += 10;
                obj_w[2] += 10;
                obj_w[3] += 10;
            }
            if (MOX::_player[i].personality == 0) {
                spec_w[2] = 4;
            }

            // Calculate objectives
            int total_obj = 0;
            for (int j = 0; j < 6; j++) {
                total_obj += obj_w[j];
            }
            while (total_obj > 1000) {
                total_obj = 0;
                for (int j = 0; j < 6; j++) {
                    obj_w[j] /= 2;
                    total_obj += obj_w[j];
                }
            }
            int rand_obj = random::Random_(total_obj);
            int selected_obj = 0;
            rand_obj -= obj_w[0];
            if (rand_obj > 0) {
                for (int j = 1; j < 6; j++) {
                    selected_obj++;
                    rand_obj -= obj_w[j];
                    if (rand_obj <= 0) break;
                }
            }

            uint8_t prev_obj = MOX::_player[i].objectives;
            MOX::_player[i].objectives = selected_obj;
            if (prev_obj == 100) {
                MOX::_player[i].objectives = 100;
            }

            // Calculate ship design theme
            int total_des = 0;
            for (int j = 0; j < 4; j++) {
                total_des += des_w[j];
            }
            while (total_des > 1000) {
                total_des = 0;
                for (int j = 0; j < 4; j++) {
                    des_w[j] /= 2;
                    total_des += des_w[j];
                }
            }
            int rand_des = random::Random_(total_des);
            int selected_des = 0;
            rand_des -= des_w[0];
            if (rand_des > 0) {
                for (int j = 1; j < 4; j++) {
                    selected_des++;
                    rand_des -= des_w[j];
                    if (rand_des <= 0) break;
                }
            }
            MOX::_player[i].ship_design_theme = selected_des;

            // Calculate ship special theme
            int total_spec = 0;
            for (int j = 0; j < 7; j++) {
                total_spec += spec_w[j];
            }
            while (total_spec > 1000) {
                total_spec = 0;
                for (int j = 0; j < 7; j++) {
                    spec_w[j] /= 2;
                    total_spec += spec_w[j];
                }
            }
            int rand_spec = random::Random_(total_spec);
            int selected_spec = 0;
            rand_spec -= spec_w[0];
            if (rand_spec > 0) {
                for (int j = 1; j < 7; j++) {
                    selected_spec++;
                    rand_spec -= spec_w[j];
                    if (rand_spec <= 0) break;
                }
            }
            MOX::_player[i].ship_special_theme = selected_spec;
        }
    }

    s_animation_header* __cdecl Get_Ship_Picture_Seg_(int16_t param_1, int16_t param_2) {
        return Do_Get_Ship_Picture_Seg(param_1, param_2);
    }

    void __cdecl Get_Derelict_Applications_(int16_t player_idx, uint16_t* app_array, int16_t* app_count) {
        int16_t best_shield_idx = 0;
        int16_t max_weapon_idx = 3;
        int16_t min_diff = 200;
        int16_t best_weapon_idx = 0;

        *app_count = 0;

        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                int16_t shield_idx = INITSHIP::Best_Ship_Shield_(i);
                if (best_shield_idx < shield_idx) {
                    best_shield_idx = shield_idx;
                }

                int16_t weapon_idx = INITSHIP::Best_Beam_Weapon_(i);
                if (static_cast<int16_t>(TECHDATA::_weapons[max_weapon_idx].max_damage) < static_cast<int16_t>(TECHDATA::_weapons[weapon_idx].max_damage)) {
                    max_weapon_idx = weapon_idx;
                }
            }
        }

        uint16_t shield_app_id = TECHDATA::_shields[best_shield_idx].tech_app_id;

        if (TECHDATA::_technology_applications[TECHDATA::_weapons[max_weapon_idx].tech_app_id].tech_field_id == 0x4A) {
            max_weapon_idx = 10;
        }

        for (int16_t i = 1; i < 40; ++i) {
            if (TECHDATA::_weapons[i].type == 0 &&
                TECHDATA::_technology_applications[TECHDATA::_weapons[i].tech_app_id].tech_field_id != 0x4A) {

                int16_t diff = static_cast<int16_t>(TECHDATA::_weapons[i].max_damage) - static_cast<int16_t>(TECHDATA::_weapons[max_weapon_idx].max_damage);

                if (diff >= 0 && diff <= min_diff && diff <= 50) {
                    if (MOX::_player[player_idx].tech_applications[TECHDATA::_weapons[i].tech_app_id] != 3) {
                        best_weapon_idx = i;
                        if (i == max_weapon_idx) {
                            min_diff = 1;
                        } else {
                            min_diff = diff;
                        }
                    }
                }
            }
        }

        if (min_diff < 200) {
            app_array[*app_count] = TECHDATA::_weapons[best_weapon_idx].tech_app_id;
            (*app_count)++;
        }

        if (MOX::_player[player_idx].tech_applications[shield_app_id] != 3) {
            app_array[*app_count] = shield_app_id;
            (*app_count)++;
        }
    }

    s_animation_header* __cdecl Get_Ship_Id_Picture_Seg_(int16_t ship_idx) {
        return Do_Get_Ship_Picture_Seg(
            MOX::_ship[ship_idx].d.previous_owner,
            MOX::_ship[ship_idx].d.picture_num
        );
    }

    s_animation_header* __cdecl Do_Get_Ship_Picture_Seg(int16_t player_idx, int16_t ship_type) {
        int32_t color = 8;
        if (player_idx < 8) {
            color = MOX::_player[player_idx].color;
        }

        return (s_animation_header*)buffer::Buffer_Reload_(s_ships_lbx_005597b4, ship_type + (color * 50), MOX::_global_cache_seg);
    }
}