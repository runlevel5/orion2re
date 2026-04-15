#include "pch.h"

namespace MOVEBOX {
    void __cdecl Draw_Galaxy_Map_Box_(s_animation_header* anim1, s_animation_header* anim2,
                                    int16_t x_base, int16_t y_base,
                                    int16_t width, int16_t height,
                                    int16_t map_x_off, int16_t map_y_off,
                                    int16_t x_adj, int16_t y_adj,
                                    int16_t view_mode, int16_t exit_flag) {
        s_colors font_colors;
        for (int i = 2; i < 8; ++i) {
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
                    for (int i = 0; i < 8; ++i) {
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

                if (MOX::_star[i].pict_type == STAR_CLASS_BLACK_HOLE) {
                    color_idx = COLONY::_using_colony_screen_palette ? 9 : 10;
                } else if (MOX::_star[i].pict_type == STAR_CLASS_DWARF) {
                    color_idx = 8;
                } else if (owner < 0 || owner > 7) {
                    if (HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, i) || owner == -1 || owner == -2) {
                        color_idx = 8;
                    } else {
                        color_idx = font_colors.colors[0];
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
                        int count = animate::Get_Animation_Frame_Count_(summ_anim);
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
        } else if (MOX::_current_screen == SCREEN_OFFICERS) {
            Do_Officer_Screen_Stuff_(map_x_off, map_y_off, scale_x, scale_y);
        }
    }

    bool __cdecl Moveable_Box_Selected_(int16_t box_idx) {
        return MOX::_box_off_flag[box_idx] < MOX::_moveable_box[box_idx].id;
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
        MOX::_moveable_box[2].field = -1000;
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

        MOX::_moveable_box[0].field = -1000;
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
        s_star_data* star = &MOX::_star[MOX::_galaxy_map_scanned_star]; // star_data size 0x71

        int32_t x_scaled = (star->x * 1000) / (int32_t)MOX::_max_map_scale;
        int16_t x = (int16_t)((x_scaled * 10) / 1659 /* 0x67b */ + 15) - 5;

        int32_t y_scaled = (star->y * 1000) / (int32_t)MOX::_max_map_scale;
        int16_t y = (int16_t)((y_scaled * 10) / 2197 /* 0x895 */) + 47 /* 0x2f */;

        graphics::Box_(x, y, 11 /* 0xb */, 12 /* 0xc */, 1 /* 0x1 */, 113 /* 0x71, 'q' */);
    }

    void __cdecl Box_Fleet_Screen_Scanned_Ship_(int16_t map_x_off, int16_t map_y_off, int32_t scale_x, int32_t scale_y) {
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
                MOVEBOX::Box_Fleet_Screen_Scanned_Ship_(map_x_off, map_y_off, scale_x, scale_y);
            }
        }
    }

    void __cdecl Do_Officer_Screen_Stuff_(int16_t param_1, int16_t param_2, int param_3, int param_4) {
        bool is_fleet_officer = OFFICER::Is_Fleet_Officer_Screen_();
        int16_t scanned_star_idx = MOX::_galaxy_map_scanned_star;

        if (scanned_star_idx >= 0) {
            s_star_data& star = MOX::_star[scanned_star_idx];
            int8_t owner = star.owner;

            uint8_t color1, color2;
            if (owner < 0 || owner >= 8) {
                color1 = 33 /* 0x21 */;
                color2 = 38 /* 0x26 */;
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
                    int8_t status = leader.status;

                    if (status <= 0) {
                        _sprintf(buffer, MOVEBOX::s_geggggg_format, star.name, leader.name);
                    } else {
                        const char* fmt;
                        if (status == 1) {
                            fmt = HAROLD::H_Message_(146 /* 0x92 */);
                        } else {
                            fmt = HAROLD::H_Message_(147 /* 0x93 */);
                        }
                        _sprintf(buffer, fmt, star.name, leader.name, (int)status);
                    }
                }
            } else {
                strcpy(buffer, HAROLD::H_Message_(148 /* 0x94 */));
            }

            MISC::Set_Fitted_Font_Style_(buffer, &font_colors, 3, 2, 0, 203 /* 0xCB */, 1);
            fonts::Print_Centered_(466 /* 0x1D2 */, 418 /* 0x1A2 */, buffer);
        }

        if (!is_fleet_officer && OFFICER::_officer_star_displayed >= 0) {
            MOVEBOX::Box_Officer_Screen_Chosen_Star_(param_1, param_2, (int16_t)param_3, (int16_t)param_4);
        }
    }

    void __cdecl Move_Box_To_Top_(int16_t box_id) {
        int16_t local_order[5] = {0, 0, 0, 0, 0};
        int16_t found_idx = -1;

        // Find the index of the box in the current order
        for (int16_t i = 0; i < 5; ++i) {
            if (MOX::_moveable_box_order[i] == box_id) {
                found_idx = i;
                break;
            }
        }

        // Extract the box from the global array and mark the slot as empty (0xFFFF)
        // If the box was not found, this logic extracts the padding word before the array.
        int16_t* const moveable_box_order = reinterpret_cast<int16_t*>(&MOX::_moveable_box_order);
        int16_t target_box_id = moveable_box_order[found_idx];
        moveable_box_order[found_idx] = -1; // 0xFFFF

        // The specified box is now at the top (index 0) of our local sorted list
        local_order[0] = target_box_id;

        // Fill slots 1 through 4 with the remaining boxes in their existing relative order
        for (int i = 1; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (MOX::_moveable_box_order[j] != -1) {
                    local_order[i] = MOX::_moveable_box_order[j];
                    MOX::_moveable_box_order[j] = -1;
                    break;
                }
            }
        }

        // Copy the reorganized local list back to the global order array
        for (int i = 0; i < 5; ++i) {
            MOX::_moveable_box_order[i] = local_order[i];
        }
    }
}