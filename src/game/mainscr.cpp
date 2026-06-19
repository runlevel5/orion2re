#include "pch.h"

namespace MAINSCR {
    int16_t _zoom_in_button;
    int16_t _zoom_out_button;
    int16_t _whole_screen_field;
    int16_t _game_button;
    int16_t _next_button;
    int16_t _design_button;
    int16_t _colonies_button;
    int16_t _planets_button;
    int16_t _fleets_button;
    int16_t _leaders_button;
    int16_t _races_button;
    int16_t _info_button;
    int16_t _treasury_window_field;
    int16_t _command_points_window_field;
    int16_t _food_window_field;
    int16_t _freighters_window_field;
    int16_t _research_window_field;
    int16_t _first_field;
    uint8_t _debug_enabled;
    int16_t _debug_field;
    int16_t _exit_game_hot_key;
    int16_t _main_screen_grid_y;
    int16_t _main_screen_grid_x;
    s_animation_header* _temp_bitmap;
    int16_t _main_screen_scanned_star;
    int8_t _moving_a_stack;
    int8_t _track_time_flag;
    int8_t _clear_fleet_fields;
    int8_t _mouse_btn_pressed;
    int8_t _merging_relocations;
    uint8_t _warn_about_cheaters;
    uint8_t _zoom_box_counter;
    uint8_t _zoom_box_color_increment;
    uint8_t _zoom_box_increment_value;
    int8_t _officer_screen_return_screen;
    int8_t _next_turn_hit;
    int16_t _zoom_box_x;
    int16_t _zoom_box_y;
    int8_t _fleet_ship_selection_status;
    int16_t _fleet_icon_chosen;

    s_hotseat_system_box_state _hotseat_system_box[MAX_PLAYERS];

    uint8_t _hotseat_map_scale[MAX_PLAYERS];

    s_hotseat_fleet_box_state _hotseat_fleet_box[MAX_PLAYERS];

    int8_t _star_name_highlight_direction = 1;

    uint8_t _star_name_highlight_value;

    uint8_t _check_next_turn_exit_field;
    int16_t _n_twinkling_stars;
    int16_t _main_screen_ship_icon_scanned;
    s_colors _ship_direction_line_green_colors = {{0x6E, 0x6F, 0x72, 0x74, 0x74, 0x72, 0x6F, 0x6E}};
    s_colors _ship_direction_line_red_colors = {{0x49, 0x4A, 0x4D, 0x4F, 0x4F, 0x4D, 0x4A, 0x49}};
    uint8_t _fltpop_field_selection_status;
    int8_t _diplo_lights_anim_frame[MAX_PLAYERS];
    s_animation_header* _player_lights_seg[MAX_PLAYERS];

    uint8_t _already_faded_into_main_screen;
    uint8_t _in_auto_next_turn_mode;

    int16_t _omniscient_hot_key = -1000;
    int16_t _all_tech_cheat = -1000;
    int16_t _leader_cheat = -1000;
    int16_t _more_money_cheat = -1000;
    int16_t _get_tech_cheat = -1000;
    int16_t _omniscience_cheat = -1000;
    int16_t _all_ai_cheat = -1000;
    int16_t _hi_score_cheat = -1000;
    int16_t _last_field;

    bool __cdecl Is_Main_Screen_Palette_() {
        uint8_t screen = (uint8_t)MOX::_previous_screen;

        if (screen == SCREEN_MAIN ||
            screen == SCREEN_UNK2 ||
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
            MOX::_star[i].owner = (int8_t)owner_id;
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

        MOX::_star_seg[final_index] = reinterpret_cast<s_animation_header*>(buffer::Buffer_Reload_(
            "BUFFER0.LBX",
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
                if (mouse::Mouse_Button_() != 0) {
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
            int8_t twinkling1 = (int8_t)(star->twinkling1 + 1);
            star->twinkling1 = twinkling1;

            int16_t frame_count = animate::Get_Animation_Frame_Count_(star_seg);
            int16_t current_frame = (int16_t)(((int16_t)twinkling1 / 2) % frame_count);

            if (static_cast<int8_t>(star->twinkling2) <= twinkling1) {
                int16_t remaining = _n_twinkling_stars - 1;
                if (remaining < 1) {
                    remaining = 0;
                }
                _n_twinkling_stars = remaining;

                star->twinkling1 = -1;
                star->twinkling2 = -1;
            }

            animate::Set_Animation_Frame_(star_seg, current_frame);
            animate::Draw_(screen_x, screen_y, star_seg);
        }
    }

    void __cdecl Print_Star_Names_() {
        int16_t window_status;
        int8_t star_type;
        s_animation_header* star_data;
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
                    Print_Star_Name(i, star_type, star_name, star_data);
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
        RUSS::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Relocation_Links_();
        RUSS::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Stars_();
        RUSS::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Gate_Icons_();
        RUSS::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Print_Star_Names_();
        RUSS::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Black_Holes_();
        RUSS::Mox_Update_();

        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Ship_Icons_(true);
        RUSS::Mox_Update_();

        graphics::Reset_Window_();
        video::Clip_Off_();
        Print_Main_Screen_Data_();
        RUSS::Mox_Update_();

        if (MOX::_game_type != 0) {
            Draw_Diplomacy_Request_Lights_();
            RUSS::Mox_Update_();
        }
    }

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
        MOX::_far_star_seg = reinterpret_cast<s_animation_header*>(buffer::Buffer_Reload_("STARBG.LBX", 0, MOX::_global_cache_seg));
        animate::Draw_(MOX::_far_star_x, MOX::_far_star_y, MOX::_far_star_seg);
        animate::Draw_(MOX::_far_star_x - 640, MOX::_far_star_y, MOX::_far_star_seg);
        animate::Draw_(MOX::_far_star_x - 640, MOX::_far_star_y - 480, MOX::_far_star_seg);
        animate::Draw_(MOX::_far_star_x, MOX::_far_star_y - 480, MOX::_far_star_seg);

        // Mid Stars - Layer 1
        MOX::_mid_star_seg = reinterpret_cast<s_animation_header*>(buffer::Buffer_Reload_("STARBG.LBX", 1, MOX::_global_cache_seg));
        animate::Draw_(MOX::_mid_star_x, MOX::_mid_star_y, MOX::_mid_star_seg);
        animate::Draw_(MOX::_mid_star_x - 640, MOX::_mid_star_y, MOX::_mid_star_seg);
        animate::Draw_(MOX::_mid_star_x - 640, MOX::_mid_star_y - 480, MOX::_mid_star_seg);
        animate::Draw_(MOX::_mid_star_x, MOX::_mid_star_y - 480, MOX::_mid_star_seg);

        // Near Stars - Layer 2
        MOX::_near_star_seg = reinterpret_cast<s_animation_header*>(buffer::Buffer_Reload_("STARBG.LBX", 2, MOX::_global_cache_seg));
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
        int16_t black_hole_slot = 0;
        int16_t x, y;

        int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();
        MISC::Save_Window_Status_(&window_status);
        graphics::Set_Window_(22, 22, 527, 421); // 0x16, 0x16, 0x20f, 0x1a5
        video::Clip_On_();

        if (MOX::_NUM_STARS > 0) {
            for (star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
                if (MOX::_star[star_idx].spectral_class == STAR_CLASS_BLACK_HOLE) {
                    Get_Star_Draw_Coords_(star_idx, &x, &y);

                    int16_t offset = zoom_dist[zoom_level] / 2;
                    x -= offset;
                    y -= offset;

                    s_animation_header* seg = Get_Star_Picture_Seg_(
                        MOX::_star[star_idx].spectral_class,
                        MOX::_star[star_idx].pict_type,
                        MOX::_star[star_idx].size
                    );

                    int16_t slot = black_hole_slot;
                    int8_t count = MOX::_black_hole_anim_count[slot];
                    int16_t frame_count = animate::Get_Animation_Frame_Count_(seg);

                    int8_t new_count = (int8_t)((count + 1) % (frame_count * 2));
                    MOX::_black_hole_anim_count[slot] = new_count;

                    animate::Set_Animation_Frame_(seg, new_count / 2);
                    animate::Draw_(x, y, seg);

                    black_hole_slot++;
                    if (black_hole_slot >= 10) {
                        black_hole_slot = 10;
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
            s_animation_header* portal_pic = (s_animation_header*)buffer::Buffer_Reload_("BUFFER0.LBX", zoom_offset + 19, MOX::_global_cache_seg);
            MOX::_gate_icon_seg[zoom_offset] = portal_pic;
            bitmap::Draw_Picture_To_Bitmap_(temp_icons[0], portal_pic);
            int16_t h = animate::Get_Height_(portal_pic);
            if (h > 0) {
                max_icon_height = h;
            }
            icon_count = 1;
        }

        if (has_artemis) {
            s_animation_header* artemis_pic = (s_animation_header*)buffer::Buffer_Reload_("BUFFER0.LBX", zoom_offset + 31, MOX::_global_cache_seg);
            MOX::_gate_icon_seg[12 + zoom_offset] = artemis_pic;
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

        if (MOX::_NUM_STARS > 0) {
            do {
                Draw_A_Gate_Icon_(star_idx);
                star_idx++;
            } while (star_idx < MOX::_NUM_STARS);
        }
    }

    /**
     * @brief Returns a 3-character unit abbreviation string for research points based on current language.
     * 
     * @param out Buffer of at least 3 bytes to store the result (including null terminator).
     */
    void __cdecl Get_Research_Unit_String_(char* out_buffer) {
        const char *src;

        switch (MOX::_settings.language) {
            case 1:
                src = "FP";
                break;
            case 2:
                src = "PR";
                break;
            case 3:
                src = "PI";
                break;
            case 4:
                src = "PR";
                break;
            default:
                src = "RP";
                break;
        }

        out_buffer[0] = src[0];
        out_buffer[1] = src[1];
        out_buffer[2] = src[2];
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

        MOX::_main_screen_seg = reinterpret_cast<s_animation_header*>(buffer::Buffer_Reload_("BUFFER0.LBX", 0, MOX::_global_cache_seg));
        animate::Draw_(0, 0, MOX::_main_screen_seg);

        Draw_Wormhole_Links_();
        Draw_Relocation_Links_();

        if (MOX::_game_type == 2 || MOX::_game_type == 3) {
            MOX::_mainscrn_diplo_lights_seg = reinterpret_cast<s_animation_header*>(buffer::Buffer_Reload_("BUFFER0.LBX", 0xD, MOX::_global_cache_seg));
            animate::Draw_(415, 0, MOX::_mainscrn_diplo_lights_seg);
        }

        if (MOX::_game_type == 2 || MOX::_game_type == 3 || MOX::_game_type == 1) {
            MOX::_mainscrn_plyr_done_lights_seg = reinterpret_cast<s_animation_header*>(buffer::Buffer_Reload_("BUFFER0.LBX", 0xD, MOX::_global_cache_seg));
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

    void __cdecl Print_Star_Name(int16_t star_id, int8_t use_bitmap, char* star_name, void* buffer) {
        HAROLD::Map_Scale_To_Zoom_Level_();
        int16_t zoom_size = HAROLD::Map_Scale_Star_Size_To_Zoom_Level_(star_id);

        int16_t draw_x, draw_y;
        Get_Star_Draw_Coords_(star_id, &draw_x, &draw_y);

        int16_t text_center_x = draw_x + 1;
        int16_t half_zoom_size = zoom_size / 2;
        int16_t text_y = half_zoom_size + (draw_y - (int16_t)MOX::_star[star_id].size);

        if (use_bitmap != 0) {
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
        if (static_cast<uint8_t>(surrender_to) < MAX_PLAYERS) {
            // Player colors derived from stack-initialized byte array
            static const uint8_t surrender_colors[MAX_PLAYERS] = { 0x80, 0x7b, 0x78, 0x2c, 0x86, 0x9f, 0x83, 0x7e };

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

        MOX::_moveable_box[0].id = system_id;

        bool box_was_selected = (MOVEBOX::Moveable_Box_Selected_(2) != 0);

        if (box_was_selected) {
            MOVEBOX::Vanish_Fleet_Movement_Box_();
            SYS::Clear_System_Display_Loaded_Flag_();
        }

        SYS::Init_System_Display_(MOX::_moveable_box[0].id);

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

        RUSS::Mox_Update_();
        MAINSCR2::_main_screen_fade_in_flag = 0;
        MAINSCR2::_force_main_screen_fade = 0;
        _already_faded_into_main_screen = 1;

        fields::Disable_Draw_All_Fields_();
        video::Set_Refresh_Stencil_();
        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        ERIC::Fast_Fade_Out_();
        RUSS::Mox_Update_();

        fonts::Load_Palette_(0, 0, 255);
        RUSS::Mox_Update_();
        palstore::Darken_Palette_(100);
        RUSS::Mox_Update_();

        MAINSCR::Add_Map_Fields_();
        RUSS::Mox_Update_();

        if (static_cast<int8_t>(box_status) != 0) {
            bool expanding_help_enabled = (MOX::_settings.expanding_help != 0);
            if (expanding_help_enabled) {
                MOX::_settings.expanding_help = 0;
            }
            RUSS::Mox_Update_();
            MAINSCR::Do_System_Popup_(MOX::_moveable_box[0].id);
            if (expanding_help_enabled) {
                MOX::_settings.expanding_help = 1;
            }
        }

        video::Set_Page_Off_();
        MAINSCR::Draw_Mini_Main_Screen_();
        RUSS::Mox_Update_();
        palette::Clear_Palette_Changes_(0, 255);
        RUSS::Mox_Sync_Update_();

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

        bool skip_fade_in = false;
        if (MOX::_reloaded_ship_palette != 0) {
            if (MOVEBOX::Moveable_Box_Selected_(2) == 0) {
                ERIC::Set_Fade_Percent_(0);
                skip_fade_in = true;
            }
        }

        if (!skip_fade_in) {
            ERIC::Fast_Fade_In_();
            RUSS::Mox_Update_();
        }

        MOX::_reloaded_ship_palette = 0;
        fields::Enable_Draw_All_Fields_();
    }

    void __cdecl Get_Star_Name_(int16_t star_idx, int8_t is_selected, int8_t* out_uses_bitmap, char* out_name, s_animation_header** out_bitmap_ptr) {
        bool galactic_lore = HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM);
        *out_name = '\0';
        *out_uses_bitmap = 0;
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
            bool has_contact = (owner >= 0 && owner < MAX_PLAYERS) && HAROLD::Contact_With_One_Colony_(star_idx);
            if (!has_contact) {
                if (!HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, star_idx)) return;

                char* src = star_ptr->name;
                char* dst = out_name;
                while ((*dst++ = *src++));

                int16_t font_style = HAROLD::Zoom_Level_Font_Style_();
                apply_font_offsets(font_style);

                if (is_outpost) {
                    MISC::Decrease_Font_Color_Range_(2);
                    strings::String_To_Lower_(out_name);
                }
                return;
            }
        }

        // Logic for Lore or Contacted
        if (galactic_lore && MOX::_PLAYER_NUM != owner && !HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, star_idx)) {
            sprintf(out_name, HAROLD::s___s__00556ae4, star_ptr->name);
        } else {
            char* src = star_ptr->name;
            char* dst = out_name;
            while ((*dst++ = *src++));
        }

        if (owner >= 0 && owner < MAX_PLAYERS) {
            HAROLD::Set_Player_Font_Style_(owner, (bool)is_selected);
        } else {
            int16_t font_style = HAROLD::Zoom_Level_Font_Style_();
            apply_font_offsets(font_style);
        }

        if (is_outpost) {
            MISC::Decrease_Font_Color_Range_(1);
            strings::String_To_Lower_(out_name);
        }

        if (HAROLD::N_Colony_Owners_(star_idx) > 1) {
            *out_uses_bitmap = Print_Star_Name_To_Bitmap_(star_idx, out_name, out_bitmap_ptr);
        }
    }

    bool __cdecl Print_Star_Name_To_Bitmap_(int16_t star_idx, char* star_name, s_animation_header** out_bitmap_ptr) {
        int16_t owner_counts[MAX_PLAYERS];
        int16_t num_owners;
        s_colors standard_colors;
        s_colors player_colors;

        bool is_omniscient = HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM);
        HAROLD::Colony_Owners_(star_idx, owner_counts, &num_owners, !is_omniscient);

        if (num_owners <= 1) {
            return false;
        }

        int16_t total_colony_count = 0;
        for (int i = 0; i < MAX_PLAYERS; ++i) {
            total_colony_count += owner_counts[i];
        }

        fonts::Get_Colors_(&standard_colors);
        fonts::Set_Font_Spacing_(1);
        allocate::Push_Block_(MOX::_screen_seg);

        uint32_t bitmap_size = bitmap::Get_Bitmap_Size_(200, 20);
        s_animation_header* bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);
        *out_bitmap_ptr = bitmap;

        bitmap::Create_Blank_Bitmap_(200, 20, bitmap);
        fonts::Print_To_Bitmap_(0, 0, star_name, bitmap);

        int16_t current_x = 0;
        for (int16_t i = 0; i < MAX_PLAYERS; ++i) {
            if (owner_counts[i] > 0) {
                int16_t width, height, d1, d2;
                MISC::Get_Bitmap_String_Range_(owner_counts[i], total_colony_count, &width, &d1, &d2, &height, star_name);
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
                MOX::_moveable_box[i].drag_field = -1000;
                MOX::_moveable_box[i].close_btn = -1000;
            }

            _zoom_in_button = fields::Add_Button_Field_(244, 428, "", MOX::_main_zoom_in_button_seg, "+", 0x28);
            _zoom_out_button = fields::Add_Button_Field_(244, 455, "", MOX::_main_zoom_out_button_seg, "-", 0x28);
            _whole_screen_field = fields::Add_Hidden_Field_(0, 0, 639, 479, "", 0x29);
            return;
        }

        MOVEBOX::Moveable_Box_Selected_(2);
        _whole_screen_field = (int16_t)0xfc18;

        _game_button = fields::Add_Button_Field_(249, 5, "", MOX::_main_game_button_seg, "G", 0x29);
        _next_button = fields::Add_Button_Field_(544, 441, "", MOX::_main_next_button_seg, "T", 0x28);
        _zoom_in_button = fields::Add_Button_Field_(244, 428, "", MOX::_main_zoom_in_button_seg, "+", 0x28);
        _zoom_out_button = fields::Add_Button_Field_(244, 455, "", MOX::_main_zoom_out_button_seg, "-", 0x28);

        _colonies_button = fields::Add_Irregular_Button_Field_(17, 434, 79, 471, "", MOX::_main_colonies_button_seg, 0, 425, "C", 0x28);
        _planets_button = fields::Add_Irregular_Button_Field_(91, 434, 154, 471, "", MOX::_main_planets_button_seg, 0, 425, "P", 0x28);
        _fleets_button = fields::Add_Irregular_Button_Field_(167, 434, 230, 471, "", MOX::_main_fleets_button_seg, 0, 425, "F", 0x28);
        _leaders_button = fields::Add_Irregular_Button_Field_(312, 435, 379, 471, "", MOX::_main_leaders_button_seg, 306, 425, "L", 0x28);
        _races_button = fields::Add_Irregular_Button_Field_(386, 435, 453, 471, "", MOX::_main_races_button_seg, 306, 425, "R", 0x28);
        _info_button = fields::Add_Irregular_Button_Field_(462, 435, 526, 471, "", MOX::_main_info_button_seg, 306, 425, "I", 0x28);

        _treasury_window_field = fields::Add_Hidden_Field_(545, 49, 613, 119, "", 0x29);
        _command_points_window_field = fields::Add_Hidden_Field_(545, 122, 613, 192, "", 0x29);
        _food_window_field = fields::Add_Hidden_Field_(545, 197, 613, 267, "", 0x29);
        _freighters_window_field = fields::Add_Hidden_Field_(545, 270, 613, 340, "", 0x29);
        _research_window_field = fields::Add_Hidden_Field_(545, 344, 613, 415, "", 0x29);

        Add_Moveable_Box_Fields_();

        int16_t window_status;
        MISC::Save_Window_Status_(&window_status);
        graphics::Set_Window_(22, 22, 527, 421);
        video::Clip_On_();
        MISC::Restore_Window_Status_(window_status);

        _first_field = MOX::_star_button[0];

        if (_debug_enabled) {
            _debug_field = fields::Add_Hidden_Field_(630, 470, 639, 479, "", 0);
        } else {
            _debug_field = (int16_t)0xfc18;
        }

        graphics::Reset_Window_();
        _exit_game_hot_key = fields::Add_Multi_Hot_Key_Field_("Q");

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
        sprintf(buffer, format_str, (int)num_granted);

        HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, buffer, 0);
    }

    /**
     * Handles main-screen cheat and debug hotkeys.
     *
     * @param key Activated hotkey field id.
     * @param unused_param Unused legacy parameter kept for compatibility.
     */
    void __cdecl Check_Hot_Keys_(int16_t key, int32_t unused_param) {
        (void)unused_param;
        char message_buffer[80];
        char dialog_text[200];

        if ((key == _omniscient_hot_key && capture::Check_Release_Version_() == 0) ||
            key == _omniscience_cheat) {
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

            strcpy(dialog_text, msg_src);
            HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, dialog_text, 0);
            MOX::_draw_main_screen_to_back = 1;
            Update_Star_Owners_();
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
            sprintf(message_buffer, msg_fmt, player->race_name, 1000);
            HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, message_buffer, 0);
            Set_Player_Cheated_Flag_(MOX::_PLAYER_NUM, 3);
            return;
        }

        if (key == _get_tech_cheat) {
            s_player* player = &MOX::_player[MOX::_PLAYER_NUM];
            const char* tech_name = TECHDATA::_technology_applications[(uint8_t)player->current_research_application].name;
            const char* msg_fmt = HAROLD::H_Message_(216); // HESTR_0D8_S_WILL_BE_KNOWN
            sprintf(message_buffer, msg_fmt, tech_name);
            player->research_accumulated = 9999;
            HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, message_buffer, 0);
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
            sprintf(dialog_text, "Current score for %s is %d", player->name, (int)score);
            HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, dialog_text, 0);
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
                    MAINSCR::Wrapper_For_Normal_System_Fields_();
                }
            }

            if (order_id == 2) {
                if (MOVEBOX::Moveable_Box_Selected_(2) != 0) {
                    int16_t node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
                    if (node_ptr == -1) {
                        MOVEBOX::Vanish_Fleet_Movement_Box_();
                    } else {
                        int16_t flag = (MOX::_moveable_box[2].active == 0) ? 1 : 0;
                        FLEETPOP::Add_Fleet_Movement_Box_Fields_(node_ptr, flag);
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

                    sprintf(buffer, format_msg, planet_name);

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
        _all_tech_cheat = fields::Add_Multi_Hot_Key_Field_("EINSTEIN");
        _more_money_cheat = fields::Add_Multi_Hot_Key_Field_("MOOLA");
        _get_tech_cheat = fields::Add_Multi_Hot_Key_Field_("MENLO");
        _omniscience_cheat = fields::Add_Multi_Hot_Key_Field_("ISEEALL");
        _hi_score_cheat = fields::Add_Multi_Hot_Key_Field_("SCORE");
        _last_field = fields::_fields_count - 1;
    }

    void __cdecl Highlight_Star_(int16_t star_idx) {
        if (MOX::_star[star_idx].spectral_class != STAR_CLASS_BLACK_HOLE) {
            Print_Highlighted_Star_Name_(star_idx);

            MOX::_selected_star_color_offset += MOX::_selected_star_color_mod;

            if (MOX::_selected_star_color_offset == 3 || MOX::_selected_star_color_offset == -1) {
                MOX::_selected_star_color_mod = -MOX::_selected_star_color_mod;
            }
        }
    }

    void __cdecl Print_Highlighted_Star_Name_(int16_t star_idx) {
        int16_t window_status_cookie;
        int8_t is_highlighted;
        char star_name[32];
        s_animation_header* bitmap_ptr = nullptr;

        MISC::Save_Window_Status_(&window_status_cookie);
        graphics::Set_Window_(0x16 /* 22 */, 0x16 /* 22 */, 0x20f /* 527 */, 0x1a5 /* 421 */);
        video::Clip_On_();

        MAINSCR::Get_Star_Name_(star_idx, 1, &is_highlighted, star_name, &bitmap_ptr);

        if (star_name[0] != '\0') {
            if (is_highlighted) {
                MISC::Tint_Bitmap_Colors_(0, 0, 0, 0, 1, MAINSCR::_star_name_highlight_value, bitmap_ptr);

                MAINSCR::_star_name_highlight_value += MAINSCR::_star_name_highlight_direction;

                if (MAINSCR::_star_name_highlight_value == 0 || MAINSCR::_star_name_highlight_value == 3) {
                    MAINSCR::_star_name_highlight_direction = -MAINSCR::_star_name_highlight_direction;
                }
            }

            MAINSCR::Print_Star_Name(star_idx, is_highlighted, star_name, bitmap_ptr);
        }

        MISC::Restore_Window_Status_(window_status_cookie);
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
            if (MOX::_mouse_XY.hovering_star_idx > -1 && type == 0) {
                *out_field = MOX::_mouse_XY.hovering_star_idx;
                return 4;
            }
            if (MOX::_mouse_XY.hovering_ship_icon_idx > -1 && type == 1) {
                *out_field = MOX::_mouse_XY.hovering_ship_icon_idx;
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
            if (MOX::_mouse_XY.hovering_ship_icon_idx > -1 && type == 1) {
                *out_field = MOX::_mouse_XY.hovering_ship_icon_idx;
                return 4;
            }
            if (MOX::_mouse_XY.hovering_star_idx > -1 && type == 0) {
                *out_field = MOX::_mouse_XY.hovering_star_idx;
                return 4;
            }
        }

        return -1;
    }

    void __cdecl Check_Stars_XY_(int16_t x, int16_t y, int8_t selected) {
        static const int32_t thresholds[3] = { 12, 10, 8 };
        char local_name1[0x26] = "None";
        char local_name2[0x26] = "None";
        bool found = false;

        (void)local_name1;
        (void)local_name2;

        MOX::_mouse_XY.star_idx = -1;
        MOX::_mouse_XY.ship_icon_idx = -1;

        if (MOX::_NUM_STARS <= 0) {
            return;
        }

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            if (found) {
                return;
            }

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

            int32_t threshold = thresholds[(int32_t)star.size];
            if (dist_sq <= threshold * threshold) {
                MOX::_mouse_XY.star_idx = star_idx;
                MOX::_mouse_XY.selected = selected;
                ERICNET::Mox_Play_Sound_(0x29);
                found = true;
            }
        }
    }

    void __cdecl Exit_Zooming_Mode_(int16_t* zooming_state) {
        *zooming_state = 0;
        MOX::_zoom_in = 0;
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
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

        MOX::_moveable_box[2].id = 1;
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
            MOX::_moveable_box[2].id = 1;
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

            if (MISC::Occluded_(box_x1, box_y1, box_x2, box_y2, icon_x, icon_y, icon_x + 13, icon_y + 10)) {
                MOX::_moveable_box[2].x = 619;
                MOX::_moveable_box[2].y = 421;
            }
        }
    }

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

        if (owner >= 0 && owner < MAX_PLAYERS) {
            HAROLD::Set_Player_Font_Style_(owner, 0);
            char* race_name = HACCESS::Race_Name_String_(owner);

            sprintf(print_buffer, MOX::s_fmt_scanned_star_info, star->name, race_name);
        } else if (owner == -1) {
            for (int16_t i = 0; i < 8; ++i) {
                if (i < 2) {
                    text_colors.colors[i] = 0x08;
                } else {
                    text_colors.colors[i] = 0x0C;
                }
            }
            sprintf(print_buffer, MOX::s_fmt_s, star->name);
        } else if (owner == -2) {
            for (int16_t i = 0; i < 8; ++i) {
                if (i < 2) {
                    text_colors.colors[i] = 0x14;
                } else {
                    text_colors.colors[i] = 0x1F;
                }
            }
            sprintf(print_buffer, MOX::s_fmt_contested, star->name);
        }

        if (owner == -1 || owner == -2) {
            fonts::Set_Font_Style_(2, &text_colors);
        }

        s_movable_box box = MOX::_moveable_box[1];
        int16_t final_x = box.x + (int16_t)box.content_offset_x + 15;
        int16_t final_y = (int16_t)box.height + (box.y + (int16_t)box.content_offset_y) - 25;

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
                int16_t button = mouse::Mouse_Button_();
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
        MOX::_mouse_XY.hovering_star_idx = -1;
        MOX::_mouse_XY.hovering_ship_icon_idx = -1;

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
            if (MOX::_system_display_ships[i].pic != NULL && MOX::_system_display_ships[i].button == input) {
                *ship_idx = MOX::_system_display_ships[i].ship_idx;
                ret_val = 0;
                break;
            }
        }

        // 2. Check for hovered ships if no click hit
        if (ret_val == -1) {
            for (int i = 0; i < 15; ++i) {
                if (MOX::_system_display_ships[i].pic != NULL && MOX::_system_display_ships[i].button == mouse_input) {
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
        for (int32_t i = 0; i < MAX_STARS; ++i) {
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
                
                sprintf(output_msg, lbx_msg, name_buf, race_buf);

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
        int16_t star_idx = (int16_t)MOX::_planet[player->home_planet_id].star_index;

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
        fields::Assign_Auto_Function_(MAINSCR::Draw_Main_Screen_, 1);
    }

    int16_t __cdecl Do_Reply_To_Diplomacy_Request_(int16_t requester_id) {
        int16_t unused_field;
        int16_t first_reply_field;
        int16_t second_reply_field;
        int16_t third_reply_field;
        int16_t selected_reply = 0;
        bool has_selection = false;

        fields::Clear_Fields_();
        MAINSCR::Clear_Main_Screen_Fields_();
        video::Set_Page_Off_();
        MAINSCR::Draw_Mini_Main_Screen_();
        video::Copy_Off_To_Back_();
        video::Set_Refresh_Stencil_();
        RUSS::Mox_Sync_Update_();

        MOX::_temp_string[0] = '\0';
        MOX::_screen_data = requester_id;

        fields::Clear_Fields_();
        MAINSCR::Clear_Main_Screen_Fields_();
        MAINSCR::Load_Reply_To_Diplomacy_Request_Popup_();
        MAINSCR::Add_Reply_To_Diplomacy_Request_Fields_(
                &first_reply_field,
                &second_reply_field,
                &third_reply_field,
                &unused_field);
        fields::Assign_Auto_Function_(MAINSCR::Draw_Reply_To_Diplomacy_Request_, 1);

        const int32_t popup_width =
                static_cast<int32_t>(animate::Get_Width_(MOX::_diplomacy_request_popup_seg));
        const int32_t popup_x = ((0x1FA - popup_width) / 2) + 0x15;

        const int32_t popup_height =
                static_cast<int32_t>(animate::Get_Height_(MOX::_diplomacy_request_popup_seg));
        const int32_t popup_y = ((0x190 - popup_height) / 2) + 0x15;

        EVANHELP::Set_Diplomacy_Request_Help_List_(popup_x, popup_y);

        while (!has_selection) {
            const int16_t input_field = fields::Get_Input_();

            if (first_reply_field == input_field) {
                selected_reply = 1;
                has_selection = true;
            } else if (second_reply_field == input_field) {
                selected_reply = 2;
                has_selection = true;
            } else if (third_reply_field == input_field) {
                selected_reply = 3;
                has_selection = true;
            }

            if (!has_selection) {
                video::Set_Page_Off_();
                MAINSCR::Draw_Reply_To_Diplomacy_Request_();
                RUSS::Mox_Sync_Update_();
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
        EVANHELP::Set_Main_Screen_Help_List_();
        MAINSCR::_already_faded_into_main_screen = 0;
        MOX::_FULL_DRAW_SCREEN_FLAG = 1;

        return selected_reply;
    }

    int16_t __cdecl Get_Star_Id_For_Officers_Screen_() {
        int16_t player_num = MOX::_PLAYER_NUM;
        int16_t home_planet_id = MOX::_player[player_num].home_planet_id;
        int16_t star_idx = -1;

        // Check if a specific system is selected in the UI
        if (MOVEBOX::Moveable_Box_Selected_(0) != 0) {
            star_idx = MOX::_moveable_box[0].id;
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

        if (first_stack_idx > -1) {
            int16_t ship_idx = MOX::_ship_node[first_stack_idx].ship_idx;

            if (MOX::_ship[ship_idx].owner != MOX::_PLAYER_NUM) {
                for (int16_t i = 0; i < MOX::_ship_stack_count; ++i) {
                    int16_t current_stack_idx = MOX::_ship_stack_start[i];
                    int16_t current_ship_idx = MOX::_ship_node[current_stack_idx].ship_idx;

                    if (MOX::_ship[current_ship_idx].owner == MOX::_PLAYER_NUM) {
                        return current_stack_idx;
                    }
                }
                return -1;
            }
        }

        return first_stack_idx;
    }

    void __cdecl Tax_Rate_Popup_() {
        int16_t scan_val;
        int16_t input_val;
        int16_t exit_flag;
        s_colors font_colors;

        font_colors.colors[0] = 0x9f;
        font_colors.colors[1] = 0xa0;
        font_colors.colors[2] = 0xa1;
        font_colors.colors[3] = 0xa2;
        input_val = 0;
        font_colors.colors[4] = 0;
        exit_flag = 0;
        font_colors.colors[5] = 0;
        font_colors.colors[6] = 0;
        font_colors.colors[7] = 0;

        MAINPUPS::_g_temp_tax_rate = (int16_t)(uint8_t)MOX::_player[MOX::_PLAYER_NUM].tax_rate;

        MAINPUPS::_last_scanned_field = -1;
        fonts::Set_Font_Style_Shadow_Down_(1, &font_colors);
        MAINPUPS::Load_Tax_Rate_Popup_();
        MAINPUPS::Do_Popup_Setup_(-1, MAINPUPS::_anims[0x24]);
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
        MOX::_ship_icon_player_id = -1;
    }

    /**
     * Mirrors the fleet-box field selection state into the underlying ship-node selection flags.
     *
     * @param field_to_select Fleet movement field id to apply.
     */
    void __cdecl Set_Painted_Fleet_Fields_(int16_t field_to_select) {
        int16_t box_ptr_idx = 0;
        int16_t icon_idx = MOX::_first_fleet_movement_icon;
        int32_t limit = (int32_t)MOX::_fleet_icon_buttons_added + (int32_t)icon_idx;

        if (limit > (int32_t)MOX::_fleet_box.count) {
            limit = (int32_t)MOX::_fleet_box.count;
        }

        while (icon_idx < (int16_t)limit) {
            if (MOX::_fleet_movement_icon[icon_idx].field == field_to_select) {
                int16_t movement_icon_idx = MOX::_fleet_movement_box_ptr[box_ptr_idx];
                int16_t node_idx = MOX::_fleet_movement_icon[movement_icon_idx].ship_node_idx;

                if (MAINSCR::Ship_Can_Be_Selected_(MOX::_ship_node[node_idx].ship_idx)) {
                    MOX::_ship_node[node_idx].selected = (int8_t)MAINSCR::_fltpop_field_selection_status;
                }
            }
            box_ptr_idx++;
            icon_idx++;
        }
    }

    void __cdecl Center_Map_(int16_t target_x, int16_t target_y) {
        int16_t old_x = MOX::_cur_map_x;
        int16_t old_y = MOX::_cur_map_y;

        // Calculate top-left corner based on centered target
        MOX::_cur_map_x = target_x - (int16_t)HAROLD::Get_Up_Scaled_Value_(253 /* 0xfd */);
        MOX::_cur_map_y = target_y - (int16_t)HAROLD::Get_Up_Scaled_Value_(200 /* 0xc8 */);

        // Clamp X to bounds [0, MAX_X - view_width]
        if (MOX::_cur_map_x < 0) {
            MOX::_cur_map_x = 0;
        }
        
        int maxX_limit = (int)MOX::_MAP_MAX_X - HAROLD::Get_Up_Scaled_Value_(506 /* 0x1fa */);
        if ((int)MOX::_cur_map_x > maxX_limit) {
            MOX::_cur_map_x = (int16_t)MOX::_MAP_MAX_X - (int16_t)HAROLD::Get_Up_Scaled_Value_(506 /* 0x1fa */);
        }

        // Clamp Y to bounds [0, MAX_Y - view_height]
        if (MOX::_cur_map_y < 0) {
            MOX::_cur_map_y = 0;
        }

        int maxY_limit = (int)MOX::_MAP_MAX_Y - HAROLD::Get_Up_Scaled_Value_(400 /* 0x190 */);
        if ((int)MOX::_cur_map_y > maxY_limit) {
            MOX::_cur_map_y = (int16_t)MOX::_MAP_MAX_Y - (int16_t)HAROLD::Get_Up_Scaled_Value_(400 /* 0x190 */);
        }

        // If viewport position changed, update parallax offsets
        if (old_x != MOX::_cur_map_x || old_y != MOX::_cur_map_y) {
            MAINSCR::Get_New_Paralax_XYs_((int16_t)old_x, (int16_t)old_y, 1);
        }

        MOX::_draw_main_screen_to_back = 1;
    }

    void __cdecl Add_Star_Fields_() {
        int16_t zoom_radii[4] = { 0x27, 0x21, 0x21, 0x18 };

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            int16_t x_coord;
            int16_t y_coord;

            MAINSCR::Get_Star_Draw_Coords_(star_idx, &x_coord, &y_coord);

            int16_t scale = HAROLD::Map_Scale_To_Zoom_Level_();
            int16_t radius;

            if (MOX::_star[star_idx].spectral_class == STAR_CLASS_BLACK_HOLE) {
                radius = zoom_radii[scale];
            } else {
                radius = HAROLD::Map_Scale_Star_Size_To_Zoom_Level_(star_idx);
            }

            int16_t adjusted_radius = (radius / 2) - 2;

            MOX::_star_button[star_idx] = fields::Add_Clipped_Circle_Field_(
                x_coord, y_coord, adjusted_radius, "", '\0'
            );
        }
    }

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

    void* __cdecl Allocate_First_Screen_Seg_(int32_t unused_param) {
        (void)unused_param;
        // Allocate 76,000 bytes (0x128E0) for other ships
        MOX::_other_ships_screen_seg = allocate::Allocate_First_Block_(MOX::_screen_seg, 76000 /* 0x128E0 */);

        // Update block metrics (return value is unused in assembly)
        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);

        // Determine space required for a 60x60 bitmap and allocate it next
        uint32_t bitmap_size = bitmap::Get_Bitmap_Size_(60 /* 0x3C */, 60 /* 0x3C */);
        MOX::_global_bitmap_seg = allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);

        return MOX::_global_bitmap_seg;
    }

    bool __cdecl Ship_Can_Be_Selected_(int16_t ship_idx) {
        return HACCESS::Player_Can_Order_Ship_(MOX::_PLAYER_NUM, &MOX::_ship[ship_idx]);
    }

    void __cdecl Do_Next_Turn_(int16_t* exit_flag, int8_t auto_mode) {
        MOX::_at_least_one_report = 0;
        MOX::_previous_report = 0;
        MOX::_just_moved_stack = 0;
        MOX::_animated_star = 0xFFFF;

        _warn_about_cheaters = static_cast<uint8_t>(1 << (MOX::_PLAYER_NUM & 0x1F));

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
                MOX::_global_exit_field = fields::Add_Hidden_Field_(0, 0, 639, 479, "\x1B", 41);
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
                mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                _next_turn_hit = 0;
                fields::Clear_Fields_();
                MOX::_current_screen = SCREEN_MAIN;
            }
        }

        *exit_flag = 1;
    }

    void __cdecl Draw_Player_Done_Lights_(void)
    {
        int8_t all_humans_ready_flag = 0;
        int16_t n_humans = HAROLD::Get_N_Remaining_Humans_();
        int16_t done_count = 0;

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_game_type == 1 /* GAME_HOTSEAT */) {
                if ((int16_t)MULTPLAY::Hotseat_Player_Status_(i) == 1) {
                    done_count++;
                }
            }
            else if (MOX::_game_type == 2 || MOX::_game_type == 3) {
                if (NETMOX::_net.players_hit_next_turn[i] != 0) {
                    done_count++;
                }
            }
        }

        if ((n_humans - 1) <= done_count) {
            all_humans_ready_flag = 1;
        }

        int16_t draw_idx = 0;
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            bool should_draw = false;

            if (MOX::_game_type == 2 || MOX::_game_type == 3) {
                if (NETMOX::_net.players_hit_next_turn[i] != 0) {
                    should_draw = true;
                }
            }
            else if (MOX::_game_type == 1) {
                if ((int16_t)MULTPLAY::Hotseat_Player_Status_(i) == 1) {
                    should_draw = true;
                }
            }

            if (should_draw) {
                int8_t color = MOX::_player[i].color;
                int16_t anim_width = animate::Get_Width_(MAINSCR::_player_lights_seg[0]);
                int16_t x_pos = (anim_width * draw_idx) + 0x49;

                if (all_humans_ready_flag == 0) {
                    animate::Set_Animation_Frame_(MAINSCR::_player_lights_seg[color], 5);
                }

                animate::Draw_(x_pos, 5, MAINSCR::_player_lights_seg[color]);
                draw_idx++;
            }
        }
    }

    void __cdecl Draw_Game_Turn_Buttons_() {
        animate::Reset_Animation_Frame_(MOX::_main_next_button_seg);
        animate::Draw_(544, 441, MOX::_main_next_button_seg);

        animate::Reset_Animation_Frame_(MOX::_main_game_button_seg);
        animate::Draw_(249, 5, MOX::_main_game_button_seg);
    }

    void __cdecl Draw_Main_Screen_Before_Stencil_() {
        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        MAINSCR::Draw_Main_Main_Screen_();
        video::Copy_Off_To_Back_();

        for (int16_t i = 4; i >= 0; --i) {
            int16_t box_id = MOX::_moveable_box_order[i];
            if (MOVEBOX::Moveable_Box_Selected_(box_id) != 0) {
                if (MOX::_stencil_box_id != box_id) {
                    int16_t mouse_btn = mouse::Mouse_Button_();
                    MOVEBOX::Move_Moveable_Box_(box_id, mouse_btn);
                    MOVEBOX::Draw_Moveable_Box_(box_id);
                }
            }
        }

        MAINSCR::Draw_Game_Turn_Buttons_();
        MOVEBOX::Draw_Moveable_Box_(MOX::_stencil_box_id);
    }

    void __cdecl Scan_Ships_XY_(int16_t mouse_x, int16_t mouse_y) {
        bool found = false;
        char icon_label[80];

        for (int16_t i = 0; i < MOX::_ship_icon_count; ++i) {
            if (found) {
                return;
            }

            int16_t ship_idx = MOX::_ship_node[MOX::_ship_icon[i].node_idx].ship_idx;
            int16_t owner = HACCESS::Ship_Owner_(ship_idx);
            s_animation_header* pict_seg = SHIPS::Get_Ship_Icon_Pict_Seg_(owner, 0);

            if (fields::_field_box_mode == 1) {
                int16_t h = animate::Get_Height_(pict_seg);
                int16_t w = animate::Get_Width_(pict_seg);
                graphics::Box_(MOX::_ship_icon[i].x, MOX::_ship_icon[i].y, w, h, 1, 10 /* 0xA */);
            }

            int16_t icon_x = MOX::_ship_icon[i].x;
            int16_t icon_y = MOX::_ship_icon[i].y;
            int16_t width = animate::Get_Width_(pict_seg);
            int16_t height = animate::Get_Height_(pict_seg);

            if (mouse_x >= icon_x && mouse_x <= (icon_x + width) &&
                mouse_y >= icon_y && mouse_y <= (icon_y + height)) {

                if (!MAINSCR::_moving_a_stack || MOX::_mouse_XY.hovering_star_idx == -1) {
                    MOX::_mouse_XY.hovering_ship_icon_idx = i;
                    MOX::_mouse_XY.selected = 0;
                    sprintf(icon_label, "ship icon %d", (int)i);
                    MOX::_mouse_XY.hovering_star_idx = -1;
                }
                found = true;
            }
        }
    }

    /**
     * Cycles through stars to find the next or previous one where the player has a colony.
     * 
     * @param p_star_idx Pointer to the current star index; updated to the new star index if found.
     * @param player_idx The player whose colonies are being searched.
     * @param direction  Direction to cycle (0 for next, non-zero for previous).
     */
    void __cdecl Cycle_Star_Icons_(int16_t* p_star_idx, int16_t player_idx, int16_t direction) {
        bool found = false;
        int16_t current_idx = *p_star_idx;

        // Search from current position to the edge of the star list
        while (true) {
            if (direction == 0 /* NEXT */) {
                current_idx++;
            } else {
                current_idx--;
            }

            bool in_bounds;
            if (direction == 0 /* NEXT */) {
                in_bounds = (int16_t)MOX::_NUM_STARS > current_idx;
            } else {
                in_bounds = current_idx > -1;
            }

            if (!in_bounds) {
                break;
            }

            if (found) {
                return;
            }

            if (HAROLD::Player_Has_Colony_(player_idx, current_idx)) {
                *p_star_idx = current_idx;
                found = true;
            }
        }

        if (found) {
            return;
        }

        // Wrap around to the other side of the list if nothing was found yet
        if (direction == 0 /* NEXT */) {
            current_idx = 0;
        } else {
            current_idx = (int16_t)MOX::_NUM_STARS - 1;
        }

        while (true) {
            bool in_bounds;
            if (direction == 0 /* NEXT */) {
                in_bounds = (int16_t)MOX::_NUM_STARS > current_idx;
            } else {
                // Backward wrap check ensures we don't pass the original star index
                in_bounds = (*p_star_idx - 1) < current_idx;
            }

            if (!in_bounds) {
                break;
            }

            if (found) {
                return;
            }

            if (HAROLD::Player_Has_Colony_(player_idx, current_idx)) {
                *p_star_idx = current_idx;
                found = true;
            }

            if (direction == 0 /* NEXT */) {
                current_idx++;
            } else {
                current_idx--;
            }
        }

        if (!found) {
            *p_star_idx = -1;
        }
    }

    void __cdecl Scan_Stars_XY_(int16_t mouse_x, int16_t mouse_y) {
        int32_t star_range_by_size[3];
        star_range_by_size[0] = 12;
        star_range_by_size[1] = 10;
        star_range_by_size[2] = 8;

        int16_t zoom_box_sizes[4];
        zoom_box_sizes[0] = 39;
        zoom_box_sizes[1] = 33;
        zoom_box_sizes[2] = 33;
        zoom_box_sizes[3] = 24;

        bool star_processed = false;
        _moving_a_stack = 0;

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            if (star_processed) {
                return;
            }

            int16_t star_draw_x, star_draw_y;
            Get_Star_Draw_Coords_(star_idx, &star_draw_x, &star_draw_y);

            int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();
            int16_t box_size;

            if (MOX::_star[star_idx].spectral_class == STAR_CLASS_BLACK_HOLE) {
                box_size = zoom_box_sizes[zoom_level];
            } else {
                box_size = HAROLD::Map_Scale_Star_Size_To_Zoom_Level_(star_idx);
            }

            int32_t dx = (int32_t)star_draw_x - (int32_t)mouse_x;
            int32_t dy = (int32_t)star_draw_y - (int32_t)mouse_y;
            int32_t dist_sq = (dx * dx) + (dy * dy);

            int32_t range = star_range_by_size[(int32_t)MOX::_star[star_idx].size];
            if (dist_sq <= (range * range) && MOX::_mouse_XY.hovering_ship_icon_idx == -1) {
                s_colors font_colors;
                font_colors.colors[0] = 0;
                font_colors.colors[1] = 10;
                font_colors.colors[2] = 12;
                font_colors.colors[3] = 14;
                font_colors.colors[4] = 16;
                font_colors.colors[5] = 18;
                font_colors.colors[6] = 20;
                font_colors.colors[7] = 22;

                if (FLEETPOP::Some_Icon_Selected_()) {
                    int16_t selected_ship_ids[MAX_SHIPS];
                    HACCESS::Get_Fleet_Box_Selected_Ship_Ids_(selected_ship_ids, MAX_SHIPS, 0xFF);
                    SHIPMOVE::Ships_Try_To_Move_To_(selected_ship_ids, star_idx, &MOX::_g_ship_move_info);
                    _moving_a_stack = 1;
                }

                MOX::_mouse_XY.selected = 0;
                MOX::_mouse_XY.hovering_ship_icon_idx = -1;
                MOX::_mouse_XY.hovering_star_idx = star_idx;

                fonts::Set_Font_Style_(2, &font_colors);
                fonts::Set_Font_Spacing_(1);

                s_star_data* star_ptr = &MOX::_star[star_idx];
                int16_t text_width = fonts::Get_String_Width_(star_ptr->name);
                
                char buffer[80];
                sprintf(buffer, "%s, len = %d", star_ptr->name, (int32_t)text_width);
                
                star_processed = true;
            }

            if (fields::_field_box_mode == 1) {
                graphics::Box_(star_draw_x - (box_size / 2), star_draw_y - (box_size / 2), box_size, box_size, 1, 10);
            }
        }
    }

    int16_t __cdecl Scan_Galaxy_Map_Fields_(int16_t field_id_1, int16_t field_id_2, int16_t* star_idx_ptr_1, int16_t* star_idx_ptr_2) {
        int16_t result = -1;
        *star_idx_ptr_1 = -1;
        *star_idx_ptr_2 = -1;

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            if (MOX::_galaxy_map_star_field[i] == field_id_2) {
                result = 0;
                *star_idx_ptr_2 = i;
                break;
            }
        }

        if (*star_idx_ptr_2 == -1) {
            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                if (MOX::_galaxy_map_star_field[i] == field_id_1 && MOX::_star[i].spectral_class != STAR_CLASS_BLACK_HOLE) {
                    result = 4;
                    *star_idx_ptr_1 = i;
                    break;
                }
            }
        }

        if (result == -1 && MOX::_galaxy_map_grid_field == field_id_2) {
            result = 2;
        }

        return result;
    }

    void __cdecl Check_Function_Keys_(int16_t key, int16_t* flag) {
        int16_t colony_id = -1;
        char msg_buffer[200];

        if (key == -1110 /* 0xfbaa */) {
            return;
        }

        if (key > -1109 && key < -1100 /* 0xfbb3 */) {
            MAINSCR::Toggle_Game_Option_(-1101 - key);
        }

        int32_t switch_idx = (int32_t)key + 1010;
        if (switch_idx > 9) {
            return;
        }

        switch (switch_idx) {
        case 0: /* DI = -1010 */
            if (MOX::_disallow_saves_code == 0) {
                FILEDEF::Save_Game_(9);
                strcpy(msg_buffer, HAROLD::H_Message_(211 /* HESTR_0D3_GAME_SAVED */));
                HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, msg_buffer, 0);
            } else {
                strcpy(msg_buffer, HAROLD::H_Message_(212 /* HESTR_0D4_SAVE_GAME_FUNCTION_DIS */));
                HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, msg_buffer, 0);
            }
            break;

        case 1: /* DI = -1009 */
            MOX::_current_screen = SCREEN_DISTANCE_BETWEEN_STARS;
            MOX::_return_screen = SCREEN_MAIN;
            *flag = 1;
            break;

        case 4: /* DI = -1006 (Next Star) */
            MAINSCR::Cycle_Star_Icons_(&MOX::_system_star_id_for_auto_select, MOX::_PLAYER_NUM, 0);
            if (MOX::_system_star_id_for_auto_select > -1 && MOX::_system_star_id_for_auto_select < MOX::_NUM_STARS) {
                if (MOX::_settings.auto_select_colony != 0) {
                    colony_id = HACCESS::Last_Colony_Selected_(MOX::_system_star_id_for_auto_select, MOX::_PLAYER_NUM);
                }
                if (colony_id > -1) {
                    MAINSCR::Do_Colony_Screen_(colony_id, flag);
                } else {
                    MOX::_moveable_box[0].id = MOX::_system_star_id_for_auto_select;
                    MAINSCR::Do_System_Popup_(MOX::_system_star_id_for_auto_select);
                }
            }
            break;

        case 5: /* DI = -1005 (Prev Star) */
            MAINSCR::Cycle_Star_Icons_(&MOX::_system_star_id_for_auto_select, MOX::_PLAYER_NUM, 1);
            if (MOX::_system_star_id_for_auto_select > -1 && MOX::_system_star_id_for_auto_select < MOX::_NUM_STARS) {
                if (MOX::_settings.auto_select_colony != 0) {
                    colony_id = HACCESS::Last_Colony_Selected_(MOX::_system_star_id_for_auto_select, MOX::_PLAYER_NUM);
                }
                if (colony_id > -1) {
                    MAINSCR::Do_Colony_Screen_(colony_id, flag);
                } else {
                    MOX::_moveable_box[0].id = MOX::_system_star_id_for_auto_select;
                    MAINSCR::Do_System_Popup_(MOX::_system_star_id_for_auto_select);
                }
            }
            break;

        case 6: /* DI = -1004 (Relocations) */
        {
            int8_t reloc_count = HACCESS::N_Star_Relocations_(MOX::_PLAYER_NUM);
            if (reloc_count > 1) {
                if (MAINSCR::_merging_relocations != 0) {
                    mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                    MAINSCR::_merging_relocations = 0;
                } else {
                    mouse::Set_Mouse_List_(&MOX::_mouse_list_relocation, 2);
                    MAINSCR::_merging_relocations = 1;
                }
            } else {
                int16_t msg_id = (reloc_count == 1) ? 209 /* 0xD1 */ : 210 /* 0xD2 */;
                strcpy(msg_buffer, HAROLD::H_Message_(msg_id));
                HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, msg_buffer, 0);
            }
            break;
        }

        case 8: /* DI = -1002 (Next Ship) */
        {
            int16_t ship_idx;
            MAINSCR::Cycle_Ship_Icons_(&MOX::_fleet_box_ship_stack, &ship_idx, 0);
            if (ship_idx > -1 && ship_idx < MOX::_ship_icon_count) {
                MAINSCR::Do_Fleet_Popup_(ship_idx, 0);
            }
            break;
        }

        case 9: /* DI = -1001 (Prev Ship) */
        {
            int16_t ship_idx;
            MAINSCR::Cycle_Ship_Icons_(&MOX::_fleet_box_ship_stack, &ship_idx, 1);
            if (ship_idx > -1 && ship_idx < MOX::_ship_icon_count) {
                MAINSCR::Do_Fleet_Popup_(ship_idx, 0);
            }
            break;
        }

        default:
            break;
        }
    }

    int32_t __cdecl Zoom_Box_Width_(int16_t width, int16_t zoom_factor) {
        return (width * 506 /* 0x1fa */) / zoom_factor;
    }

    int32_t __cdecl Zoom_Box_Height_(int16_t height, int16_t divisor) {
        return (height * 400) / divisor;
    }

    void __cdecl Draw_Main_Screen_For_Zooming_() {
        s_colors color_table;
        uint32_t saved_seed;
        int16_t width;
        int16_t height;

        saved_seed = game_random::Get_Random_Seed_();

        for (int16_t i = 0; i < 8; ++i) {
            if (i < 2 || i > 5) {
                color_table.colors[i] = 0x0C;
            }
            else if (i == 2 || i == 5) {
                color_table.colors[i] = 0x10;
            }
            else {
                color_table.colors[i] = 0x13;
            }
        }

        if (mouse::Pointer_X_() <= 22) {
            _zoom_box_x = 22;
        }
        else {
            _zoom_box_x = mouse::Pointer_X_();
        }

        if (mouse::Pointer_Y_() <= 22) {
            _zoom_box_y = 22;
        }
        else {
            _zoom_box_y = mouse::Pointer_Y_();
        }

        width = (int16_t)Zoom_Box_Width_(HAROLD::Get_Map_Scale_(MOX::_zoom_count), MOX::_cur_map_scale);
        height = (int16_t)Zoom_Box_Height_(HAROLD::Get_Map_Scale_(MOX::_zoom_count), MOX::_cur_map_scale);

        if (width + _zoom_box_x >= 536) {
            _zoom_box_x = 526 - width;
        }

        if (height + _zoom_box_y >= 420) {
            _zoom_box_y = 420 - height;
        }

        graphics::Set_Window_(_zoom_box_x, _zoom_box_y, _zoom_box_x + width, _zoom_box_y + height);
        video::Clip_On_();
        animate::Draw_(22, 22, (s_animation_header*)_temp_bitmap);

        HAROLD::Draw_Multi_Colored_Box_(
            _zoom_box_x,
            _zoom_box_y,
            _zoom_box_x + width,
            _zoom_box_y + height,
            &color_table,
            static_cast<uint16_t>(_zoom_box_color_increment)
        );

        _zoom_box_counter++;
        if ((_zoom_box_counter % 3) == 0) {
            _zoom_box_color_increment += _zoom_box_increment_value;
        }

        if ((_zoom_box_counter % 12) == 0) {
            _zoom_box_counter = 0;
            _zoom_box_increment_value = 0xFF; /* -1 */
        }

        video::Clip_Off_();
        graphics::Reset_Window_();
        game_random::Set_Random_Seed_(saved_seed);
    }

    void __cdecl Toggle_Game_Option_(int16_t option_idx) {
        if (option_idx < 0 || option_idx > 7 || option_idx == 3) {
            return;
        }

        char option_name[32];
        char message[100];
        int8_t status_flag;

        switch (option_idx) {
            case 0:
                MOX::_settings.end_of_turn_summary = (MOX::_settings.end_of_turn_summary == 0);
                strcpy(option_name, HAROLD::H_Message_(0xa6));
                status_flag = MOX::_settings.end_of_turn_summary;
                break;

            case 1:
                MOX::_settings.end_of_turn_wait = (MOX::_settings.end_of_turn_wait == 0);
                strcpy(option_name, HAROLD::H_Message_(0xa7));
                status_flag = MOX::_settings.end_of_turn_wait;
                break;

            case 2:
                MOX::_settings.enemy_moves = (MOX::_settings.enemy_moves == 0);
                strcpy(option_name, HAROLD::H_Message_(0xa8));
                status_flag = MOX::_settings.enemy_moves;
                break;

            case 4:
                MOX::_settings.auto_select_ships = (MOX::_settings.auto_select_ships == 0);
                strcpy(option_name, HAROLD::H_Message_(0xaa));
                status_flag = MOX::_settings.auto_select_ships;
                break;

            case 5:
                MOX::_settings.animations_on = (MOX::_settings.animations_on == 0);
                strcpy(option_name, HAROLD::H_Message_(0xab));
                status_flag = MOX::_settings.animations_on;
                break;

            case 6:
                MOX::_settings.auto_select_colony = (MOX::_settings.auto_select_colony == 0);
                strcpy(option_name, HAROLD::H_Message_(0xac));
                status_flag = MOX::_settings.auto_select_colony;
                if (status_flag != 0) {
                    HACCESS::Set_All_Colony_Auto_Select_Flags_();
                }
                break;

            case 7:
                MOX::_settings.show_relocation_lines = (MOX::_settings.show_relocation_lines == 0);
                strcpy(option_name, HAROLD::H_Message_(0xad));
                status_flag = MOX::_settings.show_relocation_lines;
                break;

            default:
                return;
        }

        const char* status_text = HAROLD::H_Message_(status_flag ? 0xe7 /* ON */ : 0xe8 /* OFF */);

        sprintf(message, "%s %s", option_name, status_text);

        LOADSAVE::Save_Session_Related_Settings_();
        HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, message, 0);
    }

    int16_t __cdecl Scan_Fleet_Movement_Box_Fields_(int16_t field_id_a, int16_t field_id_b, int16_t* out_chosen_offset, int16_t* out_scanned_icon_idx) {
        int16_t result = -1;
        bool found = false;
        int16_t counter = 0;

        *out_scanned_icon_idx = -1;
        *out_chosen_offset = -1;

        int limit = static_cast<int>(MOX::_fleet_icon_buttons_added) + static_cast<int>(MOX::_first_fleet_movement_icon);
        if (static_cast<int>(MOX::_fleet_box.count) < limit) {
            limit = static_cast<int>(MOX::_fleet_box.count);
        }

        int16_t i = MOX::_first_fleet_movement_icon;
        while (i < static_cast<int16_t>(limit) && !found) {
            if (FLEETPOP::Can_Select_Fleet_Box_Ship_(i)) {
                if (MOX::_fleet_movement_icon[i].field == field_id_b) {
                    found = true;
                    result = 0;
                    *out_chosen_offset = counter;
                    *out_scanned_icon_idx = i;
                }
                else if (MOX::_fleet_movement_icon[i].field + field_id_b == 0) {
                    *out_chosen_offset = counter;
                    found = true;
                    result = 1;
                }
            }
            counter++;
            i++;
        }

        if (result == -1) {
            // Redundantly recalculate limit as per assembly
            int limit2 = static_cast<int>(MOX::_fleet_icon_buttons_added) + static_cast<int>(MOX::_first_fleet_movement_icon);
            if (static_cast<int>(MOX::_fleet_box.count) < limit2) {
                limit2 = static_cast<int>(MOX::_fleet_box.count);
            }

            for (int16_t j = MOX::_first_fleet_movement_icon; j < static_cast<int16_t>(limit2); ++j) {
                if (MOX::_fleet_movement_icon[j].field == field_id_a) {
                    *out_scanned_icon_idx = j;
                    return 4;
                }
            }
        }

        return result;
    }

    void __cdecl Scan_Moveable_Box_Fields(int16_t param_1, int16_t param_2, int32_t param_3, int16_t* param_4) {
        for (int16_t box_idx = 0; box_idx < 5; ++box_idx) {
            if (!MOVEBOX::Moveable_Box_Selected_(box_idx)) {
                continue;
            }

            if (MOX::_moveable_box[box_idx].close_btn == param_2) {
                if (param_1 == 0) {
                    if (box_idx == 0) {
                        video::Set_Page_Off_();
                        Draw_Main_Screen_();
                        RUSS::Mox_Sync_Update_();
                        MOVEBOX::Vanish_System_Display_Box_();
                    } else if (box_idx == 2) {
                        HACCESS::Clear_Fleet_Box_Selected_Ship_Status_();
                        FLEETPOP::Clear_Ship_Node_Selection_Flags_();
                        MOX::_draw_main_screen_to_back = 1;
                        MOVEBOX::Vanish_Fleet_Movement_Box_();
                    } else {
                        MOX::_moveable_box[box_idx].id = MOX::_box_off_flag[box_idx];
                    }
                    continue;
                }
            } else {
                if (param_1 == 0 && box_idx != MOX::_moveable_box_order[0]) {
                    if (MOVEBOX::Box_Field_Selected_(box_idx, param_2)) {
                        MOVEBOX::Move_Box_To_Top_(MOX::_moveable_box[box_idx].index);
                    }
                }
            }

            if (MOX::_moveable_box[box_idx].index == 1) {
                int16_t scanned_star_id;
                int16_t center_star_id;
                int16_t scan_res = Scan_Galaxy_Map_Fields_((int16_t)param_3, param_2, &scanned_star_id, &center_star_id);
                MOX::_galaxy_map_scanned_star = scanned_star_id;

                if (HAROLD::Map_Scale_To_Zoom_Level_() != MOX::_max_zoom_count) {
                    if (scan_res == 2) {
                        int16_t map_x = (int16_t)(((((int32_t)MOX::_galaxy_map_grid_y * MOX::_max_map_scale) / 10) * (int16_t)(5060 / MOX::_galaxy_map_height)) / 10);
                        int16_t map_y = (int16_t)(((((int32_t)MOX::_galaxy_map_grid_x * MOX::_max_map_scale) / 10) * (int16_t)(4000 / MOX::_galaxy_map_width)) / 10);
                        Center_Map_(map_x, map_y);
                    } else if (scan_res == 0) {
                        int16_t map_x = MOX::_star[center_star_id].x;
                        int16_t map_y = MOX::_star[center_star_id].y;
                        Center_Map_(map_x, map_y);
                    }
                }
            }

            if (MOVEBOX::Moveable_Box_Selected_(2) && MOX::_moveable_box[box_idx].index == 2) {
                if (param_2 == MOX::_fleet_box_scroll_up_button) {
                    SCROLL::Scroll_Up_One_Item_(&MOX::_fltpop_scroll_bar);
                } else if (param_2 == MOX::_fleet_box_scroll_dn_button) {
                    SCROLL::Scroll_Down_One_Item_(&MOX::_fltpop_scroll_bar);
                }

                if (param_2 == MOX::_fleet_movement_all_button) {
                    if (MOX::_fleet_movement_icons_on == 1 || FLEETPOP::Fleet_Buttons_Chosen_() == MOX::_fleet_box.count) {
                        FLEETPOP::Set_Fleet_Movement_Icon_Selections_(0, 0);
                        HACCESS::Clear_Fleet_Box_Selected_Ship_Status_();
                        MOX::_eta_star_id = -1;
                    } else {
                        FLEETPOP::Set_Fleet_Movement_Icon_Selections_(1, 0);
                    }
                    continue;
                }

                int16_t local_5e2;
                int16_t scan_res = Scan_Fleet_Movement_Box_Fields_(param_3, param_2, &_fleet_icon_chosen, &local_5e2);
                MOX::_fleet_ship_scanned = -1;
                if (local_5e2 > -1) {
                    MOX::_eta_star_id = -1;
                    MOX::_fleet_ship_scanned = local_5e2;
                }

                if (scan_res == 1) {
                    fields::Deactivate_Help_List_();
                    int16_t ship_id = HACCESS::Fleet_Movement_Ship_Id_(_fleet_icon_chosen + MOX::_first_fleet_movement_icon);
                    CMBTDRW1::Detailed_View_Ship_(ship_id);
                    EVANHELP::Set_Main_Screen_Help_List_();
                    MOX::_draw_main_screen_to_back = 1;
                }

                if (MOX::_fleet_box.colonize_btn_active != 0 && MOX::_fleet_box_colonize_button == param_2) {
                    MOX::_eta_star_id = -1;
                    MOX::_current_screen = SCREEN_COLONIZATION_IN_MAIN;
                    MOX::_return_screen = SCREEN_MAIN;
                    *param_4 = 1;
                    continue;
                }

                if (MOX::_fleet_box.transport_btn_active != 0 && MOX::_fleet_box_transport_button == param_2) {
                    if (FLEETPOP::At_Least_One_Ship_Of_Type_Selected_(SHIP_TYPE_TRANSPORT) != 0) {
                        MOX::_current_screen = SCREEN_UNLOADING_TRANSPORTS_IN_MAIN;
                        MOX::_return_screen = SCREEN_MAIN;
                        *param_4 = 1;
                    } else {
                        char local_578[200];
                        strcpy(local_578, HAROLD::H_Message_(0x20 /* HESTR_020_OPTION_DISABLED_DURING */));
                        HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, local_578, 0);
                    }
                    continue;
                }

                if (MOX::_fleet_box.outpost_btn_active != 0 && MOX::_fleet_box_outpost_button == param_2) {
                    MOX::_current_screen = SCREEN_BUILDING_OUTPOSTS_IN_MAIN;
                    MOX::_return_screen = SCREEN_MAIN;
                    *param_4 = 1;
                    continue;
                }

                if (MOX::_fleet_box.engage_btn_active != 0 && MOX::_fleet_box_engage_button == param_2) {
                    int16_t star_id = HACCESS::Ship_Stack_Star_Id_(MOX::_ship_stack_start[MOX::_fleet_box_ship_stack]);
                    if (star_id > -1 && star_id < MOX::_NUM_STARS) {
                        MOX::_star[star_id].ignore_combat_ships &= ~(1 << MOX::_PLAYER_NUM);
                    }
                    continue;
                }

                if (MOX::_fleet_box.attack_btn_active != 0 && MOX::_fleet_box_attack_button == param_2 && MOX::_fleet_box_attack_button > 0) {
                    if (!FLEETPOP::Some_Icon_Selected_()) {
                        char local_4b0[200];
                        strcpy(local_4b0, HAROLD::H_Message_(0xE6 /* HESTR_0E6_NO_SHIPS_SELECTED */));
                        HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, local_4b0, 3);
                    } else {
                        int16_t local_3e8[MAX_SHIPS];
                        int16_t count = HACCESS::Get_Fleet_Box_Selected_Ship_Ids_(local_3e8, MAX_SHIPS, 255);
                        int16_t ship_id = HACCESS::Fleet_Movement_Ship_Id_(0);
                        
                        int16_t msg_idx = (count == 1) ? 0xE4 : 0xE5;
                        char local_5dc[100];
                        sprintf(local_5dc, HAROLD::H_Message_(msg_idx), count);
                        
                        if (HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, local_5dc, 1) != 0) {
                            int16_t num_deployed = SHIPS::Deploy_Ships_To_Attack_Antares_();
                            MOX::_first_node_ptr_in_stack_ = -1;
                            Update_Ship_Icons_In_Between_Turns_();
                            FLEETPOP::Set_Fleet_Movement_Icon_Selections_(0, 0);
                            HACCESS::Clear_Fleet_Box_Selected_Ship_Status_();
                            
                            if (MOX::_fleet_box.count == num_deployed) {
                                MOX::_fleet_box_ship_stack = HACCESS::Ship_Stack_From_Ship_Id_(ship_id);
                                if (MOX::_fleet_box_ship_stack == -1) {
                                    MOVEBOX::Vanish_Fleet_Movement_Box_();
                                }
                            }
                        }
                    }
                    continue;
                }
            }
        }
    }

    void __cdecl Clear_Main_Screen_Fields_() {
        _zoom_in_button = -1000;
        _zoom_out_button = -1000;
        _game_button = -1000;
        _races_button = -1000;
        _fleets_button = -1000;
        _colonies_button = -1000;
        _design_button = -1000;
        _info_button = -1000;
        _next_button = -1000;
        MOX::_system_display_grid_field = -1000;

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            MOX::_star_button[i] = -1000;
        }

        for (int16_t i = 0; i < 5; ++i) {
            MOX::_planet_buttons[i] = -1000;
        }

        for (int16_t i = 0; i < 8; ++i) {
            MOX::_system_display_ships[i].button = -1000;
        }

        MOX::_main_screen_grid_field = -1000;
        _debug_field = -1000;
        MOX::_galaxy_map_grid_field = -1000;
        MOX::_fleet_movement_all_button = -1000;
        MOX::_fleet_box_colonize_button = -1000;
        MOX::_fleet_box_outpost_button = -1000;
        MOX::_fleet_box_transport_button = -1000;
        MOX::_fleet_box_engage_button = -1000;
        MOX::_fleet_box_attack_button = -1000;
        _all_tech_cheat = -1000;
        _leader_cheat = -1000;
        _more_money_cheat = -1000;
        _get_tech_cheat = -1000;
        _omniscience_cheat = -1000;
        _all_ai_cheat = -1000;
    }

    void __cdecl Cycle_Ship_Icons_(int16_t* current_stack_idx_ptr, int16_t* out_icon_idx, int16_t direction) {
        int16_t target_player_num;
        int16_t result_stack_idx = -1;
        int16_t result_icon_idx = -1;
        int16_t start_node_idx = -1;
        bool found_stack = false;
        bool found_icon = false;

        if (MOX::_fleet_box_ship_stack > 0 && MOX::_fleet_box_ship_stack < MAX_SHIPS && MOVEBOX::Moveable_Box_Selected_(2)) {
            target_player_num = HACCESS::Ship_Stack_Owner_(MOX::_ship_stack_start[MOX::_fleet_box_ship_stack]);
        }
        else {
            target_player_num = MOX::_PLAYER_NUM;
        }

        if (MOX::_ship_icon_count > 0 && *current_stack_idx_ptr != -1) {
            int16_t stack_idx = *current_stack_idx_ptr;

            // Search from current position
            if (direction == 0 /* Forward */) {
                stack_idx++;
                while (stack_idx < MOX::_ship_stack_count) {
                    start_node_idx = MOX::_ship_stack_start[stack_idx];
                    int16_t ship_idx = MOX::_ship_node[start_node_idx].ship_idx;
                    if (HACCESS::Ship_Owner_(ship_idx) == target_player_num && MOX::_ship[ship_idx].status < 3) {
                        found_stack = true;
                        result_stack_idx = stack_idx;
                        break;
                    }
                    stack_idx++;
                }
            }
            else /* Backward */ {
                stack_idx--;
                while (stack_idx > -1) {
                    start_node_idx = MOX::_ship_stack_start[stack_idx];
                    int16_t ship_idx = MOX::_ship_node[start_node_idx].ship_idx;
                    if (HACCESS::Ship_Owner_(ship_idx) == target_player_num && MOX::_ship[ship_idx].status < 3) {
                        found_stack = true;
                        result_stack_idx = stack_idx;
                        break;
                    }
                    stack_idx--;
                }
            }

            // Wrap around if not found
            if (!found_stack) {
                if (direction == 0 /* Forward */) {
                    stack_idx = 0;
                    while (stack_idx < MOX::_ship_stack_count) {
                        start_node_idx = MOX::_ship_stack_start[stack_idx];
                        int16_t ship_idx = MOX::_ship_node[start_node_idx].ship_idx;
                        if (HACCESS::Ship_Owner_(ship_idx) == target_player_num && MOX::_ship[ship_idx].status < 3) {
                            found_stack = true;
                            result_stack_idx = stack_idx;
                            break;
                        }
                        stack_idx++;
                    }
                }
                else /* Backward */ {
                    stack_idx = MOX::_ship_stack_count - 1;
                    while (stack_idx > (*current_stack_idx_ptr - 1)) {
                        start_node_idx = MOX::_ship_stack_start[stack_idx];
                        int16_t ship_idx = MOX::_ship_node[start_node_idx].ship_idx;
                        if (HACCESS::Ship_Owner_(ship_idx) == target_player_num && MOX::_ship[ship_idx].status < 3) {
                            found_stack = true;
                            result_stack_idx = stack_idx;
                            break;
                        }
                        stack_idx--;
                    }
                }
            }
        }

        if (start_node_idx != -1 && MOX::_ship_icon_count > 0) {
            for (int16_t i = 0; i < MOX::_ship_icon_count; ++i) {
                if (MOX::_ship_icon[i].node_idx == start_node_idx) {
                    found_icon = true;
                    result_icon_idx = i;
                    break;
                }
            }
        }

        if (found_icon) {
            *out_icon_idx = result_icon_idx;
            *current_stack_idx_ptr = result_stack_idx;
        }
        else {
            *out_icon_idx = -1;
            *current_stack_idx_ptr = -1;
        }
    }

    void __cdecl Draw_Exploding_Plasma_Box_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4, int16_t param_5, int16_t param_6, int16_t param_7)
    {
        uint32_t random_seed = game_random::Get_Random_Seed_();
        MOX::_animated_star = 0xFFFF;

        if (param_7 == 2) {
            int16_t node = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
            int16_t n_icons = FLEETPOP::N_Fleet_Icons_(node);
            FLEETPOP::Fleet_Box_Data_(n_icons);
            param_5 = MOX::_fleet_box.width;
            param_6 = MOX::_fleet_box.height;
        }

        int16_t saved_window;
        MISC::Save_Window_Status_(&saved_window);
        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();

        if (animate::_clipping_flag == 1) {
            if (param_3 < video::_screen_window_x1) {
                param_3 = video::_screen_window_x1;
            }
            if (param_4 < video::_screen_window_y1) {
                param_4 = video::_screen_window_y1;
            }
            if (video::_screen_window_x2 < param_5 + param_3) {
                param_5 = video::_screen_window_x2 - param_3;
            }
            if (video::_screen_window_y2 < param_6 + param_4) {
                param_6 = video::_screen_window_y2 - param_4;
            }
        }

        int16_t original_x2 = param_3;
        int16_t original_y2 = param_4;

        fields::Disable_Draw_All_Fields_();
        param_5 -= 3;
        param_6 -= 3;
        MOVEBOX::N_Moveable_Boxes_Selected_(param_7);
        video::Set_Page_Off_();
        Draw_Main_Screen_Filled_(1);
        graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
        video::Clip_On_();
        Draw_Main_Main_Screen_();
        video::Copy_Off_To_Back_();

        uint8_t plasma_color_map[8];
        for (int i = 0; i < 8; ++i) {
            plasma_color_map[i] = static_cast<uint8_t>((i * 2) + 14);
        }

        int16_t unk_color = (int16_t)game_random::Random_(8) - 1;
        int16_t color_anim = 0;

        int16_t dest_x = original_x2;
        int16_t dest_y = original_y2;

        if (param_1 < dest_x) {
            if (param_2 <= dest_y) {
                dest_x += 3;
                dest_y += 3;
            } else {
                if (dest_y + param_6 <= 0x1a5) {
                    dest_y += param_6;
                }
                dest_y -= 3;
            }
        } else {
            if (param_2 <= dest_y) {
                if (dest_x + param_5 <= 0x20f) {
                    dest_x += param_5;
                }
                dest_x -= 3;
            } else {
                if (dest_x + param_5 <= 0x20f) {
                    dest_x += param_5;
                }
                if (dest_y + param_6 <= 0x1cb) {
                    dest_y += param_6;
                }
                dest_x -= 3;
                dest_y -= 3;
            }
        }

        int16_t step_x = (dest_x - param_1) / 10;
        int16_t step_y = (dest_y - param_2) / 10;
        int16_t accum_x = 0;
        int16_t accum_y = 0;

        for (int i = 0; i < 10; ++i) {
            int16_t w_offset;
            int16_t h_offset;

            if (original_x2 <= param_1) {
                if (param_2 <= original_y2) {
                    w_offset = (param_5 * i) / -10;
                    h_offset = (param_6 * i) / 10;
                } else {
                    w_offset = (param_5 * i) / -10;
                    h_offset = (param_6 * i) / -10;
                }
            } else {
                if (param_2 <= original_y2) {
                    w_offset = (param_5 * i) / 10;
                    h_offset = (param_6 * i) / 10;
                } else {
                    w_offset = (param_5 * i) / 10;
                    h_offset = (param_6 * i) / -10;
                }
            }

            int16_t box_x = param_1 + accum_x + w_offset;
            int16_t box_y = param_2 + accum_y + h_offset;

            if (box_x < 0x15) {
                box_x = 0x16;
            }
            if (box_x > 0x20f) {
                box_x = 0x20e;
            }

            if (box_y < 0x15) {
                box_y = 0x16;
            }
            if (box_y > 0x1a5) {
                box_y = 0x1a4;
            }

            int16_t draw_x = box_x - w_offset;
            int16_t draw_y = box_y - h_offset;

            if (i > 0) {
                video::Set_Page_Off_();
            }

            line::Plasma_Box_(draw_x, draw_y, (uint16_t)w_offset, h_offset, 3, color_anim, (uint8_t)unk_color, plasma_color_map);

            color_anim = (game_random::Random_(10) + color_anim) & 0xFF;
            RUSS::Mox_Sync_Update_();

            accum_y += step_y;
            accum_x += step_x;
        }

        if (animate::_clipping_flag == 1) {
            if (original_x2 < video::_screen_window_x1) {
                original_x2 = video::_screen_window_x1;
            }
            if (original_y2 < video::_screen_window_y1) {
                original_y2 = video::_screen_window_y1;
            }
            if (video::_screen_window_x2 < original_x2 + param_5) {
                param_5 = video::_screen_window_x2 - original_x2;
            }
            if (video::_screen_window_y2 < original_y2 + param_6) {
                param_6 = video::_screen_window_y2 - original_y2;
            }
        }

        line::Plasma_Box_(original_x2, original_y2, (uint16_t)param_5, param_6, 3, color_anim, (uint8_t)unk_color, plasma_color_map);
        RUSS::Mox_Sync_Update_();
        fields::Enable_Draw_All_Fields_();
        MOX::_draw_main_screen_to_back = 1;
        game_random::Set_Random_Seed_(random_seed);
        MISC::Restore_Window_Status_(saved_window);
    }

    void __cdecl Get_Star_Name_Info_(int16_t star_idx, int16_t* out_x, int16_t* out_y, int16_t* out_width) {
        int16_t saved_map_scale;
        int16_t zoom_star_size;
        int16_t string_width;
        int8_t is_visited;
        int16_t star_draw_x;
        int16_t star_draw_y;
        s_animation_header* unused_output = nullptr;
        char name_buffer[40];

        saved_map_scale = MOX::_cur_map_scale;
        MOX::_cur_map_scale = MOX::_max_map_scale;

        HAROLD::Map_Scale_To_Zoom_Level_();
        zoom_star_size = HAROLD::Map_Scale_Star_Size_To_Zoom_Level_(star_idx);

        HAROLD::Set_Player_Font_Style_(0, false);
        fonts::Set_Font_Spacing_(1);

        MAINSCR::Get_Star_Name_(star_idx, 0, &is_visited, name_buffer, &unused_output);
        MAINSCR::Get_Star_Draw_Coords_(star_idx, &star_draw_x, &star_draw_y);

        string_width = fonts::Get_String_Width_(name_buffer);
        *out_width = string_width;

        // Calculate center-aligned X position
        *out_x = (star_draw_x - (string_width / 2)) + 1;

        // Calculate Y position relative to star size and zoom
        *out_y = (zoom_star_size / 2 - (int16_t)MOX::_star[star_idx].size) + star_draw_y;

        MOX::_cur_map_scale = saved_map_scale;
    }

    void __cdecl Main_Receive_Message_(uint32_t from_player_id, int16_t param_2, uint16_t msg_type, uint32_t len, void* data) {
        switch (msg_type) {
        case NET_PACKET_START_GAME: {
            s_net_pack_start_game* pack = (s_net_pack_start_game*)data;
            MOX::_NUM_PLAYERS = pack->number_of_players;
            MOX::_settings.random_events = pack->random_events;
            MOX::_settings.game_difficulty = pack->game_difficulty;
            MOX::_settings.number_of_players = pack->number_of_players;
            MOX::_settings.galaxy_size = pack->galaxy_size;
            MOX::_settings.galaxy_age = pack->galaxy_age;
            MOX::_settings.strategic_combat_flag = pack->strategic_combat_flag;
            MOX::_settings.starting_civilization_level = pack->starting_civilization_level;
            MOX::_settings.xenons_exist = pack->xenons_exist;

            if (strcmp(pack->version_string, GAME_BUILD_DATE) != 0) {
                char error_msg[256];
                sprintf(error_msg, NETMOX::_incompatible_versions_error, pack->version_string, GAME_BUILD_DATE);
                exitm::Exit_With_Message(error_msg);
            }

            NETMOX::_net.host_election_id = (int16_t)pack->net_host_id;
            NETMOX::_net.num_connected_players = (int8_t)pack->num_connected_players;
            MOX::_PLAYER_NUM = -1;

            if (MOX::_NUM_PLAYERS > 0) {
                for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                    MOX::_player[i].network_player_id = pack->net_ids[i];
                    if (NETMOX::_net.net_host_id == pack->net_ids[i]) {
                        MOX::_PLAYER_NUM = i;
                    }
                }
            }

            if (MOX::_PLAYER_NUM != -1) {
                NETMOX::_net.sync_state = NET_SYNC_STATE_CLIENT_SELECT_RACE;
            }
            else {
                NETMOX::_net.sync_state = NET_SYNC_STATE_CLIENT_NO_AVAILABLE_SLOT;
            }
            break;
        }
        case NET_PACKET_PLAYER_SETUP: {
            NETMOX::_net.player_connected_flags[from_player_id] = 1;
            memcpy(&MOX::_player[from_player_id], data, (uint16_t)len);

            if (MOX::_NUM_PLAYERS > 0) {
                for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                    if (from_player_id != i && MOX::_player[i].color == MOX::_player[from_player_id].color) {
                        MOX::_player[from_player_id].color = 8;
                    }
                }
            }
            break;
        }
        case NET_PACKET_GAME_DATA_CHUNK_COUNT:
        case NET_PACKET_GAME_ORDERS_CHUNK_COUNT:
        case NET_PACKET_PICK_TARGET_INFO_CHUNK_COUNT:
        case NET_PACKET_BATTLE_RESULTS_CHUNK_COUNT:
        case NET_PACKET_YOU_ARE_TARGET_INFO_CHUNK_COUNT: {
            const s_chunk_count_packet* packet = (const s_chunk_count_packet*)data;
            NETMOX::_net.total_chunk_count = packet->total_chunk_count;
            NETMOX::_net.received_chunk_count = 0;
            NETMOX::_net.total_bytes_received = 0;
            allocate::Push_Block_(MOX::_screen_seg);
            NETMOX::_net.network_data_ptr = allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xE1000);

            if (msg_type == NET_PACKET_GAME_DATA_CHUNK_COUNT) {
                NETMOX::_net.host_election_winner_id = param_2;
                NETTURN::Net_Init_Host_Election_();
            }
            else if (msg_type == NET_PACKET_PICK_TARGET_INFO_CHUNK_COUNT) {
                NETMOX::_net.sync_state = NET_SYNC_STATE_RECEIVING_PICK_TARGET_INFO;
            }
            else if (msg_type == NET_PACKET_BATTLE_RESULTS_CHUNK_COUNT) {
                NETMOX::_net.sync_state = NET_SYNC_STATE_RECEIVING_BATTLE_RESULTS_CHUNKS;
            }
            else if (msg_type == NET_PACKET_YOU_ARE_TARGET_INFO_CHUNK_COUNT) {
                NETMOX::_net.sync_state = NET_SYNC_STATE_RECEIVING_YOU_ARE_TARGET_INFO;
            }
            break;
        }
        case NET_PACKET_GAME_DATA_CHUNK:
        case NET_PACKET_PICK_TARGET_INFO_CHUNK:
        case NET_PACKET_BATTLE_RESULTS_CHUNK:
        case NET_PACKET_YOU_ARE_TARGET_INFO_CHUNK:
        case NET_PACKET_GAME_ORDERS_CHUNK: {
            uint8_t* dest = (uint8_t*)NETMOX::_net.network_data_ptr + (NETMOX::_net.received_chunk_count * 0x7800);
            memcpy(dest, data, len & 0xFFFF);

            NETMOX::_net.received_chunk_count++;
            NETMOX::_net.total_bytes_received += (len & 0xFFFF);

            if (msg_type == NET_PACKET_GAME_ORDERS_CHUNK && NETMOX::_net.total_chunk_count == NETMOX::_net.received_chunk_count) {
                NETMOX::_net.host_sync_counter++;
                if ((uint8_t)NETMOX::_net.host_sync_counter == MOX::_NUM_PLAYERS) {
                    NETMOX::_net.sync_state = NET_SYNC_STATE_HOST_BEGIN_TURN;
                }
                else {
                    NETMOX::_net.sync_state = NET_SYNC_STATE_HOST_WAITING_FOR_ORDERS;
                }
            }
            break;
        }
        case NET_PACKET_HOST_ELECTION_RANK: {
            int8_t rank = *(int8_t*)data;
            int8_t hit_next = *(int8_t*)((uint8_t*)data + 4);
            NETMOX::_net.host_election_player_rank[from_player_id] = rank;
            NETMOX::_net.players_hit_next_turn[from_player_id] = hit_next;

            int8_t cur_rank = NETMOX::_net.host_election_player_rank[MOX::_PLAYER_NUM];
            if (cur_rank == -1 || rank < cur_rank) {
                NETMOX::_net.host_election_player_rank[MOX::_PLAYER_NUM] = rank;
            }
            break;
        }
        case NET_PACKET_AI_DIPLOMACY_REQUEST: {
            int8_t remote_id = *(int8_t*)data;
            NETMOX::_net.player_net_info[from_player_id].diplomacy_target_id = remote_id;
            NETMOX::_net.player_net_info[from_player_id].player_state = NET_PLAYER_STATE_AI_DIPLOMACY_REQUESTED;

            int next_talker = NETMOX::Get_Next_AI_Talker_(remote_id);
            if (next_talker == -1) {
                NETMOX::_net.ai_diplomacy_done_mask |= (1 << (from_player_id & 0x1F));
            }
            else {
                NETMOX::_net.ai_diplomacy_active_mask |= (1 << (from_player_id & 0x1F));
                NETMOX::_net.player_net_info[from_player_id].diplomacy_target_id = remote_id;
                NETMOX::_net.player_net_info[from_player_id].player_state = NET_PLAYER_STATE_AI_DIPLOMACY_ACTIVE;
            }
            break;
        }
        case NET_PACKET_AI_DIPLOMACY_BEGIN: {
            if (NETMOX::_net.sync_state == NET_SYNC_STATE_AI_DIPLOMACY_REQUESTED || NETMOX::_net.sync_state == NET_SYNC_STATE_AI_DIPLOMACY_DONE) {
                NETMOX::_net.sync_state = NET_SYNC_STATE_AI_DIPLOMACY_READY;
            }
            break;
        }
        case NET_PACKET_AI_DIPLOMACY_DONE: {
            if (NETMOX::_net.sync_state == NET_SYNC_STATE_AI_DIPLOMACY_REQUESTED) {
                NETMOX::_net.sync_state = NET_SYNC_STATE_AI_DIPLOMACY_DONE;
            }
            break;
        }
        case NET_PACKET_AI_DIPLOMACY_CANCEL_REQUEST: {
            if (NETMOX::_net.player_net_info[from_player_id].player_state != NET_PLAYER_STATE_AI_DIPLOMACY_REQUESTED) {
                NETMOX::_net.ai_diplomacy_active_mask &= ~(1 << (from_player_id & 0x1F));
                int next_talker = NETMOX::Get_Next_AI_Talker_(NETMOX::_net.player_net_info[from_player_id].diplomacy_target_id);
                if (next_talker != -1) {
                    NETMOX::_net.ai_diplomacy_active_mask |= (1 << (next_talker & 0x1F));
                    NETMOX::_net.player_net_info[next_talker].player_state = NET_PLAYER_STATE_AI_DIPLOMACY_ACTIVE;
                }
            }
            NETMOX::_net.player_net_info[from_player_id].diplomacy_target_id = -1;
            NETMOX::_net.player_net_info[from_player_id].player_state = NET_PLAYER_STATE_READY;
            NETMOX::_net.ai_diplomacy_cancel_mask |= (1 << (from_player_id & 0x1F));
            break;
        }
        case NET_PACKET_AI_DIPLOMACY_CANCELLED: {
            NETMOX::_net.diplomacy_target_id = -1;
            NETMOX::_net.sync_state = NET_SYNC_STATE_READY;
            break;
        }
        case NET_PACKET_AI_DIPLOMACY_END_STATE: {
            s_net_pack_end_ai_diplomacy* pack = (s_net_pack_end_ai_diplomacy*)data;
            int8_t remote_id = NETMOX::_net.player_net_info[from_player_id].diplomacy_target_id;

            memcpy(&MOX::_player[remote_id], &pack->target_player, sizeof(s_player));
            memcpy(&MOX::_player[from_player_id], &pack->player, sizeof(s_player));

            for (int i = 0; i < MOX::_NUM_STARS; ++i) {
                MOX::_star[i].surrender_to[remote_id] = pack->surrender_to[i];
            }

            NETMOX::_net.player_net_info[from_player_id].diplomacy_target_id = -1;
            NETMOX::_net.player_net_info[from_player_id].player_state = NET_PLAYER_STATE_READY;
            NETMOX::_net.players_sync_mask[remote_id] = 0xFF;
            NETMOX::_net.diplomacy_sync_mask[remote_id] = from_player_id;

            int next_talker = NETMOX::Get_Next_AI_Talker_(remote_id);
            if (next_talker != -1) {
                NETMOX::_net.ai_diplomacy_active_mask |= (1 << (next_talker & 0x1F));
                NETMOX::_net.player_net_info[next_talker].player_state = NET_PLAYER_STATE_AI_DIPLOMACY_ACTIVE;
            }
            break;
        }
        case NET_PACKET_TURN_END_REQUEST: {
            NETMOX::_net.player_is_active_flags[from_player_id] = 1;
            break;
        }
        case NET_PACKET_SEND_GAME_ORDERS_REQUEST: {
            NETMOX::_net.sync_state = NET_SYNC_STATE_RECEIVING_GAME_ORDERS;
            break;
        }
        case NET_PACKET_COMBAT_SYNC_RESET: {
            memset(NETMOX::_net.player_net_info, 0, 45 * sizeof(int32_t)); // 180 bytes
            NETMOX::_net.sync_state = NET_SYNC_STATE_COMBAT_READY;
            break;
        }
        case NET_PACKET_PICK_TARGET_INFO: {
            NETMOX::_net.sync_state = NET_SYNC_STATE_PICKING_COMBAT_TARGET;
            NETMOX::Decode_Pick_Target_Info_(data, len);
            break;
        }
        case NET_PACKET_COMBAT_TARGET_CHOSEN: {
            const s_case11_packet* packet = (const s_case11_packet*)data;
            memcpy(&NETMOX::_net.player_net_info[from_player_id].combat_defender_id, packet, 4);
            NETMOX::_net.player_net_info[from_player_id].combat_target_player_id = packet->sync_colony_id;
            NETMOX::_net.player_net_info[from_player_id].player_state = NET_PLAYER_STATE_COMBAT_TARGET_CHOSEN;
            break;
        }
        case NET_PACKET_YOU_ARE_TARGET_INFO: {
            NETMOX::Decode_You_Are_Target_Info_(data, len);
            break;
        }
        case NET_PACKET_COMBAT_EXECUTE: {
            NETMOX::_net.sync_state = NET_SYNC_STATE_COMBAT_TACTICAL_ATTACKER;
            break;
        }
        case NET_PACKET_COMBAT_RESOLUTION_BEGIN: {
            if (MOX::_settings.strategic_combat_flag != 0) {
                NETMOX::_net.sync_state = NET_SYNC_STATE_COMBAT_STRATEGIC_ATTACKER;
            }
            else if (NETMOX::_net.combat_target_player_id < MAX_PLAYERS && MOX::_player[NETMOX::_net.combat_target_player_id].objectives == 'd') {
                NETMOX::_net.sync_state = NET_SYNC_STATE_COMBAT_TACTICAL_ATTACKER;
            }
            else {
                NETMOX::_net.sync_state = NET_SYNC_STATE_COMBAT_TACTICAL_ATTACKER_VS_NONHUMAN;
            }
            break;
        }
        case NET_PACKET_BATTLE_RESULTS: {
            NETMOX::Decode_Battle_Results_(data, (uint16_t)len);
            break;
        }
        case NET_PACKET_STRATEGIC_BATTLE_RESULTS:
        case NET_PACKET_STRATEGIC_RESULTS_READY: {
            memcpy(NETMOX::_net.combat_results_data, data, sizeof(s_battle_results)); // 1742 bytes copied
            if (msg_type == NET_PACKET_STRATEGIC_BATTLE_RESULTS) {
                NETMOX::_net.sync_state = NET_SYNC_STATE_COMBAT_RESULTS_DISPLAY;
            }
            else {
                NETMOX::_net.sync_state = NET_SYNC_STATE_STRATEGIC_RESULTS_RECEIVED;
            }
            break;
        }
        case NET_PACKET_COMBAT_RESULTS_ACK: {
            NETMOX::_net.player_net_info[from_player_id].player_state = NET_PLAYER_STATE_COMBAT_READY;
            NETMOX::_net.player_net_info[from_player_id].combat_attacker_id = -1;
            NETMOX::_net.sync_phase_active_flag = 0;
            break;
        }
        case NET_PACKET_STRATEGIC_RESULTS_ACK: {
            NETMOX::_net.sync_state = NET_SYNC_STATE_COMBAT_READY;
            break;
        }
        case NET_PACKET_COMBAT_PHASE_COMPLETE: {
            NETMOX::_net.sync_state = NET_SYNC_STATE_WAITING_FOR_GAME_DATA;
            break;
        }
        case NET_PACKET_REQUEST_HUMAN_DIPLOMACY: {
            s_net_pack_req_human_diplomacy* pack = (s_net_pack_req_human_diplomacy*)data;
            if (NETMOX::_net.players_hit_next_turn[MOX::_PLAYER_NUM] == 0) {
                memcpy(&MOX::_player[from_player_id], &pack->player, sizeof(s_player));

                for (int i = 0; i < MOX::_NUM_STARS; ++i) {
                    MOX::_star[i].surrender_to[from_player_id] = pack->surrender_to[i];
                }

                if (NETMOX::_net.sync_state == NET_SYNC_STATE_HUMAN_DIPLOMACY_REQUESTED && NETMOX::_net.diplomacy_target_id == from_player_id) {
                    uint8_t mask = ~(1 << (from_player_id & 0x1F));
                    NETMOX::_net.sync_state = NET_SYNC_STATE_HUMAN_DIPLOMACY_CONNECTING;
                    NETMOX::_net.diplomacy_initiator_id = from_player_id;
                    NETMOX::_net.players_active_mask &= mask;
                    NETMOX::_net.players_ready_mask[0] &= mask;
                    NETMOX::_net.is_diplomacy_initiator = (MOX::_PLAYER_NUM < NETMOX::_net.diplomacy_target_id);
                }
                else if ((MOX::_player[MOX::_PLAYER_NUM].ignoring & (1 << (from_player_id & 0x1F))) != 0) {
                    NETMOX::_net.human_diplomacy_ignored_mask |= (1 << (from_player_id & 0x1F));
                }
                else {
                    uint8_t mask = (1 << (from_player_id & 0x1F));
                    NETMOX::_net.players_active_mask |= mask;
                    NETMOX::_net.players_ready_mask[0] |= mask;
                }
            }
            break;
        }
        case NET_PACKET_ACCEPT_HUMAN_DIPLOMACY: {
            memcpy(&MOX::_player[from_player_id], data, sizeof(s_player));

            if (NETMOX::_net.sync_state == NET_SYNC_STATE_HUMAN_DIPLOMACY_REQUESTED && NETMOX::_net.diplomacy_target_id == from_player_id) {
                uint8_t mask = ~(1 << (from_player_id & 0x1F));
                NETMOX::_net.sync_state = NET_SYNC_STATE_HUMAN_DIPLOMACY_CONNECTING;
                NETMOX::_net.diplomacy_initiator_id = from_player_id;
                NETMOX::_net.players_active_mask &= mask;
                NETMOX::_net.players_ready_mask[0] &= mask;
                NETMOX::_net.is_diplomacy_initiator = 1;
            }
            else if ((MOX::_player[MOX::_PLAYER_NUM].ignoring & (1 << (from_player_id & 0x1F))) != 0) {
                NETMOX::_net.human_diplomacy_ignored_mask |= (1 << (from_player_id & 0x1F));
            }
            else {
                uint8_t mask = (1 << (from_player_id & 0x1F));
                NETMOX::_net.players_active_mask |= mask;
                NETMOX::_net.players_ready_mask[0] |= mask;
            }
            break;
        }
        case NET_PACKET_HUMAN_DIPLOMACY_IGNORED: {
            NETMOX::_net.sync_state = NET_SYNC_STATE_HUMAN_DIPLOMACY_REJECTED;
            break;
        }
        case NET_PACKET_CANCEL_HUMAN_DIPLOMACY: {
            uint8_t mask = ~(1 << (from_player_id & 0x1F));
            NETMOX::_net.players_active_mask &= mask;
            NETMOX::_net.players_ready_mask[0] &= mask;

            if (NETMOX::_net.sync_state == NET_SYNC_STATE_HUMAN_DIPLOMACY_READY && NETMOX::_net.diplomacy_target_id == from_player_id) {
                NETMOX::_net.sync_state = NET_SYNC_STATE_READY;
                NETMOX::_net.diplomacy_target_id = -1;
                NETMOX::_net.diplomacy_active_flag = 0;
            }
            if (NETMOX::_net.sync_state == NET_SYNC_STATE_HUMAN_DIPLOMACY_CONNECTING && NETMOX::_net.diplomacy_target_id == from_player_id) {
                if (NETMOX::_net.diplomacy_active_flag == 0) {
                    NETMOX::_net.sync_state = NET_SYNC_STATE_HUMAN_DIPLOMACY_REQUESTED;
                }
                else {
                    NETMOX::_net.sync_state = NET_SYNC_STATE_READY;
                }
                NETMOX::_net.diplomacy_active_flag = 0;
                NETMOX::_net.diplomacy_initiator_id = -1;
            }
            break;
        }
        case NET_PACKET_PICK_YOUR_POSITION: {
            NETMOX::_net.sync_state = NET_SYNC_STATE_CLIENT_PICK_POSITION;
            NETMOX::_net.host_election_id = param_2;
            NETMOX::Decode_Pick_Your_Position_((s_net_pack_pick_your_position*)data, (uint16_t)len);
            break;
        }
        case NET_PACKET_PICK_YOUR_POSITION_RESULT: {
            int32_t target_id;
            memcpy(&target_id, data, sizeof(target_id));
            MOX::_player[target_id].network_player_id = param_2;
            NETMOX::_net.joining_net_id = -1;
            break;
        }
        case NET_PACKET_SYNC_PLAYER_STATE: {
            s_net_pack_end_ai_diplomacy* pack = (s_net_pack_end_ai_diplomacy*)data;
            int target_id = pack->target_player_id;

            memcpy(&MOX::_player[target_id], &pack->target_player, sizeof(s_player));
            memcpy(&MOX::_player[pack->player_num], &pack->player, sizeof(s_player));

            for (int i = 0; i < MOX::_NUM_STARS; ++i) {
                pack->surrender_to[i] = MOX::_star[i].surrender_to[target_id];
            }
            break;
        }
        case NET_PACKET_DRAGON_INFO: {
            NETMOX::_net.sync_state = NET_SYNC_STATE_DRAGON_BRIBE_PROMPT;
            NETMOX::Decode_Dragon_Info_((int32_t*)data, (uint16_t)len);
            break;
        }
        case NET_PACKET_DRAGON_NO_BRIBE: {
            NETMOX::_net.player_net_info[from_player_id].player_state = NET_PLAYER_STATE_HUMAN_DIPLOMACY_IGNORED;
            break;
        }
        case NET_PACKET_DRAGON_BRIBE_DESTINATION: {
            NETMOX::_net.field168_0x209 = *(uint8_t*)data;
            NETMOX::_net.player_net_info[from_player_id].player_state = NET_PLAYER_STATE_DRAGON_BRIBE_DESTINATION;
            break;
        }
        case NET_PACKET_HUMAN_DIPLOMAT_WAIT: {
            if (NETMOX::_net.sync_state == NET_SYNC_STATE_HUMAN_DIPLOMACY_REQUESTED && NETMOX::_net.diplomacy_target_id == from_player_id) {
                NETMOX::_net.human_diplomat_wait_flag = 1;
            }
            break;
        }
        case NET_PACKET_CHAT: {
            if (CHAT::_chat_info != nullptr) {
                CHAT::Receive_Chat_Msg_(from_player_id, (char*)data);
            }
            break;
        }
        case NET_PACKET_LOAD_GAME_START: {
            s_net_pack_start_game* pack = (s_net_pack_start_game*)data;
            if (strcmp(pack->version_string, GAME_BUILD_DATE) != 0) {
                char error_msg[256];
                sprintf(error_msg, NETMOX::_incompatible_versions_error, pack->version_string, GAME_BUILD_DATE);
                exitm::Exit_With_Message(error_msg);
            }

            int cur_players = pack->number_of_players;
            while (cur_players > 0) {
                cur_players--;
                pack->number_of_players = cur_players; // Mirroring the exact byte decrement in original
                MOX::_player[cur_players].network_player_id = pack->net_ids[cur_players];
            }
            break;
        }
        case NET_PACKET_PLAYER_COLOR_SELECTED: {
            uint8_t color_idx = *(uint8_t*)data;
            uint8_t player_idx = *(uint8_t*)((uint8_t*)data + 1);
            RACESEL::_color_flag[color_idx] = 1;
            MOX::_player[player_idx].color = color_idx;
            break;
        }
        case NET_PACKET_REQUEST_PLAYER_COLOR: {
            uint8_t color_idx = *(uint8_t*)data;
            if (RACESEL::_color_flag[color_idx] == 0) {
                RACESEL::_color_flag[color_idx] = 1;
                MOX::_player[from_player_id].color = color_idx;
                NETMOX::_net.player_color_selected_flags[from_player_id] = 1;
            }
            break;
        }
        case NET_PACKET_GNN_CHAT: {
            if (CHAT::_chat_info != nullptr) {
                CHAT::Receive_Chat_Msg_(8, (char*)data);
            }
            break;
        }
        case NET_PACKET_BATTLE_RESULTS_CHUNK_REQUEST: {
            NETMOX::_net.player_net_info[from_player_id].player_state = NET_PLAYER_STATE_BATTLE_RESULTS_CHUNK_REQUESTED;
            break;
        }
        case NET_PACKET_BATTLE_RESULTS_CHUNK_READY: {
            NETMOX::_net.sync_state = NET_SYNC_STATE_SENDING_BATTLE_RESULTS_CHUNKS;
            break;
        }
        case NET_PACKET_PLAYER_PHASE_READY: {
            NETMOX::_net.player_race_selected_flags[from_player_id] = 1;
            break;
        }
        case NET_PACKET_PLAYER_ABDICATED:
        case NET_PACKET_PLAYER_SURRENDERED: {
            NETMOX::Decode_Drop_Out_Info_(from_player_id, (s_net_pack_drop_out_info*)data);
            break;
        }
        case NET_PACKET_PLAYER_DISCONNECTED: {
            NETMOX::Decode_Player_Disconnected_Info_(data, len);
            break;
        }
        case NET_PACKET_PLAYER_RACE_SELECTED: {
            if (NETMOX::_net.host_election_id == NETMOX::_net.net_host_id) {
                exitm::Exit_With_Message(NETMOX::_only_1_host_allowed_error);
            }
            uint8_t race_idx = *(uint8_t*)data;
            uint8_t player_idx = *(uint8_t*)((uint8_t*)data + 1);
            RACESEL::_race_flag[race_idx] = 1;
            MOX::_player[player_idx].race = race_idx;
            break;
        }
        case NET_PACKET_REQUEST_PLAYER_RACE: {
            uint8_t race_idx = *(uint8_t*)data;
            if (RACESEL::_race_flag[race_idx] == 0) {
                RACESEL::_race_flag[race_idx] = 1;
                MOX::_player[from_player_id].race = race_idx;
                NETMOX::_net.player_race_selected_alt_flags[from_player_id] = 1;
            }
            break;
        }
        case NET_PACKET_FORCE_PLAYER_ELIMINATED: {
            MOX::_player[MOX::_PLAYER_NUM].eliminated = 1;
            break;
        }
        case NET_PACKET_MONSTER_UPDATE: {
            s_net_pack_monster_update monster_update;
            memcpy(&monster_update, data, sizeof(monster_update));
            EVENTS::Event_Monster_Update_((int16_t)monster_update.param_1, (int16_t)monster_update.param_2,
                                          (int16_t)monster_update.param_3);
            break;
        }
        case NET_PACKET_PLAYER_RACE_DESELECTED: {
            uint8_t race_idx = *(uint8_t*)data;
            RACESEL::_race_flag[race_idx] = 0;
            break;
        }
        case NET_PACKET_PLAYER_RACE_DESELECT_REQUEST: {
            uint8_t race_idx = *(uint8_t*)data;
            RACESEL::_race_flag[race_idx] = 0;
            NETMOX::_net.player_race_deselected_flags[from_player_id] = 1;
            break;
        }
        case NET_PACKET_RACE_SELECTION_SYNCED: {
            NETMOX::_net.race_selection_changed = 0;
            break;
        }
        case NET_PACKET_TACTICAL_COMBAT_START: {
            NETMOX::_net.tactical_combat_sync_state = NET_TACTICAL_COMBAT_SYNC_STARTED;
            break;
        }
        case NET_PACKET_TACTICAL_COMBAT_END: {
            NETMOX::_net.tactical_combat_sync_state = NET_TACTICAL_COMBAT_SYNC_FINISHED;
            break;
        }
        case NET_PACKET_NETWORK_SYNC_STATUS: {
            if (*(int32_t*)data == 0) {
                NETMOX::_net.network_sync_complete_flag = 0;
            }
            break;
        }
        case NET_PACKET_CHANGE_RELATIONS: {
            int16_t relation_params[6];
            memcpy(relation_params, data, sizeof(relation_params));
            diplomac::Change_Relations_(relation_params[0], relation_params[1], relation_params[2],
                                        relation_params[3], relation_params[4], relation_params[5]);
            break;
        }
        case NET_PACKET_HUMAN_DIPLOMACY_READY_DATA: {
            memcpy(&MOX::_player[from_player_id], data, sizeof(s_player));
            if (NETMOX::_net.sync_state == NET_SYNC_STATE_HUMAN_DIPLOMACY_CONNECTING && NETMOX::_net.diplomacy_target_id == from_player_id) {
                uint8_t mask = ~(1 << (from_player_id & 0x1F));
                NETMOX::_net.sync_state = NET_SYNC_STATE_HUMAN_DIPLOMACY_READY;
                NETMOX::_net.players_active_mask &= mask;
                NETMOX::_net.players_ready_mask[0] &= mask;
            }
            break;
        }
        case NET_PACKET_DECLARE_WAR: {
            int32_t target_player_id;
            memcpy(&target_player_id, data, sizeof(target_player_id));
            diplomac::Declare_War_((int16_t)from_player_id, (int16_t)target_player_id);
            break;
        }
        }
    }

    void __cdecl Wrapper_For_Normal_System_Fields_() {
        s_movable_box& box = MOX::_moveable_box[0];

        SYS::Init_System_Display_(box.id);
        SYS::Add_System_Display_Popup_Fields_(
                box.id,
                box.x,
                box.y,
                box.content_offset_x,
                box.content_offset_y,
                0x0D,
                0x2F,
                &box.drag_field,
                &box.close_btn,
                &box.window_field_id);
    }

    void __cdecl Mini_Main_Screen_Text_Box_(char* text_1, char* text_2) {
        video::Set_Refresh_Full_();
        TEXTBOX::Text_Box_(text_1, text_2);
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
        Draw_Mini_Main_Screen_();
        RUSS::Mox_Sync_Update_();
        video::Set_Refresh_Stencil_();
        MOX::_draw_main_screen_to_back = 1;
    }

    void __cdecl Load_Reply_To_Diplomacy_Request_Popup_() {
        allocate::Push_Block_(MOX::_screen_seg);

        MOX::_diplomacy_request_popup_seg = (s_animation_header*)farload::Far_Reload_Next_("BUFFER0.LBX", 0x0f, MOX::_screen_seg);

        MOX::_diplomacy_request_accept_seg = (s_animation_header*)farload::Far_Reload_Next_("BUFFER0.LBX", 0x10, MOX::_screen_seg);

        MOX::_diplomacy_request_invoke_seg = (s_animation_header*)farload::Far_Reload_Next_("BUFFER0.LBX", 0x11, MOX::_screen_seg);

        MOX::_diplomacy_request_wait_seg = (s_animation_header*)farload::Far_Reload_Next_("BUFFER0.LBX", 0x12, MOX::_screen_seg);
    }

    void __cdecl Add_Reply_To_Diplomacy_Request_Fields_(
        int16_t* accept_field,
        int16_t* invoke_field,
        int16_t* wait_field,
        int16_t* dismiss_field
    ) {
        const int16_t popup_left = static_cast<int16_t>(
            ((0x1FA - animate::Get_Width_(MOX::_diplomacy_request_popup_seg)) / 2) + 0x15
        );
        const int16_t popup_top = static_cast<int16_t>(
            ((0x190 - animate::Get_Height_(MOX::_diplomacy_request_popup_seg)) / 2) + 0x15
        );
        const int16_t button_top = static_cast<int16_t>(popup_top + 0x5A);

        *accept_field = fields::Add_Button_Field_(
            static_cast<int16_t>(popup_left + 0x40),
            button_top,
            "",
            MOX::_diplomacy_request_accept_seg,
            "A",
            0x28
        );

        *invoke_field = fields::Add_Button_Field_(
            static_cast<int16_t>(popup_left + 0xBC),
            button_top,
            "",
            MOX::_diplomacy_request_invoke_seg,
            "I",
            0x28
        );

        *wait_field = fields::Add_Button_Field_(
            static_cast<int16_t>(popup_left + 0x13B),
            button_top,
            "",
            MOX::_diplomacy_request_wait_seg,
            "W",
            0x28
        );

        const int16_t popup_bottom = static_cast<int16_t>(
            animate::Get_Height_(MOX::_diplomacy_request_popup_seg) + popup_top
        );
        const int16_t popup_right = static_cast<int16_t>(
            animate::Get_Width_(MOX::_diplomacy_request_popup_seg) + popup_left
        );

        fields::Add_Hidden_Field_(
            popup_left,
            popup_top,
            popup_right,
            popup_bottom,
            "",
            0
        );

        *dismiss_field = fields::Add_Hidden_Field_(
            0,
            0,
            0x27F,
            0x1DF,
            "\x1B",
            0x29
        );
    }

    void __cdecl Draw_Reply_To_Diplomacy_Request_() {
        int32_t popup_x = (0x1FA - animate::Get_Width_(MOX::_diplomacy_request_popup_seg)) / 2 + 0x15;
        int32_t popup_y = (0x190 - animate::Get_Height_(MOX::_diplomacy_request_popup_seg)) / 2 + 0x15;
        int32_t player_index = static_cast<int32_t>(MOX::_screen_data);
        uint32_t player_color =
                MOX::_main_palette_player_colors[static_cast<uint8_t>(MOX::_player[player_index].color)];
        s_colors font_colors;

        Draw_Mini_Main_Screen_();
        Draw_Main_Main_Screen_();
        animate::Draw_(static_cast<int16_t>(popup_x), static_cast<int16_t>(popup_y), MOX::_diplomacy_request_popup_seg);

        MISC::Get_Mox_Font_Colors_(4, static_cast<uint8_t>(player_color), static_cast<uint8_t>(player_color + 3), &font_colors);
        fonts::Set_Font_Style_(4, &font_colors);

        s_player* player = &MOX::_player[player_index];
        const char* message_format = HAROLD::H_Message_(0xDB);

        sprintf(MOX::_temp_string, message_format, player->name, player->race_name);
        fonts::Print_(static_cast<int16_t>(popup_x + 0x27), static_cast<int16_t>(popup_y + 0x3C), MOX::_temp_string);
    }

    void __cdecl Save_Hotseat_Map_Info_() {
        const int16_t saved_player_num = MOX::_PLAYER_NUM;
        const int32_t saved_player_index = saved_player_num;

        _hotseat_system_box[saved_player_index].x = MOX::_moveable_box[0].x;
        _hotseat_system_box[saved_player_index].y = MOX::_moveable_box[0].y;
        _hotseat_system_box[saved_player_index].system_id = MOX::_moveable_box[0].id;

        _hotseat_fleet_box[saved_player_index].x = MOX::_moveable_box[2].x;
        _hotseat_fleet_box[saved_player_index].y = MOX::_moveable_box[2].y;
        _hotseat_fleet_box[saved_player_index].box_id = MOX::_moveable_box[2].id;

        HACCESS::Save_Hotseat_Fleet_Box_Ship_(saved_player_num);

        const int32_t current_player_index = MOX::_PLAYER_NUM;
        _hotseat_map_scale[current_player_index] = static_cast<uint8_t>(MOX::_cur_map_scale);
    }
}