#include "deps.h"

namespace MAINSCR {
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

        if (static_cast<int8_t>(star->not_used1) < 0) {
            animate::Reset_Animation_Frame_(star_seg);
            animate::Draw_(screen_x, screen_y, star_seg);
        } else {
            star->not_used1++;

            int16_t frame_count = animate::Get_Animation_Frame_Count_(star_seg);

            if (static_cast<int8_t>(star->not_used2) <= static_cast<int8_t>(star->not_used1)) {
                int32_t remaining = static_cast<int32_t>(_n_twinkling_stars) - 1;
                _n_twinkling_stars = static_cast<int16_t>((remaining < 1) ? 0 : remaining);

                star->not_used1 = 0xFF;
                star->not_used2 = 0xFF;
            }

            int32_t current_frame = (static_cast<int16_t>(star->not_used1) / 2) % frame_count;
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
        uint8_t colors[8] = { 0x6e, 0x6f, 0x70, 0x70, 0x6e, 0x6f, 0x70, 0x70 };

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
                    SHIPS::Draw_Directional_Multi_Colored_Line_(x1, y1, x2, y2, colors, MOX::_multi_colored_line_start);
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
            void* artemis_pic = Buffer_Reload_(s_BUFFER0_LBX, zoom_offset + 31, MOX::_global_cache_seg);
            MOX::gate_artemis_icons[zoom_offset] = static_cast<s_animation_header*>(artemis_pic);
            bitmap::Draw_Picture_To_Bitmap_(temp_icons[icon_count], artemis_pic);
            int16_t h = animate::Get_Height_((s_animation_header*)artemis_pic);
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

    void __cdecl Set_Post_Fitted_Font_Style_(uint8_t param_1, uint8_t param_2) {
        int16_t style = MISC::Get_Font_Style_();
        int16_t spacing = MISC::Get_Font_Spacing_();
        uint8_t colors[8];

        MISC::Get_Mox_Font_Colors_(style, static_cast<char>(param_1), static_cast<char>(param_2), colors);

        fonts::Set_Outline_Color_(2, 1);
        fonts::Set_Font_Style_Outline_(style, colors);
        fonts::Set_Font_Spacing_(spacing);
    }

    /**
     * @address 004c70e0
     */
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

    void __cdecl Print_Main_Screen_Data_(void) {
        char sep = (MOX::_settings.language == 1 || MOX::_settings.language == 2) ? ',' : '.';
        uint8_t font_colors[16];
        char buffer[128];
        char monetary_unit[16];
        char research_unit[16];

        fonts::Set_Font_Style_(0, (MISC::s_colors*)&font_colors[0]);
        fonts::Get_Font_Height_();
        _sprintf(buffer, s__d_c_d_0055996c, MOX::_stardate / 10, sep, MOX::_stardate % 10);
        MISC::Set_Fitted_Font_Style_(buffer, &font_colors[8], 2, 2, -1, 0x43, 1);
        int16_t stardate_font_style = MISC::Get_Font_Style_();
        MISC::s_colors* p_stardate_colors = MISC::Font_Colors2_(stardate_font_style, 14, 23);
        helpers::memcpy(&font_colors[8], p_stardate_colors, 8);
        fonts::Set_Outline_Color_(2, 1);
        fonts::Set_Font_Style_Outline_(stardate_font_style, &font_colors[8]);
        fonts::Print_Centered_(580, 29, buffer); // 0x244, 0x1d

        if (EVENTS::Event_Check_Hyperspace_Flux_()) {
            MISC::Get_Mox_Font_Colors_(3, 'I', 'L', &font_colors[0]);
            fonts::Set_Font_Style_(3, (MISC::s_colors*)&font_colors[0]);
            MISC::Increase_Font_Color_Range_((MOX::_red_color_pulse_value * 5) / 9);
            const char* flux_msg = HAROLD::H_Message_(0xe0);
            char flux_buffer[128];
            helpers::strcpy(flux_buffer, flux_msg);
            int16_t flux_width = fonts::Get_String_Width_(flux_buffer);
            int16_t flux_x = 526 - flux_width; // 0x20e

            uint32_t* p_font_colors_32 = (uint32_t*)font_colors;
            uint32_t line_color1 = p_font_colors_32[0];
            uint32_t line_color2 = p_font_colors_32[1];

            line::Line_(flux_x, 21, 526, 21, line_color1);
            line::Line_(flux_x, 22, 526, 22, line_color2);
            fonts::Print_Right_(526, 24, flux_buffer);

            int16_t fh = fonts::Get_Font_Height_();
            line::Line_(flux_x, fh + 25, 526, fh + 25, line_color2);
            line::Line_(flux_x, fh + 26, 526, fh + 26, line_color1);
        }

        fonts::Get_Font_Height_();
        HAROLD::Get_Monetary_Unit_String_(monetary_unit);
        s_player* plr = &MOX::_player[MOX::_PLAYER_NUM];

        char* bc_comma = MISC::Get_Comma_String_(plr->bc);
        _sprintf(buffer, s__s__s_00558014, bc_comma, monetary_unit);

        if (plr->bc < 0) {
            MISC::Set_Fitted_Font_Style_(buffer, &font_colors[0], 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_('K', 'M');
            MISC::Increase_Font_Color_Range_((MOX::_red_color_pulse_value << 2) / 7);
            fonts::Set_Outline_Color_(0x49, 0x49);
        } else {
            MISC::Set_Fitted_Font_Style_(buffer, &font_colors[8], 2, 2, 4, 0x43, 1);
            fonts::Set_Outline_Color_(2, 1);
        }
        fonts::Print_Centered_(579, 96, buffer); // 0x243, 0x60

        char* surplus_bc_comma = MISC::Get_Comma_String_(plr->surplus_bc);
        const char* bc_sign = (plr->surplus_bc < 0) ? "-" : "+";
        _sprintf(buffer, s__s_s__s_00559964, bc_sign, surplus_bc_comma, monetary_unit);
        MISC::Set_Fitted_Font_Style_(buffer, &font_colors[8], 2, 2, 4, 0x43, 1);
        int16_t bc_style = MISC::Get_Font_Style_();
        int16_t bc_spacing = MISC::Get_Font_Spacing_();
        MISC::Get_Mox_Font_Colors_(bc_style, 'K', 'M', &font_colors[0]);
        MISC::Get_Mox_Font_Colors_(bc_style, 14, 23, &font_colors[8]);

        if (plr->surplus_bc < 0) {
            fonts::Set_Outline_Color_(0x49, 0x49);
            fonts::Set_Font_Style_Outline_(bc_style, &font_colors[0]);
            fonts::Set_Font_Spacing_(bc_spacing);
            MISC::Increase_Font_Color_Range_((MOX::_red_color_pulse_value << 2) / 7);
        } else {
            fonts::Set_Font_Style_Outline_(bc_style, &font_colors[8]);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Set_Font_Spacing_(bc_spacing);
        }
        fonts::Print_Centered_(579, fonts::Get_Font_Height_() + 96, buffer);

        int16_t cp_surplus = plr->command_points - plr->command_points_used;
        const char* cp_sign = (cp_surplus < 0) ? "-" : "+";
        _sprintf(buffer, s__s_d___d__00559958, cp_sign, cp_surplus, (int)plr->command_points);
        MISC::Set_Fitted_Font_Style_(buffer, &font_colors[8], 2, 2, 4, 0x43, 0);
        int16_t current_style = fonts::_current_font_style;
        int16_t current_spacing = MISC::Get_Font_Spacing_();
        MISC::Get_Mox_Font_Colors_(current_style, 'K', 'M', &font_colors[0]);
        MISC::Get_Mox_Font_Colors_(current_style, 14, 23, &font_colors[8]);

        char cp_left[64];
        _sprintf(cp_left, s__s_d_00556134, cp_sign, cp_surplus);
        int16_t cp_left_w = fonts::Get_String_Width_(cp_left);
        char cp_right[64];
        _sprintf(cp_right, s___d__00559950, (int)plr->command_points);
        int16_t cp_total_w = fonts::Get_String_Width_(cp_right) + cp_left_w + 4;

        if (cp_surplus < 0) {
            fonts::Set_Font_Style_Outline_(current_style, &font_colors[0]);
            fonts::Set_Outline_Color_(0x49, 0x49);
            fonts::Set_Font_Spacing_(current_spacing);
            MISC::Increase_Font_Color_Range_((MOX::_red_color_pulse_value << 2) / 7);
        } else {
            fonts::Set_Font_Style_Outline_(current_style, &font_colors[8]);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Set_Font_Spacing_(current_spacing);
        }
        int16_t cp_half_w = cp_total_w / 2;
        fonts::Print_(581 - cp_half_w, 181, cp_left);
        fonts::Set_Font_Style_(current_style, (MISC::s_colors*)&font_colors[8]);
        fonts::Set_Font_Spacing_(current_spacing);
        fonts::Print_(581 + cp_left_w + 4 - cp_half_w, 181, cp_right);

        fonts::Get_Font_Height_();
        char* food_comma = MISC::Get_Comma_String_(plr->surplus_food);
        const char* food_sign = (plr->surplus_food < 0) ? "-" : "+";
        _sprintf(buffer, s__s_s_00556aec, food_sign, food_comma);
        if (plr->surplus_food < 0) {
            MISC::Set_Fitted_Font_Style_(buffer, &font_colors[0], 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_('K', 'M');
            fonts::Set_Outline_Color_(0x49, 0x49);
            MISC::Increase_Font_Color_Range_((MOX::_red_color_pulse_value << 2) / 7);
        } else {
            MISC::Set_Fitted_Font_Style_(buffer, &font_colors[8], 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Set_Outline_Color_(2, 1);
        }
        fonts::Print_Centered_(579, 255, buffer);

        const char* fr_sign = (plr->surplus_freighters < 0) ? "-" : "+";
        _sprintf(buffer, s__s_d___d__00559958, fr_sign, (int)plr->surplus_freighters, (int)plr->n_freighters);
        MISC::Set_Fitted_Font_Style_(buffer, &font_colors[0], 2, 2, 4, 0x43, 1);
        int16_t fr_style = MISC::Get_Font_Style_();
        MISC::Get_Mox_Font_Colors_(fr_style, 'K', 'M', &font_colors[0]);
        MISC::Get_Mox_Font_Colors_(fr_style, 14, 23, &font_colors[8]);
        int16_t fr_spacing = MISC::Get_Font_Spacing_();
        int16_t fr_total_w = fonts::Get_String_Width_(buffer);

        _sprintf(buffer, s__s_d_00556134, fr_sign, (int)plr->surplus_freighters);
        if (plr->surplus_freighters < 0) {
            fonts::Set_Font_Style_Outline_(fr_style, &font_colors[0]);
            fonts::Set_Font_Spacing_(fr_spacing);
            MISC::Increase_Font_Color_Range_((MOX::_red_color_pulse_value << 2) / 7);
            fonts::Set_Outline_Color_(0x49, 0x49);
        } else {
            fonts::Set_Font_Style_Outline_(fr_style, &font_colors[8]);
            fonts::Set_Font_Spacing_(fr_spacing);
            fonts::Set_Outline_Color_(2, 1);
        }
        int16_t fr_half_w = (fr_total_w + 4) / 2;
        fonts::Print_(581 - fr_half_w, 330, buffer);

        fonts::Set_Outline_Color_(2, 1);
        int16_t fr_left_w = fonts::Get_String_Width_(buffer);
        _sprintf(buffer, s___d__00559950, (int)plr->n_freighters);
        fonts::Set_Font_Style_Outline_(fr_style, &font_colors[8]);
        fonts::Set_Font_Spacing_(fr_spacing);
        fonts::Print_(fr_left_w - fr_half_w + 583, 330, buffer);

        Get_Research_Unit_String_(research_unit);
        if (plr->research_breakthrough != 0) {
            fonts::Set_Outline_Color_(2, 1);
            fonts::Set_Font_Style_Outline_(0, &font_colors[8]);
            fonts::Print_Centered_(579, 403, HAROLD::H_Message_(0x183));
            return;
        }
        if (plr->current_research_field == 0) {
            fonts::Set_Outline_Color_(2, 1);
            MISC::Set_Fitted_Font_Style_(buffer, &font_colors[8], 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Print_Centered_(579, 403, HAROLD::H_Message_(0x188));
            return;
        }

        int16_t turns = COLCALC::Player_N_Turns_Until_Research_Complete_(plr);
        if (turns > 0) {
            int32_t chance = COLCALC::Chance_For_Research_Breakthrough_(plr);
            if (chance > 0) {
                _sprintf(buffer, s__d___0055612c, chance);
                MISC::Set_Fitted_Font_Style_(buffer, &font_colors[8], 2, 2, 4, 0x43, 1);
                Set_Post_Fitted_Font_Style_(14, 23);
                fonts::Set_Outline_Color_(2, 1);
                fonts::Print_Centered_(579, 403 - 2 * fonts::Get_Font_Height_(), buffer);
            }
            const char* turns_msg = HAROLD::H_Message_(turns == 1 ? 0xe1 : 0xe2);
            _sprintf(buffer, turns_msg, (int)turns);
            char at_buffer[64];
            _sprintf(at_buffer, s___s_0055994c, buffer);
            MISC::Set_Fitted_Font_Style_(at_buffer, &font_colors[8], 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            int16_t fh = fonts::Get_Font_Height_();
            int16_t turns_y = 403 - fh;
            fonts::Set_Outline_Color_(2, 1);
            int16_t msg_w = fonts::Get_String_Width_(buffer);
            int16_t start_x = (62 - msg_w) / 2 + 548;
            fonts::Print_(start_x, turns_y + 3, s___00559948);

            const char* turns_msg2 = HAROLD::H_Message_(turns == 1 ? 0xe1 : 0xe2);
            _sprintf(buffer, turns_msg2, (int)turns);
            fonts::Print_(fonts::Get_String_Width_(s___00559948) + start_x, turns_y, buffer);

            char* res_comma = MISC::Get_Comma_String_(plr->research_produced);
            _sprintf(buffer, s__s__s_00558014, res_comma, research_unit);
            MISC::Set_Fitted_Font_Style_(buffer, &font_colors[8], 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Print_Centered_(579, turns_y + fonts::Get_Font_Height_(), buffer);
        } else if (turns < 0) {
            _sprintf(buffer, s_0__s_00559940, research_unit);
            MISC::Set_Fitted_Font_Style_(buffer, &font_colors[8], 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Print_Centered_(579, 403, buffer);
        } else {
            int32_t chance = COLCALC::Chance_For_Research_Breakthrough_(plr);
            _sprintf(buffer, s__d___0055612c, chance);
            MISC::Set_Fitted_Font_Style_(buffer, &font_colors[8], 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Print_Centered_(579, 391, buffer);

            _sprintf(buffer, s__d__s_005569f0, (int)plr->research_produced, research_unit);
            MISC::Set_Fitted_Font_Style_(buffer, &font_colors[8], 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Print_Centered_(579, 391 + fonts::Get_Font_Height_(), buffer);
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
        uint8_t colors[8];
        fonts::Get_Colors_(colors);
        fonts::Set_Outline_Color_(2, 1);
        fonts::Set_Font_Style_Outline_(style, colors);
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
            uint8_t player_color_idx = (uint8_t)MOX::_player[surrender_to].color;
            uint32_t draw_color = (uint32_t)surrender_colors[player_color_idx];

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
            JIM::Fast_Fade_In_();
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
            uint8_t local_colors[8];
            MISC::s_colors* p_colors = MISC::Font_Colors2_(font_style, 1, 12);
            for (int i = 0; i < 8; ++i) local_colors[i] = p_colors->colors[i];

            if (color_offset > 0) {
                int16_t shift = (color_offset * 9) / 5;
                for (int i = 1; i < 8; ++i) {
                    int val = (int)local_colors[i] + shift;
                    if (val < (int)local_colors[0]) val = (int)local_colors[0];
                    if (val > 16) val = 16;
                    local_colors[i] = (uint8_t)val;
                }
            }
            fonts::Set_Font_Style_(font_style, (MISC::s_colors*)local_colors);
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
        uint8_t standard_colors[8];
        uint8_t player_colors[8];

        bool is_omniscient = HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM);
        HAROLD::Colony_Owners_(star_idx, owner_counts, &num_owners, !is_omniscient);

        if (num_owners <= 1) {
            return false;
        }

        int16_t total_colony_count = 0;
        for (int i = 0; i < 8; ++i) {
            total_colony_count += owner_counts[i];
        }

        fonts::Get_Colors_(standard_colors);
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
                HAROLD::Get_Player_Font_Colors_(i, fonts::_current_font_style, player_colors, false);
                MISC::Swap_Bitmap_Colors_(current_x, 0, current_x + width, height, standard_colors, player_colors, 8, bitmap);
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
                MOX::_moveable_box[i].field8_0xe = 0xfc18;
                MOX::_moveable_box[i].field9_0x10 = 0xfc18;
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
}