#include "pch.h"

namespace SHIPS {

    /**
     * @address 0041ff40
     */
    void __cdecl Clear_Auto_Colonization_Flag_(s_ship_data* ship) {
        uint8_t ship_type = ship->d.ship_type;

        if (ship_type == 1 || ship_type == 4) {
            ship->d.ship_weapon[0].type = -1;
            ship->d.ship_weapon[1].type = 0;
        }
    }

    /**
     * @address 0041ff60
     */
    void __cdecl Clear_All_Ships_Auto_Colonization_Flag_(int16_t* ship_list) {
        if (*ship_list == -1) {
            return;
        }

        int i = 0;
        do {
            int16_t ship_idx = ship_list[i];
            i++;
            SHIPS::Clear_Auto_Colonization_Flag_(&MOX::_ship[ship_idx]);
        } while (ship_list[i] != -1);
    }

    void __cdecl Kill_Ship_(int16_t ship_idx) {
        HACCESS::Ship_Owner_(ship_idx);

        bool officer_relocated = false;
        for (int16_t leader_idx = 0; leader_idx < 67; ++leader_idx) {
            if (officer_relocated) {
                break;
            }

            if (MOX::_leaders[leader_idx].type == 0 && 
                MOX::_leaders[leader_idx].location == ship_idx && 
                MOX::_leaders[leader_idx].eta > 0) {
                
                OFFICER::Move_Officer_To_Pool_(leader_idx);
                officer_relocated = true;
            }
        }

        MOX::_ship[ship_idx].location = -1;
        MOX::_ship[ship_idx].status = 5;
    }

    void __cdecl Set_Ship_Icon_XYs_() {
        int16_t ship_idx = MOX::_ship_icon_count - 1;

        if (ship_idx >= 0) {
            do {
                int16_t x;
                int16_t y;

                SHIPS::Get_Ship_Icon_Coords_(ship_idx, &x, &y);

                MOX::_ship_icon[ship_idx].x = x;
                MOX::_ship_icon[ship_idx].y = y;

                ship_idx--;
            } while (ship_idx >= 0);
        }
    }

    int16_t __cdecl Build_Nonplayer_Ship_(int16_t type, int16_t system_idx, int16_t owner) {
        s_ship_design design;
        uint8_t ship_owner = (uint8_t)owner;

        switch (type) {
        case 9 /* Guardian */:
            INITSHIP::Load_Guardian_Ship_Design_(&design);
            break;
        case 10 /* Amoeba */:
            INITSHIP::Load_Amoeba_Ship_Design_(&design, ship_owner);
            break;
        case 11 /* Crystal */:
            INITSHIP::Load_Crystal_Ship_Design_(&design, ship_owner);
            break;
        case 12 /* Dragon */:
            INITSHIP::Load_Dragon_Ship_Design_(&design, ship_owner);
            break;
        case 13 /* Eel */:
            INITSHIP::Load_Eel_Ship_Design_(&design, ship_owner);
            break;
        case 14 /* Hydra */:
            INITSHIP::Load_Hydra_Ship_Design_(&design, ship_owner);
            break;
        default:
            return -1;
        }

        int16_t ship_idx = Set_Nonplayer_Ship_XY_(system_idx, type, &design, ship_owner);

        if (ship_idx > -1) {
            Set_Nonplayer_Ship_Destination_(system_idx, ship_idx, ship_owner);
        }

        return ship_idx;
    }

    int16_t __cdecl Set_Nonplayer_Ship_Destination_(int16_t star_idx, int16_t ship_idx, uint8_t is_moving) {
        s_ship_move_info move_info;
        int16_t ship_list[2];

        ship_list[0] = ship_idx;
        ship_list[1] = -1;

        if (is_moving != 0) {
            SHIPMOVE::Ships_Try_To_Move_To_(ship_list, star_idx, &move_info);

            if (move_info.moving == 0) {
                RUSS::PFMT_(0, 1, s_Monster_ship_id_cannot_mov_00552cd8, (int)ship_idx, MOX::_star[star_idx].name, (int)star_idx);
                assert::hgetch_();
            } else {
                SHIPMOVE::Make_Ships_Move_To_(ship_list, star_idx, &move_info);
            }

            MOX::_ship[ship_idx].status = 1;
            MOX::_ship[ship_idx].location = star_idx + 500;
        } else {
            MOX::_ship[ship_idx].status = 0;
            MOX::_ship[ship_idx].location = star_idx;
        }

        return GEO::Parsecs_Between_Points_(MOX::_star[star_idx].x, MOX::_star[star_idx].y, MOX::_ship[ship_idx].x, MOX::_ship[ship_idx].y);
    }

    int16_t __cdecl Set_Nonplayer_Ship_XY_(int16_t system_idx, int16_t type, s_ship_design* design, uint8_t owner) {
        int16_t final_x_upscaled;
        int16_t final_y_upscaled;

        if (owner == 0) {
            final_x_upscaled = MOX::_star[system_idx].x;
            final_y_upscaled = MOX::_star[system_idx].y;
        } else {
            int16_t scaled_offset = HAROLD::Get_Scaled_Value_(30);
            int16_t random_iters = (int16_t)random::Random_(5);
            int16_t star_x_scaled = HAROLD::Get_Scaled_Value_(MOX::_star[system_idx].x);
            int16_t star_y_scaled = HAROLD::Get_Scaled_Value_(MOX::_star[system_idx].y);

            int16_t base_dist = (random_iters + 1) * scaled_offset + 10;
            int16_t dir_x = 0;
            int16_t dir_y = 0;

            bool x_less_than_max = (star_x_scaled + base_dist) < 526;
            bool x_greater_than_min = (star_x_scaled - base_dist) > 21;
            bool y_less_than_max = (star_y_scaled + base_dist) < 420;
            bool y_greater_than_min = (star_y_scaled - base_dist) > 21;

            if (x_less_than_max) {
                if (x_greater_than_min) {
                    dir_x = (random::Random_(2) == 1) ? -1 : 1;
                } else {
                    dir_x = 1;
                }
            } else if (x_greater_than_min) {
                dir_x = -1;
            }

            if (y_less_than_max) {
                if (y_greater_than_min) {
                    dir_y = (random::Random_(2) == 1) ? -1 : 1;
                } else {
                    dir_y = 1;
                }
            } else if (y_greater_than_min) {
                dir_y = -1;
            }

            int16_t dx = scaled_offset * dir_x;
            int16_t dy = scaled_offset * dir_y;

            if (random_iters > 0) {
                for (int i = 0; i < random_iters; ++i) {
                    dx += (int16_t)random::Random_(scaled_offset) * dir_x;
                    dy += (int16_t)random::Random_(scaled_offset) * dir_y;
                }
            }

            int16_t final_x_scaled = star_x_scaled + dx;
            int16_t final_y_scaled = star_y_scaled + dy;

            int16_t star_width_half = (int16_t)MOX::_star_width / 2;
            if (final_x_scaled < star_width_half + 22) {
                final_x_scaled = star_width_half + 22;
            } else {
                int16_t scaled_map_max_x = HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_X);
                if (final_x_scaled > (scaled_map_max_x - star_width_half)) {
                    final_x_scaled = scaled_map_max_x - star_width_half;
                }
            }

            int16_t star_height_half = (int16_t)MOX::_star_height / 2;
            if (final_y_scaled < star_height_half + 22) {
                final_y_scaled = star_height_half + 22;
            } else {
                int16_t scaled_map_max_y = HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_Y);
                if (final_y_scaled > (scaled_map_max_y - (int16_t)MOX::_star_height)) {
                    final_y_scaled = scaled_map_max_y - (int16_t)MOX::_star_height;
                }
            }

            final_x_upscaled = (int16_t)HAROLD::Get_Up_Scaled_Value_(final_x_scaled);
            final_y_upscaled = (int16_t)HAROLD::Get_Up_Scaled_Value_(final_y_scaled);
        }

        return (int16_t)SHIPMOVE::Create_Nonplayer_Ship_(type, design, final_x_upscaled, final_y_upscaled);
    }

    void __cdecl Get_Ship_Icon_Coords_(int16_t ship_idx, int16_t* x, int16_t* y) {
        int16_t star_id_check = HACCESS::Ship_Icon_Star_Id_(ship_idx);

        int16_t node_idx = MOX::_ship_icon[ship_idx].node_idx;
        int16_t actual_ship_idx = MOX::_ship_node[node_idx].ship_idx;
        s_ship_data* ship = &MOX::_ship[actual_ship_idx];

        int16_t star_id = HAROLD::XY_To_Star_Id_(ship->x, ship->y);

        if (ship->status == 1 /* STATUS_IN_TRANSIT */) {
            Get_Ship_Icon_Coords_In_Space_(ship_idx, x, y);
            return;
        }

        int16_t ox1, oy1, ox2, oy2;
        int16_t ox_arr[4], oy_arr[4];

        Get_XYs_For_Orbiting_Ships_(star_id, (int16_t)ship->owner, &ox1, &oy1, &ox2, &oy2, ox_arr, oy_arr);

        // stack_slot is at offset 6 in s_ship_icon
        int16_t slot = *reinterpret_cast<int16_t*>(&MOX::_ship_icon[ship_idx].stack_slot);

        switch (slot) {
        case 0:
            *x = ox1;
            *y = oy1;
            break;

        case 1:
            *x = ox2;
            *y = oy2;
            if (ship->owner > 9 && (MOX::_cur_map_scale == 10 || MOX::_cur_map_scale == 15)) {
                *x -= 2;
            }
            break;

        case 2:
        case 3:
        case 4: {
            int idx = slot - 2;
            if (idx > 2) {
                idx = 2;
            }
            *x = ox_arr[idx];
            *y = oy_arr[idx];
            break;
        }

        case 5:
            *x = HAROLD::Get_Scaled_Value_(ship->x);
            *y = HAROLD::Get_Scaled_Value_(ship->y);
            break;

        default:
            break;
        }
    }

    void __cdecl Draw_Directional_Multi_Colored_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_colors* colors_ptr, int16_t color_offset) {
        int start_index = 7 - (int)color_offset;
        if (start_index <= 0) {
            start_index = 0;
        }

        if (x2 > x1) {
            if (y2 >= y1) {
                line::Multi_Colored_Line_(x1, y1, x2, y2, colors_ptr, (int16_t)start_index);
                return;
            }

            s_colors local_colors;
            for (int i = 0; i < 8; ++i) {
                // Reverses the color array for the line direction
                local_colors.colors[7 - i] = colors_ptr->colors[i];
            }
            line::Multi_Colored_Line_(x1, y1, x2, y2, &local_colors, color_offset);
            return;
        }

        // x2 <= x1 case
        if (y2 > y1) {
            line::Multi_Colored_Line_(x1, y1, x2, y2, colors_ptr, (int16_t)start_index);
            return;
        }

        s_colors local_colors;
        for (int i = 0; i < 8; ++i) {
            // Reverses the color array for the line direction
            local_colors.colors[7 - i] = colors_ptr->colors[i];
        }
        line::Multi_Colored_Line_(x1, y1, x2, y2, &local_colors, color_offset);
    }

    /**
     * @address 0041f030
     */
    void __cdecl Get_XYs_For_Orbiting_Ships_(int16_t star_idx, int16_t ship_icon_idx, int16_t* out_x1, int16_t* out_y1, int16_t* out_x2, int16_t* out_y2, int16_t* out_arr_x, int16_t* out_arr_y) {
        s_animation_header* anim = SHIPS::Get_Ship_Icon_Pict_Seg_(ship_icon_idx, 0);
        int16_t width = animate::Get_Width_(anim);
        animate::Get_Height_(anim); // Result ignored by original code

        int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();

        int16_t star_x, star_y;
        MAINSCR::Get_Star_Draw_Coords_(star_idx, &star_x, &star_y);

        int16_t star_size = HAROLD::Map_Scale_Star_Size_To_Zoom_Level_(star_idx);

        int16_t zoom_offset = 1 - zoom_level;
        int16_t diff_11_minus_zoom = 11 - zoom_level;

        // Coordinates adjusted by star size and zoom level
        int16_t local_x = star_x - (star_size / 2);
        int16_t local_y = star_y - (star_size / 2);
        int16_t half_width = width / 2;

        *out_x1 = zoom_offset - half_width + local_x + star_size;
        *out_y1 = local_y + zoom_offset - 1;

        *out_x2 = local_x - half_width - zoom_offset + 2;
        *out_y2 = local_y + zoom_offset - 2;

        int16_t x_ref = *out_x2 - 1;
        int16_t y_ref = *out_y2;

        out_arr_x[0] = x_ref;
        out_arr_y[0] = y_ref + diff_11_minus_zoom;

        out_arr_x[1] = x_ref;
        out_arr_y[1] = y_ref + 2 * diff_11_minus_zoom;

        out_arr_x[2] = x_ref;
        out_arr_y[2] = y_ref + 3 * diff_11_minus_zoom;
    }

    /**
     * @address 0041fef0
     */
    void __cdecl Get_Ship_Icon_Dimensions_(int16_t zoom_level, int8_t* width, int8_t* height) {
        s_animation_header* anim;

        anim = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, (int)zoom_level + 0xCD, MOX::_global_cache_seg);

        *width = (int8_t)animate::Get_Width_(anim);
        *height = (int8_t)animate::Get_Height_(anim);
    }

    /**
     * @address 0041f200
     */
    s_animation_header* __cdecl Get_Ship_Icon_Pict_Seg_(int16_t ship_type_idx, uint8_t is_highlighted) {
        int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();
        int16_t inverted_zoom = 1;

        // Map zoom level (0-3) to inverted index (3-0)
        if (zoom_level == 3) {
            inverted_zoom = 0;
        } else if (zoom_level == 2) {
            inverted_zoom = 1;
        } else if (zoom_level == 1) {
            inverted_zoom = 2;
        } else if (zoom_level == 0) {
            inverted_zoom = 3;
        }

        if (ship_type_idx < 8) {
            uint8_t color = MOX::_player[ship_type_idx].color;
            // Assembly contains a redundant call here
            HAROLD::Map_Scale_To_Zoom_Level_();

            int index = (int)inverted_zoom + (int)color * 4;

            if (is_highlighted != 0) {
                s_animation_header* seg = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, index + 205 /* 0xcd */, MOX::_global_cache_seg);
                MOX::_hl_ship_icon_seg[index] = seg;
                return seg;
            } else {
                s_animation_header* seg = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, index + 205 /* 0xcd */, MOX::_global_cache_seg);
                MOX::_ship_icon_seg[index] = seg;
                return seg;
            }
        }

        if (ship_type_idx == 8) {
            // Monster ship uses the inverted zoom index directly
            if (is_highlighted != 0) {
                s_animation_header* seg = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, (int)inverted_zoom + 237 /* 0xed */, MOX::_global_cache_seg);
                MOX::_hl_monster_ship_icon_seg[inverted_zoom] = seg;
                return seg;
            } else {
                s_animation_header* seg = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, (int)inverted_zoom + 237 /* 0xed */, MOX::_global_cache_seg);
                MOX::_monster_ship_seg[inverted_zoom] = seg;
                return seg;
            }
        }

        if (ship_type_idx >= 9 && ship_type_idx <= 14) {
            // Antaran ships use the raw zoom level (0-3)
            int antaran_idx = (int)ship_type_idx - 9;
            int index = (int)zoom_level + antaran_idx * 4;

            if (is_highlighted != 0) {
                s_animation_header* seg = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, index + 241 /* 0xf1 */, MOX::_global_cache_seg);
                MOX::_hl_antaran_ship_icon_seg[index] = seg;
                return seg;
            } else {
                s_animation_header* seg = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, index + 241 /* 0xf1 */, MOX::_global_cache_seg);
                MOX::_antaran_ship_icons[index] = seg;
                return seg;
            }
        }

        return nullptr;
    }

    /**
     * @address 0041f150
     */
    /**
     * @address 0041f730
     */
    bool __cdecl Enemy_Ship_Heading_Toward_Our_Colony_(int16_t node_idx) {
        bool result = false;
        int16_t ship_idx = MOX::_ship_node[node_idx].ship_idx;

        if (HACCESS::Ship_Owner_(ship_idx) != MOX::_PLAYER_NUM) {
            s_ship_data* ship = &MOX::_ship[ship_idx];

            if (ship->status != 0) {
                int16_t star_idx = HAROLD::Absolute_Location_(ship->location);

                if (HAROLD::Player_Has_Colony_(MOX::_PLAYER_NUM, star_idx)) {
                    result = true;
                } else if (HAROLD::Player_Has_Outpost_(MOX::_PLAYER_NUM, star_idx)) {
                    result = true;
                }
            }
        }

        return result;
    }

    void __cdecl Do_Ship_Destination_Lines_() {
        for (int16_t i = MOX::_ship_icon_count - 1; i >= 0; --i) {
            int16_t ship_id = HACCESS::Ship_Icon_Ship_Id_(i);
            int16_t node_idx = MOX::_ship_icon[i].node_idx;
            int16_t owner = HACCESS::Ship_Owner_(ship_id);
            bool is_not_player_ship = (owner != MOX::_PLAYER_NUM);

            bool is_player_ship_moving = false;
            if (owner == MOX::_PLAYER_NUM) {
                int16_t abs_loc = HAROLD::Absolute_Location_(MOX::_ship[ship_id].location);
                if (abs_loc != MOX::_NUM_STARS) {
                    is_player_ship_moving = true;
                }
            }

            bool is_enemy_moving_to_colony = false;
            if (is_not_player_ship && MOX::_ship[ship_id].status != 0) {
                if (Enemy_Ship_Heading_Toward_Our_Colony_(node_idx)) {
                    is_enemy_moving_to_colony = true;
                }
            }

            bool is_selected = false;
            if (MOVEBOX::Moveable_Box_Selected_(2) != 0) {
                if (HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack) == node_idx) {
                    is_selected = true;
                }
            }

            if (is_player_ship_moving || is_enemy_moving_to_colony || is_selected) {
                int16_t location = MOX::_ship[ship_id].location;
                if (location >= 500 && location < 1500) {
                    int16_t abs_loc = HAROLD::Absolute_Location_(location);
                    if (abs_loc != MOX::_NUM_STARS) {
                        int16_t star_x, star_y;
                        int16_t ship_x, ship_y;

                        MAINSCR::Get_Star_Draw_Coords_(abs_loc, &star_x, &star_y);
                        SHIPS::Get_Ship_Icon_Coords_(i, &ship_x, &ship_y);

                        s_colors* colors;
                        if (is_enemy_moving_to_colony || is_not_player_ship) {
                            colors = &MAINSCR::_ship_direction_line_red_colors;
                        } else {
                            colors = &MAINSCR::_ship_direction_line_green_colors;
                        }

                        SHIPS::Draw_Ship_Destination_Line_(star_x, star_y, ship_x, ship_y, colors);

                        if (location >= 500 && location < 1000) {
                            int16_t zoom = HAROLD::Map_Scale_To_Zoom_Level_();
                            SHIPS::Print_Eta_On_Ship_Icon_(node_idx, zoom, ship_x, ship_y);
                        }
                    }
                }
            }
        }
    }

    void __cdecl Print_Eta_On_Ship_Icon_(int16_t node_idx, int16_t style_toggle, int16_t x_offset, int16_t y_offset) {
        int16_t ship_idx;
        int16_t player_idx;
        s_animation_header* pict_seg;
        int16_t width;
        int16_t height;
        s_colors colors;
        char buffer[80];
        const char* format;
        uint32_t turns_left;

        ship_idx = MOX::_ship_node[node_idx].ship_idx;
        player_idx = HACCESS::Ship_Stack_Owner_(node_idx);
        pict_seg = SHIPS::Get_Ship_Icon_Pict_Seg_(player_idx, 0);

        width = animate::Get_Width_(pict_seg);
        height = animate::Get_Height_(pict_seg);

        HAROLD::Set_Player_Font_Style_(player_idx, 0);
        fonts::Get_Colors_(&colors);

        // If style_toggle is 0 or 1, use font style 1, otherwise use 0
        fonts::Set_Font_Style_(style_toggle <= 1 ? 1 : 0, &colors);

        turns_left = (uint8_t)MOX::_ship[ship_idx].turns_left;
        format = HAROLD::H_Message_(307 /* HESTR_133_ETA_D */);

        _sprintf(buffer, format, turns_left);

        fonts::Print_Right_(width + x_offset, height + y_offset, buffer);
    }

    void __cdecl Get_Ship_Icon_Coords_In_Space_(int16_t icon_id, int16_t* x, int16_t* y) {
        int16_t ship_id = HACCESS::Ship_Icon_Ship_Id_(icon_id);
        int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();

        int8_t* width_ptr = &MOX::_ship_icon_width[zoom_level];
        int8_t* height_ptr = &MOX::_ship_icon_height[zoom_level];

        SHIPS::Get_Ship_Icon_Dimensions_(zoom_level, width_ptr, height_ptr);

        // s_ship_data size is 129 (0x81)
        s_ship_data* ship = &MOX::_ship[ship_id];

        int16_t base_x = (int16_t)(ship->x - MOX::_cur_map_x);
        int16_t scaled_x = HAROLD::Get_Scaled_Value_(base_x);
        // icon_width is shifted right by 1 to center the icon (division by 2)
        *x = (scaled_x - (int16_t)(*width_ptr >> 1)) + 21 /* 0x15 */;

        int16_t base_y = (int16_t)(ship->y - MOX::_cur_map_y);
        int16_t scaled_y = HAROLD::Get_Scaled_Value_(base_y);
        // icon_height is shifted right by 1 to center the icon (division by 2)
        *y = (scaled_y - (int16_t)(*height_ptr >> 1)) + 21 /* 0x15 */;
    }

    /**
     * @address 0041f370
     */
    /**
     * @address 0041f680
     */
    void __cdecl Draw_Ship_Destination_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_colors* colors) {
        int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();
        int zoom_index = 3 - (int)zoom_level;
        if (zoom_index <= 0) {
            zoom_index = 0;
        }

        int16_t target_x;
        int16_t target_y;

        // Check if current screen is Galaxy (0), System (12/0x0C), or Special (39/0x27)
        if (MOX::_current_screen == 0 || MOX::_current_screen == 0x27 || MOX::_current_screen == 0x0C) {
            SHIPS::Get_Ship_Icon_Dimensions_((int16_t)zoom_index, &MOX::_ship_icon_width[zoom_index], &MOX::_ship_icon_height[zoom_index]);

            // Adjust destination to the center of the ship icon
            target_x = x2 + (int16_t)((uint8_t)MOX::_ship_icon_width[zoom_index] >> 1);
            target_y = y2 + (int16_t)((uint8_t)MOX::_ship_icon_height[zoom_index] >> 1);
        } else {
            // Default fixed offset for other screens
            target_x = x2 + 4;
            target_y = y2 + 4;
        }

        SHIPS::Draw_Directional_Multi_Colored_Line_(
            target_x,
            target_y,
            x1,
            y1,
            colors,
            MOX::_multi_colored_line_start
        );
    }

    int16_t __cdecl Overlapped_Ship_Icon_Button_(int16_t* p_icon_idx, int16_t threshold_x, int16_t threshold_y) {
        int16_t current_idx = *p_icon_idx;
        int16_t result = 0;

        int diff_x = abs((int)MOX::_icon_overlay_x - (int)MOX::_ship_icon[current_idx].x);
        int diff_y = abs((int)MOX::_icon_overlay_y - (int)MOX::_ship_icon[current_idx].y);

        if (diff_x > (int)threshold_x || diff_y > (int)threshold_y) {
            MOX::_icon_overlay_x = MOX::_ship_icon[current_idx].x;
            MOX::_icon_overlay_y = MOX::_ship_icon[current_idx].y;
            result = 0;
        } else {
            int16_t overlap_indices[50];
            int16_t num_overlaps = 0;
            int16_t stop_searching = 0;

            Set_Memory_(overlap_indices, 100 /* 50 * sizeof(short) */, 0);

            HACCESS::Ship_Icon_Star_Id_(current_idx);

            for (int16_t i = 0; i < MOX::_ship_icon_count; ++i) {
                if (stop_searching != 0) {
                    break;
                }

                int d_x = abs((int)MOX::_ship_icon[i].x - (int)MOX::_ship_icon[current_idx].x);
                int d_y = abs((int)MOX::_ship_icon[i].y - (int)MOX::_ship_icon[current_idx].y);

                if (d_x < (int)threshold_x && d_y < (int)threshold_y) {
                    overlap_indices[num_overlaps] = i;
                    num_overlaps++;

                    if (num_overlaps >= 50) {
                        stop_searching = 1;
                    }
                }
            }

            if (num_overlaps > 0) {
                for (int16_t j = 0; j < num_overlaps - 1; ++j) {
                    int16_t idx1 = overlap_indices[j];
                    int16_t idx2 = overlap_indices[j + 1];

                    Swap_Structure_(&MOX::_ship_icon[idx1], &MOX::_ship_icon[idx2], sizeof(s_ship_icon));

                    int16_t temp_stack = MOX::_ship_stack_start[idx1];
                    MOX::_ship_stack_start[idx1] = MOX::_ship_stack_start[idx2];
                    MOX::_ship_stack_start[idx2] = temp_stack;
                }

                result = 1;
                *p_icon_idx = overlap_indices[0];
            }
        }

        return result;
    }

    void __cdecl Build_Ship_Icons_() {
        SHIPSTAK::Find_Ship_Stacks_();

        allocate::Mark_Block_(MOX::_global_data_seg);
        s_build_ship_icon_item* build_items = reinterpret_cast<s_build_ship_icon_item*>(
            allocate::Allocate_Next_Block_(MOX::_global_data_seg, 0x168)
        );
        uint8_t* slot_counts = reinterpret_cast<uint8_t*>(
            allocate::Allocate_Next_Block_(MOX::_global_data_seg, 0x120)
        );
        allocate::Release_Block_(MOX::_global_data_seg);

        memset(slot_counts, 0x02, 0x120);
        memset(MOX::_ship_icon, 0xFF, sizeof(s_ship_icon) * 572);
        memset(build_items, 0xFF, 0x168);

        MOX::_ship_icon_count = 0;
        bool need_sort = false;

        if (MOX::_ship_stack_count > 0) {
            for (int16_t stack_iter = 0; stack_iter < MOX::_ship_stack_count; ++stack_iter) {
                int16_t stack_id = MOX::_ship_stack_start[stack_iter];
                int16_t x = HACCESS::Ship_Stack_X_(stack_id);
                int16_t y = HACCESS::Ship_Stack_Y_(stack_id);
                int8_t owner = static_cast<int8_t>(HACCESS::Ship_Stack_Owner_(stack_id));

                int16_t ship_idx = MOX::_ship_node[stack_id].ship_idx;
                int16_t dest_star_id = HAROLD::XY_To_Star_Id_(x, y);
                int16_t star_id = SHIPSTAK::Ship_Stack_Star_Id_(stack_id);

                if (MOX::_ship[ship_idx].status == 1) {
                    need_sort = true;
                    MOX::_ship_icon[MOX::_ship_icon_count].node_idx = stack_id;
                    MOX::_ship_icon[MOX::_ship_icon_count].star_idx = star_id;
                    MOX::_ship_icon[MOX::_ship_icon_count].stack_slot = 5;
                    MOX::_ship_icon_count++;
                } else {
                    if (star_id > -1 && star_id < 500) {
                        s_build_ship_icon_item* item = &build_items[star_id];
                        int8_t item_player_idx = item->player_idx;

                        if (item_player_idx == -1) {
                            item->player_idx = owner;
                            item->ship_idx = stack_id;
                            item->icon_idx = MOX::_ship_icon_count;

                            MOX::_ship_icon[MOX::_ship_icon_count].node_idx = stack_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].star_idx = star_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].stack_slot = 0;
                            MOX::_ship_icon_count++;
                        } else if (item_player_idx != MOX::_PLAYER_NUM && owner == MOX::_PLAYER_NUM) {
                            MOX::_ship_icon[item->icon_idx].stack_slot = slot_counts[star_id];
                            uint8_t slot = static_cast<uint8_t>(slot_counts[star_id] + 1);
                            if (slot > 3) slot = 3;
                            slot_counts[star_id] = slot;

                            item->player_idx = static_cast<int8_t>(MOX::_PLAYER_NUM);
                            item->ship_idx = stack_id;
                            item->icon_idx = MOX::_ship_icon_count;

                            MOX::_ship_icon[MOX::_ship_icon_count].node_idx = stack_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].star_idx = star_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].stack_slot = 0;
                            MOX::_ship_icon_count++;
                        } else {
                            MOX::_ship_icon[MOX::_ship_icon_count].node_idx = stack_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].star_idx = star_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].stack_slot = slot_counts[star_id];

                            uint8_t slot = static_cast<uint8_t>(slot_counts[star_id] + 1);
                            if (slot > 3) slot = 3;
                            slot_counts[star_id] = slot;

                            MOX::_ship_icon_count++;
                        }
                    } else {
                        if (star_id - dest_star_id == 1000) {
                            int16_t icon_index = -1;
                            for (int16_t j = 0; j < MOX::_ship_icon_count; ++j) {
                                if (MOX::_ship_icon[j].stack_slot == 1 &&
                                    MOX::_ship_icon[j].node_idx == stack_id &&
                                    MOX::_ship_icon[j].star_idx == star_id) {
                                    icon_index = j;
                                    break;
                                }
                            }

                            // Bug in original code: if star_id >= 1000, this writes out of bounds into build_items.
                            build_items[star_id].player_idx = owner;
                            build_items[star_id].ship_idx = MOX::_ship_icon[icon_index].node_idx;
                            MOX::_ship_icon[icon_index].stack_slot = 0;
                        } else if (star_id >= 1000 && dest_star_id > -1) {
                            MOX::_ship_icon[MOX::_ship_icon_count].node_idx = stack_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].star_idx = star_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].stack_slot = 1;
                            MOX::_ship_icon_count++;
                        } else if (dest_star_id != star_id) {
                            need_sort = true;
                            MOX::_ship_icon[MOX::_ship_icon_count].node_idx = stack_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].star_idx = star_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].stack_slot = 5;
                            MOX::_ship_icon_count++;
                        } else {
                            MOX::_ship_icon[MOX::_ship_icon_count].node_idx = stack_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].star_idx = star_id;
                            MOX::_ship_icon[MOX::_ship_icon_count].stack_slot = slot_counts[star_id];

                            uint8_t slot = static_cast<uint8_t>(slot_counts[star_id] + 1);
                            if (slot > 3) slot = 3;
                            slot_counts[star_id] = slot;

                            MOX::_ship_icon_count++;
                        }
                    }
                }
            }
        }

        if (MOX::_ship_icon_count > 0 && need_sort) {
            _qsort(MOX::_ship_icon, MOX::_ship_icon_count, 12, SHIPS::qsort_ship_icon_);
        }
    }

    int16_t __cdecl Deploy_Ships_To_Attack_Antares_(void) {
        int16_t selected_ship_indices[500];
        s_ship_move_info move_info;
        int16_t selected_ship_count = 0;

        memset(selected_ship_indices, 0xFF, sizeof(selected_ship_indices));

        int16_t current_node = MOX::_ship_stack_start[MOX::_fleet_box_ship_stack];
        while (current_node != -1) {
            if (MOX::_ship_node[current_node].selected != 0) {
                selected_ship_indices[selected_ship_count++] = MOX::_ship_node[current_node].ship_idx;
            }

            current_node = MOX::_ship_node[current_node].next_node;
        }

        SHIPMOVE::Ships_Try_To_Move_To_(selected_ship_indices, MOX::_NUM_STARS, &move_info);
        SHIPMOVE::Make_Ships_Move_To_(selected_ship_indices, MOX::_NUM_STARS, &move_info);
        return selected_ship_count;
    }

} // namespace SHIPS