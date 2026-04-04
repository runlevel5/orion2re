#include "pch.h"

namespace DESBOX {

uint32_t __cdecl Application_Type_(int16_t app_id) {
    // Accesses the 'type' field (offset 12) of the app_id-th element.
    return TECHDATA::_technology_applications[app_id].type;
}

void __cdecl Update_Weapon_Modification_Data_() {
    uint16_t mods = 0;
    for (int16_t i = 1; i < 15; ++i) {
        if (MOX::_weapon_mod_field_status[i] == 1) {
            mods |= (1 << i);
        }
        DESBOX::_weapon_replacement_mods = mods;
    }
}

void __cdecl Draw_Darkened_Design_Screen_() {
    video::Set_Page_Off_();
    DESIGN::Draw_Design_Screen_();
    RUSS::Mox_Sync_Update_();
    video::Copy_Off_To_Back_();
    graphics::Reset_Window_();
    video::Clip_Off_();
    remap::Update_Glass_Remap_Colors_();
    palette::Set_Palette_Changes_(0, 255 /* 0xff */);
    video::Set_Page_Off_();
    MISC::_Darken_Fill_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */);
    video::Copy_Off_To_Back_();
}

void __cdecl Generic_Replacement_Box_() {
    int16_t field_3;
    int16_t field_1;
    int16_t field_2;

    uint8_t is_first_run = 1;
    uint8_t is_done = 0;
    MOX::_multi_colored_line_start = 0;
    uint8_t draw_bg = 1;
    DESBOX::_incr_value = 1;

    allocate::Push_Block_(MOX::_global_data_seg);
    MOX::_scroll_bar = reinterpret_cast<s_scroll_bar*>(
        allocate::Allocate_Next_Block_(MOX::_global_data_seg, 29)
    );
    memset(MOX::_scroll_bar, 0, 29);

    DESBOX::Draw_Darkened_Design_Screen_();
    fields::Clear_Fields_();
    fields::Deactivate_Help_List_();

    using FnAuto = void(__cdecl*)();
    fields::Assign_Auto_Function_(reinterpret_cast<FnAuto>(std::uintptr_t{0x0048E580}), 1);

    DESBOX::Generic_Replacement_Box_Choices_();
    DESBOX::Generic_Replacement_Box_Dimensions_();

    DESBOX::Add_Generic_Replacement_Fields_(&field_1, &field_2, &field_3);

    int16_t item_idx;
    if (MOX::_design_screen_replacement_slot == -1) {
        item_idx = 0;
        MOX::_design_screen_replacement_item = 0;
        DESBOX::_field_item_chosen = 0;
    } else {
        item_idx = MOX::_design_screen_replacement_item;
        DESBOX::_field_item_chosen = MOX::_design_screen_replacement_item;
    }

    while (true) {
        int16_t input = fields::Get_Input_();
        DESBOX::_field_item_scanned = fields::Scan_Input_();

        if (input < 0) {
            DESBOX::Draw_Generic_Replacement_Box_Help_(input, field_1, field_2, field_3);
        }

        if (input == field_1) {
            item_idx = -1;
            is_done = 1;
        } else if (input == field_2) {
            is_done = 1;
        }

        for (int16_t row = 0; row < MOX::_scroll_bar->total_rows; ++row) {
            if (is_done) {
                break;
            }
            if (MOX::_repl_fields[row] == input) {
                item_idx = row;
                is_done = 1;
            }
        }

        bool do_redraw = false;
        if (is_done || is_first_run) {
            if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                do_redraw = true;
            }
        } else if (draw_bg || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            do_redraw = true;
        }

        if (do_redraw) {
            DESBOX::Redraw_Main_Design_Screen_To_Background_();
            draw_bg = 0;
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }

        if (is_done) {
            break;
        }

        video::Set_Page_Off_();
        DESBOX::Draw_Generic_Replacement_Box_();

        if (is_first_run) {
            is_first_run = 0;
        } else {
            RUSS::Mox_Sync_Update_();
        }
    }

    if (item_idx >= 0) {
        if (MOX::_design_screen_replacement_type == 2) {
            MOX::_design->shield = MOX::_design_choice_items[item_idx];
        } else if (MOX::_design_screen_replacement_type == 8) {
            MOX::_design->computer_type = MOX::_design_choice_items[item_idx];
        }
    }

    allocate::Pop_Block_(MOX::_global_data_seg);
}

void __cdecl Weapons_Replacement_Box_() {
    char msg_arc_rack[5][30];
    char msg_format[200];
    char buf_sprintf[100];
    char desc_buf[1332];

    uint8_t force_redraw_bg = 1;
    uint8_t rack_counts[5] = {2, 5, 10, 15, 20};

    int16_t field_ok;
    int16_t field_cancel;
    int16_t sync_update_flag;
    int16_t replacement_slot;
    int16_t local_done;

    strcpy(msg_arc_rack[0], HAROLD::H_Message_(0x46));
    strcpy(msg_arc_rack[1], HAROLD::H_Message_(0x47));
    strcpy(msg_arc_rack[2], HAROLD::H_Message_(0x48));
    strcpy(msg_arc_rack[3], HAROLD::H_Message_(0x49));
    strcpy(msg_arc_rack[4], s_360_00556164);

    allocate::Push_Block_(MOX::_global_data_seg);
    MOX::_scroll_bar = reinterpret_cast<s_scroll_bar*>(
        allocate::Allocate_Next_Block_(MOX::_global_data_seg, 29)
    );

    memset(MOX::_scroll_bar, 0, 29);
    MOX::_scroll_bar->first_visible_row = 0;
    MOX::_scroll_bar->type = 3;

    _incr_value = 1;
    _wait_ctr = 0;
    _field_item_chosen = -1;
    replacement_slot = MOX::_design_screen_replacement_slot;
    _weapon_replacement_arcs = 0;
    MOX::_multi_colored_line_start = 0;

    video::Set_Page_Off_();
    DESIGN::Draw_Design_Screen_();
    RUSS::Mox_Sync_Update_();
    video::Copy_Off_To_Back_();

    memset(MOX::_design_choice_items, 0, 40 * sizeof(int16_t));

    local_done = 0;
    Weapon_Choices_(MOX::_PLAYER_NUM);

    int16_t total_rows = MOX::_scroll_bar->total_rows;
    if (total_rows > 1) {
        _qsort(reinterpret_cast<void*>(MOX::_weapon_filter), total_rows, 6, qsort_by_research_cost_);
    }

    Space_With_Weapons_();

    MOX::_scroll_bar->visible_rows = 10;
    MOX::_scroll_bar->track_len_px = 120;
    int16_t thumb_height = SCROLL::Get_Vertical_Scroll_Bar_Height_(MOX::_scroll_bar);
    MOX::_scroll_bar->thumb_height_px = thumb_height;
    
    int16_t track_len = MOX::_scroll_bar->track_len_px;
    MOX::_scroll_bar->thumb_center_y = track_len - (thumb_height / 2);
    MOX::_scroll_bar->last_thumb_center_y = -1;

    sync_update_flag = 1;
    MOX::_scroll_bar->row_step = 1;
    MOX::_multi_colored_box_color_start = 0;

    graphics::Reset_Window_();
    video::Clip_Off_();
    remap::Update_Glass_Remap_Colors_();
    palette::Set_Palette_Changes_(0, 255);
    video::Set_Page_Off_();
    MISC::_Darken_Fill_(0, 0, 639, 479);
    video::Copy_Off_To_Back_();
    fields::Clear_Fields_();

    Add_Replacement_Weapon_Fields_(&field_ok, &field_cancel);
    
    using AutoFn = void(__cdecl*)();
    fields::Assign_Auto_Function_(reinterpret_cast<AutoFn>(std::uintptr_t{0x004918B0}), 1);
    
    Get_Weapons_Box_Base_Dimensions_();
    Set_Up_Weapon_Display_(replacement_slot);
    Weapon_Replacement_Help_(MOX::_design_screen_replacement_slot);

    while (local_done == 0) {
        int16_t input = fields::Get_Input_();
        int16_t scanned_input = fields::Scan_Input_();

        if (input == field_ok || input == field_cancel) {
            local_done = 1;
        } else if (input + field_ok == 0) {
            TEXTBOX::Draw_Help_Entry_(688); // 0x2b0
        }

        if (_field_item_chosen > -1) {
            int16_t w_idx = Weapon_Index_(_field_item_chosen);
            int16_t w_type = DESIGN::Weapon_Type_(w_idx);

            for (int16_t i = 0; i < 5; ++i) {
                if (w_type == 0 && MOX::_weapon_arc_field[i] == input) {
                    if (_valid_arcs_racks[i] == 0) {
                        MISC::Get_LBX_Message_(0x4B, msg_format);
                        _sprintf(buf_sprintf, msg_format, msg_arc_rack[i]);
                        GENDRAW::Message_Box_(buf_sprintf);
                    } else {
                        _weapon_replacement_arcs = 1 << i;
                    }
                } else if ((w_type == 1 || w_type == 2) && MOX::_weapon_rack_field[i] == input) {
                    if (_valid_arcs_racks[i] == 0) {
                        MISC::Get_LBX_Message_(0x4C, msg_format);
                        _sprintf(buf_sprintf, msg_format, rack_counts[i]);
                        GENDRAW::Message_Box_(buf_sprintf);
                    } else {
                        _weapon_replacement_arcs = 0x10;
                        _weapon_replacement_rack = i;
                    }
                }
            }
        }

        if (input == MOX::_beam_filter_button) {
            _beam_filter_button_status = (_beam_filter_button_status == 0) ? 1 : 0;
        } else if (input == MOX::_missile_filter_button) {
            _missile_filter_button_status = (_missile_filter_button_status == 0) ? 1 : 0;
        } else if (input == MOX::_bomb_filter_button) {
            _bomb_filter_button_status = (_bomb_filter_button_status == 0) ? 1 : 0;
        } else if (input == MOX::_special_filter_button) {
            _special_filter_button_status = (_special_filter_button_status == 0) ? 1 : 0;
        }

        for (int16_t i = 1; i < 15; ++i) {
            if (MOX::_weapon_mod_fields[i] + input == 0) {
                Print_Weapon_Mod_Help_Description_(i);
            } else if (input == MOX::_weapon_mod_fields[i]) {
                if (_valid_mods[i] == 1) {
                    int16_t status = (MOX::_weapon_mod_field_status[i] == 0) ? 1 : 0;
                    MOX::_weapon_mod_field_status[i] = status;
                    if (i == 1 && status != 0) {
                        MOX::_weapon_mod_field_status[2] = 0;
                    } else if (i == 2 && status != 0) {
                        MOX::_weapon_mod_field_status[1] = 0;
                    }
                }
            }
        }

        int16_t scanned_weapon_index;
        int16_t scanned_item;
        int16_t scan_status = Scan_Replacement_Fields_(scanned_input, input, &scanned_item, &scanned_weapon_index);
        _field_item_scanned = scanned_item;

        if (scan_status == 0) {
            if (scanned_weapon_index == 0) {
                _weapon_replacement_arcs = 0;
                _weapon_replacement_mods = 0;
                _field_item_chosen = 0;
                local_done = 1;
            }
            if (scanned_weapon_index < 0 || _field_item_chosen == scanned_weapon_index) {
                Update_Weapon_Replacement_(&local_done, _field_item_chosen, replacement_slot);
            } else {
                _field_item_chosen = scanned_weapon_index;
                int16_t w_idx = Weapon_Index_(scanned_weapon_index);
                if (w_idx == -1) {
                    local_done = 1;
                } else {
                    int16_t current_weapon_type = MOX::_design->weapon_type[replacement_slot];
                    
                    if (current_weapon_type <= 0 || w_idx != current_weapon_type) {
                        int16_t w_type = DESIGN::Weapon_Type_(w_idx);
                        _weapon_replacement_mods = 0;
                        memset(MOX::_weapon_mod_field_status, 0, 15 * sizeof(int16_t));
                        
                        _weapon_replacement_arcs = 1;
                        if (w_type != 0) {
                            _weapon_replacement_arcs = 0x10;
                        }
                        _weapon_replacement_rack = 1;
                        
                        Update_Weapon_Modification_Data_();
                        Weapon_Replacement_Help_(_field_item_chosen);
                    } else {
                        Set_Up_Weapon_Display_(MOX::_design_screen_replacement_slot);
                    }
                }
            }
        } else if (scan_status == 1) {
            int16_t w_idx = Weapon_Index_(scanned_weapon_index);
            if (w_idx <= 0) {
                TEXTBOX::Draw_Help_Entry_(690); // 0x2b2
            } else {
                uint16_t app_id = TECHDATA::_weapons[w_idx].tech_app_id;
                EVANHELP::Get_Application_Description_(desc_buf, app_id);
                
                const char* weapon_name = TECHDATA::_weapons[w_idx].name;
                TEXTBOX::Text_Box_(weapon_name, desc_buf);
            }
        }

        for (int16_t i = 0; i < 15; ++i) {
            if (MOX::_weapon_mod_fields[i] == input) {
                if (_valid_mods[i] > 0xF8) {
                    Print_Weapon_Modification_Message_(i);
                }
            }
        }

        if (input == _weapon_arc_rack_box_field) {
            if (_field_item_chosen == -1) {
                GENDRAW::Message_Box_((char*)HAROLD::H_Message_(0x4E));
            } else {
                int16_t w_idx = Weapon_Index_(_field_item_chosen);
                int16_t w_type = DESIGN::Weapon_Type_(w_idx);
                if (w_type == 3 || w_type == 4 || w_type == 5) {
                    MISC::Get_LBX_Message_(0x4F, msg_format);
                    const char* weapon_name = TECHDATA::_weapons[w_idx].name;
                    _sprintf(buf_sprintf, msg_format, weapon_name);
                    GENDRAW::Message_Box_(buf_sprintf);
                }
            }
        }

        if (input == MOX::_beam_filter_button || input == MOX::_missile_filter_button || 
            input == MOX::_bomb_filter_button || input == MOX::_special_filter_button) {
            
            int16_t prev_filter = -1;
            int16_t current_total_rows = MOX::_scroll_bar->total_rows;
            
            if (_field_item_chosen > -1) {
                prev_filter = *(reinterpret_cast<int16_t*>(
                    reinterpret_cast<uint8_t*>(MOX::_weapon_filter) + _field_item_chosen * 6
                ));
            }
            
            Weapon_Choices_(MOX::_PLAYER_NUM);
            
            int16_t new_total_rows = MOX::_scroll_bar->total_rows;
            if (new_total_rows > 0) {
                _qsort(reinterpret_cast<void*>(MOX::_weapon_filter), new_total_rows, 6, qsort_by_research_cost_);
                
                if (new_total_rows < 10) {
                    MOX::_scroll_bar->first_visible_row = 0;
                } else {
                    int16_t current_first = MOX::_scroll_bar->first_visible_row;
                    if (new_total_rows < current_first + 10) {
                        int16_t first = new_total_rows - 10;
                        if (first < 0) {
                            first = 0;
                        }
                        MOX::_scroll_bar->first_visible_row = first;
                    }
                }
            }
            
            _field_item_chosen = Update_Field_Item_Chosen_(prev_filter);
            
            if (new_total_rows != current_total_rows) {
                Get_Weapons_Box_Base_Dimensions_();
                force_redraw_bg = 1;
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
            Update_Weapon_Replacement_(&local_done, _field_item_chosen, replacement_slot);
        }

        if (local_done == 0) {
            Update_Weapon_Modification_Data_();
            fields::Clear_Fields_();
            Add_Replacement_Weapon_Fields_(&field_ok, &field_cancel);

            if (local_done == 0 && sync_update_flag == 0) {
                if (force_redraw_bg != 0) {
                    Redraw_Main_Design_Screen_To_Background_();
                    force_redraw_bg = 0;
                }
            }

            if (local_done == 0) {
                video::Set_Page_Off_();
                Draw_Weapons_Replacement_Box_();
                if (sync_update_flag != 0) {
                    sync_update_flag = 0;
                } else {
                    RUSS::Mox_Sync_Update_();
                }
            }
        }
    }

    MOX::_scroll_bar = nullptr;
    allocate::Pop_Block_(MOX::_global_data_seg);
    fields::Deactivate_Auto_Function_();
    fields::Clear_Fields_();
    MOX::_current_screen = 3;
}

}