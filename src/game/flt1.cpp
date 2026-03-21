#include "pch.h"

namespace FLT1 {
    void __cdecl Initialize_Scroll_Bar_(s_scroll_bar* sb) {
        int16_t scroll_height;

        sb->visible_rows = 5;
        sb->track_len_px = 0xea /* 234 */;

        scroll_height = SCROLL::Get_Vertical_Scroll_Bar_Height_(sb);
        sb->thumb_height_px = scroll_height;

        sb->last_thumb_center_y = -1;
        sb->field14_0x18 = 0;
        sb->row_step = 1;

        // Division by 2 logic (CDQ, SUB, SAR) matches signed integer division behavior
        sb->thumb_center_y = sb->track_len_px - (scroll_height / 2);
    }

    void __cdecl Draw_Fltscrn_Big_Ship_Icons_(int16_t param_1) {
        int16_t local_6;
        int16_t local_8;
        int16_t local_a;

        if (param_1 == 0x1d) {
            local_a = 3;
            local_6 = 0x39;
            local_8 = 0x39;
        } else {
            local_6 = 0x38;
            local_8 = 0x39;
            local_a = 1;
        }

        int16_t index_global = MOX::_scroll_bar->first_visible_row * (int16_t)MOX::_big_icon_display_columns;

        bool done = false;
        int16_t index = 0;

        while (index_global < MOX::_n_fltscrn_big_icons) {
            if (done) {
                break;
            }

            int16_t ship_idx = MOX::_fltscrn_big_icon[index_global].ship_idx;

            s_animation_header* pic_seg = KEN::Get_Ship_Id_Picture_Seg_(ship_idx);
            int16_t width = animate::Get_Width_(pic_seg);
            int16_t height = animate::Get_Height_(pic_seg);

            int16_t x;
            int16_t y;
            FLT2::Get_Fltscrn_Big_Icon_XY_(index, &x, &y);

            int16_t half_w_diff = (0x39 - width) / 2;
            int16_t half_h_diff = (0x39 - height) / 2;

            HACCESS::Ship_Owner_(ship_idx);

            if (MOX::_scanned_big_ship == index_global && MOX::_current_screen == 4) {
                Draw_Box_Around_Scanned_Ship_(index_global);
            }

            if (MOX::_fltscrn_big_icon[index_global].selected != 0) {
                int16_t draw_x;
                int16_t draw_y;
                if (MOX::_current_screen == 4) {
                    draw_y = y + 1;
                    draw_x = x - 2;
                } else {
                    draw_y = y - 1;
                    draw_x = x - 1;
                }
                animate::Draw_(draw_x, draw_y, (s_animation_header*)MOX::_selected_box_seg);
            }

            pic_seg = KEN::Get_Ship_Id_Picture_Seg_(ship_idx);
            animate::Draw_(x + half_w_diff + 1, y + half_h_diff + 2, pic_seg);

            if (HACCESS::Is_Combat_Ship_(ship_idx)) {
                FLEETPOP::Draw_Damage_Bars_(x, y, local_6, local_8, local_a, ship_idx);
            }

            index++;
            if (MOX::_big_icon_display_rows * MOX::_big_icon_display_columns <= index) {
                done = true;
            }

            index_global++;
        }
    }

    void __cdecl Draw_Fltscrn_Ship_Destination_Lines_(int16_t param_1) {
        bool draw_dest = false;
        int16_t icon_idx = -1;
        int16_t view_x1, view_y1, view_x2, view_y2;
        int16_t out_x, out_y;
        s_colors temp_color;

        s_colors local_green = MAINSCR::_ship_direction_line_green_colors;
        s_colors local_red = MAINSCR::_ship_direction_line_red_colors;

        if (param_1 == 4) {
            if (MOX::_small_ship_stack_ptr > -1) {
                int16_t first_sel = First_Selected_Ship_();
                if (first_sel != -1) {
                    if (MOX::_galaxy_map_scanned_star > -1 && MOX::_galaxy_map_scanned_star < MOX::_NUM_STARS) {
                        icon_idx = F_Ship_Icon_From_Node_Ptr_(MOX::_ship_stack_start[MOX::_small_ship_stack_ptr]);
                        if (icon_idx > -1) {
                            draw_dest = true;
                        }
                    }
                }
            }
            view_x1 = 15;
            view_y1 = 52;
            view_x2 = 305;
            view_y2 = 182;
        } else {
            view_x1 = 306;
            view_y1 = 235;
            view_x2 = 318;
            view_y2 = 169;
        }

        if (draw_dest) {
            if (MOX::_g_ship_move_info.moving == 0 || MOX::_star[MOX::_galaxy_map_scanned_star].spectral_class == 6) {
                temp_color = local_red;
            } else {
                temp_color = local_green;
            }

            MOVEBOX::Get_Galaxy_Map_Star_XY_(
                MOX::_galaxy_map_scanned_star,
                view_x1, view_y1, view_x2, view_y2,
                0, 0, 1,
                &out_x, &out_y
            );

            SHIPS::Draw_Ship_Destination_Line_(
                out_x + 3, out_y + 3,
                MOX::_ship_icon[icon_idx].x, MOX::_ship_icon[icon_idx].y,
                &temp_color
            );
        }

        if (MOX::_ship_icon_count > 0) {
            for (int16_t i = 0; i < MOX::_ship_icon_count; ++i) {
                draw_dest = false;
                int16_t ship_id = HACCESS::Ship_Icon_Ship_Id_(i);
                int16_t owner = HACCESS::Ship_Owner_(ship_id);

                if (owner == MOX::_PLAYER_NUM) {
                    int16_t abs_loc = HAROLD::Absolute_Location_(MOX::_ship[ship_id].location);
                    if (abs_loc != MOX::_NUM_STARS && MOX::_ship[ship_id].location >= 500 && MOX::_ship[ship_id].location < 1500) {
                        draw_dest = true;
                        temp_color = local_green;
                    }
                } else {
                    if (SHIPS::Enemy_Ship_Heading_Toward_Our_Colony_(MOX::_ship_icon[i].node_idx)) {
                        draw_dest = true;
                        temp_color = local_red;
                    }
                }

                if (draw_dest) {
                    int16_t actual_star_id;
                    HAROLD::Actual_Star_Id_(MOX::_ship[ship_id].location, &actual_star_id);

                    MOVEBOX::Get_Galaxy_Map_Star_XY_(
                        actual_star_id,
                        view_x1, view_y1, view_x2, view_y2,
                        0, 0, 1,
                        &out_x, &out_y
                    );

                    SHIPS::Draw_Ship_Destination_Line_(
                        out_x + 3, out_y + 3,
                        MOX::_ship_icon[i].x, MOX::_ship_icon[i].y,
                        &temp_color
                    );
                }
            }
        }
    }

    void __cdecl Draw_Fltscrn_Small_Ship_Icons_() {
        for (int16_t i = MOX::_ship_icon_count - 1; i >= 0; --i) {
            s_animation_header* anim;
            int16_t node_idx = MOX::_ship_icon[i].node_idx;
            int16_t owner = HACCESS::Ship_Stack_Owner_(node_idx);

            if (owner < 8) {
                uint8_t color = static_cast<uint8_t>(MOX::_player[owner].color);
                animate::Reset_Animation_Frame_(MOX::_fltscrn_small_ship_icon_seg[color]);
                anim = MOX::_fltscrn_small_ship_icon_seg[color];
            } else {
                int16_t ship_idx = MOX::_ship_node[node_idx].ship_idx;
                int8_t ship_owner = MOX::_ship[ship_idx].owner;
                // Note: Indexing _shield_div_bitmap with ship_owner >= 8 accesses memory adjacent to it.
                // In the original binary, this effectively resolves to pointers in _fltscrn_monster_ship_icon.
                animate::Reset_Animation_Frame_(reinterpret_cast<s_animation_header**>(MOX::_shield_div_bitmap)[ship_owner]);
                anim = MOX::_fltscrn_monster_ship_icon[ship_owner - 8];
            }

            animate::Draw_(MOX::_ship_icon[i].x, MOX::_ship_icon[i].y, anim);
        }
    }

    void __cdecl Draw_Flashing_Small_Ship_Icon_(void) {
        int16_t node_idx;
        int16_t owner_idx;
        int16_t icon_idx;
        s_animation_header* flashing_anim;

        node_idx = MOX::_ship_stack_start[MOX::_small_ship_stack_ptr];
        owner_idx = HACCESS::Ship_Owner_(MOX::_ship_node[node_idx].ship_idx);

        if (owner_idx < 8) {
            uint8_t color_idx = (uint8_t)MOX::_player[owner_idx].color;
            flashing_anim = MOX::_fltscrn_flashing_ship_pict_seg[color_idx];
        } else {
            flashing_anim = MOX::_fltscrn_flashing_monster_ship_icon[owner_idx - 8];
        }

        icon_idx = FLT1::F_Ship_Icon_From_Node_Ptr_(node_idx);

        if (icon_idx != -1) {
            animate::Draw_(MOX::_ship_icon[icon_idx].x, MOX::_ship_icon[icon_idx].y, flashing_anim);
        }
    }

    void __cdecl Fill_FltScrn_Scroll_Bar_() {
        int16_t thumb_height = MOX::_scroll_bar->thumb_height_px;
        int16_t track_len = MOX::_scroll_bar->track_len_px;
        int16_t thumb_y = MOX::_scroll_bar->thumb_center_y;

        int16_t start_y_offset = track_len - (thumb_height / 2) - thumb_y;

        int16_t x;
        int16_t y;
        int16_t width;
        int16_t anim_x;
        uint8_t color;

        if (MOX::_current_screen == 4) {
            x = 0x25E;
            y = 0x56;
            width = 0xC;
            anim_x = 0x25D;
            color = 0xA6;
        } else {
            x = 0x265;
            y = 0x2D;
            width = 0xA;
            anim_x = 0x264;
            color = 0xAF;
        }

        graphics::Set_Window_(x, y, x + width, y + track_len);
        video::Clip_On_();

        if (width > 0) {
            int16_t y1 = y + start_y_offset + 1;
            int16_t y2 = y + start_y_offset + thumb_height;

            for (int16_t i = 0; i < width; ++i) {
                line::Line_(x, y1, x, y2, color);

                if ((i % 2) == 0) {
                    color--;
                }

                x++;
            }
        }

        video::Clip_Off_();
        graphics::Reset_Window_();

        animate::Draw_(anim_x, y, MOX::_fleet_scroll_anim_seg);
    }

    /**
     * Finds the index of the first ship that is marked as selected in the fleet screen big icons.
     * Returns -1 if no ships are selected.
     */
    int16_t __cdecl First_Selected_Ship_() {
        int16_t result = -1;
        int16_t i = 0;

        if (MOX::_n_fltscrn_big_icons > 0) {
            do {
                // The assembly checks if the result is already found at the start of the loop iteration.
                if (result != -1) {
                    break;
                }

                if (MOX::_fltscrn_big_icon[i].selected != 0) {
                    result = MOX::_fltscrn_big_icon[i].ship_idx;
                }

                i++;
            } while (i < MOX::_n_fltscrn_big_icons);
        }

        return result;
    }

    void __cdecl Set_Small_Ship_Stack_Pointer_(bool check_box) {
        MOX::_small_ship_stack_ptr = -1;
        MOX::_fltscrn_stack_owner = -1;

        if (MOVEBOX::Moveable_Box_Selected_(2) && check_box) {
            MOX::_small_ship_stack_ptr = MOX::_fleet_box_ship_stack;
        } else {
            for (int16_t i = 0; i < MOX::_ship_icon_count; ++i) {
                int16_t owner_player = HACCESS::Ship_Stack_Owner_(MOX::_ship_icon[i].node_idx);
                if (owner_player == MOX::_PLAYER_NUM) {
                    int16_t stack_idx = HACCESS::Ship_Stack_From_Ship_Icon_(i);
                    if (stack_idx > -1) {
                        MOX::_small_ship_stack_ptr = stack_idx;
                        break;
                    }
                }
            }
        }

        if (MOX::_small_ship_stack_ptr > -1) {
            FLT2::Set_Fleet_Screen_Stack_Owner_();
            MOX::_old_fltscrn_stack_owner = MOX::_fltscrn_stack_owner;
        }
    }

    void __cdecl Allocate_Fltscrn_Next_Screen_Seg_() {
        MOX::_fltscrn_big_icon = (s_big_icon*)allocate::Allocate_Next_Block_((void*)(uintptr_t)MOX::_screen_seg, 0xcb2);
        MOX::_fltscrn_icon_selection_status = (int8_t*)allocate::Allocate_Next_Block_((void*)(uintptr_t)MOX::_screen_seg, 0xfa);
        MOX::_fltscrn_ship_icon_selection_list = (int8_t*)allocate::Allocate_Next_Block_((void*)(uintptr_t)MOX::_screen_seg, 0x1f4 /* 500 */);
        MOX::_officer_id_list = (int16_t*)allocate::Allocate_Next_Block_((void*)(uintptr_t)MOX::_screen_seg, 0x86);
        MOX::_scroll_bar = (s_scroll_bar*)allocate::Allocate_Next_Block_((void*)(uintptr_t)MOX::_screen_seg, 0x1d);

        if (MOX::_scroll_bar) {
            memset(MOX::_scroll_bar, 0, 0x1d);
        }

        allocate::Get_Remaining_Block_Space_((void*)(uintptr_t)MOX::_screen_seg);
    }

    void Draw_Fleet_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004b0270})();
    }

    void Fleet_Screen_() {
        int16_t saved_map_scale = MOX::_cur_map_scale;

        union {
            uint32_t full;
            int16_t words[2];
        } scan_val;

        scan_val.words[1] = -1;
        int16_t local_34 = -1;
        int16_t exit_flag = 0;
        bool redraw_skip = false;
        bool reinit_scroll = false;

        int16_t hovered_icon_idx;
        int16_t clicked_icon_idx;
        int16_t scanned_star_id;
        int16_t clicked_star_id;
        int16_t scrap_list[500];
        int16_t scrap_count;

        SYS::Clear_System_Display_Loaded_Flag_();
        MOX::_big_icon_display_columns = 4;
        MOX::_big_icon_display_rows = 5;
        MOX::_scroll_bar = nullptr;
        _relocate_button_mode = 0;
        MOX::_big_icon_display_max_icons = 20;
        MOX::_big_icon_x_offset = 0x15B;
        MOX::_big_icon_y_offset = 0x35;

        Reload_Fleet_Screen_();
        fields::Assign_Auto_Function_(reinterpret_cast<void(*)()>(Draw_Fleet_Screen_), 1);
        MOX::_icon_overlay_x = -1;
        MOX::_icon_overlay_y = -1;
        MOX::_drawing_officer_popup = 0;
        MOX::_n_fltscrn_big_icons_added = 0;
        COLONY::_using_colony_screen_palette = 0;
        MOX::_n_fltscrn_big_icons = 0;
        MOX::_n_fltscrn_rows = 0;
        MOX::_galaxy_star_anim_count = 0;
        MOX::_n_big_icons_selected = 0;
        MOX::_scanned_small_ship = -1;
        MOX::_scanned_big_ship = -1;

        _n_player_ships = HAROLD::N_Player_Ships_(MOX::_PLAYER_NUM);
        FLT::Set_Fltscrn_Small_Ship_Icon_XYs_(15, 52, 305, 182);
        FLT2::Set_Initial_Stack_Pointer_();

        MOX::_old_small_ship_stack_ptr = -1;
        MOX::_global_var = 0;
        MOX::_skip_draw_ships = 1;
        _fleet_screen_bad_ship_id = -1;

        fields::Clear_Fields_();

        int16_t btn_relocate, btn_scrap, btn_close, btn_scroll_up, btn_scroll_down;
        int16_t btn_next_ship, btn_prev_ship, btn_add_officers, btn_big_icons, btn_unknown;

        Add_Fleet_Screen_Fields_(&btn_relocate, &btn_scrap, &btn_close, &btn_scroll_up,
                                 &btn_scroll_down, &btn_next_ship, &btn_prev_ship,
                                 &btn_add_officers, &btn_big_icons, &btn_unknown);

        Initialize_Scroll_Bar_(MOX::_scroll_bar);
        EVANHELP::Set_Fleet_Screen_Help_List_(MOX::_n_fltscrn_big_icons_added);
        fields::Disable_Draw_All_Fields_();
        MOX::_refresh_mode = 2;
        video::Set_Refresh_Stencil_();
        MOX::_draw_fleet_screen_to_back = 1;

        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        ERIC::Fast_Fade_Out_();
        fonts::Load_Palette_(8, 0, 255);
        palette::Set_Palette_Changes_(0, 255);

        if (_n_player_ships > 0) {
            KEN::Load_Player_Ship_Palette_(MOX::_ship[MOX::_fltscrn_big_icon[0].ship_idx].owner);
        }
        palstore::Darken_Palette_(100);
        video::Set_Page_Off_();
        Draw_Fleet_Screen_();

        if (_n_player_ships > 0) {
            animate::Set_Animation_Frame_(reinterpret_cast<s_animation_header*>(_multi_button_support_seg), 1);
            animate::Draw_(0x1A9, 0x1B3, reinterpret_cast<s_animation_header*>(_multi_button_support_seg));
            animate::Set_Animation_Frame_(reinterpret_cast<s_animation_header*>(_multi_button_combat_seg), 1);
            animate::Draw_(0x1E7, 0x1B3, reinterpret_cast<s_animation_header*>(_multi_button_combat_seg));
        }

        palette::Clear_Palette_Changes_(0, 255);
        RUSS::Mox_Sync_Update_();
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        ERIC::Fast_Fade_In_();
        fields::Enable_Draw_All_Fields_();

        do {
            MAINSCR::_fleet_ship_selection_status = -1;
            fields::Clear_Fields_();
            Add_Fleet_Screen_Fields_(&btn_relocate, &btn_scrap, &btn_close, &btn_scroll_up,
                                     &btn_scroll_down, &btn_next_ship, &btn_prev_ship,
                                     &btn_add_officers, &btn_big_icons, &btn_unknown);
            timer::Mark_Time_();
            MOX::_fix_palette = 0;

            int16_t input = fields::Get_Input_();
            scan_val.words[0] = fields::Scan_Input_();

            if (input < 0 && _relocate_button_mode == 1) {
                scan_val.words[1] = -1;
                local_34 = -1;
                _relocate_button_mode = 0;
                mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
            } else if (input == (int16_t)0xFBAF) {
                HACCESS::Player_Can_Order_Ship_(MOX::_PLAYER_NUM);
                char msg_buf_76[200];
                strcpy(msg_buf_76, HAROLD::H_Message_(0x76));
                HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, msg_buf_76, 0);
            } else if (input == (int16_t)0xFC13) {
                if (_merging_relocations == 0) {
                    _relocate_button_mode = 2;
                    _merging_relocations = 1;
                    mouse::Set_Mouse_List_(_mouse_list_all_to, 4);
                } else {
                    _relocate_button_mode = 0;
                    _merging_relocations = 0;
                    mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                }
            }

            if (MOX::_PLAYER_NUM == MOX::_fltscrn_stack_owner) {
                int16_t ret = FLT2::Scan_Fltscrn_Big_Icons_(scan_val.full, input, &hovered_icon_idx, &clicked_icon_idx);
                if (hovered_icon_idx >= 0) {
                    MOX::_scanned_big_ship = hovered_icon_idx;
                    MOX::_galaxy_map_scanned_star = -1;
                    MOX::_scanned_small_ship = -1;
                }
                if (ret == 1) {
                    fields::Deactivate_Help_List_();
                    CMBTDRW1::Detailed_View_Ship_(MOX::_fltscrn_big_icon[clicked_icon_idx].ship_idx);
                    EVANHELP::Set_Fleet_Screen_Help_List_(MOX::_n_fltscrn_big_icons_added);
                    MOX::_draw_fleet_screen_to_back = 1;
                }
            }

            int16_t ret_map = MAINSCR::Scan_Galaxy_Map_Fields_(scan_val.full, input, &scanned_star_id, &clicked_star_id);
            if (scanned_star_id >= 0) {
                MOX::_galaxy_map_scanned_star = scanned_star_id;
                MOX::_scanned_small_ship = -1;
            }

            if (ret_map == 1 && _relocate_button_mode == 1) {
                Cancel_Star_Relocation_(clicked_star_id);
            } else if (ret_map == 0) {
                if (_merging_relocations == 0) {
                    if (_relocate_button_mode == 1) {
                        Star_Relocation_(&scan_val.words[1], &local_34, clicked_star_id);
                    } else if (MOX::_star[clicked_star_id].spectral_class == 6) {
                        char msg_buf_78[80];
                        _sprintf(msg_buf_78, HAROLD::H_Message_(0x78));
                        HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, msg_buf_78, 3);
                    } else {
                        int16_t first_ship = First_Selected_Ship_();
                        if (first_ship >= 0) {
                            if (HACCESS::Player_Can_Order_Ship_(MOX::_PLAYER_NUM, &MOX::_ship[first_ship])) {
                                FLT2::Fltscrn_Move_Ships_(clicked_star_id);
                            } else {
                                Cant_Order_Ship_Message_(&MOX::_ship[first_ship]);
                            }
                        }
                    }
                } else {
                    _merging_relocations = 0;
                    mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                    _relocate_button_mode = 0;
                    HACCESS::Set_All_Star_Relocations_(clicked_star_id, MOX::_PLAYER_NUM);
                    char msg_buf_77[80];
                    _sprintf(msg_buf_77, HAROLD::H_Message_(0x77), MOX::_star[clicked_star_id].name);
                    HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, msg_buf_77, 0);
                    scan_val.words[1] = -1;
                    local_34 = -1;
                }
            }

            int16_t ret_small = Scan_Small_Ship_Icons_(scan_val.full, input, &hovered_icon_idx, &clicked_icon_idx);
            if (ret_small == 0) {
                MOX::_draw_fleet_screen_to_back = 1;
                MOX::_scroll_bar->thumb_center_y = MOX::_scroll_bar->track_len_px;
                if (MOX::_old_small_ship_stack_ptr != MOX::_small_ship_stack_ptr) {
                    if (MOX::_old_small_ship_stack_ptr == -1 || MOX::_small_ship_stack_ptr == -1 ||
                        HACCESS::Ship_Stack_Owner_(MOX::_ship_stack_start[MOX::_old_small_ship_stack_ptr]) != HACCESS::Ship_Stack_Owner_(MOX::_ship_stack_start[MOX::_small_ship_stack_ptr])) {
                        MOX::_skip_draw_ships = 1;
                    }
                    MOX::_scanned_big_ship = -1;
                }
            } else if (ret_small == 4 && hovered_icon_idx >= 0) {
                MOX::_scanned_small_ship = hovered_icon_idx;
                MOX::_galaxy_map_scanned_star = -1;
            }

            if (input == btn_big_icons && MOX::_n_fltscrn_big_icons > 0 &&
                HACCESS::Ship_Owner_(MOX::_fltscrn_big_icon[0].ship_idx) == MOX::_PLAYER_NUM) {
                Set_Fltscrn_Big_Icons_(MOX::_n_big_icons_selected != MOX::_n_fltscrn_big_icons);
            }

            if (input == btn_close) {
                MOX::_current_screen = 0;
                fields::Clear_Fields_();
                fields::Deactivate_Auto_Function_();
                MOX::_galaxy_map_scanned_star = -1;
                OFFICER::Move_From_Limbo_To_Pool_();
                exit_flag = 1;
            }

            if (input == btn_scrap) {
                Build_Scrap_List_(scrap_list, &scrap_count);
                uint8_t scrapped = Scrap_Ships_(MOX::_PLAYER_NUM, scrap_list, scrap_count, 1);
                scan_val.full = (scan_val.full & 0xFFFFFF00) | scrapped;

                int16_t selected_count = N_Fltscrn_Ships_Selected_();
                MOX::_draw_fleet_screen_to_back = 1;
                MOX::_old_small_ship_stack_ptr = -1;

                if (selected_count == MOX::_n_fltscrn_big_icons && (int8_t)scan_val.words[0] != 0) {
                    MOX::_n_fltscrn_big_icons = 0;
                    MOX::_galaxy_map_scanned_star = -1;
                    SHIPS::Build_Ship_Icons_();
                    FLT::Set_Fltscrn_Small_Ship_Icon_XYs_(15, 52, 305, 182);
                    Set_Small_Ship_Stack_Pointer_(0);
                }
                MOX::_scanned_big_ship = -1;
                _n_player_ships = HAROLD::N_Player_Ships_(MOX::_PLAYER_NUM);
            }

            if (input == btn_add_officers) {
                if (MOX::_add_officers_button == 0) {
                    char msg_buf_79[200];
                    strcpy(msg_buf_79, HAROLD::H_Message_(0x79));
                    HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, msg_buf_79, 0);
                } else {
                    MOX::_current_screen = 29;
                    MOX::_return_screen = 4;
                    exit_flag = 1;
                    _screen_data[4] = MOX::_system_star_id;
                }
            }

            if (input == btn_scroll_down) {
                SCROLL::Scroll_Down_One_Item_(MOX::_scroll_bar);
            } else if (input == btn_scroll_up) {
                SCROLL::Scroll_Up_One_Item_(MOX::_scroll_bar);
            }

            if (input == btn_relocate && _relocate_button_mode != 2) {
                if (_relocate_button_mode == 0) {
                    mouse::Set_Mouse_List_(_mouse_list_from, 4);
                    _relocate_button_mode = 1;
                } else if (_relocate_button_mode == 1) {
                    mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                    scan_val.words[1] = -1;
                    _relocate_button_mode = 0;
                    local_34 = -1;
                }
            }

            if (input == btn_next_ship || input == (int16_t)0xFC16) {
                int16_t old_ptr = MOX::_small_ship_stack_ptr;
                Next_Ship_Icon_();
                if (old_ptr != MOX::_small_ship_stack_ptr) {
                    reinit_scroll = true;
                    MOX::_scroll_bar->thumb_center_y = -1;
                    MOX::_draw_fleet_screen_to_back = 1;
                    MOX::_skip_draw_ships = 1;
                    MOX::_scanned_big_ship = -1;
                }
            }

            if (input == btn_prev_ship || input == (int16_t)0xFC17) {
                int16_t old_ptr = MOX::_small_ship_stack_ptr;
                Previous_Ship_Icon_();
                if (old_ptr != MOX::_small_ship_stack_ptr) {
                    MOX::_scanned_big_ship = -1;
                    MOX::_scroll_bar->thumb_center_y = -1;
                    MOX::_draw_fleet_screen_to_back = 1;
                    MOX::_skip_draw_ships = 1;
                    reinit_scroll = true;
                }
            }

            if (input == _support_filter_button || input == _combat_filter_button) {
                if (input == _support_filter_button) {
                    if (MOX::_fltscrn_support_button_status == 0 && MOX::_fltscrn_combat_button_status == 0) {
                        MOX::_fltscrn_combat_button_status = 1;
                    }
                } else {
                    if (MOX::_fltscrn_support_button_status == 0 && MOX::_fltscrn_combat_button_status == 0) {
                        MOX::_fltscrn_support_button_status = 1;
                    }
                }

                if (MOX::_small_ship_stack_ptr >= 0 && MOX::_small_ship_stack_ptr < MOX::_ship_stack_count) {
                    if (FLT2::Fltscrn_Ships_To_Be_Displayed_(MOX::_ship_stack_start[MOX::_small_ship_stack_ptr]) == 0) {
                        Next_Ship_Icon_();
                    }
                }

                redraw_skip = true;
                MOX::_scroll_bar->thumb_center_y = MOX::_scroll_bar->track_len_px;
                MOX::_draw_fleet_screen_to_back = 1;
                MOX::_skip_draw_ships = 1;
                MOX::_scroll_bar->first_visible_row = -1;
            }

            if (reinit_scroll) {
                Initialize_Scroll_Bar_(MOX::_scroll_bar);
                reinit_scroll = false;
            }

            EVANHELP::Set_Fleet_Screen_Help_List_(MOX::_n_fltscrn_big_icons_added);

            if (MOX::_fix_palette != 0) {
                int16_t owner = HACCESS::Ship_Owner_(MOX::_ship_node[MOX::_ship_stack_start[MOX::_small_ship_stack_ptr]].ship_idx);
                FLT2::Fix_Palette_(owner);
            }

            if (exit_flag == 0) {
                if (MOX::_small_ship_stack_ptr >= 0) {
                    MOX::_n_big_icons_selected = Update_Selection_Flags_();
                }
                if (redraw_skip) {
                    redraw_skip = false;
                } else {
                    video::Set_Page_Off_();
                    Draw_Fleet_Screen_();
                    RUSS::Mox_Sync_Update_();
                }
            }

            timer::Release_Time_(1);

        } while (exit_flag == 0);

        if (MOVEBOX::Moveable_Box_Selected_(2) != 0) {
            MOX::_fleet_box_ship_stack = MOX::_small_ship_stack_ptr;
            if (MOX::_small_ship_stack_ptr == -1) {
                HACCESS::Clear_Fleet_Box_Selected_Ship_Status_();
                MOVEBOX::Vanish_Fleet_Movement_Box_();
            }
        }

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        MOX::_cur_map_scale = saved_map_scale;
        MOX::_reloaded_ship_palette = 0;
    }

    int16_t __cdecl Scan_Small_Ship_Icons_(int16_t param_1, int16_t param_2, int16_t* param_3, int16_t* param_4) {
        int16_t result = -1;
        bool found = false;

        *param_3 = -1;
        *param_4 = -1;

        for (int16_t i = 0; i < MOX::_ship_icon_count; ++i) {
            if (found) {
                break;
            }
            if (MOX::_ship_icon[i].stack_id == param_2) {
                result = 0;
                SHIPS::Overlapped_Ship_Icon_Button_(&i, 7, 4);
                *param_4 = i;
                MOX::_small_ship_stack_ptr = HACCESS::Ship_Stack_From_Ship_Icon_(i);

                if (MOX::_old_small_ship_stack_ptr != MOX::_small_ship_stack_ptr) {
                    MOX::_old_small_ship_stack_ptr = -1;
                }

                found = true;
            }
        }

        if (result == -1) {
            found = false;
            for (int16_t i = 0; i < MOX::_ship_icon_count; ++i) {
                if (found) {
                    break;
                }
                if (MOX::_ship_icon[i].stack_id == param_1) {
                    result = 4;
                    found = true;
                    *param_3 = i;
                }
            }
        }

        return result;
    }

    void __cdecl Cancel_Star_Relocation_(int16_t star_idx) {
        char message[100];
        char* format;
        s_star_data* star;

        star = &MOX::_star[star_idx];

        format = const_cast<char*>(HAROLD::H_Message_(137));

        _sprintf(message, format, star);

        HACCESS::Clear_Star_Relocation_(star_idx, MOX::_PLAYER_NUM);

        HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, message, 3);
    }

    void __cdecl Star_Relocation_(int16_t* star_from_ptr, int16_t* star_to_ptr, int16_t star_idx) {
        if (*star_from_ptr == -1) {
            if (Okay_To_Set_Relocate_Star2_(star_idx)) {
                *star_from_ptr = star_idx;
                mouse::Set_Mouse_List_(_mouse_list_to, 4);
            }
        } else {
            if (Okay_To_Set_Relocate_To_Star_(*star_from_ptr, star_idx)) {
                *star_to_ptr = star_idx;
                mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

                int16_t star_from = *star_from_ptr;
                if (*star_to_ptr == star_from) {
                    Cancel_Star_Relocation_(star_from);
                } else {
                    HACCESS::Set_Relocation_(star_from, MOX::_PLAYER_NUM, star_idx);
                }

                mouse::Set_Mouse_List_(_mouse_list_from, 4);
                *star_from_ptr = -1;
                *star_to_ptr = -1;
            }
        }
    }

    void __cdecl Cant_Order_Ship_Message_(const s_ship_data* ship) {
        char buffer[80];
        const char* format;

        format = HAROLD::H_Message_(0x75 /* HESTR_075_S_CANNOT_RECEIVE_ORDER */);
        _sprintf(buffer, format, ship->d.name);

        HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, buffer, 0);

        _fleet_screen_bad_ship_id = -1;
    }

    /**
     * @address 004afd20
     */
    void __cdecl Build_Scrap_List_(int16_t* scrap_list, int16_t* list_count) {
        *list_count = 0;

        if (MOX::_n_fltscrn_big_icons > 0) {
            for (int16_t i = 0; i < (int16_t)MOX::_n_fltscrn_big_icons; ++i) {
                // Size of s_big_icon is 13 (0xD) bytes
                s_big_icon& icon = MOX::_fltscrn_big_icon[i];

                if (icon.ship_idx > -1 && icon.ship_idx < 500 && icon.selected != 0) {
                    scrap_list[*list_count] = icon.ship_idx;
                    *list_count += 1;
                }
            }
        }
    }

    /**
     * @address 004b30f0
     */
    int16_t __cdecl N_Fltscrn_Ships_Selected_() {
        int16_t selected_count = 0;
        int16_t start_index = MOX::_scroll_bar->first_visible_row * (int16_t)MOX::_big_icon_display_columns;

        for (int16_t i = start_index; i < MOX::_n_fltscrn_big_icons; ++i) {
            if (MOX::_fltscrn_big_icon[i].selected != 0) {
                selected_count++;
            }
        }

        return selected_count;
    }

    void __cdecl Next_Ship_Icon_() {
        uint16_t initial_ptr = (uint16_t)MOX::_small_ship_stack_ptr;
        int16_t original_owner = HACCESS::Ship_Stack_Owner_(MOX::_ship_stack_start[initial_ptr]);
        uint16_t current_ptr = initial_ptr + 1;
        bool done = false;

        do {
            if (current_ptr >= MOX::_ship_stack_count) {
                current_ptr = 0;
            }

            int16_t current_ship_stack = MOX::_ship_stack_start[current_ptr];
            int16_t current_owner = HACCESS::Ship_Stack_Owner_(current_ship_stack);

            if (current_owner == MOX::_PLAYER_NUM) {
                if (original_owner == MOX::_PLAYER_NUM) {
                    if (FLT2::Fltscrn_Ships_To_Be_Displayed_(current_ship_stack) > 0) {
                        MOX::_small_ship_stack_ptr = (int16_t)current_ptr;
                        done = true;
                    }
                }
            } else {
                if (HACCESS::Ship_Stack_Owner_(current_ship_stack) != MOX::_PLAYER_NUM &&
                    original_owner != MOX::_PLAYER_NUM) {
                    if (FLT2::Fltscrn_Ships_To_Be_Displayed_(current_ship_stack) > 0) {
                        MOX::_small_ship_stack_ptr = (int16_t)current_ptr;
                        done = true;
                    }
                }
            }

            if (current_ptr == initial_ptr) {
                done = true;
            }

            current_ptr++;
        } while (!done);
    }

    void __cdecl Previous_Ship_Icon_() {
        int16_t initial_ptr = MOX::_small_ship_stack_ptr;
        int16_t initial_owner = HACCESS::Ship_Stack_Owner_(MOX::_ship_stack_start[initial_ptr]);
        int16_t current_ptr = initial_ptr;
        bool found = false;

        do {
            current_ptr--;
            if (current_ptr < 0) {
                current_ptr = MOX::_ship_stack_count - 1;
            }

            int16_t ship_idx = MOX::_ship_stack_start[current_ptr];
            int16_t owner = HACCESS::Ship_Stack_Owner_(ship_idx);

            bool is_match = false;
            if (owner == MOX::_PLAYER_NUM) {
                if (MOX::_PLAYER_NUM == initial_owner) {
                    is_match = true;
                }
            } else {
                // Redundant call preserved from original binary behavior
                owner = HACCESS::Ship_Stack_Owner_(ship_idx);
                if (owner != MOX::_PLAYER_NUM && MOX::_PLAYER_NUM != initial_owner) {
                    is_match = true;
                }
            }

            if (is_match) {
                if (FLT2::Fltscrn_Ships_To_Be_Displayed_(ship_idx) > 0) {
                    MOX::_small_ship_stack_ptr = current_ptr;
                    found = true;
                }
            }

            if (initial_ptr == current_ptr) {
                found = true;
            }

        } while (!found);
    }

    // Suggested name: Update_Selection_Flags
    int16_t __cdecl Update_Selection_Flags_() {
        int16_t selected_count = 0;
        int16_t first_node_idx = MOX::_ship_stack_start[MOX::_small_ship_stack_ptr];
        int16_t ship_idx = MOX::_ship_node[first_node_idx].ship_idx;

        if (HACCESS::Ship_Owner_(ship_idx) == MOX::_PLAYER_NUM) {
            if (MOX::_n_fltscrn_big_icons > 0) {
                for (int16_t i = 0; i < MOX::_n_fltscrn_big_icons; ++i) {
                    int8_t is_selected = MOX::_fltscrn_big_icon[i].selected;
                    MOX::_fltscrn_icon_selection_status[i] = is_selected;

                    if (is_selected != 0) {
                        if (selected_count == 0) {
                            _screen_data[3] = MOX::_fltscrn_big_icon[i].ship_idx;
                        }
                        if (selected_count == 1) {
                            _screen_data[4] = MOX::_fltscrn_big_icon[i].ship_idx;
                        }
                        selected_count++;
                    }
                }
            }
        }

        return selected_count;
    }

    void __cdecl Draw_Box_Around_Scanned_Ship_(int16_t icon_idx) {
        s_big_icon* icon = &MOX::_fltscrn_big_icon[icon_idx];
        int16_t x = icon->screen_x;
        int16_t y = icon->screen_y;

        if (MOX::_current_screen == 4 /* SCR_FLEET */) {
            animate::Draw_(x - 3, y - 1, MOX::_scanned_box_seg);
        } else {
            animate::Draw_(x - 2, y - 2, MOX::_scanned_box_seg);
        }
    }

    /**
     * Searches the ship icon array for a record matching the provided node index.
     * Returns the index of the first match found, or -1 if no match exists.
     */
    int16_t __cdecl F_Ship_Icon_From_Node_Ptr_(int16_t node_idx) {
        int16_t found_index = -1;

        for (int16_t i = 0; i < MOX::_ship_icon_count; ++i) {
            if (MOX::_ship_icon[i].node_idx == node_idx) {
                found_index = i;
                break;
            }
        }

        return found_index;
    }

    void __cdecl Reload_Fleet_Screen_(void) {
        int16_t matching_officer_count = 0;

        MAINSCR::Allocate_First_Screen_Seg_(5);

        MOX::_fleet_background_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 0, MOX::_screen_seg);
        MOX::_fleet_scroll_anim_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 1, MOX::_screen_seg);
        MOX::_up_scroll_button_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 2, MOX::_screen_seg);
        MOX::_prev_fleet_button_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 3, MOX::_screen_seg);
        MOX::_next_fleet_button_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 4, MOX::_screen_seg);
        MOX::_down_scroll_button_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 5, MOX::_screen_seg);
        MOX::_fleet_all_button_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 6, MOX::_screen_seg);
        MOX::_relocate_fleet_button_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 7, MOX::_screen_seg);
        MOX::_scrap_button_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 8, MOX::_screen_seg);
        FLT1::_multi_button_support_seg = farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 9, MOX::_screen_seg);
        FLT1::_multi_button_combat_seg = farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 10, MOX::_screen_seg);
        MOX::_officers_button_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 11, MOX::_screen_seg);
        MOX::_fleet_return_button_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 12, MOX::_screen_seg);
        MOX::_dimmed_all_button_seg = farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 13, MOX::_screen_seg);
        MOX::_dimmed_relocate_button_seg = farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 14, MOX::_screen_seg);
        MOX::_dimmed_scrap_button_seg = farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 15, MOX::_screen_seg);
        MOX::_dimmed_officers_button_seg = farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 16, MOX::_screen_seg);
        MOX::_selected_box_seg = farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 17, MOX::_screen_seg);
        MOX::_scanned_box_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 18, MOX::_screen_seg);

        MOX::_temp_officers_pict = (s_animation_header**)allocate::Allocate_Next_Block_(MOX::_screen_seg, 67 /* 0x43 */);
        FLT1::Allocate_Fltscrn_Next_Screen_Seg_();

        for (int16_t i = 0; i < 67 /* 0x43 */; ++i) {
            s_leader_data* leader = &MOX::_leaders[i];
            if (leader->player_index == MOX::_PLAYER_NUM && leader->status == 1 /* Status: Active? */ && leader->type == 0 /* Type: Ship Officer? */) {
                MOX::_temp_officers_pict[matching_officer_count] = (s_animation_header*)FLT1::Load_Fltscrn_Officer_Picture_(MOX::_PLAYER_NUM, i, MOX::_screen_seg, 0);
                MOX::_officer_id_list[matching_officer_count] = i;
                matching_officer_count++;
            }
        }

        FLT1::Reload_Small_Ship_Icons_();
        FLT1::Load_Galaxy_Stars_();
    }

    void* __cdecl Load_Fltscrn_Officer_Picture_(int16_t player_num, int16_t leader_idx, void* buffer, int16_t reload_flag) {
        int16_t entry_num = (int16_t)(uint8_t)MOX::_leaders[leader_idx].pict_num + 0x2d;

        if (reload_flag != 0) {
            return farload::Far_Reload_(PTR_s_FLEET_LBX_005584a8, entry_num, buffer);
        } else {
            return farload::Far_Reload_Next_(PTR_s_FLEET_LBX_005584a8, entry_num, buffer);
        }
    }

    void __cdecl Add_Fleet_Screen_Fields_(int16_t* relocate_fleet_field_id, int16_t* scrap_button_field_id, 
                                          int16_t* fleet_return_button_field_id, int16_t* up_scroll_button_field_id, 
                                          int16_t* down_scroll_button_field_id, int16_t* next_fleet_button_field_id, 
                                          int16_t* prev_fleet_button_field_id, int16_t* officers_button_field_id, 
                                          int16_t* all_button_field_id, int16_t* big_icon_field_ids)
    {
        int16_t temp_field_id;

        if (MOX::_n_big_icons_selected > 0 && MOX::_PLAYER_NUM == MOX::_fltscrn_stack_owner && FLT1::_relocate_button_mode != 1) {
            MOX::_add_scrap_button = 1;
            *scrap_button_field_id = fields::Add_Button_Field_(549, 380, s_empty, MOX::_scrap_button_seg, s_S, 40);
        } else {
            MOX::_add_scrap_button = 0;
            *scrap_button_field_id = -1000;
        }

        if (MOX::_n_fltscrn_big_icons > 0 && MOX::_PLAYER_NUM == MOX::_fltscrn_stack_owner && FLT1::_relocate_button_mode != 1) {
            MOX::_add_all_button = 1;
            *all_button_field_id = fields::Add_Button_Field_(348, 380, s_empty, MOX::_fleet_all_button_seg, s_A, 40);
        } else {
            MOX::_add_all_button = 0;
            *all_button_field_id = -1000;
        }

        *fleet_return_button_field_id = fields::Add_Button_Field_(556, 430, s_empty, MOX::_fleet_return_button_seg, s_space, 40);
        
        *up_scroll_button_field_id = -1000;
        *down_scroll_button_field_id = -1000;
        *prev_fleet_button_field_id = -1000;
        *next_fleet_button_field_id = -1000;

        if (FLT1::_n_player_ships > 0) {
            if (MOX::_n_fltscrn_big_icons > 20) {
                *up_scroll_button_field_id = fields::Add_Button_Field_(606, 59, s_empty, MOX::_up_scroll_button_seg, s_minus, 40);
                *down_scroll_button_field_id = fields::Add_Button_Field_(605, 325, s_empty, MOX::_down_scroll_button_seg, s_plus, 40);
            }
            *prev_fleet_button_field_id = fields::Add_Button_Field_(19, 249, s_empty, MOX::_prev_fleet_button_seg, s_comma, 40);
            *next_fleet_button_field_id = fields::Add_Button_Field_(283, 249, s_empty, MOX::_next_fleet_button_seg, s_dot, 40);
        }

        MOX::_scroll_bar->dn_btn = *down_scroll_button_field_id;
        MOX::_scroll_bar->up_btn = *up_scroll_button_field_id;

        if (FLT1::_relocate_button_mode == 2) {
            FLT1::_relocate_button_mode = 2;
            *relocate_fleet_field_id = 0;
        } else {
            int h = animate::Get_Height_(MOX::_relocate_fleet_button_seg);
            int w = animate::Get_Width_(MOX::_relocate_fleet_button_seg);
            *relocate_fleet_field_id = fields::Add_Hidden_Field_(441, 380, w + 441, h + 380, s_R, 41);
        }

        bool has_officer = FLT1::At_Least_One_Officer_(MOX::_PLAYER_NUM);
        if (has_officer) {
            *officers_button_field_id = fields::Add_Button_Field_(342, 430, s_empty, MOX::_officers_button_seg, s_L, 40);
            MOX::_add_officers_button = 1;
        } else {
            int h = animate::Get_Height_(MOX::_officers_button_seg);
            int w = animate::Get_Width_(MOX::_officers_button_seg);
            *officers_button_field_id = fields::Add_Hidden_Field_(342, 430, w + 342, h + 430, s_empty, 41);
            MOX::_add_officers_button = 0;
        }

        FLT2::Add_Fltscrn_Small_Icon_Fields_();

        if (MOX::_small_ship_stack_ptr > -1) {
            FLT2::Fleet_Screen_Big_Icon_Fields_(big_icon_field_ids, 605, 86, 14, 234);
            MOX::_scroll_bar->total_rows = (int16_t)MOX::_n_fltscrn_rows;
        }

        MOVEBOX::Add_Galaxy_Map_Fields_2_(15, 52, 305, 182, 0, 0, 0, 0, 1, &temp_field_id, MOX::_galaxy_map_star_field, &MOX::_galaxy_map_grid_field);

        MAINSCR::_debug_field = fields::Add_Hidden_Field_(0, 470, 10, 479, s_empty, 41);

        if (FLT1::_n_player_ships > 0) {
            FLT1::_support_filter_button = fields::Add_Radio_Button_Field_(425, 435, s_empty, (s_animation_header*)FLT1::_multi_button_support_seg, &MOX::_fltscrn_support_button_status, s_U, 41);
            FLT1::_combat_filter_button = fields::Add_Radio_Button_Field_(487, 435, s_empty, (s_animation_header*)FLT1::_multi_button_combat_seg, &MOX::_fltscrn_combat_button_status, s_C, 41);
        } else {
            FLT1::_support_filter_button = -1000;
            FLT1::_combat_filter_button = -1000;
        }

        fields::Add_Hidden_Field_(0, 0, 639, 479, s_empty, 0);
    }

    void __cdecl Reload_Small_Ship_Icons_() {
        int16_t entry_idx = 19;

        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);

        for (int i = 0; i < 8; ++i) {
            int16_t current_entry = entry_idx++;

            MOX::_fltscrn_small_ship_icon_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(
                PTR_s_FLEET_LBX_005584a8,
                (int)current_entry,
                MOX::_screen_seg
            );

            MOX::_fltscrn_flashing_ship_pict_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(
                PTR_s_FLEET_LBX_005584a8,
                (int)current_entry,
                MOX::_screen_seg
            );
        }

        for (int i = 0; i < 7; ++i) {
            int16_t current_entry = entry_idx++;

            MOX::_fltscrn_monster_ship_icon[i] = (s_animation_header*)farload::Far_Reload_Next_(
                PTR_s_FLEET_LBX_005584a8,
                (int)current_entry,
                MOX::_screen_seg
            );

            MOX::_fltscrn_flashing_monster_ship_icon[i] = (s_animation_header*)farload::Far_Reload_Next_(
                PTR_s_FLEET_LBX_005584a8,
                (int)current_entry,
                MOX::_screen_seg
            );
        }

        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);
    }

    /**
     * @address 004b2210
     */
    void __cdecl Load_Galaxy_Stars_(void) {
        int lbx_entry = 0x22 /* 34 */;

        for (int i = 0; i < 11; ++i) {
            MOX::_fleet_galaxy_star_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(
                PTR_s_FLEET_LBX_005584a8,
                lbx_entry++,
                MOX::_screen_seg
            );
        }
    }
}