#include "pch.h"

namespace MAINSCR {
    bool __cdecl Is_Main_Screen_Palette_() {
        uint8_t screen = static_cast<uint8_t>(MOX::_previous_screen);

        if (screen == SCREEN_MAIN ||
            screen == 0xBU /* SCREEN_UNK2 */ ||
            screen == SCREEN_COLONIZATION_IN_MAIN ||
            screen == SCREEN_BUILDING_OUTPOSTS_IN_MAIN ||
            screen == SCREEN_SCRAP_FREIGHTERS ||
            screen == SCREEN_DISTANCE_BETWEEN_STARS ||
            screen == SCREEN_TECH_CHANGE ||
            screen == SCREEN_GAME ||
            screen == SCREEN_REPORTS ||
            screen == SCREEN_SHOW_COMMAND_POINTS ||
            screen == SCREEN_TURN_SUMMARY ||
            screen == SCREEN_MAIN_NEXT_TURN) {
            return true;
        }

        return false;
    }

    void __cdecl Draw_Stars_() {
        _n_twinkling_stars = 0;

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            if (MOX::_star[star_idx].spectral_class != STAR_CLASS_BLACK_HOLE && Star_On_Screen_(star_idx)) {
                Draw_A_Star_(star_idx);
            }
        }
    }

    void __cdecl Update_Star_Owners_() {
        for (int16_t i = 0; i < MOX::_NUM_STARS; i++) {
            int16_t owner_id = HAROLD::Star_Owner_(i, MOX::_PLAYER_NUM);
            MOX::_star[i].owner = static_cast<int8_t>(owner_id);
        }
    }

    void __cdecl Get_PointXY_Draw_Coords_(int16_t x, int16_t y, int16_t* out_x, int16_t* out_y) {
        int16_t scaled_x = HAROLD::Get_Scaled_Value_(static_cast<int16_t>(x - MOX::_cur_map_x));
        *out_x = scaled_x + 0x15;

        int16_t scaled_y = HAROLD::Get_Scaled_Value_(static_cast<int16_t>(y - MOX::_cur_map_y));
        *out_y = scaled_y + 0x15;
    }

    s_animation_header* __cdecl Get_Star_Picture_Seg_(int8_t spectral_class, int8_t pict_type, int8_t size) {
        int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();
        int16_t class_offset = (int16_t)spectral_class * 6;

        int16_t result_ax = zoom_level;
        if (spectral_class != STAR_CLASS_BLACK_HOLE) {
            result_ax += (int16_t)size;
        }
        result_ax += class_offset;

        int32_t final_index = (int32_t)result_ax;

        MOX::_star_seg[final_index] = reinterpret_cast<s_animation_header*>(Buffer_Reload_(
            s_BUFFER0_LBX,
            final_index + 148,
            MOX::_global_cache_seg
        ));

        return MOX::_star_seg[final_index];
    }

    void __cdecl Get_Star_Draw_Coords_(int16_t star_idx, int16_t* out_x, int16_t* out_y) {
        const s_star_data& star = MOX::_star[star_idx];

        int16_t rel_x = star.x - MOX::_cur_map_x;
        *out_x = HAROLD::Get_Scaled_Value_(rel_x) + 0x15;

        int16_t rel_y = star.y - MOX::_cur_map_y;
        *out_y = HAROLD::Get_Scaled_Value_(rel_y) + 0x15;
    }

    bool __cdecl Star_On_Screen_(int16_t star_idx) {
        int16_t x;
        int16_t y;

        Get_Star_Draw_Coords_(star_idx, &x, &y);

        if (x > 0x15 && x < 0x20f && y > 0x15 && y < 0x1a5) {
            return true;
        }

        return false;
    }

    void Draw_Mini_Main_Screen_() {
        if (MOX::_at_least_one_tactical_combat != 0) {
            MOX::_at_least_one_tactical_combat = 0;
            Fade_Into_Main_Screen_();
        }

        uint16_t saved_scale = (uint16_t)MOX::_cur_map_scale;
        uint16_t saved_zoom = MOX::_zoom_count;
        uint16_t saved_x = (uint16_t)MOX::_cur_map_x;
        uint16_t saved_y = (uint16_t)MOX::_cur_map_y;

        if (MOX::_dont_draw_main_screen_to_back == 0) {
            Draw_Main_Screen_Filled_(1);
        } else {
            Draw_Main_Screen_Filled_(0);
            MOX::_dont_draw_main_screen_to_back = 0;
        }

        Draw_Main_Main_Screen_();

        int32_t temp_val = (int32_t)MOX::_multi_colored_line_start + 1;
        int32_t sign_mask = temp_val >> 31;
        int32_t abs_val = (temp_val ^ sign_mask) - sign_mask;
        int32_t wrapped_val = ((abs_val & 7) ^ sign_mask) - sign_mask;
        MOX::_multi_colored_line_start = (int16_t)wrapped_val;

        MOX::_cur_map_scale = (int16_t)saved_scale;
        MOX::_zoom_count = saved_zoom;
        MOX::_cur_map_x = (int16_t)saved_x;
        MOX::_cur_map_y = (int16_t)saved_y;

        if (MOX::_settings.end_of_turn_wait == 0) {
            if (_check_next_turn_exit_field != 0) {
                if (Mouse_Button_() != 0) {
                    MOX::_auto_turn_exit = 1;
                }
            }
        }

        if (_check_next_turn_exit_field != 0) {
            animate::Draw_(0x220, 0x1B9, MOX::_darkened_turn_button_seg);
        }

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            MOX::_skip_fields = 1;
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }
    }

    void Draw_Mini_Main_Offscreen_() {
        fields::Disable_Draw_All_Fields_();
        video::Set_Page_Off_();
        MAINSCR::Draw_Mini_Main_Screen_();
        RUSS::Mox_Sync_Update_();
        fields::Enable_Draw_All_Fields_();
    }

    void __cdecl Draw_A_Star_(int16_t star_idx) {
        int16_t zoom_size = HAROLD::Map_Scale_Star_Size_To_Zoom_Level_(star_idx);
        s_star_data* star = &MOX::_star[star_idx];

        s_animation_header* star_seg = Get_Star_Picture_Seg_(star->spectral_class, star->pict_type, star->size);

        int16_t screen_x;
        int16_t screen_y;
        Get_Star_Draw_Coords_(star_idx, &screen_x, &screen_y);

        screen_x -= zoom_size / 2;
        screen_y -= zoom_size / 2;

        if (static_cast<int8_t>(star->twinkling1) < 0) {
            animate::Reset_Animation_Frame_(star_seg);
            animate::Draw_(screen_x, screen_y, star_seg);
        } else {
            star->twinkling1++;

            int16_t frame_count = animate::Get_Animation_Frame_Count_(star_seg);

            if (static_cast<int8_t>(star->twinkling2) <= static_cast<int8_t>(star->twinkling1)) {
                int32_t remaining = static_cast<int32_t>(_n_twinkling_stars) - 1;
                _n_twinkling_stars = static_cast<int16_t>((remaining < 1) ? 0 : remaining);

                star->twinkling1 = -1;
                star->twinkling2 = -1;
            }

            int32_t current_frame = (static_cast<int16_t>(star->twinkling1) / 2) % frame_count;
            animate::Set_Animation_Frame_(star_seg, current_frame);
            animate::Draw_(screen_x, screen_y, star_seg);
        }
    }

    void __cdecl Print_Star_Names_() {
        int16_t window_status;
        int8_t star_type;
        uint32_t star_data;
        char star_name[32];

        star_type = 0;
        star_data = 0;

        MISC::Save_Window_Status_(&window_status);
        graphics::Set_Window_(0x16, 0x16, 0x20F, 0x1A5);
        video::Clip_On_();

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            if (MOX::_star[i].spectral_class != STAR_CLASS_BLACK_HOLE) {
                Get_Star_Name_(i, 0, &star_type, star_name, &star_data);

                if (star_name[0] != '\0') {
                    Print_Star_Name(i, star_type, star_name, reinterpret_cast<void*>(star_data));
                }
            }
        }

        MISC::Restore_Window_Status_(window_status);
    }

    void __cdecl Draw_ETA_Destination_Line_() {
        int16_t x, y;
        int16_t dest_x, dest_y;
        s_colors color_buffer;

        int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();
        int32_t zoom_scale = 3 - (int32_t)zoom_level;
        if (zoom_scale < 0) {
            zoom_scale = 0;
        }

        int16_t ship_stack_node = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
        int16_t ship_icon_id = HACCESS::Ship_Icon_Id_From_Node_Ptr_(ship_stack_node);

        SHIPS::Get_Ship_Icon_Coords_(ship_icon_id, &x, &y);

        SHIPS::Get_Ship_Icon_Dimensions_(zoom_scale, &MOX::_ship_icon_width[zoom_scale], &MOX::_ship_icon_height[zoom_scale]);

        x += (uint8_t)MOX::_ship_icon_width[zoom_scale] / 2;
        y += (uint8_t)MOX::_ship_icon_height[zoom_scale] / 2;

        Get_Star_Draw_Coords_(MOX::_eta_star_id, &dest_x, &dest_y);

        if (MOX::_g_ship_move_info.moving != 0 && MOX::_g_ship_move_info.turns_left != 0) {
            color_buffer = _ship_direction_line_green_colors;

            SHIPS::Draw_Directional_Multi_Colored_Line_(x, y, dest_x, dest_y, &color_buffer, MOX::_multi_colored_line_start);
        } else if ((MOX::_g_ship_move_info.blackhole_blocks != 0 || MOX::_g_ship_move_info.hyperspace_flux != 0) ||
                 (MOX::_g_ship_move_info.out_of_range != 0 && MOX::_g_ship_move_info.immobile == 0)) {
            color_buffer = _ship_direction_line_red_colors;

            SHIPS::Draw_Directional_Multi_Colored_Line_(x, y, dest_x, dest_y, &color_buffer, MOX::_multi_colored_line_start);
        }
    }

    void __cdecl Draw_Main_Main_Screen_() {
        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Wormhole_Links_();
        MOX::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Relocation_Links_();
        MOX::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Stars_();
        MOX::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Gate_Icons_();
        MOX::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Print_Star_Names_();
        MOX::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Black_Holes_();
        MOX::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Ship_Icons_(true);
        MOX::Mox_Update_();

        graphics::Reset_Window_();
        video::Clip_Off_();
        Print_Main_Screen_Data_();
        MOX::Mox_Update_();

        if (MOX::_game_type != 0) {
            Draw_Diplomacy_Request_Lights_();
            MOX::Mox_Update_();
        }
    }

    /**
     * @address 004cd550
     */
    void __cdecl Update_Ship_Icons_In_Between_Turns_() {
        Update_Star_Owners_();
        SHIPS::Build_Ship_Icons_();
        SHIPS::Set_Ship_Icon_XYs_();
    }

    void __cdecl Draw_Wormhole_Links_() {
        int16_t window_status;
        MISC::Save_Window_Status_(&window_status);

        graphics::Set_Window_(0x16, 0x16, 0x20F, 0x1A5);
        video::Clip_On_();

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            int8_t wormhole_id = MOX::_star[star_idx].wormhole_star_id;
            if (wormhole_id != -1) {
                bool is_visible = HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, star_idx);
                if (!is_visible) {
                    is_visible = HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM);
                }

                if (is_visible) {
                    int16_t x1, y1, x2, y2;
                    MAINSCR::Get_Star_Draw_Coords_(star_idx, &x1, &y1);
                    MAINSCR::Get_Star_Draw_Coords_(static_cast<int16_t>(wormhole_id), &x2, &y2);
                    line::Line_(x1, y1, x2, y2, 4);
                }
            }
        }

        MISC::Restore_Window_Status_(window_status);
    }

    void __cdecl Draw_Paralax_() {
        int16_t window_status;
        MISC::Save_Window_Status_(&window_status);

        // Define active window for parallax background
        graphics::Set_Window_(22, 22, 527, 421); // 0x16, 0x16, 0x20F, 0x1A5
        video::Clip_On_();

        // Far Stars - Layer 0
        MOX::_far_star_seg = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_STARBG_LBX, 0, MOX::_global_cache_seg));
        animate::Draw_(MOX::_far_star_x, MOX::_far_star_y, MOX::_far_star_seg);
        animate::Draw_(MOX::_far_star_x - 640, MOX::_far_star_y, MOX::_far_star_seg);
        animate::Draw_(MOX::_far_star_x - 640, MOX::_far_star_y - 480, MOX::_far_star_seg);
        animate::Draw_(MOX::_far_star_x, MOX::_far_star_y - 480, MOX::_far_star_seg);

        // Mid Stars - Layer 1
        MOX::_mid_star_seg = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_STARBG_LBX, 1, MOX::_global_cache_seg));
        animate::Draw_(MOX::_mid_star_x, MOX::_mid_star_y, MOX::_mid_star_seg);
        animate::Draw_(MOX::_mid_star_x - 640, MOX::_mid_star_y, MOX::_mid_star_seg);
        animate::Draw_(MOX::_mid_star_x - 640, MOX::_mid_star_y - 480, MOX::_mid_star_seg);
        animate::Draw_(MOX::_mid_star_x, MOX::_mid_star_y - 480, MOX::_mid_star_seg);

        // Near Stars - Layer 2
        MOX::_near_star_seg = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_STARBG_LBX, 2, MOX::_global_cache_seg));
        animate::Draw_(MOX::_near_star_x, MOX::_near_star_y, MOX::_near_star_seg);
        animate::Draw_(MOX::_near_star_x - 640, MOX::_near_star_y, MOX::_near_star_seg);
        animate::Draw_(MOX::_near_star_x - 640, MOX::_near_star_y - 480, MOX::_near_star_seg);
        animate::Draw_(MOX::_near_star_x, MOX::_near_star_y - 480, MOX::_near_star_seg);

        MISC::Restore_Window_Status_(window_status);
    }

    void __cdecl Draw_Relocation_Links_() {
        int16_t window_status;
        s_colors colors = { 0x6e, 0x6f, 0x70, 0x70, 0x6e, 0x6f, 0x70, 0x70 };

        MISC::Save_Window_Status_(&window_status);
        graphics::Set_Window_(0x16, 0x16, 0x20F, 0x1A5);
        video::Clip_On_();

        if (MOX::_settings.show_relocation_lines != 0) {
            for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
                if (HACCESS::Star_Has_Relocation_(star_idx, MOX::_PLAYER_NUM)) {
                    int16_t x1, y1, x2, y2;
                    MAINSCR::Get_Star_Draw_Coords_(star_idx, &x1, &y1);
                    int16_t dest_idx = HACCESS::Relocation_(star_idx, MOX::_PLAYER_NUM);
                    MAINSCR::Get_Star_Draw_Coords_(dest_idx, &x2, &y2);
                    SHIPS::Draw_Directional_Multi_Colored_Line_(x1, y1, x2, y2, &colors, MOX::_multi_colored_line_start);
                }
            }
        }

        MISC::Restore_Window_Status_(window_status);
    }

    void __cdecl Draw_Black_Holes_() {
        int16_t zoom_dist[4] = { 0x27, 0x21, 0x21, 0x18 };
        int16_t window_status;
        int16_t star_idx;
        int16_t bh_anim_idx = 0;
        int16_t x, y;

        int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();
        MISC::Save_Window_Status_(&window_status);
        graphics::Set_Window_(22, 22, 527, 421); // 0x16, 0x16, 0x20f, 0x1a5
        video::Clip_On_();

        if (MOX::_NUM_STARS > 0) {
            for (star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
                auto& star = MOX::_star[star_idx];

                if (star.spectral_class == STAR_CLASS_BLACK_HOLE) {
                    Get_Star_Draw_Coords_(star_idx, &x, &y);

                    int16_t offset = zoom_dist[zoom_level] / 2;
                    x -= offset;
                    y -= offset;

                    s_animation_header* seg = Get_Star_Picture_Seg_(star.spectral_class, star.pict_type, star.size);

                    uint8_t count = MOX::_black_hole_anim_count[bh_anim_idx];
                    int16_t num_frames = animate::Get_Animation_Frame_Count_(seg);

                    // Update and wrap animation counter
                    uint8_t new_count = (count + 1) % (num_frames * 2);
                    MOX::_black_hole_anim_count[bh_anim_idx] = new_count;

                    animate::Set_Animation_Frame_(seg, new_count / 2);
                    animate::Draw_(x, y, seg);

                    // Clamp black hole animation index for the next star to array capacity
                    bh_anim_idx++;
                    if (bh_anim_idx >= 10) {
                        bh_anim_idx = 10;
                    }
                }
            }
        }

        MISC::Restore_Window_Status_(window_status);
    }

    void __cdecl Draw_A_Gate_Icon_(int16_t star_idx) {
        if (!HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, star_idx)) {
            return;
        }

        int16_t star_size = HAROLD::Map_Scale_Star_Size_To_Zoom_Level_(star_idx);
        int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();
        int16_t zoom_offset = (zoom_level == 0) ? 2 : 3;

        // These calls are in disassembly but return value is ignored.
        HAROLD::Player_Has_Stargate_(MOX::_PLAYER_NUM, star_idx);
        HAROLD::Player_Has_Jumpgate_(MOX::_PLAYER_NUM, star_idx);

        bool has_artemis = HAROLD::Player_Has_Artemis_Net_(&MOX::_player[MOX::_PLAYER_NUM], &MOX::_star[star_idx]);
        bool has_portal = HAROLD::Player_Has_Dimensional_Portal_(&MOX::_player[MOX::_PLAYER_NUM], &MOX::_star[star_idx]);

        allocate::Push_Block_(MOX::_screen_seg);

        s_animation_header* temp_icons[4];
        for (int i = 0; i < 4; ++i) {
            uint32_t size = bitmap::Get_Bitmap_Size_(18, 18);
            temp_icons[i] = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size);
            bitmap::Create_Blank_Bitmap_(18, 18, &temp_icons[i]->width);
        }

        int8_t icon_count = 0;
        int16_t max_icon_height = 0;

        if (has_portal) {
            void* portal_pic = Buffer_Reload_(s_BUFFER0_LBX, zoom_offset + 19, MOX::_global_cache_seg);
            MOX::gate_portal_icons[zoom_offset] = static_cast<s_animation_header*>(portal_pic);
            bitmap::Draw_Picture_To_Bitmap_(temp_icons[0], portal_pic);
            int16_t h = animate::Get_Height_((s_animation_header*)portal_pic);
            if (h > 0) {
                max_icon_height = h;
            }
            icon_count = 1;
        }

        if (has_artemis) {
            s_animation_header* artemis_pic = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, zoom_offset + 31, MOX::_global_cache_seg);
            MOX::gate_artemis_icons[zoom_offset] = artemis_pic;
            bitmap::Draw_Picture_To_Bitmap_(temp_icons[icon_count], artemis_pic);
            int16_t h = animate::Get_Height_(artemis_pic);
            if (h > max_icon_height) {
                max_icon_height = h;
            }
            icon_count++;
        }

        if (icon_count != 0) {
            int16_t star_x, star_y;
            Get_Star_Draw_Coords_(star_idx, &star_x, &star_y);

            int16_t draw_x, draw_y;
            int16_t off_x, off_y, act_w, act_h;
            int16_t y_base;

            switch (icon_count) {
            case 1:
                bitmap::Get_Bitmap_Actual_Size_(temp_icons[0], &off_x, &off_y, &act_w, &act_h);
                draw_x = star_x - act_w - 4;
                draw_y = (star_size / 2) - max_icon_height + star_y;
                if (has_portal) {
                    draw_y += 2;
                }
                animate::Draw_(draw_x, draw_y, temp_icons[0]);
                break;

            case 2:
                bitmap::Get_Bitmap_Actual_Size_(temp_icons[0], &off_x, &off_y, &act_w, &act_h);
                y_base = (star_size / 2) - max_icon_height;
                draw_y = y_base + star_y;
                if (has_portal) {
                    draw_y += 2;
                }
                animate::Draw_(star_x - act_w - 4, draw_y, temp_icons[0]);

                bitmap::Get_Bitmap_Actual_Size_(temp_icons[1], &off_x, &off_y, &act_w, &act_h);
                animate::Draw_(star_x + 4, y_base + star_y, temp_icons[1]);
                break;

            case 3:
            case 4:
            {
                uint32_t size = bitmap::Get_Bitmap_Size_(75, 20);
                s_animation_header* composite = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size);
                bitmap::Create_Blank_Bitmap_(75, 20, &composite->width);

                int16_t cursor_x = 0;
                for (int i = 0; i < icon_count; ++i) {
                    bitmap::Add_Picture_To_Bitmap_(cursor_x, 0, temp_icons[i], composite);
                    bitmap::Get_Bitmap_Actual_Size_(temp_icons[i], &off_x, &off_y, &act_w, &act_h);
                    cursor_x += act_w + 1;
                }

                bitmap::Get_Bitmap_Actual_Size_(composite, &off_x, &off_y, &act_w, &act_h);
                draw_x = star_x - (act_w / 2);
                draw_y = (star_size / 2) - max_icon_height + star_y;
                animate::Draw_(draw_x, draw_y, composite);
                break;
            }
            default:
                break;
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Draw_Gate_Icons_() {
        int16_t star_idx = 0;
        int16_t num_stars = MOX::_NUM_STARS;

        if (num_stars > 0) {
            do {
                Draw_A_Gate_Icon_(star_idx);
                star_idx++;
            } while (star_idx < num_stars);
        }
    }

    /**
     * @brief Returns a 3-character unit abbreviation string for research points based on current language.
     * 
     * @param out Buffer of at least 3 bytes to store the result (including null terminator).
     */
    void __cdecl Get_Research_Unit_String_(char *out) {
        const char *src;

        switch (MOX::_settings.language) {
            case 1: // French?
                src = s_FP_00558240;
                break;
            case 2: // German?
                src = s_PR_00558238;
                break;
            case 3: // Italian?
                src = s_PI_00559994;
                break;
            case 4: // Spanish?
                src = s_PR_00558238;
                break;
            default: // English (0) or others
                src = s_RP_00558248;
                break;
        }

        // Copies 3 bytes, typically 2 letters and the null terminator
        out[0] = src[0];
        out[1] = src[1];
        out[2] = src[2];
    }

    void __cdecl Draw_Diplomacy_Request_Lights_() {
        int16_t drawn_count = 0;
        uint8_t requesting_mask = NETMOX::Humans_Requesting_Diplomacy_();

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if ((requesting_mask >> (i & 0x1f)) & 1) {
                uint8_t color_idx = static_cast<uint8_t>(MOX::_player[i].color);
                s_animation_header* light_anim = _player_lights_seg[color_idx];

                // Calculate horizontal position based on width of first light segment
                int16_t base_width = animate::Get_Width_(_player_lights_seg[0]);
                int16_t x_pos = 0x1fa - (base_width * drawn_count);

                // Discarded calls from original code
                animate::Get_Width_(light_anim);
                animate::Get_Height_(light_anim);

                int16_t saved_frame = animate::Get_Current_Frame_(static_cast<s_animation_header*>(light_anim));

                // Render current animation frame for diplomacy alert
                animate::Set_Animation_Frame_(light_anim, static_cast<uint8_t>(_diplo_lights_anim_frame[color_idx]));
                animate::Draw_(x_pos, 5, light_anim);

                // Advance animation frame
                uint8_t next_frame = _diplo_lights_anim_frame[color_idx] + 1;
                _diplo_lights_anim_frame[color_idx] = next_frame;

                if (animate::Get_Animation_Frame_Count_(light_anim) <= static_cast<int16_t>(next_frame)) {
                    _diplo_lights_anim_frame[color_idx] = 0;
                }

                drawn_count++;

                // Restore previous animation frame for the segment
                animate::Set_Animation_Frame_(light_anim, saved_frame);
            }
        }
    }

    void __cdecl Draw_Nebulae_() {
        if (MOX::_NUM_NEBULAS == 0) {
            return;
        }

        s_nebula* nebula_list = MOX::_nebula;

        for (uint16_t i = 0; i < static_cast<uint16_t>(MOX::_NUM_NEBULAS); ++i) {
            int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();
            s_animation_header* sprite_ptr = MOX::_nebula_pict_seg[i][zoom_level];

            animate::Get_Width_(sprite_ptr);
            animate::Get_Height_(sprite_ptr);

            int16_t dx = nebula_list[i].x - MOX::_cur_map_x;
            int16_t screen_x = HAROLD::Get_Scaled_Value_(dx) + 0x15;

            int16_t dy = nebula_list[i].y - MOX::_cur_map_y;
            int16_t screen_y = HAROLD::Get_Scaled_Value_(dy) + 0x15;

            animate::Draw_(static_cast<int32_t>(screen_x), static_cast<int32_t>(screen_y), sprite_ptr);
        }
    }

    void __cdecl Draw_Ship_Icons_(bool force_draw) {
        int16_t selected_icon_id = -1;

        if (MOVEBOX::Moveable_Box_Selected_(2)) {
            int16_t node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
            selected_icon_id = HACCESS::Ship_Icon_Id_From_Node_Ptr_(node_ptr);
        }

        for (int16_t i = MOX::_ship_icon_count - 1; i >= 0; --i) {
            int16_t x, y;
            SHIPS::Get_Ship_Icon_Coords_(i, &x, &y);

            int16_t owner = HACCESS::Ship_Icon_Owner_(i);
            s_animation_header* pict_seg = SHIPS::Get_Ship_Icon_Pict_Seg_(owner, 0);

            MOX::_ship_icon[i].x = x;
            MOX::_ship_icon[i].y = y;

            if ((i != selected_icon_id && i != MAINSCR::_main_screen_ship_icon_scanned) || force_draw) {
                animate::Reset_Animation_Frame_(pict_seg);
                animate::Draw_(x, y, pict_seg);
            }
        }
    }

    void __cdecl Draw_Main_Screen_Filled_(int16_t should_copy_to_back) {
        graphics::Reset_Window_();
        video::Clip_Off_();
        graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
        graphics::Set_Window_(0, 0, 0x27F, 0x1DF);
        video::Clip_On_();

        Draw_Paralax_();

        MOX::_main_screen_seg = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_BUFFER0_LBX, 0, MOX::_global_cache_seg));
        animate::Draw_(0, 0, MOX::_main_screen_seg);

        Draw_Wormhole_Links_();
        Draw_Relocation_Links_();

        if (MOX::_game_type == 2 || MOX::_game_type == 3) {
            MOX::_mainscrn_diplo_lights_seg = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_BUFFER0_LBX, 0xD, MOX::_global_cache_seg));
            animate::Draw_(415, 0, MOX::_mainscrn_diplo_lights_seg);
        }

        if (MOX::_game_type == 2 || MOX::_game_type == 3 || MOX::_game_type == 1) {
            MOX::_mainscrn_plyr_done_lights_seg = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_BUFFER0_LBX, 0xD, MOX::_global_cache_seg));
            animate::Draw_(74, 0, MOX::_mainscrn_plyr_done_lights_seg);
        }

        video::Clip_Off_();
        graphics::Reset_Window_();

        graphics::Set_Window_(22, 22, 527, 421);
        video::Clip_On_();
        Draw_Nebulae_();

        video::Clip_Off_();
        graphics::Reset_Window_();

        if (should_copy_to_back == 1) {
            video::Copy_Off_To_Back_();
        }

        Print_Star_Names_();
        Print_Main_Screen_Data_();
    }

    void __cdecl Set_Post_Fitted_Font_Style_(uint8_t c1, uint8_t c2) {
        int16_t style = MISC::Get_Font_Style_();
        int16_t spacing = MISC::Get_Font_Spacing_();
        s_colors colors;

        MISC::Get_Mox_Font_Colors_(style, (char)c1, (char)c2, &colors);

        fonts::Set_Outline_Color_(2, 1);
        fonts::Set_Font_Style_Outline_(style, &colors);
        fonts::Set_Font_Spacing_(spacing);
    }

    void __cdecl Set_Popup_Draw_XY_(int16_t popup_type, int16_t ship_icon_idx, int16_t* out_w, int16_t* out_h) {
        int16_t draw_x;
        int16_t draw_y;

        if (popup_type == 2) {
            draw_x = MOX::_ship_icon[ship_icon_idx].x - 21;
            draw_y = MOX::_ship_icon[ship_icon_idx].y - 21;
        } else {
            Get_Star_Draw_Coords_(ship_icon_idx, &draw_x, &draw_y);
        }

        Popup_XY_(popup_type, draw_x, draw_y, out_w, out_h);
    }

    void __cdecl Popup_XY_(int16_t index, int16_t x, int16_t y, int16_t* out_w, int16_t* out_h) {
        if (x < 253 && y < 200) {
            *out_w = 527 - MOX::_moveable_box[index].width;
            if (index == 0) {
                *out_h = 421 - MOX::_moveable_box[0].height;
            } else {
                *out_h = 421;
            }
        } else if (x < 253 && y > 200) {
            *out_w = 527 - MOX::_moveable_box[index].width;
            *out_h = 22;
        } else if (x > 253 && y < 200) {
            *out_w = 22;
            if (index == 0) {
                *out_h = 421 - MOX::_moveable_box[0].height;
            } else {
                *out_h = 421;
            }
        } else {
            *out_w = 22;
            *out_h = 22;
        }
    }

    void __cdecl Print_Star_Name(int16_t star_id, int8_t simple_draw, char* star_name, void* buffer) {
        HAROLD::Map_Scale_To_Zoom_Level_();
        int16_t zoom_size = HAROLD::Map_Scale_Star_Size_To_Zoom_Level_(star_id);

        int16_t draw_x, draw_y;
        Get_Star_Draw_Coords_(star_id, &draw_x, &draw_y);

        int16_t text_center_x = draw_x + 1;
        int16_t half_zoom_size = zoom_size / 2;
        int16_t text_y = half_zoom_size + (draw_y - (int16_t)MOX::_star[star_id].size);

        if (simple_draw != 0) {
            int16_t width = fonts::Get_String_Width_(star_name);
            animate::Draw_(text_center_x - (width / 2), text_y, (s_animation_header*)buffer);
            return;
        }

        int16_t style = HAROLD::Zoom_Level_Font_Style_();
        s_colors colors;
        fonts::Get_Colors_(&colors);
        fonts::Set_Outline_Color_(2, 1);
        fonts::Set_Font_Style_Outline_(style, &colors);
        fonts::Set_Font_Spacing_(1);

        int16_t width = fonts::Get_String_Width_(star_name);
        int16_t start_x = text_center_x - (width / 2);

        if (MOX::_max_map_scale == MOX::_cur_map_scale) {
            if (start_x < 22) { // 0x16
                fonts::Print_(22, text_y, star_name);
            } else if (start_x + width > 527) { // 0x20f
                fonts::Print_Right_(527, text_y, star_name);
            } else {
                fonts::Print_Centered_(text_center_x, text_y, star_name);
            }
        } else {
            fonts::Print_Centered_(text_center_x, text_y, star_name);
        }

        int8_t surrender_to = MOX::_star[star_id].surrender_to[MOX::_PLAYER_NUM];
        if (static_cast<uint8_t>(surrender_to) < 8) {
            // Player colors derived from stack-initialized byte array
            static const uint8_t surrender_colors[8] = { 0x80, 0x7b, 0x78, 0x2c, 0x86, 0x9f, 0x83, 0x7e };

            HAROLD::Set_Player_Font_Style_(0, false);
            fonts::Set_Font_Spacing_(1);

            int16_t line_width = fonts::Get_String_Width_(star_name);
            uint8_t player_color_idx = MOX::_player[surrender_to].color;
            uint8_t draw_color = surrender_colors[player_color_idx];

            int16_t line_start_x = draw_x - (line_width / 2);
            int16_t font_height = fonts::Get_Font_Height_();
            int16_t line_y = half_zoom_size + (font_height - (int16_t)MOX::_star[star_id].size) + draw_y;

            line::Line_(line_start_x, line_y, line_width + line_start_x, line_y, draw_color);

            if (MOX::_cur_map_scale < 30) { // 0x1e
                uint32_t secondary_color = draw_color + 1;
                if (player_color_idx == 3 || player_color_idx == 5) {
                    secondary_color = draw_color - 3;
                }
                line::Line_(line_start_x, line_y + 1, line_width + line_start_x, line_y + 1, secondary_color);
            }
        }
    }

    void __cdecl Do_System_Popup_(int16_t system_id) {
        FLEETPOP::Set_Fleet_Movement_Icon_Selections_(0, 0);

        MOX::_moveable_box[0].off_flag = system_id;

        bool box_was_selected = (MOVEBOX::Moveable_Box_Selected_(2) != 0);

        if (box_was_selected) {
            MOVEBOX::Vanish_Fleet_Movement_Box_();
            SYS::Clear_System_Display_Loaded_Flag_();
        }

        SYS::Init_System_Display_(MOX::_moveable_box[0].off_flag);

        int16_t star_x, star_y;
        Get_Star_Draw_Coords_(system_id, &star_x, &star_y);

        Set_Popup_Draw_XY_(0, system_id, &MOX::_moveable_box[0].x, &MOX::_moveable_box[0].y);

        if (MOX::_settings.expanding_help != 0) {
            Draw_Exploding_Plasma_Box_(
                star_x,
                star_y,
                MOX::_moveable_box[0].x,
                MOX::_moveable_box[0].y,
                MOX::_moveable_box[0].width,
                MOX::_moveable_box[0].height,
                0
            );
        } else if (box_was_selected) {
            fields::Clear_Fields_();
            Clear_Main_Screen_Fields_();
            Draw_Mini_Main_Offscreen_();
        }

        MOVEBOX::Move_Box_To_Top_(0);
    }

    void __cdecl Fade_Into_Main_Screen_() {
        int16_t box_status = MOVEBOX::Moveable_Box_Selected_(0);

        if (_already_faded_into_main_screen != 0) {
            _already_faded_into_main_screen = 0;
            return;
        }

        MOX::Mox_Update_();
        MAINSCR2::_main_screen_fade_in_flag = 0;
        MAINSCR2::_force_main_screen_fade = 0;
        _already_faded_into_main_screen = 1;

        fields::Disable_Draw_All_Fields_();
        video::Set_Refresh_Stencil_();
        mouse::Set_Mouse_List_((uint8_t*)MOX::_mouse_list_none, 1);
        ERIC::Fast_Fade_Out_();
        MOX::Mox_Update_();

        fonts::Load_Palette_(0, 0, 255);
        MOX::Mox_Update_();
        palstore::Darken_Palette_(100);
        MOX::Mox_Update_();

        MAINSCR::Add_Map_Fields_();
        MOX::Mox_Update_();

        if (static_cast<int8_t>(box_status) != 0) {
            bool expanding_help_enabled = (MOX::_settings.expanding_help != 0);
            if (expanding_help_enabled) {
                MOX::_settings.expanding_help = 0;
            }
            MOX::Mox_Update_();
            MAINSCR::Do_System_Popup_(MOX::_moveable_box[0].off_flag);
            if (expanding_help_enabled) {
                MOX::_settings.expanding_help = 1;
            }
        }

        video::Set_Page_Off_();
        MAINSCR::Draw_Mini_Main_Screen_();
        MOX::Mox_Update_();
        palette::Clear_Palette_Changes_(0, 255);
        RUSS::Mox_Sync_Update_();

        mouse::Set_Mouse_List_((uint8_t*)MOX::_mouse_list_arrow, 1);

        bool skip_fade_in = false;
        if (MOX::_reloaded_ship_palette != 0) {
            if (MOVEBOX::Moveable_Box_Selected_(2) == 0) {
                ERIC::Set_Fade_Percent_(0);
                skip_fade_in = true;
            }
        }

        if (!skip_fade_in) {
            ERIC::Fast_Fade_In_();
            MOX::Mox_Update_();
        }

        MOX::_reloaded_ship_palette = 0;
        fields::Enable_Draw_All_Fields_();
    }

    void __cdecl Get_Star_Name_(int16_t star_idx, int8_t is_selected, int8_t* param_3, char* str, void* param_5) {
        bool galactic_lore = HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM);
        *str = '\0';
        *param_3 = 0;
        HAROLD::Map_Scale_To_Zoom_Level_();

        int16_t color_offset = (is_selected != 0) ? (int16_t)MOX::_selected_star_color_offset : 0;
        bool is_outpost = (bool)HAROLD::Star_Is_Outpost_Star_(star_idx);
        s_star_data* star_ptr = &MOX::_star[star_idx];
        int8_t owner = star_ptr->owner;

        auto apply_font_offsets = [&](int16_t font_style) {
            s_colors local_colors;
            s_colors* p_colors = MISC::Font_Colors2_(font_style, 1, 12);
            for (int i = 0; i < 8; ++i) local_colors.colors[i] = p_colors->colors[i];

            if (color_offset > 0) {
                int16_t shift = (color_offset * 9) / 5;
                for (int i = 1; i < 8; ++i) {
                    int val = (int)local_colors.colors[i] + shift;
                    if (val < (int)local_colors.colors[0]) val = (int)local_colors.colors[0];
                    if (val > 16) val = 16;
                    local_colors.colors[i] = (uint8_t)val;
                }
            }
            fonts::Set_Font_Style_(font_style, &local_colors);
        };

        if (!galactic_lore) {
            bool has_contact = (owner >= 0 && owner <= 7) && HAROLD::Contact_With_One_Colony_(star_idx);
            if (!has_contact) {
                if (!HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, star_idx)) return;

                char* src = star_ptr->name;
                char* dst = str;
                while ((*dst++ = *src++));

                int16_t font_style = HAROLD::Zoom_Level_Font_Style_();
                apply_font_offsets(font_style);

                if (is_outpost) {
                    MISC::Decrease_Font_Color_Range_(2);
                    strings::String_To_Lower_(str);
                }
                return;
            }
        }

        // Logic for Lore or Contacted
        if (galactic_lore && MOX::_PLAYER_NUM != owner && !HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, star_idx)) {
            _sprintf(str, HAROLD::s___s__00556ae4, star_ptr->name);
        } else {
            char* src = star_ptr->name;
            char* dst = str;
            while ((*dst++ = *src++));
        }

        if (owner >= 0 && owner <= 7) {
            HAROLD::Set_Player_Font_Style_(owner, (bool)is_selected);
        } else {
            int16_t font_style = HAROLD::Zoom_Level_Font_Style_();
            apply_font_offsets(font_style);
        }

        if (is_outpost) {
            MISC::Decrease_Font_Color_Range_(1);
            strings::String_To_Lower_(str);
        }

        if (HAROLD::N_Colony_Owners_(star_idx) > 1) {
            *param_3 = Print_Star_Name_To_Bitmap_(star_idx, str, param_5);
        }
    }

    bool __cdecl Print_Star_Name_To_Bitmap_(int16_t star_idx, char* str, void* out_bitmap_ptr) {
        int16_t owner_counts[8];
        int16_t num_owners;
        s_colors standard_colors;
        s_colors player_colors;

        bool is_omniscient = HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM);
        HAROLD::Colony_Owners_(star_idx, owner_counts, &num_owners, !is_omniscient);

        if (num_owners <= 1) {
            return false;
        }

        int16_t total_colony_count = 0;
        for (int i = 0; i < 8; ++i) {
            total_colony_count += owner_counts[i];
        }

        fonts::Get_Colors_(&standard_colors);
        fonts::Set_Font_Spacing_(1);
        allocate::Push_Block_(MOX::_screen_seg);

        uint32_t bitmap_size = bitmap::Get_Bitmap_Size_(200, 20);
        void* bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);
        *reinterpret_cast<void**>(out_bitmap_ptr) = bitmap;

        bitmap::Create_Blank_Bitmap_(200, 20, bitmap);
        fonts::Print_To_Bitmap_(0, 0, str, bitmap);

        int16_t current_x = 0;
        for (int16_t i = 0; i < 8; ++i) {
            if (owner_counts[i] > 0) {
                int16_t width, height, d1, d2;
                MISC::Get_Bitmap_String_Range_(owner_counts[i], total_colony_count, &width, &d1, &d2, &height, str);
                HAROLD::Get_Player_Font_Colors_(i, fonts::_current_font_style, &player_colors, false);
                MISC::Swap_Bitmap_Colors_(current_x, 0, current_x + width, height, &standard_colors, &player_colors, 8, bitmap);
                current_x += width;
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
        return true;
    }

    void Add_Map_Fields_() {
        if (MOX::_zoom_in == 1) {
            Clear_Main_Screen_Fields_();
            for (int i = 0; i < 5; ++i) {
                MOX::_moveable_box[i].field = -1000;
                MOX::_moveable_box[i].field8_0x10 = -1000;
            }

            _zoom_in_button = fields::Add_Button_Field_(244, 428, s__00551100, MOX::_main_zoom_in_button_seg, s_plus_00551118, 0x28);
            _zoom_out_button = fields::Add_Button_Field_(244, 455, s__00551100, MOX::_main_zoom_out_button_seg, s_minus_0055111c, 0x28);
            _whole_screen_field = fields::Add_Hidden_Field_(0, 0, 639, 479, s__00551100, 0x29);
            return;
        }

        MOVEBOX::Moveable_Box_Selected_(2);
        _whole_screen_field = (int16_t)0xfc18;

        _game_button = fields::Add_Button_Field_(249, 5, s__00551100, MOX::_main_game_button_seg, s_G_0055241c, 0x29);
        _next_button = fields::Add_Button_Field_(544, 441, s__00551100, MOX::_main_next_button_seg, s_T_005598f8, 0x28);
        _zoom_in_button = fields::Add_Button_Field_(244, 428, s__00551100, MOX::_main_zoom_in_button_seg, "+", 0x28);
        _zoom_out_button = fields::Add_Button_Field_(244, 455, s__00551100, MOX::_main_zoom_out_button_seg, "-", 0x28);

        _colonies_button = fields::Add_Irregular_Button_Field_(17, 434, 79, 471, s__00551100, MOX::_main_colonies_button_seg, 0, 425, s_C_00552420, 0x28);
        _planets_button = fields::Add_Irregular_Button_Field_(91, 434, 154, 471, s__00551100, MOX::_main_planets_button_seg, 0, 425, s_P_005569d8, 0x28);
        _fleets_button = fields::Add_Irregular_Button_Field_(167, 434, 230, 471, s__00551100, MOX::_main_fleets_button_seg, 0, 425, s_F_005598f4, 0x28);
        _leaders_button = fields::Add_Irregular_Button_Field_(312, 435, 379, 471, s__00551100, MOX::_main_leaders_button_seg, 306, 425, s_L_005560e8, 0x28);
        _races_button = fields::Add_Irregular_Button_Field_(386, 435, 453, 471, s__00551100, MOX::_main_races_button_seg, 306, 425, s_R_0055824c, 0x28);
        _info_button = fields::Add_Irregular_Button_Field_(462, 435, 526, 471, s__00551100, MOX::_main_info_button_seg, 306, 425, s_I_0055801c, 0x28);

        _treasury_window_field = fields::Add_Hidden_Field_(545, 49, 613, 119, s__00551100, 0x29);
        _command_points_window_field = fields::Add_Hidden_Field_(545, 122, 613, 192, s__00551100, 0x29);
        _food_window_field = fields::Add_Hidden_Field_(545, 197, 613, 267, s__00551100, 0x29);
        _freighters_window_field = fields::Add_Hidden_Field_(545, 270, 613, 340, s__00551100, 0x29);
        _research_window_field = fields::Add_Hidden_Field_(545, 344, 613, 415, s__00551100, 0x29);

        Add_Moveable_Box_Fields_();

        int16_t window_status;
        MISC::Save_Window_Status_(&window_status);
        graphics::Set_Window_(22, 22, 527, 421);
        video::Clip_On_();
        MISC::Restore_Window_Status_(window_status);

        _first_field = MOX::_star_button;

        if (_debug_enabled) {
            _debug_field = fields::Add_Hidden_Field_(630, 470, 639, 479, s__00551100, 0);
        } else {
            _debug_field = (int16_t)0xfc18;
        }

        graphics::Reset_Window_();
        _exit_game_hot_key = fields::Add_Multi_Hot_Key_Field_(s_Q_005598f0);

        MOX::_main_screen_grid_field = fields::Add_Grid_Field_(22, 22, 1, 1, 506, 400, &MOX::_main_screen_grid_x, &MOX::_main_screen_grid_y, 0);
    }

    void __cdecl Set_Player_Cheated_Flag_(int16_t player_idx, int8_t cheated_this_turn) {
        MOX::_player[player_idx].cheated = 1;
        MOX::_player[player_idx].cheated_this_turn = cheated_this_turn;
    }

    void __cdecl Give_Me_All_Tech_Advances_() {
        char buffer[80];
        int16_t tech_app_idx = 1;
        int16_t num_granted = 0;

        s_player* current_player = &MOX::_player[MOX::_PLAYER_NUM];

        do {
            if (COLCALC::Player_May_Get_Tech_App_(current_player, (int)tech_app_idx)) {
                num_granted++;
                COLCALC::Player_Gets_Tech_App_(current_player, tech_app_idx);
            }
            tech_app_idx++;
        } while (tech_app_idx < 204); // 0xcc

        const char* format_str = HAROLD::H_Message_(0xdc);
        _sprintf(buffer, format_str, (int)num_granted);

        HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, buffer, 0);
    }

    void __cdecl Check_Hot_Keys_(int16_t key, int param_2) {
        char buffer[200];
        char small_buffer[80];

        if (key == _omniscience_cheat_alt) {
            if (capture::Check_Release_Version_() == 0) {

                s_player* player = &MOX::_player[MOX::_PLAYER_NUM];
                player->traits[TRAIT_OMNISCIENCE] = (player->traits[TRAIT_OMNISCIENCE] == 0);

                const char* msg_src;
                if (player->traits[TRAIT_OMNISCIENCE] == 0) {
                    msg_src = HAROLD::H_Message_(214); // HESTR_0D6_YOU_HAVE_CHOSEN_POORLY
                }
                else {
                    msg_src = HAROLD::H_Message_(213); // HESTR_0D5_USE_THE_FORCE_LUKE_OMN
                    Set_Player_Cheated_Flag_(MOX::_PLAYER_NUM, 1);
                }

                strcpy(buffer, msg_src);
                HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, buffer, 0);
                MOX::_draw_main_screen_to_back = 1;
                Update_Star_Owners_();
                return;
            }
        }

        if (key == _all_tech_cheat) {
            Give_Me_All_Tech_Advances_();
            Set_Player_Cheated_Flag_(MOX::_PLAYER_NUM, 2);
            return;
        }

        if (key == _more_money_cheat) {
            s_player* player = &MOX::_player[MOX::_PLAYER_NUM];
            player->bc += 1000;
            const char* msg_fmt = HAROLD::H_Message_(215); // HESTR_0D7_S_HAS_BEEN_GIVEN_A_GIF
            _sprintf(small_buffer, msg_fmt, player->race_name, 1000);
            HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, small_buffer, 0);
            Set_Player_Cheated_Flag_(MOX::_PLAYER_NUM, 3);
            return;
        }

        if (key == _get_tech_cheat) {
            s_player* player = &MOX::_player[MOX::_PLAYER_NUM];
            const char* tech_name = TECHDATA::_technology_applications[(uint8_t)player->current_research_application].name;
            const char* msg_fmt = HAROLD::H_Message_(216); // HESTR_0D8_S_WILL_BE_KNOWN
            _sprintf(small_buffer, msg_fmt, tech_name);
            player->research_accumulated = 9999;
            HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, small_buffer, 0);
            Set_Player_Cheated_Flag_(MOX::_PLAYER_NUM, 4);
            return;
        }

        if (key == _all_ai_cheat) {
            RUSS::Make_All_AI_Game_();
            return;
        }

        if (key == _hi_score_cheat) {
            int16_t score = SCORE::Do_Calculate_Hi_Score_(MOX::_PLAYER_NUM);
            s_player* player = &MOX::_player[MOX::_PLAYER_NUM];
            _sprintf(buffer, s_Current_score_for_s_is_d, player->name, (int)score);
            HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, buffer, 0);
        }
    }

    void __cdecl Add_Moveable_Box_Fields_() {
        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();

        for (int16_t i = 0; i < 5; ++i) {
            int16_t order_id = MOX::_moveable_box_order[i];
            MOVEBOX::Clear_Moveable_Box_Fields_(order_id);

            if (order_id == 0) {
                if (MOVEBOX::Moveable_Box_Selected_(0) != 0) {
                    Wrapper_For_Normal_System_Fields_();
                }
            }

            if (order_id == 2) {
                if (MOVEBOX::Moveable_Box_Selected_(2) != 0) {
                    int16_t node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
                    if (node_ptr == -1) {
                        MOVEBOX::Vanish_Fleet_Movement_Box_();
                    } else {
                        int32_t flag = (MOX::_moveable_box[2].active < 1) ? 1 : 0;
                        Add_Fleet_Movement_Box_Fields_(node_ptr, flag);
                    }
                }
            }
        }

        graphics::Reset_Window_();
        video::Clip_Off_();
    }

    void __cdecl Do_Colony_Screen_(int16_t colony_idx, int16_t* status_flag) {
        if (colony_idx >= 0) {
            s_colony* colony = &MOX::_colony[colony_idx];

            if (colony->owner == MOX::_PLAYER_NUM) {
                int16_t planet_idx = colony->planet;

                if (colony->outpost_flag != 0) {
                    char* planet_name = HACCESS::Get_Planet_Name_(planet_idx);
                    const char* format_msg = HAROLD::H_Message_(217);
                    char buffer[80];

                    _sprintf(buffer, format_msg, planet_name);

                    HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, buffer, 0);
                } else {
                    MOX::_current_screen = SCREEN_COLONY;
                    MOX::_return_screen = SCREEN_MAIN;

                    s_planet_data* planet = &MOX::_planet[planet_idx];
                    MOX::_screen_data = static_cast<int16_t>(planet->star_index);
                    COLONY::_orbit_temp = static_cast<int16_t>(planet->orbit);

                    *status_flag = 1;
                }
            }
        }
    }

    void __cdecl Add_Cheats_() {
        _all_tech_cheat = fields::Add_Multi_Hot_Key_Field_(s_EINSTEIN_005597e8);
        _more_money_cheat = fields::Add_Multi_Hot_Key_Field_(s_MOOLA_005597f8);
        _get_tech_cheat = fields::Add_Multi_Hot_Key_Field_(s_MENLO_00559800);
        _omniscience_cheat = fields::Add_Multi_Hot_Key_Field_(s_ISEEALL_00559808);
        _hi_score_cheat = fields::Add_Multi_Hot_Key_Field_(s_SCORE_00559820);
        _last_field = fields::_fields_count - 1;
    }

    void __cdecl Highlight_Star_(short star_idx) {
        if (MOX::_star[star_idx].spectral_class != STAR_CLASS_BLACK_HOLE) {
            Print_Highlighted_Star_Name_(star_idx);

            MOX::_selected_star_color_offset += MOX::_selected_star_color_mod;

            if (MOX::_selected_star_color_offset == 3 || MOX::_selected_star_color_offset == -1) {
                MOX::_selected_star_color_mod = -MOX::_selected_star_color_mod;
            }
        }
    }

    int16_t __cdecl Get_Mouse_Field_(int16_t type, int16_t* out_field, int16_t* out_index) {
        bool is_box_selected = MOVEBOX::Moveable_Box_Selected_(2);

        *out_field = -1;
        *out_index = -1;

        if (is_box_selected) {
            if (MOX::_mouse_XY.star_idx > -1 && type == 0) {
                *out_index = MOX::_mouse_XY.star_idx;
                return (int16_t)(MOX::_mouse_XY.selected != 1);
            }
            if (MOX::_mouse_XY.ship_icon_idx > -1 && type == 1) {
                *out_index = MOX::_mouse_XY.ship_icon_idx;
                return (int16_t)(MOX::_mouse_XY.selected != 1);
            }
            if (MOX::_mouse_XY.field3_0x6 > -1 && type == 0) {
                *out_field = MOX::_mouse_XY.field3_0x6;
                return 4;
            }
            if (MOX::_mouse_XY.field1_0x2 > -1 && type == 1) {
                *out_field = MOX::_mouse_XY.field1_0x2;
                return 4;
            }
        } else {
            if (MOX::_mouse_XY.ship_icon_idx > -1 && type == 1) {
                *out_index = MOX::_mouse_XY.ship_icon_idx;
                return (int16_t)(MOX::_mouse_XY.selected != 1);
            }
            if (MOX::_mouse_XY.star_idx > -1 && type == 0) {
                *out_index = MOX::_mouse_XY.star_idx;
                return (int16_t)(MOX::_mouse_XY.selected != 1);
            }
            if (MOX::_mouse_XY.field1_0x2 > -1 && type == 1) {
                *out_field = MOX::_mouse_XY.field1_0x2;
                return 4;
            }
            if (MOX::_mouse_XY.field3_0x6 > -1 && type == 0) {
                *out_field = MOX::_mouse_XY.field3_0x6;
                return 4;
            }
        }

        return -1;
    }

    void __cdecl Check_Stars_XY_(int16_t x, int16_t y, int8_t selected) {
        static const int32_t thresholds[3] = { 12, 10, 8 };

        MOX::_mouse_XY.star_idx = -1;
        MOX::_mouse_XY.ship_icon_idx = -1;

        if (MOX::_NUM_STARS <= 0) {
            return;
        }

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            int16_t screen_x, screen_y;
            Get_Star_Draw_Coords_(star_idx, &screen_x, &screen_y);
            HAROLD::Map_Scale_To_Zoom_Level_();

            const s_star_data& star = MOX::_star[star_idx];

            if (star.spectral_class != STAR_CLASS_BLACK_HOLE) {
                HAROLD::Map_Scale_Star_Size_To_Zoom_Level_(star_idx);
            }

            int32_t dx = (int32_t)screen_x - (int32_t)x;
            int32_t dy = (int32_t)screen_y - (int32_t)y;
            int32_t dist_sq = dx * dx + dy * dy;

            int32_t threshold = thresholds[static_cast<uint8_t>(star.size)];
            if (dist_sq <= threshold * threshold) {
                MOX::_mouse_XY.star_idx = star_idx;
                MOX::_mouse_XY.selected = selected;
                ERICNET::Mox_Play_Sound_(0x29);
                return;
            }
        }
    }

    void __cdecl Exit_Zooming_Mode_(int16_t* zooming_state) {
        *zooming_state = 0;
        MOX::_zoom_in = 0;
        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
        fields::Assign_Auto_Function_(MAINSCR::Draw_Main_Screen_, 1);
        EVANHELP::Set_Main_Screen_Help_List_();
    }

    void __cdecl Check_Ships_XY_(int16_t mouse_x, int16_t mouse_y, int8_t selection_id) {
        MOX::_mouse_XY.star_idx = -1;
        MOX::_mouse_XY.ship_icon_idx = -1;

        if (MOX::_ship_icon_count <= 0) {
            return;
        }

        for (int16_t i = 0; i < MOX::_ship_icon_count; ++i) {
            s_ship_icon* icon = &MOX::_ship_icon[i];
            int16_t ship_idx = MOX::_ship_node[icon->node_idx].ship_idx;

            int16_t owner = HACCESS::Ship_Owner_(ship_idx);
            s_animation_header* anim = SHIPS::Get_Ship_Icon_Pict_Seg_(owner, 0);

            if (mouse_x >= icon->x) {
                int16_t width = animate::Get_Width_(anim);
                if (mouse_x <= icon->x + width) {
                    if (mouse_y >= icon->y) {
                        int16_t height = animate::Get_Height_(anim);
                        if (mouse_y <= icon->y + height) {
                            MOX::_mouse_XY.ship_icon_idx = i;
                            MOX::_mouse_XY.selected = selection_id;
                            ERICNET::Mox_Play_Sound_(0x29);
                            return;
                        }
                    }
                }
            }
        }
    }

    void __cdecl Do_Fleet_Popup_(int16_t ship_icon_id, int8_t check_overlap) {
        int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();
        SYS::Clear_System_Display_Loaded_Flag_();
        
        MOX::_ship_icon_player_id = -1;
        MOX::_fleet_ship_scanned = -1;
        MOX::_eta_star_id = -1;
        
        HACCESS::Clear_Fleet_Box_Selected_Ship_Status_();
        FLEETPOP::Clear_Ship_Node_Selection_Flags_();
        
        MOX::_first_time = 1;
        MOX::_just_moved_stack = 0;

        if (check_overlap != 0) {
            if (SHIPS::Overlapped_Ship_Icon_Button_(&ship_icon_id, 13 - zoom_level, 9 - zoom_level) != 0) {
                MOX::_draw_main_screen_to_back = 1;
            }
        }

        int16_t old_stack_owner = -1;
        if (MOVEBOX::Moveable_Box_Selected_(0)) {
            MOVEBOX::Vanish_System_Display_Box_();
            MAINSCR::Draw_Mini_Main_Offscreen_();
            MOX::_first_node_ptr_in_stack_ = -1;
        } else if (MOVEBOX::Moveable_Box_Selected_(2)) {
            old_stack_owner = HACCESS::Ship_Stack_Owner_(MOX::_ship_stack_start[MOX::_fleet_box_ship_stack]);
        }

        MOX::_moveable_box[2].off_flag = 1;
        MOX::_fleet_box_ship_stack = HACCESS::Ship_Stack_From_Ship_Icon_(ship_icon_id);

        if (MOX::_fleet_box_ship_stack == -1) {
            MOVEBOX::Vanish_Fleet_Movement_Box_();
            return;
        }

        MAINSCR::Set_Popup_Draw_XY_(2, ship_icon_id, &MOX::_moveable_box[2].x, &MOX::_moveable_box[2].y);

        if (MOX::_settings.expanding_help != 0) {
            MOX::_draw_expanding_box = 1;
        }

        int16_t new_stack_owner = HACCESS::Ship_Stack_Owner_(MOX::_ship_stack_start[MOX::_fleet_box_ship_stack]);
        if (new_stack_owner != old_stack_owner || MOX::_mainscr_palette != 2) {
            if (HAROLD::_set_fade_for_first_fltpop == 0) {
                fields::Disable_Draw_All_Fields_();
                MOVEBOX::Vanish_Fleet_Movement_Box_();
                MAINSCR::Redraw_Main_Screen_(0);
                fields::Enable_Draw_All_Fields_();
            } else {
                ERIC::Set_Fade_Percent_(0);
                HAROLD::_set_fade_for_first_fltpop = 0;
                MAINSCR::Draw_Mini_Main_Screen_();
            }
            MOX::_moveable_box[2].off_flag = 1;
            int16_t force_reload = (new_stack_owner != MOX::_PLAYER_NUM);
            FLEETPOP::Reload_Fleet_Box_Palette_(new_stack_owner, force_reload);
        }

        MOVEBOX::Move_Box_To_Top_(2);
        MOX::_add_help_fields = 0;
        MOX::_fleet_movement_bar_value = 0x66;

        if (MOX::_fleet_box_ship_stack > -1 && MOX::_moveable_box[2].x < 32 && MOX::_moveable_box[2].y < 32) {
            int16_t icon_x, icon_y;
            SHIPS::Get_Ship_Icon_Coords_(ship_icon_id, &icon_x, &icon_y);
            
            int16_t box_x1 = MOX::_moveable_box[2].x;
            int16_t box_y1 = MOX::_moveable_box[2].y;
            int16_t box_x2 = box_x1 + (int16_t)MOX::_moveable_box[2].width;
            int16_t box_y2 = box_y1 + (int16_t)MOX::_moveable_box[2].height;

            if (MISC::Occluded_(box_x1, box_y1, box_x2, box_y2, icon_x, icon_y, icon_x + 10, icon_y + 13)) {
                MOX::_moveable_box[2].x = 619;
                MOX::_moveable_box[2].y = 421;
            }
        }
    }

    /**
     * @address 004c6da0
     */
    void __cdecl Draw_Scanned_Ship_Seg_() {
        int16_t icon_idx = _main_screen_ship_icon_scanned;
        int16_t owner_idx = HACCESS::Ship_Icon_Owner_(icon_idx);

        s_animation_header* anim = SHIPS::Get_Ship_Icon_Pict_Seg_(owner_idx, 1);

        int16_t local_y, local_x;
        SHIPS::Get_Ship_Icon_Coords_(icon_idx, &local_y, &local_x);

        int16_t final_x = MOX::_ship_icon[icon_idx].x;
        int16_t final_y = MOX::_ship_icon[icon_idx].y;

        animate::Draw_(final_x, final_y, anim);
    }

    void __cdecl Draw_Fleet_Box_Selected_Ship_Seg_() {
        int16_t node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
        int16_t ship_icon_id = HACCESS::Ship_Icon_Id_From_Node_Ptr_(node_ptr);

        int16_t owner = HACCESS::Ship_Icon_Owner_(ship_icon_id);
        s_animation_header* anim = SHIPS::Get_Ship_Icon_Pict_Seg_(owner, 1);

        int16_t local_x;
        int16_t local_y;
        SHIPS::Get_Ship_Icon_Coords_(ship_icon_id, &local_x, &local_y);

        animate::Draw_(MOX::_ship_icon[ship_icon_id].x, MOX::_ship_icon[ship_icon_id].y, anim);
    }

    void __cdecl Print_Galaxy_Map_Scanned_Star_() {
        s_colors text_colors;
        char print_buffer[80];

        int16_t star_idx = (int16_t)MOX::_galaxy_map_scanned_star;
        s_star_data* star = &MOX::_star[star_idx];
        int16_t owner = (int16_t)star->owner;

        if (owner >= 0 && owner < 8) {
            HAROLD::Set_Player_Font_Style_(owner, 0);
            char* race_name = HACCESS::Race_Name_String_(owner);

            _sprintf(print_buffer, MOX::s_fmt_scanned_star_info, star->name, race_name);
        } else if (owner == -1) {
            for (int16_t i = 0; i < 8; ++i) {
                if (i / 2 == 0) {
                    text_colors.colors[i] = 0x08;
                } else {
                    text_colors.colors[i] = 0x0C;
                }
            }
            _sprintf(print_buffer, MOX::s_fmt_s, star->name);
        } else if (owner == -2) {
            for (int16_t i = 0; i < 8; ++i) {
                if (i / 2 == 0) {
                    text_colors.colors[i] = 0x14;
                } else {
                    text_colors.colors[i] = 0x1F;
                }
            }
            _sprintf(print_buffer, MOX::s_fmt_contested, star->name);
        }

        fonts::Set_Font_Style_(2, &text_colors);

        s_movable_box box = MOX::_moveable_box[1];
        int16_t final_x = box.x + (int16_t)box.field12_0x18 + 15;
        int16_t final_y = (int16_t)box.height + (box.y + (int16_t)box.field13_0x1a) - 25;

        fonts::Print_(final_x, final_y, print_buffer);
    }

    void Draw_Main_Screen_While_Moving_Boxes_() {
        if (MOX::_draw_main_screen_stencilled == 1) {
            Draw_Main_Screen_Before_Stencil_();
            MOX::_draw_main_screen_stencilled = 0;
            return;
        }

        graphics::Set_Window_(0x16 /* 22 */, 0x16 /* 22 */, 0x20F /* 527 */, 0x1A5 /* 421 */);
        video::Clip_On_();

        for (int16_t i = 4; i >= 0; --i) {
            int16_t box_id = MOX::_moveable_box_order[i];
            if (MOVEBOX::Moveable_Box_Selected_(box_id) != 0 && MOX::_stencil_box_id != box_id) {
                MOVEBOX::Draw_Moveable_Box_(box_id);
            }
        }

        if (MOX::_stencil_box_id > -1) {
            if (MOX::_stencil_box_id < 5) {
                int16_t button = Mouse_Button_();
                MOVEBOX::Move_Moveable_Box_(MOX::_stencil_box_id, button);
            }
            if (MOX::_stencil_box_id > -1 && MOX::_stencil_box_id < 5) {
                MOVEBOX::Draw_Moveable_Box_(MOX::_stencil_box_id);
            }
        }

        video::Clip_Off_();
        graphics::Reset_Window_();
        Print_Main_Screen_Data_();

        if (MOX::_stencil_box_id == -2) {
            video::Set_Page_Off_();
            Draw_Main_Screen_();
            MOX::_stencil_box_id = -1;
        }
    }

    void __cdecl Check_Mouse_XY_Fields_() {
        int16_t mouse_x = mouse::Pointer_X_();
        int16_t mouse_y = mouse::Pointer_Y_();

        MOX::_mouse_XY.selected = 0;
        MOX::_mouse_XY.field3_0x6 = -1;
        MOX::_mouse_XY.field1_0x2 = -1;

        bool is_in_box = false;

        if (MOVEBOX::Moveable_Box_Selected_(0)) {
            if (mouse_x > MOX::_moveable_box[0].x &&
                mouse_x < (MOX::_moveable_box[0].x + (int16_t)MOX::_moveable_box[0].width) &&
                mouse_y > MOX::_moveable_box[0].y &&
                mouse_y < (MOX::_moveable_box[0].y + (int16_t)MOX::_moveable_box[0].height)) {
                is_in_box = true;
            }
        } else if (MOVEBOX::Moveable_Box_Selected_(2)) {
            if (mouse_x > MOX::_moveable_box[2].x &&
                mouse_x < (MOX::_moveable_box[2].x + (int16_t)MOX::_moveable_box[2].width) &&
                mouse_y > MOX::_moveable_box[2].y &&
                mouse_y < (MOX::_moveable_box[2].y + (int16_t)MOX::_moveable_box[2].height)) {
                is_in_box = true;
            }
        }

        if (mouse_x > 21 && mouse_x < 527 &&
            mouse_y > 21 && mouse_y < 421) {
            if (!is_in_box) {
                Scan_Stars_XY_(mouse_x, mouse_y);
                Scan_Ships_XY_(mouse_x, mouse_y);
            }
        }
    }

    /**
     * Updates parallax background positions based on map movement.
     * Returns the vertical wrap count for the far background layer.
     */
    int16_t __cdecl Get_New_Paralax_XYs_(int16_t target_map_x, int16_t target_map_y, int16_t scale) {
        int16_t dx = (static_cast<int32_t>(target_map_x) - MOX::_cur_map_x) / scale;
        int16_t dy = (static_cast<int32_t>(target_map_y) - MOX::_cur_map_y) / scale;

        int16_t near_x = MOX::_near_star_x;
        int16_t mid_x = MOX::_mid_star_x;
        int16_t far_x = MOX::_far_star_x;

        if (target_map_x != MOX::_cur_map_x) {
            // sign_x logic calculates -abs(dx)
            int16_t sign_x = (dx > 0) ? -1 : (dx < 0 ? 1 : 0);
            int16_t step_x = sign_x * dx;
            
            far_x  -= step_x;
            mid_x  -= 2 * step_x;
            near_x -= 4 * step_x;
        }

        int16_t near_y = MOX::_near_star_y;
        int16_t mid_y = MOX::_mid_star_y;
        int16_t far_y = MOX::_far_star_y;

        if (target_map_y != MOX::_cur_map_y) {
            // sign_y logic calculates -abs(dy)
            int16_t sign_y = (dy > 0) ? -1 : (dy < 0 ? 1 : 0);
            int16_t step_y = sign_y * dy;

            far_y  -= step_y;
            mid_y  -= 2 * step_y;
            near_y -= 4 * step_y;
        }

        // Apply screen wrap (640x480)
        if (near_x < 0) near_x += 640;
        if (near_y < 0) near_y += 480;
        if (mid_x < 0)  mid_x  += 640;
        if (mid_y < 0)  mid_y  += 480;
        if (far_x < 0)  far_x  += 640;
        if (far_y < 0)  far_y  += 480;

        MOX::_near_star_x = near_x % 640;
        MOX::_near_star_y = near_y % 480;
        MOX::_mid_star_x  = mid_x  % 640;
        MOX::_mid_star_y  = mid_y  % 480;
        MOX::_far_star_x  = far_x  % 640;

        int16_t wrap_count_y = far_y / 480;
        MOX::_far_star_y = far_y % 480;

        return wrap_count_y;
    }

    int16_t __cdecl Scan_System_Display_Fields_(int16_t mouse_input, int16_t input, int16_t *colony_btns, 
                                                int16_t *m_planet_idx, int16_t *planet_idx, 
                                                int16_t *m_ship_idx, int16_t *ship_idx, int16_t *selection) {
        int16_t ret_val = -1;
        int16_t gate_idx = -1;

        *planet_idx = -1;
        *m_planet_idx = -1;
        *ship_idx = -1;
        *m_ship_idx = -1;
        *selection = -1;

        // 1. Check for clicked ships
        for (int i = 0; i < 15; ++i) {
            if (MOX::_system_display_ships[i].field4_0x8 != 0 && MOX::_system_display_ships[i].button == input) {
                *ship_idx = MOX::_system_display_ships[i].ship_idx;
                ret_val = 0;
                break;
            }
        }

        // 2. Check for hovered ships if no click hit
        if (ret_val == -1) {
            for (int i = 0; i < 15; ++i) {
                if (MOX::_system_display_ships[i].field4_0x8 != 0 && MOX::_system_display_ships[i].button == mouse_input) {
                    *m_ship_idx = MOX::_system_display_ships[i].ship_idx;
                    ret_val = 4;
                    break;
                }
            }
        }

        // 3. Check for colony button interactions (Click, Right-Click popup, or Hover)
        if (ret_val == -1) {
            for (int i = 0; i < 5; ++i) {
                int16_t btn_id = colony_btns[i];
                if (input == btn_id) {
                    *planet_idx = ERIC::Get_Sint_Index_From_List_(MOX::_sys_disp[i].orbit, MOX::_star[MOX::_system_star_id].planet_index, 5);
                    ret_val = 0;
                    break;
                }
                if ((int)input + (int)btn_id == 0) { // Right-click or special interaction
                    if (MOX::_planet[MOX::_sys_disp[i].orbit].planet_type != PLANET_TYPE_GAS_GIANT) {
                        SYS::Potential_Colony_Info_Popup_(MOX::_sys_disp[i].orbit);
                        MOX::_FULL_DRAW_SCREEN_FLAG = 1;
                    }
                    return -1;
                }
                if (mouse_input == btn_id) {
                    *m_planet_idx = ERIC::Get_Sint_Index_From_List_(MOX::_sys_disp[i].orbit, MOX::_star[MOX::_system_star_id].planet_index, 5);
                    ret_val = 4;
                }
            }
        }

        // 4. Check for system grid interactions if still no hit
        if (ret_val == -1) {
            int16_t center_x = MOX::_system_display_base_x + 171; // 0xab
            int16_t center_y = MOX::_system_display_base_y + 134; // 0x86
            int16_t ptr_x = mouse::Pointer_X_();
            int16_t ptr_y = mouse::Pointer_Y_();

            // Grid click scan
            if (input == MOX::_system_display_grid_field) {
                int16_t orbit_id = GEO::Get_Orbit_From_X_Y_(center_x - ptr_x, center_y - ptr_y);
                if (orbit_id >= 0) {
                    for (int i = 0; i < 5; ++i) {
                        int16_t p_idx = MOX::_star[MOX::_system_star_id].planet_index[i];
                        if (p_idx > -1 && MOX::_planet[p_idx].orbit == orbit_id && MOX::_planet[p_idx].planet_type == PLANET_TYPE_ASTEROID) {
                            *planet_idx = i;
                            ret_val = 0;
                            break;
                        }
                    }
                }
            }

            // Grid hover scan
            if (ret_val == -1) {
                int16_t orbit_id = -1;
                if (ptr_x > MOX::_system_display_base_x && 
                    ptr_x < MOX::_system_display_base_x + (int16_t)MOX::_moveable_box[0].width &&
                    ptr_y > MOX::_system_display_base_y && 
                    ptr_y < MOX::_system_display_base_y + (int16_t)MOX::_moveable_box[0].height) 
                {
                    orbit_id = GEO::Get_Orbit_From_X_Y_(center_x - ptr_x, center_y - ptr_y);
                }

                if (orbit_id >= 0) {
                    for (int i = 0; i < 5; ++i) {
                        int16_t p_idx = MOX::_star[MOX::_system_star_id].planet_index[i];
                        if (p_idx > -1 && MOX::_planet[p_idx].orbit == orbit_id && MOX::_planet[p_idx].planet_type == PLANET_TYPE_ASTEROID) {
                            *m_planet_idx = i;
                            ret_val = 4;
                            break;
                        }
                    }
                }
            }
        }

        // 5. Check for gate icon hover
        if (ret_val == -1) {
            for (int i = 0; i < 4; ++i) {
                if (MOX::_gate_icon_field[i] == mouse_input) {
                    gate_idx = i;
                    ret_val = 4;
                    break;
                }
            }
        }

        // Determine final selection result for UI feedback
        *selection = -1;
        if (*m_planet_idx > -1) {
            *selection = *m_planet_idx;
        } else if (*m_ship_idx >= 0) {
            *selection = *m_ship_idx + 1000;
        } else if (gate_idx > -1) {
            *selection = gate_idx + 2000;
        }

        return ret_val;
    }

    void __cdecl Clear_Twinkling_Star_Data_() {
        for (int32_t i = 0; i < 72; ++i) {
            MOX::_star[i].twinkling1 = -1;
            MOX::_star[i].twinkling2 = -1;
        }
        _n_twinkling_stars = 0;
    }

    void __cdecl Warn_About_Cheating_Players_() {
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (i == MOX::_PLAYER_NUM) continue;

            s_player* other_player = &MOX::_player[i];
            s_player* local_player = &MOX::_player[MOX::_PLAYER_NUM];

            if (other_player->cheated_this_turn != 0 && !(local_player->warned_about_cheating_player & (1 << (i & 0x1F)))) {
                char name_buf[52];
                char race_buf[52];
                char lbx_msg[500];
                char output_msg[500];

                char* src = other_player->name;
                char* dst = name_buf;
                while ((*dst++ = *src++) != '\0');

                src = other_player->race_name;
                dst = race_buf;
                while ((*dst++ = *src++) != '\0');

                strings::String_To_Upper_(name_buf);
                strings::String_To_Upper_(race_buf);

                other_player->cheated = 1;

                MISC::Get_LBX_Message_(22 /* LBX_MSG_CHEAT_WARNING */, lbx_msg);
                
                _sprintf(output_msg, lbx_msg, name_buf, race_buf);

                HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, output_msg, 3);
                video::Set_Page_Off_();
                MAINSCR::Draw_Mini_Main_Screen_();
                RUSS::Mox_Sync_Update_();

                local_player->warned_about_cheating_player |= (1 << (i & 0x1F));
            }
        }

        MOX::_player[MOX::_PLAYER_NUM].cheated_this_turn = 0;
    }

    void __cdecl Change_Home_Star_Name_() {
        int16_t player_idx = MOX::_PLAYER_NUM;
        s_player* player = &MOX::_player[player_idx];
        int16_t star_idx = static_cast<int16_t>(MOX::_planet[player->home_planet_id].star_index);

        int16_t center_y = namestar::Star_Name_Popup_Map_Center_Y_();
        int16_t center_x = namestar::Star_Name_Popup_Map_Center_X_();

        namestar::Change_Home_Star_Name_Popup_(center_x, center_y, star_idx);

        fields::Clear_Fields_();
        MAINSCR::Clear_Main_Screen_Fields_();
        MAINSCR::Add_Cheats_();
        MAINSCR::Add_Map_Fields_();
        video::Set_Page_Off_();
        MAINSCR::Draw_Mini_Main_Screen_();
        RUSS::Mox_Sync_Update_();
        // TYPE MISMATCH: Assign_Auto_Function_ takes int16_t delay as second argument in codebase, but decompilation has int32_t active.
        fields::Assign_Auto_Function_(MAINSCR::Draw_Main_Screen_, 1);
    }

    int16_t __cdecl Do_Reply_To_Diplomacy_Request_(int16_t requester_id) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c99b0});
        return fn(requester_id);
    }

    int16_t __cdecl Get_Star_Id_For_Officers_Screen_() {
        int16_t player_num = MOX::_PLAYER_NUM;
        int16_t home_planet_id = MOX::_player[player_num].home_planet_id;
        int16_t star_idx = -1;

        // Check if a specific system is selected in the UI
        if (MOVEBOX::Moveable_Box_Selected_(0) != 0) {
            star_idx = MOX::_moveable_box[0].off_flag;
            if (star_idx != -1) {
                if (HAROLD::Player_Has_Colony_(player_num, star_idx)) {
                    return star_idx;
                }
            }
        }

        // Search for the first star system where the player has a colony, starting from home
        int16_t home_star_idx = (int16_t)MOX::_planet[home_planet_id].star_index;
        star_idx = home_star_idx;

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            if (HAROLD::Player_Has_Colony_(player_num, star_idx)) {
                return star_idx;
            }
            star_idx = (int16_t)((star_idx + 1) % MOX::_NUM_STARS);
        }

        // Fallback to home star if no colony found or galaxy has no stars
        if (star_idx == -1) {
            star_idx = home_star_idx;
        }

        return star_idx;
    }

    /**
     * Iterates through the ship stack to find the first ship belonging to the current player.
     * If the first ship in the stack is already owned by the player, it returns that index.
     * Otherwise, it searches the entire stack.
     */
    int16_t __cdecl Get_Ship_Stack_For_Officers_Screen_() {
        int16_t first_stack_idx = MOX::_ship_stack_start[0];

        // 0xFFFF is -1, indicating an empty/invalid stack entry
        if (first_stack_idx != -1) {
            // Note: _ship_node[first_stack_idx].ship_idx maps stack index to global ship array index
            // owner is at offset 0x63 in s_ship_data
            int16_t ship_idx = MOX::_ship_node[first_stack_idx].ship_idx;

            // If the first ship in stack is not owned by the current player, search the rest
            if (MOX::_ship[ship_idx].owner != MOX::_PLAYER_NUM) {
                for (int16_t i = 0; i < MOX::_ship_stack_count; ++i) {
                    int16_t current_stack_idx = MOX::_ship_stack_start[i];
                    int16_t current_ship_idx = MOX::_ship_node[current_stack_idx].ship_idx;

                    if (MOX::_ship[current_ship_idx].owner == (uint8_t)MOX::_PLAYER_NUM) {
                        return current_stack_idx;
                    }
                }
                return -1;
            }
        }

        return first_stack_idx;
    }

    /**
     * @address 004c8c70
     */
    void __cdecl Tax_Rate_Popup_() {
        int16_t scan_val;
        int16_t input_val;
        int16_t exit_flag;
        uint8_t font_colors[8];

        font_colors[0] = 0x9f;
        font_colors[1] = 0xa0;
        font_colors[2] = 0xa1;
        font_colors[3] = 0xa2;
        input_val = 0;
        font_colors[4] = 0;
        exit_flag = 0;
        font_colors[5] = 0;
        font_colors[6] = 0;
        font_colors[7] = 0;

        s_player* current_player = &MOX::_player[MOX::_PLAYER_NUM];
        MAINPUPS::_g_temp_tax_rate = static_cast<int16_t>(static_cast<uint8_t>(current_player->tax_rate));

        MAINPUPS::_last_scanned_field = -1;
        fonts::Set_Font_Style_Shadow_Down_(1, reinterpret_cast<s_colors*>(font_colors));
        MAINPUPS::Load_Tax_Rate_Popup_();
        MAINPUPS::Do_Popup_Setup_(-1, DAT_00565e20);
        ERICHELP::Set_Tax_Rate_Popup_Help_List_();
        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(MAINPUPS::Draw_Tax_Rate_Popup_, 1);
        MAINPUPS::Add_Tax_Rate_Fields_();
        MAINPUPS::Draw_Tax_Rate_Popup_();
        RUSS::Mox_Sync_Update_();
        video::Set_Refresh_Differential_();

        do {
            input_val = fields::Get_Input_();
            scan_val = fields::Scan_Input_();
            MAINPUPS::Evaluate_Tax_Rate_Input_(&input_val, &scan_val, &exit_flag);
            MAINPUPS::Draw_Tax_Rate_Popup_();
            RUSS::Mox_Sync_Update_();
        } while (exit_flag == 0);

        MAINPUPS::Do_Popup_Cleanup_();
    }

    int16_t __cdecl Check_For_Diplomacy_Request_() {
        int16_t processed_any = 0;
        int16_t processed_count = 0;
        int16_t requester_id = (int16_t)NETMOX::Human_Requesting_Diplomacy_();

        if (requester_id != -1) {
            processed_any = 1;
            do {
                if (processed_count > 0) {
                    Fade_Into_Main_Screen_();
                }

                int16_t reply_type = Do_Reply_To_Diplomacy_Request_(requester_id);

                if (reply_type == 1) {
                    MAINSCR2::_force_main_screen_fade = 1;
                    NETMOX::Accept_Diplomacy_Request_((int32_t)requester_id);
                } else if (reply_type == 2) {
                    NETMOX::Refuse_Diplomacy_Request_((int32_t)requester_id);
                } else if (reply_type == 3) {
                    NETMOX::Tell_Diplomacy_Requester_To_Wait_((int32_t)requester_id);
                }

                processed_count++;
                requester_id = (int16_t)NETMOX::Human_Requesting_Diplomacy_();
            } while (requester_id != -1);
        }

        return processed_any;
    }

    void __cdecl Set_Reload_Fleet_Box_Palette_Flag_() {
        MOX::_mainscr_palette = 101;
        MOX::_ship_icon_player_id = (uint16_t)-1;
    }

    /**
     * @brief Redraws the main game screen, handling full redraws with fades or mini updates.
     *
     * @param redraw_full If true, performs a full screen clear, fade out, and fade in.
     */
    void __cdecl Redraw_Main_Screen_(uint8_t redraw_full) {
        if (redraw_full != 0) {
            ERIC::Fast_Fade_Out_();
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 649 /* 0x289 */, 479 /* 0x1df */, 0);
            RUSS::Mox_Sync_Update_();
            video::Set_Refresh_Full_();
            video::Set_Page_Off_();
            Draw_Main_Screen_Filled_(1);
            ERIC::Fast_Fade_In_();
            video::Set_Refresh_Stencil_();

            if (MOVEBOX::Moveable_Box_Selected_(2) != 0) {
                if (HACCESS::Get_Stack_Start_From_Saved_Ship_Array_() != 0) {
                    int16_t ship_stack_idx = MOX::_fleet_box_ship_stack;
                    int16_t ship_id = MOX::_ship_stack_start[ship_stack_idx];
                    int16_t owner = HACCESS::Ship_Stack_Owner_(ship_id);
                    FLEETPOP::Reload_Fleet_Box_Palette_(owner, (owner != MOX::_PLAYER_NUM));
                } else {
                    MOVEBOX::Vanish_Fleet_Movement_Box_();
                }
            }
        } else {
            video::Set_Page_Off_();
            Draw_Mini_Main_Screen_();
            RUSS::Mox_Sync_Update_();

            if (MOVEBOX::Moveable_Box_Selected_(2) != 0) {
                if (HACCESS::Get_Stack_Start_From_Saved_Ship_Array_() != 0) {
                    int16_t ship_stack_idx = MOX::_fleet_box_ship_stack;
                    int16_t ship_id = MOX::_ship_stack_start[ship_stack_idx];
                    int16_t owner = HACCESS::Ship_Stack_Owner_(ship_id);

                    video::Set_Page_Off_();
                    FLEETPOP::Reload_Fleet_Box_Palette_(owner, (owner != MOX::_PLAYER_NUM));
                    Draw_Main_Screen_();
                    RUSS::Mox_Sync_Update_();
                } else {
                    MOVEBOX::Vanish_Fleet_Movement_Box_();
                }
            }
        }
    }

    void __cdecl Scroll_Map_(int16_t x, int16_t y) {
        int16_t target_x = x - HAROLD::Get_Up_Scaled_Value_(253 /* 0xFD */);
        int16_t target_y = y - HAROLD::Get_Up_Scaled_Value_(200 /* 0xC8 */);

        if (target_x < 0) {
            target_x = 0;
        }

        int16_t max_x_offset = HAROLD::Get_Up_Scaled_Value_(506 /* 0x1FA */);
        if (MOX::_MAP_MAX_X - max_x_offset < target_x) {
            target_x = MOX::_MAP_MAX_X - max_x_offset;
        }

        if (target_y < 0) {
            target_y = 0;
        }

        int16_t max_y_offset = HAROLD::Get_Up_Scaled_Value_(400 /* 0x190 */);
        if (MOX::_MAP_MAX_Y - max_y_offset < target_y) {
            target_y = MOX::_MAP_MAX_Y - max_y_offset;
        }

        int16_t old_x = MOX::_cur_map_x;
        int16_t old_y = MOX::_cur_map_y;
        int32_t distance = special::Range_(old_x, old_y, target_x, target_y);

        // Check if movement is significant (over 50 pixels) and coordinates actually changed
        if ((int16_t)distance > 50 && (target_x != old_x || target_y != old_y)) {
            int16_t num_steps = (int16_t)distance / 50;
            video::Set_Refresh_Full_();

            if (num_steps > 0) {
                int16_t count = num_steps;
                do {
                    MAINSCR::Get_New_Paralax_XYs_(target_x, target_y, num_steps);
                    special::Interpolate_Line_(&MOX::_cur_map_x, &MOX::_cur_map_y, target_x, target_y, 50);

                    MOX::_animated_star = 0xFFFF;
                    video::Set_Page_Off_();
                    MAINSCR::Draw_Main_Screen_Filled_(0);
                    MAINSCR::Draw_Main_Screen_();
                    RUSS::Mox_Sync_Update_();
                    count--;
                } while (count != 0);
            }

            MOX::_animated_star = 0xFFFF;
            video::Set_Page_Off_();
            MAINSCR::Draw_Main_Screen_Filled_(1);
            video::Set_Refresh_Stencil_();
        } else {
            // Immediate update for small or no movement
            MOX::_cur_map_x = target_x;
            MOX::_draw_main_screen_to_back = 1;
            MOX::_animated_star = 0xFFFF;
            MOX::_cur_map_y = target_y;
        }
    }

    /**
     * @address 004c4980
     */
    void* __cdecl Allocate_First_Screen_Seg_(int32_t param) {
        // Allocate 76,000 bytes (0x128E0) for other ships
        MOX::_other_ships_screen_seg = allocate::Allocate_First_Block_(MOX::_screen_seg, 76000 /* 0x128E0 */);

        // Update block metrics (return value is unused in assembly)
        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);

        // Determine space required for a 60x60 bitmap and allocate it next
        uint32_t bitmap_size = bitmap::Get_Bitmap_Size_(60 /* 0x3C */, 60 /* 0x3C */);
        MOX::_global_bitmap_seg = allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);

        return MOX::_global_bitmap_seg;
    }

    /**
     * @address 004c4e90
     */
    void __cdecl Do_Next_Turn_(int16_t* exit_flag, int8_t auto_mode) {
        MOX::_at_least_one_report = 0;
        MOX::_previous_report = 0;
        MOX::_just_moved_stack = 0;
        MOX::_animated_star = 0xFFFF;

        _warn_about_cheaters = (int8_t)(1 << (MOX::_PLAYER_NUM & 0x1F));

        if (MOX::_game_type == 1 /* Hotseat */) {
            Save_Hotseat_Map_Info_();
            if (MOX::_game_type == 1) {
                MOX::_player[MOX::_PLAYER_NUM].player_done_flags = 1;
                MOX::_current_screen = SCREEN_HOTSEAT_SELECT_PLAYER;
            } else {
                if (MOX::_game_type == 2 || MOX::_game_type == 3) {
                    MOX::_current_screen = SCREEN_NET_NEXT_TURN;
                } else {
                    MOX::_current_screen = SCREEN_MAIN_NEXT_TURN;
                }
                MOX::_return_screen = SCREEN_REPORTS;
            }
        } else {
            if (MOX::_game_type == 2 || MOX::_game_type == 3) {
                MOX::_current_screen = SCREEN_NET_NEXT_TURN;
            } else {
                MOX::_current_screen = SCREEN_MAIN_NEXT_TURN;
            }
            MOX::_return_screen = SCREEN_REPORTS;
        }

        Set_Reload_Fleet_Box_Palette_Flag_();
        SYS::Clear_System_Display_Loaded_Flag_();

        if (auto_mode == 0) {
            _in_auto_next_turn_mode = 0;
            fields::Clear_Fields_();
            Clear_Main_Screen_Fields_();

            if (MOX::_settings.end_of_turn_wait == 0) {
                MOX::_global_exit_field = fields::Add_Hidden_Field_(0, 0, 639, 479, s__00551120, 41);
                *exit_flag = 1;
                return;
            }
        } else {
            _in_auto_next_turn_mode = 1;
            MISC::Increment_Mouse_Clock_Image_();
            int16_t input = fields::Get_Input_();
            MOX::_auto_turn_exit = (uint8_t)input;

            if (MOX::_auto_turn_exit != 0) {
                _check_next_turn_exit_field = 0;
                mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
                _next_turn_hit = 0;
                fields::Clear_Fields_();
                MOX::_current_screen = SCREEN_MAIN;
            }
        }

        *exit_flag = 1;
    }
}