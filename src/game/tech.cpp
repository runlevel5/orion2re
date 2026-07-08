#include "pch.h"

namespace {
    /** Draws one application name inside the grouped technology list popup. */
    void __cdecl Draw_Tech_List_App_(int16_t x, int16_t y, uint8_t tech_app_id) {
        char buffer[80];

        buffer[0] = '^';
        strlcpy(buffer + 1, TECH::Technology_Applications_Name_(tech_app_id, MOX::_PLAYER_NUM), sizeof(buffer) - 1);
        BILL::Squeeze_Print_(x, y, buffer, 0xD5);
    }
}

namespace TECH {
    char _hyper_field_name_buffer[64];

    uint8_t _saved_hyper_advanced_tech[8];

    int32_t _tech_select_pos[8] = {
        0x001E00B0, 0x001F0193, 0x008700B0, 0x00870193,
        0x00F000B0, 0x00F00193, 0x015B00B0, 0x015B0193,
    };

    int32_t _tech_change_pos[8] = {
        0x001E005F, 0x001F0142, 0x0087005F, 0x00870142,
        0x00F0005F, 0x00F00142, 0x015B005F, 0x015B0142,
    };

    int16_t _tech_field_y1[4] = {0, 34, 49, 64};

    int16_t _tech_field_y2[4] = {33, 48, 63, 78};

    int16_t _app_name_y[8] = {19, 34, 49, 64, 79, 0, 0, 0};

    int16_t _list_window_x_offsets[8] = {205, 6, 205, 6, 205, 6, 205, 6};

    int16_t _tech_button_pos[16] = {
        21, 30, 248, 31,
        21, 135, 248, 135,
        21, 240, 248, 240,
        21, 347, 248, 347,
    };

    uint8_t _entry_to_group[8] = {4, 2, 6, 8, 7, 1, 3, 5};

    s_colors _tech_select_colors[5] = {
        {{241, 244, 244, 244, 243, 243, 244, 245}},
        {{241, 247, 247, 247, 245, 246, 247, 248}},
        {{241, 248, 248, 248, 247, 248, 248, 249}},
        {{241, 245, 245, 245, 244, 245, 245, 245}},
        {{241, 244, 244, 244, 243, 243, 244, 244}},
    };

    s_colors _tech_change_colors[5] = {
        {{225, 228, 228, 228, 227, 227, 228, 229}},
        {{225, 231, 231, 231, 229, 230, 231, 232}},
        {{225, 232, 232, 232, 231, 232, 232, 232}},
        {{225, 229, 229, 229, 228, 229, 229, 229}},
        {{225, 228, 228, 228, 227, 227, 228, 228}},
    };

    int16_t _g_scrn_y;
    int16_t _g_scrn_x;
    int16_t _g_first_tech_field;
    s_fieldsav_data _tech_field_save;
    s_list_item* _g_selected_list_entry_p;
    int16_t _g_last_tech_field;
    uint8_t _g_cursor_box_color;
    uint8_t _g_changing_tech;
    s_list_item_research* _g_selected_entry_p;
    char _tech_group_names[10][20];
    s_list_item_research* _g_entries_p;
    s_colors _tech_color[5];

    s_list_state _g_tech_list_header = {
        0,
        73,
        223,
        424,
        _app_name_y,
        _tech_field_y1,
        _tech_field_y2,
        8,
        0,
        nullptr,
        nullptr,
        0,
        0,
        4,
        3,
        (void*)Draw_Tech_List_App_,
        10,
        0xFF,
        &_tech_color[1],
        {&_tech_color[4], &_tech_color[3], &_tech_color[0], &_tech_color[3]},
        &_tech_color[2],
    };

    void __cdecl Tech_Select_() {
        void* animation_buffer;

        palette::Save_Palette_();
        animation_buffer = allocate::Allocate_First_Block_(MOX::_screen_seg, 0x7C830);
        file_ani::Set_File_Animation_Buffer_(animation_buffer);
        SCIENCE::Show_Off_Researched_Tech_(MOX::_PLAYER_NUM);
        TECH::_Tech_Select_(0);
        ERIC::Fast_Fade_Out_();
        TECH::Reset_Screen_();
    }

    void __cdecl _Tech_Select_(uint8_t changing_tech) {
        int16_t extra_fields_cnt;
        int16_t first_btn_field;
        int16_t last_btn_field;
        int16_t accept_btn_id;
        int i;
        int16_t player_race;
        char science_race_lbx_name[64];
        MOX2::action_func_t auto_func;
        s_animation_header* tech_select_lbx_images[9];
        s_animation_header* entry_buttons[8];
        s_animation_header* science_lbx = nullptr;
        uint8_t saved_current_field;
        int16_t input_val;

        RUSS::Mox_Update_();
        fields::Clear_Fields_();
        video::Set_Refresh_Stencil_();
        MOX2::Get_Auto_Function_(&auto_func);
        allocate::Push_Block_(MOX::_screen_seg);
        Save_Hyper_Tech_Values_(MOX::_PLAYER_NUM);
        Promote_Hyper_Field_Values_(MOX::_PLAYER_NUM);
        fields::Disable_Cancel_();

        MOX::_cur_plyr_ptr = &MOX::_player[MOX::_PLAYER_NUM];
        _g_changing_tech = changing_tech;

        saved_current_field = MOX::_cur_plyr_ptr->current_research_field;

        s_list_item_research* entries = (s_list_item_research*)(
            allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(s_list_item_research) * 8)
            );
        MOX::_tech_desc_seg = allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x57C);

        RUSS::Mox_Update_();

        if (changing_tech == 1) {
            _g_scrn_x = 0x50;
            _g_scrn_y = 0;
            for (i = 0; i < 5; ++i) {
                _tech_color[i] = _tech_change_colors[i];
            }
            _g_cursor_box_color = 0xEF;

            tech_select_lbx_images[0] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0xE, MOX::_screen_seg);
            RUSS::Mox_Update_();
            entry_buttons[0] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0xF, MOX::_screen_seg);
            entry_buttons[1] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x10, MOX::_screen_seg);
            entry_buttons[2] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x11, MOX::_screen_seg);
            entry_buttons[3] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x12, MOX::_screen_seg);
            RUSS::Mox_Update_();
            entry_buttons[4] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x13, MOX::_screen_seg);
            entry_buttons[5] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x14, MOX::_screen_seg);
            entry_buttons[6] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x15, MOX::_screen_seg);
            RUSS::Mox_Update_();
            entry_buttons[7] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x16, MOX::_screen_seg);

            tech_select_lbx_images[8] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x1B,
                MOX::_screen_seg);
        }
        else {
            _g_scrn_x = 0xA1;
            _g_scrn_y = 0;
            for (i = 0; i < 5; ++i) {
                _tech_color[i] = _tech_select_colors[i];
            }
            _g_cursor_box_color = 0xFF;

            tech_select_lbx_images[0] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0, MOX::_screen_seg);
            RUSS::Mox_Update_();
            entry_buttons[0] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 1, MOX::_screen_seg);
            entry_buttons[1] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 2, MOX::_screen_seg);
            entry_buttons[2] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 3, MOX::_screen_seg);
            RUSS::Mox_Update_();
            entry_buttons[3] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 4, MOX::_screen_seg);
            entry_buttons[4] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 5, MOX::_screen_seg);
            entry_buttons[5] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 6, MOX::_screen_seg);
            RUSS::Mox_Update_();
            entry_buttons[6] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 7, MOX::_screen_seg);
            entry_buttons[7] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 8, MOX::_screen_seg);

            tech_select_lbx_images[8] = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0xD, MOX::_screen_seg);
        }

        RUSS::Mox_Update_();

        _g_tech_list_header.marker_color = _g_cursor_box_color;

        if (changing_tech == 1) {
            accept_btn_id = fields::Add_Button_Field_(
                _g_scrn_x + 0xBD, _g_scrn_y + 0x1C4, "", tech_select_lbx_images[8], "\x1B", '('
            );
            MOX::_cur_plyr_ptr->current_research_field = 0;
            Init_Entry_Positions_(entries);
            extra_fields_cnt = Init_Entry_Data_(entries, MOX::_cur_plyr_ptr->research_accumulated);
            MOX::_cur_plyr_ptr->current_research_field = static_cast<char>(saved_current_field);
        }
        else {
            accept_btn_id = -1;
            Init_Entry_Positions_(entries);
            if (static_cast<uint8_t>(MOX::_cur_plyr_ptr->current_research_field) >= TECH_FIELD_BIOLOGY) {
                MOX::_cur_plyr_ptr->current_research_field = 0;
            }
            extra_fields_cnt = Init_Entry_Data_(entries, 0);
        }

        _g_first_tech_field = fields::Get_Field_Counter_();

        for (i = 0; i < 8; ++i) {
            _g_last_tech_field = fields::Add_Hidden_Field_(
                entries[i].x - 2, entries[i].y + 18,
                entries[i].x + 215, entries[i].y + 99,
                "", 0
            );
        }

        RUSS::Mox_Update_();

        if (extra_fields_cnt == 0) {
            allocate::Pop_Block_(MOX::_screen_seg);
            MOX2::Put_Auto_Function_(&auto_func);
            fields::Clear_Fields_();
            Restore_Hyper_Tech_Values_(MOX::_PLAYER_NUM);
            fields::Deactivate_Help_List_();
            return;
        }

        first_btn_field = fields::Get_Field_Counter_();
        Setup_Entry_Buttons_(entries, entry_buttons);

        last_btn_field = fields::Get_Field_Counter_() - 1;

        fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, "", 0);

        _g_entries_p = entries;

        player_race = MOX::_player[MOX::_PLAYER_NUM].race;
        snprintf(science_race_lbx_name, sizeof(science_race_lbx_name), "SR_R%x_SC.LBX", player_race);

        RUSS::Mox_Update_();

        if (changing_tech == 0) {
            file_ani::Open_File_Animation_(science_race_lbx_name, 0);
            science_lbx = (s_animation_header*)farload::Far_Reload_Next_("SCIENCE.LBX", 0, MOX::_screen_seg);
        }

        while (true) {
            RUSS::Mox_Update_();

            if (changing_tech == 0) {
                video::Set_Page_Off_();
                graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);

                if (MOX::_player[MOX::_PLAYER_NUM].race == 7) {
                    file_ani::Set_File_Animation_Frame_(0x11);
                }
                else {
                    file_ani::Set_File_Animation_Frame_(0);
                }
                file_ani::Draw_File_Animation_();

                graphics::Fill_(_g_scrn_x + 4, _g_scrn_y + 4, _g_scrn_x + 0x1D7, _g_scrn_y + 0x1D8, 0);
                animate::Draw_Palette_(tech_select_lbx_images[0]);
                palette::Refresh_Palette_();
                animate::Draw_Palette_(science_lbx);
                palette::Refresh_Palette_();
            }

            fields::Set_Input_Delay_(5);

            if (changing_tech == 1) {
                BILLHELP::Technology_Change_Help_();
            }
            else {
                BILLHELP::Technology_Select_Help_();
            }

            RUSS::Mox_Update_();
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;

            fields::Assign_Auto_Function_(Draw_Tech_Select_, 4);

            graphics::Fill_(_g_scrn_x + 4, _g_scrn_y + 4, _g_scrn_x + 0x1D7, _g_scrn_y + 0x1D8, 0);
            animate::Draw_(_g_scrn_x, _g_scrn_y, tech_select_lbx_images[0]);
            palette::Refresh_Palette_();
            Display_All_Entry_Text_(_g_entries_p);

            RUSS::Mox_Update_();
            video::Copy_Off_To_Back_();
            Draw_Tech_Select_();

            if (changing_tech == 0) {
                JIM::Bill_Picture_To_Mouse_(tech_select_lbx_images[8]);
            }

            ERIC::Fast_Fade_In_();
            palette::Reset_Cycle_Palette_Color_();
            fields::Clear_Mouse_Buffer_();

            RUSS::Mox_Update_();

            bool redraw = false;

            while (!redraw) {
                if (changing_tech == 0) {
                    JIM::Play_Streaming_Music_(0x75, -2, 1);
                }

                input_val = fields::Get_Input_();

                if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                    redraw = true;
                    break;
                }

                if (input_val < 0) {
                    input_val = -input_val;
                    if (input_val < first_btn_field || input_val > last_btn_field) {
                        if (Set_Selected_Entry_(entries, input_val) != nullptr) {
                            _g_selected_entry_p = Get_Selected_Entry_(_g_entries_p);
                            int16_t app_id = _g_selected_entry_p->app_ids[_g_selected_entry_p->selected_app_index];
                            if (app_id != 0) {
                                Display_Selected_Entry_Text_(_g_selected_entry_p);
                                Draw_Application_Description_(0x119, app_id);
                                redraw = true;
                                break;
                            }
                        }
                        input_val = 0;
                    }
                }

                if (input_val <= 0) {
                    timer::Mark_Time_();
                    Draw_Tech_Select_();
                    timer::Release_Time_(2);
                    RUSS::Mox_Sync_Update_();
                }
                else {
                    if (input_val == accept_btn_id) {
                        allocate::Pop_Block_(MOX::_screen_seg);
                        MOX2::Put_Auto_Function_(&auto_func);
                        fields::Clear_Fields_();
                        Restore_Hyper_Tech_Values_(MOX::_PLAYER_NUM);
                        fields::Deactivate_Help_List_();
                        return;
                    }
                    else if (input_val < first_btn_field) {
                        int16_t blink_cnt = 3;
                        _g_selected_entry_p = Get_Selected_Entry_(entries);
                        do {
                            timer::Mark_Time_();
                            if ((blink_cnt & 1) == 0) {
                                Display_Selected_Entry_Text_(_g_selected_entry_p);
                            }
                            --blink_cnt;
                            RUSS::Mox_Sync_Update_();
                            timer::Release_Time_(4);
                        } while (blink_cnt != 0);

                        MOX::_cur_plyr_ptr->current_research_field = static_cast<char>(_g_selected_entry_p->tech_field_id);
                        MOX::_cur_plyr_ptr->current_research_application =
                            static_cast<char>(_g_selected_entry_p->app_ids[_g_selected_entry_p->selected_app_index]);
                        
                        allocate::Pop_Block_(MOX::_screen_seg);
                        MOX2::Put_Auto_Function_(&auto_func);
                        fields::Clear_Fields_();
                        Restore_Hyper_Tech_Values_(MOX::_PLAYER_NUM);
                        fields::Deactivate_Help_List_();
                        return;
                    }
                    else if (input_val <= last_btn_field) {
                        int16_t btn_idx = input_val - first_btn_field;
                        s_list_item_research* clicked_entry = &entries[btn_idx];
                        animate::Set_Animation_Frame_(clicked_entry->button_image, 1);
                        animate::Draw_(
                            _tech_button_pos[btn_idx * 2] + _g_scrn_x,
                            _tech_button_pos[btn_idx * 2 + 1] + _g_scrn_y,
                            clicked_entry->button_image
                        );
                        video::Copy_Off_To_Back_();
                        clicked_entry->button_value = 0;
                        _Tech_List_(clicked_entry, changing_tech);
                        redraw = true;
                        break;
                    }
                }
            }
        }
    }

    void __cdecl Restore_Hyper_Tech_Values_(int16_t player_idx) {
        memcpy(
            MOX::_player[player_idx].hyper_advanced_tech,
            TECH::_saved_hyper_advanced_tech,
            sizeof(MOX::_player[player_idx].hyper_advanced_tech)
        );
    }

    void __cdecl Save_Hyper_Tech_Values_(int16_t player_idx) {
        memcpy(
            TECH::_saved_hyper_advanced_tech,
            MOX::_player[player_idx].hyper_advanced_tech,
            sizeof(TECH::_saved_hyper_advanced_tech)
        );
    }

    void __cdecl Init_Entry_Positions_(s_list_item_research* research_list) {
        for (int i = 0; i < 8; ++i) {
            s_list_item_research& entry = research_list[i];

            entry.id = (int16_t)TECH::_entry_to_group[i];

            int32_t pos;
            if (TECH::_g_changing_tech) {
                pos = TECH::_tech_change_pos[i];
            }
            else {
                pos = TECH::_tech_select_pos[i];
            }

            // Unpack HIWORD/LOWORD into y and x coordinates
            entry.x = (int16_t)pos;
            entry.y = (int16_t)(pos >> 16);

            entry.field_name[0] = '\0';
            entry.tech_field_id = 0;
            entry.current_app_index = 0;
            entry.selected_app_index = 0;
            entry.visible_app_count = 0;
            entry.label_x = entry.x + 10;
            entry.list_win_x = TECH::_list_window_x_offsets[i] + TECH::_g_scrn_x;

            for (int j = 0; j < 4; ++j) {
                entry.app_label_y[j] = TECH::_app_name_y[j] + entry.y + 21;
                entry.app_names[j][0] = '\0';
            }
        }
    }

    void Setup_Entry_Buttons_(s_list_item_research* items, s_animation_header** images) {
        for (int i = 0; i < 8; ++i) {
            if (items[i].tech_field_id != 0) {
                items[i].button_value = 0;
                items[i].button_field_id = fields::Add_Radio_Button_Field_(
                    TECH::_tech_button_pos[i * 2] + TECH::_g_scrn_x,
                    TECH::_tech_button_pos[i * 2 + 1] + TECH::_g_scrn_y,
                    "",
                    images[i],
                    &items[i].button_value,
                    "",
                    40
                );
                items[i].button_image = images[i];
            }
        }
    }

    void __cdecl Draw_Tech_Select_() {
        int16_t input_field = fields::Scan_Input_();

        if (input_field >= _g_first_tech_field && input_field <= _g_last_tech_field) {
            int32_t index = input_field - _g_first_tech_field;
            _g_selected_entry_p = &_g_entries_p[index];
            input_field = _g_selected_entry_p->app_click_field_ids[_g_selected_entry_p->visible_app_count - 1];
        }

        if (input_field > 0) {
            Set_Selected_Entry_(_g_entries_p, input_field);
        }

        _g_selected_entry_p = Get_Selected_Entry_(_g_entries_p);
        Display_Selected_Entry_Text_(_g_selected_entry_p);
        Draw_Little_Arrow_(_g_selected_entry_p);

        palette::Cycle_Palette_Color_(_g_cursor_box_color, 8, 32, 0, 40, 63, 8, 8);
    }

    void __cdecl Display_All_Entry_Text_(s_list_item_research* items) {
        s_list_item_research* current_item = items;
        s_list_item_research* end_item = items + 8;

        do {
            TECH::Display_Entry_Text_(current_item);
            current_item++;
        } while (current_item != end_item);
    }

    s_list_item_research* __cdecl Set_Selected_Entry_(s_list_item_research* items, int16_t target_field_id) {
        bool is_searching = true;
        s_list_item_research* result = nullptr;

        for (int i = 0; i < 8; ++i) {
            items[i].current_app_index = 0;

            if (is_searching && items[i].tech_field_id != 0 && items[i].visible_app_count > 0) {
                for (int16_t j = 0; j < items[i].visible_app_count; ++j) {
                    if (items[i].app_click_field_ids[j] == target_field_id) {
                        is_searching = false;
                        result = &items[i];
                        items[i].current_app_index = 1;
                        items[i].selected_app_index = j;
                    }
                }
            }
        }
        return result;
    }

    s_list_item_research* __cdecl Get_Selected_Entry_(s_list_item_research* entries) {
        s_list_item_research* current = entries;
        s_list_item_research* end = &entries[8];

        do {
            if (current->current_app_index != 0) {
                return current;
            }
            current++;
        } while (current != end);

        return nullptr;
    }

    int16_t __cdecl Init_Entry_Data_(s_list_item_research* entries, int32_t research_cost_offset) {
        char no_available_app_text[80];
        JIM::Get_Text_Message_("billtext.lbx", 62, no_available_app_text, sizeof(no_available_app_text));
        int16_t valid_count = 0;

        s_list_item_research* item = entries;
        for (int i = 0; i < 8; ++i, ++item) {
            int16_t tech_field_id = MOX::_first_field_in_group[item->id];

            if (tech_field_id != 0) {
                while (true) {
                    if (MOX::_cur_plyr_ptr->tech_fields[tech_field_id] == 2 &&
                        MOX::_cur_plyr_ptr->current_research_field != tech_field_id) {
                        break;
                    }
                    tech_field_id = TECHDATA::_technology_fields[tech_field_id].next_field_id;
                    if (tech_field_id == 0) {
                        break;
                    }
                }
            }

            if (tech_field_id == 0) continue;

            if (tech_field_id < TECH_FIELD_BIOLOGY) {
                int visible_app_count = 0;
                int16_t* techs = TECHDATA::_technology_fields[tech_field_id].tech;
                for (int app_index = 0; app_index < 4; ++app_index) {
                    int16_t app_id = techs[app_index];
                    if (MOX::_cur_plyr_ptr->tech_applications[app_id] == TECH_RESEARCH_STATUS_AVAILABLE) {
                        strlcpy(item->app_names[visible_app_count], Technology_Applications_Name_(app_id, MOX::_PLAYER_NUM), sizeof(item->app_names[visible_app_count]));
                        item->app_ids[visible_app_count] = app_id;
                        item->app_click_field_ids[visible_app_count] = fields::Add_Hidden_Field_(
                            item->x,
                            _tech_field_y1[visible_app_count] + item->y + 0x15,
                            item->x + 0xDA,
                            _tech_field_y2[visible_app_count] + item->y + 0x15,
                            "",
                            0
                        );
                        ++visible_app_count;
                    }
                }

                if (visible_app_count == 0) {
                    strlcpy(item->app_names[0], no_available_app_text, sizeof(item->app_names[0]));
                    item->app_ids[0] = 0;
                    item->app_click_field_ids[0] = fields::Add_Hidden_Field_(
                        item->x,
                        _tech_field_y1[0] + item->y + 0x15,
                        item->x + 0xDA,
                        _tech_field_y2[0] + item->y + 0x15,
                        "",
                        0
                    );
                    ++visible_app_count;
                }

                item->visible_app_count = visible_app_count;
            }
            else {
                uint16_t app_id = Get_Hyper_Tech_App_ID_(tech_field_id);
                strlcpy(item->app_names[0], Technology_Applications_Name_(app_id, MOX::_PLAYER_NUM), sizeof(item->app_names[0]));
                item->app_ids[0] = Get_Hyper_Tech_App_ID_(tech_field_id);
                item->app_click_field_ids[0] = fields::Add_Hidden_Field_(
                    item->x,
                    _tech_field_y1[0] + item->y + 0x15,
                    item->x + 0xDA,
                    _tech_field_y2[0] + item->y + 0x15,
                    "",
                    0
                );
                item->visible_app_count = 1;
            }

            item->tech_field_id = tech_field_id;

            int32_t cost = COLCALC::Player_Research_Cost_(MOX::_cur_plyr_ptr, tech_field_id) - research_cost_offset;
            if (cost < 0) {
                cost = 0;
            }
            item->research_cost_remaining = cost;

            char fmt[8];
            const char* fmt_ptr;
            switch (MOX::_settings.language) {
            case 0: fmt_ptr = "%i RP"; break;
            case 1: fmt_ptr = "%i FP"; break;
            case 3: fmt_ptr = "%i RP"; break;
            case 4: fmt_ptr = "%i PR"; break;
            case 2:
            default: fmt_ptr = "%i RP"; break;
            }
            strlcpy(fmt, fmt_ptr, sizeof(fmt));
            snprintf(item->buf, sizeof(item->buf), fmt, item->research_cost_remaining);

            strlcpy(item->field_name, Technology_Fields_Name_(tech_field_id), sizeof(item->field_name));

            ++valid_count;
        }
        return valid_count;
    }

    void __cdecl Display_Entry_Text_(s_list_item_research* item) {
        if (item == nullptr || item->tech_field_id == 0) {
            return;
        }

        bool is_current_field = (static_cast<uint8_t>(MOX::_cur_plyr_ptr->current_research_field) == item->tech_field_id);

        s_colors* color;
        if (is_current_field) {
            color = &TECH::_tech_color[2];
        }
        else {
            color = &TECH::_tech_color[0];
        }

        fonts::Set_Font_Style_(4, color);
        BILL::Squeeze_Print_(item->x, item->y + 21, item->field_name, 218);

        fonts::Set_Font_Style_(3, color);
        fonts::Print_Right_(item->x + 212, item->y + 2, item->buf);

        bool is_creative = false;
        if (is_current_field) {
            if (MOX::_cur_plyr_ptr->traits[TRAIT_CREATIVE] == 1) {
                is_creative = true;
            }
            else {
                int16_t tid = item->tech_field_id;
                if (tid == 0x37 || tid == 0x39 || tid == 0x1D || tid == 0x16 || tid == 0x1C || tid == 0x17) {
                    is_creative = true;
                }
            }
        }

        int16_t i = 0;
        do {
            s_colors* app_color = &TECH::_tech_color[0];

            if (is_current_field) {
                if (is_creative) {
                    app_color = &TECH::_tech_color[2];
                }
                else {
                    uint8_t cur_app = static_cast<uint8_t>(MOX::_cur_plyr_ptr->current_research_application);
                    if (strcasecmp(item->app_names[i], TECHDATA::_technology_applications[cur_app].name) == 0) {
                        app_color = &TECH::_tech_color[2];
                    }
                }
            }

            fonts::Set_Font_Style_(3, app_color);
            BILL::Squeeze_Print_(item->label_x, item->app_label_y[i], item->app_names[i], 208);

            i++;
        } while (i < item->visible_app_count);
    }

    void __cdecl Draw_Little_Arrow_(s_list_item_research* entry) {
        if (entry == nullptr) {
            return;
        }

        if (entry->current_app_index == 0) {
            return;
        }

        int16_t tech_id = entry->tech_field_id;
        if (tech_id == 0) {
            return;
        }

        int16_t i = entry->selected_app_index;
        int16_t upper_bound = i + 1;

        if (MOX::_player[MOX::_PLAYER_NUM].traits[TRAIT_CREATIVE] == 1 ||
            tech_id == 0x37 ||
            tech_id == 0x39 ||
            tech_id == 0x1D ||
            tech_id == 0x16 ||
            tech_id == 0x1C ||
            tech_id == 0x17)
        {
            i = 0;
            upper_bound = entry->visible_app_count;
        }

        for (; i < upper_bound; i++) {
            uint8_t color = _g_cursor_box_color;
            int16_t label_x = entry->label_x;
            int16_t y = entry->app_label_y[i] + 5;
            int16_t x1 = label_x - 9;
            int16_t x2 = label_x - 2;
            int16_t x3 = label_x - 5;
            int16_t y0 = entry->app_label_y[0] - 3;

            line::Line_(x1, y0, x1, y, color);
            line::Line_(x1, y, x2, y, color);
            line::Line_(x3, y - 3, x2, y, color);
            line::Line_(x3, y + 3, x2, y, color);
        }
    }

    void __cdecl Draw_Application_Description_(int32_t unused_x, uint16_t app_idx) {
        char text_buffer[80];
        char lbx_name[20];
        char cost_str[20];
        char desc_buffer[1420];

        (void)unused_x;

        fields::Deactivate_Auto_Function_();
        allocate::Push_Block_(MOX::_screen_seg);
        fields::Disable_Draw_All_Fields_();

        void* out_buffer = allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x57C);

        TEXTBOX::Get_Help_Lbx_Name_(lbx_name, sizeof(lbx_name));
        s_help_record* loaded_data = (s_help_record*)farload::Far_Reload_Data_(lbx_name, 0, out_buffer, app_idx, 1, sizeof(s_help_record));

        int32_t cost = New_Get_Tech_Cost_(app_idx, 1, MOX::_PLAYER_NUM);
        JIM::Get_Text_Message_("billtext.lbx", 0x3D, text_buffer, sizeof(text_buffer));

        snprintf(cost_str, sizeof(cost_str), "%i", cost);
        strlcat(text_buffer, cost_str, sizeof(text_buffer));

        const char* suffix;
        switch (MOX::_settings.language) {
        case 0:
            suffix = " RP";
            break;
        case 1:
            suffix = " FP";
            break;
        case 2:
            suffix = " RP";
            break;
        case 3:
            suffix = " RP";
            break;
        case 4:
            suffix = " PR";
            break;
        default:
            suffix = " RP";
            break;
        }
        strlcat(text_buffer, suffix, sizeof(text_buffer));

        /* fmtpara: \aF3. selects font 3, \x1A3 fully justifies the body text,
           then \r starts a new paragraph and \aY+3.\x1A2 centers the cost line. */
        snprintf(
            desc_buffer,
            sizeof(desc_buffer),
            "\aF3.\x1A" "3%s\r\aY+3.\x1A" "2%s",
            loaded_data->body,
            text_buffer
        );
        TEXTBOX::Text_Box_(loaded_data->title, desc_buffer);

        allocate::Pop_Block_(MOX::_screen_seg);
        fields::Activate_Auto_Function_();
        fields::Clear_Mouse_Buffer_();
        fields::Enable_Draw_All_Fields_();
    }

    int16_t __cdecl _Tech_List_(s_list_item_research* entry, uint8_t is_change_mode) {
        char msg_buf[80];
        char title_buf[32];
        s_animation_header* win_bg_1;
        s_animation_header* win_bg_2;
        MOX2::action_func_t auto_funcs[11];
        int16_t clear_field;
        int16_t text_x;
        int16_t fill_x2;
        s_animation_header* bg_capture;
        int16_t win_x;
        int16_t start_field;
        int16_t button_2_id;
        int16_t list_bottom_field;
        int16_t hidden_field_1;
        int16_t button_1_id;
        s_animation_header* pic;
        s_animation_header* pic_00;
        int16_t input;

        RUSS::Mox_Update_();
        JIM::Get_Text_Message_("billtext.lbx", 0x3F, msg_buf, sizeof(msg_buf));
        JIM::Get_Text_Message_("billtext.lbx", 0x40, _tech_group_names[0], sizeof(_tech_group_names[0]));
        JIM::Get_Text_Message_("billtext.lbx", 0x41, _tech_group_names[1], sizeof(_tech_group_names[0]));
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_("billtext.lbx", 0x42, _tech_group_names[2], sizeof(_tech_group_names[0]));
        JIM::Get_Text_Message_("billtext.lbx", 0x43, _tech_group_names[3], sizeof(_tech_group_names[0]));
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_("billtext.lbx", 0x44, _tech_group_names[4], sizeof(_tech_group_names[0]));
        JIM::Get_Text_Message_("billtext.lbx", 0x45, _tech_group_names[5], sizeof(_tech_group_names[0]));
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_("billtext.lbx", 0x46, _tech_group_names[6], sizeof(_tech_group_names[0]));
        JIM::Get_Text_Message_("billtext.lbx", 0x47, _tech_group_names[7], sizeof(_tech_group_names[0]));
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_("billtext.lbx", 0x48, _tech_group_names[8], sizeof(_tech_group_names[0]));
        JIM::Get_Text_Message_("billtext.lbx", 0x49, _tech_group_names[9], sizeof(_tech_group_names[0]));
        RUSS::Mox_Update_();

        MOX2::Get_Auto_Function_(auto_funcs);
        allocate::Push_Block_(MOX::_screen_seg);
        FIELDSAV::Save_Field_Stats_(&_tech_field_save);
        fields::Disable_Cancel_();

        if (is_change_mode == 1) {
            win_bg_1 = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x17, MOX::_screen_seg);
            win_bg_2 = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x18, MOX::_screen_seg);
            RUSS::Mox_Update_();
            pic = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x19, MOX::_screen_seg);
            pic_00 = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x1A, MOX::_screen_seg);
        }
        else {
            win_bg_1 = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x09, MOX::_screen_seg);
            win_bg_2 = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x0A, MOX::_screen_seg);
            RUSS::Mox_Update_();
            pic = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x0B, MOX::_screen_seg);
            pic_00 = (s_animation_header*)farload::Far_Reload_Next_("TECHSEL.LBX", 0x0C, MOX::_screen_seg);
        }
        RUSS::Mox_Update_();

        _g_tech_list_header.items = (s_list_item*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x33E0);
        win_x = entry->list_win_x;
        _g_tech_list_header.x = win_x + 0x10;

        uint32_t bg_size = bitmap::Get_Bitmap_Size_(0x1B5, 0x190);
        bg_capture = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bg_size);
        bitmap::Create_Blank_Bitmap_(0x1B5, 0x190, bg_capture);
        bitmap::Screen_Bitmap_Capture_(0x54, 0x28, 0x207, 0x1B7, bg_capture);

        if ((win_x - _g_scrn_x) <= 6) {
            if (is_change_mode != 0) {
                BILLHELP::Tech_Change_List_Left_Help_();
            }
            else {
                BILLHELP::Tech_Select_List_Left_Help_();
            }
        }
        else {
            if (is_change_mode != 0) {
                BILLHELP::Tech_Change_List_Right_Help_();
            }
            else {
                BILLHELP::Tech_Select_List_Right_Help_();
            }
        }

        Get_Group_List_(&_g_tech_list_header, entry->id);
        LIST::Init_List_Data_(&_g_tech_list_header);

        button_1_id = fields::Add_Button_Field_(win_x + 0xF7, 0x4D, "", pic, "", 0);
        button_2_id = fields::Add_Button_Field_(win_x + 0xF8, 0x18E, "", pic_00, "", 0);
        start_field = fields::Get_Field_Counter_();
        fields::Assign_Auto_Function_(Draw_Tech_List_, 4);

        fill_x2 = win_x + 0x10B;
        text_x = win_x + 0x86;
        clear_field = start_field - 1;

        while (true) {
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            graphics::Fill_(0x54, 0x28, 0x207, 0x1B7, 0);
            animate::Draw_(0x54, 0x28, bg_capture);
            graphics::Fill_(win_x, 0x1F, fill_x2, 0x1C1, 0);
            animate::Draw_(win_x, 0x1F, win_bg_1);

            strlcpy(title_buf, _tech_group_names[entry->id], sizeof(title_buf));
            strlcat(title_buf, msg_buf, sizeof(title_buf));

            fonts::Set_Font_Style_(4, _tech_color);
            fonts::Print_Centered_(text_x, 0x2F, title_buf);

            LIST::Display_List_Page_(&_g_tech_list_header);
            video::Copy_Off_To_Back_();
            LIST::Set_List_Up_Down_Field_Drawing_(&_g_tech_list_header, button_1_id, button_2_id);

            RUSS::Mox_Sync_Update_();
            timer::Mark_Time_();

            fields::Clear_Fields_Above_(clear_field);
            list_bottom_field = LIST::Add_Fields_To_List_Page_(&_g_tech_list_header);

            hidden_field_1 = fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, "\x1B", 0);
            fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, "", 0);

            timer::Release_Time_(4);

            while (true) {
                input = fields::Get_Input_();
                if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                    break;
                }

                if (input > 0) {
                    if (input == button_2_id) {
                        if (LIST::Next_List_Page_(&_g_tech_list_header) != 0) {
                            break;
                        }
                        continue;
                    }
                    if (input == button_1_id) {
                        if (LIST::Previous_List_Page_(&_g_tech_list_header) != 0) {
                            break;
                        }
                        continue;
                    }
                }
                else if (input == 0 || input == -hidden_field_1) {
                    timer::Mark_Time_();
                    Draw_Tech_List_();
                    RUSS::Mox_Sync_Update_();
                    timer::Release_Time_(4);
                    continue;
                }
                else {
                    int16_t abs_input = -input;
                    if (abs_input >= start_field && abs_input <= list_bottom_field) {
                        _g_selected_list_entry_p = LIST::Get_Selected_List_Entry_(&_g_tech_list_header);
                        if (_g_selected_list_entry_p != nullptr) {
                            uint8_t app_id = _g_selected_list_entry_p->tech_app_ids[_g_selected_list_entry_p->current_app_index];
                            if (app_id != 0) {
                                LIST::Display_Selected_List_Text_(&_g_tech_list_header, _g_selected_list_entry_p);
                                Draw_Application_Description_(0x119, app_id);
                                break;
                            }
                        }
                        continue;
                    }
                }

                int16_t match_val = (input > 0) ? input : -input;
                if (match_val == hidden_field_1) {
                    animate::Draw_(win_x + 0xBB, 0x1B2, win_bg_2);
                    timer::Mark_Time_();
                    RUSS::Mox_Sync_Update_();
                    timer::Release_Time_(4);
                    MOX2::Put_Auto_Function_(auto_funcs);
                    FIELDSAV::Restore_Field_Stats_(&_tech_field_save);
                    allocate::Pop_Block_(MOX::_screen_seg);
                    fields::Deactivate_Help_List_();
                    return input;
                }
            }
        }
    }

    /** Builds a star-and-roman-numeral planet name for technology text. */
    void __cdecl Do_Get_Planet_Name_(int16_t planet_idx, char* out_str, uint8_t localized_flag) {
        char buffer[200];

        strlcpy(buffer, " ", sizeof(buffer));

        char star_idx = MOX::_planet[planet_idx].star_index;
        char planet_num = HAROLD::Planet_Number_(planet_idx);

        const char* numeral_str;
        if (localized_flag == 0) {
            numeral_str = MOX::_roman_literals[planet_num + 1];
        }
        else {
            numeral_str = MOX::_roman_literals[planet_num + 1];
            if (strcmp(numeral_str, "I") == 0) {
                numeral_str = HAROLD::H_Message_(0xa0);
            }
        }

        s_star_data* star = &MOX::_star[star_idx];

        snprintf(buffer, sizeof(buffer), "%s %s", star->name, numeral_str);

        if (strlen(buffer) > 30) {
            buffer[30] = '\0';
        }

        strlcpy(out_str, buffer, 31);
    }

    int16_t __cdecl Get_Hyper_Tech_App_ID_(int16_t tech_id) {
        switch (tech_id) {
        case TECH_FIELD_BIOLOGY:
            return TECH_APP_BIOLOGY;
        case TECH_FIELD_POWER:
            return TECH_APP_POWER;
        case TECH_FIELD_PHYSICS_2:
            return TECH_APP_PHYSICS;
        case TECH_FIELD_CONSTRUCTION:
            return TECH_APP_CONSTRUCTION;
        case TECH_FIELD_FIELDS:
            return TECH_APP_FIELDS;
        case TECH_FIELD_CHEMISTRY_2:
            return TECH_APP_CHEMISTRY;
        case TECH_FIELD_COMPUTERS:
            return TECH_APP_COMPUTERS;
        case TECH_FIELD_SOCIOLOGY:
            return TECH_APP_SOCIOLOGY;
        default:
            return tech_id;
        }
    }

    char* __cdecl Technology_Fields_Name_(int16_t tech_field_id) {
        if (tech_field_id < TECH_FIELD_BIOLOGY) {
            return TECHDATA::_technology_fields[tech_field_id].name;
        }

        strlcpy(_hyper_field_name_buffer, MOX::_hyper_field_title, sizeof(_hyper_field_name_buffer));
        return _hyper_field_name_buffer;
    }

    int32_t __cdecl New_Get_Tech_Cost_(int16_t id, uint8_t is_tech_app, int16_t player_idx) {
        int16_t tech_field_id;
        if (is_tech_app != 0) {
            tech_field_id = TECHDATA::_technology_applications[id].tech_field_id;
        } else {
            tech_field_id = id;
        }
        return COLCALC::Player_Research_Cost_(&MOX::_player[player_idx], tech_field_id);
    }

    void __cdecl Get_Group_List_(s_list_state *state, int16_t group_idx) {
        uint8_t num_items = 0;
        s_list_item *item = state->items;
        uint16_t u_group_idx = static_cast<uint16_t>(group_idx);
        int16_t field_id = static_cast<int8_t>(MOX::_first_field_in_group[u_group_idx]);

        while (field_id != 0) {
            uint8_t status = MOX::_cur_plyr_ptr->tech_fields[field_id];
            item->group_id = static_cast<uint8_t>(field_id);

            strlcpy(item->group_name, Technology_Fields_Name_(static_cast<uint16_t>(field_id)), sizeof(item->group_name));

            if (MOX::_cur_plyr_ptr->current_research_field == field_id) {
                status = 4;
            } else if (field_id >= TECH_FIELD_BIOLOGY &&
                       MOX::_cur_plyr_ptr->hyper_advanced_tech[field_id - TECH_FIELD_BIOLOGY] > 20) {
                status = 3;
            }

            if (status != 3) {
                if (status == 0) {
                    for (int i = 0; i < 4; ++i) {
                        int16_t tech_id = TECHDATA::_technology_fields[field_id].tech[i];

                        if (MOX::_cur_plyr_ptr->tech_applications[tech_id] == TECH_RESEARCH_STATUS_RESEARCHED) {
                            status = 1;
                            break;
                        }
                    }
                }

                if (status >= 1) {
                    uint8_t app_count = 0;
                    for (int i = 0; i < 4; ++i) {
                        int16_t tech_id = TECHDATA::_technology_fields[field_id].tech[i];

                        uint8_t app_status = MOX::_cur_plyr_ptr->tech_applications[tech_id];

                        if (app_status >= 1) {
                            if (status == 4) {
                                app_status = 4;
                            } else if (app_status == 1 && status == 2) {
                                app_status = 2;
                            }

                            item->tech_app_ids[app_count] = static_cast<uint8_t>(tech_id);
                            item->tech_app_status[app_count] = app_status;
                            app_count++;
                        }
                    }

                    if (app_count != 0) {
                        item->app_count = app_count;
                        item->group_color_index = status;
                        num_items++;
                        item++;
                    }
                }
            }

            field_id = TECHDATA::_technology_fields[field_id].next_field_id;
        }

        state->num_items = num_items;
    }

    void __cdecl Draw_Tech_List_() {
        int16_t input = fields::Scan_Input_();
        if (input > 0) {
            LIST::Set_Selected_List_Entry_(&_g_tech_list_header, input);
        }

        _g_selected_list_entry_p = LIST::Get_Selected_List_Entry_(&_g_tech_list_header);

        LIST::Display_Selected_List_Text_For_Tech_List_(&_g_tech_list_header, _g_selected_list_entry_p);

        palette::Cycle_Palette_Color_(_g_cursor_box_color, 8, 32, 0, 40, 63, 8, 8);
    }

    char* __cdecl Technology_Applications_Name_(int16_t tech_app_id, int16_t player_idx) {
        if (tech_app_id < TECH_APP_BIOLOGY) {
            return TECHDATA::_technology_applications[tech_app_id].name;
        }

        uint8_t hyper_tech = MOX::_player[player_idx].hyper_advanced_tech[tech_app_id - TECH_APP_BIOLOGY];

        strlcpy(MOX::_temp_string_buf, TECHDATA::_technology_applications[tech_app_id].name, sizeof(MOX::_temp_string_buf));
        strlcat(MOX::_temp_string_buf, " ", sizeof(MOX::_temp_string_buf));

        if (hyper_tech > 0 && hyper_tech <= 20) {
            strlcat(MOX::_temp_string_buf, MOX::_roman_literals[hyper_tech], sizeof(MOX::_temp_string_buf));
        } else {
            char* end_ptr = MOX::_temp_string_buf + strlen(MOX::_temp_string_buf);
            snprintf(end_ptr, sizeof(MOX::_temp_string_buf) - (size_t)(end_ptr - MOX::_temp_string_buf), "%u", (unsigned)hyper_tech);
        }

        return MOX::_temp_string_buf;
    }

    void __cdecl Display_Selected_Entry_Text_(s_list_item_research* entry) {
        if (entry != nullptr && entry->current_app_index != 0 && entry->tech_field_id != 0) {
            fonts::Set_Font_Style_(4, &_tech_color[1]);
            BILL::Squeeze_Print_(entry->x, entry->y + 0x15, entry->field_name, 0xDA);

            fonts::Set_Font_Style_(3, &_tech_color[1]);
            fonts::Print_Right_(entry->x + 0xD4, entry->y + 2, entry->buf);

            if (entry->visible_app_count > entry->selected_app_index) {
                BILL::Squeeze_Print_(entry->label_x,
                                     entry->app_label_y[entry->selected_app_index],
                                     entry->app_names[entry->selected_app_index],
                                     0xD0);
            }

            graphics::Box_(entry->x - 2, entry->y + 0x12, 0xDA, 0x52, 1, _g_cursor_box_color);
        }
    }

    void __cdecl Tech_Change_() {
        allocate::Allocate_First_Block_(MOX::_screen_seg, 4);
        video::Set_Page_Off_();
        fields::Draw_Visible_Fields_();
        video::Copy_Off_To_Back_();
        _Tech_Select_(1);
        MOX::_current_screen = MOX::_return_screen;
    }

    void __cdecl Promote_Hyper_Field_Values_(int16_t player_idx) {
        for (int i = 0; i < 8; ++i) {
            MOX::_player[player_idx].hyper_advanced_tech[i]++;
        }
    }

    void __cdecl Reset_Screen_() {
        palette::Restore_Palette_();
        BILL::Turn_Off_Palettes_(0, 0xff);
        palette::Clear_Palette_Changes_(0, 0xff);
        fields::Deactivate_Help_List_();
        COLONY::Reset_Mouse_Picture_();
        file_ani::Reset_File_Animation_Buffer_();
        JIM::Play_Background_Music_();
    }
}