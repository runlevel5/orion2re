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

    void* __cdecl Get_Star_Picture_Seg_(int8_t spectral_class, int8_t pict_type, int8_t size) {
        int16_t zoom_level = HAROLD::Map_Scale_To_Zoom_Level_();
        int16_t class_offset = (int16_t)spectral_class * 6;

        int16_t result_ax = zoom_level;
        if (spectral_class != STAR_CLASS_BLACK_HOLE) {
            result_ax += (int16_t)size;
        }
        result_ax += class_offset;

        int32_t final_index = (int32_t)result_ax;

        MOX::_star_seg[final_index] = Buffer_Reload_(
            s_BUFFER0_LBX,
            final_index + 148,
            MOX::_global_cache_seg
        );

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

    void __cdecl Draw_A_Star_(int16_t star_idx) {
        int16_t zoom_size = HAROLD::Map_Scale_Star_Size_To_Zoom_Level_(star_idx);
        s_star_data* star = &MOX::_star[star_idx];

        void* star_seg = Get_Star_Picture_Seg_(star->spectral_class, star->pict_type, star->size);

        int16_t screen_x;
        int16_t screen_y;
        Get_Star_Draw_Coords_(star_idx, &screen_x, &screen_y);

        screen_x -= zoom_size / 2;
        screen_y -= zoom_size / 2;

        if (static_cast<int8_t>(star->not_used1) < 0) {
            animate::Reset_Animation_Frame_(static_cast<int16_t*>(star_seg));
            animate::Draw_(screen_x, screen_y, star_seg);
        } else {
            star->not_used1++;

            int16_t frame_count = animate::Get_Animation_Frame_Count_(static_cast<int16_t*>(star_seg));

            if (static_cast<int8_t>(star->not_used2) <= static_cast<int8_t>(star->not_used1)) {
                int32_t remaining = static_cast<int32_t>(_n_twinkling_stars) - 1;
                _n_twinkling_stars = static_cast<int16_t>((remaining < 1) ? 0 : remaining);

                star->not_used1 = 0xFF;
                star->not_used2 = 0xFF;
            }

            int32_t current_frame = (static_cast<int16_t>(star->not_used1) / 2) % frame_count;
            animate::Set_Animation_Frame_(static_cast<int16_t*>(star_seg), current_frame);
            animate::Draw_(screen_x, screen_y, star_seg);
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
                    is_visible = OFFICER::One_Leader_With_Galactic_Lore_(MOX::_PLAYER_NUM);
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

                    void* seg = Get_Star_Picture_Seg_(star.spectral_class, star.pict_type, star.size);

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

    void __cdecl Draw_Gate_Icons_() {
        int16_t star_idx = 0;
        int16_t num_stars = MOX::_NUM_STARS;

        if (num_stars > 0) {
            do {
                Draw_A_Gate_Icon_(static_cast<int32_t>(star_idx));
                star_idx++;
            } while (star_idx < num_stars);
        }
    }
}