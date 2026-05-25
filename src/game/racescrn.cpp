#include "pch.h"

namespace RACESCRN {

    void __cdecl Calculate_Icon_Group_Data_(s_racescrn_mouse_group* group) {
        int16_t width = (group->x2 - group->x1) + 1;
        group->total_height = (group->y2 - group->y1) + 1;

        if (group->count <= 1) {
            group->spacing_x = group->icon_width;
            group->total_width = group->icon_width;
        } else {
            // Calculate spacing based on available width and icon count
            int16_t spacing = (width - group->icon_width) / (group->count - 1);

            // Clamp spacing: cannot exceed icon width and cannot be negative
            if (spacing > group->icon_width) {
                spacing = group->icon_width;
            }

            if (spacing < 0) {
                spacing = 0;
            }

            group->spacing_x = spacing;
            group->total_width = group->icon_width + (group->count - 1) * spacing;

            // Ensure total width does not exceed the defined bounding box width
            if (group->total_width > width) {
                group->total_width = width;
            }
        }
    }

    void __cdecl Setup_Icon_Group_Data_(s_racescrn_mouse_group* group, s_racescrn_icon_layout* layout, int16_t count, s_animation_header* sprite1, s_animation_header* sprite2) {
        group->x1 = layout->x1;
        group->y1 = layout->y1;
        group->x2 = layout->x2;
        group->y2 = layout->y2;

        group->total_height = layout->spacing_x;

        group->render_fn = layout->render_fn;

        group->count = count;

        if (sprite1 == nullptr) {
            group->icon_width = 0;
        } else {
            group->icon_width = animate::Get_Width_(sprite1);
        }

        group->sprite1 = sprite1;
        group->sprite2 = sprite2;

        Calculate_Icon_Group_Data_(group);
    }

    void __cdecl Draw_Race_Text_(s_racescrn_race_display* display) {
        char buffer[200];
        char string2[100];
        int16_t spy_bonus;
        int16_t agent_bonus;

        for (int i = 0; i < 7; ++i) {
            int16_t race_text_x = _race_text[i * 2];
            int16_t race_text_y = _race_text[i * 2 + 1];

            if (display->is_empty_slot != 0 || 
                (HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM) && MOX::_player[MOX::_PLAYER_NUM].contact[display->player_idx] == 0)) 
            {
                fonts::Set_Font_Style_(4, &_race_normal_color);
                fonts::Print_Centered_(race_text_x + 0x5B, race_text_y + 10, _no_contact_str);

                if (i < MOX::_active_count && 
                    HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM) && 
                    MOX::_player[MOX::_PLAYER_NUM].contact[display->player_idx] == 0) 
                {
                    fonts::Set_Font_Style_Outline_(2, &_race_high_color);
                    BILL::Squeeze_Centered_Print_(
                        _race_picture[i * 2],
                        _race_picture[i * 2 + 1] + 0x4C,
                        display->player_name,
                        0x4C
                    );
                }
            } else {
                fonts::Set_Font_Style_Outline_(2, &_race_high_color);
                BILL::Squeeze_Centered_Print_(
                    _race_picture[i * 2],
                    _race_picture[i * 2 + 1] + 0x4C,
                    display->player_name,
                    0x4C
                );

                if (display->is_eliminated == 0) {
                    buffer[0] = s_empty_string[0];

                    int16_t treaty = MOX::_cur_plyr_ptr->treaty[display->player_idx];
                    if (treaty > 0) {
                        if (treaty > 5) {
                            treaty = 5;
                        }
                        strcpy(buffer, MOX::_treaty_labels[treaty]);
                        _strupr(buffer);
                    }

                    if (MOX::_cur_plyr_ptr->research_treaty[display->player_idx] != 0) {
                        int16_t research_level = MOX::_cur_plyr_ptr->current_research_agreement_level[display->player_idx];
                        const char* type_str;

                        switch (MOX::_settings_language) {
                            case 0:
                                type_str = research_level >= 0 ? s_rp_str : s_bc_str;
                                break;
                            case 1:
                                type_str = research_level >= 0 ? s_fp_str : s_mc_str;
                                break;
                            case 2:
                                type_str = research_level >= 0 ? s_rp_str : s_bc_str;
                                break;
                            case 3:
                                type_str = research_level >= 0 ? s_rp_str : s_bc_str;
                                break;
                            case 4:
                                type_str = research_level >= 0 ? s_pr_str : s_bc_str;
                                break;
                            default:
                                type_str = research_level >= 0 ? s_rp_str : s_bc_str;
                                break;
                        }

                        sprintf(string2, s_research_fmt, _research_treaty_buffer, research_level, type_str);
                        strcat(buffer, string2);
                    }

                    if (MOX::_cur_plyr_ptr->trade_treaty[display->player_idx] != 0) {
                        int16_t trade_level = MOX::_cur_plyr_ptr->current_trade_agreement_level[display->player_idx];
                        const char* format = (MOX::_settings_language == 1) ? s_trade_mc_fmt : s_trade_bc_fmt;
                        sprintf(string2, format, _trade_treaty_buffer, trade_level);
                        strcat(buffer, string2);
                    }

                    if (MOX::_cur_plyr_ptr->tribute_treaty[display->player_idx] != 0) {
                        strcat(buffer, s_carriage_return);
                        strcpy(string2, _giving_treaty_buffer);
                        int stat_num = (MOX::_cur_plyr_ptr->tribute_treaty[display->player_idx] == 1) ? 5 : 10;
                        JIM::Decode_Text_Stat_(string2, stat_num, 0x40);
                        strcat(buffer, string2);
                    } else if (MOX::_player[display->player_idx].tribute_treaty[MOX::_PLAYER_NUM] != 0) {
                        strcat(buffer, s_carriage_return);
                        strcpy(string2, _receiving_treaty_buffer);
                        int stat_num = (MOX::_player[display->player_idx].tribute_treaty[MOX::_PLAYER_NUM] == 1) ? 5 : 10;
                        JIM::Decode_Text_Stat_(string2, stat_num, 0x40);
                        strcat(buffer, string2);
                    }

                    if (strlen(buffer) < 1) {
                        strcpy(buffer, MOX::_treaty_labels[0]);
                    }

                    fonts::Set_Font_Style_(2, &_race_normal_color);
                    BILL::Squeeze_Paragraph_Centered_(race_text_x, race_text_y, 0xB6, 0x2B, buffer, 2);
                }
            }
            display++;
        }

        fonts::Set_Font_Style_(3, &_race_normal_color);
        spy::Compute_Spy_Bonuses_(MOX::_cur_plyr_ptr, &spy_bonus, &agent_bonus);

        sprintf(string2, s_spy_bonus_fmt, _spy_bonus_str, spy_bonus);
        BILL::Squeeze_Centered_Print_(0x1A1, 0x174, string2, 0x62);

        sprintf(string2, s_agent_bonus_fmt, _agent_bonus_str, agent_bonus);
        BILL::Squeeze_Centered_Print_(0x207, 0x174, string2, 0x61);
    }

    void __cdecl Draw_Relations_Sliders_(s_racescrn_race_display* displays) {
        allocate::Push_Block_(MOX::_screen_seg);
        s_animation_header* bar_anim = (s_animation_header*)farload::Far_Reload_Next_(s_races_lbx, 3, MOX::_screen_seg);

        for (int16_t i = 0; i < 7; ++i) {
            int16_t x;
            int16_t y;
            s_animation_header* slider_img;

            if (i < MOX::_active_count) {
                animate::Draw_(_race_bar[i * 2], _race_bar[i * 2 + 1], bar_anim);
                y = displays[i].relationship_slider_y;
                x = _race_slider_x[i];
                slider_img = displays[i].relationship_slider_img;
            } else {
                x = _race_slider_x[i];
                slider_img = displays[i].relationship_slider_img;
                y = _race_bar[i * 2 + 1] + 0x4B;
            }

            animate::Draw_(x, y, slider_img);
        }

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Draw_Race_Photos_(s_racescrn_race_display* race_displays) {
        for (int i = 0; i < (int)MOX::_active_count; ++i) {
            s_racescrn_race_display* display = &race_displays[i];
            int16_t x = _race_picture[i * 2];
            int16_t y = _race_picture[i * 2 + 1];

            animate::Draw_(x, y, display->race_portrait_img);

            if (display->is_eliminated != 0) {
                animate::Draw_(x, y, display->eliminated_overlay_img);
            }

            uint8_t color_idx = MOX::_player[display->player_idx].color;

            graphics::Box_(x, y, 76, 88, 1, _banner_color_high.colors[color_idx]);
            graphics::Box_(x + 1, y + 1, 74, 86, 1, _banner_color_low.colors[color_idx]);

            uint8_t edge_color = _banner_color_edge.colors[color_idx];
            graphics::Dot_(x, y, edge_color);
            graphics::Dot_(x + 75, y, edge_color);
            graphics::Dot_(x, y + 87, edge_color);
            graphics::Dot_(x + 75, y + 87, edge_color);
        }
    }

    void __cdecl Init_Race_Display_Data_(s_racescrn_race_display* race_display, s_racescrn_mouse_group* agent_group) {
        memset(race_display, 0, sizeof(s_racescrn_race_display) * 7);

        s_animation_header* spy_agent_icon_sprite = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, MOX::_player[MOX::_PLAYER_NUM].race + 0x2E, MOX::_screen_seg);
        s_animation_header* slider_sprite = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, 2, MOX::_screen_seg);
        s_animation_header* eliminated_overlay_sprite = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, 0x1F, MOX::_screen_seg);

        for (int16_t i = 0; i < 7; ++i) {
            race_display[i].relationship_slider_img = slider_sprite;

            if (i < MOX::_active_count) {
                uint8_t active_player_idx = MOX::_active_player[i];
                s_player* player = &MOX::_player[active_player_idx];
                int16_t relations_val = MOX::_cur_plyr_ptr->relations[active_player_idx] + 100;

                int16_t relation_idx = 0;
                while (RACESCRN::_relation_name_table[relation_idx] < (uint8_t)relations_val) {
                    relation_idx++;
                }

                race_display[i].relationship_text = RACESCRN::_relation_text[relation_idx];
                int16_t remap_index = relations_val / 8;
                uint8_t slider_remap_idx = RACESCRN::_slider_remap_list[remap_index];

                race_display[i].list_idx = i;
                race_display[i].player_idx = active_player_idx;
                race_display[i].player = player;
                race_display[i].player_name = player->name;
                race_display[i].race = (int16_t)(uint8_t)player->race;
                race_display[i].relationship_slider_y = RACESCRN::_race_bar_y[i * 2] + (int16_t)slider_remap_idx - 6;
                race_display[i].eliminated_overlay_img = eliminated_overlay_sprite;

                if (!player->eliminated) {
                    int16_t spy_count = BILL::Get_My_Spy_Number_(active_player_idx);
                    race_display[i].agent_mission_idx = (int16_t)BILL::Get_My_Spy_Mission_(active_player_idx) + 1;

                    RACESCRN::Setup_Icon_Group_Data_(&race_display[i].spy_icons_group, &RACESCRN::_spy_group_data[i], spy_count, spy_agent_icon_sprite, nullptr);

                    race_display[i].spy_icon_img = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, i + 10, MOX::_screen_seg);
                    race_display[i].sabotage_icon_img = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, i + 17, MOX::_screen_seg);
                    race_display[i].assassination_icon_img = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, i + 24, MOX::_screen_seg);
                } else {
                    race_display[i].is_eliminated = 1;
                }

                race_display[i].race_portrait_img = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, race_display[i].race + 32, MOX::_screen_seg);
            } else {
                race_display[i].is_empty_slot = 1;
            }
        }

        int16_t agent_count = BILL::Get_My_Agent_Number_();
        RACESCRN::Setup_Icon_Group_Data_(agent_group, &_agent_group_data, agent_count, spy_agent_icon_sprite, nullptr);
    }

    int16_t __cdecl Setup_Main_Fields_(int16_t* ok_btn_id, int16_t* agriculture_btn_id, int16_t* agriculture_field_id,
                               int16_t* research_btn_id, int16_t* research_field_id, int16_t* industry_btn_id,
                               int16_t* industry_field_id, int16_t* diplomacy_btn_id, int16_t* diplomacy_field_id)
    {
        fields::Clear_Fields_();

        s_animation_header* img = (s_animation_header*)farload::Far_Reload_Next_(s_races_lbx, 9, MOX::_screen_seg);
        *ok_btn_id = fields::Add_Button_Field_(535, 433, s_empty_string, img, s_space, 40 /* '(' */);

        *agriculture_field_id = 0;
        *research_field_id = 0;
        *industry_field_id = 0;
        *diplomacy_field_id = 0;

        if (MOX::_active_count < 1) {
            *industry_btn_id = 0;
            *research_btn_id = 0;
            *agriculture_btn_id = 0;
            *diplomacy_btn_id = 0;
        } else {
            img = (s_animation_header*)farload::Far_Reload_Next_(s_races_lbx, 6, MOX::_screen_seg);
            *industry_btn_id = fields::Add_Radio_Button_Field_(429, 444, s_empty_string, img, industry_field_id, s_I, 40 /* '(' */);

            img = (s_animation_header*)farload::Far_Reload_Next_(s_races_lbx, 7, MOX::_screen_seg);
            *research_btn_id = fields::Add_Radio_Button_Field_(429, 423, s_empty_string, img, research_field_id, s_R, 40 /* '(' */);

            img = (s_animation_header*)farload::Far_Reload_Next_(s_races_lbx, 8, MOX::_screen_seg);
            *agriculture_btn_id = fields::Add_Radio_Button_Field_(334, 423, s_empty_string, img, agriculture_field_id, s_A, 40 /* '(' */);

            img = (s_animation_header*)farload::Far_Reload_Next_(s_races_lbx, 60, MOX::_screen_seg);
            *diplomacy_btn_id = fields::Add_Radio_Button_Field_(334, 444, s_empty_string, img, diplomacy_field_id, s_D, 40 /* '(' */);
        }

        return (int16_t)(fields::Get_Field_Counter_() - 1);
    }

    void __cdecl Move_Icon_Group_Icons_(s_racescrn_mouse_group* src, s_racescrn_mouse_group* dst, uint16_t count) {
        if (src->count < count) {
            count = src->count;
        }

        if (count != 0) {
            dst->count += count;
            src->count -= count;
            dst->icon_width = src->icon_width;
            dst->sprite1 = src->sprite1;

            Calculate_Icon_Group_Data_(dst);
            Calculate_Icon_Group_Data_(src);
        }
    }

    void __cdecl Move_Spy_Group_From_Mouse_(s_racescrn_mouse_group* target_group) {
        int16_t num_to_move = _g_mouse_group_p->count;

        if (num_to_move != 0) {
            if (target_group->count + num_to_move > 63) {
                int16_t available_space = 63 - target_group->count;
                _g_spy_xfer_count = num_to_move - available_space;
                num_to_move = available_space;
            }

            Move_Icon_Group_Icons_(_g_mouse_group_p, target_group, num_to_move);

            if (_g_mouse_group_p->count == 0) {
                JIM::Bill_Picture_To_Mouse_(_g_mouse_seg);
            } else {
                Redraw_Spy_Mouse_(_arrow_seg);
            }
        }
    }

    /**
     * @address 0049da90
     */
    void __cdecl Move_Spies_Group_(s_racescrn_mouse_group* mouse_group, s_animation_header* mouse_sprite) {
        Move_Icon_Group_Icons_(mouse_group, RACESCRN::_g_mouse_group_p, RACESCRN::_g_spy_xfer_count);
        Redraw_Spy_Mouse_(mouse_sprite);
    }

    /**
     * @address 0049d960
     */
    void __cdecl Adjust_Spy_Mission_Data_() {
        int16_t active_count = MOX::_active_count;

        // Iterate through all active race display slots
        for (int i = 0; i < (int)active_count; ++i) {
            // Default mission index 0 (likely 'None') is updated to 3 (likely 'Passive/Hide')
            if (_g_display_p[i].agent_mission_idx == 0) {
                _g_display_p[i].agent_mission_idx = 3;
            }
        }
    }

    int8_t __cdecl Setup_Race_Display_Fields_(s_racescrn_race_display* race_displays, int16_t base_field_id) {
        if (MOX::_active_count <= 0) {
            return 0;
        }

        fields::Clear_Fields_Above_(base_field_id);

        RACESCRN::_g_agent_field = fields::Add_Hidden_Field_(328, 386, 613, 415, RACESCRN::s_empty_string, 0);

        for (int i = 0; i < (int)MOX::_active_count; ++i) {
            s_racescrn_race_display* display = &race_displays[i];
            int16_t btn_x = RACESCRN::_race_spy_btns[i * 2];
            int16_t btn_y = RACESCRN::_race_spy_btns[i * 2 + 1];

            // Add mission toggle buttons (Spy, Sabotage, Assassination)
            fields::Add_Multi_Button_Field_(btn_x, btn_y, RACESCRN::s_empty_string, display->spy_icon_img, &display->agent_mission_idx, 1, RACESCRN::s_empty_string, 41);
            fields::Add_Multi_Button_Field_(btn_x + 76, btn_y, RACESCRN::s_empty_string, display->sabotage_icon_img, &display->agent_mission_idx, 2, RACESCRN::s_empty_string, 41);
            fields::Add_Multi_Button_Field_(btn_x + 149, btn_y, RACESCRN::s_empty_string, display->assassination_icon_img, &display->agent_mission_idx, 3, RACESCRN::s_empty_string, 41);

            // Add interaction field for spy/agent area
            int16_t spy_x = RACESCRN::_race_spies[i * 2];
            int16_t spy_y = RACESCRN::_race_spies[i * 2 + 1];
            display->main_interaction_field_id = fields::Add_Hidden_Field_(spy_x, spy_y - 6, spy_x + 187, spy_y + 25, RACESCRN::s_empty_string, 0);

            // Add interaction field for relationship bar area
            int16_t bar_x = RACESCRN::_race_bar[i * 2];
            int16_t bar_y = RACESCRN::_race_bar[i * 2 + 1];
            display->relationshiop_field_id = fields::Add_Hidden_Field_(bar_x - 6, bar_y - 6, bar_x + 8, bar_y + 88, RACESCRN::s_empty_string, 0);
        }

        return 1;
    }

    int8_t __cdecl Setup_Race_Who_Fields_(s_racescrn_race_display* race_displays, int16_t field_id_base) {
        if (MOX::_active_count <= 0) {
            return 0;
        }

        fields::Clear_Fields_Above_(field_id_base);

        int16_t x_offset = 0;
        auto race_picture = reinterpret_cast<int16_t*>(std::uintptr_t{0x00558128});

        for (int16_t i = 0; i < (int16_t)MOX::_active_count; ++i) {
            if (i == 4) {
                x_offset = 320;
            }

            int16_t y_base = race_picture[i * 2 + 1];

            race_displays[i].main_interaction_field_id = fields::Add_Hidden_Field_(
                x_offset,
                y_base,
                x_offset + 319,
                y_base + 88,
                s_empty_string,
                0
            );
        }

        _g_agent_field = 9999;
        return 1;
    }

    /**
     * @brief Finds the spy group corresponding to a specific UI field ID.
     * 
     * @param field_id The UI field ID to look up.
     * @param out_index Pointer to store the index of the race display entry found.
     * @return Pointer to the s_racescrn_mouse_group if found, otherwise nullptr.
     */
    s_racescrn_mouse_group* __cdecl Get_Spy_Group_For_Field_(int16_t field_id, int16_t* out_index) {
        *out_index = 0;

        if (_g_agent_field == field_id) {
            return _g_agent_group_p;
        }

        if (MOX::_active_count > 0) {
            for (int16_t i = 0; i < MOX::_active_count; ) {
                if (_g_display_p[i].main_interaction_field_id == field_id) {
                    return &_g_display_p[i].spy_icons_group;
                }

                i++;
                *out_index = i;
            }
        }

        return nullptr;
    }

    void __cdecl Update_Spy_Stuff_(s_racescrn_race_display *race_displays) {
        int16_t active_count = MOX::_active_count;
        s_racescrn_race_display *end = &race_displays[active_count];

        for (s_racescrn_race_display *display = race_displays; display != end; ++display) {
            if (display->agent_mission_idx > 0) {
                BILL::Set_My_Spy_Mission_(display->player_idx, display->agent_mission_idx - 1);
            }
            BILL::Set_My_Spy_Number_(display->player_idx, (uint8_t)display->spy_icons_group.count);
        }

        BILL::Set_My_Agent_Number_((uint8_t)RACESCRN::_g_agent_group_p->count);
    }

    void __cdecl Draw_Race_Screen_() {
        s_racescrn_race_display* display;
        int16_t input_field;
        int16_t mouse_x;
        int16_t mouse_y;
        char text_buffer[100];
        int32_t race_idx;

        s_racescrn_race_display* end = &RACESCRN::_g_display_p[MOX::_active_count];
        input_field = fields::Scan_Input_();
        mouse_x = mouse::Pointer_X_();
        mouse_y = mouse::Pointer_Y_();

        fields::Enable_Draw_All_Fields_();
        fields::Draw_Visible_Fields_();
        fields::Disable_Draw_All_Fields_();

        for (display = RACESCRN::_g_display_p; display != end; ++display) {
            int16_t list_idx = display->list_idx;
            RACESCRN::Draw_Icon_Group_(&display->spy_icons_group);

            if (RACESCRN::_g_in_who_mode != 0) {
                if (display->main_interaction_field_id == input_field) {
                    graphics::Box_(RACESCRN::_race_picture[list_idx * 2], RACESCRN::_race_picture[list_idx * 2 + 1], 0x4C, 0x58, 1, 0xFF);
                }
            } else {
                if (display->main_interaction_field_id == input_field) {
                    if (RACESCRN::_g_in_spy_mode != 0 || display->spy_icons_group.count > 0) {
                        fonts::Set_Font_Style_Outline_(2, &RACESCRN::_race_high_color);
                        RACESCRN::_g_spy_xfer_count = RACESCRN::Draw_Icon_Group_Cursor_(&display->spy_icons_group, mouse_x, 0xFF, 1);
                    }
                } else if (display->relationshiop_field_id == input_field) {
                    fonts::Set_Font_Style_Outline_(2, &RACESCRN::_race_high_color);
                    fonts::Print_Centered_(RACESCRN::_race_bar[list_idx * 2] + 4, display->relationship_slider_y + 1, display->relationship_text);
                }
            }

            if ((1 << display->player_idx) & MOX::_cur_plyr_ptr->ignoring) {
                fonts::Set_Font_Style_Outline_(1, &RACESCRN::_race_high_color);
                fonts::Print_Centered_(RACESCRN::_race_picture[list_idx * 2] + 0x25, RACESCRN::_race_picture[list_idx * 2 + 1] + 0x42, RACESCRN::_ignoring_str);
            }
        }

        RACESCRN::Draw_Icon_Group_(RACESCRN::_g_agent_group_p);

        if (input_field == RACESCRN::_g_agent_field) {
            if (RACESCRN::_g_agent_group_p->count > 0 || RACESCRN::_g_in_spy_mode != 0) {
                fonts::Set_Font_Style_Outline_(2, &RACESCRN::_race_high_color);
                RACESCRN::_g_spy_xfer_count = RACESCRN::Draw_Icon_Group_Cursor_(RACESCRN::_g_agent_group_p, mouse_x, 0xFF, 1);
            }
        }

        if (RACESCRN::_g_mouse_group_p->count != 0) {
            video::Clip_On_();
            animate::Draw_(mouse_x + 3, mouse_y + 2, RACESCRN::_g_mouse_group_p->sprite2);
            video::Clip_Off_();
        }

        palette::Cycle_Palette_Color_(0xFF, 10, 20, 5, 40, 60, 10, 7);
        fonts::Set_Font_Style_Outline_(4, &RACESCRN::_race_high_color);

        if (RACESCRN::_net_state == 1) {
            animate::Draw_(0x85, 0xBF, (s_animation_header*)RACESCRN::_netbox_seg);
            strcpy(text_buffer, RACESCRN::_diplomacy_net_requeting_buffer);
            race_idx = NETMOX::Net_Diplomacy_With_();
            JIM::Decode_Text_Race_(text_buffer, (int16_t)race_idx, 100);
            FMTPARA::Print_Formatted_Paragraph_(0x8F, 0xDA, 0x15E, text_buffer, 2);
        } else if (RACESCRN::_net_state == 2) {
            animate::Draw_(0x85, 0xBF, (s_animation_header*)RACESCRN::_netbox_seg);
            strcpy(text_buffer, RACESCRN::_diplomacy_net_wait_buffer);
            race_idx = NETMOX::Net_Diplomacy_With_();
            JIM::Decode_Text_Race2_(text_buffer, (int16_t)race_idx, 100);
            FMTPARA::Print_Formatted_Paragraph_(0x8F, 0xD1, 0x15E, text_buffer, 2);
        }
    }

    void __cdecl Redraw_Spy_Mouse_(s_animation_header* spy_icon) {
        char num_str[16];

        Draw_Icon_Group_(_g_mouse_group_p);
        bitmap::Add_Picture_To_Bitmap_(1, 1, spy_icon, _g_mouse_group_p->sprite2);

        if (_g_spy_xfer_count > 1) {
            fonts::Set_Font_Style_Outline_(2, &_race_high_color);
            itoa(static_cast<int32_t>(_g_spy_xfer_count), num_str, 10);
            fonts::Print_To_Bitmap_(8, 4, num_str, _g_mouse_group_p->sprite2);
        }

        COLONY::Clear_Mouse_Picture_();
    }

    int16_t __cdecl Draw_Icon_Group_Cursor_(s_racescrn_mouse_group* group, int16_t x_pos, uint8_t color, bool show_count) {
        if (group->count == 0) {
            graphics::Box_(group->x1, group->y1, group->icon_width, group->total_height, 1, color);
            return 0;
        }

        int16_t max_x = group->total_width + group->x1 - 1;
        if (x_pos > max_x) {
            x_pos = max_x;
        }
        if (x_pos < group->x1) {
            x_pos = group->x1;
        }

        int16_t index = (x_pos - group->x1) / group->spacing_x;
        if (index >= group->count) {
            index = group->count - 1;
        }

        int16_t remaining_count = group->count - index;
        if (remaining_count > 0) {
            int16_t draw_x = group->spacing_x * index + group->x1;
            int16_t width = (max_x - draw_x) + 1;

            graphics::Box_(draw_x, group->y1, width, group->total_height, 1, color);

            if (show_count && remaining_count > 1) {
                fonts::Print_Integer_(draw_x + 3, group->y1 + 3, remaining_count);
            }

            return remaining_count;
        }

        return 0;
    }

    void __cdecl Draw_Icon_Group_(s_racescrn_mouse_group* group) {
        if (group->render_fn != nullptr) {
            auto fn = reinterpret_cast<render_fn>(group->render_fn);
            fn(group);
        }

        if (group->count > 0) {
            int16_t current_x = group->x1;
            for (int16_t i = 0; i < group->count; ++i) {
                if (group->sprite2 == nullptr) {
                    animate::Draw_(current_x, group->y1, group->sprite1);
                } else {
                    bitmap::Add_Picture_To_Bitmap_(current_x, group->y1, group->sprite1, group->sprite2);
                }
                current_x += group->spacing_x;
            }
        }
    }

    void __cdecl Race_Screen_() {
        int16_t selected_race_idx = -1;
        int16_t old_mission_idx = -1;
        MOX2::action_func_t auto_function = nullptr;

        MOX2::Get_Auto_Function_(&auto_function);
        palette::Save_Palette_();
        fields::Disable_Draw_All_Fields_();
        RUSS::Mox_Update_();

        for (int i = 0; i < 17; ++i) {
            JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x21 + i, _relation_text[i], 15);
            RUSS::Mox_Update_();
        }

        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x32, _ignoring_str, 20);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x33, _no_contact_str, 20);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x34, _spy_bonus_str, 20);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x35, _agent_bonus_str, 20);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x39, _research_treaty_buffer, 30);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x3A, _trade_treaty_buffer, 30);
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x3B, _giving_treaty_buffer, 80);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x3C, _receiving_treaty_buffer, 80);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x4C, _diplomacy_net_requeting_buffer, 100);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x4D, _diplomacy_net_wait_buffer, 100);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtex2_lbx_00556038, 0x1A, _declare_war_buffer, 120);
        RUSS::Mox_Update_();
        
        ERIC::Fast_Fade_Out_();

        while (true) {
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            RUSS::Mox_Update_();

            s_racescrn_race_display race_display_data[7];
            s_racescrn_mouse_group agent_group;
            char message_buffer[120];
            s_racescrn_mouse_group mouse_group;

            MOX::_cur_plyr_ptr = MOX::_player + MOX::_PLAYER_NUM;
            _g_display_p = race_display_data;
            _g_mouse_group_p = &mouse_group;
            _g_agent_group_p = &agent_group;

            RUSS::Mox_Update_();
            BILLHELP::Races_Help_();
            fields::Clear_Fields_();
            video::Set_Page_Off_();
            video::Set_Refresh_Stencil_();
            RUSS::Mox_Update_();

            uint32_t bmp_size = bitmap::Get_Bitmap_Size_(187, 25);
            s_animation_header* sprite = static_cast<s_animation_header*>(allocate::Allocate_First_Block_(MOX::_screen_seg, bmp_size));
            bitmap::Create_Blank_Bitmap_(187, 25, sprite);
            RUSS::Mox_Update_();

            s_animation_header* anim = reinterpret_cast<s_animation_header*>(
                farload::Far_Reload_Next_(s_races_lbx_005574e8, 0, MOX::_screen_seg));
            RUSS::Mox_Update_();

            _arrow_seg = reinterpret_cast<s_animation_header*>(
                farload::Far_Reload_Next_(s_races_lbx_005574e8, 1, MOX::_screen_seg));
            RUSS::Mox_Update_();

            _g_mouse_seg = reinterpret_cast<s_animation_header*>(
                farload::Far_Reload_Next_(s_races_lbx_005574e8, 4, MOX::_screen_seg));
            RUSS::Mox_Update_();

            int16_t sprite_idx = 5;
            switch (MOX::_settings.language) {
                case 0: sprite_idx = 5; break;
                case 1: sprite_idx = 61; break;
                case 2: sprite_idx = 62; break;
                case 3: sprite_idx = 5; break;
                case 4: sprite_idx = 63; break;
            }
            s_animation_header* mouse_pic = reinterpret_cast<s_animation_header*>(
                farload::Far_Reload_Next_(s_races_lbx_005574e8, sprite_idx, MOX::_screen_seg));
            RUSS::Mox_Update_();

            _netbox_seg = farload::Far_Reload_Next_(s_races_lbx_005574e8, 59, MOX::_screen_seg);
            RUSS::Mox_Update_();

            JIM::Bill_Picture_To_Mouse_(_g_mouse_seg);
            BILL::Get_Players_Dead_Or_Alive_Or_Omniscient_(0, 0);
            RUSS::Mox_Update_();

            Init_Race_Display_Data_(race_display_data, &agent_group);
            RUSS::Mox_Update_();

            Setup_Icon_Group_Data_(&mouse_group, &_mouse_group_data, 0, nullptr, sprite);
            RUSS::Mox_Update_();

            graphics::Fill_(0, 0, 639, 479, 0);
            animate::Draw_(0, 0, anim);
            RUSS::Mox_Update_();

            if (ERIC::Get_Fade_Percent_() > 0) {
                BILL::Turn_Off_Palettes_(0, 255);
            }
            palette::Reset_Cycle_Palette_Color_();
            RUSS::Mox_Update_();

            fonts::Set_Outline_Color_(177, 177);
            Draw_Race_Photos_(race_display_data);
            RUSS::Mox_Update_();

            Draw_Race_Text_(race_display_data);
            RUSS::Mox_Update_();

            BILL::Get_Active_Players_(0, 0);
            Draw_Relations_Sliders_(race_display_data);
            RUSS::Mox_Update_();

            video::Copy_Off_To_Back_();

            int16_t exit_field;
            int16_t btn_diplomacy;
            int16_t btn_report;
            int16_t btn_ignoring;
            int16_t btn_war;
            int16_t who_cancel_field = 0;

            int16_t state_diplomacy = 0;
            int16_t state_report = 0;
            int16_t state_ignoring = 0;
            int16_t state_war = 0;

            int16_t display_base = Setup_Main_Fields_(&exit_field, &btn_diplomacy, &state_diplomacy, &btn_report, &state_report, &btn_ignoring, &state_ignoring, &btn_war, &state_war);
            RUSS::Mox_Update_();

            Setup_Race_Display_Fields_(race_display_data, display_base);

            who_cancel_field = fields::Add_Hidden_Field_(0, 0, 639, 479, s__005510f0, 0);

            fields::Assign_Auto_Function_(Draw_Race_Screen_, 2);
            RUSS::Mox_Update_();
            Draw_Race_Screen_();
            RUSS::Mox_Update_();

            s_racescrn_mouse_group* active_spy_group = nullptr;
            palette::Clear_Palette_Changes_(0, 255);
            RUSS::Mox_Sync_Update_();
            ERIC::Fast_Fade_In_();
            
            _g_in_spy_mode = 0;
            _g_in_who_mode = 0;

            int16_t active_action_field = 0;
            uint8_t net_cancel_requested = 0;
            uint8_t human_rejected = 0;

            while (true) {
                int16_t input = fields::Get_Input_();
                
                if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                    if (_g_in_spy_mode) {
                        Move_Spy_Group_From_Mouse_(active_spy_group);
                    }
                    Update_Spy_Stuff_(race_display_data);
                    break;
                }

                if (input < 0) {
                    input = -input;
                }

                if (input == 0) {
                    timer::Mark_Time_();
                    Draw_Race_Screen_();
                    RUSS::Mox_Sync_Update_();
                    timer::Release_Time_(2);
                    continue;
                }

                if (input == exit_field) {
                    if (_g_in_spy_mode) {
                        Move_Spy_Group_From_Mouse_(active_spy_group);
                        if (active_spy_group != &agent_group) {
                            race_display_data[selected_race_idx].agent_mission_idx = old_mission_idx;
                        }
                    }
                    Update_Spy_Stuff_(race_display_data);
                    ERIC::Fast_Fade_Out_();
                    MOX2::Put_Auto_Function_(&auto_function);
                    fields::Deactivate_Help_List_();
                    COLONY::Reset_Mouse_Picture_();
                    palette::Set_Palette_Changes_(0, 255);
                    palette::Restore_Palette_();
                    BILL::Turn_Off_Palettes_(0, 255);
                    fields::Enable_Draw_All_Fields_();
                    MOX::_current_screen = 0;
                    return;
                }

                if (input > display_base) {
                    if (_g_in_who_mode) {
                        if (input == who_cancel_field) {
                            break;
                        }

                        int16_t race_display_idx = input - display_base - 1;
                        selected_race_idx = race_display_idx;
                        int16_t target_player_idx = race_display_data[race_display_idx].player_idx;

                        if (active_action_field == btn_ignoring) {
                            MOX::_cur_plyr_ptr->ignoring ^= (uint8_t)(1 << (target_player_idx & 0x1F));
                            break;
                        } else if (active_action_field == btn_report) {
                            JIM::Bill_Picture_To_Mouse_(_g_mouse_seg);
                            ERIC::Fast_Fade_Out_();
                            Update_Spy_Stuff_(race_display_data);
                            RACERPRT::Race_Report_Screen_(target_player_idx);
                            RUSS::Mox_Update_();
                            break;
                        } else if (active_action_field == btn_war) {
                            Update_Spy_Stuff_(race_display_data);
                            if (MOX::_cur_plyr_ptr->treaty[target_player_idx] >= 4) {
                                break;
                            }
                            strcpy(message_buffer, _declare_war_buffer);
                            JIM::Decode_Text_Race_(message_buffer, target_player_idx, 120);
                            if (GENDRAW::Do_Confirmation_Box_(message_buffer) == 1) {
                                MOX::_player[MOX::_PLAYER_NUM].delayed_diplomacy_orders[target_player_idx] = 0;
                            }
                            break;
                        } else if (active_action_field == btn_diplomacy) {
                            Update_Spy_Stuff_(race_display_data);
                            MOX::_cur_plyr_ptr->ignoring &= (uint8_t)~(1 << (target_player_idx & 0x1F));
                            
                            if (MOX::_game_type != 2 && MOX::_game_type != 3) {
                                DIP_SCRN::Diplomacy_Screen_(target_player_idx);
                                break;
                            }

                            if (MOX::_player[target_player_idx].objectives != 100) {
                                _net_state = 1;
                                net_cancel_requested = 0;
                                NETMOX::Net_Request_AI_Diplomacy_((uint8_t)target_player_idx);
                                if (NETMOX::Net_May_Begin_AI_Diplomacy_() == 0) {
                                    if (net_cancel_requested == 0) {
                                        if (NETMOX::No_AI_Diplomacy_Allowed_() == 0) {
                                            while (true) {
                                                timer::Mark_Time_();
                                                fields::Clear_Fields_();
                                                fields::Add_Hidden_Field_(0, 0, 639, 479, s__005510f0, 0);
                                                if (fields::Get_Input_() != 0) {
                                                    net_cancel_requested = 1;
                                                }
                                                Draw_Race_Screen_();
                                                RUSS::Mox_Sync_Update_();
                                                RUSS::Mox_Update_();
                                                timer::Release_Time_(2);
                                                if (NETMOX::Net_May_Begin_AI_Diplomacy_() != 0) break;
                                                if (net_cancel_requested != 0) break;
                                                if (NETMOX::No_AI_Diplomacy_Allowed_() != 0) break;
                                            }
                                        }
                                    }
                                }
                                _net_state = 0;
                                if (net_cancel_requested == 0) {
                                    if (NETMOX::No_AI_Diplomacy_Allowed_() != 0) {
                                        JIM::Get_Text_Message_(s_billtex2_lbx_00556038, 27, message_buffer, 80);
                                        GENDRAW::Message_Box_(message_buffer);
                                    } else {
                                        DIP_SCRN::Diplomacy_Screen_(target_player_idx);
                                        NETMOX::Net_End_AI_Diplomacy_();
                                    }
                                } else {
                                    NETMOX::Net_Cancel_AI_Diplomacy_Request_();
                                    while (NETMOX::Net_Cancel_AI_Diplomacy_Confirmed_() == 0) {
                                        RUSS::Mox_Update_();
                                    }
                                }
                                break;
                            } else {
                                human_rejected = 0;
                                net_cancel_requested = 0;
                                _net_state = 1;
                                NETMOX::Net_Request_Human_Diplomacy_((uint8_t)target_player_idx);
                                if (NETMOX::Net_May_Begin_Human_Diplomacy_() == 0) {
                                    if (net_cancel_requested == 0) {
                                        if (NETMOX::Human_Hit_Next_Turn_(target_player_idx) == 0) {
                                            while (true) {
                                                timer::Mark_Time_();
                                                fields::Clear_Fields_();
                                                fields::Add_Hidden_Field_(0, 0, 639, 479, s__005510f0, 0);
                                                if (fields::Get_Input_() != 0) {
                                                    if (NETMOX::Allowed_To_Cancel_Human_Diplomacy_() != 0) {
                                                        net_cancel_requested = 1;
                                                    }
                                                }
                                                if (NETMOX::Human_Diplomat_Says_Wait_() != 0) {
                                                    _net_state = 2;
                                                }
                                                if (NETMOX::Net_Human_Rejected_Diplomacy_() != 0) {
                                                    human_rejected = 1;
                                                }
                                                Draw_Race_Screen_();
                                                RUSS::Mox_Sync_Update_();
                                                RUSS::Mox_Update_();
                                                timer::Release_Time_(2);
                                                if (human_rejected != 0) break;
                                                if (NETMOX::Net_May_Begin_Human_Diplomacy_() != 0) break;
                                                if (net_cancel_requested != 0) break;
                                                if (NETMOX::Human_Hit_Next_Turn_(target_player_idx) != 0) break;
                                            }
                                        }
                                    }
                                }
                                _net_state = 0;
                                if (NETMOX::Human_Hit_Next_Turn_(target_player_idx) != 0) {
                                    JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 55, message_buffer, 80);
                                    GENDRAW::Message_Box_(message_buffer);
                                    NETMOX::Net_End_Human_Diplomacy_();
                                } else {
                                    if (human_rejected != 0) {
                                        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 56, message_buffer, 80);
                                        GENDRAW::Message_Box_(message_buffer);
                                        NETMOX::Net_End_Human_Diplomacy_();
                                    } else {
                                        if (NETMOX::Net_May_Begin_Human_Diplomacy_() != 0) {
                                            DIP_SCRN::Diplomacy_Screen_(target_player_idx);
                                            NETMOX::Net_End_Human_Diplomacy_();
                                        } else {
                                            NETMOX::Net_Cancel_Human_Diplomacy_Request_();
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    } else {
                        state_diplomacy = 0;
                        state_report = 0;
                        state_ignoring = 0;
                        state_war = 0;

                        int16_t hover_race_idx = 0;
                        s_racescrn_mouse_group* spy_group = Get_Spy_Group_For_Field_(input, &hover_race_idx);
                        
                        if (_g_in_spy_mode) {
                            if (spy_group == nullptr) {
                                spy_group = active_spy_group;
                                if (active_spy_group != &agent_group) {
                                    race_display_data[selected_race_idx].agent_mission_idx = old_mission_idx;
                                }
                            }
                            Move_Spy_Group_From_Mouse_(spy_group);
                            if (_g_mouse_group_p->count == 0) {
                                _g_in_spy_mode = 0;
                                active_spy_group = nullptr;
                            }
                        } else {
                            if (spy_group != nullptr && spy_group->count > 0) {
                                selected_race_idx = hover_race_idx;
                                Move_Spies_Group_(spy_group, _arrow_seg);
                                _g_in_spy_mode = 1;
                                old_mission_idx = race_display_data[selected_race_idx].agent_mission_idx;
                                active_spy_group = spy_group;
                            }
                        }
                        Adjust_Spy_Mission_Data_();
                    }
                } else {
                    if (_g_in_spy_mode) {
                        Move_Spy_Group_From_Mouse_(active_spy_group);
                        if (active_spy_group != &agent_group) {
                            race_display_data[selected_race_idx].agent_mission_idx = old_mission_idx;
                        }
                        if (_g_mouse_group_p->count == 0) {
                            _g_in_spy_mode = 0;
                        }
                    }

                    if (_g_in_who_mode) {
                        JIM::Bill_Picture_To_Mouse_(_g_mouse_seg);
                        state_diplomacy = 0;
                        state_report = 0;
                        state_ignoring = 0;
                        state_war = 0;
                        Setup_Race_Display_Fields_(race_display_data, display_base);
                        who_cancel_field = fields::Add_Hidden_Field_(0, 0, 639, 479, s__005510f0, 0);
                        _g_in_who_mode = 0;
                    } else {
                        if (input == btn_ignoring) {
                            state_diplomacy = 0; state_report = 0; state_war = 0;
                        } else if (input == btn_report) {
                            state_diplomacy = 0; state_ignoring = 0; state_war = 0;
                        } else if (input == btn_diplomacy) {
                            state_report = 0; state_ignoring = 0; state_war = 0;
                        } else if (input == btn_war) {
                            state_report = 0; state_ignoring = 0; state_diplomacy = 0;
                        }

                        active_action_field = input;
                        fields::Enable_Draw_All_Fields_();
                        fields::Draw_Visible_Fields_();
                        fields::Disable_Draw_All_Fields_();
                        fields::Draw_All_Fields_();
                        video::Copy_Off_To_Back_();

                        Setup_Race_Who_Fields_(race_display_data, display_base);
                        who_cancel_field = fields::Add_Hidden_Field_(0, 0, 639, 479, s__005510f0, 0);
                        COLONY::Picture_To_Mouse_(mouse_pic);
                        _g_in_who_mode = 1;
                    }
                }
            }
        }
    }
}