#include "pch.h"

namespace DESIGN {
    /**
     * Draws the currently edited ship design, including derived combat, cost, and space statistics.
     */
    void __cdecl Print_Current_Design_() {
        s_ship_design local_design;
        int16_t bonus_mis_evade, bonus_beam_def, bonus_beam_atk;
        s_colors style3_colors, style2_colors;
        char buffer[128];
        char mod_str[128];
        char arc_str[128];

        Build_Design_Template_(&local_design);
        INITSHIP::Get_Design_Combat_Bonuses_(local_design, &bonus_beam_atk, &bonus_beam_def, &bonus_mis_evade);

        graphics::Reset_Window_();
        video::Clip_Off_();
        fonts::Set_Outline_Color_(0, 0);

        MISC::Get_Mox_Font_Colors_(3, 'U', '[', &style3_colors);
        MISC::Get_Mox_Font_Colors_(2, 'U', '[', &style2_colors);

        fonts::Set_Font_Style_(3, &style3_colors);
        fonts::Set_Font_Spacing_(1);
        fonts::Print_(0xEC, 0x38, TECHDATA::_drives[MOX::_design->ftl_type].name);

        fonts::Set_Font_Style_(2, &style2_colors);
        fonts::Set_Font_Spacing_(1);
        snprintf(buffer, sizeof(buffer), HAROLD::H_Message_(HESTR_17E_D_PARSECS_TURN), TECHDATA::_drives[MOX::_design->ftl_type].warp_speed);
        fonts::Print_(0xFB, 0x47, buffer);

        snprintf(buffer, sizeof(buffer), HAROLD::H_Message_(HESTR_054_D_COMBAT_SPEED), Current_Design_Combat_Speed_(MOX::_design));
        fonts::Print_(0xFB, 0x56, buffer);

        fonts::Set_Font_Style_(3, &style3_colors);
        fonts::Set_Font_Spacing_(1);
        fonts::Print_(0xEC, 0x6B, TECHDATA::_armor[MOX::_design->armor_type].name);

        fonts::Set_Font_Style_(2, &style2_colors);
        fonts::Set_Font_Spacing_(1);
        snprintf(buffer, sizeof(buffer), HAROLD::H_Message_(HESTR_055_D_STRUCTURE_POINTS), (int)Current_Design_Structural_Points_());
        fonts::Print_(0xFB, 0x7A, buffer);

        snprintf(buffer, sizeof(buffer), HAROLD::H_Message_(HESTR_056_D_ARMOR_POINTS), (int)Current_Design_Armor_Absorption_());
        fonts::Print_(0xFB, 0x89, buffer);

        fonts::Set_Font_Style_(3, &style3_colors);
        fonts::Set_Font_Spacing_(1);
        if (_design_shield_replacement_field == MOX::_scanned_field) {
            Set_Flashing_Colors_(3, 1);
        }

        if (MOX::_design->shield == SHIELD_NO_SHIELD) {
            fonts::Print_(0x1B5, 0x38, HAROLD::H_Message_(HESTR_036_NO_SHIELD));
            fonts::Set_Font_Style_(2, &style2_colors);
            fonts::Set_Font_Spacing_(1);
            fonts::Print_(0x1C5, 0x47, HAROLD::H_Message_(HESTR_057_NO_STRENGTH));
            fonts::Print_(0x1C5, 0x55, HAROLD::H_Message_(HESTR_058_NO_DAMAGE_POINTS_BLOCK));
        }
        else {
            fonts::Print_(0x1B5, 0x38, TECHDATA::_shields[MOX::_design->shield].name);
            fonts::Get_String_Width_(TECHDATA::_shields[MOX::_design->shield].name);
            fonts::Set_Font_Style_(2, &style2_colors);
            fonts::Set_Font_Spacing_(1);
            snprintf(buffer, sizeof(buffer), HAROLD::H_Message_(HESTR_059_D_STRENGTH), (int)Shield_Strength_());
            fonts::Print_(0x1C5, 0x47, buffer);

            int points = Current_Shield_Damage_Points_Blocked_();
            snprintf(buffer, sizeof(buffer), HAROLD::H_Message_(HESTR_05A_D_DAMAGE_POINTS_BLOCKE), points, points == 1 ? "" : "s");
            fonts::Print_(0x1C5, 0x55, buffer);
        }

        fonts::Set_Font_Style_(3, &style3_colors);
        fonts::Set_Font_Spacing_(1);
        if (_design_computer_replacement_field == MOX::_scanned_field) {
            Set_Flashing_Colors_(3, 1);
        }
        fonts::Print_(0x1B5, 0x61, TECHDATA::_computers[MOX::_design->computer_type].name);

        fonts::Set_Font_Style_(2, &style2_colors);
        fonts::Set_Font_Spacing_(1);
        snprintf(buffer, sizeof(buffer), HAROLD::H_Message_(HESTR_05B_D_BEAM_ATTACK), (int)TECHDATA::_computers[MOX::_design->computer_type].bonus);
        fonts::Print_(0x1C5, 0x70, buffer);

        fonts::Set_Font_Style_(3, &style3_colors);
        fonts::Set_Font_Spacing_(1);
        fonts::Print_(0x1B5, 0x7D, HAROLD::H_Message_(HESTR_05C_BEAM_DEFENSE));
        snprintf(buffer, sizeof(buffer), "%s%d", (bonus_beam_def >= 0 ? "+" : "-"), (int16_t)bonus_beam_def);
        fonts::Print_Right_(0x26A, 0x7D, buffer);

        fonts::Print_(0x1B5, 0x89, HAROLD::H_Message_(HESTR_05D_MISSILE_EVASION));
        snprintf(buffer, sizeof(buffer), "%d%%", (int)bonus_mis_evade);
        fonts::Print_Right_(0x26A, 0x89, buffer);

        fonts::Set_Font_Spacing_(2);
        graphics::Set_Window_(0x10, 0x9B, 0x272, 0x11C);
        video::Clip_On_();
        fonts::Set_Font_Style_(3, &style3_colors);
        fonts::Print_(0x53, 0x9C, HAROLD::H_Message_(HESTR_05E_WEAPON_TYPE));
        fonts::Print_(0xF9, 0x9C, HAROLD::H_Message_(HESTR_05F_DAMAGE));
        fonts::Print_(0x13C, 0x9C, HAROLD::H_Message_(HESTR_060_ARC));
        fonts::Print_(0x19E, 0x9C, HAROLD::H_Message_(HESTR_030_SPACE));
        fonts::Print_(0x167, 0x9C, HAROLD::H_Message_(HESTR_185_COST));
        fonts::Print_(0x1DF, 0x9C, HAROLD::H_Message_(HESTR_061_MODIFICATIONS));

        bool show_replacement_msg = false;
        int y_pos = 0xAA;
        for (int i = 0; i < 8; ++i) {
            int16_t slot = (int16_t)i + (int16_t)MOX::_first_weapon_ctr;
            if (i - (int)MOX::_scanned_field == -1000) {
                if (MOX::_design->weapon_type[slot] <= WEAPON_NO_WEAPONS) {
                    show_replacement_msg = true;
                }
                else {
                    Set_Flashing_Colors_(2, 2);
                }
            }
            else {
                fonts::Set_Font_Style_(2, &style2_colors);
            }

            if (MOX::_design->weapon_type[slot] != WEAPON_NO_WEAPONS) {
                Weapon_Mod_String_(MOX::_design->weapon_specials[i], mod_str);
                Weapon_Arc_String_(MOX::_design->weapon_firing_arc[i], arc_str);

                const char* base_name = (MOX::_design->weapon_count[slot] < 2) ?
                    TECHDATA::_weapons[MOX::_design->weapon_type[slot]].name :
                    TECHDATA::_weapons[MOX::_design->weapon_type[slot]].name_plural;

                if (Weapon_Needs_Racks_(MOX::_design->weapon_type[slot])) {
                    snprintf(buffer, sizeof(buffer), "%s (%d)", base_name, (int)MOX::_design->weapon_ammo[slot]);
                    fonts::Print_(0x55, y_pos, buffer);
                }
                else {
                    fonts::Print_(0x55, y_pos, base_name);
                }

                fonts::Print_Integer_Right_(0x31, y_pos, MOX::_design->weapon_count[slot]);
                Design_Weapon_Damage_String_(slot, buffer, -1, -1);

                if (strcmp(buffer, "special") == 0) {
                    MISC::Change_Font_Style_(1);
                    fonts::Print_Centered_(0x10B, y_pos, buffer);
                    MISC::Change_Font_Style_(2);
                }
                else {
                    fonts::Print_Centered_(0x10B, y_pos, buffer);
                }

                fonts::Print_Centered_(0x149, y_pos, arc_str);
                int per_space = 0;
                if (MOX::_design->weapon_count[slot] != 0) {
                    per_space = MOX::_design->weapon_space[slot] / MOX::_design->weapon_count[slot];
                }
                if (per_space <= 1) {
                    per_space = 1;
                }
                snprintf(buffer, sizeof(buffer), "%d", per_space);
                fonts::Print_Centered_(0x1B4, y_pos, buffer);
                int per_cost = 0;
                if (MOX::_design->weapon_count[slot] != 0) {
                    per_cost = MOX::_design->weapon_cost[slot] / MOX::_design->weapon_count[slot];
                }
                if (per_cost <= 1) {
                    per_cost = 1;
                }
                snprintf(buffer, sizeof(buffer), "%d", per_cost);
                fonts::Print_Centered_(0x179, y_pos, buffer);
                fonts::Print_Centered_(0x211, y_pos, mod_str);
                y_pos += 13;
            }
        }

        if (show_replacement_msg) {
            int remaining = 8 - MOX::_n_weapons_loaded;
            if (remaining > 0) {
                int centered_y = y_pos;
                if (remaining > 1) centered_y += ((int)MOX::_n_weapons_loaded * -13 + 0x7F) / 2 - 14;

                if (Space_For_Weapon_Replacement_()) {
                    fonts::Set_Font_Style_(2, &style2_colors);
                    Set_Flashing_Colors_(2, 2);
                    fonts::Print_Centered_(0x136, centered_y, HAROLD::H_Message_(HESTR_17F_ADD_NEW_WEAPON));
                }
                else {
                    s_colors* colors = MISC::Font_Colors2_(2, 'I', 'M');
                    fonts::Set_Font_Style_(2, colors);
                    Set_Flashing_Colors_(2, 2);
                    fonts::Print_Centered_(0x136, centered_y, HAROLD::H_Message_(_printed_space_avail < 1 ? HESTR_01A_NOT_ENOUGH_SPACE_TO_AD : HESTR_018_NO_NEW_WEAPONS_CAN_BE_));
                }
            }
        }

        video::Clip_Off_();
        graphics::Set_Window_(0x11, 0x131, 0x272, 0x1B0);
        video::Clip_On_();
        fonts::Set_Font_Style_(3, &style3_colors);
        fonts::Print_(0x1E, 0x132, HAROLD::H_Message_(HESTR_062_SPECIAL_SYSTEM));
        fonts::Print_(0x118, 0x132, HAROLD::H_Message_(HESTR_038_DESCRIPTION));

        fonts::Set_Font_Style_(2, &style2_colors);
        y_pos = 0x140;
        show_replacement_msg = false;
        for (int i = 0; i < 8; ++i) {
            int16_t slot = (int16_t)i + (int16_t)MOX::_first_special_system_ctr;
            if (i - (int)MOX::_scanned_field == -2000) {
                if (MOX::_design->special_devices[slot] <= SPECIAL_NO_SPECIAL) {
                    show_replacement_msg = true;
                }
                else {
                    fonts::Set_Font_Style_(2, &style2_colors);
                    Set_Flashing_Colors_(2, 2);
                }
            }
            else {
                fonts::Set_Font_Style_(2, &style2_colors);
            }

            if (MOX::_design->special_devices[slot] != SPECIAL_NO_SPECIAL) {
                int dev_idx = MOX::_design->special_devices[slot];
                const char* dev_name = TECHDATA::_specials[dev_idx].name;
                fonts::Print_(0x23, y_pos, dev_name);
                fonts::Print_(0x10E, y_pos, MOX::_special_desc_string[dev_idx]);
                y_pos += 13;
            }
        }

        if (show_replacement_msg) {
            int remaining = 8 - MOX::_n_special_systems;
            if (remaining > 0) {
                int centered_y = y_pos;
                if (remaining > 1) centered_y += ((int)MOX::_n_special_systems * -13 + 0x7D) / 2 - 14;

                if (Space_For_System_Replacement_()) {
                    fonts::Set_Font_Style_(2, &style2_colors);
                    Set_Flashing_Colors_(2, 2);
                    fonts::Print_Centered_(0x136, centered_y, HAROLD::H_Message_(HESTR_180_ADD_NEW_SPECIAL_SYSTEM));
                }
                else {
                    s_colors* colors = MISC::Font_Colors2_(2, 'I', 'N');
                    fonts::Set_Font_Style_(2, colors);
                    Set_Flashing_Colors_(2, 2);
                    fonts::Print_Centered_(0x136, centered_y, HAROLD::H_Message_(_printed_space_avail < 1 ? HESTR_01B_NOT_ENOUGH_SPACE_TO_AD : HESTR_019_NO_NEW_SPECIAL_SYSTEMS));
                }
            }
        }

        video::Clip_Off_();
        graphics::Reset_Window_();

        void* ship_pic = KEN::Get_Ship_Picture_Seg_(MOX::_PLAYER_NUM, MOX::_design->picture_type);
        int pic_w = animate::Get_Width_((s_animation_header*)ship_pic);
        int pic_h = animate::Get_Height_((s_animation_header*)ship_pic);
        animate::Draw_(((0x36 - pic_w) / 2) + 0x27, ((0x3A - pic_h) / 2) + 0x3A, (s_animation_header*)ship_pic);

        fonts::Set_Font_Style_(3, &style3_colors);
        fonts::Get_Font_Height_();
        fonts::Print_(0x19, 0x81, HAROLD::H_Message_(HESTR_030_SPACE));
        fonts::Print_Integer_Right_(0x6A, 0x81, MOX::_design->hull_space);

        s_colors style4_colors;
        MISC::Get_Mox_Font_Colors_(4, 'U', 'Y', &style4_colors);
        fonts::Set_Font_Style_(4, &style4_colors);
        int f_h = fonts::Get_Font_Height_();
        int bottom_y = (0x18 - f_h) / 2 + 0x1BC;

        fonts::Print_(0x18, bottom_y, HAROLD::H_Message_(HESTR_185_COST));
        fonts::Print_Integer_Right_(0x87, bottom_y, (int)Printed_Design_Cost_());
        fonts::Print_(0xA3, bottom_y, HAROLD::H_Message_(HESTR_063_SPACE_AVAILABLE));
        fonts::Print_Integer_Right_(0x160, bottom_y, _printed_space_avail);
    }

    /**
     * Runs the design-screen event loop for the currently selected ship design slot.
     */
    void __cdecl Design_Screen_() {
        video::Set_Refresh_Full_();
        int16_t design_idx = MOX::_temp_star_handle;
        char log_path[100];
        MOX::_first_weapon_ctr = 0;
        MOX::_multi_colored_line_start = 0;
        MOX::_first_special_system_ctr = 0;
        _replacement_wait_ctr = 0;
        DESBOX::_incr_value = 1;
        _use_augmented_engines = 1;

        fields::Assign_Auto_Function_(Draw_Design_Screen_, 2);
        fields::Clear_Fields_();
        _design_size_field = -1000;
        mouse::Set_Mouse_List_(&MOX::_mouse_list_variable, 1);

        if (MOX::_first_design_screen_call == 0) {
            _changed_design_name = 0;
            KEN::Load_Player_Ship_Palette_(MOX::_PLAYER_NUM);
            graphics::Reset_Window_();
            video::Clip_Off_();
            MOX::_design_size = MOX::_design->ship_size;
            fields::Clear_Fields_();
            _design_size_field = -1000;
            Add_Design_Buttons_();
            MOX::_draw_design_screen_to_back = 1;
            video::Set_Page_Off_();
            _printed_space_avail = MOX::_design->hull_space - MOX::_design->space_used;
            _printed_cost = MOX::_design->total_cost;
            Draw_Design_Screen_();
            RUSS::Mox_Sync_Update_();
        }
        else {
            MOX::_draw_design_screen_to_back = 1;
            Load_Design_Screen_Pictures_();
            Build_Initial_Design_Template_(MOX::_PLAYER_NUM, design_idx);
            MOX::_original_cost = Total_Design_Cost_();
            if (MOX::_refit_ship != 0) {
                MOX::_design->total_cost = 0;
            }
            _printed_space_avail = MOX::_design->hull_space - MOX::_design->space_used;
            _printed_cost = MOX::_design->total_cost;
            MOX::_design_size = MOX::_design->ship_size;
            fields::Clear_Fields_();
            _design_size_field = -1000;
            Add_Design_Buttons_();
            fields::Disable_Draw_All_Fields_();
            video::Set_Refresh_Full_();
            ERIC::Fast_Fade_Out_();
            fonts::Load_Palette_(4, 0, 0xFF);
            palette::Set_Palette_Changes_(0, 0xFF);
            KEN::Load_Player_Ship_Palette_(MOX::_PLAYER_NUM);
            palette::Refresh_Palette_();
            palstore::Darken_Palette_(100);
            video::Set_Page_Off_();
            Draw_Design_Screen_();
            palette::Clear_Palette_Changes_(0, 0xFF);
            RUSS::Mox_Sync_Update_();
            ERIC::Fast_Fade_In_();
            fields::Enable_Draw_All_Fields_();
            MOX::_first_design_screen_call = 0;
            graphics::Reset_Window_();
            video::Clip_Off_();
        }

        bool update_buttons = true;
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        EVANHELP::Set_Design_Screen_Help_List_(MOX::_design->num_weapon_slots, MOX::_design->num_special_slots);

        int16_t screen_exit_flag = 0;
        do {
            timer::Mark_Time_();
            int16_t input_field = fields::Get_Input_();
            MOX::_scanned_field = fields::Scan_Input_();

            if (_changed_design_name == 0) {
                char* default_name = HACCESS::Design_Name_(MOX::_PLAYER_NUM, design_idx);
                char* p1 = MOX::_design->name;
                char* p2 = default_name;
                while (*p1 && *p1 == *p2) { p1++; p2++; }
                if (*p1 != *p2) {
                    _changed_design_name = 1;
                }
            }

            if (MOX::_design_name_field == input_field) {
                update_buttons = false;
            }
            else if (input_field != 0) {
                update_buttons = true;
            }

            if (MOX::_design_cancel_button == input_field) {
                screen_exit_flag = 1;
                MOX::_temp_star_handle = 0;
                MOX::_current_screen = static_cast<SCREEN>(MOX::_return_screen);
            }

            if (MOX::_design_clear_button == input_field) {
                Clear_Design_();
                EVANHELP::Set_Design_Screen_Help_List_(MOX::_design->num_weapon_slots, MOX::_design->num_special_slots);
            }

            if (MOX::_design_build_button == input_field) {
                Update_Player_Design_(MOX::_PLAYER_NUM, design_idx);
                MOX::_current_screen = static_cast<SCREEN>(MOX::_return_screen);
                screen_exit_flag = 1;
                MOX::_temp_star_handle = 1;
            }

            if (MAINSCR::_debug_field == input_field) {
                snprintf(log_path, sizeof(log_path), "cstdes%d.log", (int)MOX::_design->ship_size);
            }

            int16_t needs_refresh = 0;
            Check_Generic_Replacement_Fields_(input_field, &needs_refresh, &screen_exit_flag);
            Check_Weapon_Fields_(input_field, &needs_refresh, &screen_exit_flag);
            Check_Special_System_Fields_(input_field, &needs_refresh, &screen_exit_flag);
            Check_Plus_Minus_Buttons_(input_field);

            if (MOX::_design->ship_size != MOX::_design_size) {
                MOX::_design->ship_size = MOX::_design_size;
                MOX::_design->hull_space = TECHDATA::_hull_data[MOX::_design->ship_size].size;
                needs_refresh = 1;
                if (MOX::_design->ship_size == SHIP_SIZE_DOOM_STAR) {
                    MOX::_design->picture_type = 43;
                }
                else {
                    MOX::_design->picture_type = MOX::_design->ship_size * 8;
                }
            }

            for (int16_t i = 0; i < 8; ++i) {
                int16_t weapon_slot = MOX::_first_weapon_ctr + i;
                if (MOX::_weapons_count_plus_button[i] == input_field) {
                    int32_t new_space = Weapon_Space_(MOX::_PLAYER_NUM, MOX::_design->weapon_type[weapon_slot],
                        MOX::_design->weapon_count[weapon_slot] + 1,
                        (uint8_t)MOX::_design->weapon_firing_arc[weapon_slot],
                        (uint8_t)MOX::_design->weapon_ammo[weapon_slot],
                        MOX::_design->weapon_specials[weapon_slot]);
                    int16_t delta = (int16_t)new_space - (int16_t)MOX::_design->weapon_space[weapon_slot];
                    int16_t candidate = (int16_t)MOX::_design->space_used + delta;
                    if ((int32_t)candidate <= MOX::_design->hull_space) {
                        MOX::_design->weapon_count[weapon_slot]++;
                        if (MOX::_design->weapon_count[weapon_slot] > 99) {
                            MOX::_design->weapon_count[weapon_slot] = 99;
                        }
                        _replacement_wait_ctr = 0;
                        needs_refresh = 1;
                    }
                    break;
                }
                else if (MOX::_weapons_count_minus_button[i] == input_field) {
                    MOX::_design->weapon_count[weapon_slot]--;
                    if (MOX::_design->weapon_count[weapon_slot] < 1) {
                        MOX::_design->weapon_count[weapon_slot] = 1;
                    }
                    _replacement_wait_ctr = 0;
                    needs_refresh = 1;
                    break;
                }
            }

            if (needs_refresh != 0) {
                Update_Calculated_Design_Data_(MOX::_PLAYER_NUM);
                needs_refresh = 0;
            }

            if (update_buttons) {
                fields::Clear_Fields_();
                _design_size_field = -1000;
                Add_Design_Buttons_();
            }

            for (int16_t i = 0; i <= SHIP_SIZE_DOOM_STAR; ++i) {
                if (MOX::_design_ship_size_field[i] == input_field) {
                    if (_add_size_button_flag[i] < 241) {
                        _changed_design_name = 0;
                        int16_t pic = MOX::_design->picture_type;
                        int16_t sign = pic >> 15;
                        int16_t pic_val = ((((pic ^ sign) - sign) & 7) ^ sign) - sign;
                        int16_t race = (uint8_t)MOX::_player[MOX::_PLAYER_NUM].race;
                        Get_Ship_Design_Name_(race, MOX::_design->ship_size, pic_val, MOX::_design->name);
                    }
                    else {
                        Print_Ship_Size_Error_Message_(i);
                    }
                }
            }

            if (screen_exit_flag == 0) {
                video::Set_Page_Off_();
                _printed_space_avail = MOX::_design->hull_space - MOX::_design->space_used;
                _printed_cost = MOX::_design->total_cost;
                Draw_Design_Screen_();
                RUSS::Mox_Sync_Update_();
            }
            else if (screen_exit_flag == 99) {
                video::Set_Page_Off_();
                Draw_Design_Screen_();
                RUSS::Mox_Sync_Update_();
            }

            strings::Trim_(MOX::_design->name);
            if (MOX::_design->name[0] == '\0') {
                char* default_name = HACCESS::Design_Name_(MOX::_PLAYER_NUM, design_idx);
                char* dst = MOX::_design->name;
                while ((*dst++ = *default_name++));
            }

            timer::Release_Time_(2);

            if (screen_exit_flag != 0) {
                fields::Enable_Mouse_Wait_();
                fields::Clear_Fields_();
                MOX::_reloaded_ship_palette = 0;
                if (MOX::_current_screen != SCREEN_DESIGN) {
                    JIM::Play_Background_Music_();
                }
                return;
            }
        } while (true);
    }

    /**
     * Loads the design-screen artwork and related tech-description text resources.
     */
    void __cdecl Load_Design_Screen_Pictures_() {
        RUSS::Mox_Update_();
        MOX::_design_screen_seg = (s_animation_header*)farload::Far_Reload_("DESIGN.LBX", 0, MOX::_screen_seg);
        MOX::_design_icon_lf_arrow_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", 1, MOX::_screen_seg);
        MOX::_design_icon_rt_arrow_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", 2, MOX::_screen_seg);
        MOX::_clear_design_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", 3, MOX::_screen_seg);
        MOX::_cancel_design_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", 4, MOX::_screen_seg);
        MOX::_build_design_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", 5, MOX::_screen_seg);
        RUSS::Mox_Update_();

        for (int i = 0; i <= SHIP_SIZE_DOOM_STAR; ++i) {
            MOX::_design_size_button_seg[i] = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", 6 + i, MOX::_screen_seg);
        }
        for (int i = 0; i <= SHIP_SIZE_DOOM_STAR; ++i) {
            MOX::_darkened_size_button_seg[i] = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", 6 + i, MOX::_screen_seg);
        }

        int idx = 12;
        _dark_build_design_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        RUSS::Mox_Update_();

        MOX::_plus_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_minus_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_d_scroll_up_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_d_scroll_dn_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        _dull_plus_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        _dull_minus_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_dull_d_scroll_up_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_dull_d_scroll_dn_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);

        MOX::_d_scroll_bar_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);

        MOX::_beam_filter_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_missile_filter_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_bomb_filter_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_special_filter_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_weapons_cancel_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_weapons_accept_button_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        RUSS::Mox_Update_();

        MOX::_weapons_top_box_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_weapons_mid_box_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_weapons_bot_box_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_weapons_selected_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        RUSS::Mox_Update_();

        MOX::_weapons_rack_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);

        for (int i = 0; i < 5; ++i) {
            MOX::_weapons_arc_pict_seg[i] = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        }
        for (int i = 0; i < 5; ++i) {
            MOX::_weapons_arc_text_seg[i] = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        }
        RUSS::Mox_Update_();
        for (int i = 0; i < 5; ++i) {
            MOX::_weapons_rack_text_seg[i] = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        }

        MOX::_weapons_blank_mods_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        for (int i = 0; i < 18; ++i) {
            MOX::_weapons_special_seg[i] = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        }
        RUSS::Mox_Update_();

        MOX::_system_repl_scroll_bar_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_system_repl_box_top_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_system_repl_box_mid_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_system_repl_box_btm_seg = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);
        MOX::_system_repl_box_cancel_btn = (s_animation_header*)farload::Far_Reload_Next_("DESIGN.LBX", idx++, MOX::_screen_seg);

        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);
        MOX::_design = (s_current_design*)allocate::Allocate_First_Block_(MOX::_global_data_seg, sizeof(*MOX::_design));
        MOX::_weapon_filter = (s_weapon_filter_item*)allocate::Allocate_Next_Block_(MOX::_global_data_seg, sizeof(*MOX::_weapon_filter) * 40);

        char tech_lbx[20];
        Get_Tech_Description_Lbx_Name_(tech_lbx);

        for (int i = 0; i < 39; ++i) {
            MOX::_special_desc_string[i] = (char*)farload::Far_Reload_Next_Data_(tech_lbx, 1, MOX::_global_data_seg, i, 1, 100);
            RUSS::Mox_Update_();
        }
        for (int i = 0; i < 40; ++i) {
            MOX::_weapon_notes_string[i] = (char*)farload::Far_Reload_Next_Data_(tech_lbx, 3, MOX::_global_data_seg, i, 1, 100);
            RUSS::Mox_Update_();
        }
    }

    /**
     * Rebuilds the clickable controls for the current design-screen state.
     */
    void __cdecl Add_Design_Buttons_() {
        for (int16_t i = 0; i <= SHIP_SIZE_DOOM_STAR; ++i) {
            _add_size_button_flag[i] = Add_Size_Button_();
        }

        int16_t field_x1, field_y1, field_x2, field_y2;
        for (int16_t i = 0; i <= SHIP_SIZE_DOOM_STAR; ++i) {
            Get_Ship_Size_Button_Field_XY_(i, &field_x1, &field_y1, &field_x2, &field_y2);
            if (_add_size_button_flag[i] == 1) {
                MOX::_design_ship_size_field[i] = fields::Add_Multi_Button_Field_(field_x1, field_y1, "", MOX::_design_size_button_seg[i], &MOX::_design_size, i, "", 0x29 /* ) */);
            }
            else {
                MOX::_design_ship_size_field[i] = fields::Add_Hidden_Field_(field_x1, field_y1, field_x2, field_y2, "", 0x29 /* ) */);
            }
        }

        bool replacement_exists_3 = Replacement_Exists_(3);
        int16_t weapon_y = 0xA9;
        for (int16_t i = 0; i < 8; ++i) {
            int16_t weapon_idx = i + MOX::_first_weapon_ctr;
            MOX::_weapons_clear_button[i] = -1000;

            if (((i < 1) || (MOX::_design->weapon_type[weapon_idx] <= WEAPON_NO_WEAPONS)) && !replacement_exists_3) {
                MOX::_weapons_count_minus_button[i] = -1000;
                MOX::_weapons_count_plus_button[i] = -1000;
                MOX::_weapons_field[i] = -1000;
            }
            else {
                if (MOX::_design->weapon_type[weapon_idx] <= WEAPON_NO_WEAPONS || MOX::_design->weapon_count[weapon_idx] < 2) {
                    MOX::_weapons_count_minus_button[i] = -1000;
                    if (MOX::_design->weapon_type[weapon_idx] > WEAPON_NO_WEAPONS) {
                        fields::Add_Hidden_Field_(0x37, weapon_y, 0x44, weapon_y + 12, "", 0);
                    }
                }
                else {
                    MOX::_weapons_count_minus_button[i] = fields::Add_Button_Field_(0x37, weapon_y, "", MOX::_minus_button_seg, "", 0x28 /* ( */);
                }

                int32_t next_weapon_space = Weapon_Space_(MOX::_PLAYER_NUM, MOX::_design->weapon_type[weapon_idx],
                    MOX::_design->weapon_count[weapon_idx] + 1,
                    (uint8_t)MOX::_design->weapon_firing_arc[weapon_idx],
                    (uint8_t)MOX::_design->weapon_ammo[weapon_idx],
                    MOX::_design->weapon_specials[weapon_idx]);

                if (MOX::_design->weapon_type[weapon_idx] <= WEAPON_NO_WEAPONS ||
                    ((MOX::_design->hull_space - MOX::_design->space_used) - (int32_t)next_weapon_space + (int32_t)MOX::_design->weapon_space[weapon_idx] < 0)) {
                    MOX::_weapons_count_plus_button[i] = -1000;
                    if (MOX::_design->weapon_type[weapon_idx] > WEAPON_NO_WEAPONS) {
                        fields::Add_Hidden_Field_(0x13, weapon_y, 0x20, weapon_y + 12, "", 0x29 /* ) */);
                    }
                }
                else {
                    MOX::_weapons_count_plus_button[i] = fields::Add_Button_Field_(0x13, weapon_y, "", MOX::_plus_button_seg, "", 0x28 /* ( */);
                }

                int16_t h_x, h_w;
                if (MOX::_design->weapon_type[weapon_idx] <= WEAPON_NO_WEAPONS) {
                    h_x = 0x10;
                    h_w = 0x26F;
                }
                else {
                    h_x = 0x4D;
                    h_w = 0x22C;
                }
                MOX::_weapons_field[i] = fields::Add_Hidden_Field_(h_x, weapon_y, h_w, weapon_y + 13, "", 0x29 /* ) */);
            }
            weapon_y += 13;
        }

        bool replacement_exists_9 = Replacement_Exists_(9);
        int16_t special_y = 0x13F;
        for (int16_t i = 0; i < 8; ++i) {
            MOX::_special_system_clear_button[i] = -1000;
            if (replacement_exists_9) {
                MOX::_special_system_field[i] = fields::Add_Hidden_Field_(0x11, special_y, 0x26F, special_y + 13, "", 0x29 /* ) */);
            }
            else {
                MOX::_special_system_field[i] = -1000;
            }
            special_y += 13;
        }

        MOX::_ship_icon_lf_button = fields::Add_Button_Field_(0x11, 0x52, "", MOX::_design_icon_lf_arrow_seg, "", 0x28 /* ( */);
        MOX::_ship_icon_rt_button = fields::Add_Button_Field_(0x5E, 0x52, "", MOX::_design_icon_rt_arrow_seg, "", 0x28 /* ( */);

        s_colors colors;
        MISC::Get_Mox_Font_Colors_(4, 'U', 'Y', &colors);
        fonts::Set_Font_Style_(4, &colors);

        int16_t input_h = fonts::Get_Font_Height_() + 2;
        int16_t text_h = fonts::Get_Font_Height_();
        int16_t name_y = ((0x1B - text_h) / 2) + 0x11;
        MOX::_design_name_field = fields::Add_Continuous_String_Input_Field_(
            0x12, name_y, 0x86, input_h,
            MOX::_design->name, 15, 0, 0, 0x29 /* ) */, 0
        );

        _design_shield_replacement_field = fields::Add_Hidden_Field_(0x1B5, 0x38, 0x273, 0x5F, "", 0x29 /* ) */);
        _design_computer_replacement_field = fields::Add_Hidden_Field_(0x1B5, 0x61, 0x273, 0x7B, "", 0x29 /* ) */);

        MOX::_design_cancel_button = fields::Add_Button_Field_(0x1CD, 0x1BB, "", MOX::_cancel_design_button_seg, "\x1B", 0x28 /* ( */);
        MOX::_design_clear_button = fields::Add_Button_Field_(0x176, 0x1BB, "", MOX::_clear_design_button_seg, "L", 0x28 /* ( */);

        if (MOX::_design->hull_space < MOX::_design->space_used) {
            MOX::_design_build_button = -1000;
        }
        else {
            MOX::_design_build_button = fields::Add_Button_Field_(0x223, 0x1BB, "", MOX::_build_design_button_seg, "B", 0x28 /* ( */);
        }

        MAINSCR::_debug_field = fields::Add_Hidden_Field_(0, 0x1D6, 10, 0x1DF, "", 0x29 /* ) */);
        fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, "", 0);
    }

}