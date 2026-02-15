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

    /**
     * Calculates the display level (0-5) of an officer based on their experience 
     * and the player's Warlord trait.
     */
    int16_t __cdecl Owned_Officer_Level_(int leader_idx) {
        int8_t warlord = 0;

        // Leader index 0x42 (66) appears to be a special case (e.g., Orion) where traits don't apply
        if (leader_idx != 0x42 /* 66 */) {
            int8_t player_idx = MOX::_leaders[leader_idx].player_index;
            // Accessing the first trait (index 0), typically TRAIT_WARLORD
            warlord = MOX::_player[static_cast<int>(player_idx)].traits[TRAIT_WARLORD];
        }

        int16_t level = OFFICER::Get_Officer_Base_Level_(MOX::_leaders[leader_idx].xp, warlord);

        // Caps the reported level at 5
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
        int32_t cost = Officer_Cost_(player_num, leader_idx);
        int16_t maintenance = (int16_t)((cost + 99) / 100);

        bool has_maintenance_reduction_skill = Officer_Has_General_Skill_(player_num, leader_idx, 28 /* 0x1c */) ||
                                               Officer_Has_General_Skill_(player_num, leader_idx, 29 /* 0x1d */);

        if (leader_idx == 65 /* 0x41 */ || has_maintenance_reduction_skill) {
            return 0;
        }

        if (maintenance < 1) {
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

        _sprintf(formatted_msg, template_str, name, (int)costs, (int)upkeep);

        uint8_t result = Hire_Officer_Popup_(MOX::_PLAYER_NUM, leader_idx, formatted_msg);

        if (result != 0) {
            Hire_Officer_(MOX::_PLAYER_NUM, leader_idx);
            _officer_button_mode = -1;
        } else {
            if (_officer_button_mode == 0) {
                mouse::Set_Mouse_List_(reinterpret_cast<uint8_t*>(&DIP_SCRN::_diplomacy_hand_mouse), 1);
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
        int32_t cost = Officer_Cost_(MOX::_PLAYER_NUM, leader_idx);

        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);

        // Leader status 4 (0x4) generally represents being available for hire.
        s_leader_data* leader = &MOX::_leaders[leader_idx];

        if (leader->status == 4) {
            s_player* player = &MOX::_player[MOX::_PLAYER_NUM];
            if (player->bc < cost) {
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

        _sprintf(output_buffer, format_string, MOX::_player[MOX::_PLAYER_NUM].bc, leader_name, (int32_t)cost);

        Officer_Screen_User_Box_(output_buffer, 0);

        MOX::_officer_selected = -1;
    }

    void __cdecl Already_Hired_Message_(int16_t officer_id) {
        char format_buffer[200];
        char output_buffer[200];

        const char* localized_format = HAROLD::H_Message_(HESTR_122_S_IS_ALREADY_IN_YOUR_S);

        strcpy(format_buffer, localized_format);

        char* leader_name = Leader_Name_(officer_id, 1);

        _sprintf(output_buffer, format_buffer, leader_name);

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
                if (leader_id != -1) {
                    uint8_t* ldrpict_base = reinterpret_cast<uint8_t*>(OFFICER::_ldrpict);
                    s_animation_header* portrait_anim = *reinterpret_cast<s_animation_header**>(ldrpict_base + i * 6);
                    
                    int16_t anim_w = animate::Get_Width_(portrait_anim);
                    int16_t anim_h = animate::Get_Height_(portrait_anim);

                    s_leader_data& leader = MOX::_leaders[leader_id];
                    s_animation_header* draw_anim;
                    
                    if (leader.eta <= 0 && leader.status != 4) {
                        draw_anim = portrait_anim;
                    } else {
                        draw_anim = *reinterpret_cast<s_animation_header**>(ldrpict_base + i * 6 + 24);
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

        mouse::Set_Mouse_List_(mouse::_mouse_list_none, 1);
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

        mouse::Set_Mouse_List_(mouse::_mouse_list_arrow, 1);
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
                _sprintf(buffer, s_GEGGGGG_format, ship->d.name, leader->name);
            } else {
                const char* format;
                if (eta == 1) {
                    format = HAROLD::H_Message_(HESTR_092_S_S_D_TURN);
                } else {
                    format = HAROLD::H_Message_(HESTR_093_S_S_D_TURNS);
                }
                _sprintf(buffer, format, ship->d.name, leader->name, (int)eta);
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
        int8_t needs_help_reset = 0;
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

            Check_Officer_Fields_(input, scanned_id, (int16_t*)big_icons_scanned, (int16_t*)big_icons_ids);

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
                            if (MOX::_officer_selected > -1 && MOX::_officer_selected < 'C') {
                                if (Leader_Can_Be_Assigned_((int16_t)MOX::_officer_selected) == 0) {
                                    char buffer[200];
                                    const char* fmt = HAROLD::H_Message_(0x12D /* HESTR_12D_S_CANNOT_BE_PUT_INTO_S */);
                                    _sprintf(buffer, fmt, Leader_Name_((int16_t)MOX::_officer_selected, 0));
                                    Officer_Screen_User_Box_(buffer, 0);
                                } else {
                                    Assign_Officer_((int16_t)MOX::_officer_selected, MOX::_fltscrn_big_icon[(int16_t)b_idx].ship_idx);
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
        if (current_leader_idx > -1 && current_leader_idx < 67 /* 0x43 */) {
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
        if (current_officer_idx > -1 && current_officer_idx < 67 /* 0x43 */) {
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

    uint32_t __cdecl Scan_Offscrn_Big_Icon_Fields_(int16_t param_1, int16_t param_2, int16_t* param_3, int16_t* param_4) {
        int16_t start_idx = MOX::_scroll_bar->first_visible_row * static_cast<int16_t>(static_cast<int8_t>(MOX::_big_icon_display_columns));
        int16_t end_idx = start_idx + 20;

        if (end_idx > static_cast<int16_t>(MOX::_n_fltscrn_big_icons)) {
            end_idx = static_cast<int16_t>(MOX::_n_fltscrn_big_icons);
        }

        uint32_t result = 0xFFFF;
        bool found = false;
        *param_3 = -1;
        *param_4 = -1;

        if (start_idx < 0 || end_idx < 0) {
            found = true;
        }

        if (start_idx < end_idx) {
            for (int16_t i = start_idx; i < end_idx; ++i) {
                if (found) break;

                auto& icon = MOX::_fltscrn_big_icon[i];
                if (icon.ship_idx > -1) {
                    if (icon.field9_0x9 == param_2) {
                        result = 0;
                        found = true;
                        *param_4 = i;
                    }
                    else if (icon.field9_0x9 + param_2 == 0) {
                        result = 1;
                        found = true;
                        *param_4 = i;
                    }
                }
            }
        }

        if (static_cast<uint16_t>(result) == 0xFFFF) {
            if (start_idx < end_idx && !found) {
                for (int16_t i = start_idx; i < end_idx; ++i) {
                    auto& icon = MOX::_fltscrn_big_icon[i];
                    if (icon.field9_0x9 == param_1 && icon.ship_idx > -1) {
                        result = 4;
                        *param_3 = i;
                        break;
                    }
                }
            }
        }

        return result;
    }

    void __cdecl Check_Officer_Fields_(int16_t param_1, int16_t param_2, int16_t* param_3, int16_t* param_4) {
        bool is_fleet_screen = Is_Fleet_Officer_Screen_();
        int16_t field1, field2;
        int16_t scan_result = Scan_Officer_Fields_(param_3, param_4, param_2, param_1, &MOX::_officer_scanned, &field2, &field1);

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
                    if (mode != 0) {
                        MOX::_officer_selected = -1;
                    }
                } else if (mode == 2 /* Dismiss */) {
                    char format_str[200];
                    char final_msg[200];
                    strcpy(format_str, HAROLD::H_Message_(HESTR_126_DISMISS_S));
                    mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
                    _sprintf(final_msg, format_str, Leader_Name_(MOX::_officer_selected, 1));

                    if (Officer_Screen_User_Box_(final_msg, 1) != 0) {
                        Dismiss_Officer_(MOX::_officer_selected);
                        MOX::_officer_selected = -1;
                    } else {
                        mouse::Set_Mouse_List_(reinterpret_cast<uint8_t*>(&DIP_SCRN::_diplomacy_hand_mouse), 1);
                    }
                    MOX::_draw_officer_to_back = 1;
                } else if (mode == 1 /* Relocate */) {
                    if (MOX::_leaders[field2].status == 0 || MOX::_leaders[field2].status == 2) {
                        MOX::_officer_selected = -1;
                        _officer_button_mode = -1;
                        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
                    } else {
                        Move_Officer_To_Limbo_(field2);
                    }
                    MOX::_draw_officer_to_back = 1;
                } else if (is_fleet_screen) {
                    if (MOX::_offpop_big_icon_selected >= 0) {
                        int16_t ship_idx = MOX::_fltscrn_big_icon[MOX::_offpop_big_icon_selected].ship_idx;
                        if (Leader_Can_Be_Assigned_(field2) != 0) {
                            Assign_Officer_(MOX::_officer_selected, ship_idx);
                            MOX::_draw_officer_to_back = 1;
                            Reset_Officer_Selection_();
                        } else {
                            char format_str[200];
                            char final_msg[200];
                            strcpy(format_str, HAROLD::H_Message_(HESTR_127_S_MUST_BE_HIRED_BEFORE));
                            _sprintf(final_msg, format_str, Leader_Name_(MOX::_officer_selected, 0));
                            Officer_Screen_User_Box_(final_msg, 0);
                        }
                        MOX::_officer_selected = -1;
                    }
                } else if (_officer_star_chosen != -1 && HAROLD::Player_Has_Colony_(MOX::_PLAYER_NUM, _officer_star_chosen)) {
                    if (Leader_Can_Be_Assigned_(MOX::_officer_selected) != 0) {
                        if (MOX::_leaders[static_cast<uint8_t>(MOX::_officer_selected)].location != _officer_star_chosen) {
                            Assign_Officer_(MOX::_officer_selected, _officer_star_chosen);
                            MOX::_draw_officer_to_back = 1;
                        }
                    } else {
                        char format_str[200];
                        char final_msg[200];
                        if (MOX::_leaders[static_cast<uint8_t>(MOX::_officer_selected)].status == 4) {
                            strcpy(format_str, HAROLD::H_Message_(HESTR_127_S_MUST_BE_HIRED_BEFORE));
                            _sprintf(final_msg, format_str, Leader_Name_(MOX::_officer_selected, 0));
                        } else {
                            strcpy(format_str, HAROLD::H_Message_(HESTR_128_YOU_CANNOT_ASSIGN_S_TO));
                            _sprintf(final_msg, format_str, Leader_Name_(MOX::_officer_selected, 0), MOX::_star[_officer_star_chosen].name);
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
                MOX::_star[location].officer_index[(uint8_t)leader.player_index] = -1;
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

    // Suggested name: Hire_Officer
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

    void __cdecl Assign_Officer_(int16_t leader_idx, int16_t ship_idx) {
        char format_buf[200];
        char display_buf[200];
        int8_t eta;

        if (Is_Fleet_Officer_Screen_()) {
            if (HACCESS::Ship_Owner_(ship_idx) != MOX::_PLAYER_NUM) {
                strcpy(format_buf, HAROLD::H_Message_(HESTR_117_YOU_MAY_NOT_ASSIGN_S_T));
                _sprintf(display_buf, format_buf, Leader_Name_(leader_idx, 0));
                Officer_Screen_User_Box_(display_buf, 3);
                return;
            }

            eta = Get_Ship_Leader_ETA_(MOX::_PLAYER_NUM, leader_idx, ship_idx);
            s_ship_data* target_ship = &MOX::_ship[ship_idx];

            if (eta == 0) {
                strcpy(format_buf, HAROLD::H_Message_(HESTR_119_ASSIGN_S_TO_S));
                _sprintf(display_buf, format_buf, Leader_Name_(leader_idx, 0), target_ship);
            } else {
                strcpy(format_buf, HAROLD::H_Message_(HESTR_118_IT_WILL_TAKE_D_TURNS_F));
                if (MOX::_settings.language == 4) {
                    _sprintf(display_buf, format_buf, Leader_Name_(leader_idx, 0), (int)eta, target_ship);
                } else {
                    _sprintf(display_buf, format_buf, (int)eta, Leader_Name_(leader_idx, 0), target_ship);
                }
            }

            if (Officer_Screen_User_Box_(display_buf, 1)) {
                int16_t existing_leader = target_ship->officer_index;
                bool proceed = true;

                if (existing_leader != -1) {
                    s_leader_data* leader = &MOX::_leaders[existing_leader];
                    int16_t msg_id = (leader->eta < 1) ? HESTR_11B_S_CURRENTLY_ON_S_WILL_ : HESTR_11A_S_IN_ROUTE_TO_S_WILL_B;

                    strcpy(format_buf, HAROLD::H_Message_(msg_id));
                    _sprintf(display_buf, format_buf, Leader_Name_(existing_leader, 1), target_ship);
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
                _sprintf(display_buf, format_buf, Leader_Name_(leader_idx, 0), target_star);
            } else {
                strcpy(format_buf, HAROLD::H_Message_(HESTR_118_IT_WILL_TAKE_D_TURNS_F));
                if (MOX::_settings.language == 4) {
                    _sprintf(display_buf, format_buf, Leader_Name_(leader_idx, 0), (int)eta, target_star);
                } else {
                    _sprintf(display_buf, format_buf, (int)eta, Leader_Name_(leader_idx, 0), target_star);
                }
            }

            if (Officer_Screen_User_Box_(display_buf, 1)) {
                int16_t existing_leader = (int16_t)target_star->officer_index[MOX::_PLAYER_NUM];
                bool proceed = true;

                if (leader_idx != existing_leader && existing_leader != -1) {
                    s_leader_data* leader = &MOX::_leaders[existing_leader];
                    int16_t msg_id = (leader->eta < 1) ? HESTR_11B_S_CURRENTLY_ON_S_WILL_ : HESTR_11A_S_IN_ROUTE_TO_S_WILL_B;

                    strcpy(format_buf, HAROLD::H_Message_(msg_id));
                    _sprintf(display_buf, format_buf, Leader_Name_(existing_leader, 1), target_star);
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
            result = GENDRAW::Warning_Box_(const_cast<char*>(message));
        } else if (type == 3) {
            GENDRAW::Draw_Warning_Box_(message);
        } else if (type == 5) {
            int8_t duration = static_cast<int8_t>(MOX::_global_var & 0xFF);
            if (duration < 0) {
                duration = 0x7E;
            }

            *reinterpret_cast<int8_t*>(&MOX::_global_var) = duration;
            Timed_Text_Box_(nullptr, message, static_cast<int16_t>(duration));
            *reinterpret_cast<int8_t*>(&MOX::_global_var) = 0;
        } else {
            GENDRAW::Message_Box_(const_cast<char*>(message));
        }

        video::Set_Page_Off_();
        MOX::_draw_officer_to_back = 1;
        OFFICER::Draw_Officer_Screen_();
        RUSS::Mox_Sync_Update_();

        return result;
    }

    /**
     * @address 0043d750
     */
    void __cdecl Reset_Officer_Selection_() {
        // Reset the currently selected officer index
        MOX::_officer_selected = -1;

        // BUG: Accessing array with index -1 if no icon was selected
        // Reset the selection state of the big icon if one was selected.
        MOX::_fltscrn_big_icon[MOX::_offpop_big_icon_selected].selected = 0;

        // Reset the icon selection index
        MOX::_offpop_big_icon_selected = -1;
    }

    void __cdecl Print_Officer_Skill_Help_(int16_t skill_id, int16_t bonus, int16_t leader_idx) {
        char leader_name_buf[80];
        char skill_desc_buf[256];
        char skill_name_buf[80];

        char* leader_raw_name = Leader_Name_(leader_idx, 1);
        _sprintf(leader_name_buf, MOX::s_fmt_s, leader_raw_name);

        if (MOX::_settings.language != 4 && MOX::_leaders[leader_idx].title[0] != '\0') {
            strcat(leader_name_buf, ", ");
        }

        int16_t skill_value;
        if (skill_id == 28) {
            skill_value = 10;
        } else if (skill_id == 29) {
            skill_value = 15;
        } else {
            skill_value = bonus;
        }

        _sprintf(skill_desc_buf, _officer_skill_description_string[skill_id / 2], leader_name_buf, (int)skill_value);

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

            _sprintf(final_message, template_buffer, leader_name);

            Officer_Screen_User_Box_(final_message, 3);
            return;
        }

        int16_t location = leader.location;
        int8_t player_idx = leader.player_index;

        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);

        if (Is_Fleet_Officer_Screen_()) {
            if (location > -1 && location < static_cast<int16_t>(MOX::_NUM_SHIPS)) {
                MOX::_ship[location].officer_index = -1;
            }
        } else {
            if (location > -1) {
                MOX::_star[location].officer_index[(uint8_t)player_idx] = -1;
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
                if (static_cast<uint8_t>(MOX::_previous_screen) == 1) {
                    MOX::_officer_selected = -1;
                    OFFICER::_officer_star_displayed = DESIGN::_player_design_num;
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

        FLT2::Build_Fltscrn_Big_Icon_Fields_(MOX::_small_ship_stack_ptr, MOX::_scroll_bar->visible_rows);
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

    void __cdecl Wrapper_For_Offscrn_Data_(int16_t param_1, int16_t param_2) {
        // Calculations use 16-bit intermediate as per IMUL CX, ADD CX sequence
        int16_t scaled = param_1 * 0x6D;

        Print_Officer_Data_(
            param_1,
            param_2,
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
                if (leader.player_index == (char)player_idx) {
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
        target.player_index = (char)player_idx;
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
        int16_t owner = (int16_t)(uint8_t)ship->owner;

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

        int16_t adjusted_owner_idx = node_idx;
        if (node_idx >= 5000) {
            adjusted_owner_idx -= 0x137F;
            adjusted_owner_idx = HACCESS::Ship_Stack_Owner_(adjusted_owner_idx);
            HACCESS::Race_Name_(adjusted_owner_idx, race_name);
        } else if (owner > 8) {
            adjusted_owner_idx = HACCESS::Ship_Stack_Owner_(adjusted_owner_idx);
            HACCESS::Race_Name_(adjusted_owner_idx, race_name);
        }

        if (race_name[0] == '\0') {
            int16_t total, n_colony, n_outpost, n_transport;
            int32_t hull_counts[6];

            SYS::N_Ships_By_Type_At_Location_(ship_x, ship_y, owner, &total, &n_colony, &n_outpost, &n_transport, hull_counts);

            uint8_t player_color_idx = 0x49; // 'I'
            if (owner < 8) {
                uint8_t player_color = MOX::_player[owner].color;
                player_color_idx = MOX::_main_palette_player_colors[player_color];
            }

            s_colors local_260, local_230;
            MISC::Get_Mox_Font_Colors_(1, player_color_idx, player_color_idx + 5, &local_260);

            for (int i = 1; i < 8; ++i) {
                uint8_t base = local_260.colors[i];
                local_260.colors[i] = base + 2;
                local_230.colors[i] = base + 4;
            }

            fonts::Set_Font_Special_Colors_(&local_230);
            fonts::Set_Font_Style_Outline_(1, &local_260);
            fonts::Set_Font_Spacing_(1);

            HACCESS::Race_Name_(owner, race_name);

            char text_buffer[512];
            const char* fleet_msg = HAROLD::H_Message_(HESTR_131_S_FLEET);
            _sprintf(text_buffer, fleet_msg, race_name);
            strings::String_To_Upper_(text_buffer);

            char temp_fmt[256];
            if (n_outpost > 0) {
                HESTRNGS str_id = (n_outpost == 1) ? HESTR_102_D_OUTPOST : HESTR_103_D_OUTPOSTS;
                _sprintf(temp_fmt, HAROLD::H_Message_(str_id), (int)n_outpost);
                strcat(text_buffer, temp_fmt);
            }

            if (n_transport > 0) {
                HESTRNGS str_id = (n_transport == 1) ? HESTR_104_D_TRANSPORT : HESTR_105_D_TRANSPORTS;
                _sprintf(temp_fmt, HAROLD::H_Message_(str_id), (int)n_transport);
                strcat(text_buffer, temp_fmt);
            }

            if (n_colony > 0) {
                HESTRNGS str_id = (n_colony == 1) ? HESTR_106_D_COLONY_SHIP : HESTR_107_D_COLONY_SHIPS;
                _sprintf(temp_fmt, HAROLD::H_Message_(str_id), (int)n_colony);
                strcat(text_buffer, temp_fmt);
            }

            for (int i = 0; i < 6; ++i) {
                int16_t count = (int16_t)hull_counts[i];
                if (count > 0) {
                    char* h_name = (count == 1) ? TECHDATA::_hull_data[i].name : TECHDATA::_hull_data[i].size_name;
                    _sprintf(temp_fmt, "%d %s, ", (int)count, h_name);
                    strcat(text_buffer, temp_fmt);
                }
            }

            size_t len = strlen(text_buffer);
            if (len > 2) {
                text_buffer[len - 2] = '\0';
            }

            int16_t text_height = FMTPARA::Get_Formatted_Paragraph_Max_Height_();
            if (text_height > 21) {
                fonts::Set_Font_Special_Colors_(&local_230);
                fonts::Set_Font_Style_Outline_(0, &local_260);
            }

            ERIC::Print_Paragraph_Centered_(print_x, print_y, print_width, 22, text_buffer);
        } else {
            strings::String_To_Upper_(race_name);

            s_colors* p_colors = MISC::Font_Colors2_(4, 'L', 'O');
            s_colors local_260;
            memcpy(local_260.colors, p_colors->colors, 8);

            fonts::Set_Font_Style_Outline_(4, &local_260);
            fonts::Print_Centered_(print_x + 102, print_y + 5, race_name);
        }
    }

    void __cdecl Load_Fleet_Galaxy_Stars_() {
        for (int i = 0; i < 11; ++i) {
            MOX::_fleet_galaxy_star_seg[i] = (s_animation_header*)MOX::farload::Far_Reload_Next_(
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
    void __cdecl Load_Officer_Skill_Icons_(void* param_1) {
        for (int i = 0; i < 27; ++i) {
            MOX::_officer_skill_icon_seg[i] = reinterpret_cast<s_animation_header*>(
                MOX::farload::Far_Reload_Next_(
                    reinterpret_cast<const char*>(PTR_s_OFFICER_LBX),
                    0x58 + i,
                    param_1
                )
            );
        }
    }

    void __cdecl Reload_Officer_Screen_Pictures_() {
        bitmap::Get_Bitmap_Size_(100, 100);

        auto& player = MOX::_player[MOX::_PLAYER_NUM];
        int8_t home_star_idx = MOX::_planet[player.home_planet_id].star_index;

        MOX::Mox_Update_();
        MAINSCR::Allocate_First_Screen_Seg_(5);
        FLT1::Allocate_Fltscrn_Next_Screen_Seg_();

        MOX::_officer_pictures_screen_seg = MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, 85000);
        
        _ldrpict = MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x30);
        std::memset(_ldrpict, 0, 0x30);

        _prev_leaders = static_cast<s_prev_leader_data*>(MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xF71));

        for (int16_t i = 0; i < 0x43; ++i) {
            _prev_leaders[i].status = MOX::_leaders[i].status;
            _prev_leaders[i].location = MOX::_leaders[i].location;
            _prev_leaders[i].eta = static_cast<short>(MOX::_leaders[i].eta);

            if (MOX::_leaders[i].type == 1 /* Ship Captain */) {
                if (_prev_leaders[i].status == 0 || _prev_leaders[i].status == 4) {
                    _prev_leaders[i].location = home_star_idx;
                }
            }
        }

        MOX::_officers_original_ship_id = static_cast<int16_t*>(MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x10C));
        MOX::_officers_original_star_id = static_cast<int16_t*>(MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x10C));

        Build_Captain_Id_List_();
        MOX::Mox_Update_();

        const char* lbx = PTR_s_OFFICER_LBX;
        MOX::_officer_main_seg = reinterpret_cast<s_animation_header*>(MOX::farload::Far_Reload_Next_(lbx, 0, MOX::_screen_seg));
        MOX::_officer_fleet_box_seg = reinterpret_cast<s_animation_header*>(MOX::farload::Far_Reload_Next_(lbx, 1, MOX::_screen_seg));
        MOX::_officer_colony_box_seg = reinterpret_cast<s_animation_header*>(MOX::farload::Far_Reload_Next_(lbx, 2, MOX::_screen_seg));
        MOX::_officer_colony_ldrs_button_seg = reinterpret_cast<s_animation_header*>(MOX::farload::Far_Reload_Next_(lbx, 3, MOX::_screen_seg));
        MOX::_officer_ship_ldrs_button_seg = reinterpret_cast<s_animation_header*>(MOX::farload::Far_Reload_Next_(lbx, 4, MOX::_screen_seg));
        MOX::_officer_up_button_seg = MOX::farload::Far_Reload_Next_(lbx, 5, MOX::_screen_seg);
        
        MOX::Mox_Update_();

        MOX::_officer_dn_button_seg = MOX::farload::Far_Reload_Next_(lbx, 6, MOX::_screen_seg);
        MOX::_officer_prev_button_seg = MOX::farload::Far_Reload_Next_(lbx, 7, MOX::_screen_seg);
        MOX::_officer_next_button_seg = MOX::farload::Far_Reload_Next_(lbx, 8, MOX::_screen_seg);
        MOX::_officer_hire_button_seg = MOX::farload::Far_Reload_Next_(lbx, 9, MOX::_screen_seg);
        MOX::_officer_pool_button_seg_inactive = MOX::farload::Far_Reload_Next_(lbx, 10, MOX::_screen_seg);
        MOX::_officer_dismiss_button_seg_active = MOX::farload::Far_Reload_Next_(lbx, 11, MOX::_screen_seg);
        MOX::_officer_return_button_seg = MOX::farload::Far_Reload_Next_(lbx, 12, MOX::_screen_seg);
        MOX::_officer_cancel_hire_button_seg = MOX::farload::Far_Reload_Next_(lbx, 13, MOX::_screen_seg);

        MOX::Mox_Update_();

        MOX::_selected_box_seg = MOX::farload::Far_Reload_Next_(lbx, 14, MOX::_screen_seg);
        MOX::_scanned_box_seg = MOX::farload::Far_Reload_Next_(lbx, 15, MOX::_screen_seg);
        MOX::_fleet_scroll_anim_seg = MOX::farload::Far_Reload_Next_(lbx, 16, MOX::_screen_seg);
        MOX::_dull_officer_hire_button_seg = MOX::farload::Far_Reload_Next_(lbx, 17, MOX::_screen_seg);
        MOX::_dull_officer_pool_button_seg = MOX::farload::Far_Reload_Next_(lbx, 18, MOX::_screen_seg);
        MOX::_officer_pool_button_seg_active = MOX::farload::Far_Reload_Next_(lbx, 19, MOX::_screen_seg);
        MOX::_officer_dismiss_button_seg_inactive = MOX::farload::Far_Reload_Next_(lbx, 20, MOX::_screen_seg);

        MOX::Mox_Update_();

        MOX::_officer_normal_window_seg = MOX::farload::Far_Reload_Next_(lbx, 0x8D, MOX::_screen_seg);
        MOX::_officer_bright_window_seg = MOX::farload::Far_Reload_Next_(lbx, 0x8E, MOX::_screen_seg);

        MOX::Mox_Update_();

        MOX::_temp_officers_pict = MOX::allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x30);

        Get_Officer_Pictures_();
        Load_Officer_Skill_Name_Descriptions_();
        Load_Officer_Skill_Icons_(MOX::_screen_seg);
        Reload_Offscrn_Small_Ship_Icons_();
        Load_Fleet_Galaxy_Stars_();
    }
}