#include "pch.h"

namespace FLT1 {
    void __cdecl Initialize_Scroll_Bar_(s_scroll_bar* sb) {
        int16_t scroll_height;

        sb->visible_rows = 5;
        sb->track_len_px = 0xea /* 234 */;

        scroll_height = SCROLL::Get_Vertical_Scroll_Bar_Height_(sb);
        sb->thumb_height_px = scroll_height;

        sb->last_thumb_center_y = -1;
        sb->type = 0;
        sb->row_step = 1;

        // Division by 2 logic (CDQ, SUB, SAR) matches signed integer division behavior
        sb->thumb_center_y = sb->track_len_px - (scroll_height / 2);
    }

    void __cdecl Draw_Fltscrn_Big_Ship_Icons_(int16_t param_1) {
        int16_t damage_width;
        int16_t damage_y_off;
        int16_t damage_x_off;

        if (param_1 == 0x1d) {
            damage_width = 3;
            damage_y_off = 0x39;
            damage_x_off = 0x39;
        } else {
            damage_x_off = 0x38;
            damage_y_off = 0x39;
            damage_width = 1;
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

            int32_t width_diff = 0x39 - width;
            int16_t half_w_diff = (int16_t)((width_diff - (width_diff >> 31)) >> 1);
            int32_t height_diff = 0x39 - height;
            int16_t half_h_diff = (int16_t)((height_diff - (height_diff >> 31)) >> 1);

            (void)HACCESS::Ship_Owner_(ship_idx);

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
                animate::Draw_(draw_x, draw_y, MOX::_selected_box_seg);
            }

            pic_seg = KEN::Get_Ship_Id_Picture_Seg_(ship_idx);
            animate::Draw_(x + half_w_diff + 1, y + half_h_diff + 2, pic_seg);

            if (HACCESS::Is_Combat_Ship_(ship_idx)) {
                FLEETPOP::Draw_Damage_Bars_(x, y, damage_width, damage_y_off, damage_x_off, ship_idx);
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
                uint8_t color = MOX::_player[owner].color;
                animate::Reset_Animation_Frame_(MOX::_fltscrn_small_ship_icon_seg[color]);
                anim = MOX::_fltscrn_small_ship_icon_seg[color];
            } else {
                int16_t ship_idx = MOX::_ship_node[node_idx].ship_idx;
                int8_t ship_owner = MOX::_ship[ship_idx].owner;
                animate::Reset_Animation_Frame_(MOX::_fltscrn_monster_ship_icon[ship_owner - 8]);
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
        MOX::_fltscrn_ship_icon_selection_list = (int16_t*)allocate::Allocate_Next_Block_((void*)(uintptr_t)MOX::_screen_seg, 0x1f4 /* 500 */);
        MOX::_officer_id_list = (int16_t*)allocate::Allocate_Next_Block_((void*)(uintptr_t)MOX::_screen_seg, 0x86);
        MOX::_scroll_bar = (s_scroll_bar*)allocate::Allocate_Next_Block_((void*)(uintptr_t)MOX::_screen_seg, 0x1d);

        if (MOX::_scroll_bar) {
            memset(MOX::_scroll_bar, 0, 0x1d);
        }

        allocate::Get_Remaining_Block_Space_((void*)(uintptr_t)MOX::_screen_seg);
    }

    void __cdecl Draw_Fleet_Screen_() {
        if (MOX::_draw_fleet_screen_to_back != 0 || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            graphics::Reset_Window_();
            graphics::Fill_(0, 0, 639, 479, 0);
            animate::Draw_(0, 0, MOX::_fleet_background_seg);
            FLT1::Draw_Fltscrn_Small_Ship_Icons_();
            video::Copy_Off_To_Back_();

            if (MOX::_draw_fleet_screen_to_back != 0) {
                MOX::_draw_fleet_screen_to_back = 0;
            }
            if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            }
        }

        if (MOX::_galaxy_map_scanned_star > -1) {
            FLT2::Print_Fltscrn_Scanned_Star_Name_();
        }

        if (MOX::_scanned_big_ship > -1 && MOX::_scanned_big_ship < 250 && MOX::_current_screen == 4) {
            graphics::Set_Window_(15, 282, 320, 465);
            video::Clip_On_();
            FLT2::Print_Scanned_Ship_Data_(MOX::_fltscrn_big_icon[MOX::_scanned_big_ship].ship_idx);
            video::Clip_Off_();
            graphics::Reset_Window_();
        }

        FLT1::Draw_Fltscrn_Relocation_Lines_(4);
        FLT1::Draw_Fltscrn_Ship_Destination_Lines_(4);
        MOVEBOX::Draw_Galaxy_Map_Box_(nullptr, nullptr, 15, 52, 305, 182, 0, 0, 0, 0, 1, -1);

        if (FLT1::_relocate_button_mode != 1) {
            if (mouse::Mouse_Button_() == 1) {
                int16_t icon_limit = MOX::_big_icon_display_rows * (int16_t)MOX::_big_icon_display_columns;
                int16_t start_idx = MOX::_scroll_bar->first_visible_row * (int16_t)MOX::_big_icon_display_columns;

                for (int16_t i = 0; i < icon_limit; ++i) {
                    int16_t icon_idx = start_idx + i;
                    if (icon_idx >= MOX::_n_fltscrn_big_icons) {
                        break;
                    }

                    int16_t input_id = fields::Auto_Input_();
                    if (MOX::_fltscrn_big_icon[icon_idx].button_id == input_id) {
                        if (MAINSCR::_fleet_ship_selection_status == -1) {
                            MAINSCR::_fleet_ship_selection_status = (MOX::_fltscrn_big_icon[icon_idx].selected == 0);
                        }
                        MOX::_fltscrn_big_icon[icon_idx].selected = MAINSCR::_fleet_ship_selection_status;
                        if (MOX::_fltscrn_big_icon[icon_idx].selected == 0) {
                            MOX::_draw_main_screen_to_back = 1;
                        }
                        FLT1::Draw_Box_Around_Scanned_Ship_(icon_idx);
                    }
                }
            }
        }

        if (MOX::_scanned_small_ship > -1) {
            OFFICER::Print_Galmap_Scanned_Ship_(MOX::_scanned_small_ship);
        }

        if (MOX::_small_ship_stack_ptr > -1) {
            FLT1::Draw_Flashing_Small_Ship_Icon_();
        }

        if (MOX::_n_fltscrn_big_icons > 0) {
            if (SCROLL::Draw_Generic_Vertical_Scroll_Bar_(MOX::_scroll_bar, FLT1::Fill_FltScrn_Scroll_Bar_)) {
                MOX::_scanned_big_ship = -1;
            }
        }

        if (MOX::_fltscrn_stack_owner > -1 && MOX::_skip_draw_ships == 0 && MOX::_small_ship_stack_ptr > -1) {
            FLT1::Draw_Fltscrn_Big_Ship_Icons_(4);
        } else {
            MOX::_skip_draw_ships = 0;
        }

        s_animation_header* relocate_anim;
        if (FLT1::_relocate_button_mode == 2) {
            relocate_anim = (s_animation_header*)MOX::_dimmed_relocate_button_seg;
        } else {
            if (FLT1::_relocate_button_mode == 0) {
                animate::Reset_Animation_Frame_(MOX::_relocate_fleet_button_seg);
            } else {
                animate::Set_Animation_Frame_(MOX::_relocate_fleet_button_seg, 1);
            }
            relocate_anim = MOX::_relocate_fleet_button_seg;
        }
        animate::Draw_(441, 380, relocate_anim);

        if (MOX::_add_scrap_button == 0) {
            animate::Draw_(549, 380, (s_animation_header*)MOX::_dimmed_scrap_button_seg);
        }
        if (MOX::_add_all_button == 0) {
            animate::Draw_(348, 380, (s_animation_header*)MOX::_dimmed_all_button_seg);
        }
        if (MOX::_add_officers_button == 0) {
            animate::Draw_(342, 430, (s_animation_header*)MOX::_dimmed_officers_button_seg);
        }

        int32_t next_idx = (int32_t)MOX::_multi_colored_line_start + 1;
        MOX::_multi_colored_line_start = (int16_t)(next_idx % 8);
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
        int16_t btn_next_ship, btn_prev_ship, btn_add_officers, btn_all_ships, btn_scroll_field;

        Add_Fleet_Screen_Fields_(&btn_relocate, &btn_scrap, &btn_close, &btn_scroll_up,
                                 &btn_scroll_down, &btn_next_ship, &btn_prev_ship,
                                 &btn_add_officers, &btn_all_ships, &btn_scroll_field);

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
                                     &btn_add_officers, &btn_all_ships, &btn_scroll_field);
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
                HACCESS::Clear_All_Star_Relocations_(MOX::_PLAYER_NUM);
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
                        sprintf(msg_buf_78, HAROLD::H_Message_(0x78));
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
                    sprintf(msg_buf_77, HAROLD::H_Message_(0x77), MOX::_star[clicked_star_id].name);
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

            if (input == btn_all_ships && MOX::_n_fltscrn_big_icons > 0 &&
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
                    MOX::_temp_star_handle = MOX::_system_star_id;
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

        sprintf(message, format, star);

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
        sprintf(buffer, format, ship->d.name);

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
     * @address 004b2920
     */
    void __cdecl Plunder_Ships_For_Tech_(int16_t* ship_indices, int16_t count, int8_t flag) {
        for (int16_t i = 0; i < count; ++i) {
            // Structure size is 129 bytes (0x81)
            s_ship_data* ship = &MOX::_ship[ship_indices[i]];
            if (ship->d.previous_owner != ship->owner) {
                Get_Technology_From_Scrapped_Ship_(ship, MOX::_PLAYER_NUM, flag);
            }
        }
    }

    /**
     * @address 004b28c0
     */
    int16_t __cdecl Get_Scrap_Ship_Value_() {
        int16_t total_value = 0;

        if (MOX::_n_fltscrn_big_icons > 0) {
            for (int16_t i = 0; i < MOX::_n_fltscrn_big_icons; ++i) {
                if (MOX::_fltscrn_big_icon[i].selected != 0) {
                    total_value += maintain::Ship_Scrap_Value_(&MOX::_ship[MOX::_fltscrn_big_icon[i].ship_idx]);
                }
            }
        }

        return total_value;
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
        int16_t initial_ptr = MOX::_small_ship_stack_ptr;
        int16_t original_owner = HACCESS::Ship_Stack_Owner_(MOX::_ship_stack_start[initial_ptr]);
        int16_t current_ptr = initial_ptr;
        bool done = false;

        do {
            current_ptr++;
            if (MOX::_ship_stack_count <= current_ptr) {
                current_ptr = 0;
            }

            int16_t current_ship_stack = MOX::_ship_stack_start[current_ptr];
            int16_t current_owner = HACCESS::Ship_Stack_Owner_(current_ship_stack);

            if (current_owner == MOX::_PLAYER_NUM && original_owner == MOX::_PLAYER_NUM) {
                if (FLT2::Fltscrn_Ships_To_Be_Displayed_(current_ship_stack) > 0) {
                    MOX::_small_ship_stack_ptr = current_ptr;
                    done = true;
                }
            } else {
                current_owner = HACCESS::Ship_Stack_Owner_(current_ship_stack);
                if (current_owner != MOX::_PLAYER_NUM && original_owner != MOX::_PLAYER_NUM) {
                    if (FLT2::Fltscrn_Ships_To_Be_Displayed_(current_ship_stack) > 0) {
                        MOX::_small_ship_stack_ptr = current_ptr;
                        done = true;
                    }
                }
            }

            if (current_ptr == initial_ptr) {
                done = true;
            }
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
                            MOX::_screen_mouse_list_type = MOX::_fltscrn_big_icon[i].ship_idx;
                        }
                        if (selected_count == 1) {
                            MOX::_temp_star_handle = MOX::_fltscrn_big_icon[i].ship_idx;
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
        MOX::_selected_box_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 17, MOX::_screen_seg);
        MOX::_scanned_box_seg = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_FLEET_LBX, 18, MOX::_screen_seg);

        MOX::_temp_officers_pict = (s_animation_header**)allocate::Allocate_Next_Block_(MOX::_screen_seg, 67 * sizeof(s_animation_header*));
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

    void* __cdecl Load_Fltscrn_Officer_Picture_(int16_t player_num, int16_t leader_idx, void* buffer, uint8_t reload_flag) {
        int16_t entry_num = (int16_t)MOX::_leaders[leader_idx].pict_num + 0x2d;

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
                                          int16_t* all_button_field_id, int16_t* scroll_field_id)
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
            FLT2::Fleet_Screen_Big_Icon_Fields_(scroll_field_id, 605, 86, 14, 234);
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
     * @brief Checks if the given player has at least one officer (leader) assigned.
     *
     * Iterates through the global leaders array to find any leader that belongs
     * to the given player and has an active status (status >= 0).
     *
     * @return true if at least one active officer is found, false otherwise.
     */
    bool __cdecl At_Least_One_Officer_(int16_t player_num) {
        (void)player_num;

        for (int16_t i = 0; i < 67; ++i) {
            if (MOX::_leaders[i].player_index == MOX::_PLAYER_NUM && MOX::_leaders[i].status >= 0) {
                return true;
            }
        }
        return false;
    }

    void __cdecl Get_Technology_From_Scrapped_Ship_(s_ship_data* ship, int16_t player_id, char show_msg) {
        uint16_t tech_apps[25];
        uint16_t got_tech_apps[25];

        for (int i = 0; i < 25; i++) {
            tech_apps[i] = 0xFFFF;
            got_tech_apps[i] = 0;
        }

        int16_t num_new_techs = 0;

        int16_t fuel_id = INITSHIP::Best_Ship_Fuel_(ship->owner);
        if (fuel_id > -1 && fuel_id < 6) {
            uint16_t tech_id = TECHDATA::_fuel[fuel_id].tech_app_id;
            if (!HACCESS::Player_Has_Tech_(tech_id, player_id)) {
                tech_apps[num_new_techs++] = tech_id;
            }
        }

        uint16_t drive_tech_id = TECHDATA::_drives[ship->d.ftl_type].tech_app_id;
        if (!HACCESS::Player_Has_Tech_(drive_tech_id, player_id)) {
            tech_apps[num_new_techs++] = drive_tech_id;
        }

        uint16_t comp_tech_id = TECHDATA::_computers[ship->d.computer_type].tech_app_id;
        if (!HACCESS::Player_Has_Tech_(comp_tech_id, player_id)) {
            tech_apps[num_new_techs++] = comp_tech_id;
        }

        uint16_t armor_tech_id = TECHDATA::_armor[ship->d.armor_type].tech_app_id;
        if (!HACCESS::Player_Has_Tech_(armor_tech_id, player_id)) {
            tech_apps[num_new_techs++] = armor_tech_id;
        }

        for (int i = 0; i < 8; i++) {
            int16_t weapon_type = ship->d.ship_weapon[i].type;
            if (weapon_type > 0) {
                uint16_t weapon_tech_id = TECHDATA::_weapons[weapon_type].tech_app_id;
                if (!HACCESS::Player_Has_Tech_(weapon_tech_id, player_id)) {
                    tech_apps[num_new_techs++] = weapon_tech_id;
                }
            }
        }

        for (int i = 1; i < 39; i++) {
            if (struct_::Test_Bit_Field_(ship->d.special_device_flags, i)) {
                uint16_t special_tech_id = TECHDATA::_specials[i].tech_app_id;
                if (!HACCESS::Player_Has_Tech_(special_tech_id, player_id)) {
                    tech_apps[num_new_techs++] = special_tech_id;
                }
            }
        }

        if (num_new_techs > 0) {
            char lbx_message[200];
            char message[500];

            MISC::Get_LBX_Message_(116, lbx_message);
            sprintf(message, lbx_message, ship->d.name);

            int32_t first_idx = random::Random_(num_new_techs) - 1;
            int16_t num_acquired = 1;
            got_tech_apps[0] = tech_apps[first_idx];

            for (int i = 0; i < num_new_techs; i++) {
                if (i != first_idx) {
                    if (((int32_t)random::Random_(100) - 1) < ((6 - num_acquired) * 5)) {
                        got_tech_apps[num_acquired++] = tech_apps[i];
                    }
                }
            }

            for (int i = 0; i < num_acquired; i++) {
                uint16_t tech_id = got_tech_apps[i];
                if (tech_id != 0) {
                    COLCALC::Player_Gets_Tech_App_(&MOX::_player[player_id], tech_id);

                    strcat(message, s_0_0055110c);
                    strcat(message, s___00556070);
                    strcat(message, TECHDATA::_technology_applications[tech_id].name);
                    strcat(message, s__005569f8);
                }
            }

            strcpy(MOX::_temp_string, ship->d.name);

            if (show_msg != 0) {
                HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, message, 2);
            }
        }
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

    void __cdecl Draw_Fltscrn_Relocation_Lines_(int16_t param_1) {
        s_colors colors;
        int16_t local_a;
        int16_t view_y;
        int16_t view_x;
        int16_t zoom;
        int16_t x1, y1;
        int16_t x2, y2;

        colors.colors[0] = 6;
        colors.colors[1] = 7;
        colors.colors[2] = 7;
        colors.colors[3] = 8;
        colors.colors[4] = 8;
        colors.colors[5] = 9;
        colors.colors[6] = 9;
        colors.colors[7] = 10;

        if (param_1 == 4) {
            local_a = 52 /* 0x34 */;
            view_y = 15 /* 0x0F */;
            view_x = 305 /* 0x131 */;
            zoom = 182 /* 0xB6 */;
        } else {
            local_a = 235 /* 0xEB */;
            view_y = 306 /* 0x132 */;
            view_x = 318 /* 0x13E */;
            zoom = 169 /* 0xA9 */;
        }

        for (int16_t star_idx_00 = 0; star_idx_00 < (int16_t)MOX::_NUM_STARS; ++star_idx_00) {
            if (HACCESS::Star_Has_Relocation_(star_idx_00, MOX::_PLAYER_NUM)) {
                int16_t relocation_target_idx = HACCESS::Relocation_(star_idx_00, MOX::_PLAYER_NUM);

                MOVEBOX::Get_Galaxy_Map_Star_XY_(star_idx_00, view_y, local_a, view_x, zoom, 0, 0, 1, &x1, &y1);
                MOVEBOX::Get_Galaxy_Map_Star_XY_(relocation_target_idx, view_y, local_a, view_x, zoom, 0, 0, 1, &x2, &y2);

                SHIPS::Draw_Ship_Destination_Line_(x2 + 3, y2 + 3, x1, y1, &colors);
            }
        }
    }

    /**
     * @address 004b2c20
     */
    uint8_t __cdecl Okay_To_Set_Relocate_Star2_(int16_t star_idx) {
        uint8_t result;

        result = FLT1::Okay_To_Set_Relocate_Star_(star_idx, 0);
        mouse::Set_Mouse_List_(FLT1::_mouse_list_from, 4);

        return result;
    }

    /**
     * Checks if it is valid to relocate a fleet from one star to another,
     * specifically checking for black hole blockades.
     */
    int8_t __cdecl Scrap_Ships_(int16_t player_idx, int16_t* ship_indices, int16_t num_ships, int8_t interactive) {
        bool give_bc = true;
        bool has_officer = false;

        int16_t star_id = HAROLD::XY_To_Star_Id_(MOX::_ship[ship_indices[0]].x, MOX::_ship[ship_indices[0]].y);

        if (star_id == -1 || !HAROLD::Player_Has_Colony_(player_idx, star_id)) {
            give_bc = false;
            if (interactive != 0) {
                int16_t msg_id;
                if (num_ships == 1) {
                    msg_id = MISC::Is_Vowel_(MOX::_player[player_idx].race_name[0]) ? 123 /* 0x7b */ : 122 /* 0x7a */;
                } else {
                    msg_id = MISC::Is_Vowel_(MOX::_player[player_idx].race_name[0]) ? 125 /* 0x7d */ : 124 /* 0x7c */;
                }

                char msg_buf[300];
                sprintf(msg_buf, HAROLD::H_Message_(msg_id), MOX::_player[player_idx].race_name);

                if (HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, msg_buf, 1) == 0) {
                    char cancel_msg_buf[200];
                    strcpy(cancel_msg_buf, HAROLD::H_Message_(127 /* 0x7f */));
                    HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, cancel_msg_buf, 3);
                    return 0;
                }
            }
        }

        if (num_ships > 0) {
            for (int i = 0; i < num_ships; ++i) {
                int16_t ship_idx = ship_indices[i];
                if (MOX::_ship[ship_idx].officer_index > -1) {
                    if (interactive != 0) {
                        char leader_name_buf[200];
                        strcpy(leader_name_buf, OFFICER::Leader_Name_(MOX::_ship[ship_idx].officer_index, 1));

                        char msg_buf[300];
                        sprintf(msg_buf, HAROLD::H_Message_(126 /* 0x7e */), &MOX::_ship[ship_idx], leader_name_buf);

                        if (HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, msg_buf, 1) == 0) {
                            char cancel_msg_buf[200];
                            strcpy(cancel_msg_buf, HAROLD::H_Message_(127 /* 0x7f */));
                            HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, cancel_msg_buf, 3);
                            return 0;
                        }
                    }
                    has_officer = true;
                }
            }
        }

        int16_t scrap_value = Get_Scrap_Ship_Value_();

        if (interactive != 0 && scrap_value > 0) {
            int16_t msg_id = (num_ships == 1) ? 128 /* 0x80 */ : 129 /* 0x81 */;

            char msg_buf[300];
            sprintf(msg_buf, HAROLD::H_Message_(msg_id), scrap_value);

            if (HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, msg_buf, 1) == 0) {
                char cancel_msg_buf[200];
                strcpy(cancel_msg_buf, HAROLD::H_Message_(127 /* 0x7f */));
                HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, cancel_msg_buf, 3);
                return 0;
            }

            if (give_bc) {
                MOX::_player[player_idx].bc += scrap_value;
                Plunder_Ships_For_Tech_(ship_indices, num_ships, interactive);
            }
        }

        if (has_officer) {
            if (num_ships > 0) {
                for (int i = 0; i < num_ships; ++i) {
                    int16_t ship_idx = ship_indices[i];
                    int16_t officer_idx = MOX::_ship[ship_idx].officer_index;
                    if (officer_idx > -1) {
                        MOX::_leaders[officer_idx].status = 0;
                        MOX::_leaders[officer_idx].location = -1;
                        MOX::_leaders[officer_idx].eta = 0;
                        MOX::_ship[ship_idx].officer_index = -1;
                    }
                }
            }
        }

        if (num_ships > 0) {
            for (int i = 0; i < num_ships; ++i) {
                SHIPS::Kill_Ship_(ship_indices[i]);
            }
        }

        COLCALC::Do_Player_Colony_Calculations_(player_idx);
        return 1;
    }

    void __cdecl Set_Fltscrn_Big_Icons_(int16_t select_flag) {
        if (select_flag != 0) {
            if (MOX::_n_fltscrn_big_icons > 0) {
                for (int16_t i = 0; i < MOX::_n_fltscrn_big_icons; ++i) {
                    MOX::_fltscrn_big_icon[i].selected = 1;
                }
            }
        } else {
            if (MOX::_n_fltscrn_big_icons > 0) {
                for (int16_t i = 0; i < MOX::_n_fltscrn_big_icons; ++i) {
                    MOX::_fltscrn_big_icon[i].selected = 0;
                }
            }
        }
    }

    uint8_t __cdecl Okay_To_Set_Relocate_To_Star_(int16_t src_star_idx, int16_t dst_star_idx) {
        uint8_t result;
        char message[100];
        const char* fmt;

        result = FLT1::Okay_To_Set_Relocate_Star_(dst_star_idx, 1);
        mouse::Set_Mouse_List_(FLT1::_mouse_list_to, 4);

        if (result != 0) {
            // Check if the path from src_star to dst_star is blocked by a black hole
            if (struct_::Test_Bit_Field_(MOX::_star[src_star_idx].black_hole_blocks, dst_star_idx) != 0) {
                fmt = HAROLD::H_Message_(0x82 /* 130 */);

                // Note: passing pointer to s_star_data is equivalent to passing star.name
                // because name is at offset 0 of s_star_data.
                sprintf(message, fmt, MOX::_star[src_star_idx].name, MOX::_star[dst_star_idx].name);

                result = 0;
                HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, message, 3);
            }
        }

        return result;
    }

    bool __cdecl Okay_To_Set_Relocate_Star_(int16_t star_idx, uint8_t prompt_flag)
    {
        int16_t guarded_by_race = 0;
        char msg_buf[80];
        char race_name_buf[20];
        char sprintf_buf[120];

        msg_buf[0] = '\0';
        bool ok_to_relocate = true;

        s_star_data* star = &MOX::_star[star_idx];

        if (star->spectral_class == 6 /* Black Hole */) {
            ok_to_relocate = false;
            if (prompt_flag == 0) {
                strcpy(msg_buf, HAROLD::H_Message_(0x84));
            } else {
                strcpy(msg_buf, HAROLD::H_Message_(0x83));
            }
        } else {
            if (HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, star_idx)) {
                if (HAROLD::Star_Guarded_By_Monster_(star_idx, &guarded_by_race)) {
                    HACCESS::Race_Name_(guarded_by_race, race_name_buf);
                    mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

                    if (prompt_flag != 0) {
                        sprintf(sprintf_buf, HAROLD::H_Message_(0x87), star->name, race_name_buf);
                        ok_to_relocate = (bool)HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, sprintf_buf, 1);
                    } else {
                        ok_to_relocate = false;
                    }
                } else {
                    if (prompt_flag == 0 && !HACCESS::Player_Has_Colony_In_System_(MOX::_PLAYER_NUM, star_idx)) {
                        sprintf(msg_buf, HAROLD::H_Message_(0x88), star->name);
                        ok_to_relocate = false;
                    }
                }
            } else {
                ok_to_relocate = false;
                if (prompt_flag == 0) {
                    strcpy(msg_buf, HAROLD::H_Message_(0x86));
                } else {
                    strcpy(msg_buf, HAROLD::H_Message_(0x85));
                }
            }
        }

        if (!ok_to_relocate && msg_buf[0] != '\0') {
            HAROLD::User_Box_(&MOX::_draw_fleet_screen_to_back, msg_buf, 3);
        }

        return ok_to_relocate;
    }
}