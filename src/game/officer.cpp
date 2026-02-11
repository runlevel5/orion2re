#include "deps.h"
#include <cmath>
#include <cstdio>
#include <cstring>

namespace OFFICER {
    int16_t __cdecl Get_Officer_Level_(int16_t leader_idx) {
        int16_t current_xp = MOX::_leaders[leader_idx].xp;

        int16_t level = Get_Officer_Base_Level_(current_xp, 0);

        if (level > 5) {
            level = 5;
        }

        return level;
    }

    int16_t __cdecl Get_Officer_Base_Level_(int16_t xp, int8_t warlord) {
        if (xp < 60) {
            return 0;
        }
        if (xp < 150) {
            return 1;
        }
        if (xp < 300) {
            return 2;
        }
        if (xp < 500) {
            return 3;
        }
        if (xp < 1000) {
            return 4;
        }
        return (warlord != 0) ? 5 : 4;
    }

    int32_t __cdecl Officer_Skill_Bonus_(uint32_t param_1, int16_t leader_idx, int16_t skill_idx) {
        int16_t level;

        if (MOX::_leaders[leader_idx].player_index == -1) {
            level = Get_Officer_Level_(leader_idx);
        } else {
            level = Owned_Officer_Level_(static_cast<int32_t>(leader_idx));
        }

        int32_t current_level_val = static_cast<int32_t>(level) + 1;
        int32_t skill_div = static_cast<int32_t>(MOX::_skill_data[skill_idx].unknown2);
        int32_t bonus_base = static_cast<int32_t>(MOX::_skill_data[skill_idx].skill_bonus);

        int32_t intermediate = (current_level_val + skill_div - 1) / skill_div;

        return (intermediate * bonus_base) / 10;
    }

    int16_t __cdecl Owned_Officer_Level_(int leader_idx) {
        int8_t warlord_trait = 0;

        // leader_idx 0x42 (66) likely refers to a special entity or unassigned leader
        if (leader_idx != 0x42) {
            int8_t player_idx = MOX::_leaders[leader_idx].player_index;
            warlord_trait = MOX::_player[player_idx].traits[TRAIT_WARLORD];
        }

        int16_t level = OFFICER::Get_Officer_Base_Level_(MOX::_leaders[leader_idx].xp, warlord_trait);

        // Cap the maximum level at 5
        if (level > 5) {
            level = 5;
        }

        return level;
    }

    int16_t __cdecl Officer_Skill_Value_(int16_t leader_index) {
        int16_t total_value = 0;
        s_leader_data& leader = MOX::_leaders[leader_index];

        for (int16_t skill_idx = 0; skill_idx < 54; ++skill_idx) {
            s_skill_data& skill = MOX::_skill_data[skill_idx];

            if (skill.unknown1 == 0) {
                if (OFFICER::Officer_Has_General_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                    int16_t val;
                    if (skill_idx == 2 || skill_idx == 3) {
                        // type 0 (Ship Officer) gets 3, type 1 (Colony Leader) gets 1
                        val = (leader.type == 0) ? 3 : 1;
                        if (skill_idx == 3) {
                            val *= 2;
                        }
                    } else {
                        val = (int8_t)skill.unknown3;
                    }
                    total_value += val;
                }
            }

            if (leader.type == 0) { // Ship Officer
                if (skill.unknown1 == 1) {
                    if (OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                        total_value += (int8_t)skill.unknown3;
                    }
                }
            } else if (leader.type == 1) { // Colony Leader
                if (skill.unknown1 == 2) {
                    if (OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                        total_value += (int8_t)skill.unknown3;
                    }
                }
            }
        }

        for (int i = 0; i < 3; ++i) {
            uint8_t app_idx = leader.tech_application[i];
            if (app_idx != 0) {
                int16_t field_id = TECHDATA::_technology_applications[app_idx].tech_field_id;
                if (field_id < 0) {
                    // Logic from disassembly: resets total_value to 1 if field_id is invalid/negative
                    total_value = 1;
                } else {
                    total_value += OFFICER::Get_Research_Cost_Skill_Value_(TECHDATA::_technology_fields[field_id].cost);
                }
            }
        }

        return total_value;
    }

    void __cdecl Get_Random_Skills_(uint32_t* primary_mask, uint32_t* secondary_mask, int16_t skill_class, int16_t count, int8_t randomize_class) {
        *primary_mask = 0;
        *secondary_mask = 0;

        if (count <= 0) {
            return;
        }

        int16_t remaining_count = count;
        do {
            int16_t selected_idx = 0;
            int16_t options_found = 0;
            int16_t current_class = skill_class;

            if (randomize_class != 0) {
                if (random::Random_(7) < 3) {
                    current_class = 0;
                }
            }

            for (int j = 0; j < 54; j += 2) {
                if (current_class == 0) {
                    // Search for secondary skills
                    if (MOX::_skill_data[j].unknown1 == 0 &&
                        ((*secondary_mask & MOX::_skill_data[j].skill_mask) == 0) &&
                        ((*secondary_mask & MOX::_skill_data[j + 1].skill_mask) == 0)) 
                    {
                        options_found++;
                        if (random::Random_(options_found) == 1) {
                            if (random::Random_(2) == 1) {
                                selected_idx = (int16_t)j;
                            } else {
                                selected_idx = (int16_t)(j + 1);
                            }
                        }
                    }
                } else {
                    // Search for primary skills of specific class
                    if ((int16_t)MOX::_skill_data[j].unknown1 == skill_class &&
                        ((*primary_mask & MOX::_skill_data[j].skill_mask) == 0) &&
                        ((*primary_mask & MOX::_skill_data[j + 1].skill_mask) == 0)) 
                    {
                        options_found++;
                        if (random::Random_(options_found) == 1) {
                            if (random::Random_(2) == 1) {
                                selected_idx = (int16_t)j;
                            } else {
                                selected_idx = (int16_t)(j + 1);
                            }
                        }
                    }
                }
            }

            if (current_class == 0) {
                *secondary_mask += MOX::_skill_data[selected_idx].skill_mask;
            } else {
                *primary_mask += MOX::_skill_data[selected_idx].skill_mask;
            }

            remaining_count--;
        } while (remaining_count != 0);
    }

    bool __cdecl One_Leader_With_Galactic_Lore_(int16_t player_idx) {
        return (bool)MOX::_player[player_idx].has_galactic_lore;
    }

    bool __cdecl Officer_Has_General_Skill_(int16_t player_num, int16_t leader_index, int16_t skill_id) {
        return (MOX::_skill_data[skill_id].skill_mask & MOX::_leaders[leader_index].general_skills) != 0;
    }

    bool __cdecl Officer_Has_Special_Skill_(int16_t player_num, int16_t leader_idx, int16_t skill_idx) {
        uint32_t mask = MOX::_skill_data[skill_idx].skill_mask;
        uint32_t leader_skills = MOX::_leaders[leader_idx].special_skills;

        return (mask & leader_skills) != 0;
    }

    int16_t __cdecl Get_Research_Cost_Skill_Value_(uint32_t cost) {
        if (cost < 1000) {
            return 1;
        }

        if (cost >= 5000) {
            return 4;
        }

        return 3;
    }

    /**
     * Increases an officer's experience to the minimum required for the next level.
     * The experience thresholds are 60, 150, 300, 500, and 1000 for levels 1 through 5.
     * If the officer is already at the maximum level (5), no change is made.
     */
    void __cdecl Increase_Officer_Level_By_One_(int16_t leader_index) {
        // Experience thresholds for levels 1, 2, 3, 4, 5
        const int16_t level_thresholds[] = { 60, 150, 300, 500, 1000 };

        s_leader_data& leader = MOX::_leaders[leader_index];
        int8_t warlord = 0;

        // Check if the officer's owner has the Warlord trait
        if (leader.player_index != -1) {
            if (MOX::_player[(uint8_t)leader.player_index].traits[TRAIT_WARLORD] != 0) {
                warlord = 1;
            }
        }

        // Calculate the current level based on XP and Warlord status
        int16_t current_level = Get_Officer_Base_Level_(leader.xp, warlord);

        // Increase level if not already at max (level 5 / Elite)
        if (current_level >= 0 && current_level < 5) {
            leader.xp = level_thresholds[current_level];
        }
    }

    void __cdecl Deassign_Officer_(int16_t leader_idx) {
        s_leader_data& leader = MOX::_leaders[leader_idx];
        int16_t player_idx = (int16_t)leader.player_index;

        OFFICER::Increase_Officer_Level_By_One_(leader_idx);

        if (leader.type == 1) { // Colony Administrator
            int16_t star_idx = leader.location;
            if (star_idx != -1) {
                if (leader.status == 5) { // Active/Assigned status
                    MOX::_star[star_idx].system_special = 0;
                }
                if (player_idx != -1 && MOX::_star[star_idx].officer_index[player_idx] == (int8_t)leader_idx) {
                    MOX::_star[star_idx].officer_index[player_idx] = -1;
                }
            }
        } else if (leader.type == 0 && player_idx != -1) { // Ship Officer
            int16_t ship_idx = leader.location;
            if (ship_idx != -1) {
                if (HACCESS::Ship_Owner_(ship_idx) == player_idx && MOX::_ship[ship_idx].officer_index == leader_idx) {
                    MOX::_ship[ship_idx].officer_index = -1;
                }
            }
        }

        // Reset leader state to unassigned
        leader.eta = 0;
        leader.status = -1;
        leader.location = -1;
        leader.player_index = -1;
    }

    void __cdecl Allocate_Adv_Civ_Game_Officers_() {
        int16_t leader_indices[MAX_LEADERS];
        uint8_t is_leader_assigned[MAX_LEADERS];

        for (int i = 0; i < MAX_LEADERS; ++i) {
            is_leader_assigned[i] = 0;
        }

        // Phase 1: Assign one leader to each Repulsive player from a shuffled pool of 10
        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            leader_indices[i] = i;
        }

        for (int i = 0; i < 10; ++i) {
            int16_t idx1 = (int16_t)random::Random_(10) - 1;
            int16_t idx2 = (int16_t)random::Random_(10) - 1;
            if (idx1 != idx2) {
                int16_t tmp = leader_indices[idx1];
                leader_indices[idx1] = leader_indices[idx2];
                leader_indices[idx2] = tmp;
            }
        }

        int16_t repulsive_pool_idx = 0;
        for (int16_t player_idx = 0; player_idx < (int16_t)MOX::_NUM_PLAYERS; ++player_idx) {
            if (MOX::_player[player_idx].traits[TRAIT_REPULSIVE] != 0) {
                int16_t leader_idx = leader_indices[repulsive_pool_idx++];
                if (leader_idx >= 0) {
                    MOX::_leaders[leader_idx].status = 4;
                    MOX::_leaders[leader_idx].player_index = (char)player_idx;
                    is_leader_assigned[leader_idx] = 1;
                }
            }
        }

        // Phase 2: Assign two leaders to each non-Repulsive player from a shuffled pool of 25
        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            leader_indices[i] = i;
        }

        for (int i = 0; i < 25; ++i) {
            int16_t idx1 = (int16_t)random::Random_(25) - 1;
            int16_t idx2 = (int16_t)random::Random_(25) - 1;
            if (idx1 != idx2) {
                int16_t tmp = leader_indices[idx1];
                leader_indices[idx1] = leader_indices[idx2];
                leader_indices[idx2] = tmp;
            }
        }

        for (int16_t player_idx = 0; player_idx < (int16_t)MOX::_NUM_PLAYERS; ++player_idx) {
            if (MOX::_player[player_idx].traits[TRAIT_REPULSIVE] == 0) {
                for (int count = 0; count < 2; ++count) {
                    int16_t pool_idx = 0;
                    int16_t leader_idx;
                    while (true) {
                        leader_idx = leader_indices[pool_idx];
                        if (is_leader_assigned[leader_idx] == 0) break;
                        pool_idx++;
                    }

                    if (leader_idx >= 0) {
                        MOX::_leaders[leader_idx].status = 4;
                        MOX::_leaders[leader_idx].player_index = (char)player_idx;
                        is_leader_assigned[leader_idx] = 1;
                    }
                }
            }
        }
    }

    int32_t __cdecl Officer_Cost_(int16_t player_idx, int16_t leader_idx) {
        uint16_t level_data = Get_Officer_Level_(leader_idx);
        int16_t level_multiplier = (int16_t)((level_data & 0xFF) + 1);

        int16_t base_calc = MOX::_leaders[leader_idx].skill_value * level_multiplier;
        int16_t bonus = Bonus_For_Famous_Heroes_(player_idx);

        int16_t total_cost = base_calc * 10;

        if (Officer_Has_General_Skill_(player_idx, leader_idx, 28) ||
            Officer_Has_General_Skill_(player_idx, leader_idx, 29)) {
            total_cost = base_calc * 20;
        }

        int32_t final_cost = (int32_t)total_cost - (int32_t)bonus;

        if (final_cost < 1) {
            return 0;
        }

        return final_cost;
    }

    int16_t __cdecl Officer_Maintenance_(int16_t player_num, int16_t leader_idx) {
        int32_t cost = OFFICER::Officer_Cost_(player_num, leader_idx);

        int16_t maintenance_cost = (int16_t)((cost + 99) / 100);

        bool has_cost_reduction_skill = OFFICER::Officer_Has_General_Skill_(player_num, leader_idx, 28) ||
                                         OFFICER::Officer_Has_General_Skill_(player_num, leader_idx, 29);

        if (leader_idx != 65 && !has_cost_reduction_skill) {
            if (maintenance_cost < 1) {
                maintenance_cost = 1;
            }
            return maintenance_cost;
        }

        return 0;
    }

    bool Is_Fleet_Officer_Screen_() {
        return MOX::_officer_scrn_type == 0;
    }

    void __cdecl Check_For_Colony_Calculation_Update_() {
        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            s_leader_data& leader = MOX::_leaders[i];

            if (leader.type == 1) {
                int16_t original_star_id = MOX::_officers_original_star_id[i];

                if (leader.location != original_star_id && leader.status == 0) {
                    COLCALC::Star_Colony_Calculation_(original_star_id, MOX::_PLAYER_NUM);
                }
            }
        }
    }

    void __cdecl Move_From_Limbo_To_Pool_() {
        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            if (MOX::_leaders[i].status == 2) {
                MOX::_leaders[i].status = 0;
                MOX::_leaders[i].eta = 0;
            }
        }
    }

    void __cdecl Set_Original_Stars_() {
        for (int32_t i = 0; i < MAX_LEADERS; ++i) {
            MOX::_officers_original_star_id[i] = MOX::_leaders[i].location;
        }
    }

    void __cdecl Set_Original_Ships_() {
        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            MOX::_officers_original_ship_id[i] = MOX::_leaders[i].location;
        }
    }

    int16_t __cdecl Bonus_For_Famous_Heroes_(int16_t player_idx) {
        int16_t max_bonus = 0;
        int16_t current_skill_bonus = 0;

        for (int16_t leader_idx = 0; leader_idx < MAX_LEADERS; ++leader_idx) {
            auto& leader = MOX::_leaders[leader_idx];

            // Check if the leader belongs to the player and is in a valid status (0 or 1)
            if (leader.player_index == player_idx && (leader.status == 0 || leader.status == 1)) {

                bool has_famous = Officer_Has_General_Skill_(player_idx, leader_idx, 10 /* FAMOUS */);
                if (has_famous) {
                    current_skill_bonus = (int16_t)Officer_Skill_Bonus_(player_idx, leader_idx, 10 /* FAMOUS */);
                } else {
                    bool has_super_famous = Officer_Has_General_Skill_(player_idx, leader_idx, 11 /* SUPER_FAMOUS */);
                    if (has_super_famous) {
                        current_skill_bonus = (int16_t)Officer_Skill_Bonus_(player_idx, leader_idx, 11 /* SUPER_FAMOUS */);
                    }
                }

                // Track the maximum bonus found across all valid leaders
                if (current_skill_bonus > max_bonus) {
                    max_bonus = current_skill_bonus;
                }
            }
        }

        return max_bonus;
    }

    void Draw_Officer_Screen_() {
        char buffer[100];
        int16_t hire_cost, maint_cost;
        bool is_fleet_screen = Is_Fleet_Officer_Screen_();

        if (MOX::_draw_officer_to_back || MOX::_FULL_DRAW_SCREEN_FLAG != 0 || OFFICER::_full_draw) {
            Draw_Officer_Screen_To_Back_();
        }

        graphics::Reset_Window_();
        video::Clip_Off_();

        if (is_fleet_screen) {
            if (!OFFICER::_offscrn_skip_draw_ships) {
                if (SCROLL::Draw_Generic_Vertical_Scroll_Bar_(MOX::_scroll_bar, reinterpret_cast<void*>(FLT1::Fill_FltScrn_Scroll_Bar_))) {
                    MOX::_scanned_big_ship = -1;
                }
                if (MOX::_small_ship_stack_ptr > -1) {
                    FLT1::Draw_Fltscrn_Big_Ship_Icons_(29);
                }
            } else {
                OFFICER::_offscrn_skip_draw_ships = 0;
            }
        } else {
            Wrapper_For_Offscrn_System_Popup_(OFFICER::_officer_star_displayed);
        }

        MOVEBOX::Draw_Galaxy_Map_Box_(nullptr, 0, 306, 235, 318, 169, 0, 0, 0, 0, 1, 0);
        FLT1::Draw_Fltscrn_Ship_Destination_Lines_(29);
        FLT1::Draw_Fltscrn_Small_Ship_Icons_();

        if (MOX::_scanned_small_ship > -1) {
            Print_Galmap_Scanned_Ship_(MOX::_scanned_small_ship);
        }

        if (MOX::_small_ship_stack_ptr > -1 && is_fleet_screen) {
            FLT1::Draw_Flashing_Small_Ship_Icon_();
        }

        if (MOX::_scanned_big_ship > -1 && is_fleet_screen) {
            Print_Scanned_Ship_Name_();
        }

        for (int16_t i = 0; i < MOX::_n_fltscrn_officers; ++i) {
            if (MOX::_officer_id_list[i] > -1) {
                Wrapper_For_Offscrn_Data_(i, MOX::_officer_id_list[i]);
            }
        }

        if (OFFICER::_officer_button_mode == 2) {
            animate::Set_Animation_Frame_(reinterpret_cast<s_animation_header*>(MOX::_officer_dismiss_button_seg_active), 1);
            animate::Draw_(463, 441, reinterpret_cast<s_animation_header*>(MOX::_officer_dismiss_button_seg_active));
        } else if (OFFICER::_officer_dismiss_btn == -1000) {
            animate::Draw_(463, 441, reinterpret_cast<s_animation_header*>(MOX::_officer_dismiss_button_seg_inactive));
        }

        if (OFFICER::_officer_button_mode == 1) {
            animate::Set_Animation_Frame_(reinterpret_cast<s_animation_header*>(MOX::_officer_pool_button_seg_active), 1);
            animate::Draw_(388, 441, reinterpret_cast<s_animation_header*>(MOX::_officer_pool_button_seg_active));
        } else if (OFFICER::_officer_pool_btn == -1000) {
            animate::Draw_(388, 441, reinterpret_cast<s_animation_header*>(MOX::_officer_pool_button_seg_inactive));
        }

        if (!Leaders_In_Hiring_Pool_()) {
            animate::Draw_(313, 441, reinterpret_cast<s_animation_header*>(MOX::_dull_officer_pool_button_seg));
        }

        if (OFFICER::_officer_button_mode == 0) {
            animate::Draw_(300, 441, reinterpret_cast<s_animation_header*>(MOX::_dull_officer_hire_button_seg));
            if (MOX::_officer_scanned > -1) {
                Get_Officer_Costs_(MOX::_officer_scanned, &hire_cost, &maint_cost);
                if (maint_cost == 0) {
                    _sprintf(buffer, HAROLD::H_Message_(HESTR_129_D_BCS_NO_MAINTENANCE), (int)hire_cost);
                } else {
                    _sprintf(buffer, HAROLD::H_Message_(HESTR_121_D_BCS_AND_D_BC_TURN), (int)hire_cost, (int)maint_cost);
                }
                MISC::Set_Fitted_Font_Style_(buffer, &OFFICER::_normal_text_colors, 4, 2, 0, 132, 1);
                fonts::Print_(315, 450, buffer);
            }
        }

        if (!is_fleet_screen && OFFICER::_officer_star_displayed != -1) {
            int star_idx = OFFICER::_officer_star_displayed;
            int8_t leader_idx = MOX::_star[star_idx].officer_index[MOX::_PLAYER_NUM];
            if (leader_idx == -1) {
                strcpy(buffer, MOX::_star[star_idx].name);
            } else {
                s_leader_data* leader = &MOX::_leaders[leader_idx];
                if (leader->eta < 1) {
                    _sprintf(buffer, MOX::s_fmt_scanned_star_info, MOX::_star[star_idx].name, leader->name);
                } else {
                    int msg_id = (leader->eta == 1) ? HESTR_092_S_S_D_TURN : HESTR_093_S_S_D_TURNS;
                    _sprintf(buffer, HAROLD::H_Message_(msg_id), MOX::_star[star_idx].name, leader->name, (int)leader->eta);
                }
            }
            MISC::Set_Fitted_Font_Style_(buffer, &OFFICER::_normal_text_colors, 3, 2, 0, 203, 1);
            fonts::Print_Centered_(466, 210, buffer);
        }

        if ((char)MOX::_global_var == 'c') {
            void* seg = is_fleet_screen ? MOX::_officer_colony_ldrs_button_seg_fleet : MOX::_officer_colony_ldrs_button_seg_system;
            animate::Set_Animation_Frame_(reinterpret_cast<s_animation_header*>(seg), 1);
            animate::Draw_(is_fleet_screen ? 156 : 10, 11, reinterpret_cast<s_animation_header*>(seg));
        }

        bool type_not_one = MOX::_officer_scrn_type != 1;
        animate::Set_Animation_Frame_(reinterpret_cast<s_animation_header*>(MOX::_officer_colony_ldrs_button_seg_fleet), type_not_one);
        animate::Draw_(160, 10, reinterpret_cast<s_animation_header*>(MOX::_officer_colony_ldrs_button_seg_fleet));

        animate::Set_Animation_Frame_(reinterpret_cast<s_animation_header*>(MOX::_officer_colony_ldrs_button_seg_system), !type_not_one);
        animate::Draw_(7, 10, reinterpret_cast<s_animation_header*>(MOX::_officer_colony_ldrs_button_seg_system));

        OFFICER::_offscrn_flash_incr++;
        if (OFFICER::_offscrn_flash_incr % 25 == 0) {
            OFFICER::_offscrn_flash_dir = -OFFICER::_offscrn_flash_dir;
            OFFICER::_offscrn_flash_incr = 0;
        }
        if (OFFICER::_offscrn_flash_incr % 5 == 0) {
            OFFICER::_offscrn_flash_value += OFFICER::_offscrn_flash_dir;
        }

        MOX::_multi_colored_line_start = (std::abs(MOX::_multi_colored_line_start + 1)) % 8;
    }

    void Officers_Screen_() {
        uint16_t saved_map_scale = (uint16_t)MOX::_cur_map_scale;
        char needs_help_reset = 0;
        int16_t hire_btn, cancel_btn, fleet_view_btn, star_view_btn, ok_btn, scroll_up_btn, scroll_down_btn, prev_btn, next_btn, dummy_field;
        int big_icons_scanned[4], big_icons_ids[4];

        graphics::Reset_Window_();
        video::Clip_Off_();
        fields::Clear_Fields_();

        COLONY::_using_colony_screen_palette = 0;
        _offscrn_flash_incr = 0;
        _offscrn_skip_draw_ships = 0;
        _full_draw = 0;
        _offscrn_flash_value = 2;
        MOX::_n_fltscrn_big_icons_added = 0;
        _offscrn_ask_for_hire_popup = 0;
        MOX::_n_fltscrn_big_icons = 0;
        MOX::_n_fltscrn_rows = 0;
        _offscrn_flash_dir = 1;
        _officer_button_mode = -1;
        _fleet_icon_ship_id_to_move = -1;
        MOX::_small_ship_stack_ptr = -1;
        MOX::_old_small_ship_stack_ptr = -1;
        MOX::_old_first_fltscrn_icon = -1;
        MOX::_galaxy_map_scanned_star = -1;
        MOX::_n_big_icons_selected = 0;
        MOX::_galaxy_star_anim_count = 0;
        MOX::_system_icon_scanned = -1;
        MOX::_scanned_big_ship = -1;
        MOX::_scanned_small_ship = -1;
        MOX::_fltscrn_bar_value = -1;

        SYS::Clear_System_Display_Loaded_Flag_();
        palette::Clear_Palette_Changes_(0, 0xFF);
        ERIC::Fast_Fade_Out_();

        if (MOX::_previous_screen == 1) {
            MOX::_officer_scrn_type = 1;
            if (DESIGN::_player_design_num > -1) {
                MOX::_small_ship_stack_ptr = DESIGN::_player_design_num;
            }
        }
        if (MOX::_officer_scrn_type == -1) {
            MOX::_officer_scrn_type = 0;
        }

        Reload_Officer_Screen_Pictures_();
        SHIPS::Build_Ship_Icons_();
        FLT::Set_Fltscrn_Small_Ship_Icon_XYs_(0x132, 0xEB, 0x13E, 0xA9);

        if (Is_Fleet_Officer_Screen_()) {
            if (MOX::_small_ship_stack_ptr < 0) {
                FLT2::Set_Initial_Stack_Pointer_();
            }
        } else {
            _officer_star_displayed = DESIGN::_player_design_num;
            _officer_star_chosen = DESIGN::_player_design_num;
            if (DESIGN::_player_design_num == -1) {
                _officer_star_chosen = MOX::_planet[MOX::_player[MOX::_PLAYER_NUM].home_planet_id].star_index;
            }
            SYS::Init_System_Display_(DESIGN::_player_design_num);
        }

        MOX::_draw_officer_to_back = 1;
        if (MOX::_small_ship_stack_ptr != -1) {
            if (HACCESS::Ship_Stack_Owner_(MOX::_ship_stack_start[MOX::_small_ship_stack_ptr]) != MOX::_PLAYER_NUM) {
                MOX::_small_ship_stack_ptr = -1;
            }
        }

        fields::Assign_Auto_Function_(Draw_Officer_Screen_, 1);
        MOX::_big_icon_display_columns = 5;
        MOX::_big_icon_display_rows = 3;
        MOX::_big_icon_display_max_icons = 0x0F;
        MOX::_big_icon_x_offset = 0x12E;
        MOX::_big_icon_y_offset = 0x13;

        Add_Officer_Screen_Fields_(&hire_btn, &cancel_btn, &star_view_btn, &fleet_view_btn, &ok_btn, (int)big_icons_scanned, (int)big_icons_ids, &scroll_up_btn, &scroll_down_btn, &prev_btn, &next_btn, &dummy_field);
        FLT1::Initialize_Scroll_Bar_(MOX::_scroll_bar);

        MOX::_offpop_big_icon_selected = -1;
        MOX::_system_star_id = -1;
        MOX::_officer_selected = -1;
        MOX::_drawing_officer_popup = 1;
        MOX::_officer_scanned = -1;

        SYS::Clear_System_Display_Loaded_Flag_();
        Set_Original_Stars_();
        Set_Original_Ships_();
        MOX::_draw_officer_to_back = 1;
        MOX::_global_var = 1;
        MOX::_return_screen = (int16_t)MOX::_previous_screen;
        Fade_Into_Leaders_Screen_();
        MOX::_global_var = 0;

        int16_t exit_screen = 0;
        EVANHELP::Set_Officer_Screen_Help_();

        do {
            bool is_fleet = Is_Fleet_Officer_Screen_();
            timer::Mark_Time_();
            int16_t input = fields::Get_Input_();
            int16_t scanned_id = fields::Scan_Input_();

            if (cancel_btn == input) {
                mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
                MOX::_offpop_big_icon_selected = -1;
                _officer_button_mode = -1;
                MOX::_officer_selected = -1;
                MOX::_draw_officer_to_back = 1;
            }

            if (hire_btn == input) {
                if (MOX::_officer_selected < 0) {
                    if (_officer_button_mode == 0) {
                        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
                        MOX::_offpop_big_icon_selected = -1;
                        MOX::_draw_officer_to_back = 1;
                        _officer_button_mode = -1;
                    } else {
                        mouse::Set_Mouse_List_(reinterpret_cast<uint8_t*>(&DIP_SCRN::_diplomacy_hand_mouse), 1);
                        MOX::_offpop_big_icon_selected = -1;
                        _officer_button_mode = 0;
                        MOX::_officer_selected = -1;
                    }
                } else {
                    Do_Hire_Officer_((int16_t)MOX::_officer_selected);
                }
            }

            Check_Officer_Fields_(input, scanned_id, big_icons_scanned, big_icons_ids);

            if (_officer_button_mode == -1) {
                int16_t scanned_ship_id, scanned_ship_idx;
                if (FLT1::Scan_Small_Ship_Icons_(scanned_id, input, &scanned_ship_id, &scanned_ship_idx) == 0) {
                    if (Is_Fleet_Officer_Screen_()) {
                        MOX::_draw_officer_to_back = 1;
                        Reload_Officer_Screen_Pictures_();
                    }
                }
                else if (scanned_ship_id > -1) {
                    MOX::_scanned_small_ship = scanned_ship_id;
                    MOX::_galaxy_map_scanned_star = -1;
                    MOX::_scanned_big_ship = -1;
                }
            }
            if (_officer_button_mode == -1 && is_fleet) {
                int16_t b_scanned_id;
                uint16_t b_idx;
                int scan_res = Scan_Offscrn_Big_Icon_Fields_(scanned_id, input, &b_scanned_id, &b_idx);
                MOX::_scanned_big_ship = b_scanned_id;
                if (b_scanned_id > -1) {
                    MOX::_galaxy_map_scanned_star = -1;
                    MOX::_scanned_small_ship = -1;
                }
                if (scan_res == 0) {
                    MOX::_draw_officer_to_back = 1;
                    if (MOX::_offpop_big_icon_selected == (int16_t)b_idx) {
                        MOX::_fltscrn_big_icon[(int16_t)b_idx].selected = 0;
                        MOX::_offpop_big_icon_selected = -1;
                    } else if ((int16_t)b_idx > -1) {
                        if (HACCESS::Is_Combat_Ship_(MOX::_fltscrn_big_icon[(int16_t)b_idx].ship_idx)) {
                            MOX::_offpop_big_icon_selected = (int16_t)b_idx;
                            FLT2::Clear_All_Selection_Status_();
                            MOX::_fltscrn_big_icon[MOX::_offpop_big_icon_selected].selected = 1;
                            if (MOX::_officer_selected > -1 && MOX::_officer_selected < 'C') {
                                if (Leader_Can_Be_Assigned_((int16_t)MOX::_officer_selected) == 0) {
                                    char buffer[200];
                                    const char* fmt = HAROLD::H_Message_(0x12D /* HESTR_12D_S_CANNOT_BE_PUT_INTO_S */);
                                    _sprintf(buffer, fmt, Leader_Name_((int16_t)MOX::_officer_selected, 0));
                                    Officer_Screen_User_Box_(buffer, 0);
                                } else {
                                    Officer_Screen_User_Box_((int16_t)MOX::_officer_selected, MOX::_fltscrn_big_icon[(int16_t)b_idx].ship_idx);
                                    MOX::_draw_officer_to_back = 1;
                                    Reset_Officer_Selection_();
                                }
                                if (MOX::_offpop_big_icon_selected > -1) {
                                    MOX::_fltscrn_big_icon[MOX::_offpop_big_icon_selected].selected = 0;
                                }
                            }
                        } else {
                            Officer_Screen_User_Box_(HAROLD::H_Message_(0x12C /* HESTR_12C_YOU_MAY_NOT_PLACE_LEAD */), 3);
                        }
                    }
                } else if (scan_res == 1) {
                    int16_t ship_idx = MOX::_fltscrn_big_icon[(int16_t)b_idx].ship_idx;
                    fields::Deactivate_Help_List_();
                    CMBTDRW1::Detailed_View_Ship_(ship_idx);
                    EVANHELP::Set_Officer_Screen_Help_();
                    MOX::_draw_officer_to_back = 1;
                }
            }
            if (_officer_button_mode == -1) {
                int16_t star_scan_id, star_id;
                int map_res = MAINSCR::Scan_Galaxy_Map_Fields_(scanned_id, input, &star_scan_id, &star_id);
                if (star_scan_id > -1) {
                    MOX::_galaxy_map_scanned_star = star_scan_id;
                    MOX::_scanned_small_ship = -1;
                    MOX::_scanned_big_ship = -1;
                }
                if (map_res == 0 && !is_fleet && star_id > -1 && HACCESS::Player_Has_Colony_In_System_(MOX::_PLAYER_NUM, star_id)) {
                    if (star_id == _officer_star_chosen) {
                        _officer_star_chosen = -1;
                    } else {
                        MOX::_officer_selected = -1;
                        MOX::_draw_officer_to_back = 1;
                        _officer_star_chosen = star_id;
                    }
                    _officer_star_displayed = star_id;
                }
            }

            if (_officer_button_mode == -1 && !is_fleet) {
                int16_t orbit_scanned, dummy_val1, dummy_val2, icon_scanned, ship_scanned;
                int16_t sys_res = MAINSCR::Scan_System_Display_Fields_(scanned_id, input, MOX::_planet_buttons, &dummy_val1, &orbit_scanned, &dummy_val2, &icon_scanned, &ship_scanned);
                MOX::_system_display_ship_scanned = ship_scanned;
                if (icon_scanned > -1) {
                    MOX::_system_icon_scanned = icon_scanned;
                }
                if (sys_res == 0 && orbit_scanned > -1) {
                    int16_t planet_idx = HAROLD::Planet_Index_(_officer_star_displayed, orbit_scanned);
                    if (MOX::_planet[planet_idx].colony_index > -1 && MOX::_planet[planet_idx].colony_index < MOX::_NUM_COLONIES && MAINSCR::_officer_screen_return_screen == 0x27) {
                        Officer_Screen_User_Box_(HAROLD::H_Message_(0x12E), 0);
                    }
                }
            }

            if (_officer_button_mode == -1) {
                if (input == next_btn) {
                    if (Is_Fleet_Officer_Screen_()) {
                        int16_t dummy_out;
                        MAINSCR::Cycle_Ship_Icons_(&MOX::_small_ship_stack_ptr, &dummy_out, 1);
                        needs_help_reset = 1;
                        MOX::_scanned_big_ship = -1;
                    } else {
                        MAINSCR::Cycle_Star_Icons_(&_officer_star_displayed, MOX::_PLAYER_NUM, 1);
                        _officer_star_chosen = _officer_star_displayed;
                    }
                } else if (input == prev_btn) {
                    if (Is_Fleet_Officer_Screen_()) {
                        int16_t dummy_out;
                        MAINSCR::Cycle_Ship_Icons_(&MOX::_small_ship_stack_ptr, &dummy_out, 0);
                        needs_help_reset = 1;
                        MOX::_scanned_big_ship = -1;
                    } else {
                        MAINSCR::Cycle_Star_Icons_(&_officer_star_displayed, MOX::_PLAYER_NUM, 0);
                        _officer_star_chosen = _officer_star_displayed;
                    }
                }
            }

            if (_officer_dismiss_btn == input) {
                if (_officer_button_mode == 2) {
                    _officer_button_mode = -1;
                    mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
                } else {
                    _officer_button_mode = 2;
                    mouse::Set_Mouse_List_(reinterpret_cast<uint8_t*>(&DIP_SCRN::_diplomacy_hand_mouse), 1);
                }
            }

            if (_officer_pool_btn == input) {
                if (_officer_button_mode == 1) {
                    _officer_button_mode = -1;
                    mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
                } else {
                    if (MOX::_officer_selected > -1) {
                        Move_Officer_To_Limbo_((int16_t)MOX::_officer_selected);
                    } else {
                        _officer_button_mode = 1;
                        mouse::Set_Mouse_List_(reinterpret_cast<uint8_t*>(&DIP_SCRN::_diplomacy_hand_mouse), 1);
                    }
                }
            }

            if (_officer_button_mode == -1) {
                if (scroll_down_btn == input) {
                    SCROLL::Scroll_Down_One_Item_(MOX::_scroll_bar);
                } else if (scroll_up_btn == input) {
                    SCROLL::Scroll_Up_One_Item_(MOX::_scroll_bar);
                }
            }

            if (_officer_button_mode == -1) {
                bool screen_toggle = false;
                if (fleet_view_btn == input) {
                    if (MOX::_officer_scrn_type != 1) {
                        MOX::_officer_scrn_type = 1;
                        screen_toggle = true;
                    }
                } else if (star_view_btn == input) {
                    if (MOX::_officer_scrn_type != 0) {
                        MOX::_officer_scrn_type = 0;
                        screen_toggle = true;
                    }
                }
                if (screen_toggle) {
                    Change_Screen_Type_(&needs_help_reset);
                }
            }

            if (ok_btn == input) {
                exit_screen = 1;
                MOX::_current_screen = MOX::_return_screen;
            }

            fields::Clear_Fields_();
            Add_Officer_Screen_Fields_(&hire_btn, &cancel_btn, &star_view_btn, &fleet_view_btn, &ok_btn, (int)big_icons_scanned, (int)big_icons_ids, &scroll_up_btn, &scroll_down_btn, &prev_btn, &next_btn, &dummy_field);

            if (needs_help_reset != 0) {
                needs_help_reset = 0;
                EVANHELP::Set_Officer_Screen_Help_();
                if (MOX::_officer_scrn_type == 0) {
                    FLT1::Initialize_Scroll_Bar_(MOX::_scroll_bar);
                }
            }

            if (exit_screen == 0) {
                video::Set_Page_Off_();
                Draw_Officer_Screen_();
                RUSS::Mox_Sync_Update_();
            }

            timer::Release_Time_(1);

        } while (exit_screen == 0);

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
        Move_From_Limbo_To_Pool_();
        Check_For_Colony_Calculation_Update_();
        COLCALC::Do_Player_Colony_Calculations_(MOX::_PLAYER_NUM);

        MOX::_cur_map_scale = (int16_t)saved_map_scale;
        MOX::_reloaded_ship_palette = 0;
        _prev_leaders = 0;
        MOX::_scroll_bar = nullptr;
    }
}