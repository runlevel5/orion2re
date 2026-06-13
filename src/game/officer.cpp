#include "pch.h"

namespace OFFICER {
    char s_OFFICER_LBX_00556980[16] = "OFFICER.LBX";
    s_colors _normal_text_colors = {{61, 68, 67, 68, 68, 68, 68, 68}};
    s_colors _red_colors = {{75, 129, 77, 129, 129, 129, 129, 129}};
    s_colors _selected_text_colors = {{0x3E, 0x47, 0x45, 0x47, 0x47, 0x47, 0x47, 0x47}};
    int16_t _officer_dismiss_btn;
    uint8_t _offscrn_skip_draw_ships;
    uint8_t _full_draw;
    int8_t _officer_button_mode;
    int16_t _officer_pool_btn;
    uint8_t _offscrn_flash_incr;
    uint8_t _offscrn_ask_for_hire_popup;
    int16_t _fleet_icon_ship_id_to_move;
    int8_t _offscrn_flash_dir;
    int16_t _officer_star_displayed;
    uint8_t _offscrn_flash_value;
    int16_t _officer_star_chosen;
    s_prev_leader_data* _prev_leaders;
    s_officer_picture_slot* _ldrpict;
    const char* PTR_s_OFFICER_LBX = s_OFFICER_LBX_00556980;
    char* _officer_skill_description_string[27];
    char* _officer_skill_name_string[27];
    s_officer_skill_help _officer_skill_help[4];
    const char s__00551100[1] = "";

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

    int32_t __cdecl Officer_Skill_Bonus_(uint32_t unused, int16_t leader_idx, int16_t skill_idx) {
        int16_t level;
        (void)unused;

        if (MOX::_leaders[leader_idx].player_index == -1) {
            level = Get_Officer_Level_(leader_idx);
        } else {
            level = Owned_Officer_Level_(leader_idx);
        }

        int32_t current_level_val = static_cast<int32_t>(level) + 1;
        int32_t skill_div = (int32_t)MOX::_skill_data[skill_idx].level_up;
        int32_t bonus_base = (int32_t)MOX::_skill_data[skill_idx].strength;

        int32_t intermediate = (current_level_val + skill_div - 1) / skill_div;

        return (intermediate * bonus_base) / 10;
    }

    int16_t __cdecl Owned_Officer_Level_(int16_t leader_idx) {
        int8_t warlord;
        s_leader_data* leader = &MOX::_leaders[leader_idx];
        int16_t xp = leader->xp;

        // Leader index 66 (0x42) is handled as having no warlord bonus
        if (leader_idx == 0x42) {
            warlord = 0;
        } else {
            int player_idx = (int)(int8_t)leader->player_index;
            // trait[30] corresponds to TRAIT_WARLORD
            warlord = MOX::_player[player_idx].traits[TRAIT_WARLORD];
        }

        int16_t level = Get_Officer_Base_Level_(xp, warlord);

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

            if (skill.skill_type == 0) {
                if (OFFICER::Officer_Has_General_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                    int16_t val = skill.cost;
                    if (skill_idx == 2 || skill_idx == 3) {
                        // type 0 (Ship Officer) gets 3, type 1 (Colony Leader) gets 1
                        val += (leader.type == 0) ? 3 : 1;
                        if (skill_idx == 3) {
                            val += (leader.type == 0) ? 3 : 1;
                        }
                    }
                    total_value += val;
                }
            }

            if (leader.type == 0) { // Ship Officer
                if (skill.skill_type == 1) {
                    if (OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                        total_value += skill.cost;
                    }
                }
            } else if (leader.type == 1) { // Colony Leader
                if (skill.skill_type == 2) {
                    if (OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                        total_value += skill.cost;
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
                if (game_random::Random_(7) < 3) {
                    current_class = 0;
                }
            }

            for (int j = 0; j < 54; j += 2) {
                if (current_class == 0) {
                    // Search for secondary skills
                    if (MOX::_skill_data[j].skill_type == 0 &&
                        ((*secondary_mask & MOX::_skill_data[j].skill_mask) == 0) &&
                        ((*secondary_mask & MOX::_skill_data[j + 1].skill_mask) == 0)) 
                    {
                        options_found++;
                        if (game_random::Random_(options_found) == 1) {
                            if (game_random::Random_(2) == 1) {
                                selected_idx = (int16_t)j;
                            } else {
                                selected_idx = (int16_t)(j + 1);
                            }
                        }
                    }
                } else {
                    // Search for primary skills of specific class
                    if ((int16_t)MOX::_skill_data[j].skill_type == skill_class &&
                        ((*primary_mask & MOX::_skill_data[j].skill_mask) == 0) &&
                        ((*primary_mask & MOX::_skill_data[j + 1].skill_mask) == 0)) 
                    {
                        options_found++;
                        if (game_random::Random_(options_found) == 1) {
                            if (game_random::Random_(2) == 1) {
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

    int16_t __cdecl Get_Research_Cost_Skill_Value_(int32_t cost) {
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
            if (MOX::_player[leader.player_index].traits[TRAIT_WARLORD] != 0) {
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

    /**
     * @address 0043D5B0
     */
    void __cdecl Deassign_Officer_(int16_t leader_idx) {
        s_leader_data& leader = MOX::_leaders[leader_idx];
        int16_t player_idx = (int16_t)leader.player_index;

        OFFICER::Increase_Officer_Level_By_One_(leader_idx);

        if (leader.type == 1) { // Colony Administrator
            int16_t star_idx = leader.location;
            if (star_idx != -1) {
                if (leader.status == 5) { // Active/Assigned status
                    MOX::_star[star_idx].system_special = SYSTEM_SPECIAL_NO_SPECIAL;
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
            int16_t idx1 = (int16_t)game_random::Random_(10) - 1;
            int16_t idx2 = (int16_t)game_random::Random_(10) - 1;
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
                    MOX::_leaders[leader_idx].player_index = (int8_t)player_idx;
                    is_leader_assigned[leader_idx] = 1;
                }
            }
        }

        // Phase 2: Assign two leaders to each non-Repulsive player from a shuffled pool of 25
        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            leader_indices[i] = i;
        }

        for (int i = 0; i < 25; ++i) {
            int16_t idx1 = (int16_t)game_random::Random_(25) - 1;
            int16_t idx2 = (int16_t)game_random::Random_(25) - 1;
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
                        MOX::_leaders[leader_idx].player_index = (int8_t)player_idx;
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

    /**
     * Calculates the maintenance cost for an officer in a specific slot.
     * Logic: (cost + 99) / 100, clamped to minimum 1, unless the officer has
     * specific skills or is in a specific slot (0x41).
     * @address 0043e8c0
     */
    int16_t __cdecl Officer_Maintenance_(int16_t player_idx, int16_t leader_idx) {
        int16_t cost = (int16_t)OFFICER::Officer_Cost_(player_idx, leader_idx);
        int16_t maintenance = (int16_t)(((int32_t)cost + 99) / 100);

        bool has_free_maintenance_skill = false;
        if (OFFICER::Officer_Has_General_Skill_(player_idx, leader_idx, 28 /* 0x1c */)) {
            has_free_maintenance_skill = true;
        } else if (OFFICER::Officer_Has_General_Skill_(player_idx, leader_idx, 29 /* 0x1d */)) {
            has_free_maintenance_skill = true;
        }

        // Slot 0x41 or maintenance skills result in 0 maintenance
        if (leader_idx == 65 /* 0x41 */ || has_free_maintenance_skill) {
            return 0;
        }

        // Clamp maintenance to a minimum of 1
        if (maintenance <= 1) {
            maintenance = 1;
        }

        return maintenance;
    }

    /**
     * Attempts to initiate the hiring process for an officer/leader.
     * Checks if the leader is available (status 4) and if the player has sufficient funds.
     */
    /**
     * @address 00443580
     */
    uint8_t __cdecl Ask_For_Hire_(int16_t leader_idx) {
        int16_t upkeep;
        int16_t costs;
        char template_str[200];
        char formatted_msg[200];

        Get_Officer_Costs_(leader_idx, &costs, &upkeep);
        fields::Deactivate_Help_List_();
        _full_draw = 1;

        int16_t msg_id = (upkeep == 1) ? HESTR_124_S_WILL_COST_D_BCS_INIT 
                                       : HESTR_125_S_WILL_COST_D_BCS_INIT;

        const char* template_ptr = HAROLD::H_Message_(msg_id);
        strcpy(template_str, template_ptr);

        char* name = Leader_Name_(leader_idx, 1);

        sprintf(formatted_msg, template_str, name, (int)costs, (int)upkeep);

        uint8_t result = Hire_Officer_Popup_(MOX::_PLAYER_NUM, leader_idx, formatted_msg);

        if (result != 0) {
            Hire_Officer_(MOX::_PLAYER_NUM, leader_idx);
            _officer_button_mode = -1;
        } else {
            if (_officer_button_mode == 0) {
                mouse::Set_Mouse_List_(&DIP_SCRN::_diplomacy_hand_mouse, 1);
                MOX::_officer_selected = -1;
            }
        }

        EVANHELP::Set_Officer_Screen_Help_();
        _full_draw = 0;
        fields::Assign_Auto_Function_(Draw_Officer_Screen_, 1);
        MOX::_draw_officer_to_back = 1;

        return result;
    }

    int16_t __cdecl Do_Hire_Officer_(int16_t leader_idx) {
        int16_t result = 0;
        int16_t cost = (int16_t)Officer_Cost_(MOX::_PLAYER_NUM, leader_idx);

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

        // Leader status 4 (0x4) generally represents being available for hire.
        s_leader_data* leader = &MOX::_leaders[leader_idx];

        if (leader->status == 4) {
            s_player* player = &MOX::_player[MOX::_PLAYER_NUM];
            if (player->bc < (int32_t)cost) {
                Not_Enough_BCs_Message_(leader_idx, cost);
            } else {
                result = (int16_t)Ask_For_Hire_(leader_idx);
            }
        } else {
            Already_Hired_Message_(leader_idx);
        }

        return result;
    }

    /**
     * @address 00443430
     */
    /**
     * @address 004434c0
     */
    void __cdecl Not_Enough_BCs_Message_(int16_t leader_idx, int16_t cost) {
        char format_string[200];
        char output_buffer[256];

        const char* msg = HAROLD::H_Message_(HESTR_123_YOU_HAVE_D_BC_IN_YOUR_);
        strcpy(format_string, msg);

        char* leader_name = Leader_Name_(leader_idx, 1);

        sprintf(output_buffer, format_string, MOX::_player[MOX::_PLAYER_NUM].bc, leader_name, (int32_t)cost);

        Officer_Screen_User_Box_(output_buffer, 0);

        MOX::_officer_selected = -1;
    }

    void __cdecl Already_Hired_Message_(int16_t officer_id) {
        char format_buffer[200];
        char output_buffer[200];

        const char* localized_format = HAROLD::H_Message_(HESTR_122_S_IS_ALREADY_IN_YOUR_S);

        strcpy(format_buffer, localized_format);

        char* leader_name = Leader_Name_(officer_id, 1);

        sprintf(output_buffer, format_buffer, leader_name);

        Officer_Screen_User_Box_(output_buffer, 0);

        MOX::_officer_selected = -1;
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

    void __cdecl Save_Officers_Original_Colony_Id_() {
        for (int32_t i = 0; i < MAX_LEADERS; ++i) {
            MOX::_officers_original_star_id[i] = MOX::_leaders[i].location;
        }
    }

    void __cdecl Save_Officers_Original_Ship_Id_() {
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

    void __cdecl Draw_Officer_Screen_To_Back_() {
        bool is_fleet_screen = OFFICER::Is_Fleet_Officer_Screen_();
        video::Set_Page_Back_();
        graphics::Reset_Window_();
        video::Clip_Off_();
        graphics::Fill_(0, 0, 639, 479, 0);

        if (MOX::_n_fltscrn_officers > 0) {
            int16_t y_screen_offset = 0;
            for (int16_t i = 0; i < MOX::_n_fltscrn_officers; ++i) {
                int16_t leader_id = MOX::_officer_id_list[i];
                if (leader_id > -1) {
                    s_animation_header* portrait_anim = OFFICER::_ldrpict[i].pic;
                    
                    int16_t anim_w = animate::Get_Width_(portrait_anim);
                    int16_t anim_h = animate::Get_Height_(portrait_anim);

                    s_leader_data& leader = MOX::_leaders[leader_id];
                    s_animation_header* draw_anim;
                    
                    if (leader.eta <= 0 && leader.status != 4) {
                        draw_anim = portrait_anim;
                    } else {
                        draw_anim = OFFICER::_ldrpict[i + 4].pic;
                    }

                    int16_t x = (75 - anim_w) / 2 + 12;
                    int16_t y = y_screen_offset + 37 + (90 - anim_h) / 2;
                    animate::Draw_(x, y, draw_anim);
                }
                y_screen_offset += 109;
            }
        }

        animate::Draw_(0, 0, MOX::_officer_main_seg);

        if (is_fleet_screen) {
            OFFICER::Draw_Fleet_Box_();
        } else {
            OFFICER::Draw_Colony_Box_();
        }

        video::Copy_Back_To_Off_();
        MOX::_draw_officer_to_back = 0;
        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }
        video::Set_Page_Off_();
    }

    void __cdecl Fade_Into_Leaders_Screen_() {
        fields::Disable_Draw_All_Fields_();

        MOX::_refresh_mode = 2;
        video::Set_Refresh_Stencil_();
        MOX::_draw_officer_to_back = 1;

        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        ERIC::Fast_Fade_Out_();

        fonts::Load_Palette_(8, 0, 0xFF);
        palette::Set_Palette_Changes_(0, 0xFF);

        if (Is_Fleet_Officer_Screen_() && MOX::_small_ship_stack_ptr > -1) {
            int16_t ship_id = MOX::_ship_stack_start[MOX::_small_ship_stack_ptr];
            int16_t owner_id = HACCESS::Ship_Stack_Owner_(ship_id);
            KEN::Load_Player_Ship_Palette_(owner_id);
        }

        palstore::Darken_Palette_(100);
        video::Set_Page_Off_();
        Draw_Officer_Screen_();
        palette::Clear_Palette_Changes_(0, 0xFF);
        RUSS::Mox_Sync_Update_();

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        ERIC::Fast_Fade_In_();

        fields::Enable_Draw_All_Fields_();
    }

    void __cdecl Print_Scanned_Ship_Name_() {
        char buffer[80];

        int16_t ship_idx = MOX::_fltscrn_big_icon[MOX::_scanned_big_ship].ship_idx;
        HACCESS::Ship_Owner_(ship_idx);

        s_ship_data* ship = &MOX::_ship[ship_idx];
        int16_t officer_idx = ship->officer_index;

        fonts::Set_Outline_Color_(0, 0);

        if (officer_idx < 0) {
            strcpy(buffer, ship->d.name);
        } else {
            s_leader_data* leader = &MOX::_leaders[officer_idx];
            int8_t eta = leader->eta;

            if (eta < 1) {
                sprintf(buffer, MOX::s_fmt_scanned_star_info, ship->d.name, leader->name);
            } else {
                const char* format;
                if (eta == 1) {
                    format = HAROLD::H_Message_(HESTR_092_S_S_D_TURN);
                } else {
                    format = HAROLD::H_Message_(HESTR_093_S_S_D_TURNS);
                }
                sprintf(buffer, format, ship->d.name, leader->name, (int)eta);
            }
        }

        MISC::Set_Fitted_Font_Style_(buffer, &OFFICER::_normal_text_colors, 3, 2, 4, 204, 1);
        fonts::Print_Centered_(466, 211, buffer);
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
                if (SCROLL::Draw_Generic_Vertical_Scroll_Bar_(MOX::_scroll_bar, FLT1::Fill_FltScrn_Scroll_Bar_)) {
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
            animate::Set_Animation_Frame_(MOX::_officer_dismiss_button_seg, 1);
            animate::Draw_(463, 441, MOX::_officer_dismiss_button_seg);
        } else if (OFFICER::_officer_dismiss_btn == -1000) {
            animate::Draw_(463, 441, MOX::_dull_officer_dismiss_button_seg);
        }

        if (OFFICER::_officer_button_mode == 1) {
            animate::Set_Animation_Frame_(MOX::_officer_pool_button_seg, 1);
            animate::Draw_(388, 441, MOX::_officer_pool_button_seg);
        } else if (OFFICER::_officer_pool_btn == -1000) {
            animate::Draw_(388, 441, MOX::_dull_officer_pool_button_seg);
        }

        if (!Leaders_In_Hiring_Pool_()) {
            animate::Draw_(313, 441, MOX::_dull_officer_hire_button_seg);
        }

        if (OFFICER::_officer_button_mode == 0) {
            animate::Draw_(300, 441, MOX::_officer_hire_replace_seg);
            if (MOX::_officer_scanned > -1) {
                Get_Officer_Costs_(MOX::_officer_scanned, &hire_cost, &maint_cost);
                if (maint_cost == 0) {
                    sprintf(buffer, HAROLD::H_Message_(HESTR_129_D_BCS_NO_MAINTENANCE), (int)hire_cost);
                } else {
                    sprintf(buffer, HAROLD::H_Message_(HESTR_121_D_BCS_AND_D_BC_TURN), (int)hire_cost, (int)maint_cost);
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
                    sprintf(buffer, MOX::s_fmt_scanned_star_info, MOX::_star[star_idx].name, leader->name);
                } else {
                    int msg_id = (leader->eta == 1) ? HESTR_092_S_S_D_TURN : HESTR_093_S_S_D_TURNS;
                    sprintf(buffer, HAROLD::H_Message_(msg_id), MOX::_star[star_idx].name, leader->name, (int)leader->eta);
                }
            }
            MISC::Set_Fitted_Font_Style_(buffer, &OFFICER::_normal_text_colors, 3, 2, 0, 203, 1);
            fonts::Print_Centered_(466, 210, buffer);
        }

        if (MOX::_global_var == 'c') {
            s_animation_header* seg = is_fleet_screen ? MOX::_officer_ship_ldrs_button_seg : MOX::_officer_colony_ldrs_button_seg;
            animate::Set_Animation_Frame_(seg, 1);
            animate::Draw_(is_fleet_screen ? 156 : 10, 11, seg);
        }

        bool type_not_one = MOX::_officer_scrn_type != 1;
        animate::Set_Animation_Frame_(MOX::_officer_ship_ldrs_button_seg, type_not_one);
        animate::Draw_(160, 10, MOX::_officer_ship_ldrs_button_seg);

        animate::Set_Animation_Frame_(MOX::_officer_colony_ldrs_button_seg, !type_not_one);
        animate::Draw_(7, 10, MOX::_officer_colony_ldrs_button_seg);

        OFFICER::_offscrn_flash_incr++;
        if (OFFICER::_offscrn_flash_incr % 25 == 0) {
            OFFICER::_offscrn_flash_dir = -OFFICER::_offscrn_flash_dir;
            OFFICER::_offscrn_flash_incr = 0;
        }
        if (OFFICER::_offscrn_flash_incr % 5 == 0) {
            OFFICER::_offscrn_flash_value += OFFICER::_offscrn_flash_dir;
        }

        int32_t next_idx = (int32_t)MOX::_multi_colored_line_start + 1;
        int32_t sign = next_idx >> 31;
        MOX::_multi_colored_line_start =
            (int16_t)(((((next_idx ^ sign) - sign) & 7) ^ sign) - sign);
    }

    void Officers_Screen_() {
        uint16_t saved_map_scale = (uint16_t)MOX::_cur_map_scale;
        int8_t needs_help_reset = 0;
        int16_t hire_btn, cancel_btn, fleet_view_btn, colony_view_btn, ok_btn, scroll_up_btn, scroll_down_btn, prev_btn, next_btn, dummy_field;
        int16_t big_icons_scanned[4], big_icons_ids[4];

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

        if (MOX::_previous_screen == SCREEN_COLONY) {
            MOX::_officer_scrn_type = 1;
            if (MOX::_temp_star_handle > -1) {
                MOX::_small_ship_stack_ptr = MOX::_temp_star_handle;
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
            _officer_star_displayed = MOX::_temp_star_handle;
            _officer_star_chosen = MOX::_temp_star_handle;
            if (MOX::_temp_star_handle == -1) {
                _officer_star_chosen = MOX::_planet[MOX::_player[MOX::_PLAYER_NUM].home_planet_id].star_index;
            }
            SYS::Init_System_Display_(MOX::_temp_star_handle);
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

        Add_Officer_Screen_Fields_(&hire_btn, &cancel_btn, &fleet_view_btn, &colony_view_btn, &ok_btn, big_icons_scanned, big_icons_ids, &scroll_up_btn, &scroll_down_btn, &prev_btn, &next_btn, &dummy_field);
        OFFICER::Initialize_Scroll_Bar_(MOX::_scroll_bar);

        MOX::_offpop_big_icon_selected = -1;
        MOX::_system_star_id = -1;
        MOX::_officer_selected = -1;
        MOX::_drawing_officer_popup = 1;
        MOX::_officer_scanned = 0;

        SYS::Clear_System_Display_Loaded_Flag_();
        Save_Officers_Original_Colony_Id_();
        Save_Officers_Original_Ship_Id_();
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
                mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                MOX::_offpop_big_icon_selected = -1;
                _officer_button_mode = -1;
                MOX::_officer_selected = -1;
                MOX::_draw_officer_to_back = 1;
            }

            if (hire_btn == input) {
                if (MOX::_officer_selected < 0) {
                    if (_officer_button_mode == 0) {
                        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                        MOX::_offpop_big_icon_selected = -1;
                        MOX::_draw_officer_to_back = 1;
                        _officer_button_mode = -1;
                    } else {
                        mouse::Set_Mouse_List_(&DIP_SCRN::_diplomacy_hand_mouse, 1);
                        MOX::_offpop_big_icon_selected = -1;
                        _officer_button_mode = 0;
                        MOX::_officer_selected = -1;
                    }
                } else {
                    Do_Hire_Officer_((int16_t)MOX::_officer_selected);
                }
            }

            Check_Officer_Fields_(input, scanned_id, (int16_t*)big_icons_scanned, (int16_t*)big_icons_ids);

            if (_officer_button_mode == -1) {
                int16_t scanned_ship_id, scanned_ship_idx;
                if (FLT1::Scan_Small_Ship_Icons_(scanned_id, input, &scanned_ship_id, &scanned_ship_idx) != 0) {
                    if (scanned_ship_id > -1) {
                        MOX::_scanned_small_ship = scanned_ship_id;
                        MOX::_galaxy_map_scanned_star = -1;
                        MOX::_scanned_big_ship = -1;
                    }
                } else if (is_fleet) {
                    MOX::_draw_officer_to_back = 1;
                    Reload_Officer_Screen_Pictures_();
                }
            }
            if (_officer_button_mode == -1 && is_fleet) {
                int16_t b_scanned_id;
                int16_t b_idx;
                uint32_t scan_res = Scan_Offscrn_Big_Icon_Fields_(scanned_id, input, &b_scanned_id, &b_idx);
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
                            if (MOX::_officer_selected > -1 && MOX::_officer_selected < MAX_LEADERS) {
                                if (Leader_Can_Be_Assigned_((int16_t)MOX::_officer_selected) == 0) {
                                    char buffer[200];
                                    const char* fmt = HAROLD::H_Message_(0x12D /* HESTR_12D_S_CANNOT_BE_PUT_INTO_S */);
                                    sprintf(buffer, fmt, Leader_Name_((int16_t)MOX::_officer_selected, 0));
                                    Officer_Screen_User_Box_(buffer, 0);
                                } else {
                                    Assign_Captain_((int16_t)MOX::_officer_selected, MOX::_fltscrn_big_icon[(int16_t)b_idx].ship_idx);
                                    MOX::_draw_officer_to_back = 1;
                                    Deselect_Offpop_Icons_();
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
                    mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                } else {
                    _officer_button_mode = 2;
                    mouse::Set_Mouse_List_(&DIP_SCRN::_diplomacy_hand_mouse, 1);
                }
            }

            if (_officer_pool_btn == input) {
                if (_officer_button_mode == 1) {
                    _officer_button_mode = -1;
                    mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                } else {
                    if (MOX::_officer_selected > -1) {
                        Move_Officer_To_Limbo_((int16_t)MOX::_officer_selected);
                    } else {
                        _officer_button_mode = 1;
                        mouse::Set_Mouse_List_(&DIP_SCRN::_diplomacy_hand_mouse, 1);
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
                if (colony_view_btn == input) {
                    if (MOX::_officer_scrn_type != 1) {
                        MOX::_officer_scrn_type = 1;
                        screen_toggle = true;
                    }
                } else if (fleet_view_btn == input) {
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
            Add_Officer_Screen_Fields_(&hire_btn, &cancel_btn, &fleet_view_btn, &colony_view_btn, &ok_btn, big_icons_scanned, big_icons_ids, &scroll_up_btn, &scroll_down_btn, &prev_btn, &next_btn, &dummy_field);

            if (needs_help_reset != 0) {
                needs_help_reset = 0;
                EVANHELP::Set_Officer_Screen_Help_();
                if (MOX::_officer_scrn_type == 0) {
                    OFFICER::Initialize_Scroll_Bar_(MOX::_scroll_bar);
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
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        Move_From_Limbo_To_Pool_();
        Check_For_Colony_Calculation_Update_();
        COLCALC::Do_Player_Colony_Calculations_(MOX::_PLAYER_NUM);

        MOX::_cur_map_scale = (int16_t)saved_map_scale;
        MOX::_reloaded_ship_palette = 0;
        _prev_leaders = 0;
        MOX::_scroll_bar = nullptr;
    }

    void __cdecl Draw_Colony_Box_() {
        animate::Draw_(300, 12, MOX::_officer_colony_box_seg);
    }

    void __cdecl Draw_Fleet_Box_() {
        animate::Draw_(300, 12, MOX::_officer_fleet_box_seg);
    }

    void __cdecl Text_Box_With_Refresh_(const char* title, const char* body) {
        TEXTBOX::Text_Box_(title, body);

        if (OFFICER::_full_draw == 0) {
            video::Set_Page_Off_();
            MOX::_draw_officer_to_back = 1;
            OFFICER::Draw_Officer_Screen_();
            RUSS::Mox_Sync_Update_();
        }
    }

    bool __cdecl Ships_In_Same_Place_(int16_t ship_index_1, int16_t ship_index_2) {
        // ship structure size: 0x81 (129) bytes
        const auto& ship1 = MOX::_ship[ship_index_1];
        const auto& ship2 = MOX::_ship[ship_index_2];

        // Coordinates x and y are shorts at offsets 0x67 and 0x69 respectively
        if (ship1.x == ship2.x && ship1.y == ship2.y) {
            return true;
        }

        return false;
    }

    void __cdecl Assign_Leader_To_Star_(s_player* player, s_leader_data* leader, s_star_data* star, int8_t eta) {
        // Calculate indices based on base pointers
        int32_t player_index = static_cast<int32_t>(player - MOX::_player);
        int32_t leader_index = static_cast<int32_t>(leader - MOX::_leaders);
        int32_t star_index = static_cast<int32_t>(star - MOX::_star);

        // Check if there's already an officer assigned to this star for the player
        int8_t current_leader_idx = star->officer_index[player_index];
        if (current_leader_idx > -1 && current_leader_idx < MAX_LEADERS) {
            s_leader_data* prev_leader = &MOX::_leaders[current_leader_idx];
            prev_leader->eta = 0;
            prev_leader->status = 0;
            prev_leader->location = -1;
        }

        // If the leader was already at another location, clear that assignment
        if (leader->location > -1) {
            MOX::_star[leader->location].officer_index[player_index] = -1;
        }

        // Set the leader's new assignment details
        leader->location = static_cast<int16_t>(star_index);
        leader->eta = eta;
        leader->status = 1;

        // Record the leader's index at the star system for the player
        star->officer_index[player_index] = static_cast<int8_t>(leader_index);
    }

    void __cdecl Assign_Leader_To_Ship_(s_player* player, s_leader_data* leader, s_ship_data* ship, int8_t eta) {
        int leader_idx = leader - MOX::_leaders;
        int ship_idx = ship - MOX::_ship;

        // If the ship already has an officer, clear that officer's assignment data.
        short current_officer_idx = ship->officer_index;
        if (current_officer_idx > -1 && current_officer_idx < MAX_LEADERS) {
            s_leader_data* old_officer = &MOX::_leaders[current_officer_idx];
            old_officer->status = 0;
            old_officer->location = -1;
            old_officer->eta = 0;
        }

        // If this leader was already assigned to another ship, clear that ship's officer index.
        if (leader->location > -1) {
            MOX::_ship[leader->location].officer_index = -1;
        }

        // Apply new assignment
        leader->location = (short)ship_idx;
        leader->eta = eta;
        leader->status = 1;
        ship->officer_index = (short)leader_idx;
    }

    int16_t __cdecl Scan_Officer_Fields_(
        int16_t* field_x_a,
        int16_t* field_x_b,
        int16_t current_x,
        int16_t scan_x,
        int8_t* out_current_officer_id,
        int16_t* out_scanned_officer_id,
        int16_t* out_skill_id) {
        int16_t match_type = -1;

        *out_current_officer_id = -1;
        *out_scanned_officer_id = -1;
        *out_skill_id = -1;

        for (int16_t officer_slot = 0; officer_slot < 4 && match_type == -1; ++officer_slot) {
            for (int16_t skill_slot = 0; skill_slot < 5 && match_type == -1; ++skill_slot) {
                if (OFFICER::_officer_skill_help[officer_slot].skill_help_x[skill_slot] == scan_x) {
                    match_type = 0;
                    *out_scanned_officer_id = MOX::_officer_id_list[officer_slot];
                } else if (scan_x + OFFICER::_officer_skill_help[officer_slot].skill_help_x[skill_slot] == 0) {
                    match_type = 1;
                    *out_scanned_officer_id = MOX::_officer_id_list[officer_slot];
                    *out_skill_id = OFFICER::_officer_skill_help[officer_slot].skill_id[skill_slot];
                }
            }
        }

        for (int16_t officer_slot = 0; officer_slot < 4 && match_type == -1; ++officer_slot) {
            if (field_x_a[officer_slot] == scan_x || field_x_b[officer_slot] == scan_x) {
                match_type = 0;
                *out_scanned_officer_id = MOX::_officer_id_list[officer_slot];
            } else if (scan_x + field_x_b[officer_slot] == 0) {
                match_type = 1;
                *out_scanned_officer_id = MOX::_officer_id_list[officer_slot];
            }
        }

        if (match_type == -1) {
            for (int16_t officer_slot = 0; officer_slot < 4; ++officer_slot) {
                if (field_x_a[officer_slot] == current_x || field_x_b[officer_slot] == current_x) {
                    match_type = 4;
                    *out_current_officer_id = static_cast<int8_t>(MOX::_officer_id_list[officer_slot]);
                }

                if (match_type == -1) {
                    for (int16_t skill_slot = 0; skill_slot < 5 && match_type == -1; ++skill_slot) {
                        if (OFFICER::_officer_skill_help[officer_slot].skill_help_x[skill_slot] == current_x) {
                            match_type = 4;
                            *out_current_officer_id = static_cast<int8_t>(MOX::_officer_id_list[officer_slot]);
                        }
                    }
                }
            }
        }

        return match_type;
    }

    uint32_t __cdecl Scan_Offscrn_Big_Icon_Fields_(int16_t current_field_id, int16_t scanned_field_id, int16_t* out_current_icon_idx, int16_t* out_scanned_icon_idx) {
        int16_t start_idx = MOX::_scroll_bar->first_visible_row * MOX::_big_icon_display_columns;
        int16_t end_idx = start_idx + 0x14;

        if (end_idx > static_cast<int16_t>(MOX::_n_fltscrn_big_icons)) {
            end_idx = static_cast<int16_t>(MOX::_n_fltscrn_big_icons);
        }

        uint16_t result = 0xFFFF;
        bool stop = false;
        *out_current_icon_idx = -1;
        *out_scanned_icon_idx = -1;

        if (start_idx < 0 || end_idx < 0) {
            stop = true;
        }

        if (start_idx < end_idx) {
            for (int16_t i = start_idx; i < end_idx; ++i) {
                if (stop) break;

                auto& icon = MOX::_fltscrn_big_icon[i];
                if (icon.ship_idx > -1) {
                    if (icon.button_id == scanned_field_id) {
                        result = 0;
                        stop = true;
                        *out_scanned_icon_idx = i;
                    }
                    else if ((int32_t)icon.button_id + (int32_t)scanned_field_id == 0) {
                        result = 1;
                        stop = true;
                        *out_scanned_icon_idx = i;
                    }
                }
            }
        }

        if (result == 0xFFFF) {
            if (start_idx < end_idx && !stop) {
                for (int16_t i = start_idx; i < end_idx; ++i) {
                    auto& icon = MOX::_fltscrn_big_icon[i];
                    if (icon.button_id == current_field_id && icon.ship_idx > -1) {
                        result = 4;
                        *out_current_icon_idx = i;
                        break;
                    }
                }
            }
        }

        return result;
    }

    void __cdecl Check_Officer_Fields_(int16_t input, int16_t scanned, int16_t* officer_fields, int16_t* portrait_fields) {
        bool is_fleet_screen = Is_Fleet_Officer_Screen_();
        int16_t field1, field2;
        int16_t scan_result = Scan_Officer_Fields_(officer_fields, portrait_fields, scanned, input, &MOX::_officer_scanned, &field2, &field1);

        if (scan_result == 0) {
            if (static_cast<int16_t>(MOX::_officer_selected) == field2) {
                MOX::_officer_selected = -1;
                return;
            }

            if (field2 >= 0) {
                MOX::_officer_selected = static_cast<int8_t>(field2);
                int8_t mode = _officer_button_mode;

                if (mode == 0 || (mode == -1 && MOX::_leaders[field2].status == 4 /* Limbo */)) {
                    Do_Hire_Officer_(field2);
                    MOX::_draw_officer_to_back = 1;
                    if (_officer_button_mode != 0) {
                        MOX::_officer_selected = -1;
                    }
                } else if (mode == 2 /* Dismiss */) {
                    char format_str[200];
                    char final_msg[200];
                    strcpy(format_str, HAROLD::H_Message_(HESTR_126_DISMISS_S));
                    mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                    sprintf(final_msg, format_str, Leader_Name_(MOX::_officer_selected, 1));

                    if (Officer_Screen_User_Box_(final_msg, 1) != 0) {
                        Dismiss_Officer_(MOX::_officer_selected);
                        MOX::_officer_selected = -1;
                    } else {
                        mouse::Set_Mouse_List_(&DIP_SCRN::_diplomacy_hand_mouse, 1);
                    }
                    MOX::_draw_officer_to_back = 1;
                } else if (mode == 1 /* Relocate */) {
                    if (MOX::_leaders[field2].status == 0 || MOX::_leaders[field2].status == 2) {
                        MOX::_officer_selected = -1;
                        _officer_button_mode = -1;
                        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                    } else {
                        Move_Officer_To_Limbo_(field2);
                    }
                    MOX::_draw_officer_to_back = 1;
                } else if (is_fleet_screen) {
                    if (MOX::_offpop_big_icon_selected >= 0) {
                        int16_t ship_idx = MOX::_fltscrn_big_icon[MOX::_offpop_big_icon_selected].ship_idx;
                        if (Leader_Can_Be_Assigned_(field2) != 0) {
                            Assign_Captain_(MOX::_officer_selected, ship_idx);
                            MOX::_draw_officer_to_back = 1;
                            Deselect_Offpop_Icons_();
                        } else {
                            char format_str[200];
                            char final_msg[200];
                            strcpy(format_str, HAROLD::H_Message_(HESTR_127_S_MUST_BE_HIRED_BEFORE));
                            sprintf(final_msg, format_str, Leader_Name_(MOX::_officer_selected, 0));
                            Officer_Screen_User_Box_(final_msg, 0);
                        }
                        MOX::_officer_selected = -1;
                    }
                } else if (_officer_star_chosen != -1 && HAROLD::Player_Has_Colony_(MOX::_PLAYER_NUM, _officer_star_chosen)) {
                    if (Leader_Can_Be_Assigned_(MOX::_officer_selected) != 0) {
                        if (MOX::_leaders[static_cast<uint8_t>(MOX::_officer_selected)].location != _officer_star_chosen) {
                            Assign_Captain_(MOX::_officer_selected, _officer_star_chosen);
                            MOX::_draw_officer_to_back = 1;
                        }
                    } else {
                        char format_str[200];
                        char final_msg[200];
                        if (MOX::_leaders[static_cast<uint8_t>(MOX::_officer_selected)].status == 4) {
                            strcpy(format_str, HAROLD::H_Message_(HESTR_127_S_MUST_BE_HIRED_BEFORE));
                            sprintf(final_msg, format_str, Leader_Name_(MOX::_officer_selected, 0));
                        } else {
                            strcpy(format_str, HAROLD::H_Message_(HESTR_128_YOU_CANNOT_ASSIGN_S_TO));
                            sprintf(final_msg, format_str, Leader_Name_(MOX::_officer_selected, 0), MOX::_star[_officer_star_chosen].name);
                        }
                        Officer_Screen_User_Box_(final_msg, 0);
                    }
                    MOX::_officer_selected = -1;
                }
            }
        } else if (scan_result == 1) {
            if (field1 != -1) {
                int32_t bonus = Officer_Skill_Bonus_(MOX::_PLAYER_NUM, field2, field1);
                Print_Officer_Skill_Help_(field1, static_cast<int16_t>(bonus), field2);
            } else {
                Find_Selected_Leader_(field2);
            }
        }
    }

    /**
     * @address 00443fe0
     */
    void __cdecl Dismiss_Officer_(int16_t officer_idx) {
        s_leader_data& leader = MOX::_leaders[officer_idx];
        int16_t location = leader.location;

        if (leader.type == 0 /* SHIP_OFFICER */) {
            if (location > -1 && location < static_cast<int16_t>(MOX::_NUM_SHIPS)) {
                // Remove leader reference from the ship
                MOX::_ship[location].officer_index = -1;
            }
        } else {
            if (location > -1 && location < static_cast<int16_t>(MOX::_NUM_STARS)) {
                // Remove leader reference from the star system
                MOX::_star[location].officer_index[leader.player_index] = -1;
            }
        }

        Deassign_Officer_(officer_idx);
        Build_Captain_Id_List_();
        Get_Officer_Pictures_();
        EVANHELP::Set_Officer_Screen_Help_();

        OFFICER::_officer_button_mode = -1;
    }

    bool __cdecl Leader_Can_Be_Assigned_(int16_t leader_idx) {
        int8_t status = MOX::_leaders[leader_idx].status;

        if (status == 0 || status == 1 || status == 2) {
            return true;
        }

        return false;
    }

    void __cdecl Hire_Officer_(int16_t player_idx, int16_t leader_idx) {
        int16_t maintenance_cost;
        int16_t hire_cost;

        Get_Officer_Costs_(leader_idx, &hire_cost, &maintenance_cost);

        // Deduct hiring cost from player BC
        MOX::_player[player_idx].bc -= (int32_t)hire_cost;

        // Status 1 likely means 'hired' or 'active'
        Set_Officer_To_Player_(leader_idx, player_idx, 1);

        // If officer type is 0 (Ship Officer), check for Galactic Lore skills
        if (MOX::_leaders[leader_idx].type == 0) {
            bool has_lore = Officer_Has_Special_Skill_(player_idx, leader_idx, 0x12 /* SKILL_GALACTIC_LORE_1 */);
            if (!has_lore) {
                has_lore = Officer_Has_Special_Skill_(player_idx, leader_idx, 0x13 /* SKILL_GALACTIC_LORE_2 */);
            }

            if (has_lore) {
                MOX::_player[player_idx].has_galactic_lore = 1;
            }
        }
    }

    int8_t __cdecl Get_Ship_Leader_ETA_(int16_t player_idx, int16_t leader_idx, int16_t ship_idx) {
        int16_t leader_loc;

        if (_prev_leaders == nullptr) {
            leader_loc = HAROLD::Absolute_Location_(MOX::_leaders[leader_idx].location);
        } else {
            leader_loc = _prev_leaders[leader_idx].location;
        }

        int16_t home_star_id = MOX::_planet[MOX::_player[player_idx].home_planet_id].star_index;
        bool at_same_place = false;

        if (leader_loc != -1) {
            at_same_place = Ships_In_Same_Place_(leader_loc, ship_idx);
        }

        if (_prev_leaders == nullptr) {
            return at_same_place ? 0 : 5;
        }

        int8_t status = _prev_leaders[leader_idx].status;
        if (status != 0 && status != 4) {
            if (at_same_place) {
                return static_cast<int8_t>(_prev_leaders[leader_idx].eta);
            }
            return 5;
        }

        const auto& ship = MOX::_ship[ship_idx];
        if (ship.location == home_star_id) {
            return 0;
        }

        if (HAROLD::XY_To_Star_Id_(ship.x, ship.y) == home_star_id) {
            return 0;
        }

        return 5;
    }

    int8_t __cdecl Get_Star_Leader_ETA_(int16_t player_index, int16_t leader_index, int16_t target_star_index) {
        short home_star_index = MOX::_planet[MOX::_player[player_index].home_planet_id].star_index;

        if (OFFICER::_prev_leaders == nullptr) {
            if (target_star_index == home_star_index && MOX::_leaders[leader_index].status == 0) {
                return 0;
            }
        } else {
            auto& prev = OFFICER::_prev_leaders[leader_index];
            if (prev.status == 0 || prev.status == 4) {
                if (target_star_index == home_star_index) {
                    return 0;
                }
            } else if (target_star_index == prev.location) {
                return (int8_t)prev.eta;
            }
        }

        return 5;
    }

    void __cdecl Assign_Captain_(int16_t leader_idx, int16_t ship_idx) {
        char format_buf[200];
        char display_buf[200];
        int8_t eta;

        if (Is_Fleet_Officer_Screen_()) {
            if (HACCESS::Ship_Owner_(ship_idx) != MOX::_PLAYER_NUM) {
                strcpy(format_buf, HAROLD::H_Message_(HESTR_117_YOU_MAY_NOT_ASSIGN_S_T));
                sprintf(display_buf, format_buf, Leader_Name_(leader_idx, 0));
                Officer_Screen_User_Box_(display_buf, 3);
                return;
            }

            eta = Get_Ship_Leader_ETA_(MOX::_PLAYER_NUM, leader_idx, ship_idx);
            s_ship_data* target_ship = &MOX::_ship[ship_idx];

            if (eta == 0) {
                strcpy(format_buf, HAROLD::H_Message_(HESTR_119_ASSIGN_S_TO_S));
                sprintf(display_buf, format_buf, Leader_Name_(leader_idx, 0), target_ship);
            } else {
                strcpy(format_buf, HAROLD::H_Message_(HESTR_118_IT_WILL_TAKE_D_TURNS_F));
                if (MOX::_settings.language == 4) {
                    sprintf(display_buf, format_buf, Leader_Name_(leader_idx, 0), (int)eta, target_ship);
                } else {
                    sprintf(display_buf, format_buf, (int)eta, Leader_Name_(leader_idx, 0), target_ship);
                }
            }

            if (Officer_Screen_User_Box_(display_buf, 1)) {
                int16_t existing_leader = target_ship->officer_index;
                bool proceed = true;

                if (existing_leader != -1) {
                    s_leader_data* leader = &MOX::_leaders[existing_leader];
                    int16_t msg_id = (leader->eta < 1) ? HESTR_11B_S_CURRENTLY_ON_S_WILL_ : HESTR_11A_S_IN_ROUTE_TO_S_WILL_B;

                    strcpy(format_buf, HAROLD::H_Message_(msg_id));
                    sprintf(display_buf, format_buf, Leader_Name_(existing_leader, 1), target_ship);
                    proceed = Officer_Screen_User_Box_(display_buf, 1) != 0;
                }

                if (proceed) {
                    Assign_Leader_To_Ship_(&MOX::_player[MOX::_PLAYER_NUM], &MOX::_leaders[leader_idx], target_ship, eta);
                }
            }
        } else {
            // Colony Officer Screen logic
            eta = Get_Star_Leader_ETA_(MOX::_PLAYER_NUM, leader_idx, ship_idx);
            s_star_data* target_star = &MOX::_star[ship_idx];

            if (eta == 0) {
                strcpy(format_buf, HAROLD::H_Message_(HESTR_119_ASSIGN_S_TO_S));
                sprintf(display_buf, format_buf, Leader_Name_(leader_idx, 0), target_star);
            } else {
                strcpy(format_buf, HAROLD::H_Message_(HESTR_118_IT_WILL_TAKE_D_TURNS_F));
                if (MOX::_settings.language == 4) {
                    sprintf(display_buf, format_buf, Leader_Name_(leader_idx, 0), (int)eta, target_star);
                } else {
                    sprintf(display_buf, format_buf, (int)eta, Leader_Name_(leader_idx, 0), target_star);
                }
            }

            if (Officer_Screen_User_Box_(display_buf, 1)) {
                int16_t existing_leader = (int16_t)target_star->officer_index[MOX::_PLAYER_NUM];
                bool proceed = true;

                if (leader_idx != existing_leader && existing_leader != -1) {
                    s_leader_data* leader = &MOX::_leaders[existing_leader];
                    int16_t msg_id = (leader->eta < 1) ? HESTR_11B_S_CURRENTLY_ON_S_WILL_ : HESTR_11A_S_IN_ROUTE_TO_S_WILL_B;

                    strcpy(format_buf, HAROLD::H_Message_(msg_id));
                    sprintf(display_buf, format_buf, Leader_Name_(existing_leader, 1), target_star);
                    proceed = Officer_Screen_User_Box_(display_buf, 1) != 0;
                }

                if (proceed) {
                    Assign_Leader_To_Star_(&MOX::_player[MOX::_PLAYER_NUM], &MOX::_leaders[leader_idx], target_star, eta);
                }
            }
        }
    }

    int16_t __cdecl Officer_Screen_User_Box_(const char* message, int16_t type) {
        int16_t result = -1;

        if (type == 1) {
            result = GENDRAW::Confirmation_Box_(message);
        } else if (type == 3) {
            GENDRAW::Warning_Box_(message);
        } else if (type == 5) {
            int8_t duration = MOX::_global_var;
            if (duration < 0) {
                duration = 0x7E;
            }

            MOX::_global_var = duration;
            TEXTBOX::Timed_Text_Box_(nullptr, message, static_cast<int16_t>(duration));
            MOX::_global_var = 0;
        } else {
            GENDRAW::Message_Box_(message);
        }

        video::Set_Page_Off_();
        MOX::_draw_officer_to_back = 1;
        OFFICER::Draw_Officer_Screen_();
        RUSS::Mox_Sync_Update_();

        return result;
    }

    /**
     * Updates the status and estimated time of arrival (ETA) for all officers.
     * Decrements ETA for traveling officers and increments it for those in a pending/cooldown state.
     */
    void __cdecl Decrement_Officer_ETA_() {
        for (int16_t leader_idx = 0; leader_idx < MAX_LEADERS; ++leader_idx) {
            s_leader_data& leader = MOX::_leaders[leader_idx];

            if (leader.status == 4) {
                // Officer is likely in a cooldown or "killed" state
                leader.eta++;
                if (leader.eta >= 30 /* 0x1e */) {
                    Deassign_Officer_(leader_idx);
                }
            } else if (leader.status == 1) {
                // Officer is traveling or active
                if (leader.eta > 0) {
                    leader.eta--;
                    if (leader.eta == 0 && leader.type == 1 /* Colony Leader */) {
                        // Recalculate colony effects once the leader arrives
                        COLCALC::Star_Colony_Calculation_(leader.location, (int16_t)leader.player_index);
                    }
                }
            }
        }
    }

    void __cdecl Deselect_Offpop_Icons_() {
        MOX::_officer_selected = -1;

        if (MOX::_offpop_big_icon_selected > -1) {
            MOX::_fltscrn_big_icon[MOX::_offpop_big_icon_selected].selected = 0;
        }

        MOX::_offpop_big_icon_selected = -1;
    }

    void __cdecl Print_Officer_Skill_Help_(int16_t skill_id, int16_t bonus, int16_t leader_idx) {
        char leader_name_buf[80];
        char skill_desc_buf[256];
        char skill_name_buf[80];

        char* leader_raw_name = Leader_Name_(leader_idx, 1);
        sprintf(leader_name_buf, MOX::s_fmt_s, leader_raw_name);

        if (MOX::_settings.language != 4 && MOX::_leaders[leader_idx].title[0] != '\0') {
            strcat(leader_name_buf, ",");
        }

        int16_t skill_value;
        if (skill_id == 28) {
            skill_value = 10;
        } else if (skill_id == 29) {
            skill_value = 15;
        } else {
            skill_value = bonus;
        }

        sprintf(skill_desc_buf, _officer_skill_description_string[skill_id / 2], leader_name_buf, (int)skill_value);

        strcpy(skill_name_buf, _officer_skill_name_string[skill_id / 2]);
        MISC::Replace_Char_(skill_name_buf, '_', ' ');

        if (MOX::_current_screen == SCREEN_OFFICERS) {
            Text_Box_With_Refresh_(skill_name_buf, skill_desc_buf);
        } else {
            TEXTBOX::Text_Box_(skill_name_buf, skill_desc_buf);
        }
    }

    /**
     * @address 0043ff00
     */
    void __cdecl Move_Officer_To_Limbo_(int16_t leader_idx) {
        s_leader_data& leader = MOX::_leaders[leader_idx];

        if (leader.status == 4 /* CAPTURED/PRISONER */) {
            const char* message_template = HAROLD::H_Message_(HESTR_12B_YOU_MAY_NOT_PLACE_S_IN);
            char template_buffer[200];
            strcpy(template_buffer, message_template);

            char* leader_name = Leader_Name_(leader_idx, 0);
            char final_message[200];

            sprintf(final_message, template_buffer, leader_name);

            Officer_Screen_User_Box_(final_message, 3);
            return;
        }

        int16_t location = leader.location;
        int8_t player_idx = leader.player_index;

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

        if (Is_Fleet_Officer_Screen_()) {
            if (location > -1 && location < static_cast<int16_t>(MOX::_NUM_SHIPS)) {
                MOX::_ship[location].officer_index = -1;
            }
        } else {
            if (location > -1) {
                MOX::_star[location].officer_index[player_idx] = -1;
            }
        }

        leader.status = 2; /* LIMBO/READY */
        leader.location = -1;
        _officer_button_mode = -1;
        MOX::_officer_selected = -1;
    }

    void __cdecl Change_Screen_Type_(int8_t* out_flag) {
        MOX::_small_ship_stack_ptr = -1;
        MOX::_system_icon_scanned = -1;
        MOX::_scanned_big_ship = -1;
        MOX::_scanned_small_ship = -1;
        MOX::_system_star_id = -1;

        SYS::Clear_System_Display_Loaded_Flag_();

        if (MOX::_officer_scrn_type == 1 /* OFFICER_SCREEN */) {
            if (OFFICER::_officer_star_displayed == -1) {
                if (static_cast<uint8_t>(MOX::_previous_screen) == SCREEN_COLONY) {
                    MOX::_officer_selected = -1;
                    OFFICER::_officer_star_displayed = MOX::_temp_star_handle;
                } else {
                    OFFICER::_officer_star_displayed = MAINSCR::Get_Star_Id_For_Officers_Screen_();
                    MOX::_officer_selected = -1;
                }
            }

            SYS::Init_System_Display_(OFFICER::_officer_star_displayed);
            MOX::_draw_officer_to_back = 1;
            OFFICER::_officer_star_chosen = OFFICER::_officer_star_displayed;
            OFFICER::Reload_Officer_Screen_Pictures_();

            *out_flag = 1;
            MOX::_draw_officer_to_back = 1;
            return;
        }

        FLT1::Set_Small_Ship_Stack_Pointer_(0);

        if (MOX::_small_ship_stack_ptr != -1) {
            int16_t stack_id = MOX::_ship_stack_start[MOX::_small_ship_stack_ptr];
            if (HACCESS::Ship_Stack_Owner_(stack_id) != MOX::_PLAYER_NUM) {
                MOX::_small_ship_stack_ptr = -1;
            }
        }

        FLT2::Build_Fltscrn_Big_Icon_Fields_(
            MOX::_small_ship_stack_ptr,
            MOX::_scroll_bar->first_visible_row
        );
        OFFICER::Reload_Officer_Screen_Pictures_();

        MOX::_draw_officer_to_back = 1;
        OFFICER::_offscrn_skip_draw_ships = 1;
        OFFICER::Draw_Officer_Screen_();
        RUSS::Mox_Sync_Update_();
        KEN::Load_Player_Ship_Palette_(MOX::_PLAYER_NUM);

        *out_flag = 1;
        MOX::_old_small_ship_stack_ptr = -1;
        MOX::_draw_officer_to_back = 1;
        MOX::_officer_selected = -1;
    }

    /**
     * @address 00442640
     */
    void __cdecl Wrapper_For_Offscrn_Help_Fields_() {
        int16_t officer_slot = 0;
        int16_t y = 0x32;

        while (officer_slot < 4) {
            const int16_t officer_id = MOX::_officer_id_list[officer_slot];

            if (officer_id < 0) {
                for (int16_t skill_index = 0; skill_index < 5; ++skill_index) {
                    OFFICER::_officer_skill_help[officer_slot].skill_help_x[skill_index] = -1000;
                    OFFICER::_officer_skill_help[officer_slot].skill_id[skill_index] = 0;
                }
            } else {
                OFFICER::Add_Skill_Description_Help_Fields_(
                    0x5e,
                    y,
                    (uint32_t)MOX::_PLAYER_NUM,
                    officer_id,
                    (int16_t*)&OFFICER::_officer_skill_help[officer_slot]
                );
            }

            y = static_cast<int16_t>(y + 0x6d);
            ++officer_slot;
        }
    }

    void __cdecl Wrapper_For_Offscrn_System_Popup_(int16_t system_id) {
        if (system_id > -1) {
            if (SYS::Ok_To_View_System_(system_id, MOX::_PLAYER_NUM)) {
                SYS::Init_System_Display_(system_id);
            }
        }

        SYS::Draw_System_Display_Popup_(
            system_id,
            0,
            s__00551100,
            306 /* 0x132 */,
            17 /* 0x11 */,
            0,
            0,
            3,
            3
        );
    }

    /**
     * @address 00442370
     */
    bool __cdecl Leaders_In_Hiring_Pool_() {
        bool is_fleet_screen = Is_Fleet_Officer_Screen_();
        bool found = false;

        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            const auto& leader = MOX::_leaders[i];

            bool type_match;
            if (is_fleet_screen) {
                type_match = (leader.type == 0);
            } else {
                type_match = (leader.type == 1);
            }

            if (type_match && leader.status == 4 && 
                static_cast<int16_t>(leader.player_index) == MOX::_PLAYER_NUM) {
                found = true;
                break;
            }
        }

        return found;
    }

    void __cdecl Wrapper_For_Offscrn_Data_(int16_t officer_list_index, int16_t officer_id) {
        // Calculations use 16-bit intermediate as per IMUL CX, ADD CX sequence
        int16_t scaled = officer_list_index * 0x6D;

        Print_Officer_Data_(
            officer_list_index,
            officer_id,
            0x5C,
            (int32_t)(scaled + 0x26),
            8,
            (int32_t)(scaled + 0x22),
            0x53,
            0x60,
            0xCC,
            3
        );
    }

    void __cdecl Get_Officer_Costs_(int16_t officer_idx, int16_t* out_cost, int16_t* out_maintenance) {
        int16_t player_num = MOX::_PLAYER_NUM;

        *out_cost = static_cast<int16_t>(Officer_Cost_(player_num, officer_idx));

        *out_maintenance = Officer_Maintenance_(player_num, officer_idx);

        if (Officer_Has_General_Skill_(player_num, officer_idx, 0x1c) ||
            Officer_Has_General_Skill_(player_num, officer_idx, 0x1d)) {
            *out_maintenance = 0;
        }
    }

    void __cdecl Give_Leader_Specials_To_Player_(int16_t leader_idx, int16_t player_idx) {
        if (leader_idx == 47) {
            int16_t gov_type = static_cast<int16_t>(Get_Advanced_Government_Type_(player_idx));
            int16_t tech_app = COLCALC::Get_Govt_Type_Tech_App_(gov_type);

            if (COLCALC::Player_May_Get_Tech_App_(&MOX::_player[player_idx], tech_app)) {
                COLCALC::Player_Gets_Tech_App_(&MOX::_player[player_idx], tech_app);
            }
        } else {
            for (int i = 0; i < 3; ++i) {
                uint8_t tech_app = MOX::_leaders[leader_idx].tech_application[i];
                if (tech_app != 0) {
                    if (COLCALC::Player_May_Get_Tech_App_(&MOX::_player[player_idx], (int32_t)tech_app)) {
                        COLCALC::Player_Gets_Tech_App_(&MOX::_player[player_idx], (int16_t)tech_app);
                    }
                }
            }
        }

        if (MOX::_leaders[leader_idx].type == 0) {
            bool has_skill = Officer_Has_Special_Skill_(player_idx, leader_idx, 18);
            if (!has_skill) {
                has_skill = Officer_Has_Special_Skill_(player_idx, leader_idx, 19);
            }

            if (has_skill) {
                MOX::_player[player_idx].has_galactic_lore = 1;
            }
        }
    }

    void __cdecl Set_Officer_To_Player_(int16_t officer_idx, int16_t player_idx, uint8_t is_instant) {
        int16_t count = 0;
        int16_t replace_idx = -1;
        int16_t max_eta = 0;

        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            if (i != officer_idx) {
                auto& leader = MOX::_leaders[i];
                if (leader.player_index == (int8_t)player_idx) {
                    if (leader.status != -2 && leader.status != -1) {
                        if (MOX::_leaders[officer_idx].type == leader.type) {
                            if (leader.status == 4 && (int16_t)leader.eta > max_eta) {
                                replace_idx = i;
                                max_eta = (int16_t)leader.eta;
                            }
                            count++;
                        }
                    }
                }
            }
        }

        if (count == 4) {
            if (replace_idx == -1) {
                return;
            }
            Deassign_Officer_(replace_idx);
        }

        auto& target = MOX::_leaders[officer_idx];
        target.eta = 0;
        target.player_index = (int8_t)player_idx;
        target.status = (is_instant == 0) ? 4 : 0;

        if (is_instant != 0) {
            Give_Leader_Specials_To_Player_(officer_idx, player_idx);
        }

        if (target.type == 1) {
            COLCALC::Do_Player_Colony_Calculations_(player_idx);
        }
    }

    void __cdecl Print_Galmap_Scanned_Ship_(int16_t icon_idx) {
        int16_t node_idx = MOX::_ship_icon[icon_idx].node_idx;
        int16_t ship_idx = MOX::_ship_node[node_idx].ship_idx;
        s_ship_data* ship = &MOX::_ship[ship_idx];
        int16_t ship_x = ship->x;
        int16_t ship_y = ship->y;
        int16_t owner = (int16_t)ship->owner;

        fonts::Set_Outline_Color_(0, 0);

        int16_t print_x, print_y, print_width;
        if (MOX::_current_screen == SCREEN_OFFICERS) {
            print_x = 0x16C;
            print_y = 0x19C;
            print_width = 0xCB;
        } else {
            print_x = 0x43;
            print_y = 0xF8;
            print_width = 0xC7;
        }

        char race_name[256];
        race_name[0] = '\0';

        if (node_idx >= 5000) {
            int16_t adjusted_node_idx = node_idx - 0x137F;
            adjusted_node_idx = HACCESS::Ship_Stack_Owner_(adjusted_node_idx);
            HACCESS::Race_Name_(adjusted_node_idx, race_name);
        } else if (owner > 8) {
            int16_t adjusted_node_idx = HACCESS::Ship_Stack_Owner_(node_idx);
            HACCESS::Race_Name_(adjusted_node_idx, race_name);
        }

        if (race_name[0] != '\0') {
            strings::String_To_Upper_(race_name);

            s_colors* p_colors = MISC::Font_Colors2_(4, 'L', 'O');
            s_colors local_colors;
            memcpy(local_colors.colors, p_colors->colors, 8);

            fonts::Set_Font_Style_Outline_(4, &local_colors);
            fonts::Print_Centered_(print_x + 102, print_y + 5, race_name);
        } else {
            int16_t n_unknown, n_colony_ships, n_outposts, n_transports;
            int16_t hull_counts[6];

            SYS::N_Ships_By_Type_At_Location_(ship_x, ship_y, owner, &n_unknown, &n_colony_ships, &n_outposts, &n_transports, hull_counts);

            uint8_t player_palette_color = 0x49; // 'I'
            if (owner < MAX_PLAYERS) {
                uint8_t player_color = MOX::_player[owner].color;
                player_palette_color = MOX::_main_palette_player_colors[player_color];
            }

            s_colors mox_colors;
            MISC::Get_Mox_Font_Colors_(1, player_palette_color, player_palette_color + 5, &mox_colors);

            s_colors special_colors;
            for (int i = 1; i < 8; ++i) {
                mox_colors.colors[i] += 2;
                special_colors.colors[i] = mox_colors.colors[i] + 2;
            }

            fonts::Set_Font_Special_Colors_(&special_colors);
            fonts::Set_Font_Style_Outline_(1, &mox_colors);
            fonts::Set_Font_Spacing_(1);

            HACCESS::Race_Name_(owner, race_name);

            char construction_buffer[512];
            char temp_sprintf_buf[256];

            sprintf(construction_buffer, HAROLD::H_Message_(HESTR_131_S_FLEET), race_name);
            strings::String_To_Upper_(construction_buffer);

            if (n_outposts > 0) {
                int16_t msg_id = (n_outposts == 1) ? HESTR_102_D_OUTPOST : HESTR_103_D_OUTPOSTS;
                sprintf(temp_sprintf_buf, HAROLD::H_Message_(msg_id), (int)n_outposts);
                strcat(construction_buffer, temp_sprintf_buf);
            }

            if (n_transports > 0) {
                int16_t msg_id = (n_transports == 1) ? HESTR_104_D_TRANSPORT : HESTR_105_D_TRANSPORTS;
                sprintf(temp_sprintf_buf, HAROLD::H_Message_(msg_id), (int)n_transports);
                strcat(construction_buffer, temp_sprintf_buf);
            }

            if (n_colony_ships > 0) {
                int16_t msg_id = (n_colony_ships == 1) ? HESTR_106_D_COLONY_SHIP : HESTR_107_D_COLONY_SHIPS;
                sprintf(temp_sprintf_buf, HAROLD::H_Message_(msg_id), (int)n_colony_ships);
                strcat(construction_buffer, temp_sprintf_buf);
            }

            for (int i = 0; i < 6; ++i) {
                if (hull_counts[i] > 0) {
                    const char* hull_name = (hull_counts[i] == 1) ? TECHDATA::_hull_data[i].name : TECHDATA::_hull_data[i].size_name;
                    sprintf(temp_sprintf_buf, "%d %s, ", (int)hull_counts[i], hull_name);
                    strcat(construction_buffer, temp_sprintf_buf);
                }
            }

            size_t len = strlen(construction_buffer);
            if (len > 2) {
                construction_buffer[len - 2] = '\0';
            }

            int16_t text_height = FMTPARA::Get_Formatted_Paragraph_Max_Height_(print_width, construction_buffer);
            if (text_height > 21) {
                fonts::Set_Font_Special_Colors_(&special_colors);
                fonts::Set_Font_Style_Outline_(0, &mox_colors);
            }

            ERIC::Print_Paragraph_Centered_(print_x, print_y, print_width, 22, construction_buffer);
        }
    }

    void __cdecl Load_Fleet_Galaxy_Stars_() {
        for (int i = 0; i < 11; ++i) {
            MOX::_fleet_galaxy_star_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(
                PTR_s_OFFICER_LBX,
                0x82 + i,
                MOX::_screen_seg
            );
        }
    }

    /**
     * Loads 27 skill icons from the OFFICER.LBX starting at entry 0x58 (88).
     * These icons are stored in the global _officer_skill_icon_seg array.
     */
    void __cdecl Load_Officer_Skill_Icons_(void* buffer) {
        for (int i = 0; i < 27; ++i) {
            MOX::_officer_skill_icon_seg[i] = (s_animation_header*)(
                farload::Far_Reload_Next_(
                    (const char*)PTR_s_OFFICER_LBX,
                    0x58 + i,
                    buffer
                )
            );
        }
    }

    void __cdecl Reload_Officer_Screen_Pictures_() {
        bitmap::Get_Bitmap_Size_(100, 100);

        auto& player = MOX::_player[MOX::_PLAYER_NUM];
        int8_t home_star_idx = MOX::_planet[player.home_planet_id].star_index;

        RUSS::Mox_Update_();
        MAINSCR::Allocate_First_Screen_Seg_(5);
        FLT1::Allocate_Fltscrn_Next_Screen_Seg_();

        MOX::_officer_pictures_screen_seg = allocate::Allocate_Next_Block_(MOX::_screen_seg, 85000);
        
        _ldrpict = (s_officer_picture_slot*)allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(*_ldrpict) * 8);
        memset(_ldrpict, 0, sizeof(*_ldrpict) * 8);

        _prev_leaders = static_cast<s_prev_leader_data*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xF71));

        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            _prev_leaders[i].status = MOX::_leaders[i].status;
            _prev_leaders[i].location = MOX::_leaders[i].location;
            _prev_leaders[i].eta = static_cast<short>(MOX::_leaders[i].eta);

            if (MOX::_leaders[i].type == 1 /* Ship Captain */) {
                if (_prev_leaders[i].status == 0 || _prev_leaders[i].status == 4) {
                    _prev_leaders[i].location = home_star_idx;
                }
            }
        }

        MOX::_officers_original_ship_id = static_cast<int16_t*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x10C));
        MOX::_officers_original_star_id = static_cast<int16_t*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x10C));

        Build_Captain_Id_List_();
        RUSS::Mox_Update_();

        const char* lbx = PTR_s_OFFICER_LBX;
        MOX::_officer_main_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 0, MOX::_screen_seg));
        MOX::_officer_fleet_box_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 1, MOX::_screen_seg));
        MOX::_officer_colony_box_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 2, MOX::_screen_seg));
        MOX::_officer_colony_ldrs_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 3, MOX::_screen_seg));
        MOX::_officer_ship_ldrs_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 4, MOX::_screen_seg));
        MOX::_officer_up_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 5, MOX::_screen_seg));
        
        RUSS::Mox_Update_();

        MOX::_officer_dn_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 6, MOX::_screen_seg));
        MOX::_officer_prev_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 7, MOX::_screen_seg));
        MOX::_officer_next_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 8, MOX::_screen_seg));
        MOX::_officer_hire_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 9, MOX::_screen_seg));
        MOX::_officer_pool_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 10, MOX::_screen_seg));
        MOX::_officer_dismiss_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 11, MOX::_screen_seg));
        MOX::_officer_return_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 12, MOX::_screen_seg));
        MOX::_officer_cancel_hire_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 13, MOX::_screen_seg));

        RUSS::Mox_Update_();

        MOX::_selected_box_seg = (s_animation_header*)farload::Far_Reload_Next_(lbx, 14, MOX::_screen_seg);
        MOX::_scanned_box_seg = (s_animation_header*)farload::Far_Reload_Next_(lbx, 15, MOX::_screen_seg);
        MOX::_fleet_scroll_anim_seg = (s_animation_header*)farload::Far_Reload_Next_(lbx, 16, MOX::_screen_seg);
        MOX::_officer_hire_replace_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 17, MOX::_screen_seg));
        MOX::_dull_officer_hire_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 18, MOX::_screen_seg));
        MOX::_dull_officer_pool_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 19, MOX::_screen_seg));
        MOX::_dull_officer_dismiss_button_seg = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(lbx, 20, MOX::_screen_seg));

        RUSS::Mox_Update_();

        MOX::_officer_normal_window_seg = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x8D, MOX::_screen_seg);
        MOX::_officer_bright_window_seg = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x8E, MOX::_screen_seg);

        RUSS::Mox_Update_();

        MOX::_temp_officers_pict = (s_animation_header**)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x30);

        Get_Officer_Pictures_();
        Load_Officer_Skill_Name_Descriptions_();
        Load_Officer_Skill_Icons_(MOX::_screen_seg);
        Reload_Offscrn_Small_Ship_Icons_();
        Load_Fleet_Galaxy_Stars_();
    }

    bool __cdecl Player_Can_Hire_More_Officers_(int16_t player_idx) {
        int16_t fleet_officers = N_Fleet_Officers_Owned_By_Player_(player_idx);
        int16_t star_officers = N_Star_Officers_Owned_By_Player_(player_idx);

        return (static_cast<int32_t>(fleet_officers) + static_cast<int32_t>(star_officers)) < 8;
    }

    /**
     * @address 00442cd0
     */
    void __cdecl Generate_Random_Officer_(int16_t player_index) {
        int16_t leader_index = Select_Leader_For_Hire_(player_index);

        MOX::_player[player_index].officer_for_hire = (int8_t)leader_index;

        if (MOX::_player[player_index].officer_for_hire > -1) {
            MOX::_player[player_index].stardate_last_offered_hero = static_cast<uint16_t>(MOX::_stardate);

            MOX::_leaders[leader_index].player_index = (int8_t)player_index;
        }
    }

    void __cdecl Random_Officer_Check_(int16_t player_idx) {
        bool should_generate = false;
        int16_t chance = Chance_To_Hire_Hero_(player_idx);

        if (Player_Can_Hire_More_Officers_(player_idx)) {
            if (chance > 0) {
                // game_random::Random_(100) returns 1..100
                if ((static_cast<int16_t>(game_random::Random_(100)) - 1) < chance) {
                    should_generate = true;
                }
            }

            if (should_generate) {
                Generate_Random_Officer_(player_idx);
            }
        }
    }

    /**
     * Checks if the officer's status is valid/active.
     *
     * @param officer_index The index into the _leaders array.
     * @return True if the officer status is 0, 1, 3, or 4; false otherwise.
     */
    bool __cdecl Officer_Status_Ok_(int16_t officer_index) {
        int8_t status = MOX::_leaders[officer_index].status;

        // Valid statuses are 0, 1, 3, or 4 based on disassembly branching logic
        if (status == 0 || status == 1 || status == 4 || status == 3) {
            return true;
        }

        return false;
    }

    /**
     * @brief Asserts or validates that leaders who are marooned at special systems 
     * (Orion or Antares-type systems) are correctly associated with their location.
     */
    void __cdecl Assert_Marooned_Leaders_() {
        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            int8_t special = MOX::_star[star_idx].system_special;

            // Checks for Orion (8) or specific player-range systems (80 to 80 + _NUM_PLAYERS)
            if (special == SYSTEM_SPECIAL_MAROONED_HERO || (special >= 80 && (special - 80) < MOX::_NUM_PLAYERS)) {
                bool found = false;
                // Iterate through all possible leaders (67 slots)
                for (int leader_idx = 0; leader_idx < MAX_LEADERS; ++leader_idx) {
                    if (found) {
                        break;
                    }

                    // Check if leader status is 5 (marooned) and location matches current star
                    if (MOX::_leaders[leader_idx].status == 5 &&
                        MOX::_leaders[leader_idx].location == star_idx) {
                        found = true;
                    }
                }
            }
        }
    }

    /**
     * Counts the number of fleet officers owned by a specific player whose status is OK.
     * @param player_index The index of the player to check for.
     * @return The count of matching fleet officers.
     */
    int16_t __cdecl N_Fleet_Officers_Owned_By_Player_(int16_t player_index) {
        int16_t count = 0;

        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            // Check if leader belongs to the player and is of type fleet (0)
            if ((int16_t)MOX::_leaders[i].player_index == player_index && MOX::_leaders[i].type == 0) {
                if (Officer_Status_Ok_(i)) {
                    count++;
                }
            }
        }

        return count;
    }

    /**
     * Counts the number of star officers owned by a specific player that have a valid status.
     * @param player_index The index of the player to check for.
     * @return The count of valid star officers owned by the player.
     */
    int16_t __cdecl N_Star_Officers_Owned_By_Player_(int16_t player_index) {
        int16_t count = 0;

        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            // Check if the leader belongs to the specified player and is of type 1 (Star Officer)
            if (MOX::_leaders[i].player_index == (int8_t)player_index && MOX::_leaders[i].type == 1) {
                if (Officer_Status_Ok_(i)) {
                    count++;
                }
            }
        }

        return count;
    }

    /**
     * Iterates through all officers and grants a level/XP increment if they are hired.
     */
    int8_t __cdecl Officer_Made_Level_Report_(int16_t player_idx, int8_t process_popups) {
        int8_t found = 0;

        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            // Check if a report was found in previous iteration
            if (found != 0) {
                return 1;
            }

            s_leader_data* leader = &MOX::_leaders[i];
            bool is_active = false;

            // Logic to determine if the officer/leader is active or in a pool
            if (((leader->status == 1) && (leader->location != -1)) || (leader->status == 0)) {
                is_active = true;
            }

            if (is_active && (leader->player_index == (int8_t)player_idx) && (leader->display_level_popup != 0)) {
                if (process_popups != 0) {
                    int16_t loc = leader->location;

                    // If leader is an Officer (type 0) and assigned to a ship
                    if (leader->type == 0 && loc > -1) {
                        loc = MOX::_ship[loc].location;

                        // Resolve ship location to an absolute map index
                        if (loc < MAX_SHIPS || loc >= 1000) {
                            loc = HAROLD::Absolute_Location_(loc);
                        } else {
                            // Location is in-transit or invalid for report
                            loc = -1;
                        }
                    }

                    MAINPUPS::Officer_Gains_Level_Popup_(loc, i);
                    leader->display_level_popup = 0;
                }
                found = 1;
            }
        }

        return found;
    }

    void __cdecl Check_For_Officer_Level_() {
        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            s_leader_data& leader = MOX::_leaders[i];

            // A status of 0 or 1 indicates the leader is in active service (hired or standby)
            if ((leader.status == 1 || leader.status == 0) && (int8_t)leader.player_index != -1) {
                Give_Officer_XP_(static_cast<int16_t>(leader.player_index), i, 1);
            }
        }
    }

    int16_t __cdecl Select_Leader_For_Hire_(int16_t player_idx) {
        int16_t selected_leader_idx = -1;
        int16_t attempts = 0;
        bool found = false;

        int16_t n_fleet_officers = N_Fleet_Officers_Owned_By_Player_(player_idx);
        int16_t n_star_officers = N_Star_Officers_Owned_By_Player_(player_idx);
        int32_t delta_stardate = antarans::Delta_Stardate_();

        // Initial candidate pool limit calculation
        int16_t limit = (int16_t)MOX::_NUM_PLAYERS + 10 + (int16_t)(delta_stardate / 5);

        s_player* player = &MOX::_player[player_idx];

        // Trait modifiers: Charismatic adds to the pool, Repulsive halves it.
        if (player->traits[TRAIT_CHARISMATIC] != 0) {
            limit += 10;
        } else if (player->traits[TRAIT_REPULSIVE] != 0) {
            limit /= 2;
        }

        // Limit is capped at 67 (typically the total number of hireable leaders)
        if (limit > MAX_LEADERS) {
            limit = MAX_LEADERS;
        }

        do {
            int32_t roll = game_random::Random_((int32_t)limit);
            int16_t leader_idx = (int16_t)roll - 1;

            bool type_match = false;
            char leader_type = MOX::_leaders[leader_idx].type;

            // Check if leader type (Fleet=0 or Star=1) fits current player slots (max 4 of each)
            if ((leader_type == 0 && n_fleet_officers < 4) || 
                (leader_type == 1 && n_star_officers < 4)) {
                type_match = true;
            }

            if (type_match) {
                if (Leader_Available_For_Hire_(leader_idx)) {
                    found = true;
                    selected_leader_idx = leader_idx;
                }
            }

            attempts++;
            if (attempts > 100) {
                found = true;
            }
        } while (!found);

        return selected_leader_idx;
    }

    /**
     * Calculates the indices of the player, leader, and ship pointers,
     * determines the ETA for assigning the leader to the ship,
     * and performs the assignment.
     */
    void __cdecl Ai_Assign_Leader_To_Ship_(s_player* player, s_leader_data* leader, s_ship_data* ship) {
        int16_t ship_idx = static_cast<int16_t>(ship - MOX::_ship);
        int16_t leader_idx = static_cast<int16_t>(leader - MOX::_leaders);
        int16_t player_idx = static_cast<int16_t>(player - MOX::_player);

        int8_t eta = Get_Ship_Leader_ETA_(player_idx, leader_idx, ship_idx);

        Assign_Leader_To_Ship_(player, leader, ship, eta);
    }

    int16_t __cdecl Chance_To_Hire_Hero_(int16_t player_idx) {
        int16_t n_fleet_officers = N_Fleet_Officers_Owned_By_Player_(player_idx);
        int16_t n_star_officers = N_Star_Officers_Owned_By_Player_(player_idx);

        if (antarans::Delta_Stardate_() < 5) {
            return 0;
        }

        int16_t chance;
        uint16_t last_offered = MOX::_player[player_idx].stardate_last_offered_hero;
        if (last_offered == 0) {
            chance = (int16_t)antarans::Delta_Stardate_() + 1;
        } else {
            chance = (int16_t)MOX::_stardate - (int16_t)last_offered + 1;
        }

        if (MOX::_player[player_idx].traits[TRAIT_CHARISMATIC]) {
            chance += 5;
        }
        if (MOX::_player[player_idx].traits[TRAIT_REPULSIVE]) {
            chance -= 10;
        }

        for (int16_t leader_idx = 0; leader_idx < MAX_LEADERS; ++leader_idx) {
            int16_t level_plus_1;
            if (MOX::_leaders[leader_idx].player_index == -1) {
                level_plus_1 = Get_Officer_Level_(leader_idx) + 1;
            } else {
                level_plus_1 = Owned_Officer_Level_(leader_idx) + 1;
            }

            if (MOX::_leaders[leader_idx].player_index == player_idx) {
                int8_t status = MOX::_leaders[leader_idx].status;
                if (status == 0 || status == 1) {
                    if (Officer_Has_General_Skill_(player_idx, leader_idx, 10 /* Famous */)) {
                        chance += level_plus_1;
                    } else if (Officer_Has_General_Skill_(player_idx, leader_idx, 11 /* Reputation */)) {
                        chance += (level_plus_1 * 15) / 10;
                    }
                }
            }
        }

        int32_t n_officers = (int32_t)n_star_officers + (int32_t)n_fleet_officers + 1;
        if (n_officers < 1) {
            n_officers = 1;
        }

        return (int16_t)((int32_t)chance / n_officers);
    }

    void __cdecl Kill_Officer_(int16_t leader_idx) {
        s_leader_data* leader = &MOX::_leaders[leader_idx];
        int16_t loc = leader->location;

        if (loc > -1) {
            if (leader->type == 1) {
                int16_t player_idx = (int16_t)leader->player_index;
                if (player_idx > -1 && player_idx < MAX_PLAYERS) {
                    MOX::_star[loc].officer_index[player_idx] = -1;
                }
            } else {
                MOX::_ship[loc].officer_index = -1;
            }
        }

        leader->status = -2;
        leader->eta = 0;
        leader->location = -1;

        if (leader->type == 1) {
            COLCALC::Do_Player_Colony_Calculations_((int16_t)leader->player_index);
        }
    }

    s_animation_header* __cdecl Load_Officer_Picture_(
        int32_t unused,
        int16_t leader_idx,
        s_animation_header* dest,
        uint8_t use_far_reload,
        int16_t pic_type
    ) {
        int16_t pic_offset = 0;

        if (pic_type == 0) {
            pic_offset = 0x15;
        } else if (pic_type == 1) {
            pic_offset = 0xD2;
        } else if (pic_type == 2) {
            pic_offset = 0x115;
        }

        int32_t frame_index = pic_offset + static_cast<uint8_t>(MOX::_leaders[leader_idx].pict_num);

        if (use_far_reload == 0) {
            return (s_animation_header*)farload::Far_Reload_Next_(PTR_s_OFFICER_LBX, frame_index, dest);
        } else {
            return (s_animation_header*)farload::Far_Reload_(PTR_s_OFFICER_LBX, frame_index, dest);
        }
    }

    void __cdecl Initialize_Scroll_Bar_(s_scroll_bar* sb) {
        sb->visible_rows = 3;
        sb->track_len_px = 0x79;

        int16_t height = SCROLL::Get_Vertical_Scroll_Bar_Height_(sb);

        sb->thumb_height_px = height;
        sb->last_thumb_center_y = -1;
        sb->type = 2;
        sb->row_step = 1;

        // Division logic (CDQ; SUB EAX, EDX; SAR EAX, 1) implements signed height / 2
        sb->thumb_center_y = sb->track_len_px - (height / 2);
    }

    void __cdecl Move_Officer_To_Pool_(int16_t leader_idx) {
        int16_t location = MOX::_leaders[leader_idx].location;
        int8_t player_idx = MOX::_leaders[leader_idx].player_index;

        if (location != -1 && MOX::_leaders[leader_idx].status != 5) {
            if (MOX::_leaders[leader_idx].type == 0) {
                if (MOX::_ship[location].officer_index == leader_idx) {
                    MOX::_ship[location].officer_index = -1;
                }
            } else if (MOX::_leaders[leader_idx].type == 1) {
                MOX::_star[location].officer_index[player_idx] = -1;
            }
        }

        MOX::_leaders[leader_idx].status = 0;
        MOX::_leaders[leader_idx].eta = 0;
        MOX::_leaders[leader_idx].location = -1;

        if (MOX::_leaders[leader_idx].type == 1) {
            COLCALC::Do_Player_Colony_Calculations_((int16_t)MOX::_leaders[leader_idx].player_index);
        }
    }

    void __cdecl Build_Captain_Id_List_() {
        int16_t count = 0;

        for (int16_t i = 0; i < MAX_LEADERS; i++) {
            MOX::_officer_id_list[i] = -1;
        }

        for (int16_t i = 0; i < MAX_LEADERS; i++) {
            bool is_valid = (MOX::_leaders[i].status >= 0 || MOX::_leaders[i].status == 4);
            bool is_type_0 = (MOX::_leaders[i].type == 0);
            bool is_type_1 = (MOX::_leaders[i].type == 1);
            bool is_fleet_screen = Is_Fleet_Officer_Screen_();

            if (is_valid && MOX::_leaders[i].player_index == MOX::_PLAYER_NUM) {
                if ((is_fleet_screen && is_type_0) || (!is_fleet_screen && is_type_1)) {
                    MOX::_officer_id_list[count] = i;
                    count++;
                    if (count >= 4) {
                        break;
                    }
                }
            }
        }

        MOX::_n_fltscrn_officers = count;
    }

    void __cdecl Get_Officer_Pictures_() {
        int16_t picture_slot_index = 0;

        if (MOX::_n_fltscrn_officers > 0) {
            do {
                const int16_t officer_id = MOX::_officer_id_list[picture_slot_index];

                if (officer_id > -1) {
                    s_animation_header* picture = OFFICER::Load_Officer_Picture_(
                        (int32_t)MOX::_PLAYER_NUM,
                        officer_id,
                        reinterpret_cast<s_animation_header*>(MOX::_officer_pictures_screen_seg),
                        static_cast<uint8_t>(picture_slot_index == 0),
                        0);

                    OFFICER::_ldrpict[picture_slot_index].pic = picture;
                    OFFICER::_ldrpict[picture_slot_index].officer_id = MOX::_officer_id_list[picture_slot_index];

                    picture = OFFICER::Load_Darkened_Officer_Picture_(
                        MOX::_PLAYER_NUM,
                        officer_id,
                        MOX::_officer_pictures_screen_seg,
                        0);

                    OFFICER::_ldrpict[picture_slot_index + 4].pic = picture;
                    OFFICER::_ldrpict[picture_slot_index + 4].officer_id = MOX::_officer_id_list[picture_slot_index];
                }

                ++picture_slot_index;
            } while (picture_slot_index < MOX::_n_fltscrn_officers);
        }
    }

    void __cdecl Reload_Offscrn_Small_Ship_Icons_() {
        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);

        int16_t resource_idx = 0x73;

        for (int i = 0; i < 8; ++i) {
            int32_t current_idx = resource_idx;
            resource_idx++;

            MOX::_fltscrn_small_ship_icon_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(
                PTR_s_OFFICER_LBX, current_idx, MOX::_screen_seg);

            MOX::_fltscrn_flashing_ship_pict_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(
                PTR_s_OFFICER_LBX, current_idx, MOX::_screen_seg);
        }

        for (int j = 0; j < 7; ++j) {
            int32_t current_idx = resource_idx;
            resource_idx++;

            MOX::_fltscrn_monster_ship_icon[j] = (s_animation_header*)farload::Far_Reload_Next_(
                PTR_s_OFFICER_LBX, current_idx, MOX::_screen_seg);

            MOX::_fltscrn_flashing_monster_ship_icon[j] = (s_animation_header*)farload::Far_Reload_Next_(
                PTR_s_OFFICER_LBX, current_idx, MOX::_screen_seg);
        }

        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);
    }

    void __cdecl Load_Officer_Skill_Name_Descriptions_() {
        char lbx_name[20];

        Get_Skill_Description_LBX_Name_(lbx_name);

        for (int16_t i = 0; i < 27; ++i) {
            _officer_skill_name_string[i] = (char*)farload::Far_Reload_Next_Data_(
                lbx_name, 0, MOX::_screen_seg, i, 1, 0x26);
        }

        RUSS::Mox_Update_();

        for (int16_t i = 0; i < 27; ++i) {
            _officer_skill_description_string[i] = (char*)farload::Far_Reload_Next_Data_(
                lbx_name, 1, MOX::_screen_seg, i, 1, 0xFF);
        }
    }

    void __cdecl Add_Officer_Screen_Fields_(
        int16_t* hire_btn,
        int16_t* cancel_hire_btn,
        int16_t* ship_ldrs_btn,
        int16_t* colony_ldrs_btn,
        int16_t* return_btn,
        int16_t* help_field_1,
        int16_t* help_field_2,
        int16_t* up_btn,
        int16_t* dn_btn,
        int16_t* next_btn,
        int16_t* prev_btn,
        int16_t* big_icon_fields
    ) {
        bool is_fleet_officer_screen = Is_Fleet_Officer_Screen_();
        bool leaders_in_pool = Leaders_In_Hiring_Pool_();
        bool has_officers = MOX::_n_fltscrn_officers > 0;

        if (_officer_button_mode != 0 && has_officers) {
            _officer_dismiss_btn = fields::Add_Hidden_Field_(
                0x1cf, 
                0x1b9, 
                animate::Get_Width_(MOX::_officer_dismiss_button_seg) + 0x1cf, 
                animate::Get_Height_(MOX::_officer_dismiss_button_seg) + 0x1b9, 
                "D",
                0x29
            );
        } else {
            _officer_dismiss_btn = -1000;
        }

        if (_officer_button_mode != 0 && has_officers) {
            _officer_pool_btn = fields::Add_Hidden_Field_(
                0x184, 
                0x1b9, 
                animate::Get_Width_(MOX::_officer_pool_button_seg) + 0x184, 
                animate::Get_Height_(MOX::_officer_pool_button_seg) + 0x1b9, 
                "P",
                0x29
            );
        } else {
            _officer_pool_btn = -1000;
        }

        if (leaders_in_pool && _officer_button_mode != 0) {
            *hire_btn = fields::Add_Button_Field_(
                0x139, 
                0x1b9, 
                s__00551100,
                MOX::_officer_hire_button_seg, 
                "H",
                0x28
            );
        } else {
            *hire_btn = -1000;
        }

        if (_officer_button_mode == 0 && leaders_in_pool) {
            *cancel_hire_btn = fields::Add_Button_Field_(
                0x1cc, 
                0x1b7, 
                s__00551100,
                MOX::_officer_cancel_hire_button_seg, 
                "X", 
                0x28
            );
        } else {
            *cancel_hire_btn = -1000;
        }

        *ship_ldrs_btn = fields::Add_Hidden_Field_(
            0x9c, 
            0xb, 
            animate::Get_Width_((s_animation_header*)MOX::_officer_ship_ldrs_button_seg) + 0x9c, 
            animate::Get_Height_((s_animation_header*)MOX::_officer_ship_ldrs_button_seg) + 0xb, 
            "S", 
            0x29
        );

        *colony_ldrs_btn = fields::Add_Hidden_Field_(
            0x9, 
            0xb, 
            animate::Get_Width_((s_animation_header*)MOX::_officer_colony_ldrs_button_seg) + 0x9, 
            animate::Get_Height_((s_animation_header*)MOX::_officer_colony_ldrs_button_seg) + 0xb, 
            "C",
            0x29
        );

        *return_btn = fields::Add_Button_Field_(
            0x21a, 
            0x1b9, 
            s__00551100,
            MOX::_officer_return_button_seg, 
            "\x1B", 
            0x28
        );

        Wrapper_For_Offscrn_Help_Fields_();

        int16_t y = 0x22;
        for (int16_t i = 0; i < 4; i++) {
            if (i < MOX::_n_fltscrn_officers) {
                help_field_1[i] = fields::Add_Hidden_Field_(
                    0x5c, 
                    y, 
                    0x128, 
                    y + 0x6c, 
                    s__00551100,
                    0x29
                );
                help_field_2[i] = fields::Add_Hidden_Field_(
                    0x8, 
                    y, 
                    0x5b, 
                    y + 0x60, 
                    s__00551100,
                    0x29
                );
            } else {
                help_field_1[i] = -1000;
                help_field_2[i] = -1000;
            }
            y += 0x6d;
        }

        *up_btn = -1000;
        *dn_btn = -1000;
        *big_icon_fields = -1000;

        if (is_fleet_officer_screen) {
            *up_btn = fields::Add_Button_Field_(
                0x265, 
                0x16, 
                s__00551100,
                MOX::_officer_up_button_seg, 
                "-", 
                0x28
            );
            *dn_btn = fields::Add_Button_Field_(
                0x265, 
                0xaa, 
                s__00551100,
                MOX::_officer_dn_button_seg, 
                "+", 
                0x28
            );
            FLT2::Fleet_Screen_Big_Icon_Fields_(big_icon_fields, 0x264, 0x2d, 0xa, 0x79);
            MOX::_scroll_bar->total_rows = (short)MOX::_n_fltscrn_rows;
        } else {
            Add_Display_Popup_Fields_(_officer_star_displayed);
        }

        MOX::_scroll_bar->up_btn = *up_btn;
        MOX::_scroll_bar->dn_btn = *dn_btn;

        *next_btn = fields::Add_Button_Field_(
            0x238, 
            0xcd, 
            s__00551100,
            MOX::_officer_next_button_seg, 
            ".",
            0x28
        );

        *prev_btn = fields::Add_Button_Field_(
            0x147, 
            0xcd, 
            s__00551100,
            MOX::_officer_prev_button_seg, 
            ",",
            0x28
        );

        MAINSCR::_debug_field = fields::Add_Hidden_Field_(
            0x0, 
            0x1d6, 
            0xa, 
            0x1df, 
            s__00551100,
            0x29
        );

        FLT2::Add_Fltscrn_Small_Icon_Fields_();

        int16_t local_2;
        MOVEBOX::Add_Galaxy_Map_Fields_2_(
            0x132, 
            0xeb, 
            0x13e, 
            0xa9, 
            0, 
            0, 
            0, 
            0, 
            1, 
            &local_2, 
            MOX::_galaxy_map_star_field, 
            &MOX::_galaxy_map_grid_field
        );

        fields::Add_Hidden_Field_(
            0x0, 
            0x0, 
            0x27f, 
            0x1df, 
            s__00551100,
            0x0
        );
    }

    void __cdecl Give_Officer_XP_(int16_t player_idx, int16_t leader_idx, int16_t xp_amount) {
        int16_t current_level;
        int16_t new_level;

        if (MOX::_leaders[leader_idx].player_index == -1) {
            current_level = Get_Officer_Level_(leader_idx);
        } else {
            current_level = OFFICER::Owned_Officer_Level_(leader_idx);
        }

        MOX::_leaders[leader_idx].xp += xp_amount;

        if (MOX::_leaders[leader_idx].xp > 30000) {
            MOX::_leaders[leader_idx].xp = 30000;
        }

        if (MOX::_leaders[leader_idx].player_index == -1) {
            new_level = Get_Officer_Level_(leader_idx);
        } else {
            new_level = OFFICER::Owned_Officer_Level_(leader_idx);
        }

        if (new_level != current_level) {
            if (MOX::_leaders[leader_idx].type == 1 && MOX::_leaders[leader_idx].location != -1) {
                COLCALC::Star_Colony_Calculation_(MOX::_leaders[leader_idx].location, player_idx);
            }
            MOX::_leaders[leader_idx].display_level_popup = 1;
        }
    }

    bool __cdecl Officer_Is_Female_(int16_t officer_idx) {
        if (officer_idx == 10 ||
            officer_idx == 22 ||
            officer_idx == 34 ||
            officer_idx == 37 ||
            officer_idx == 39 ||
            officer_idx == 60) {
            return true;
        }

        return false;
    }

    void __cdecl Add_Display_Popup_Fields_(int16_t system_id) {
        int16_t local_4;
        int16_t local_2;

        SYS::Add_System_Display_Popup_Fields_(system_id, 0x132, 0x11, 0, 0, 0, 0, &local_4, nullptr, &local_2);
    }

    void __cdecl Print_ETA_On_Officer_Picture_(int16_t leader_idx, int16_t x, int16_t y, int16_t w) {
        char buffer[20];

        int8_t eta = MOX::_leaders[leader_idx].eta;

        fonts::Set_Font_Style_Outline_Heavy_(2, &_red_colors);
        fonts::Set_Outline_Color_(0, 0);

        int16_t font_height = fonts::Get_Font_Height_();
        int16_t text_y = y - (font_height / 2) + 45;

        const char* format = HAROLD::H_Message_(303); /* 0x12F */
        sprintf(buffer, format, static_cast<int>(eta));

        int16_t text_w = fonts::Get_String_Width_(buffer);
        int16_t text_x = x + (w / 2 - text_w / 2);
        int16_t text_end_x = text_x + text_w;

        fonts::Print_(text_x, text_y, buffer);

        line::Line_(text_x, text_y - 4, text_end_x, text_y - 4, 129);
        line::Line_(text_x, text_y - 3, text_end_x, text_y - 3, 75);
        line::Line_(text_x, text_y + font_height, text_end_x, text_y + font_height, 75);
        line::Line_(text_x, text_y + font_height + 1, text_end_x, text_y + font_height + 1, 129);
    }

    char* __cdecl Leader_Name_(int16_t leader_idx, char include_title, void* star_ptr) {
        (void)star_ptr;
        char title_prefix[200];
        int16_t level;

        if (MOX::_leaders[leader_idx].player_index == -1) {
            level = Get_Officer_Level_(leader_idx);
        } else {
            level = Owned_Officer_Level_(leader_idx);
        }

        MOX::_temp_string[0] = '\0';

        if (MOX::_leaders[leader_idx].type == '\0') {
            strcpy(MOX::_temp_string, MOX::_officer_level_names[level]);
        } else {
            strcpy(MOX::_temp_string, MOX::_star_officer_level_names[level]);
        }

        strcat(MOX::_temp_string, " ");
        strcat(MOX::_temp_string, MOX::_leaders[leader_idx].name);

        if (include_title == '\0') {
            return MOX::_temp_string;
        }

        if (MOX::_leaders[leader_idx].title[0] == '\0') {
            return MOX::_temp_string;
        }

        if (MOX::_settings.language == 4) {
            Get_Italian_The_(leader_idx, title_prefix);
        } else {
            if (MOX::_settings.language == 2 && MISC::Is_Vowel_(MOX::_leaders[leader_idx].title[0])) {
                MISC::Get_LBX_Message_(0x186, title_prefix);
            } else {
                if (!Officer_Is_Female_(leader_idx)) {
                    MISC::Get_LBX_Message_(0x110, title_prefix);
                } else {
                    MISC::Get_LBX_Message_(0x182, title_prefix);
                }
            }
        }

        strcat(MOX::_temp_string, title_prefix);
        strcat(MOX::_temp_string, MOX::_leaders[leader_idx].title);

        return MOX::_temp_string;
    }

    void __cdecl Officer_Name_(char* dst, int16_t player_id, int16_t officer_idx) {
        strcpy(dst, Leader_Name_(officer_idx, 0));
    }

    void __cdecl Find_Selected_Leader_(int16_t leader_idx) {
        char msg_buf1[200];
        char msg_buf2[200];
        char msg_buf3[200];

        int16_t location = MOX::_leaders[leader_idx].location;
        char type = MOX::_leaders[leader_idx].type;

        msg_buf2[0] = '\0';
        msg_buf3[0] = '\0';

        if (location == -1) {
            int16_t msg_idx;
            if (MOX::_leaders[leader_idx].status == 0) {
                msg_idx = HESTR_111_S_IS_IN_THE_OFFICER_PO;
            } else {
                msg_idx = HESTR_112_S_DOES_NOT_HAVE_A_LOCA;
            }
            strcpy(msg_buf1, HAROLD::H_Message_(msg_idx));
            sprintf(msg_buf2, msg_buf1, Leader_Name_(leader_idx, 0));
        }

        if (msg_buf2[0] == '\0') {
            if (type == 0) {
                if (MOX::_ship[location].status == 4) {
                    int16_t abs_loc = HAROLD::Absolute_Location_(MOX::_ship[location].location);
                    strcpy(msg_buf1, HAROLD::H_Message_(HESTR_113_S_IS_LOCATED_ON_S_WHIC));
                    sprintf(msg_buf2, msg_buf1, Leader_Name_(leader_idx, 0), &MOX::_ship[location], &MOX::_star[abs_loc]);
                } else {
                    int16_t stack_id = HACCESS::Ship_Stack_From_Ship_Id_(location);
                    if (stack_id == -1) {
                        strcpy(msg_buf1, HAROLD::H_Message_(HESTR_114_S_CANNOT_BE_LOCATED));
                        sprintf(msg_buf2, msg_buf1, Leader_Name_(leader_idx, 0));
                    } else {
                        bool diff = MOX::_small_ship_stack_ptr != stack_id;
                        MOX::_small_ship_stack_ptr = stack_id;
                        _fleet_icon_ship_id_to_move = location;
                        if (diff) {
                            sprintf(msg_buf3, HAROLD::H_Message_(HESTR_115_TRANSFERING_DISPLAY_TO), &MOX::_ship[location]);
                        }
                    }
                }
            } else {
                _officer_star_chosen = HAROLD::Absolute_Location_(location);
                bool diff = _officer_star_displayed != _officer_star_chosen;
                _officer_star_displayed = _officer_star_chosen;
                if (diff) {
                    sprintf(msg_buf3, HAROLD::H_Message_(HESTR_116_TRANSFERING_DISPLAY_TO), &MOX::_star[_officer_star_chosen]);
                }
            }
        }

        if (msg_buf2[0] != '\0') {
            Officer_Screen_User_Box_(msg_buf2, 3);
        } else if (msg_buf3[0] != '\0') {
            MOX::_global_var = 0;
            if (MOX::_settings.game_difficulty != 0) {
                MOX::_global_var = 0xf;
            }
            Officer_Screen_User_Box_(msg_buf3, 5);
        }
    }

    int16_t __cdecl Assign_Loknar_Ship_To_Player_(int16_t player_idx) {
        int16_t ship_idx = -1;

        if (MOX::_NUM_SHIPS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
                if (ship_idx != -1) {
                    break;
                }
                if (HACCESS::Ship_Owner_(i) == 9) {
                    ship_idx = i;
                }
            }
        }

        if (ship_idx > -1 && ship_idx < MOX::_NUM_SHIPS) {
            s_ship_design design;
            INITSHIP::Load_Loknar_Ship_Design_(&design);

            s_ship_data new_ship;
            memset(&new_ship, 0, sizeof(s_ship_data));

            new_ship.d = design;

            new_ship.d.previous_owner = (uint8_t)player_idx;
            new_ship.owner = static_cast<char>(player_idx);
            new_ship.location = 0;
            new_ship.x = MOX::_star[0].x;
            new_ship.y = MOX::_star[0].y;
            new_ship.status = 0;
            new_ship.officer_index = 0x41;

            MOX::_ship[ship_idx] = new_ship;
        }

        return ship_idx;
    }

int16_t __cdecl Get_Advanced_Government_Type_(int16_t player_idx) {
    switch (static_cast<int32_t>(MOX::_player[player_idx].traits[TRAIT_CURRENT_GOVERNMENT]) / 2) {
    case 0:
        return 1;
    case 1:
        return 3;
    case 2:
        return 5;
    case 3:
        return 7;
    default:
        return 0;
    }
}

void __cdecl Give_Player_Loknar_Special_(int16_t player_idx, int16_t* techs, int16_t* num_techs) {
    bool dont_assign = false;
    int16_t ship_idx = Assign_Loknar_Ship_To_Player_(player_idx);

    if (ship_idx == -1) {
        dont_assign = true;
    }

    if (N_Fleet_Officers_Owned_By_Player_(player_idx) >= 4) {
        dont_assign = true;
    }

    if (dont_assign) {
        MOX::_leaders[0x41].location = -1;
        MOX::_leaders[0x41].player_index = -1;
        MOX::_leaders[0x41].status = 6;
        if (ship_idx != -1) {
            MOX::_ship[ship_idx].officer_index = -1;
        }
    } else {
        if (ship_idx != -1) {
            MOX::_leaders[0x41].location = ship_idx;
            MOX::_leaders[0x41].player_index = static_cast<int8_t>(player_idx);
            MOX::_leaders[0x41].status = 1;
            Give_Leader_Specials_To_Player_(0x41, player_idx);
        }
    }

    *num_techs = 0;

    if (COLCALC::Player_May_Get_Tech_App_(&MOX::_player[player_idx], 0x2F)) {
        COLCALC::Player_Gets_Tech_App_(&MOX::_player[player_idx], 0x2F);
        techs[*num_techs] = 0x2F;
        *num_techs += 1;
    }

    Get_Random_Antaran_Tech_(player_idx, 3, techs, num_techs);
    MAINSCR::Update_Ship_Icons_In_Between_Turns_();
}

void __cdecl Make_Loknar_Message_(char* output, const int16_t* tech_app_ids, int16_t tech_app_count) {
  const int16_t message_id = (MOX::_leaders[0x41].status == 6) ? 0x25 : 0x26;

  strcpy(output, HAROLD::H_Message_(message_id));

  if (tech_app_count > 0) {
    /* fmtpara \x1A0 restores left alignment before each bullet line in the reward list. */
    for (int16_t tech_index = 0; tech_index < tech_app_count; ++tech_index) {
      strcat(output, "\x1A" "0");
      strcat(output, "^ ");
      strcat(output, TECHDATA::_technology_applications[tech_app_ids[tech_index]].name);
      strcat(output, string::s_Newline);
    }
  }
}

void __cdecl Rescued_Loknar_Popup_(int16_t unused, int16_t* tech_app_ids, int16_t tech_app_count) {
    char buffer[500];
    (void)unused;

    if (tech_app_count > 0) {
        Make_Loknar_Message_(buffer, tech_app_ids, tech_app_count);
        HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, buffer, 0);
        MOX::_FULL_DRAW_SCREEN_FLAG = 1;
    }
}

void __cdecl Get_Random_Antaran_Tech_(int16_t player_idx, int16_t count, int16_t* tech_list, int16_t* num_techs) {
    int16_t available_techs[TECH_APP_COUNT];
    int16_t available_count = 0;

    memset(available_techs, -1, sizeof(available_techs));

    Get_Available_Antaran_Tech_(player_idx, available_techs, &available_count);

    if (available_count < count) {
        count = available_count;
    }

    while (count != 0) {
        int32_t rnd = (int32_t)game_random::Random_((uint32_t)available_count);
        int16_t tech_id = available_techs[rnd - 1];

        COLCALC::Player_Gets_Tech_App_(&MOX::_player[player_idx], tech_id);

        available_count--;
        int16_t last_tech = available_techs[available_count];

        tech_list[*num_techs] = tech_id;
        (*num_techs)++;
        count--;

        available_techs[rnd - 1] = last_tech;
    }
}

int8_t __cdecl Leader_Available_For_Hire_(int16_t leader_idx) {
    if (MOX::_leaders[leader_idx].player_index == -1 &&
        leader_idx != 0x41 &&
        leader_idx != 0x42 &&
        MOX::_leaders[leader_idx].status == -1) {
        return 1;
    }
    return 0;
}

uint8_t __cdecl Hire_Officer_Popup_(int16_t player_num, int16_t officer_id, char* message) {
    MAINPUPS::_g_officer = officer_id;
    MAINPUPS::_g_str_ptr = message;
    MAINPUPS::_on_officer_screen_flag = 1;

    uint8_t result = (uint8_t)MAINPUPS::Random_New_Officer_Popup_(player_num);

    MAINPUPS::_on_officer_screen_flag = 0;
    return result;
}

int32_t __cdecl Calculate_Officer_Cost_(int16_t player_idx, int16_t leader_idx) {
    return Officer_Cost_(player_idx, leader_idx);
}

bool __cdecl Marooned_Hero_At_Star_Can_Join_(int16_t star_idx, int16_t* hero_idx, int16_t player_idx) {
    bool found = false;
    int16_t deassign_idx = -1;
    bool can_join = false;

    *hero_idx = -1;

    for (int16_t i = 0; i < MAX_LEADERS; i++) {
        if (found) {
            break;
        }

        if (MOX::_leaders[i].status == 5 && MOX::_leaders[i].location == star_idx) {
            int16_t count;
            if (MOX::_leaders[i].type == 0) {
                count = N_Fleet_Officers_Owned_By_Player_(player_idx);
            } else {
                count = N_Star_Officers_Owned_By_Player_(player_idx);
            }

            *hero_idx = i;
            can_join = true;
            found = true;

            if (count >= 4) {
                can_join = false;
                deassign_idx = i;
            }
        }
    }

    if (deassign_idx != -1) {
        Deassign_Officer_(deassign_idx);
        return can_join;
    }

    if (*hero_idx != -1) {
        MOX::_leaders[*hero_idx].player_index = static_cast<int8_t>(player_idx);
        MOX::_leaders[*hero_idx].location = -1;
        Set_Officer_To_Player_(*hero_idx, player_idx, 1);
        Move_Officer_To_Pool_(*hero_idx);
    }

    return can_join;
}

void __cdecl Ai_Assign_Leader_To_Star_(s_player* player, s_leader_data* leader, s_star_data* star) {
    int16_t player_idx = static_cast<int16_t>(player - MOX::_player);
    int16_t leader_idx = static_cast<int16_t>(leader - MOX::_leaders);
    int16_t star_idx = static_cast<int16_t>(star - MOX::_star);

    int8_t eta = Get_Star_Leader_ETA_(player_idx, leader_idx, star_idx);
    Assign_Leader_To_Star_(player, leader, star, eta);
}

/**
 * Select the localized LBX file name for officer skill text.
 * @param lbx_name Receives the selected 8.3 LBX file name.
 */
void __cdecl Get_Skill_Description_LBX_Name_(char* lbx_name) {
    const char* skill_description_lbx_name = "SKILDESC.LBX";

    switch (MOX::_settings.language) {
    case 1:
        skill_description_lbx_name = "GERSKLLS.LBX";
        break;
    case 2:
        skill_description_lbx_name = "FRESKLLS.LBX";
        break;
    case 3:
        skill_description_lbx_name = "SPASKLLS.LBX";
        break;
    case 4:
        skill_description_lbx_name = "ITASKLLS.LBX";
        break;
    default:
        break;
    }

    memcpy(lbx_name, skill_description_lbx_name, 13);
}

void __cdecl Get_Available_Antaran_Tech_(int16_t player_idx,
                                        int16_t *available_techs,
                                        int16_t *available_count) {
  *available_count = 0;

  for (int16_t tech_idx = 0; tech_idx < TECH_APP_COUNT; ++tech_idx) {
    if (TECHDATA::_technology_applications[tech_idx].tech_field_id == TECH_FIELD_XENON_TECHNOLOGY) {
      if (COLCALC::Player_May_Get_Tech_App_(&MOX::_player[player_idx], tech_idx)) {
        available_techs[*available_count] = tech_idx;
        ++(*available_count);
      }
    }
  }
}

void __cdecl Get_Italian_The_(int16_t leader_idx, char* output) {
    output[0] = '\0';

    if (MISC::Is_Vowel_(MOX::_leaders[leader_idx].title[0])) {
        strcpy(output, HAROLD::H_Message_(HESTR_186_S));
        return;
    }

    if (leader_idx == 0x1a || leader_idx == 0x2b || leader_idx == 0x34) {
        strcpy(output, ", lo ");
        return;
    }

    if (Officer_Is_Female_(leader_idx)) {
        strcpy(output, HAROLD::H_Message_(0x182 /* HESTR_182_THE */));
        return;
    }

    strcpy(output, HAROLD::H_Message_(0x110 /* HESTR_110_THE */));
}

s_animation_header* __cdecl Load_Darkened_Officer_Picture_(int16_t player_num, int16_t leader_idx, void* buffer, uint8_t load_now) {
    const int16_t image_id = static_cast<int16_t>(static_cast<uint8_t>(MOX::_leaders[leader_idx].pict_num) + 0x8f);

    if (load_now != 0) {
        return (s_animation_header*)farload::Far_Reload_(PTR_s_OFFICER_LBX, image_id, buffer);
    }

    return (s_animation_header*)farload::Far_Reload_Next_(PTR_s_OFFICER_LBX, (int)image_id, buffer);
}

void __cdecl Wrapper_For_Colony_Officer_Popup_Data_(int16_t x, int16_t y, int16_t officer_idx) {
    _offscrn_ask_for_hire_popup = 1;
    Print_Officer_Data_(-1, officer_idx, x + 0x5f, y + 5, x + 10, y + 2, 0x50, 0x5a, 0xc3, 0xe);
    _offscrn_ask_for_hire_popup = 0;
}

void __cdecl Get_N_Skills_Per_Officer_Displayed_(int16_t* skill_counts) {
    for (int16_t i = 0; i < 4; ++i) {
        int16_t officer_idx = MOX::_officer_id_list[i];
        if (officer_idx > -1) {
            int16_t player_idx = MOX::_leaders[officer_idx].player_index;
            char type = MOX::_leaders[officer_idx].type;

            if (type == 0) {
                for (int16_t skill_idx = 0; skill_idx < 54; ++skill_idx) {
                    if ((MOX::_skill_data[skill_idx].skill_type == 0 && OFFICER::Officer_Has_General_Skill_(player_idx, officer_idx, skill_idx)) ||
                        (MOX::_skill_data[skill_idx].skill_type == 1 && OFFICER::Officer_Has_Special_Skill_(player_idx, officer_idx, skill_idx))) {
                        skill_counts[i]++;
                    }
                }
            } else if (type == 1) {
                for (int16_t skill_idx = 0; skill_idx < 54; ++skill_idx) {
                    if ((MOX::_skill_data[skill_idx].skill_type == 0 && OFFICER::Officer_Has_General_Skill_(player_idx, officer_idx, skill_idx)) ||
                        (MOX::_skill_data[skill_idx].skill_type == 2 && OFFICER::Officer_Has_Special_Skill_(player_idx, officer_idx, skill_idx))) {
                        skill_counts[i]++;
                    }
                }
            }
        }
    }
}

void __cdecl Draw_Officer_Skill_Icon_(int16_t x, int16_t y, int16_t skill_idx) {
    animate::Draw_(x, static_cast<int16_t>(y - 4), MOX::_officer_skill_icon_seg[skill_idx]);
}

void __cdecl Print_Officer_Data_(
    int16_t officer_list_index,
    int16_t officer_id,
    int16_t text_x,
    int16_t text_y,
    int16_t portrait_x,
    int16_t portrait_y,
    int16_t portrait_width,
    int16_t portrait_status_offset_y,
    int16_t right_column_offset_x,
    int16_t unk_10
) {
    s_leader_data& leader = MOX::_leaders[officer_id];
    const int16_t leader_location = leader.location;
    const int16_t officer_status = static_cast<int16_t>(leader.status);
    const uint16_t officer_type = leader.type;
    const int16_t officer_eta = static_cast<int16_t>(leader.eta);

    int16_t status4_cost = 0;
    int16_t other_cost = 0;
    s_colors text_colors{};
    char text_buffer[80];
    char status_text[80];
    char money_unit[24];
    char dot_text[3];
    int16_t current_y = text_y;

    OFFICER::Get_Officer_Costs_(officer_id, &status4_cost, &other_cost);
    fonts::Set_Outline_Color_(0, 0);

    if (MOX::_current_screen == SCREEN_OFFICERS) {
        if (officer_list_index > -1 &&
            MOX::_officer_id_list[officer_list_index] ==
                static_cast<int16_t>(MOX::_officer_scanned)) {
            text_colors = OFFICER::_selected_text_colors;
        } else if (officer_list_index > -1 &&
                   MOX::_officer_id_list[officer_list_index] ==
                       static_cast<int16_t>(MOX::_officer_selected)) {
            text_colors = OFFICER::_selected_text_colors;
        } else {
            text_colors = OFFICER::_normal_text_colors;
        }
    } else {
        text_colors = OFFICER::_normal_text_colors;
    }

    if (MOX::_current_screen == SCREEN_OFFICERS && OFFICER::_offscrn_ask_for_hire_popup == 0) {
        sprintf(text_buffer, "%s", OFFICER::Leader_Name_(officer_id, false));
        fonts::Set_Font_Style_Outline_(3, &text_colors);
        fonts::Set_Font_Spacing_(1);

        const int16_t name_width = fonts::Get_String_Width_(text_buffer);
        const int16_t name_x =
            static_cast<int16_t>((0xa0 - name_width) / 2 + 0x16 + text_x);

        fonts::Print_(name_x, current_y, text_buffer);
    }

    if (MOX::_current_screen == SCREEN_OFFICERS && OFFICER::_offscrn_ask_for_hire_popup == 0) {
        text_colors.colors[0] = ';';
        fonts::Set_Font_Style_(0, &text_colors);

        if (officer_status == 4) {
            const char* label = HAROLD::H_Message_(0x108);
            const int16_t label_width = fonts::Get_String_Width_(label);

            HAROLD::Get_Monetary_Unit_String_(money_unit);
            sprintf(text_buffer, "%d %s", static_cast<int32_t>(status4_cost), money_unit);

            fonts::Print_Right_(
                static_cast<int16_t>(text_x + right_column_offset_x - 2),
                current_y,
                text_buffer);

            fonts::Print_(
                static_cast<int16_t>(text_x + right_column_offset_x - label_width - 2),
                static_cast<int16_t>(current_y + fonts::Get_Font_Height_()),
                label);
        } else {
            const char* label = HAROLD::H_Message_(0x109);
            const int16_t label_width = fonts::Get_String_Width_(label);

            if (other_cost == 0) {
                strcpy(text_buffer, HAROLD::H_Message_(0x10a));
            } else {
                HAROLD::Get_Monetary_Unit_String_(money_unit);
                sprintf(text_buffer, "%d %s", static_cast<int32_t>(other_cost), money_unit);
            }

            const int16_t label_x =
                static_cast<int16_t>(text_x + right_column_offset_x - label_width - 2);

            fonts::Print_(label_x, current_y, text_buffer);
            fonts::Print_(label_x, static_cast<int16_t>(current_y + fonts::Get_Font_Height_()), label);
        }

        text_colors.colors[0] = '=';
    }

    status_text[0] = '\0';

    if (officer_status == 0) {
        fonts::Set_Font_Style_(2, &text_colors);
        strcpy(status_text, HAROLD::H_Message_(0x10b));
    } else if (officer_status == 2) {
        fonts::Set_Font_Style_(2, &text_colors);
        strcpy(status_text, HAROLD::H_Message_(0x10c));
    } else if (officer_status == 1) {
        fonts::Set_Font_Style_(2, &text_colors);

        if (officer_type == 0 && leader_location > -1) {
            strcpy(status_text, MOX::_ship[leader_location].d.name);
        } else if (officer_type == 1 && leader_location > -1) {
            strcpy(status_text, MOX::_star[leader_location].name);
        }
    } else if (officer_status == 4) {
        int16_t unused_cost_a = 0;
        int16_t unused_cost_b = 0;

        OFFICER::Get_Officer_Costs_(officer_id, &unused_cost_a, &unused_cost_b);

        if (MOX::_current_screen == SCREEN_OFFICERS) {
            sprintf(
                status_text,
                HAROLD::H_Message_(0x10d),
                0x1e - static_cast<int32_t>(leader.eta));
        } else {
            strcpy(status_text, HAROLD::H_Message_(0x10e));
        }

        fonts::Set_Font_Style_(2, &OFFICER::_red_colors);
    }

    if (MOX::_current_screen == SCREEN_OFFICERS && OFFICER::_offscrn_ask_for_hire_popup == 0) {
        int16_t centered_status_y =
            static_cast<int16_t>(portrait_y + portrait_status_offset_y + 1);
        s_colors saved_colors{};

        fonts::Get_Colors_(&saved_colors);
        const int16_t fitted_spacing = MISC::Get_Font_Spacing_();
        MISC::Set_Fitted_Font_Style_(
            status_text,
            &saved_colors,
            fonts::_current_font_style,
            fitted_spacing,
            4,
            0x4A,
            1);

        const int32_t vertical_adjust = ((-12 - fonts::Get_Font_Height_()) / 2) + 12;
        if (vertical_adjust < 0) {
            centered_status_y =
                static_cast<int16_t>(centered_status_y + vertical_adjust);
        }

        fonts::Print_Centered_(
            static_cast<int16_t>(portrait_x + portrait_width / 2),
            centered_status_y,
            status_text);
    }

    status_text[0] = '\0';

    if (officer_eta > 0 && officer_status == 1) {
        OFFICER::Print_ETA_On_Officer_Picture_(
            officer_id, portrait_x, portrait_y, portrait_width);
    }

    fonts::Set_Font_Style_Outline_(3, &text_colors);
    fonts::Set_Font_Spacing_(1);

    if (MOX::_current_screen == SCREEN_OFFICERS && OFFICER::_offscrn_ask_for_hire_popup == 0) {
        current_y = static_cast<int16_t>(current_y + 0x10);
    }

    memcpy(dot_text, " .", 3);
    fonts::Get_String_Width_(dot_text);
    OFFICER::Get_Skill_Data_Max_Pixel_Width_(officer_id);

    const int16_t skill_column_x = static_cast<int16_t>(text_x + 2);

    for (int16_t skill_idx = 0; skill_idx < 0x36; ++skill_idx) {
        bool has_skill = false;

        if (officer_type == 0) {
            if (MOX::_skill_data[skill_idx].skill_type == 1 &&
                OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, officer_id, skill_idx)) {
                has_skill = true;
            }
        } else if (officer_type == 1) {
            if (MOX::_skill_data[skill_idx].skill_type == 2 &&
                OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, officer_id, skill_idx)) {
                has_skill = true;
            }
        }

        if (has_skill) {
            OFFICER::Draw_Officer_Skill_Icon_(skill_column_x, current_y, skill_idx / 2);

            const int16_t name_x = static_cast<int16_t>(skill_column_x + 0x16);
            sprintf(text_buffer, "%s ", MOX::_skill_data[skill_idx].name);
            fonts::Print_(name_x, current_y, text_buffer);
            fonts::Get_String_Width_(text_buffer);

            const int16_t bonus =
                static_cast<int16_t>(OFFICER::Officer_Skill_Bonus_(MOX::_PLAYER_NUM, officer_id, skill_idx));

            sprintf(
                text_buffer,
                MOX::_skill_data[skill_idx].format_str,
                static_cast<int32_t>(bonus));

            const int16_t value_x =
                (MOX::_current_screen == SCREEN_OFFICERS)
                    ? static_cast<int16_t>(text_x + right_column_offset_x - 3)
                    : static_cast<int16_t>(text_x + right_column_offset_x + 3);

            fonts::Print_Right_(value_x, current_y, text_buffer);
            fonts::Set_Font_Spacing_(1);
            current_y = static_cast<int16_t>(current_y + 0x11);
        }
    }

    for (int16_t skill_idx = 0; skill_idx < 0x36; ++skill_idx) {
        bool has_skill = false;

        if (MOX::_skill_data[skill_idx].skill_type == 0 &&
            OFFICER::Officer_Has_General_Skill_(MOX::_PLAYER_NUM, officer_id, skill_idx)) {
            has_skill = true;
        }

        if (has_skill) {
            OFFICER::Draw_Officer_Skill_Icon_(skill_column_x, current_y, skill_idx / 2);

            const int16_t name_x = static_cast<int16_t>(skill_column_x + 0x16);
            sprintf(text_buffer, "%s ", MOX::_skill_data[skill_idx].name);
            fonts::Print_(name_x, current_y, text_buffer);
            fonts::Get_String_Width_(text_buffer);

            const int16_t bonus =
                static_cast<int16_t>(OFFICER::Officer_Skill_Bonus_(MOX::_PLAYER_NUM, officer_id, skill_idx));

            sprintf(
                text_buffer,
                MOX::_skill_data[skill_idx].format_str,
                static_cast<int32_t>(bonus));

            const int16_t value_x =
                (MOX::_current_screen == SCREEN_OFFICERS)
                    ? static_cast<int16_t>(text_x + right_column_offset_x - 3)
                    : static_cast<int16_t>(text_x + right_column_offset_x + 3);

            fonts::Print_Right_(value_x, current_y, text_buffer);
            fonts::Set_Font_Spacing_(1);
            current_y = static_cast<int16_t>(current_y + 0x11);
        }
    }
}

int16_t __cdecl Get_Skill_Data_Max_Pixel_Width_(int16_t leader_idx) {
    int16_t max_width = 0;
    char formatted_bonus[68];
    const int16_t player_idx = static_cast<int8_t>(MOX::_leaders[leader_idx].player_index);

    for (int16_t skill_idx = 0; skill_idx < 54; ++skill_idx) {
        const uint8_t skill_type = MOX::_skill_data[skill_idx].skill_type;
        bool has_skill = false;

        if (skill_type == 0) {
            has_skill = OFFICER::Officer_Has_General_Skill_(player_idx, leader_idx, skill_idx);
        } else if (skill_type >= 1 && skill_type <= 2) {
            has_skill = OFFICER::Officer_Has_Special_Skill_(player_idx, leader_idx, skill_idx);
        }

        if (!has_skill) {
            continue;
        }

        const int16_t bonus = (int16_t)OFFICER::Officer_Skill_Bonus_(static_cast<uint32_t>(player_idx), leader_idx, skill_idx);
        sprintf(formatted_bonus, MOX::_skill_data[skill_idx].format_str, static_cast<int>(bonus));

        const int16_t width = fonts::Get_String_Width_(formatted_bonus);
        if (width > max_width) {
            max_width = width;
        }
    }

    if (max_width == 0) {
        max_width = 20;
    }

    return max_width;
}

void __cdecl Wrapper_To_Add_Officer_Popup_Help_Fields_(
    int16_t skill_id_base,
    int16_t skill_count_minus_one,
    int16_t popup_param) {
    memset(OFFICER::_officer_skill_help, 0, sizeof(OFFICER::_officer_skill_help));

    OFFICER::Add_Skill_Description_Help_Fields_(
        skill_id_base + 0x61,
        skill_count_minus_one + 1,
        (uint16_t)MOX::_PLAYER_NUM,
        popup_param,
        (int16_t*)OFFICER::_officer_skill_help);
}

/**
 * Add hidden help fields for each visible officer skill row.
 * @param help_fields Receives five field IDs followed by five skill IDs.
 */
void __cdecl Add_Skill_Description_Help_Fields_(
    int16_t left,
    int16_t top,
    uint32_t unused,
    int16_t leader_index,
    int16_t* help_fields
) {
    (void)unused;

    uint16_t leader_type = MOX::_leaders[leader_index].type;
    int16_t slot = 0;
    int16_t current_y = top;

    for (int16_t skill_index = 0; skill_index < 54; ++skill_index) {
        bool add_field = false;

        if (leader_type == 0) {
            if (MOX::_skill_data[skill_index].skill_type == 1) {
                add_field = OFFICER::Officer_Has_Special_Skill_(
                    MOX::_PLAYER_NUM,
                    leader_index,
                    skill_index
                );
            }
        } else if (leader_type == 1) {
            if (MOX::_skill_data[skill_index].skill_type == 2) {
                add_field = OFFICER::Officer_Has_Special_Skill_(
                    MOX::_PLAYER_NUM,
                    leader_index,
                    skill_index
                );
            }
        }

        if (add_field) {
            help_fields[slot] = fields::Add_Hidden_Field_(
                left,
                static_cast<int16_t>(current_y - 1),
                static_cast<int16_t>(left + 0x00BE),
                static_cast<int16_t>(current_y + 0x000F),
                s__00551100,
                0x29
            );
            help_fields[slot + 5] = skill_index;
            current_y = static_cast<int16_t>(current_y + 0x11);
            ++slot;
        }
    }

    current_y = static_cast<int16_t>(top + slot * 0x11);

    for (int16_t skill_index = 0; skill_index < 54; ++skill_index) {
        bool add_field = false;

        if (MOX::_skill_data[skill_index].skill_type == 0) {
            add_field = OFFICER::Officer_Has_General_Skill_(
                MOX::_PLAYER_NUM,
                leader_index,
                skill_index
            );
        }

        if (add_field) {
            help_fields[slot] = fields::Add_Hidden_Field_(
                left,
                current_y,
                static_cast<int16_t>(left + 0x00BE),
                static_cast<int16_t>(current_y + 0x000F),
                s__00551100,
                0x29
            );
            help_fields[slot + 5] = skill_index;
            current_y = static_cast<int16_t>(current_y + 0x11);
            ++slot;
        }
    }

    while (slot < 5) {
        help_fields[slot] = static_cast<int16_t>(0xFC18);
        help_fields[slot + 5] = 0;
        ++slot;
    }
}

void __cdecl Wrapper_For_Officer_Popup_Help_Fields_(int16_t field_x, int16_t officer_idx) {
    int16_t skill_id = -1;

    for (int16_t skill_index = 0; skill_index < 5; ++skill_index) {
        if (skill_id != -1) {
            break;
        }

        if (static_cast<int32_t>(OFFICER::_officer_skill_help[0].skill_help_x[skill_index]) +
                static_cast<int32_t>(field_x) ==
            0) {
            skill_id = OFFICER::_officer_skill_help[0].skill_id[skill_index];
        }
    }

    if (skill_id == -1) {
        return;
    }

    allocate::Push_Block_(MOX::_screen_seg);
    OFFICER::Load_Officer_Skill_Name_Descriptions_();

    const int16_t skill_bonus =
        static_cast<int16_t>(OFFICER::Officer_Skill_Bonus_(static_cast<uint32_t>(MOX::_PLAYER_NUM), officer_idx, skill_id));

    OFFICER::Print_Officer_Skill_Help_(skill_id, skill_bonus, officer_idx);
    allocate::Pop_Block_(MOX::_screen_seg);
}

}