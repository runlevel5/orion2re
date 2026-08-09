#include "pch.h"

namespace DESBOX {

    uint16_t _weapon_replacement_mods;

    int8_t _incr_value;

    int16_t _field_item_scanned;
    int16_t _field_item_chosen;
    int16_t _replacement_box_middle_height;
    int16_t _replacement_scroll_field;
    int16_t _replacement_box_width;
    int16_t _replacement_box_base_y;
    int16_t _replacement_box_base_x;
    int16_t _replacement_box_height;
    uint8_t _beam_filter_button_status = 1;
    uint8_t _missile_filter_button_status = 1;
    uint8_t _bomb_filter_button_status = 1;
    uint8_t _special_filter_button_status = 1;
    int16_t _replacement_scroll_dn_button;
    int16_t _replacement_scroll_up_button;
    int16_t _weapon_arc_rack_box_field;
    int16_t _weapon_replacement_arcs;
    uint16_t _weapon_replacement_rack;
    uint8_t _wait_ctr;
    uint8_t _valid_mods[16];
    uint8_t _valid_arcs_racks[5];

    const char s__00551100[] = "";
    const char s_space[] = "\x1b";      // fmtpara color-attribute control byte; not a printable space.
    const char s__00551120[] = "\x1b";  // fmtpara color-attribute control byte reused as a non-printing field label.

    int16_t __cdecl Special_System_Choices_() {
        memset(MOX::_design->replacement_item_unlocked, 0, sizeof(MOX::_design->replacement_item_unlocked));

        const int16_t replacement_slot = MOX::_design_screen_replacement_slot;

        for (int i = 0; i < SPECIAL_COUNT; ++i) {
            MOX::_design_choice_items[i] = -1;
            MOX::_design_choice_item_selected[i] = 0;
        }

        MOX::_design_choice_items[0] = 0;
        MOX::_scroll_bar->total_rows = 1;

        for (int i = 0; i < 39; ++i) {
            if (MOX::_scroll_bar->total_rows >= SPECIAL_COUNT) {
                break;
            }

            uint16_t tech_app_id = TECHDATA::_specials[i].tech_app_id;
            if (MOX::_player[MOX::_PLAYER_NUM].tech_applications[tech_app_id] == TECH_RESEARCH_STATUS_RESEARCHED) {
                MOX::_design_choice_items[MOX::_scroll_bar->total_rows] = (short)i;
                MOX::_scroll_bar->total_rows++;
            }
        }

        for (short slot_idx = 0; slot_idx < 8; ++slot_idx) {
            short device_id = MOX::_design->special_devices[slot_idx];
            if (device_id > 0) {
                for (short choice_idx = 0; choice_idx < MOX::_scroll_bar->total_rows; ++choice_idx) {
                    if (device_id == MOX::_design_choice_items[choice_idx] &&
                        MOX::_design_screen_replacement_slot != -1 &&
                        MOX::_design->special_devices[MOX::_design_screen_replacement_slot] != device_id) {
                        MOX::_design_choice_item_selected[choice_idx] = 1;
                    }
                }
            }
        }

        short valid_choice_count = 0;
        if (MOX::_scroll_bar->total_rows > 1) {
            for (short i = 1; i < MOX::_scroll_bar->total_rows; ++i) {
                int choice_special_idx = (int)MOX::_design_choice_items[i];
                uint16_t tech_app_id = TECHDATA::_specials[choice_special_idx].tech_app_id;
                
                int category = DESIGN::Miniaturization_Category_(tech_app_id);
                int16_t tech_level = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, tech_app_id);
                
                uint16_t base_space = TECHDATA::_specials[choice_special_idx].space_per_ship_type[MOX::_design->ship_size];
                int space_required = DESIGN::Space_Given_M_Level_(base_space, tech_level, category);

                if (replacement_slot > -1 &&
                    MOX::_design->special_devices[replacement_slot] > 0) {
                    space_required -= MOX::_design->special_device_space[replacement_slot];
                }

                if ((MOX::_design->space_used + space_required <= MOX::_design->hull_space) &&
                    (MOX::_design_choice_item_selected[i] == 0)) {
                    valid_choice_count++;
                }
            }
        }

        return valid_choice_count;
    }

    void __cdecl Special_Systems_Box_() {
        char msg42[120];
        char msg41[200];
        char msg42_base[200];
        char msg43_base[200];
        char desc[1332];

        int16_t btn1;
        int16_t btn2;
        int16_t redraw_warning;
        int16_t input_field;
        int16_t done;
        int16_t exclude_special;
        int16_t scanned_field;
        int16_t item_count;
        int16_t exclusion_result;
        int8_t redraw_bg;

        redraw_bg = 1;
        allocate::Push_Block_(MOX::_global_data_seg);
        MOX::_scroll_bar = (s_scroll_bar*)allocate::Allocate_Next_Block_(MOX::_global_data_seg, sizeof(*MOX::_scroll_bar));
        memset(MOX::_scroll_bar, 0, sizeof(*MOX::_scroll_bar));
        
        _field_item_chosen = -1;
        MOX::_scroll_bar->first_visible_row = 0;
        _incr_value = 1;
        MOX::_multi_colored_line_start = 0;
        MOX::_scroll_bar->thumb_center_y = 140;
        MOX::_scroll_bar->visible_rows = 10;
        MOX::_scroll_bar->track_len_px = 140;
        MOX::_scroll_bar->type = 2;
        MOX::_scroll_bar->row_step = 1;

        Draw_Darkened_Design_Screen_();
        redraw_warning = 1;
        Special_System_Choices_();
        
        MOX::_scroll_bar->thumb_height_px = SCROLL::Get_Vertical_Scroll_Bar_Height_(MOX::_scroll_bar);
        Set_Up_Special_System_Scroll_Bar_();
        Special_System_Replacement_Box_Dimensions_(610, MOX::_scroll_bar->total_rows);
        
        fields::Assign_Auto_Function_(Draw_Special_Systems_Replacement_Box, 3);
        fields::Clear_Fields_();
        Add_Special_System_Fields_(&btn1, &btn2);
        
        _wait_ctr = 0;
        done = 0;

        while (done == 0) {
            input_field = fields::Get_Input_();
            scanned_field = fields::Scan_Input_();
            _field_item_scanned = -1;

            int16_t num_visible = MOX::_scroll_bar->total_rows;
            if (num_visible > 10) {
                num_visible = 10;
            }

            if (num_visible > 0) {
                for (int16_t i = 0; i < num_visible; i++) {
                    if (done != 0) {
                        break;
                    }

                    int16_t item_idx = MOX::_scroll_bar->first_visible_row + i;
                    int16_t field = MOX::_repl_fields[i];

                    if (scanned_field == field) {
                        _field_item_scanned = i;
                    }

                    if (input_field == field) {
                        if (item_idx == 0) {
                            if (MOX::_design->special_devices[MOX::_design_screen_replacement_slot] > 0) {
                                DESIGN::Clear_Special_System_Slot_(MOX::_design_screen_replacement_slot);
                            }
                            done = 1;
                        } else {
                            exclusion_result = Special_System_Exclusion_(MOX::_design_choice_items[item_idx], &exclude_special);
                            item_count = Items_Selected_();

                            if (MOX::_design_choice_items[item_idx] != 4) {
                                int16_t m_cat = DESIGN::Miniaturization_Category_(TECHDATA::_specials[MOX::_design_choice_items[item_idx]].tech_app_id);
                                int16_t t_level = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, TECHDATA::_specials[MOX::_design_choice_items[item_idx]].tech_app_id);
                                DESIGN::Space_Given_M_Level_(TECHDATA::_specials[MOX::_design_choice_items[item_idx]].space_per_ship_type[MOX::_design->ship_size], t_level, (int8_t)m_cat);
                            }

                            int8_t can_add = (item_idx == 0) || (item_count < 8 && exclusion_result == 0);
                            int16_t current_slot_item = MOX::_design->special_devices[MOX::_design_screen_replacement_slot];

                            if (MOX::_design_choice_item_selected[item_idx] == 0 ||
                                MOX::_design_choice_items[item_idx] == current_slot_item) {
                                
                                if (can_add) {
                                    done = 1;
                                    _field_item_chosen = item_idx;
                                    MOX::_design->special_devices[MOX::_design_screen_replacement_slot] = MOX::_design_choice_items[item_idx];
                                } else if (exclusion_result != 0) {
                                    if (strcmp(TECHDATA::_specials[MOX::_design_choice_items[item_idx]].name, TECHDATA::_specials[exclude_special].name) == 0) {
                                        MISC::Get_LBX_Message_(0x42, msg42_base);
                                        snprintf(msg42, sizeof(msg42), msg42_base, TECHDATA::_specials[MOX::_design_choice_items[item_idx]].name);
                                    } else {
                                        MISC::Get_LBX_Message_(0x43, msg43_base);
                                        char* name_item = TECHDATA::_specials[MOX::_design_choice_items[item_idx]].name;
                                        char* name_excl = TECHDATA::_specials[exclude_special].name;
                                        const char* article_item = MISC::Is_Vowel_(name_item[0]) ? "an" : "a";
                                        const char* article_excl = MISC::Is_Vowel_(name_excl[0]) ? "an" : "a";
                                        snprintf(msg42, sizeof(msg42), msg43_base, article_excl, name_excl, article_item, name_item);
                                    }
                                    GENDRAW::Warning_Box_(msg42);
                                    redraw_warning = 1;
                                }
                            } else {
                                MISC::Get_LBX_Message_(0x41, msg41);
                                snprintf(msg42, sizeof(msg42), msg41, TECHDATA::_specials[MOX::_design_choice_items[item_idx]].name);
                                GENDRAW::Warning_Box_(msg42);
                                redraw_warning = 1;
                            }
                        }
                    } else if (input_field == -field) {
                        int16_t item_id = MOX::_design_choice_items[item_idx];
                        if (item_id <= 0) {
                            TEXTBOX::Draw_Help_Entry_(691);
                        } else {
                            EVANHELP::Get_Application_Description_(desc, sizeof(desc), TECHDATA::_specials[item_id].tech_app_id);
                            TEXTBOX::Text_Box_(TECHDATA::_specials[item_id].name, desc);
                        }
                    }

                    num_visible = MOX::_scroll_bar->total_rows;
                    if (num_visible > 10) {
                        num_visible = 10;
                    }
                }
            }

            if (MOX::_special_scroll_up_button[0] == input_field) {
                SCROLL::Scroll_Up_One_Item_(MOX::_scroll_bar);
                _wait_ctr = 0;
            }
            if (MOX::_special_scroll_dn_button == input_field) {
                SCROLL::Scroll_Down_One_Item_(MOX::_scroll_bar);
                _wait_ctr = 0;
            }

            if (btn1 == input_field || btn2 == input_field) {
                done = 1;
            } else if (input_field == -btn1) {
                TEXTBOX::Draw_Help_Entry_(689);
            } else if (input_field == -btn2) {
                TEXTBOX::Draw_Help_Entry_(687);
            }

            fields::Clear_Fields_();
            Add_Special_System_Fields_(&btn1, &btn2);

            if (done == 0) {
                if (redraw_warning == 0 && redraw_bg != 0) {
                    Redraw_Main_Design_Screen_To_Background_();
                    redraw_bg = 0;
                }
                
                if (done == 0) {
                    video::Set_Page_Off_();
                    Draw_Special_Systems_Replacement_Box();
                    
                    if (redraw_warning != 0) {
                        redraw_warning = 0;
                    } else {
                        RUSS::Mox_Sync_Update_();
                    }
                }
            }
        }

        MOX::_design->special_system_scroll_first_row = static_cast<int8_t>(MOX::_scroll_bar->first_visible_row);
        MOX::_design->special_system_scroll_thumb_y = MOX::_scroll_bar->thumb_center_y;
        MOX::_scroll_bar = nullptr;

        allocate::Pop_Block_(MOX::_global_data_seg);
        MOX::_current_screen = SCREEN_DESIGN;
        
        DESIGN::Update_Calculated_Design_Data_(MOX::_PLAYER_NUM);
        fields::Clear_Fields_();
    }
    void __cdecl Add_Replacement_Weapon_Fields_(int16_t* cancel_btn_field, int16_t* screen_field) {
        int16_t mod_fields[WEAPON_MOD_COUNT] = {0};
        int16_t mod_xs[WEAPON_MOD_COUNT] = {0};
        int16_t mod_ys[WEAPON_MOD_COUNT] = {0};
        char choice_str[80];

        int16_t mid_box_h = animate::Get_Height_(MOX::_weapons_mid_box_seg);
        if (MOX::_scroll_bar->total_rows < 7) {
            mid_box_h = 0x66;
        }

        int16_t top_box_h = animate::Get_Height_(MOX::_weapons_top_box_seg);
        int16_t bot_box_h = animate::Get_Height_(MOX::_weapons_bot_box_seg);

        *cancel_btn_field = -1000;
        *screen_field = -1000;
        _replacement_scroll_field = -1000;

        int16_t repl_y = _replacement_box_base_y + top_box_h;
        Clear_Weapon_Replacement_Fields_();

        int16_t visible_count = MOX::_scroll_bar->total_rows;
        if (visible_count > 10) {
            visible_count = 10;
        }

        if (visible_count > 0) {
            int16_t field_y = repl_y + 0x13;
            for (int16_t i = 0; i < visible_count; i++) {
                (void)Weapon_Index_(MOX::_scroll_bar->first_visible_row + i);
                MOX::_repl_fields[i] = fields::Add_Hidden_Field_(
                    _replacement_box_base_x + 0x1c,
                    field_y,
                    _replacement_box_base_x + 0x256,
                    field_y + 0xc,
                    s__00551100,
                    0x29
                );

                int16_t current_visible = MOX::_scroll_bar->total_rows;
                if (current_visible > 10) {
                    current_visible = 10;
                }
                visible_count = current_visible;
                field_y += 0xe;
            }
        }

        _replacement_scroll_field = -1000;

        if (MOX::_scroll_bar->total_rows <= 10) {
            _replacement_scroll_up_button = -1000;
        } else {
            _replacement_scroll_up_button = fields::Add_Button_Field_(
                _replacement_box_width + _replacement_box_base_x - 0x20,
                _replacement_box_base_y + top_box_h + 1,
                s__00551100,
                MOX::_d_scroll_up_button_seg,
                "-",
                0x28
            );
        }

        if (MOX::_scroll_bar->total_rows <= 10) {
            _replacement_scroll_field = -1000;
        } else {
            _replacement_scroll_field = fields::Add_Scroll_Field_(
                _replacement_box_base_x + 0x25d,
                _replacement_box_base_y + top_box_h + 0x14,
                0, 0x78, 0, 0x78, 9, 0x78,
                &MOX::_scroll_bar->thumb_center_y,
                s__00551100,
                0
            );
        }

        int16_t total_top_mid_h = top_box_h + mid_box_h;
        int16_t total_top_mid_h_minus_9 = total_top_mid_h - 9;

        if (MOX::_scroll_bar->total_rows <= 10) {
            _replacement_scroll_dn_button = -1000;
        } else {
            _replacement_scroll_dn_button = fields::Add_Button_Field_(
                _replacement_box_width + _replacement_box_base_x - 0x20,
                _replacement_box_base_y + total_top_mid_h - 0xe,
                s__00551100,
                MOX::_d_scroll_dn_button_seg,
                "+",
                0x28
            );
        }

        int16_t filters_y1 = _replacement_box_base_y + total_top_mid_h + 0xa;
        int16_t filters_y2 = _replacement_box_base_y + total_top_mid_h + 0x22;

        MOX::_beam_filter_button = fields::Add_Hidden_Field_(
            _replacement_box_base_x + 0x66,
            filters_y1,
            _replacement_box_base_x + 0xc5,
            filters_y2,
            "B",
            0x29
        );

        MOX::_missile_filter_button = fields::Add_Hidden_Field_(
            _replacement_box_base_x + 0xcc,
            filters_y1,
            _replacement_box_base_x + 0x13f,
            filters_y2,
            "M",
            0x29
        );

        MOX::_bomb_filter_button = fields::Add_Hidden_Field_(
            _replacement_box_base_x + 0x146,
            filters_y1,
            _replacement_box_base_x + 0x1a6,
            filters_y2,
            "O",
            0x29
        );

        MOX::_special_filter_button = fields::Add_Hidden_Field_(
            _replacement_box_base_x + 0x1ad,
            filters_y1,
            _replacement_box_base_x + 0x20f,
            filters_y2,
            "S",
            0x29
        );

        if (_field_item_chosen > -1) {
            int16_t weapon_idx = Weapon_Index_(_field_item_chosen);
            int16_t arc_y_offset = total_top_mid_h;
            int16_t rack_y_offset = total_top_mid_h;

            for (int16_t i = 0; i < 5; i++) {
                int16_t type = DESIGN::Weapon_Type_(weapon_idx);
                int16_t needs_racks = DESIGN::Weapon_Needs_Racks_(weapon_idx);

                if (needs_racks) {
                    DESIGN::Weapon_Space_(MOX::_PLAYER_NUM, weapon_idx, 1, _weapon_replacement_arcs, DESIGN::Missile_Rack_Quantity_(i), _weapon_replacement_mods);
                    int16_t rx = _replacement_box_base_x + 0x77;
                    int16_t ry = _replacement_box_base_y + rack_y_offset + 0x54;
                    MOX::_weapon_rack_field[i] = fields::Add_Hidden_Field_(
                        rx,
                        ry,
                        rx + 0x55,
                        ry + 0xf,
                        s__00551100,
                        0x29
                    );
                    _valid_arcs_racks[i] = 1;
                } else {
                    if (_field_item_chosen > -1 && type == 0) {
                        int16_t rx = _replacement_box_base_x + 0x77;
                        int16_t ry = _replacement_box_base_y + arc_y_offset + 0x54;
                        New_Arc_Space_((uint8_t)i);
                        MOX::_weapon_arc_field[i] = fields::Add_Hidden_Field_(
                            rx,
                            ry,
                            rx + 0x5c,
                            ry + 0xd,
                            s__00551100,
                            0x29
                        );
                        _valid_arcs_racks[i] = 1;
                    }
                    _weapon_replacement_rack = 0;
                }
                arc_y_offset += 0x10;
                rack_y_offset += 0x0f;
            }
        }

        int16_t rack_box_x = _replacement_box_base_x + 0x1d;
        int16_t rack_box_y = _replacement_box_base_y + total_top_mid_h + 0x46;
        _weapon_arc_rack_box_field = fields::Add_Hidden_Field_(
            rack_box_x,
            rack_box_y,
            rack_box_x + animate::Get_Width_(MOX::_weapons_rack_seg),
            rack_box_y + animate::Get_Height_(MOX::_weapons_rack_seg),
            s__00551100,
            0x29
        );

        Valid_Modifications_(_valid_mods);
        Get_Weapon_Mod_Field_XYs_(mod_fields, mod_xs, mod_ys);

        int16_t mods_base_y = _replacement_box_base_y + total_top_mid_h;

        for (int16_t i = 1; i < WEAPON_MOD_COUNT; i++) {
            if (_field_item_chosen > -1) {
                if (Mod_Allowed_On_Weapon_Type_((uint8_t)i)) {
                    if (Should_Add_Mod_(i)) {
                        int16_t mod_y = mods_base_y + mod_ys[i];
                        int16_t mod_x = _replacement_box_base_x + mod_xs[i];
                        MOX::_weapon_mod_fields[i] = fields::Add_Hidden_Field_(
                            mod_x,
                            mod_y,
                            mod_x + 0x8c,
                            mod_y + 0xd,
                            s__00551100,
                            0x29
                        );
                    }
                }
            }
        }

        *cancel_btn_field = fields::Add_Button_Field_(
            _replacement_box_base_x + 0x219,
            _replacement_box_base_y + total_top_mid_h_minus_9 + 0x5b,
            s__00551100,
            MOX::_weapons_cancel_button_seg,
            s_space,
            0x28
        );

        if (!Weapon_Choice_Valid_(choice_str, sizeof(choice_str))) {
            int16_t accept_y = _replacement_box_base_y + total_top_mid_h_minus_9 + 0x8b;
            int16_t accept_x = _replacement_box_base_x + 0x21a;
            int16_t accept_h = animate::Get_Height_(MOX::_weapons_accept_button_seg);
            int16_t accept_w = animate::Get_Width_(MOX::_weapons_accept_button_seg);
            MOX::_replacement_box_accept_button = fields::Add_Hidden_Field_(
                accept_x,
                accept_y,
                accept_x + accept_w,
                accept_y + accept_h,
                s__00551100,
                0x29
            );
        } else {
            MOX::_replacement_box_accept_button = fields::Add_Button_Field_(
                _replacement_box_base_x + 0x21a,
                _replacement_box_base_y + total_top_mid_h_minus_9 + 0x8b,
                s__00551100,
                MOX::_weapons_accept_button_seg,
                "A",
                0x28
            );
        }

        fields::Add_Hidden_Field_(
            _replacement_box_base_x,
            _replacement_box_base_y,
            _replacement_box_base_x + animate::Get_Width_(MOX::_weapons_top_box_seg),
            _replacement_box_base_y + top_box_h + mid_box_h + bot_box_h,
            s__00551100,
            0
        );

        *screen_field = fields::Add_Hidden_Field_(0, 0, 0x27f, 0x1df, s_space, 0x29);

        MOX::_scroll_bar->up_btn = _replacement_scroll_up_button;
        MOX::_scroll_bar->dn_btn = _replacement_scroll_dn_button;
    }

    int16_t __cdecl Scan_Replacement_Fields_(int16_t field_id, int16_t target_field, int16_t* out_field_index, int16_t* out_visible_row) {
        int16_t result = -1;
        *out_field_index = -1;
        *out_visible_row = -1;

        int16_t row_limit;

        row_limit = MOX::_scroll_bar->total_rows;
        if (row_limit > 10) {
            row_limit = 10;
        }

        for (int16_t i = 0; i < row_limit; ++i) {
            if (MOX::_repl_fields[i] == target_field) {
                result = 0;
                *out_visible_row = MOX::_scroll_bar->first_visible_row + i;
            } else if (MOX::_repl_fields[i] + target_field == 0) {
                result = 1;
                *out_visible_row = MOX::_scroll_bar->first_visible_row + i;
            }

            row_limit = MOX::_scroll_bar->total_rows;
            if (row_limit > 10) {
                row_limit = 10;
            }
        }

        if (result == -1) {
            row_limit = MOX::_scroll_bar->total_rows;
            if (row_limit > 10) {
                row_limit = 10;
            }

            for (int16_t i = 0; i < row_limit; ++i) {
                if (MOX::_repl_fields[i] == field_id) {
                    result = 4;
                    *out_field_index = i;
                }

                row_limit = MOX::_scroll_bar->total_rows;
                if (row_limit > 10) {
                    row_limit = 10;
                }
            }
        }

        return result;
    }

    void __cdecl Update_Weapon_Replacement_(int16_t* exit_flag, int16_t weapon_choice, int16_t slot_idx) {
        char msg_buffer[80];

        if (weapon_choice == 0) {
            if (MOX::_design->weapon_type[slot_idx] > 0) {
                DESIGN::Clear_Weapon_Slot_(slot_idx);
                DESIGN::Update_Calculated_Design_Data_(MOX::_PLAYER_NUM);
            }
            MOX::_current_screen = SCREEN_DESIGN;
            *exit_flag = 1;
            return;
        }

        if (DESBOX::Weapon_Choice_Valid_(msg_buffer, sizeof(msg_buffer)) == 0) {
            GENDRAW::Message_Box_(msg_buffer);
            return;
        }

        if (weapon_choice > -1) {
            // We preserve the low word of the previous space to scale weapon count later
            int16_t old_space_low = (int16_t)MOX::_design->weapon_space[slot_idx];
            bool weapon_exists = MOX::_design->weapon_type[slot_idx] > 0;

            int16_t new_weapon_idx = DESBOX::Weapon_Index_(weapon_choice);
            MOX::_design->weapon_type[slot_idx] = new_weapon_idx;
            MOX::_design->weapon_count[slot_idx] = 1;

            DESBOX::Set_New_Weapon_Modifications_(&MOX::_design->weapon_specials[slot_idx]);
            MOX::_design->weapon_firing_arc[slot_idx] = (int8_t)DESBOX::_weapon_replacement_arcs;

            // Side-effect calls required by the engine state
            DESIGN::Weapon_Type_(DESBOX::Weapon_Index_(weapon_choice));

            if (DESIGN::Weapon_Needs_Racks_(MOX::_design->weapon_type[slot_idx])) {
                MOX::_design->weapon_ammo[slot_idx] = (int8_t)DESIGN::Missile_Rack_Quantity_(DESBOX::_weapon_replacement_rack);
            } else {
                MOX::_design->weapon_ammo[slot_idx] = 0;
            }

            MOX::_design->weapon_space[slot_idx] = DESIGN::Weapon_Space_(
                MOX::_PLAYER_NUM,
                MOX::_design->weapon_type[slot_idx],
                MOX::_design->weapon_count[slot_idx],
                (uint16_t)MOX::_design->weapon_firing_arc[slot_idx],
                (uint16_t)MOX::_design->weapon_ammo[slot_idx],
                (uint16_t)MOX::_design->weapon_specials[slot_idx]
            );

            MOX::_design->weapon_cost[slot_idx] = DESIGN::Weapon_Cost_(
                MOX::_design->weapon_type[slot_idx],
                MOX::_design->weapon_count[slot_idx],
                (int16_t)MOX::_design->weapon_firing_arc[slot_idx],
                (int16_t)MOX::_design->weapon_ammo[slot_idx],
                MOX::_design->weapon_specials[slot_idx]
            );

            if (weapon_exists) {
                int16_t new_space_low = (int16_t)MOX::_design->weapon_space[slot_idx];
                if (new_space_low > 0) {
                    int16_t new_count = old_space_low / new_space_low;
                    if (new_count < 1) {
                        new_count = 1;
                    }
                    if (new_count > 99) {
                        new_count = 99;
                    }
                    MOX::_design->weapon_count[slot_idx] = new_count;
                }
            }

            DESIGN::Update_Calculated_Design_Data_(MOX::_PLAYER_NUM);
            MOX::_current_screen = SCREEN_DESIGN;
            *exit_flag = 1;
        }
    }

    void __cdecl Print_Weapon_Modification_Message_(int16_t mod_idx) {
        char message[200];
        char format[200];

        message[0] = '\0';

        uint8_t mod_code = _valid_mods[mod_idx];

        switch (mod_code) {
            case 0xF9: {
                Weapon_Index_(_field_item_chosen);
                int16_t space = static_cast<int16_t>(New_Mod_Space_(static_cast<uint8_t>(mod_idx)));
                MISC::Get_LBX_Message_(0x3A, format);
                snprintf(message, sizeof(message), format, 
                        TECHDATA::_weapon_modifications[mod_idx].name, 
                        (MOX::_design->space_used - MOX::_design->hull_space) + space);
                break;
            }
            case 0xFA: {
                int16_t wpn_idx = Weapon_Index_(_field_item_chosen);
                uint16_t app_id = TECHDATA::_weapons[wpn_idx].tech_app_id;
                int16_t min_level = DESIGN::Miniaturization_Level_(MOX::_PLAYER_NUM, app_id);
                int16_t level_diff = static_cast<int8_t>(TECHDATA::_weapon_modifications[mod_idx].level) - min_level;

                int16_t msg_idx;
                if (level_diff == 1) {
                    msg_idx = 0x3B;
                } else {
                    msg_idx = 0x3C;
                }
                MISC::Get_LBX_Message_(msg_idx, format);
                snprintf(message, sizeof(message), format, level_diff, 
                        TECHDATA::_technology_applications[app_id].name, 
                        TECHDATA::_weapon_modifications[mod_idx].name);
                break;
            }
            case 0xFB: {
                strlcpy(message, HAROLD::H_Message_(0x3D), sizeof(message));
                break;
            }
            case 0xFC: {
                strlcpy(message, HAROLD::H_Message_(0x3E), sizeof(message));
                break;
            }
            case 0xFD: {
                strlcpy(message, HAROLD::H_Message_(0x3F), sizeof(message));
                break;
            }
            case 0xFE: {
                int16_t wpn_idx = Weapon_Index_(_field_item_chosen);
                MISC::Get_LBX_Message_(0x40, format);
                snprintf(message, sizeof(message), format, 
                        TECHDATA::_weapon_modifications[mod_idx].name, 
                        TECHDATA::_weapons[wpn_idx].name);
                break;
            }
        }

        if (message[0] != '\0') {
            GENDRAW::Message_Box_(message);
        }
    }

    int16_t __cdecl Update_Field_Item_Chosen_(int16_t weapon_idx) {
        int16_t result = -1;
        int16_t total_rows = MOX::_scroll_bar->total_rows;

        if (total_rows > 1 && weapon_idx > -1) {
            bool found = false;
            for (int16_t i = 0; i < total_rows; ++i) {
                if (found) {
                    break;
                }

                if (MOX::_weapon_filter[i].weapon_idx == weapon_idx) {
                    result = i;
                    found = true;
                }
            }
        }

        return result;
    }

    void __cdecl Generic_Replacement_Box_Choices_() {
        MOX::_scroll_bar->total_rows = 1;

        memset(MOX::_design_choice_items, 0, sizeof(MOX::_design_choice_items));

        memset(MOX::_design->replacement_item_unlocked, 0, sizeof(MOX::_design->replacement_item_unlocked));

        if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_SHIELD) {
            MOX::_scroll_bar->total_rows = SHIELD_COUNT;
            for (int16_t idx = 0; idx < MOX::_scroll_bar->total_rows; ++idx) {
                // If it is the first item (basic shield) or it has been researched (tech status 3)
                if (idx == 0 || MOX::_player[MOX::_PLAYER_NUM].tech_applications[TECHDATA::_shields[idx].tech_app_id] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    MOX::_design_choice_items[idx] = idx;
                    MOX::_design->replacement_item_unlocked[idx] = 1;
                } else {
                    MOX::_design_choice_items[idx] = -1;
                }
            }
        } else if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_COMPUTER) {
            MOX::_scroll_bar->total_rows = COMPUTER_COUNT;
            for (int16_t idx = 0; idx < MOX::_scroll_bar->total_rows; ++idx) {
                // If it is the first item (basic computer) or it has been researched (tech status 3)
                if (idx == 0 || MOX::_player[MOX::_PLAYER_NUM].tech_applications[TECHDATA::_computers[idx].tech_app_id] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    MOX::_design_choice_items[idx] = idx;
                    MOX::_design->replacement_item_unlocked[idx] = 1;
                } else {
                    MOX::_design_choice_items[idx] = -1;
                }
            }
        }
    }

    void __cdecl Generic_Replacement_Box_Dimensions_(void) {
        int16_t cancel_btn_height = animate::Get_Height_(MOX::_system_repl_box_cancel_btn);

        s_animation_header* top_seg_ptr = nullptr;
        if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_SHIELD ||
            MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_COMPUTER) {
            top_seg_ptr = MOX::_system_repl_box_top_seg;
        }

        int16_t top_seg_height = animate::Get_Height_(top_seg_ptr);

        int16_t rows = MOX::_scroll_bar->total_rows;
        if (rows > 10) {
            rows = 10;
        }

        DESBOX::_replacement_box_middle_height = (rows * 16) + cancel_btn_height;

        int16_t btm_seg_height = animate::Get_Height_(MOX::_system_repl_box_btm_seg);
        DESBOX::_replacement_box_width = animate::Get_Width_(top_seg_ptr);

        DESBOX::_replacement_box_height = DESBOX::_replacement_box_middle_height + top_seg_height + btm_seg_height;

        DESBOX::_replacement_box_base_x = (640 - DESBOX::_replacement_box_width) / 2;
        DESBOX::_replacement_box_base_y = (440 - DESBOX::_replacement_box_height) / 2;
    }

    void __cdecl Add_Generic_Replacement_Fields_(int16_t* cancel_btn_field, int16_t* bg_field, int16_t* top_box_field) {
        s_animation_header* top_seg = nullptr;
        if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_SHIELD ||
            MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_COMPUTER) {
            top_seg = MOX::_system_repl_box_top_seg;
        }

        int16_t current_row_y_offset = animate::Get_Height_(top_seg);

        for (int16_t i = 0; i < MOX::_scroll_bar->total_rows; ++i) {
            if (MOX::_design_choice_items[i] == -1) {
                MOX::_repl_fields[i] = -1000 /* 0xfc18 */;
            } else {
                int16_t y1 = DESBOX::_replacement_box_base_y + current_row_y_offset + 18;
                MOX::_repl_fields[i] = fields::Add_Hidden_Field_(
                    DESBOX::_replacement_box_base_x + 10,
                    y1,
                    DESBOX::_replacement_box_base_x + 600,
                    y1 + 13,
                    s__00551100,
                    41 /* 0x29 */
                );
                current_row_y_offset += 14;
            }
        }

        int16_t cancel_btn_x = DESBOX::_replacement_box_width - animate::Get_Width_(MOX::_system_repl_box_cancel_btn);
        cancel_btn_x = cancel_btn_x + DESBOX::_replacement_box_base_x - 35;

        int16_t cancel_btn_y = DESBOX::_replacement_box_height - animate::Get_Height_(MOX::_system_repl_box_cancel_btn);
        cancel_btn_y = cancel_btn_y + DESBOX::_replacement_box_base_y - 16;

        *cancel_btn_field = fields::Add_Button_Field_(
            cancel_btn_x,
            cancel_btn_y,
            s__00551100,
            MOX::_system_repl_box_cancel_btn,
            s__00551120,
            40 /* 0x28 */
        );

        int16_t top_box_x2 = DESBOX::_replacement_box_base_x + animate::Get_Width_(MOX::_weapons_top_box_seg);
        int16_t top_box_y2 = DESBOX::_replacement_box_base_y + DESBOX::_replacement_box_height;

        *top_box_field = fields::Add_Hidden_Field_(
            DESBOX::_replacement_box_base_x,
            DESBOX::_replacement_box_base_y,
            top_box_x2,
            top_box_y2,
            s__00551100,
            0
        );

        *bg_field = fields::Add_Hidden_Field_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, s__00551120, 41 /* 0x29 */);
    }

    void __cdecl Draw_Generic_Replacement_Box_Help_(int16_t input_field, int16_t clear_btn_field, int16_t ok_btn_field, int16_t box_field) {
        int16_t help_id = -1;

        if (input_field + clear_btn_field == 0) {
            if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_SHIELD) {
                help_id = 0x2ab;
            } else if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_COMPUTER) {
                help_id = 0x2ae;
            }
        } else if (input_field + ok_btn_field == 0) {
            if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_SHIELD) {
                help_id = 0x2aa;
            } else if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_COMPUTER) {
                help_id = 0x2ad;
            }
        } else if (input_field + box_field == 0) {
            if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_SHIELD) {
                help_id = 0x2a9;
            } else if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_COMPUTER) {
                help_id = 0x2ac;
            }
        }

        if (help_id != -1) {
            TEXTBOX::Draw_Help_Entry_(help_id);
            return;
        }

        bool found = false;
        int16_t choice_item = -1;

        for (int16_t i = 0; i < MOX::_scroll_bar->total_rows; ++i) {
            if ((MOX::_repl_fields[i] + input_field == 0) && (MOX::_design_choice_items[i] != -1)) {
                choice_item = MOX::_design_choice_items[i];
                found = true;
                break;
            }
        }

        if (!found) {
            return;
        }

        if (choice_item == 0) {
            int16_t entry_id = -1;
            if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_SHIELD) {
                entry_id = 0x2b4;
            } else if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_COMPUTER) {
                entry_id = 0x2b5;
            }
            if (entry_id != -1) {
                TEXTBOX::Draw_Help_Entry_(entry_id);
            }
            return;
        }

        if (choice_item > 0) {
            char title[100];
            char description[1316];
            description[0] = '\0';
            const char* name_ptr = nullptr;

            if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_SHIELD) {
                EVANHELP::Get_Application_Description_(description, sizeof(description), TECHDATA::_shields[choice_item].tech_app_id);
                name_ptr = TECHDATA::_shields[choice_item].name;
            } else if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_COMPUTER) {
                EVANHELP::Get_Application_Description_(description, sizeof(description), TECHDATA::_computers[choice_item].tech_app_id);
                name_ptr = TECHDATA::_computers[choice_item].name;
            }

            strlcpy(title, name_ptr, sizeof(title));

            if (description[0] != '\0') {
                TEXTBOX::Text_Box_(title, description);
            }
        }
    }

    void __cdecl Redraw_Main_Design_Screen_To_Background_() {
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639, 479, 0);
        DESIGN::Draw_Design_Screen_();
        graphics::Reset_Window_();
        video::Clip_Off_();
        remap::Update_Glass_Remap_Colors_();
        palette::Set_Palette_Changes_(0, 255);
        MISC::_Darken_Fill_(0, 0, 639, 479);
        video::Copy_Off_To_Back_();
    }

    void __cdecl Get_Weapons_Box_Base_Dimensions_() {
        int16_t mid_height = animate::Get_Height_(MOX::_weapons_mid_box_seg);

        if (MOX::_scroll_bar->total_rows < 7) {
            mid_height = 0x66;
        }

        int16_t top_height = animate::Get_Height_(MOX::_weapons_top_box_seg);
        int16_t bot_height = animate::Get_Height_(MOX::_weapons_bot_box_seg);

        _replacement_box_width = 0x27C;
        _replacement_box_base_x = 2;
        _replacement_box_height = bot_height + top_height + mid_height;

        int32_t base_y = (0x195 - _replacement_box_height) / 2 + 0x26;

        if (base_y <= 0x26) {
            base_y = 0x26;
        }

        _replacement_box_base_y = static_cast<int16_t>(base_y);
    }

    int32_t __cdecl New_Mod_Space_(uint8_t mod_bit_index) {
        if (DESBOX::_field_item_chosen <= -1) {
            return 0;
        }

        int16_t weapon_idx = DESBOX::Weapon_Index_(DESBOX::_field_item_chosen);
        uint16_t mods_mask = 0;

        for (int16_t i = 0; i < WEAPON_MOD_COUNT; ++i) {
            if (MOX::_weapon_mod_field_status[i] == 1) {
                mods_mask |= (1 << i);
            }
        }

        // Add the additional modification specified by the bit index
        mods_mask |= (1 << mod_bit_index);

        return DESIGN::Weapon_Space_(
            MOX::_PLAYER_NUM,
            weapon_idx,
            1,
            DESBOX::_weapon_replacement_arcs,
            DESBOX::_weapon_replacement_rack,
            mods_mask
        );
    }

    void __cdecl Draw_Weapons_Replacement_Box_() {
        int16_t mid_box_height = animate::Get_Height_(MOX::_weapons_mid_box_seg);

        int16_t total_rows = MOX::_scroll_bar->total_rows;
        if (total_rows < 7) {
            mid_box_height = 0x66;
        }

        int16_t top_box_height = animate::Get_Height_(MOX::_weapons_top_box_seg);

        // This height value is unused, but the function is purposefully called for potential side effects.
        animate::Get_Height_(MOX::_weapons_bot_box_seg);

        animate::Draw_(_replacement_box_base_x, _replacement_box_base_y, MOX::_weapons_top_box_seg);

        int16_t current_y = _replacement_box_base_y + top_box_height;

        graphics::Set_Window_(_replacement_box_base_x, current_y, 0x27F, current_y + mid_box_height);
        video::Clip_On_();

        animate::Draw_(_replacement_box_base_x, current_y, MOX::_weapons_mid_box_seg);

        graphics::Set_Window_(_replacement_box_base_x, current_y, 0x256, current_y + mid_box_height);
        video::Clip_On_();

        Print_Main_Weapon_Box_(_replacement_box_base_x + 0x16, current_y);

        graphics::Reset_Window_();
        video::Clip_Off_();

        if (_replacement_scroll_field != -1000) {
            SCROLL::Draw_Generic_Vertical_Scroll_Bar_(MOX::_scroll_bar, Draw_Weapons_Scroll_Bar_);
        }

        int16_t bottom_y = current_y + mid_box_height;

        animate::Draw_(_replacement_box_base_x, bottom_y, MOX::_weapons_bot_box_seg);

        if (_field_item_chosen > -1) {
            int16_t weapon_index = Weapon_Index_(_field_item_chosen);
            if (DESIGN::Weapon_Needs_Racks_(weapon_index) != 0) {
                Draw_Weapons_Rack_Box_(_replacement_box_base_x, bottom_y);
            } else {
                Draw_Weapons_Arc_Box_(_replacement_box_base_x, bottom_y);
            }
        } else {
            Draw_Weapons_Arc_Box_(_replacement_box_base_x, bottom_y);
        }

        Draw_Weapons_Mod_Box_(_replacement_box_base_x, bottom_y);

        int16_t filter_button_y = bottom_y + 10;

        animate::Set_Animation_Frame_(MOX::_beam_filter_button_seg, _beam_filter_button_status);
        animate::Draw_(_replacement_box_base_x + 0x66, filter_button_y, MOX::_beam_filter_button_seg);

        animate::Set_Animation_Frame_(MOX::_missile_filter_button_seg, _missile_filter_button_status);
        animate::Draw_(_replacement_box_base_x + 0xCC, filter_button_y, MOX::_missile_filter_button_seg);

        animate::Set_Animation_Frame_(MOX::_bomb_filter_button_seg, _bomb_filter_button_status);
        animate::Draw_(_replacement_box_base_x + 0x146, filter_button_y, MOX::_bomb_filter_button_seg);

        animate::Set_Animation_Frame_(MOX::_special_filter_button_seg, _special_filter_button_status);
        animate::Draw_(_replacement_box_base_x + 0x1AD, filter_button_y, MOX::_special_filter_button_seg);

        JIM::Play_Streaming_Music_(8, -1, 1);
    }

    void __cdecl Draw_Weapons_Scroll_Bar_() {
        int16_t top_height = animate::Get_Height_(MOX::_weapons_top_box_seg);
        animate::Get_Height_(MOX::_weapons_mid_box_seg);

        int16_t win_x1 = DESBOX::_replacement_box_base_x + 599 /* 0x257 */;
        int16_t base_y = DESBOX::_replacement_box_base_y + top_height;
        int16_t win_y1 = base_y + 19 /* 0x13 */;

        int16_t base_y_plus_20 = base_y + 20 /* 0x14 */;
        int16_t line_x = DESBOX::_replacement_box_base_x + 605 /* 0x25D */;
        int16_t color = 0x5B /* 91 */;

        fields::Auto_Input_();
        mouse::Mouse_Button_();

        int16_t window_status;
        MISC::Save_Window_Status_(&window_status);

        int16_t bar_height = animate::Get_Height_(MOX::_d_scroll_bar_seg);
        int16_t win_y2 = win_y1 + bar_height - 2;
        int16_t bar_width = animate::Get_Width_(MOX::_d_scroll_bar_seg);
        int16_t win_x2 = win_x1 + bar_width;

        graphics::Set_Window_(win_x1, win_y1, win_x2, win_y2);
        video::Clip_On_();

        int16_t thumb_half_height = (MOX::_scroll_bar->thumb_height_px + (MOX::_scroll_bar->thumb_height_px < 0 ? 1 : 0)) >> 1;
        int16_t offset_y = (MOX::_scroll_bar->track_len_px - thumb_half_height) - MOX::_scroll_bar->thumb_center_y;

        int16_t line_y1 = base_y_plus_20 + offset_y + 1;

        for (int16_t i = 0; i < 9; ++i) {
            int16_t line_y2 = MOX::_scroll_bar->thumb_height_px + offset_y + base_y_plus_20;
            line::Line_(line_x, line_y1, line_x, line_y2, (uint8_t)color);

            if (i % 2 == 0) {
                color--;
            }
            line_x++;
        }

        animate::Draw_(win_x1, win_y1, MOX::_d_scroll_bar_seg);
        MISC::Restore_Window_Status_(window_status);
    }

    void __cdecl Draw_Weapons_Rack_Box_(int16_t x, int16_t y) {
        animate::Draw_(x + 29, y + 70, MOX::_weapons_rack_seg);
        animate::Draw_(x + 33, y + 73, MOX::_weapons_arc_pict_seg[4]);

        int16_t current_y = y + 88;
        for (int16_t i = 0; i < 5; ++i) {
            if (DESBOX::_weapon_replacement_rack == i) {
                animate::Set_Animation_Frame_(MOX::_weapons_rack_text_seg[i], 2);
            } else {
                if (DESBOX::_valid_arcs_racks[i] != 0) {
                    animate::Set_Animation_Frame_(MOX::_weapons_rack_text_seg[i], 1);
                } else {
                    animate::Reset_Animation_Frame_(MOX::_weapons_rack_text_seg[i]);
                }
            }

            animate::Draw_(x + 127, current_y, MOX::_weapons_rack_text_seg[i]);
            current_y += 15;
        }
    }

    void __cdecl Draw_Weapons_Arc_Box_(int16_t x, int16_t y) {
        uint8_t arc_status[5] = { 0, 0, 0, 0, 0 };

        if (_weapon_replacement_arcs & WEAPON_FIRING_ARC_FORWARD_EXTENDED) {
            arc_status[1] = 1;
        } else if (_weapon_replacement_arcs & WEAPON_FIRING_ARC_BACK_EXTENDED) {
            arc_status[2] = 1;
        } else if (_weapon_replacement_arcs & WEAPON_FIRING_ARC_REAR) {
            arc_status[3] = 1;
        } else if (_weapon_replacement_arcs & WEAPON_FIRING_ARC_360) {
            arc_status[4] = 1;
        } else {
            arc_status[0] = 1;
        }

        animate::Draw_(x + 29, y + 70, MOX::_weapons_rack_seg);

        int16_t current_y = y + 84;
        for (int i = 0; i < 5; ++i) {
            animate::Set_Animation_Frame_(MOX::_weapons_arc_text_seg[i], (int16_t)arc_status[i]);
            animate::Draw_(x + 119, current_y, MOX::_weapons_arc_text_seg[i]);

            if (arc_status[i] == 1) {
                animate::Draw_(x + 33, y + 73, MOX::_weapons_arc_pict_seg[i]);
            }
            current_y += 16;
        }
    }

    int32_t __cdecl New_Arc_Space_(uint8_t arc_index) {
        if (DESBOX::_field_item_chosen > -1) {
            int16_t weapon_idx = DESBOX::Weapon_Index_(DESBOX::_field_item_chosen);
            uint16_t arc_mask = (uint16_t)(WEAPON_FIRING_ARC_FORWARD << arc_index);

            return DESIGN::Weapon_Space_(
                MOX::_PLAYER_NUM,
                weapon_idx,
                1,
                arc_mask,
                DESBOX::_weapon_replacement_rack,
                DESBOX::_weapon_replacement_mods
            );
        }

        return 0;
    }

    int16_t __cdecl Valid_Modifications_(uint8_t* mod_status_list) {
        int16_t valid_count = 0;

        memset(mod_status_list, 0, WEAPON_MOD_COUNT * sizeof(*mod_status_list));

        if (_field_item_chosen > -1) {
            int16_t weapon_idx = Weapon_Index_(_field_item_chosen);
            uint16_t tech_app_id = TECHDATA::_weapons[weapon_idx].tech_app_id;

            for (int16_t mod_idx = 1; mod_idx < WEAPON_MOD_COUNT; mod_idx++) {
                if (mod_idx == WEAPON_MOD_EMISSIONS_GUIDANCE) {
                    if (TECHDATA::_weapons[weapon_idx].type == WEAPON_TYPE_MISSILE &&
                        MOX::_player[MOX::_PLAYER_NUM].tech_applications[TECH_APP_EMISSIONS_GUIDANCE_SYSTEM] == TECH_RESEARCH_STATUS_RESEARCHED) {
                        New_Mod_Space_(static_cast<uint8_t>(mod_idx));
                        valid_count++;
                        mod_status_list[mod_idx] = 1;
                    } else {
                        mod_status_list[mod_idx] = -3;
                    }
                } else {
                    if (((TECHDATA::_weapons[weapon_idx].available_mods >> mod_idx) & 1) == 0) {
                        mod_status_list[mod_idx] = -2;
                    } else {
                        int16_t miniat_level = DESIGN::Miniaturization_Level_(MOX::_PLAYER_NUM, tech_app_id);
                        if (static_cast<int8_t>(TECHDATA::_weapon_modifications[mod_idx].level) > miniat_level) {
                            mod_status_list[mod_idx] = -6;
                        } else {
                            New_Mod_Space_(static_cast<uint8_t>(mod_idx));
                            valid_count++;
                            mod_status_list[mod_idx] = 1;
                        }
                    }
                }
            }
        }
        return valid_count;
    }

    void __cdecl Generic_Replacement_Box_() {
        uint8_t redraw_flag = 1;
        bool first_time = true;
        bool done = false;

        MOX::_multi_colored_line_start = 0;
        _incr_value = 1;

        allocate::Push_Block_(MOX::_global_data_seg);
        MOX::_scroll_bar = (s_scroll_bar*)allocate::Allocate_Next_Block_(MOX::_global_data_seg, sizeof(*MOX::_scroll_bar));
        memset(MOX::_scroll_bar, 0, sizeof(*MOX::_scroll_bar));

        Draw_Darkened_Design_Screen_();
        fields::Clear_Fields_();
        fields::Deactivate_Help_List_();
        fields::Assign_Auto_Function_(Draw_Generic_Replacement_Box_, 1);
        Generic_Replacement_Box_Choices_();
        Generic_Replacement_Box_Dimensions_();

        int16_t clear_btn_field;
        int16_t ok_btn_field;
        int16_t box_field;
        Add_Generic_Replacement_Fields_(&clear_btn_field, &ok_btn_field, &box_field);

        int16_t chosen_index;
        if (MOX::_design_screen_replacement_slot == -1) {
            chosen_index = 0;
            MOX::_design_screen_replacement_item = 0;
            _field_item_chosen = 0;
        } else {
            chosen_index = MOX::_design_screen_replacement_item;
            _field_item_chosen = MOX::_design_screen_replacement_item;
        }

        while (true) {
            int16_t input = fields::Get_Input_();
            _field_item_scanned = (uint16_t)fields::Scan_Input_();

            if (input < 0) {
                Draw_Generic_Replacement_Box_Help_(input, clear_btn_field, ok_btn_field, box_field);
            }

            if (input == clear_btn_field) {
                chosen_index = -1;
                done = true;
            } else if (input == ok_btn_field) {
                done = true;
            }

            for (int16_t i = 0; i < MOX::_scroll_bar->total_rows; i++) {
                if (done) {
                    break;
                }
                if (MOX::_repl_fields[i] == input) {
                    chosen_index = i;
                    done = true;
                }
            }

            if ((!done && !first_time && redraw_flag != 0) || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                Redraw_Main_Design_Screen_To_Background_();
                redraw_flag = 0;
                MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            }

            if (done) {
                break;
            }

            video::Set_Page_Off_();
            Draw_Generic_Replacement_Box_();

            if (first_time) {
                first_time = false;
            } else {
                RUSS::Mox_Sync_Update_();
            }
        }

        if (chosen_index >= 0) {
            if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_SHIELD) {
                MOX::_design->shield = MOX::_design_choice_items[chosen_index];
            } else if (MOX::_design_screen_replacement_type == TECH_APPLICATION_TYPE_COMPUTER) {
                MOX::_design->computer_type = MOX::_design_choice_items[chosen_index];
            }
        }

        allocate::Pop_Block_(MOX::_global_data_seg);
    }

    void __cdecl Weapons_Replacement_Box_() {
        char front_arc_text[30];
        char rear_arc_text[30];
        char beam_arc_text[30];
        char left_arc_text[30];
        char full_arc_text[4];
        char message_box_text[100];
        char invalid_arc_format[200];
        char invalid_rack_format[200];
        char incompatible_weapon_format[200];
        char tech_application_description[1332];

        strlcpy(front_arc_text, HAROLD::H_Message_(0x46), sizeof(front_arc_text));
        strlcpy(rear_arc_text, HAROLD::H_Message_(0x47), sizeof(rear_arc_text));
        strlcpy(beam_arc_text, HAROLD::H_Message_(0x48), sizeof(beam_arc_text));
        strlcpy(left_arc_text, HAROLD::H_Message_(0x49), sizeof(left_arc_text));
        strlcpy(full_arc_text, "360", sizeof(full_arc_text));

        char* arc_strings[5] = { front_arc_text, rear_arc_text, beam_arc_text, left_arc_text, full_arc_text };

        allocate::Push_Block_(MOX::_global_data_seg);
        MOX::_scroll_bar = (s_scroll_bar*)allocate::Allocate_Next_Block_(MOX::_global_data_seg, sizeof(*MOX::_scroll_bar));
        memset(MOX::_scroll_bar, 0, sizeof(*MOX::_scroll_bar));

        MOX::_scroll_bar->first_visible_row = 0;
        MOX::_scroll_bar->type = 3;

        _incr_value = 1;
        _wait_ctr = 0;
        _field_item_chosen = -1;

        int16_t replacement_slot = MOX::_design_screen_replacement_slot;

        _weapon_replacement_arcs = WEAPON_FIRING_ARC_NONE;
        MOX::_multi_colored_line_start = 0;

        video::Set_Page_Off_();
        DESIGN::Draw_Design_Screen_();
        RUSS::Mox_Sync_Update_();
        video::Copy_Off_To_Back_();

        memset(MOX::_design_choice_items, 0, sizeof(MOX::_design_choice_items));

        int16_t done_flag = 0;

        Weapon_Choices_(MOX::_PLAYER_NUM);

        if (MOX::_scroll_bar->total_rows > 1) {
            qsort(MOX::_weapon_filter, MOX::_scroll_bar->total_rows, sizeof(MOX::_weapon_filter[0]), qsort_by_research_cost_);
        }

        Space_With_Weapons_();

        MOX::_scroll_bar->visible_rows = 10;
        MOX::_scroll_bar->track_len_px = 0x78;
        MOX::_scroll_bar->thumb_height_px = SCROLL::Get_Vertical_Scroll_Bar_Height_(MOX::_scroll_bar);
        MOX::_scroll_bar->thumb_center_y = MOX::_scroll_bar->track_len_px - (MOX::_scroll_bar->thumb_height_px / 2);
        MOX::_scroll_bar->last_thumb_center_y = -1;

        int16_t first_time_flag = 1;
        MOX::_scroll_bar->row_step = 1;
        MOX::_multi_colored_box_color_start = 0;

        graphics::Reset_Window_();
        video::Clip_Off_();
        remap::Update_Glass_Remap_Colors_();
        palette::Set_Palette_Changes_(0, 0xFF);
        video::Set_Page_Off_();
        MISC::_Darken_Fill_(0, 0, 0x27F, 0x1DF);
        video::Copy_Off_To_Back_();
        fields::Clear_Fields_();

        int16_t cancel_btn_field;
        int16_t background_field;
        Add_Replacement_Weapon_Fields_(&cancel_btn_field, &background_field);
        fields::Assign_Auto_Function_(Draw_Weapons_Replacement_Box_, 1);
        Get_Weapons_Box_Base_Dimensions_();
        Set_Up_Weapon_Display_(replacement_slot);
        Weapon_Replacement_Help_(_field_item_chosen);

        int16_t redraw_flag = 1;

        while (true) {
            if (done_flag != 0) break;

            int16_t input = fields::Get_Input_();
            int16_t scan_input_val = fields::Scan_Input_();

            if (input == background_field || input == cancel_btn_field) {
                done_flag = 1;
            } else if (input + cancel_btn_field == 0) {
                TEXTBOX::Draw_Help_Entry_(0x2B0);
            }

            if (_field_item_chosen > -1) {
                int16_t weapon_idx = Weapon_Index_(_field_item_chosen);
                int16_t weap_type = DESIGN::Weapon_Type_(weapon_idx);

                for (int16_t i = 0; i < 5; ++i) {
                    if (weap_type == 0 && MOX::_weapon_arc_field[i] == input) {
                        if (_valid_arcs_racks[i] == 0) {
                            MISC::Get_LBX_Message_(0x4B, invalid_arc_format);
                            snprintf(message_box_text, sizeof(message_box_text), invalid_arc_format, arc_strings[i]);
                            GENDRAW::Message_Box_(message_box_text);
                        } else {
                            _weapon_replacement_arcs = WEAPON_FIRING_ARC_FORWARD << i;
                        }
                        continue;
                    } else if ((weap_type == 1 || weap_type == 2) && MOX::_weapon_rack_field[i] == input) {
                        if (_valid_arcs_racks[i] == 0) {
                            MISC::Get_LBX_Message_(0x4C, invalid_rack_format);
                            snprintf(message_box_text, sizeof(message_box_text), invalid_rack_format, DESIGN::Missile_Rack_Quantity_(i));
                            GENDRAW::Message_Box_(message_box_text);
                        } else {
                            _weapon_replacement_arcs = WEAPON_FIRING_ARC_360;
                            _weapon_replacement_rack = i;
                        }
                        continue;
                    }
                }
            }

            if (input == MOX::_beam_filter_button) {
                _beam_filter_button_status = _beam_filter_button_status == 0 ? 1 : 0;
            } else if (input == MOX::_missile_filter_button) {
                _missile_filter_button_status = _missile_filter_button_status == 0 ? 1 : 0;
            } else if (input == MOX::_bomb_filter_button) {
                _bomb_filter_button_status = _bomb_filter_button_status == 0 ? 1 : 0;
            } else if (input == MOX::_special_filter_button) {
                _special_filter_button_status = _special_filter_button_status == 0 ? 1 : 0;
            }

            for (int16_t i = 1; i < WEAPON_MOD_COUNT; ++i) {
                if (-MOX::_weapon_mod_fields[i] == input) {
                    Print_Weapon_Mod_Help_Description_(i);
                } else if (MOX::_weapon_mod_fields[i] == input && _valid_mods[i] == 1) {
                    MOX::_weapon_mod_field_status[i] = MOX::_weapon_mod_field_status[i] == 1 ? 0 : 1;
                    if (i == 1 && MOX::_weapon_mod_field_status[i] != 0) {
                        MOX::_weapon_mod_field_status[2] = 0;
                    } else if (i == 2 && MOX::_weapon_mod_field_status[i] != 0) {
                        MOX::_weapon_mod_field_status[1] = 0;
                    }
                }
            }

            int16_t field_item_scanned_out;
            int16_t scanned_idx;
            int16_t scan_result = Scan_Replacement_Fields_(scan_input_val, input, &field_item_scanned_out, &scanned_idx);
            _field_item_scanned = field_item_scanned_out;

            if (scan_result == 0) {
                if (scanned_idx == 0) {
                    _weapon_replacement_arcs = WEAPON_FIRING_ARC_NONE;
                    _weapon_replacement_mods = 0;
                    _field_item_chosen = 0;
                    done_flag = 1;
                }

                if (scanned_idx < 0 || _field_item_chosen == scanned_idx) {
                    Update_Weapon_Replacement_(&done_flag, _field_item_chosen, replacement_slot);
                } else {
                    _field_item_chosen = scanned_idx;
                    int16_t weap_idx = Weapon_Index_(scanned_idx);
                    if (weap_idx == -1) {
                        done_flag = 1;
                    } else if (MOX::_design->weapon_type[replacement_slot] < 1 || weap_idx != MOX::_design->weapon_type[replacement_slot]) {
                        int16_t wtype = DESIGN::Weapon_Type_(weap_idx);
                        _weapon_replacement_mods = 0;
                        memset(MOX::_weapon_mod_field_status, 0, sizeof(MOX::_weapon_mod_field_status));
                        _weapon_replacement_arcs = WEAPON_FIRING_ARC_FORWARD;
                        if (wtype != 0) {
                            _weapon_replacement_arcs = WEAPON_FIRING_ARC_360;
                        }
                        _weapon_replacement_rack = 1;
                        Update_Weapon_Modification_Data_();
                        Weapon_Replacement_Help_(_field_item_chosen);
                    } else {
                        Set_Up_Weapon_Display_(MOX::_design_screen_replacement_slot);
                    }
                }
            } else if (scan_result == 1) {
                int16_t weap_idx = Weapon_Index_(scanned_idx);
                if (weap_idx <= 0) {
                    TEXTBOX::Draw_Help_Entry_(0x2B2);
                } else {
                    EVANHELP::Get_Application_Description_(tech_application_description, sizeof(tech_application_description), TECHDATA::_weapons[weap_idx].tech_app_id);
                    TEXTBOX::Text_Box_(TECHDATA::_weapons[weap_idx].name, tech_application_description);
                }
            }

            for (int16_t i = 0; i < WEAPON_MOD_COUNT; ++i) {
                if (MOX::_weapon_mod_fields[i] == input && _valid_mods[i] > 0xF8) {
                    Print_Weapon_Modification_Message_(i);
                }
            }

            if (input == _weapon_arc_rack_box_field) {
                if (_field_item_chosen == -1) {
                    GENDRAW::Message_Box_((char*)HAROLD::H_Message_(0x4E));
                } else {
                    int16_t weap_idx = Weapon_Index_(_field_item_chosen);
                    int16_t wtype = DESIGN::Weapon_Type_(weap_idx);
                    if (wtype == 3 || wtype == 4 || wtype == 5) {
                        MISC::Get_LBX_Message_(0x4F, incompatible_weapon_format);
                        snprintf(message_box_text, sizeof(message_box_text), incompatible_weapon_format, TECHDATA::_weapons[weap_idx].name);
                        GENDRAW::Message_Box_(message_box_text);
                    }
                }
            }

            if (input == MOX::_beam_filter_button || input == MOX::_missile_filter_button ||
                input == MOX::_bomb_filter_button || input == MOX::_special_filter_button) {
                int16_t old_total_rows = MOX::_scroll_bar->total_rows;
                int16_t prev_weap_idx = -1;

                if (_field_item_chosen > -1) {
                    prev_weap_idx = MOX::_weapon_filter[_field_item_chosen].weapon_idx;
                }

                Weapon_Choices_(MOX::_PLAYER_NUM);

                if (MOX::_scroll_bar->total_rows > 0) {
                    qsort(MOX::_weapon_filter, MOX::_scroll_bar->total_rows, sizeof(MOX::_weapon_filter[0]), qsort_by_research_cost_);
                    int16_t rows = MOX::_scroll_bar->total_rows;
                    if (rows < 10) {
                        MOX::_scroll_bar->first_visible_row = 0;
                    } else if (MOX::_scroll_bar->first_visible_row + 10 > rows) {
                        int32_t val = rows - 10;
                        if (val < 0) val = 0;
                        MOX::_scroll_bar->first_visible_row = (int16_t)val;
                    }
                }

                _field_item_chosen = Update_Field_Item_Chosen_(prev_weap_idx);

                if (MOX::_scroll_bar->total_rows != old_total_rows) {
                    Get_Weapons_Box_Base_Dimensions_();
                    redraw_flag = 1;
                }

                Weapon_Replacement_Help_(_field_item_chosen);
                Space_With_Weapons_();
            }

            if (input == _replacement_scroll_up_button) {
                _wait_ctr = 0;
                SCROLL::Scroll_Up_One_Item_(MOX::_scroll_bar);
            }

            if (input == _replacement_scroll_dn_button) {
                _wait_ctr = 0;
                SCROLL::Scroll_Down_One_Item_(MOX::_scroll_bar);
            }

            if (input == MOX::_replacement_box_accept_button) {
                Update_Weapon_Replacement_(&done_flag, _field_item_chosen, replacement_slot);
            }

            if (done_flag != 0) break;

            Update_Weapon_Modification_Data_();
            fields::Clear_Fields_();
            Add_Replacement_Weapon_Fields_(&cancel_btn_field, &background_field);

            if (first_time_flag == 0 && redraw_flag != 0) {
                Redraw_Main_Design_Screen_To_Background_();
                redraw_flag = 0;
            }

            if (done_flag == 0) {
                video::Set_Page_Off_();
                Draw_Weapons_Replacement_Box_();
                if (first_time_flag != 0) {
                    first_time_flag = 0;
                } else {
                    RUSS::Mox_Sync_Update_();
                }
            }
        }

        MOX::_scroll_bar = nullptr;
        allocate::Pop_Block_(MOX::_global_data_seg);
        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();
        MOX::_current_screen = SCREEN_DESIGN;
    }

    int8_t __cdecl Application_Type_(int16_t tech_app_id) {
        return static_cast<int8_t>(TECHDATA::_technology_applications[tech_app_id].type);
    }

    void __cdecl Set_Up_Weapon_Scroll_Bar_() {
        if (MOX::_scroll_bar->total_rows < 10) {
            MOX::_scroll_bar->first_visible_row = 0;
            MOX::_scroll_bar->thumb_center_y = MOX::_scroll_bar->track_len_px;
        } else {
            int16_t step = (1200 / MOX::_scroll_bar->total_rows + 9) / 10;

            if (MOX::_scroll_bar->first_visible_row + MOX::_scroll_bar->visible_rows > MOX::_scroll_bar->total_rows) {
                MOX::_scroll_bar->thumb_center_y = MOX::_scroll_bar->visible_rows * step;
                MOX::_scroll_bar->first_visible_row = MOX::_scroll_bar->total_rows - 10;
            } else {
                MOX::_scroll_bar->thumb_center_y = MOX::_scroll_bar->track_len_px - (_field_item_chosen * step);

                int32_t limit = MOX::_scroll_bar->visible_rows * step;
                if (limit > MOX::_scroll_bar->thumb_center_y) {
                    MOX::_scroll_bar->thumb_center_y = (int16_t)limit;
                }

                if (MOX::_scroll_bar->thumb_center_y > MOX::_scroll_bar->track_len_px) {
                    MOX::_scroll_bar->thumb_center_y = MOX::_scroll_bar->track_len_px;
                }

                if (MOX::_scroll_bar->thumb_center_y < 0) {
                    MOX::_scroll_bar->thumb_center_y = 0;
                }
            }
        }

        MOX::_scroll_bar->thumb_center_y -= MOX::_scroll_bar->thumb_height_px / 2;

        if (MOX::_scroll_bar->thumb_center_y < 0) {
            MOX::_scroll_bar->thumb_center_y = MOX::_scroll_bar->thumb_height_px / 2;
        }

        MOX::_scroll_bar->last_thumb_center_y = -1;
    }

    int16_t __cdecl Weapon_Choices_(int16_t player_idx) {
        memset(MOX::_design->replacement_item_unlocked, 0, sizeof(MOX::_design->replacement_item_unlocked));
        memset(MOX::_weapon_filter, 0xFF, 40 * sizeof(s_weapon_filter_item));

        MOX::_weapon_filter[0].weapon_idx = 0;
        MOX::_weapon_filter[0].tech_cost = 99999;
        MOX::_design->replacement_item_unlocked[0] = 1;
        MOX::_scroll_bar->total_rows = 1;

        for (int16_t w_idx = 1; w_idx < 40; w_idx++) {
            MOX::_design->replacement_item_unlocked[w_idx] = 0;
            uint16_t tech_app_id = TECHDATA::_weapons[w_idx].tech_app_id;

            if (MOX::_player[player_idx].tech_applications[tech_app_id] == TECH_RESEARCH_STATUS_RESEARCHED) {
                if (w_idx > -1 && w_idx < 40) {
                    int16_t w_type = DESIGN::Weapon_Type_(w_idx);
                    bool add_weapon = false;

                    switch (w_type) {
                    case 0:
                        if (_beam_filter_button_status != 0) {
                            add_weapon = true;
                        }
                        break;
                    case 1:
                    case 2:
                        if (_missile_filter_button_status != 0) {
                            add_weapon = true;
                        }
                        break;
                    case 3:
                        if (_bomb_filter_button_status != 0) {
                            add_weapon = true;
                        }
                        break;
                    case 4:
                    case 5:
                        if (_special_filter_button_status != 0) {
                            if (TECHDATA::_weapons[w_idx].weapon_id == WEAPON_TRACTOR_BEAM) {
                                if (!Have_Tractor_Beam_Weapon_()) {
                                    add_weapon = true;
                                }
                            } else {
                                add_weapon = true;
                            }
                        }
                        break;
                    default:
                        break;
                    }

                    if (add_weapon) {
                        int16_t tech_field_id = TECHDATA::_technology_applications[tech_app_id].tech_field_id;
                        int16_t row = MOX::_scroll_bar->total_rows;

                        MOX::_weapon_filter[row].weapon_idx = w_idx;
                        MOX::_weapon_filter[row].tech_cost = TECHDATA::_technology_fields[tech_field_id].cost;

                        int16_t rep_slot = MOX::_design_screen_replacement_slot;
                        if (MOX::_design->weapon_type[rep_slot] > 0 &&
                            MOX::_design->weapon_type[rep_slot] == w_idx) {
                            _field_item_chosen = row;
                        }

                        MOX::_scroll_bar->total_rows++;
                    }
                }
            }

            if (MOX::_scroll_bar->total_rows >= 40) {
                break;
            }
        }

        Set_Up_Weapon_Scroll_Bar_();

        int16_t valid_weapons = 0;

        if (MOX::_scroll_bar->total_rows > 1) {
            for (int16_t i = 1; i < MOX::_scroll_bar->total_rows; i++) {
                int16_t w_idx = Weapon_Index_(i);
                int16_t needs_rack = DESIGN::Weapon_Needs_Racks_(w_idx);
                uint16_t tech_app_id = TECHDATA::_weapons[w_idx].tech_app_id;
                int16_t size1, size2;

                if (needs_rack != 0) {
                    int8_t m_cat = DESIGN::Miniaturization_Category_(tech_app_id);
                    int16_t tech_level = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, tech_app_id);
                    size1 = static_cast<int16_t>(DESIGN::Space_Given_M_Level_(TECHDATA::_weapons[w_idx].size, tech_level, m_cat));

                    int16_t tech_level2 = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, tech_app_id);
                    int16_t rack_size = DESIGN::Weapon_Rack_Size_(2);
                    size2 = static_cast<int16_t>(DESIGN::Space_Given_M_Level_(rack_size, tech_level2, m_cat));
                } else {
                    int8_t m_cat = DESIGN::Miniaturization_Category_(tech_app_id);
                    int16_t tech_level = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, tech_app_id);
                    size1 = static_cast<int16_t>(DESIGN::Space_Given_M_Level_(TECHDATA::_weapons[w_idx].size, tech_level, m_cat));
                    size2 = 0;
                }

                int16_t total_space = static_cast<int16_t>(MOX::_design->space_used) + size1 + size2;
                int16_t rep_slot = MOX::_design_screen_replacement_slot;

                if (MOX::_design->weapon_type[rep_slot] > 0) {
                    total_space -= static_cast<int16_t>(MOX::_design->weapon_space[rep_slot]);
                }

                if (MOX::_design->hull_space >= total_space) {
                    valid_weapons++;
                }
            }
        }

        if (valid_weapons != 0) {
            return 1;
        }

        strlcpy(MOX::_temp_string, HAROLD::H_Message_(0x45), sizeof(MOX::_temp_string));
        return 0;
    }

    void __cdecl Draw_Weapons_Mod_Box_(int16_t x, int16_t y) {
        int16_t mod_types[WEAPON_MOD_COUNT] = {0};
        int16_t mod_y[WEAPON_MOD_COUNT] = {0};
        int16_t mod_x[WEAPON_MOD_COUNT] = {0};

        animate::Draw_(x + 0xe0, y + 0x46, MOX::_weapons_blank_mods_seg);

        Get_Weapon_Mod_Field_XYs_(mod_types, mod_x, mod_y);

        for (int16_t i = 1; i < WEAPON_MOD_COUNT; i++) {
            if (_field_item_chosen > -1) {
                if (Mod_Allowed_On_Weapon_Type_((uint8_t)i)) {
                    if (Should_Add_Mod_(i)) {
                        int32_t frame;

                        if (MOX::_weapon_mod_field_status[i] == 1) {
                            frame = 2;
                        } else if (_valid_mods[i] == 1) {
                            frame = 1;
                        } else {
                            frame = 0;
                        }

                        s_animation_header* anim = MOX::_weapons_special_seg[mod_types[i]];
                        animate::Set_Animation_Frame_(anim, frame);
                        animate::Draw_(x + mod_x[i], y + mod_y[i], anim);
                    }
                }
            }
        }
    }

    bool __cdecl Have_Tractor_Beam_Weapon_() {
        bool has_tractor_beam = false;

        if (MOX::_design_screen_replacement_slot == -1 ||
            MOX::_design->weapon_type[MOX::_design_screen_replacement_slot] != WEAPON_TRACTOR_BEAM) {

            for (int16_t i = 0; i < 8; ++i) {
                if (has_tractor_beam) {
                    break;
                }
                if (MOX::_design->weapon_type[i] == WEAPON_TRACTOR_BEAM) {
                    has_tractor_beam = true;
                }
            }
        }

        return has_tractor_beam;
    }

    void __cdecl Space_With_Weapons_() {
        MOX::_design->replacement_item_unlocked[0] = 1;

        for (int16_t i = 1; i < 40; ++i) {
            int16_t weapon_idx = Weapon_Index_(i);
            MOX::_design->replacement_item_unlocked[i] = 0;

            if (weapon_idx > -1) {
                if (MOX::_player[MOX::_PLAYER_NUM].tech_applications[TECHDATA::_weapons[weapon_idx].tech_app_id] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    int16_t space = Space_With_Weapon_(weapon_idx);
                    if (space <= MOX::_design->hull_space) {
                        MOX::_design->replacement_item_unlocked[i] = 1;
                    }
                }
            }
        }
    }

    int16_t __cdecl Space_With_Weapon_(int16_t weapon_idx) {
        int16_t zero_space = 0;
        int16_t rack_space;
        int16_t weapon_space;

        if (weapon_idx > 0) {
            if (DESIGN::Weapon_Needs_Racks_(weapon_idx)) {
                uint16_t tech_app_id = TECHDATA::_weapons[weapon_idx].tech_app_id;
                int8_t category = DESIGN::Miniaturization_Category_(tech_app_id);
                int16_t tech_level = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, tech_app_id);
                weapon_space = (int16_t)DESIGN::Space_Given_M_Level_(TECHDATA::_weapons[weapon_idx].size, tech_level, category);

                tech_app_id = TECHDATA::_weapons[weapon_idx].tech_app_id;
                category = DESIGN::Miniaturization_Category_(tech_app_id);
                tech_level = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, tech_app_id);
                int16_t rack_base_size = DESIGN::Weapon_Rack_Size_(2);
                rack_space = (int16_t)DESIGN::Space_Given_M_Level_(rack_base_size, tech_level, category);
            } else {
                uint16_t tech_app_id = TECHDATA::_weapons[weapon_idx].tech_app_id;
                int8_t category = DESIGN::Miniaturization_Category_(tech_app_id);
                int16_t tech_level = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, tech_app_id);
                weapon_space = (int16_t)DESIGN::Space_Given_M_Level_(TECHDATA::_weapons[weapon_idx].size, tech_level, category);
                rack_space = 0;
            }
        } else {
            weapon_space = zero_space;
            rack_space = zero_space;
        }

        int16_t total_space;
        memcpy(&total_space, &MOX::_design->space_used, sizeof(total_space));
        total_space = (int16_t)(total_space + rack_space + weapon_space);

        if (MOX::_design_screen_replacement_slot > -1) {
            int slot = MOX::_design_screen_replacement_slot;
            if (MOX::_design->weapon_type[slot] > 0) {
                int16_t slot_weapon_space;
                memcpy(&slot_weapon_space, &MOX::_design->weapon_space[slot], sizeof(slot_weapon_space));
                total_space = (int16_t)(total_space - slot_weapon_space);
            }
        }

        return total_space;
    }

    void __cdecl Set_Up_Weapon_Display_(int16_t weapon_index) {
        memset(MOX::_weapon_mod_field_status, 0, sizeof(MOX::_weapon_mod_field_status));

        _weapon_replacement_arcs = WEAPON_FIRING_ARC_NONE;

        if (MOX::_design->weapon_type[weapon_index] <= 0) {
            _weapon_replacement_mods = 0;
        } else {
            _weapon_replacement_arcs = MOX::_design->weapon_firing_arc[weapon_index];
            _weapon_replacement_mods = MOX::_design->weapon_specials[weapon_index];

            bool found = false;
            if (MOX::_scroll_bar->total_rows > 1) {
                for (int16_t i = 1; i < MOX::_scroll_bar->total_rows; ++i) {
                    if (MOX::_weapon_filter[i].weapon_idx == MOX::_design->weapon_type[weapon_index]) {
                        _field_item_chosen = i;
                        MOX::_scroll_bar->first_visible_row = i;
                        Set_Up_Weapon_Scroll_Bar_();
                        found = true;
                        break;
                    }
                }
            }

            if (!found) {
                _field_item_chosen = -1;
            }

            if (DESIGN::Weapon_Needs_Racks_(MOX::_design->weapon_type[weapon_index]) != 0) {
                _weapon_replacement_rack = DESIGN::Missile_Rack_Index_(
                    MOX::_design->weapon_ammo[weapon_index]);
            } else {
                _weapon_replacement_rack = 1;
            }
        }

        for (int16_t i = 1; i < WEAPON_MOD_COUNT; ++i) {
            if ((_weapon_replacement_mods >> i) & 1) {
                MOX::_weapon_mod_field_status[i] = 1;
            }
        }

        Update_Weapon_Modification_Data_();
    }

    void __cdecl Print_Weapon_Mod_Help_Description_(int16_t mod_id) {
        char help_lbx_name[20];
        char desc_text[2664];
        char mod_text[1332];

        TEXTBOX::Get_Help_Lbx_Name_(help_lbx_name, sizeof(help_lbx_name));
        allocate::Push_Block_(MOX::_screen_seg);

        void* block = allocate::Allocate_Next_Block_(MOX::_screen_seg, 5000);
        allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(s_help_record));

        s_help_record* base_desc = static_cast<s_help_record*>(
            farload::Far_Reload_Next_Data_(help_lbx_name, 0, block, 400, 1, sizeof(s_help_record))
        );
        strlcpy(desc_text, base_desc->body, sizeof(desc_text));
        strlcat(desc_text, "\n\n", sizeof(desc_text));

        s_help_record* mod_desc = static_cast<s_help_record*>(
            farload::Far_Reload_Next_Data_(help_lbx_name, 0, block, mod_id + 400, 1, sizeof(s_help_record))
        );
        strlcpy(mod_text, mod_desc->body, sizeof(mod_text));
        strlcat(desc_text, mod_text, sizeof(desc_text));

        TEXTBOX::Text_Box_(mod_desc->title, desc_text);

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Weapon_Replacement_Help_(int16_t weapon_idx) {
        uint8_t needs_racks = 0;

        if (weapon_idx >= 0 && weapon_idx < MOX::_scroll_bar->total_rows) {
            int16_t actual_weapon_idx = Weapon_Index_(weapon_idx);
            if (actual_weapon_idx > 0 && DESIGN::Weapon_Needs_Racks_(actual_weapon_idx) != 0) {
                needs_racks = 1;
            }
        }

        int16_t total_rows = MOX::_scroll_bar->total_rows;

        EVANHELP::Set_Weapon_Replacement_Screen_Help_List_(
            total_rows,
            _replacement_box_base_x,
            _replacement_box_base_y,
            needs_racks
        );
    }

    void __cdecl Draw_Generic_Replacement_Box_() {
        int16_t type = MOX::_design_screen_replacement_type;
        int16_t current_x = DESBOX::_replacement_box_base_x;
        int16_t current_y = DESBOX::_replacement_box_base_y;

        if (type == TECH_APPLICATION_TYPE_SHIELD || type == TECH_APPLICATION_TYPE_COMPUTER) {
            animate::Draw_(current_x, current_y, MOX::_system_repl_box_top_seg);
        }

        current_y += animate::Get_Height_(MOX::_system_repl_box_top_seg);

        graphics::Set_Window_(current_x, current_y, current_x + DESBOX::_replacement_box_width, current_y + DESBOX::_replacement_box_middle_height);
        video::Clip_On_();
        animate::Draw_(current_x, current_y, MOX::_system_repl_box_mid_seg);
        video::Clip_Off_();
        graphics::Reset_Window_();

        current_y += DESBOX::_replacement_box_middle_height;
        animate::Draw_(current_x, current_y, MOX::_system_repl_box_btm_seg);

        if (type == TECH_APPLICATION_TYPE_SHIELD) {
            DESBOX::Print_Shield_Data_(DESBOX::_replacement_box_base_x, DESBOX::_replacement_box_base_y);
        } else if (type == TECH_APPLICATION_TYPE_COMPUTER) {
            DESBOX::Print_Computer_Data_(DESBOX::_replacement_box_base_x, DESBOX::_replacement_box_base_y);
        }

        JIM::Play_Streaming_Music_(8, -1, 1);
    }

    void __cdecl Draw_Darkened_Design_Screen_() {
        video::Set_Page_Off_();
        DESIGN::Draw_Design_Screen_();
        RUSS::Mox_Sync_Update_();
        video::Copy_Off_To_Back_();
        graphics::Reset_Window_();
        video::Clip_Off_();
        remap::Update_Glass_Remap_Colors_();
        palette::Set_Palette_Changes_(0, 0xFF);
        video::Set_Page_Off_();
        MISC::_Darken_Fill_(0, 0, 0x27F, 0x1DF);
        video::Copy_Off_To_Back_();
    }

    void __cdecl Update_Weapon_Modification_Data_() {
        uint16_t mods = 0;
        for (int16_t i = 1; i < WEAPON_MOD_COUNT; ++i) {
            if (MOX::_weapon_mod_field_status[i] == 1) {
                mods |= (1 << i);
            }
            _weapon_replacement_mods = mods;
        }
    }

    int16_t __cdecl Weapon_Index_(int16_t index) {
        return MOX::_weapon_filter[index].weapon_idx;
    }

    int __cdecl qsort_by_research_cost_(const void* a, const void* b) {
        const s_weapon_filter_item* item1 = static_cast<const s_weapon_filter_item*>(a);
        const s_weapon_filter_item* item2 = static_cast<const s_weapon_filter_item*>(b);
        return item2->tech_cost - item1->tech_cost;
    }

    void __cdecl Set_New_Weapon_Modifications_(uint16_t* modifications) {
        *modifications = 0;
        if (_field_item_chosen > -1) {
            Weapon_Index_(_field_item_chosen);
            *modifications = 0;

            for (int16_t i = 1; i < WEAPON_MOD_COUNT; ++i) {
                if (MOX::_weapon_mod_field_status[i] == 1) {
                    *modifications |= (1 << i);
                }
            }
        }
    }

    void __cdecl Clear_Weapon_Replacement_Fields_() {
        for (int16_t i = 0; i < 10; ++i) {
            MOX::_repl_fields[i] = -1000;
        }

        for (int16_t i = 0; i < 5; ++i) {
            MOX::_weapon_arc_field[i] = -1000;
            MOX::_weapon_rack_field[i] = -1000;
        }

        for (int16_t i = 0; i < WEAPON_MOD_COUNT; ++i) {
            MOX::_weapon_mod_fields[i] = -1000;
        }
    }

    int8_t __cdecl Should_Add_Mod_(int16_t mod_idx) {
        uint8_t mod_status = DESBOX::_valid_mods[mod_idx];

        if (mod_status == 0x01 || mod_status == 0xF9 || mod_status == 0xFC || mod_status == 0xFB) {
            return 1;
        }

        return 0;
    }

    int8_t __cdecl Mod_Allowed_On_Weapon_Type_(uint8_t mod_idx) {
        int16_t weapon_type = -1;

        if (DESBOX::_field_item_chosen > -1) {
            int16_t weapon_index = DESBOX::Weapon_Index_(DESBOX::_field_item_chosen);
            weapon_type = DESIGN::Weapon_Type_(weapon_index);
        }

        uint8_t shift = mod_idx & 0x1F;

        if (weapon_type == WEAPON_TYPE_BEAM) {
            return (int8_t)((0x01FE >> shift) & 1);
        }

        if (weapon_type == WEAPON_TYPE_MISSILE) {
            return (int8_t)((0x3E00 >> shift) & 1);
        }

        if (weapon_type == WEAPON_TYPE_TORPEDO) {
            return (int8_t)((0x6520 >> shift) & 1);
        }

        return 0;
    }

    int16_t __cdecl Weapon_Choice_Valid_(char* error_msg_out, int32_t error_msg_out_size) {
        int16_t is_valid = 1;
        if (error_msg_out_size > 0) {
            error_msg_out[0] = '\0';
        }

        if (DESBOX::_field_item_chosen > -1) {
            int16_t weapon_idx = DESBOX::Weapon_Index_(DESBOX::_field_item_chosen);
            if (DESIGN::Weapon_Type_(weapon_idx) == WEAPON_TYPE_BEAM && DESBOX::_weapon_replacement_arcs == -1) {
                is_valid = 0;
                const char* msg = HAROLD::H_Message_(0x44 /* MSG_MUST_SELECT_FIRING_ARC */);
                if (error_msg_out_size > 0) {
                    strlcpy(error_msg_out, msg, (size_t)error_msg_out_size);
                }
            }
        }

        // Bound check the selection against scroll bar total rows.
        if (DESBOX::_field_item_chosen < 0 || DESBOX::_field_item_chosen > MOX::_scroll_bar->total_rows) {
            DESBOX::_field_item_chosen = 0;
        }

        return is_valid;
    }

    void __cdecl Get_Weapon_Mod_Field_XYs_(int16_t* mod_indices, int16_t* x_coords, int16_t* y_coords) {
        int16_t mod_count = 0;
        int16_t y_offset = 11;

        if (DESBOX::_field_item_chosen <= -1) {
            return;
        }

        for (int16_t i = 1; i < WEAPON_MOD_COUNT; ++i) {
            int16_t mod_idx = -1;

            if (DESBOX::Should_Add_Mod_(i)) {
                int16_t weapon_idx = DESBOX::Weapon_Index_(DESBOX::_field_item_chosen);
                int16_t weapon_type = DESIGN::Weapon_Type_(weapon_idx);

                // Mod indices mapping for Beam (0), Missile (1), and Fighter (2) types
                if (weapon_type == 0 || weapon_type == 1 || weapon_type == 2) {
                    switch (i) {
                        case 1:  mod_idx = 0; break;
                        case 2:  mod_idx = 1; break;
                        case 3:  mod_idx = 2; break;
                        case 4:  mod_idx = 3; break;
                        case 5:  mod_idx = (weapon_type == 0) ? 4 : 13; break;
                        case 6:  mod_idx = 5; break;
                        case 7:  mod_idx = 6; break;
                        case 8:  mod_idx = (weapon_type == 0) ? 7 : 14; break;
                        case 9:  mod_idx = 8; break;
                        case 10: mod_idx = (weapon_type == 1) ? 12 : 16; break;
                        case 11: mod_idx = 9; break;
                        case 12: mod_idx = 10; break;
                        case 13: mod_idx = (weapon_type == 1) ? 11 : 15; break;
                        case 14: mod_idx = 17; break;
                    }
                }

                // Vertical UI alignment based on weapon category
                switch (weapon_type) {
                    case 0: /* Beam */
                    case 2: /* Fighter */
                    case 3: /* Special */
                        y_offset = 17;
                        break;
                    case 1: /* Missile */
                        y_offset = 21;
                        break;
                }

                // UI list is organized into two columns
                if (mod_count % 2 == 0) {
                    x_coords[i] = 233 /* 0xE9 */;
                } else {
                    x_coords[i] = 386 /* 0x182 */;
                }

                y_coords[i] = (mod_count / 2) * 17 + y_offset + 70 /* 0x46 */;
                mod_indices[i] = mod_idx;
                mod_count++;
            }
        }
    }

    void __cdecl Print_Main_Weapon_Box_(int16_t x, int16_t y) {
        s_colors base_colors = *MISC::Font_Colors2_(3, 0x55, 0x5B);
        fonts::Set_Font_Style_(3, &base_colors);

        int16_t ty = y + 3;
        fonts::Print_(x + 0x1C, ty, HAROLD::H_Message_(0x2E));
        fonts::Print_Right_(x + 0xC6, ty, HAROLD::H_Message_(0x2F));
        fonts::Print_(x + 0xFD, ty, HAROLD::H_Message_(0x30));
        fonts::Print_(x + 0xD3, ty, HAROLD::H_Message_(0x2B));
        fonts::Print_(x + 0x183, ty, HAROLD::H_Message_(0x31));

        int16_t row_idx = MOX::_scroll_bar->first_visible_row;
        int16_t loop_count = 0;
        int16_t cur_y = y + 0x13;

        while (true) {
            bool selected = false;
            int16_t weapon_idx = Weapon_Index_(row_idx);
            int32_t weapon_cost = 0;
            int32_t weapon_space = 0;

            if (row_idx > 0) {
                if (_field_item_chosen <= -1 || (weapon_idx > 0 && _field_item_chosen != row_idx)) {
                    if (weapon_idx > 0) {
                        int16_t rack_ammo = (DESIGN::Weapon_Needs_Racks_(weapon_idx) != 0)
                            ? DESIGN::Missile_Rack_Quantity_(1)
                            : 0;
                        weapon_cost = DESIGN::Weapon_Cost_(weapon_idx, 1, WEAPON_FIRING_ARC_FORWARD, rack_ammo, 0);
                        weapon_space = DESIGN::Weapon_Space_(MOX::_PLAYER_NUM, weapon_idx, 1, WEAPON_FIRING_ARC_FORWARD, rack_ammo, 0);
                    }
                } else if (weapon_idx > 0 && _field_item_chosen == row_idx) {
                    int16_t rack_ammo = 0;
                    if (DESIGN::Weapon_Needs_Racks_(weapon_idx) != 0) {
                        rack_ammo = DESIGN::Missile_Rack_Quantity_(_weapon_replacement_rack);
                    }
                    weapon_cost = DESIGN::Weapon_Cost_(weapon_idx, 1, _weapon_replacement_arcs, rack_ammo, _weapon_replacement_mods);
                    weapon_space = DESIGN::Weapon_Space_(MOX::_PLAYER_NUM, weapon_idx, 1, _weapon_replacement_arcs, rack_ammo, _weapon_replacement_mods);
                }
            }

            if (_field_item_chosen > -1 && _field_item_chosen == row_idx) {
                selected = true;
            }

            if (_field_item_scanned == loop_count && row_idx > -1 && MOX::_design->replacement_item_unlocked[row_idx] != 0) {
                s_colors colors = *MISC::Font_Colors2_(2, 0x55, 0x58);
                fonts::Set_Font_Style_(2, &colors);
                MISC::Increase_Font_Color_Range_(MOX::_multi_colored_line_start);
            } else {
                s_colors* pcolors;
                if (selected) {
                    pcolors = MISC::Font_Colors2_(2, 0x55, 0x5C);
                } else if (row_idx <= -1) {
                    pcolors = MISC::Font_Colors2_(2, 0x55, 0x59);
                } else if (MOX::_design->replacement_item_unlocked[row_idx] != 0) {
                    pcolors = MISC::Font_Colors2_(2, 0x55, 0x5A);
                } else {
                    pcolors = MISC::Font_Colors2_(2, 0x55, 0x57);
                }
                s_colors colors = *pcolors;
                fonts::Set_Font_Style_(2, &colors);
            }

            char cost_str[20];
            char space_str[20];
            snprintf(cost_str, sizeof(cost_str), "%d", weapon_cost);
            snprintf(space_str, sizeof(space_str), "%d", weapon_space);

            if (selected) {
                animate::Draw_(x, cur_y + 1, MOX::_weapons_selected_seg);
            }

            int16_t num_weapons = 0;
            if (weapon_space > 0) {
                // Keep original game bug: division of 0 by weapon_space instead of remaining space
                num_weapons = 0 / weapon_space;
                if (num_weapons >= 99) {
                    num_weapons = 99;
                }
                if (MOX::_design->replacement_item_unlocked[row_idx] != 0 && num_weapons <= 1) {
                    num_weapons = 1;
                }
            }

            char name_str[80];
            if (weapon_idx <= 0) {
                strlcpy(name_str, HAROLD::H_Message_(0x32), sizeof(name_str));
            } else {
                snprintf(name_str, sizeof(name_str), "%s (%d)", TECHDATA::_weapons[weapon_idx].name, num_weapons);
            }

            fonts::Set_Font_Spacing_(1);
            fonts::Print_(x + 0x0C, cur_y, name_str);
            MISC::Set_Font_Default_Spacing_();

            DESIGN::Design_Weapon_Damage_String_(weapon_idx, name_str, sizeof(name_str), _field_item_chosen, row_idx);

            if (strcmp(name_str, HAROLD::H_Message_(0x65)) == 0) {
                MISC::Change_Font_Style_(1);
                fonts::Print_Right_(x + 0xC6, cur_y + 2, name_str);
                MISC::Change_Font_Style_(2);
            } else {
                fonts::Print_Right_(x + 0xC6, cur_y, name_str);
            }

            fonts::Print_Right_(x + 0xE9, cur_y, cost_str);
            fonts::Print_Right_(x + 0x11F, cur_y, space_str);

            if (weapon_idx > 0) {
                DESIGN::Tech_Level_(MOX::_PLAYER_NUM, TECHDATA::_weapons[weapon_idx].tech_app_id);
                if (DESIGN::Weapon_Needs_Racks_(weapon_idx) > 0) {
                    DESIGN::Weapon_Rack_Size_(2);
                }
            }

            if (weapon_idx == 0) {
                name_str[0] = '\0';
            } else {
                strlcpy(name_str, MOX::_weapon_notes_string[weapon_idx], sizeof(name_str));
                s_colors colors;
                fonts::Get_Colors_(&colors);
                fonts::Set_Font_Style_(2, &colors);
                fonts::Set_Font_Spacing_(1);
                fonts::Print_(x + 0x131, cur_y, name_str);
                MISC::Set_Font_Default_Spacing_();
            }

            cur_y += 14;
            loop_count++;
            row_idx = MOX::_scroll_bar->first_visible_row + loop_count;

            if (loop_count >= 10 || MOX::_scroll_bar->total_rows <= row_idx) {
                _wait_ctr++;
                if (_wait_ctr >= 2) {
                    _wait_ctr = 0;
                    MOX::_multi_colored_line_start += static_cast<int8_t>(_incr_value);
                    if (MOX::_multi_colored_line_start >= 3) {
                        _incr_value = -1;
                        MOX::_multi_colored_line_start = 3;
                    } else if (MOX::_multi_colored_line_start <= -1) {
                        _incr_value = 1;
                        MOX::_multi_colored_line_start = -1;
                    }
                }
                break;
            }
        }
    }

    void __cdecl Print_Shield_Data_(int16_t x, int16_t y) {
        s_colors title_colors = *MISC::Font_Colors2_(4, 'U', '[');

        graphics::Set_Window_(x, y, x + DESBOX::_replacement_box_width, y + DESBOX::_replacement_box_height);
        video::Clip_On_();

        int16_t text_y = y + 0x37;
        fonts::Set_Font_Style_(4, &title_colors);

        int16_t text_x = x + 0x14;

        char msg_34[80];
        MISC::Get_LBX_Message_(0x34, msg_34);

        char msg_35[80];
        MISC::Get_LBX_Message_(0x35, msg_35);

        fonts::Print_(text_x, text_y, HAROLD::H_Message_(0x33));
        fonts::Print_(text_x + 0xdc, text_y, msg_34);
        fonts::Print_(text_x + 0x15e, text_y, msg_35);

        int16_t msg_34_width = fonts::Get_String_Width_(msg_34);
        int16_t msg_35_width = fonts::Get_String_Width_(msg_35);

        s_colors color_selected = *MISC::Font_Colors2_(3, 'U', '[');
        s_colors color_unlocked = *MISC::Font_Colors2_(3, 'U', 'Y');
        s_colors color_locked = *MISC::Font_Colors2_(3, 'U', 'W');

        text_y += 0x12;

        for (int16_t i = 0; i < MOX::_scroll_bar->total_rows; ++i) {
            s_colors color_scanned;
            s_colors* current_colors;

            if (MOX::_repl_fields[i] == static_cast<int16_t>(DESBOX::_field_item_scanned) && MOX::_design->replacement_item_unlocked[i] != 0) {
                color_scanned.colors[0] = 0;
                for (int j = 1; j < 8; ++j) {
                    color_scanned.colors[j] = color_unlocked.colors[j] + static_cast<uint8_t>(MOX::_multi_colored_line_start);
                }
                current_colors = &color_scanned;
            } else if (DESBOX::_field_item_chosen == i) {
                current_colors = &color_selected;
            } else if (MOX::_design->replacement_item_unlocked[i] != 0) {
                current_colors = &color_unlocked;
            } else {
                current_colors = &color_locked;
            }

            fonts::Set_Font_Style_(3, current_colors);

            int16_t item = MOX::_design_choice_items[i];
            if (item == 0) {
                fonts::Print_(text_x + 3, text_y, HAROLD::H_Message_(0x36));

                // Potential bug: original code swaps msg_34_width and msg_35_width centering calculations here
                fonts::Print_Integer_Centered_(text_x + 0xdc + msg_35_width / 2, text_y, 0);
                fonts::Print_Integer_Centered_(text_x + 0x15e + msg_34_width / 2, text_y, 0);

                text_y += 14;
            } else if (item > 0) {
                fonts::Print_(text_x + 3, text_y, TECHDATA::_shields[item].name);

                int16_t tech_level = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, TECHDATA::_shields[item].tech_app_id);
                int16_t cost = DESIGN::Cost_Given_M_Level_(TECHDATA::_shields[item].cost_per_ship_type[MOX::_design->ship_size], tech_level);

                // Potential bug: original code swaps msg_34_width and msg_35_width centering calculations here
                fonts::Print_Integer_Centered_(text_x + 0xdc + msg_35_width / 2, text_y, cost);

                tech_level = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, TECHDATA::_shields[item].tech_app_id);
                int16_t space = static_cast<int16_t>(DESIGN::Space_Given_M_Level_(TECHDATA::_shields[item].space_per_ship_type[MOX::_design->ship_size], tech_level, 0));

                fonts::Print_Integer_Centered_(text_x + 0x15e + msg_34_width / 2, text_y, space);

                text_y += 14;
            }
        }

        graphics::Reset_Window_();
        video::Clip_Off_();

        DESBOX::_wait_ctr++;
        if (DESBOX::_wait_ctr >= 2) {
            DESBOX::_wait_ctr = 0;
            MOX::_multi_colored_line_start += (int16_t)DESBOX::_incr_value;

            if (MOX::_multi_colored_line_start >= 3) {
                DESBOX::_incr_value = -1;
                MOX::_multi_colored_line_start = 3;
            } else if (MOX::_multi_colored_line_start <= -1) {
                DESBOX::_incr_value = 1;
                MOX::_multi_colored_line_start = -1;
            }
        }
    }

    void __cdecl Print_Computer_Data_(int16_t x, int16_t y) {
        s_colors colors_0;
        s_colors colors_1;
        s_colors colors_2;
        s_colors colors_3;
        char lbx_msg1[80];
        char lbx_msg2[80];
        char lbx_msg3[80];
        char sprint_buf[80];

        colors_0 = *MISC::Font_Colors2_(4, 0x55, 0x5B);

        graphics::Set_Window_(
            x,
            y,
            x + _replacement_box_width,
            y + _replacement_box_height
        );

        video::Clip_On_();

        int16_t current_y = y + 0x37;

        fonts::Set_Font_Style_(4, &colors_0);

        int16_t current_x = x + 0x14;

        MISC::Get_LBX_Message_(0x29, lbx_msg1);
        MISC::Get_LBX_Message_(0x2A, lbx_msg2);
        MISC::Get_LBX_Message_(0x2B, lbx_msg3);

        int16_t x1 = current_x + 3;

        fonts::Print_(x1, current_y, lbx_msg1);
        fonts::Print_(current_x + 0xE1, current_y, lbx_msg2);
        fonts::Print_(current_x + 0x195, current_y, lbx_msg3);

        int16_t w1 = fonts::Get_String_Width_(lbx_msg1);
        fonts::Get_String_Width_(lbx_msg2);

        current_y += 0x12;

        int16_t w3 = fonts::Get_String_Width_(lbx_msg3);

        colors_0 = *MISC::Font_Colors2_(3, 0x55, 0x5B);
        colors_1 = *MISC::Font_Colors2_(3, 0x55, 0x59);
        colors_2 = *MISC::Font_Colors2_(3, 0x55, 0x57);

        for (int16_t idx = 0; idx < MOX::_scroll_bar->total_rows; idx++) {
            s_colors* current_color;

            if (MOX::_repl_fields[idx] == _field_item_scanned && MOX::_design->replacement_item_unlocked[idx] != 0) {
                colors_3.colors[0] = 0;
                for (int i = 1; i < 8; i++) {
                    colors_3.colors[i] = colors_1.colors[i] + static_cast<uint8_t>(MOX::_multi_colored_line_start);
                }
                current_color = &colors_3;
            } else if (idx == _field_item_chosen) {
                current_color = &colors_0;
            } else if (MOX::_design->replacement_item_unlocked[idx] != 0) {
                current_color = &colors_1;
            } else {
                current_color = &colors_2;
            }

            fonts::Set_Font_Style_(3, current_color);

            int16_t item = MOX::_design_choice_items[idx];
            int16_t final_cost = 0;

            if (item == 0) {
                fonts::Print_(x1, current_y, HAROLD::H_Message_(0x2D));
                fonts::Print_Integer_Centered_(current_x + 0xE1 + w1 / 2, current_y, 0);
            } else if (item > 0) {
                int16_t tech_lvl = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, TECHDATA::_computers[item].tech_app_id);
                int16_t comp_cost = DESIGN::Computer_Cost_(item);
                final_cost = DESIGN::Cost_Given_M_Level_(comp_cost, tech_lvl);

                fonts::Print_(x1, current_y, TECHDATA::_computers[item].name);

                snprintf(sprint_buf, sizeof(sprint_buf), strings::s_fmt_plus_d_00557ed0, TECHDATA::_computers[item].bonus);
                fonts::Print_Centered_(current_x + 0xE1 + w1 / 2, current_y, sprint_buf);
            } else {
                continue;
            }

            fonts::Print_Integer_Centered_(current_x + 0x195 + w3 / 2, current_y, final_cost);
            current_y += 0xE;
        }

        graphics::Reset_Window_();
        video::Clip_Off_();

        _wait_ctr++;
        if (_wait_ctr >= 2) {
            _wait_ctr = 0;
            MOX::_multi_colored_line_start += (int16_t)_incr_value;

            if (MOX::_multi_colored_line_start >= 3) {
                _incr_value = -1;
                MOX::_multi_colored_line_start = 3;
            } else if (MOX::_multi_colored_line_start <= -1) {
                _incr_value = 1;
                MOX::_multi_colored_line_start = -1;
            }
        }
    }

    void __cdecl Set_Up_Special_System_Scroll_Bar_() {
        int temp_calc;
        int16_t step_size;
        int16_t center_y;

        // Load initial track length
        center_y = MOX::_scroll_bar->track_len_px;

        // Calculate step_size:
        // row_step is treated as unsigned (uint8_t) for this calculation based on XOR EDX,EDX / MOV DL load.
        // Formula: (((10 * row_step * track_len_px) + 90) / total_rows) / 10
        temp_calc = 10 * (int)(uint8_t)MOX::_scroll_bar->row_step * (int)MOX::_scroll_bar->track_len_px + 90;
        step_size = (int16_t)((temp_calc / (int)MOX::_scroll_bar->total_rows) / 10);

        // Subtract half thumb height (standard signed division used in assembly)
        center_y -= (int16_t)(MOX::_scroll_bar->thumb_height_px / 2);

        MOX::_scroll_bar->first_visible_row = (int16_t)MOX::_design->special_system_scroll_first_row;

        // Final thumb center y calculation
        center_y -= (int16_t)(MOX::_scroll_bar->first_visible_row * step_size);

        MOX::_scroll_bar->thumb_center_y = center_y;
        MOX::_scroll_bar->last_thumb_center_y = center_y;
        MOX::_scroll_bar->thumb_center_y_copy = center_y;
    }

    void __cdecl Draw_Special_Systems_Replacement_Box() {
        int16_t x = _replacement_box_base_x;
        int16_t y = _replacement_box_base_y;

        animate::Draw_(x, y, MOX::_system_repl_box_top_seg);

        y += animate::Get_Height_(MOX::_system_repl_box_top_seg);

        graphics::Set_Window_(x, y, x + _replacement_box_width, y + _replacement_box_middle_height);
        video::Clip_On_();
        animate::Draw_(x, y, MOX::_system_repl_box_mid_seg);
        video::Clip_Off_();
        graphics::Reset_Window_();

        animate::Draw_(x, y + _replacement_box_middle_height, MOX::_system_repl_box_btm_seg);

        Print_Special_System_Data_(_replacement_box_base_x, _replacement_box_base_y);

        if (MOX::_scroll_bar->total_rows > 10 && _replacement_scroll_field != -1000) {
            SCROLL::Draw_Generic_Vertical_Scroll_Bar_(MOX::_scroll_bar, Draw_Special_Systems_Scroll_Bar_);
        }

        JIM::Play_Streaming_Music_(8, -1, 1);
    }

    void __cdecl Special_System_Replacement_Box_Dimensions_(int32_t ignored_param, int16_t row_count) {
        int16_t cancel_btn_h;
        int16_t top_seg_h;
        int16_t btm_seg_h;

        cancel_btn_h = animate::Get_Height_(MOX::_system_repl_box_cancel_btn);
        top_seg_h = animate::Get_Height_(MOX::_system_repl_box_top_seg);
        btm_seg_h = animate::Get_Height_(MOX::_system_repl_box_btm_seg);

        if (row_count > 10) {
            row_count = 10;
        }

        // Calculation: (row_count * 16) + cancel_btn_height + 20
        DESBOX::_replacement_box_middle_height = (row_count * 16) + cancel_btn_h + 20;

        // Total height = middle_height + top_segment_height + bottom_segment_height
        DESBOX::_replacement_box_height = DESBOX::_replacement_box_middle_height + top_seg_h + btm_seg_h;

        DESBOX::_replacement_box_width = animate::Get_Width_(MOX::_system_repl_box_top_seg);

        // Center calculation on 640x440 coordinate space
        DESBOX::_replacement_box_base_x = (640 - DESBOX::_replacement_box_width) / 2;
        DESBOX::_replacement_box_base_y = (440 - DESBOX::_replacement_box_height) / 2;
    }

    int16_t __cdecl Special_System_Exclusion_(int16_t special_idx, int16_t* out_conflict_id) {
        int16_t found = 0;
        *out_conflict_id = 0;

        for (int16_t i = 0; i < 6; ++i) {
            // Check if conflict already found in previous iterations
            if (found != 0) {
                break;
            }

            int16_t exclude_id = (int16_t)TECHDATA::_specials[special_idx].exclude[i];
            if (exclude_id > 0) {
                for (int16_t j = 0; j < 8; ++j) {
                    if (found != 0) {
                        break;
                    }

                    int16_t current_special = MOX::_design->special_devices[j];
                    if (exclude_id == current_special) {
                        // A conflict is only valid if the conflicting special is not the one 
                        // currently being replaced/selected in the design screen
                        if (MOX::_design->special_devices[MOX::_design_screen_replacement_slot] != current_special) {
                            found = 1;
                            *out_conflict_id = current_special;
                        }
                    }
                }
            }
        }

        return found;
    }

    int16_t __cdecl Items_Selected_() {
        int16_t selected_count = 0;
        int16_t row_count = MOX::_scroll_bar->total_rows;
        if (row_count > 10) {
            row_count = 10;
        }
        for (int16_t i = 0; i < row_count; ++i) {
            if (MOX::_design_choice_item_selected[i] != 0) {
                selected_count++;
            }
        }

        return selected_count;
    }

    void __cdecl Add_Special_System_Fields_(int16_t* cancel_field_id_out, int16_t* screen_field_id_out) {
        int16_t scroll_x;
        int16_t scroll_y;
        int16_t loop_limit;

        loop_limit = MOX::_scroll_bar->total_rows;
        if (loop_limit > 10) {
            loop_limit = 10;
        }

        if (loop_limit > 0) {
            for (int16_t i = 0; i < loop_limit; ++i) {
                int16_t field_y1 = _replacement_box_base_y + 89 /* 0x59 */ + (i * 16);
                MOX::_repl_fields[i] = fields::Add_Hidden_Field_(
                    _replacement_box_base_x + 25 /* 0x19 */,
                    field_y1,
                    _replacement_box_base_x + 25 + 559 /* 0x22F */,
                    field_y1 + 16,
                    s__00551100,
                    41 /* 0x29 */
                );

                loop_limit = MOX::_scroll_bar->total_rows;
                if (loop_limit > 10) {
                    loop_limit = 10;
                }
            }
        }

        _replacement_scroll_field = -1000 /* 0xFC18 */;

        if (MOX::_scroll_bar->total_rows > 10) {
            scroll_x = _replacement_box_base_x + 602 /* 0x25A */;
            scroll_y = _replacement_box_base_y + 93 /* 0x5D */;
            _replacement_scroll_field = fields::Add_Scroll_Field_(
                scroll_x,
                scroll_y,
                0,
                140 /* 0x8C */,
                0,
                140 /* 0x8C */,
                9,
                140 /* 0x8C */,
                &MOX::_scroll_bar->thumb_center_y,
                s__00551100,
                0
            );
        } else {
            _replacement_scroll_field = -1000 /* 0xFC18 */;
        }

        if (MOX::_scroll_bar->total_rows > 10 && scroll_x > 0 && scroll_y > 0) {
            int16_t up_btn_y = scroll_y - animate::Get_Height_(MOX::_d_scroll_up_button_seg) - 3;
            MOX::_special_scroll_up_button[0] = fields::Add_Button_Field_(
                scroll_x,
                up_btn_y,
                s__00551100,
                MOX::_d_scroll_up_button_seg,
                "-",
                40 /* 0x28 */
            );

            int16_t dn_btn_y = scroll_y + animate::Get_Height_(MOX::_system_repl_scroll_bar_seg) + 3;
            MOX::_special_scroll_dn_button = fields::Add_Button_Field_(
                scroll_x,
                dn_btn_y,
                s__00551100,
                MOX::_d_scroll_dn_button_seg,
                "+",
                40 /* 0x28 */
            );
        } else {
            MOX::_special_scroll_up_button[0] = -1000 /* 0xFC18 */;
            MOX::_special_scroll_dn_button = -1000 /* 0xFC18 */;
            _replacement_scroll_field = -1000 /* 0xFC18 */;
        }

        int16_t cancel_btn_x = _replacement_box_base_x + _replacement_box_width - animate::Get_Width_(MOX::_system_repl_box_cancel_btn) - 35 /* 0x23 */;
        int16_t cancel_btn_y = _replacement_box_base_y + _replacement_box_height - animate::Get_Height_(MOX::_system_repl_box_cancel_btn) - 16 /* 0x10 */;
        *cancel_field_id_out = fields::Add_Button_Field_(
            cancel_btn_x,
            cancel_btn_y,
            s__00551100,
            MOX::_system_repl_box_cancel_btn,
            s__00551120,
            40 /* 0x28 */
        );

        fields::Add_Hidden_Field_(
            _replacement_box_base_x,
            _replacement_box_base_y,
            _replacement_box_base_x + _replacement_box_width,
            _replacement_box_base_y + _replacement_box_height,
            s__00551100,
            0
        );

        *screen_field_id_out = fields::Add_Hidden_Field_(0, 0, 639 /* 0x27F */, 479 /* 0x1DF */, s__00551100, 41 /* 0x29 */);

        MOX::_scroll_bar->up_btn = MOX::_special_scroll_up_button[0];
        MOX::_scroll_bar->dn_btn = MOX::_special_scroll_dn_button;
    }

    void __cdecl Print_Special_System_Data_(int16_t base_x, int16_t base_y) {
        int16_t cost;
        int16_t space;
        int16_t item_idx;
        int16_t j;
        int16_t fitted_font_style = 3;
        s_colors saved_font_colors;
        s_colors header_colors;
        s_colors unavailable_colors;
        s_colors available_colors;
        char space_str[20];
        char cost_str[20];
        char special_name[80];
        char special_desc[200];

        header_colors = *MISC::Font_Colors2_(4, 'U', '\\');
        fonts::Set_Font_Style_(4, &header_colors);

        int16_t local_y = base_y + 0x46;
        fonts::Print_(base_x + 0x44, local_y, HAROLD::H_Message_(0x37));
        fonts::Print_(base_x + 0xbe, local_y, HAROLD::H_Message_(0x30));
        fonts::Print_(base_x + 0x106, local_y, HAROLD::H_Message_(0x2b));
        fonts::Print_(base_x + 0x156, local_y, HAROLD::H_Message_(0x38));

        unavailable_colors = *MISC::Font_Colors2_(2, 'U', 'W');
        header_colors = *MISC::Font_Colors2_(2, 'U', '\\');
        available_colors = *MISC::Font_Colors2_(3, 'U', 'Z');

        local_y = base_y + 0x59;

        graphics::Set_Window_(base_x + 0x19, base_y + 0x40, base_x + 0x247, _replacement_box_middle_height + base_y + 0x45);
        video::Clip_On_();

        int16_t i = 1;
        do {
            fonts::Set_Font_Spacing_(1);
            fonts::Get_Colors_(&saved_font_colors);
            MISC::Set_Fitted_Font_Style_(TECHDATA::_specials[i].name, &saved_font_colors, fitted_font_style, 1, -1, 0xa5, 1);
            int16_t style = MISC::Get_Font_Style_();
            if (style < fitted_font_style) {
                fitted_font_style = style;
            }
            i++;
        } while (i < 0x27);

        j = 0;
        int16_t total_rows = MOX::_scroll_bar->total_rows;
        if (total_rows > 10) {
            total_rows = 10;
        }

        if (total_rows > 0) {
            do {
                item_idx = j + MOX::_scroll_bar->first_visible_row;
                int16_t special_id = MOX::_design_choice_items[item_idx];
                if (special_id > -1) {
                    System_Added_Cost_And_Space_(item_idx, &cost, &space);

                    strlcpy(special_desc, MOX::_special_desc_string[special_id], sizeof(special_desc));
                    strlcpy(special_name, TECHDATA::_specials[special_id].name, sizeof(special_name));

                    int16_t item_space = space;
                    if (MOX::_design_screen_replacement_slot > -1 && MOX::_design->special_devices[MOX::_design_screen_replacement_slot] > 0) {
                        item_space -= MOX::_design->special_device_space[MOX::_design_screen_replacement_slot];
                    }

                    bool can_fit = true;
                    if (item_idx != 0) {
                        if (MOX::_design->space_used + item_space > MOX::_design->hull_space) {
                            can_fit = false;
                        }
                        else {
                            int16_t excl;
                            if (Special_System_Exclusion_(special_id, &excl) != 0) {
                                can_fit = false;
                            }
                        }
                    }

                    if (!can_fit) {
                        fonts::Set_Font_Style_(fitted_font_style, &unavailable_colors);
                    }
                    else {
                        fonts::Set_Font_Style_(fitted_font_style, &available_colors);
                    }

                    if (_field_item_scanned == j && can_fit) {
                        s_colors temp_colors;
                        temp_colors = *MISC::Font_Colors2_(2, 'U', 'X');
                        fonts::Set_Font_Style_(fitted_font_style, &temp_colors);
                        MISC::Increase_Font_Color_Range_(MOX::_multi_colored_line_start);
                    }

                    if (item_idx == 0) {
                        fonts::Print_(base_x + 0x20, local_y, HAROLD::H_Message_(0x39));
                        fonts::Print_Right_(base_x + 0xe3, local_y, "0");
                        fonts::Print_Right_(base_x + 0x124, local_y, "0");
                        fonts::Print_(base_x + 0x13e, local_y, " ");
                    }
                    else {
                        MISC::Get_Font_Style_();
                        snprintf(cost_str, sizeof(cost_str), "%d", cost);
                        snprintf(space_str, sizeof(space_str), "%d", space);

                        fonts::Set_Font_Spacing_(1);
                        fonts::Print_(base_x + 0x20, local_y, special_name);
                        MISC::Set_Font_Default_Spacing_();

                        fonts::Print_Right_(base_x + 0xe3, local_y, space_str);
                        fonts::Print_Right_(base_x + 0x124, local_y, cost_str);

                        fonts::Get_Colors_(&saved_font_colors);
                        fonts::Set_Font_Style_(fitted_font_style, &saved_font_colors);

                        fonts::Set_Font_Spacing_(1);
                        fonts::Print_(base_x + 0x13e, local_y, special_desc);
                        MISC::Set_Font_Default_Spacing_();
                    }
                }

                local_y += 0x10;
                j++;

                total_rows = MOX::_scroll_bar->total_rows;
                if (total_rows > 10) {
                    total_rows = 10;
                }
            } while (j < total_rows);
        }

        if (MOX::_scroll_bar->first_visible_row > 0) {
            animate::Draw_(_replacement_box_width + _replacement_box_base_x - 0x20,
                _replacement_box_base_y + 0x47,
                MOX::_dull_d_scroll_up_button_seg);
        }

        if (MOX::_scroll_bar->total_rows > 10 &&
            MOX::_scroll_bar->first_visible_row + 10 > MOX::_scroll_bar->total_rows) {
            animate::Draw_(_replacement_box_width + _replacement_box_base_x - 0x20,
                _replacement_box_height + _replacement_box_base_y - 0x1e,
                MOX::_dull_d_scroll_dn_button_seg);
        }

        video::Clip_Off_();
        graphics::Reset_Window_();

        _wait_ctr++;
        if (_wait_ctr > 2) {
            _wait_ctr = 0;
            MOX::_multi_colored_line_start += static_cast<int8_t>(_incr_value);

            if (MOX::_multi_colored_line_start > 2) {
                _incr_value = -1;
                MOX::_multi_colored_line_start = 3;
            }
            else if (MOX::_multi_colored_line_start <= -1) {
                _incr_value = 1;
                MOX::_multi_colored_line_start = -1;
            }
        }
    }

    void __cdecl Draw_Special_Systems_Scroll_Bar_() {
        int16_t saved_window_status;

        int16_t window_x1 = _replacement_box_base_x + 0x254;
        int16_t window_y1 = _replacement_box_base_y + 0x5C;
        int16_t line_x = _replacement_box_base_x + 0x25A;
        int16_t line_y1 = _replacement_box_base_y + 0x5D;
        uint8_t color = 0x5B; /* 91 */

        mouse::Mouse_Button_();
        fields::Auto_Input_();
        MISC::Save_Window_Status_(&saved_window_status);

        int16_t window_y2 = animate::Get_Height_(MOX::_system_repl_scroll_bar_seg) + window_y1;
        int16_t window_x2 = animate::Get_Width_(MOX::_system_repl_scroll_bar_seg) + window_x1;

        graphics::Set_Window_(window_x1, window_y1, window_x2, window_y2);
        video::Clip_On_();

        int16_t offset = MOX::_scroll_bar->track_len_px - (MOX::_scroll_bar->thumb_height_px / 2) - MOX::_scroll_bar->thumb_center_y;
        int16_t y1 = line_y1 + offset + 1;

        for (int16_t i = 0; i < 9; ++i) {
            int16_t y2 = MOX::_scroll_bar->thumb_height_px + offset + line_y1;
            line::Line_(line_x, y1, line_x, y2, color);

            if ((i % 2) == 0) {
                color--;
            }
            line_x++;
        }

        animate::Draw_(window_x1, window_y1, MOX::_system_repl_scroll_bar_seg);
        MISC::Restore_Window_Status_(saved_window_status);
    }

    void __cdecl System_Added_Cost_And_Space_(int16_t item_slot_idx, int16_t* out_cost, int16_t* out_space) {
        int16_t choice_idx = MOX::_design_choice_items[item_slot_idx];
        s_tech_specials_data* p_special = &TECHDATA::_specials[choice_idx];
        int16_t ship_size = MOX::_design->ship_size;

        int16_t base_cost = p_special->cost_per_ship_type[ship_size];
        uint16_t base_space = p_special->space_per_ship_type[ship_size];

        if (choice_idx == 4) {
            *out_cost = base_cost;
            *out_space = (int16_t)base_space;
            return;
        }

        uint16_t tech_app_id = p_special->tech_app_id;
        int16_t tech_level_cost = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, tech_app_id);
        *out_cost = DESIGN::Cost_Given_M_Level_(base_cost, tech_level_cost);

        int8_t miniaturization_category = DESIGN::Miniaturization_Category_(tech_app_id);
        int16_t tech_level_space = DESIGN::Tech_Level_(MOX::_PLAYER_NUM, tech_app_id);
        *out_space = (int16_t)DESIGN::Space_Given_M_Level_(base_space, tech_level_space, miniaturization_category);
    }
}