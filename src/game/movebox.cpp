#include "pch.h"

namespace MOVEBOX {
    void __cdecl Draw_Galaxy_Map_Box_(s_animation_header* anim1, s_animation_header* unused_anim2,
                                    int16_t x_base, int16_t y_base,
                                    int16_t width, int16_t height,
                                    int16_t map_x_off, int16_t map_y_off,
                                    int16_t x_adj, int16_t y_adj,
                                    int16_t view_mode, int16_t exit_flag) {
        (void)unused_anim2;

        s_colors font_colors;
        for (int32_t i = 2; i < 8; ++i) {
            font_colors.colors[i] = (uint8_t)(i + 100);
        }

        int16_t y_start = y_base + y_adj;
        int16_t x_start = x_base + x_adj;

        int32_t scale_x = 506000 / width;
        int32_t scale_y = 400000 / height;

        if (exit_flag == 1) return;

        if (exit_flag > -1) {
            int16_t box_w = width;
            int16_t box_h = height;
            if (COLONY::_using_colony_screen_palette == 0) {
                box_w = MOX::_moveable_box[1].width;
                box_h = MOX::_moveable_box[1].height;
            }

            if (view_mode != 4) {
                if (anim1 == nullptr || COLONY::_using_colony_screen_palette != 0) {
                    graphics::Set_Window_(x_start, y_start, x_start + box_w, y_start + box_h);
                    video::Clip_On_();
                    if (MOX::_current_screen != SCREEN_RACE && COLONY::_using_colony_screen_palette == 0) {
                        graphics::Fill_(x_start, y_start, x_start + box_w, y_start + box_h, 0);
                    }
                    video::Clip_Off_();
                    graphics::Reset_Window_();
                } else {
                    for (int32_t i = 0; i < 8; ++i) {
                        font_colors.colors[i] = (i == 0) ? 0x62 : (uint8_t)(i + 100);
                    }
                    if (MOX::_current_screen == SCREEN_MAIN) {
                        fonts::Set_Font_Spacing_(2);
                        fonts::Set_Font_Style_Shadow_Down_(3, &font_colors);
                        fonts::Set_Outline_Color_(0x62, 0x65);
                        const char* msg = HAROLD::H_Message_(0xF3);
                        fonts::Print_Centered_(x_start + 87, y_start + 13, msg);
                        MISC::Set_Font_Default_Spacing_();
                    }
                }
            }
        }

        graphics::Set_Window_(map_x_off + x_start, map_y_off + y_start, 
                              map_x_off + width + x_start, map_y_off + height + y_start);
        video::Clip_On_();

        if (MOX::_NUM_STARS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                int32_t sx = x_start + map_x_off + ((MOX::_star[i].x * 1000 / MOX::_max_map_scale) * 10) / scale_x;
                int32_t sy = y_start + map_y_off + ((MOX::_star[i].y * 1000 / MOX::_max_map_scale) * 10) / scale_y;

                int8_t owner = MOX::_star[i].owner;
                uint16_t color_idx;

                if (MOX::_star[i].spectral_class == STAR_CLASS_BLACK_HOLE) {
                    color_idx = COLONY::_using_colony_screen_palette ? 9 : 10;
                } else if (MOX::_star[i].spectral_class == STAR_CLASS_DWARF) {
                    color_idx = 8;
                } else if (owner < 0 || owner >= MAX_PLAYERS) {
                    if (HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, i) || owner == -1 || owner == -2) {
                        color_idx = 8;
                    } else {
                        color_idx = 0;
                    }
                } else {
                    color_idx = (uint8_t)MOX::_player[owner].color;
                }

                if (view_mode == 0) {
                    animate::Draw_(sx - 1, sy - 1, MOX::_star_colors_seg[color_idx]);
                } else if (view_mode == 1 || view_mode == 4) {
                    animate::Reset_Animation_Frame_(MOX::_fleet_galaxy_star_seg[color_idx]);
                    animate::Draw_(sx - 2, sy - 2, MOX::_fleet_galaxy_star_seg[color_idx]);
                } else if (view_mode == 2) {
                    int16_t scanned = MOX::_scanned_field;
                    if (scanned >= 1000) scanned -= 1000;
                    s_animation_header* summ_anim = MOX::_planet_summary_galaxy_star_seg[color_idx];
                    if (i == scanned) {
                        animate::Set_Animation_Frame_(summ_anim, (int)MOX::_galaxy_star_anim_count);
                        int32_t count = animate::Get_Animation_Frame_Count_(summ_anim);
                        MOX::_galaxy_star_anim_count = (uint8_t)((MOX::_galaxy_star_anim_count + 1) % count);
                    } else {
                        animate::Reset_Animation_Frame_(summ_anim);
                    }
                    animate::Draw_(sx - 2, sy - 2, summ_anim);
                } else if (view_mode == 3) {
                    if (MOX::_galaxy_map_scanned_star != i) {
                        animate::Reset_Animation_Frame_(MOX::_colony_galaxy_star_seg[color_idx]);
                    }
                    animate::Draw_(sx - 1, sy - 1, MOX::_colony_galaxy_star_seg[color_idx]);
                }
            }
        }

        video::Clip_Off_();
        graphics::Reset_Window_();

        if (MOX::_current_screen == SCREEN_FLEET) {
            if (MOX::_galaxy_map_scanned_star > -1) {
                Do_Fleet_Screen_Stuff_(map_x_off, map_y_off, scale_x, scale_y);
            }
            return;
        }

        if (MOX::_current_screen == SCREEN_OFFICERS) {
            Do_Officer_Screen_Stuff_(map_x_off, map_y_off, scale_x, scale_y);
        }
    }

    int16_t __cdecl Moveable_Box_Selected_(int16_t box_index) {
        return (int16_t)(
            MOX::_box_off_flag[box_index] < MOX::_moveable_box[box_index].id
        );
    }

    void __cdecl Vanish_Fleet_Movement_Box_() {
        MOX::_moveable_box[2].id = MOX::_box_off_flag[2];

        if (MOX::_fleet_box.count > 0) {
            for (int16_t i = 0; i < MOX::_fleet_box.count; ++i) {
                MOX::_fleet_movement_icon[i].field = -1000;
            }
        }

        MOX::_fleet_movement_all_button = -1000;
        MOX::_fleet_box_colonize_button = -1000;
        MOX::_moveable_box[2].drag_field = -1000;
        MOX::_moveable_box[2].close_btn = -1000;

        MOX::_mouse_XY.ship_icon_idx = -1;
        MOX::_eta_star_id = -1;
        MOX::_fleet_ship_scanned = -1;

        HACCESS::Clear_Fleet_Box_Selected_Ship_Status_();
        EVANHELP::Set_Main_Screen_Help_List_();

        MOX::_reloaded_ship_palette = 0;
    }

    void __cdecl Vanish_System_Display_Box_() {
        MOX::_moveable_box[0].id = MOX::_box_off_flag[0];

        for (int32_t i = 0; i < 5; ++i) {
            MOX::_planet_buttons[i] = -1000;
        }

        for (int32_t i = 0; i < 8; ++i) {
            MOX::_system_display_ships[i].button = -1000;
        }

        MOX::_moveable_box[0].drag_field = -1000;
        MOX::_moveable_box[0].close_btn = -1000;
    }

    bool __cdecl Mouse_Within_Fleet_Box_() {
        bool within = false;
        int16_t px = mouse::Pointer_X_();
        int16_t py = mouse::Pointer_Y_();

        const s_movable_box& box = MOX::_moveable_box[2];

        int32_t bottom_val = (int32_t)box.height + (int32_t)box.y;
        if (bottom_val >= 0x1A5) {
            bottom_val = 0x1A5;
        }

        int16_t bottom = (int16_t)bottom_val;
        int16_t right = (int16_t)(box.width + box.x);

        if (px >= box.x && px <= right && py >= box.y && py <= bottom) {
            within = true;
        }

        return within;
    }

    void __cdecl Box_Fleet_Screen_Scanned_Star_() {
        s_star_data* star = &MOX::_star[MOX::_galaxy_map_scanned_star];

        int32_t x_scaled = (star->x * 1000) / (int32_t)MOX::_max_map_scale;
        int16_t x = (int16_t)(((x_scaled * 10) / 1659) + 15) - 5;

        int32_t y_scaled = (star->y * 1000) / (int32_t)MOX::_max_map_scale;
        int16_t y = (int16_t)((y_scaled * 10) / 2197) + 47;

        graphics::Box_(x, y, 11, 12, 1, 113);
    }

    void __cdecl Box_Fleet_Screen_Scanned_Ship_() {
        int16_t ship_idx = MOX::_scanned_small_ship;
        auto& icon = MOX::_ship_icon[ship_idx];

        graphics::Box_(
            icon.x + 4,
            icon.y + 2,
            11,
            11,
            1,
            141
        );
    }

    void __cdecl Do_Fleet_Screen_Stuff_(int16_t map_x_off, int16_t map_y_off, int32_t scale_x, int32_t scale_y) {
        if (MOX::_galaxy_map_scanned_star > -1) {
            MOVEBOX::Box_Fleet_Screen_Scanned_Star_();
        }

        if (MOX::_scanned_small_ship > -1) {
            int16_t stack_id = HACCESS::Ship_Stack_From_Ship_Icon_(MOX::_scanned_small_ship);
            if (stack_id > -1 && stack_id != MOX::_small_ship_stack_ptr) {
                MOVEBOX::Box_Fleet_Screen_Scanned_Ship_();
            }
        }
    }

    void __cdecl Do_Officer_Screen_Stuff_(int16_t map_x_off, int16_t map_y_off, int16_t scale_x, int16_t scale_y) {
        bool is_fleet_officer = OFFICER::Is_Fleet_Officer_Screen_();
        int16_t scanned_star_idx = MOX::_galaxy_map_scanned_star;

        if (scanned_star_idx >= 0) {
            s_star_data& star = MOX::_star[scanned_star_idx];
            int8_t owner = star.owner;

            uint8_t color1, color2;
            if (owner < 0 || owner >= MAX_PLAYERS) {
                color1 = 33;
                color2 = 38;
            } else {
                uint8_t player_color_idx = MOX::_player[owner].color;
                uint8_t palette_color = MOX::_main_palette_player_colors[player_color_idx];
                if (palette_color == 32) {
                    color1 = 8;
                    color2 = 22;
                } else {
                    color1 = palette_color + 1;
                    color2 = palette_color + 5;
                }
            }

            s_colors font_colors;
            MISC::Get_Mox_Font_Colors_(3, color1, color2, &font_colors);
            MOVEBOX::Box_Officer_Screen_Scanned_Star_();
            fonts::Set_Font_Style_(3, &font_colors);

            char buffer[40];
            bool can_see_info = HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, scanned_star_idx) ||
                               HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM) ||
                               HAROLD::Contact_With_One_Colony_(scanned_star_idx);

            if (can_see_info) {
                int8_t off_idx = star.officer_index[MOX::_PLAYER_NUM];
                bool has_colony = HAROLD::Player_Has_Colony_(MOX::_PLAYER_NUM, scanned_star_idx);

                if (off_idx == -1 || !has_colony) {
                    strcpy(buffer, star.name);
                } else {
                    s_leader_data& leader = MOX::_leaders[off_idx];
                    int8_t eta = leader.eta;

                    if (eta <= 0) {
                        sprintf(buffer, MOX::s_fmt_scanned_star_info, star.name, leader.name);
                    } else {
                        const char* fmt;
                        if (eta == 1) {
                            fmt = HAROLD::H_Message_(146);
                        } else {
                            fmt = HAROLD::H_Message_(147);
                        }
                        sprintf(buffer, fmt, star.name, leader.name, eta);
                    }
                }
            } else {
                strcpy(buffer, HAROLD::H_Message_(148));
            }

            MISC::Set_Fitted_Font_Style_(buffer, &font_colors, 3, 2, 0, 203, 1);
            fonts::Print_Centered_(466, 418, buffer);
        }

        if (!is_fleet_officer && OFFICER::_officer_star_displayed >= 0) {
            MOVEBOX::Box_Officer_Screen_Chosen_Star_(map_x_off, map_y_off, scale_x, scale_y);
        }
    }

    void __cdecl Move_Box_To_Top_(int16_t box_id) {
        int16_t reordered_box_ids[5] = {0, 0, 0, 0, 0};
        const int16_t reordered_box_count =
            (int16_t)(sizeof(reordered_box_ids) / sizeof(reordered_box_ids[0]));
        int16_t found_idx = -1;

        for (int16_t i = 0; i < reordered_box_count; ++i) {
            if (MOX::_moveable_box_order[i] == box_id) {
                found_idx = i;
                break;
            }
        }

        int16_t target_box_id;
        if (found_idx >= 0) {
            target_box_id = MOX::_moveable_box_order[found_idx];
            MOX::_moveable_box_order[found_idx] = -1;
        } else {
            target_box_id = MOX::_moveable_box_reorder_fallback;
            MOX::_moveable_box_reorder_fallback = -1;
        }

        reordered_box_ids[0] = target_box_id;

        for (int16_t i = 1; i < reordered_box_count; ++i) {
            for (int16_t j = 0; j < reordered_box_count; ++j) {
                if (MOX::_moveable_box_order[j] != -1) {
                    reordered_box_ids[i] = MOX::_moveable_box_order[j];
                    MOX::_moveable_box_order[j] = -1;
                    break;
                }
            }
        }

        for (int16_t i = 0; i < reordered_box_count; ++i) {
            MOX::_moveable_box_order[i] = reordered_box_ids[i];
        }
    }

    int16_t __cdecl N_Moveable_Boxes_Selected_(int16_t excluded_box_idx) {
        int16_t box_index = 0;
        int16_t selected_count = 0;

        for (; box_index < 5; ++box_index) {
            if (box_index != excluded_box_idx) {
                if (MOVEBOX::Moveable_Box_Selected_(box_index) != 0) {
                    ++selected_count;
                }
            }
        }

        return selected_count;
    }

    void __cdecl Remove_Box_Check_() {
    }

    void __cdecl Get_Galaxy_Map_Star_XY_(int16_t star_idx,
                                         int16_t base_x,
                                         int16_t base_y,
                                         int16_t map_width,
                                         int16_t map_height,
                                         int16_t offset_x,
                                         int16_t offset_y,
                                         int16_t star_marker_mode,
                                         int16_t* out_x,
                                         int16_t* out_y) {
        const int32_t x_divisor = 506000 / static_cast<int32_t>(map_width) + 1;
        const int32_t y_divisor = 400000 / static_cast<int32_t>(map_height) + 1;
        const int32_t map_scale = static_cast<int32_t>(MOX::_max_map_scale);

        s_star_data* const star = &MOX::_star[star_idx];

        const int32_t scaled_x = (static_cast<int32_t>(star->x) * 1000) / map_scale;
        const int32_t scaled_y = (static_cast<int32_t>(star->y) * 1000) / map_scale;

        const int16_t star_center_offset = (star_marker_mode == 0) ? 2 : 3;

        int16_t screen_x = static_cast<int16_t>((scaled_x * 10) / x_divisor);
        screen_x = static_cast<int16_t>(screen_x - star_center_offset);
        screen_x = static_cast<int16_t>(screen_x + base_x);
        screen_x = static_cast<int16_t>(screen_x + offset_x);
        *out_x = screen_x;

        int16_t screen_y = static_cast<int16_t>((scaled_y * 10) / y_divisor);
        screen_y = static_cast<int16_t>(screen_y - star_center_offset);
        screen_y = static_cast<int16_t>(screen_y + base_y);
        screen_y = static_cast<int16_t>(screen_y + offset_y);
        *out_y = screen_y;
    }

    void __cdecl Get_Galaxy_Map_Space_XY_(int16_t ship_icon_idx,
                                          int16_t origin_x,
                                          int16_t origin_y,
                                          int16_t x_zoom,
                                          int16_t y_zoom,
                                          int16_t* out_x,
                                          int16_t* out_y) {
        const s_ship_icon& icon = MOX::_ship_icon[ship_icon_idx];

        const int32_t scaled_y =
            ((static_cast<int32_t>(icon.y) - 21) *
             ((static_cast<int32_t>(y_zoom) * 1000) / 400 + 1)) /
            1000;

        const int32_t scaled_x =
            ((static_cast<int32_t>(icon.x) - 21) *
             ((static_cast<int32_t>(x_zoom) * 1000) / 506 + 1)) /
            1000;

        *out_x = static_cast<int16_t>(origin_x + scaled_x);
        *out_y = static_cast<int16_t>(origin_y + scaled_y);
    }

    void __cdecl Box_Officer_Screen_Chosen_Star_(int16_t left, int16_t top, int16_t x_divisor, int16_t y_divisor) {
        const int16_t displayed_star_index = OFFICER::_officer_star_displayed;
        const s_star_data& star = MOX::_star[displayed_star_index];

        const int32_t box_x =
            static_cast<int32_t>(left) +
            0x132 +
            (((static_cast<int32_t>(star.x) * 1000) / static_cast<int32_t>(MOX::_max_map_scale)) * 10) /
                static_cast<int32_t>(x_divisor);

        const int32_t box_y =
            static_cast<int32_t>(top) +
            0xEB +
            (((static_cast<int32_t>(star.y) * 1000) / static_cast<int32_t>(MOX::_max_map_scale)) * 10) /
                static_cast<int32_t>(y_divisor);

        if (displayed_star_index == OFFICER::_officer_star_chosen) {
            graphics::Box_((int16_t)(box_x - 5), (int16_t)(box_y - 5), 0x0B, 0x0C, 1, (uint8_t)(OFFICER::_offscrn_flash_value + 0x6E));
            return;
        }

        graphics::Box_((int16_t)(box_x - 5), (int16_t)(box_y - 5), 0x0B, 0x0C, 1, 0x1E);
    }

    void __cdecl Box_Officer_Screen_Scanned_Star_() {
        const int32_t scanned_star_index = static_cast<int32_t>(MOX::_galaxy_map_scanned_star);
        s_star_data* const scanned_star = &MOX::_star[scanned_star_index];
        const int32_t map_scale = static_cast<int32_t>(MOX::_max_map_scale);

        const int32_t scaled_x = ((((int32_t)scanned_star->x * 1000) / map_scale) * 10) / 0x637;
        const int32_t scaled_y = ((((int32_t)scanned_star->y * 1000) / map_scale) * 10) / 0x93e;

        const int16_t box_x = (int16_t)((uint16_t)(scaled_x + 0x132) - 5);
        const int16_t box_y = (int16_t)((uint16_t)scaled_y + 0xE6);

        graphics::Box_(box_x, box_y, 0x0B, 0x0C, 1, 0x71);
    }

    void __cdecl Add_Galaxy_Map_Fields_2_(
            int16_t map_x,
            int16_t map_y,
            int16_t map_width,
            int16_t map_height,
            int16_t offset_x,
            int16_t offset_y,
            uint32_t unused_arg7,
            uint32_t unused_arg8,
            int16_t field_style,
            int16_t* out_special_field_id,
            int16_t* star_field_ids,
            int16_t* out_grid_field_id) {
        (void)unused_arg7;
        (void)unused_arg8;

        const int16_t window_top = static_cast<int16_t>(map_y + offset_y);
        const int16_t window_left = static_cast<int16_t>(map_x + offset_x);

        graphics::Set_Window_(
                window_left,
                window_top,
                static_cast<int16_t>(offset_x + map_width + map_x),
                static_cast<int16_t>(offset_y + map_height + map_y));

        if (MOX::_current_screen == SCREEN_COLONY) {
            *out_special_field_id = static_cast<int16_t>(0xFC18);
        }

        if (MOX::_NUM_STARS > 0) {
            for (int16_t star_index = 0; star_index < MOX::_NUM_STARS; ++star_index) {
                int16_t star_y;
                int16_t star_x;

                MOVEBOX::Get_Galaxy_Map_Star_XY_(
                        star_index,
                        map_x,
                        map_y,
                        map_width,
                        map_height,
                        offset_x,
                        offset_y,
                        field_style,
                        &star_x,
                        &star_y);

                int16_t field_id;
                if (field_style == 0) {
                    field_id = fields::Add_Hidden_Field_(
                            star_x,
                            star_y,
                            static_cast<int16_t>(star_x + 5),
                            static_cast<int16_t>(star_y + 5),
                            MOX::s__00551100,
                            0x29);
                } else {
                    field_id = fields::Add_Hidden_Field_(
                            static_cast<int16_t>(star_x - 3),
                            static_cast<int16_t>(star_y - 3),
                            static_cast<int16_t>(star_x + 8),
                            static_cast<int16_t>(star_y + 9),
                            MOX::s__00551100,
                            0x29);
                }

                star_field_ids[star_index] = field_id;
            }
        }

        if (MOX::_current_screen == SCREEN_COLONY) {
            *out_grid_field_id = fields::Add_Grid_Field_(
                    window_left,
                    window_top,
                    1,
                    1,
                    map_width,
                    map_height,
                    &MOX::_galaxy_map_grid_y,
                    &MOX::_galaxy_map_grid_x,
                    0);
        }

        graphics::Reset_Window_();
    }

    void __cdecl Draw_Moveable_Box_(int16_t box_id) {
        const int32_t box_index = box_id;
        s_movable_box& box = MOX::_moveable_box[box_index];

        if (static_cast<int16_t>(MOX::_box_off_flag[box_index]) >= box.id) {
            return;
        }

        if (SYS::_skip_draw_system_display != 0) {
            SYS::_skip_draw_system_display = 0;
            return;
        }

        if (box.index == 0) {
            SYS::Wrapper_For_Normal_System_Popup_();
            return;
        }

        if (box.index == 1) {
            const int32_t pict_index = box_index * 5;

            MOVEBOX::Draw_Galaxy_Map_Box_(
                    MOX::_moveable_box_pict[pict_index],
                    MOX::_moveable_box_pict[pict_index + 1],
                    box.x,
                    box.y,
                    MOX::_galaxy_map_height,
                    MOX::_galaxy_map_width,
                    8,
                    0x26,
                    box.content_offset_x,
                    box.content_offset_y,
                    0,
                    0);
            return;
        }

        if (box.index != 2) {
            return;
        }

        FLEETPOP::Draw_Fleet_Movement_Box_();
    }

    int8_t __cdecl Box_Was_Open_(int16_t box_index) {
        return MOX::_saved_boxes[box_index].is_open;
    }

    void __cdecl Restore_Boxes_() {
        for (int16_t box_index = 0; box_index < 5; ++box_index) {
            if (MOX::_saved_boxes[box_index].field_id != -1000) {
                const int8_t was_open = MOX::_saved_boxes[box_index].is_open;

                MOX::_moveable_box[box_index].id = MOX::_saved_boxes[box_index].field_id;

                if (was_open != 0) {
                    if (box_index == 0) {
                        SYS::Init_System_Display_(MOX::_moveable_box[0].id);
                    } else if (box_index == 2) {
                        if (HACCESS::Get_Stack_Start_From_Saved_Ship_Array_() == 0) {
                            MOVEBOX::Vanish_Fleet_Movement_Box_();
                        }
                    }
                }

                MOX::_saved_boxes[box_index].is_open = 0;
                MOX::_saved_boxes[box_index].field_id = -1000;
            }
        }
    }

    void __cdecl Save_And_Remove_Boxes_() {
        for (int16_t box_idx = 0; box_idx < 5; ++box_idx) {
            const int16_t is_open = (int16_t)MOVEBOX::Moveable_Box_Selected_(box_idx);

            MOX::_saved_boxes[box_idx].field_id = MOX::_moveable_box[box_idx].id;
            MOX::_saved_boxes[box_idx].is_open = static_cast<int8_t>(is_open);

            if (box_idx == 0 && is_open != 0) {
                MOVEBOX::Vanish_System_Display_Box_();
            }

            if (box_idx == 2 && is_open != 0) {
                MOVEBOX::Vanish_Fleet_Movement_Box_();
            }

            MOX::_mouse_XY.star_idx = -1;
            MOX::_moveable_box[box_idx].id =
                    static_cast<int16_t>(MOX::_box_off_flag[MOX::_moveable_box[box_idx].index]);
        }
    }

    void __cdecl Clear_Moveable_Box_Fields_(int16_t box_index) {
        MOX::_moveable_box[box_index].close_btn = -1000;
        MOX::_moveable_box[box_index].drag_field = -1000;
        MOX::_moveable_box[box_index].window_field_id = -1000;

        if (box_index == 0) {
            for (int16_t i = 0; i < 5; ++i) {
                MOX::_planet_buttons[i] = -1000;
            }
            return;
        }

        if (box_index == 1) {
            MOX::_galaxy_map_grid_field = -1000;

            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                MOX::_galaxy_map_star_field[i] = -1000;
            }
            return;
        }

        if (box_index == 2) {
            MOX::_fleet_box_scroll_up_button = -1000;
            MOX::_fleet_box_scroll_dn_button = -1000;
            MOX::_fleet_movement_bar_field = -1000;
            MOX::_moveable_box[2].close_btn = -1000;
            MOX::_moveable_box[2].drag_field = -1000;

            if (MOVEBOX::Moveable_Box_Selected_(2) != 0) {
                for (int16_t i = 0; i < MOX::_fleet_box.count; ++i) {
                    MOX::_fleet_movement_icon[i].field = -1000;
                }
            }
        }
    }

    uint16_t __cdecl Box_Field_Selected_(int16_t box_index, int16_t field_id) {
        if ((MOX::_moveable_box[box_index].drag_field == field_id) ||
            (MOX::_moveable_box[box_index].window_field_id == field_id)) {
            return 1;
        }

        if ((MOX::_moveable_box[1].index == box_index) &&
            (field_id == MOX::_galaxy_map_grid_field)) {
            return 1;
        }

        if ((MOX::_moveable_box[2].index == box_index) &&
            ((field_id == MOX::_fleet_movement_bar_field) ||
             (field_id == MOX::_fleet_movement_all_button))) {
            return 1;
        }

        return 0;
    }

    void __cdecl Move_Moveable_Box_(int16_t moveable_box_idx, int16_t mouse_button_state) {
        int16_t special_clamp_height = 0;
        int16_t box_height;
        int16_t box_width;

        if (moveable_box_idx == 2) {
            special_clamp_height = 0x1c;

            if (MOX::_mouse_button_pressed == 2) {
                MOX::_fleet_box.mouse_pressed = 1;
            } else {
                MOX::_fleet_box.mouse_pressed = 0;
            }
        } else if (moveable_box_idx == 0) {
            special_clamp_height = 0x2b;
        }

        if (moveable_box_idx == 2) {
            box_width = MOX::_fleet_box.width;
            box_height = MOX::_fleet_box.height;

            if (MOX::_fleet_box.colonize_btn_active != 0) {
                MOX::_fleet_box.colonize_btn_active = 2;
            }
        } else {
            s_movable_box& movable_box = MOX::_moveable_box[moveable_box_idx];

            if (MOVEBOX::Moveable_Box_Selected_(moveable_box_idx) != 0) {
                box_width = movable_box.width;
                box_height = movable_box.height;
            } else {
                box_width = movable_box.unselected_width;
                box_height = movable_box.unselected_height;
            }
        }

        if (mouse_button_state != 0) {
            s_movable_box& movable_box = MOX::_moveable_box[moveable_box_idx];

            if (fields::Auto_Input_() == movable_box.drag_field || movable_box.active == 1) {
                fields::Deactivate_Help_List_();
                MOX::_mouse_button_pressed = static_cast<int8_t>(mouse_button_state);
                fields::Disable_Mouse_Pressed_Field_Update_();
                fields::Disable_Draw_All_Fields_();
                MOX::_skip_fields = 1;

                if (MOX::_stencil_box_id == -1) {
                    MOX::_stencil_box_id = moveable_box_idx;
                    MOX::_draw_main_screen_stencilled = 1;
                }

                fields::Disable_Draw_All_Fields_();

                movable_box.content_offset_x =
                        static_cast<int16_t>(mouse::Pointer_X_() - fields::Pressed_Mouse_X_());
                movable_box.content_offset_y =
                        static_cast<int16_t>(mouse::Pointer_Y_() - fields::Pressed_Mouse_Y_());

                if (static_cast<int32_t>(movable_box.content_offset_x) + movable_box.x < 0x16) {
                    movable_box.content_offset_x = static_cast<int16_t>(0x16 - movable_box.x);
                }

                if (static_cast<int32_t>(movable_box.content_offset_x) + movable_box.x >
                    0x20e - box_width) {
                    movable_box.content_offset_x =
                            static_cast<int16_t>(0x20f - movable_box.x - box_width);
                }

                if (static_cast<int32_t>(movable_box.content_offset_y) + movable_box.y < 0x16) {
                    movable_box.content_offset_y = static_cast<int16_t>(0x16 - movable_box.y);
                }

                if (mouse_button_state == 2 && moveable_box_idx == 0) {
                    if (static_cast<int32_t>(movable_box.content_offset_y) + special_clamp_height +
                            movable_box.y >
                        0x1a4) {
                        movable_box.content_offset_y =
                                static_cast<int16_t>(0x1a5 - special_clamp_height - movable_box.y);
                    }
                } else {
                    if (static_cast<int32_t>(movable_box.content_offset_y) + movable_box.y >
                        0x1a4 - box_height) {
                        movable_box.content_offset_y =
                                static_cast<int16_t>(0x1a5 - movable_box.y - box_height);
                    }
                }

                movable_box.active = 1;
                return;
            }
        }

        MOX::_draw_main_screen_to_back = 1;
        fields::Enable_Draw_All_Fields_();
        EVANHELP::Set_Main_Screen_Help_List_();
        fields::Clear_Fields_();
        MOVEBOX::Clear_Moveable_Box_Fields_(moveable_box_idx);
        MOX::_skip_fields = 1;

        {
            s_movable_box& movable_box = MOX::_moveable_box[moveable_box_idx];

            if (movable_box.active == 1) {
                movable_box.x = static_cast<int16_t>(
                        movable_box.x + (mouse::Pointer_X_() - fields::Pressed_Mouse_X_()));

                const int16_t pointer_y = mouse::Pointer_Y_();
                const int16_t pressed_mouse_y = fields::Pressed_Mouse_Y_();
                MOX::_stencil_box_id = -2;
                movable_box.y =
                        static_cast<int16_t>(movable_box.y + (pointer_y - pressed_mouse_y));
            }

            if (movable_box.x < 0x16) {
                movable_box.x = 0x16;
            }

            if (movable_box.x > 0x20e - box_width) {
                movable_box.x = static_cast<int16_t>(0x20f - box_width);
            }

            if (movable_box.y < 0x16) {
                movable_box.y = 0x16;
            }

            if (MOX::_mouse_button_pressed == 2 && moveable_box_idx == 0) {
                if (movable_box.y > 0x1a4 - special_clamp_height) {
                    movable_box.y = static_cast<int16_t>(0x1a5 - special_clamp_height);
                }
            } else {
                if (movable_box.y > 0x1a4 - box_height) {
                    movable_box.y = static_cast<int16_t>(0x1a5 - box_height);
                }
            }

            movable_box.content_offset_x = 0;
            movable_box.content_offset_y = 0;
            movable_box.active = 0;
        }

        MOVEBOX::Move_Box_To_Top_(moveable_box_idx);

        if (moveable_box_idx == 2) {
            MOX::_fleet_box.x = MOX::_moveable_box[2].x;
            MOX::_fleet_box.y = MOX::_moveable_box[2].y;
        }

        MOX::_draw_main_screen_to_back = 1;
        EVANHELP::Set_Main_Screen_Help_List_();
    }

    void __cdecl Moveable_Box_Dimensions_(
        int16_t box_index,
        int16_t* left,
        int16_t* top,
        int16_t* right,
        int16_t* bottom) {
        *left = MOX::_moveable_box[box_index].x;
        *top = MOX::_moveable_box[box_index].y;

        if (box_index == 2) {
            *right = *left + MOX::_fleet_box.width;
            *bottom = *top + MOX::_fleet_box.field8_0xe;
            return;
        }

        *right = MOX::_moveable_box[box_index].width + *left;
        *bottom = MOX::_moveable_box[box_index].height + *top;
    }

    void __cdecl Add_Whole_Window_Field_(int16_t moveable_box_index) {
        int16_t height;
        int16_t width;
        int16_t y;
        int16_t x;

        Moveable_Box_Dimensions_(moveable_box_index, &x, &y, &width, &height);
        MOX::_moveable_box[moveable_box_index].window_field_id =
            fields::Add_Hidden_Field_(x, y, width, height, MOX::s__00551100, 0);
    }
}