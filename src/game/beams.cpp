#include "pch.h"

namespace BEAMS {
    static const s_colors* Build_Damage_Indicator_Color_Window_(s_colors* dst, const uint8_t* src) {
        memcpy(dst->colors, src, sizeof(dst->colors));
        return dst;
    }

    static void Set_Weapon_Visual_FX_Colors_(s_weapon_visual_fx* fx, int32_t r1, int32_t g1, int32_t b1,
                                             int32_t r2, int32_t g2, int32_t b2) {
        fx->r1 = r1;
        fx->g1 = g1;
        fx->b1 = b1;
        fx->r2 = r2;
        fx->g2 = g2;
        fx->b2 = b2;
    }

    void __cdecl Reset_Beam_Swapping_Colors_() {
        BEAMS::_old_color[0] = -1;
        BEAMS::_old_color[1] = -1;
    }

    /**
     * @address 00420ca0
     */
    void __cdecl Init_Beams_() {
        _l_anims = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(COLONY::_BEAMS_LBX, 0, MOX::_screen_seg));
        Reset_Beam_Swapping_Colors_();
    }

    s_animation_header* __cdecl Reflection_Field_Anims_(int16_t p1, int16_t p2) {
        int16_t entry_num = (int16_t)(p2 * 5 + p1 + 0x59);

        return (s_animation_header*)buffer::Buffer_Reload_(
            COLONY::_BEAMS_LBX,
            (int32_t)entry_num,
            MOX::_global_cache_seg
        );
    }

    void __cdecl Load_Reflection_Field_Hit_(int8_t should_load) {
        if (should_load != 0) {
            s_animation_header* anim = BEAMS::Reflection_Field_Anims_(0, 0);
            animate::Draw_Palette_(anim);
            palette::Refresh_Palette_();
        }
    }

    void __cdecl Load_Ship_Burst_() {
        int entry_idx;

        for (int i = 0; i < 2; ++i) {
            if (i == 0) {
                entry_idx = 1;
            } else {
                entry_idx = 33;
            }

            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 16; ++k) {
                    _laser_burst_anim[i][j][k] = (s_animation_header*)farload::Far_Reload_Next_(
                        COLONY::_BEAMS_LBX,
                        entry_idx,
                        MOX::_screen_seg
                    );
                    entry_idx++;
                }
            }
        }
    }

    void __cdecl Load_Combat_Beam_Stuff_(int8_t reflection_hit_param) {
        Init_Beams_();
        FIREPTS::Load_FP_Data_From_LBX_();
        Load_Ship_Burst_();
        Load_Ship_Explosion_();
        Load_Shield_Hit_();
        Load_Reflection_Field_Hit_(reflection_hit_param);

        _mauler_ball_seg = reinterpret_cast<s_animation_header*>(
            farload::Far_Reload_Next_(COLONY::_BEAMS_LBX, 0x58, MOX::_screen_seg)
        );
    }

    /**
     * @address 00420c50
     */
    void __cdecl Load_Ship_Explosion_() {
        for (int i = 0; i < 2; ++i) {
            int entry_index = i + 0x41;
            for (int j = 0; j < 16; ++j) {
                _explosion_anim[i][j] = (s_animation_header*)farload::Far_Reload_Next_(
                    COLONY::_BEAMS_LBX,
                    entry_index,
                    MOX::_screen_seg
                );
            }
        }
    }

    /**
     * @address 00420ae0
     */
    void __cdecl Draw_Ship_Explosion_(int16_t x, int16_t y, int16_t type_idx, int16_t frame_idx, int16_t size_idx) {
        s_animation_header* anim = _explosion_anim[size_idx][type_idx];
        int16_t count1 = animate::Get_Animation_Frame_Count_(anim);
        animate::Set_Animation_Frame_(anim, frame_idx % (count1 - 1));

        anim = _explosion_anim[size_idx][type_idx];
        int16_t count2 = animate::Get_Animation_Frame_Count_(anim);
        animate::Set_Animation_Frame_(anim, frame_idx % (count2 - 1));

        animate::Draw_(x - 8, y - 8, anim);
    }

    /**
     * @brief Retrieves or reloads the shield animation header based on ship size and flags.
     * @param beam_idx The type index of the beam or shield visual.
     * @param size_idx The ship size index (e.g., 0=Small, 1=Medium, etc.).
     * @param is_special Flag indicating a special or modified state.
     * @param is_heavy Flag indicating a heavy variant of the shield.
     * @return A pointer to the s_animation_header for the requested animation.
     */
    s_animation_header* __cdecl Shield_Anims_(int16_t beam_idx, int16_t size_idx, int16_t is_special, int16_t is_heavy) {
        if (size_idx >= 4) {
            // For large sizes, reload from LBX using a specific offset
            return (s_animation_header*)buffer::Buffer_Reload_(COLONY::_BEAMS_LBX, size_idx * 5 + beam_idx + 113 /* 0x71 */, MOX::_global_cache_seg);
        }

        if (is_heavy != 0) {
            // Heavy variant offset
            return (s_animation_header*)buffer::Buffer_Reload_(COLONY::_BEAMS_LBX, size_idx + 129 /* 0x81 */, MOX::_global_cache_seg);
        }

        if (size_idx < 4 && is_special == 0) {
            // Return from the pre-cached shield animations
            return BEAMS::_shield_anims[beam_idx][size_idx][0];
        }

        // Default reload path
        return (s_animation_header*)buffer::Buffer_Reload_(COLONY::_BEAMS_LBX, size_idx * 5 + beam_idx + 109 /* 0x6D */, MOX::_global_cache_seg);
    }

    void __cdecl Load_Shield_Hit_() {
        int8_t palettes[16];
        int entry_index = 0x43; /* 67 */

        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < 5; ++i) {
                // Load hit animations from BEAMS.LBX starting at entry 67.
                // The animations are stored in a 5x4 grid (shield type x direction).
                _shield_anims[i][j][0] = (s_animation_header*)farload::Far_Reload_Next_(
                    COLONY::_BEAMS_LBX,
                    entry_index,
                    MOX::_screen_seg
                );
                entry_index++;
            }
        }

        // Generate 16 brightness levels for shield impact effects.
        for (int i = 0; i < 16; ++i) {
            palettes[i] = (int8_t)(i * 4 + 30);
        }

        bitmap::Create_Brighten_Bitmap_Palettes_(MOX::_screen_seg, palettes, 16);
    }

    /**
     * @address 00422980
     */
    void __cdecl Squish_Out_Msg_Queue_Slot_(int16_t index) {
        // BUG: removing an entry from the queue while iterating it in Draw_Damage_Message_Queue_
        // will cause the next entry to be skipped.
        for (int i = (int)index; i < 34; ++i) {
            _msg_queue[i] = _msg_queue[i + 1];
        }
        _n_damage_msgs--;
    }

    /**
     * @address 004228f0
     */
    void __cdecl Add_Damage_Message_To_Queue_(int16_t x, int16_t y, int16_t total_dmg, int16_t struct_dmg, int16_t shield_dmg, uint8_t side) {
        if (BEAMS::_n_damage_msgs < 35 /* 0x23 */) {
            // Check if there is any positive damage value to report
            if (total_dmg > 0 || struct_dmg > 0 || shield_dmg > 0) {
                int16_t current_index = BEAMS::_n_damage_msgs;
                s_beam_dmg_msg& msg = BEAMS::_msg_queue[current_index];

                msg.anim_timer = 0;
                msg.x = x;
                msg.y = y;
                msg.total_dmg = total_dmg;
                msg.hull_dmg = struct_dmg;
                msg.shield_dmg = shield_dmg;
                msg.side = side;

                BEAMS::_n_damage_msgs++;
            }
        }
    }

    void __cdecl Add_Damage_Indicator_For_Ship_(int16_t dx, int16_t dy, int16_t ship_idx, uint8_t side, int16_t total_damage, int16_t struct_damage) {
        int16_t x_center;
        int16_t y_center;
        int16_t size_offset;

        // Size class: 0=Tiny, 1=Small, 2=Medium, 3=Large, 4=Huge, 5=Titan/Starbase
        size_offset = static_cast<int16_t>(MOX::_combat_data[ship_idx].size_class) * 8;

        COMBINIT::Ship_Center_Offsets_(ship_idx, &x_center, &y_center);

        x_center += dx;
        y_center += dy;

        if (side != 0) {
            x_center += size_offset;
        } else {
            x_center -= size_offset;
        }

        y_center -= size_offset;

        BEAMS::Add_Damage_Message_To_Queue_(x_center, y_center, total_damage, struct_damage, 0, side);
    }

    void __cdecl Draw_Shield_Hit_Once_Through_(int16_t ship_idx, int16_t param_2, int16_t draw_x, int16_t draw_y) {
        int8_t size_class = MOX::_combat_data[ship_idx].size_class;
        int16_t shield_size = Ship_Size_To_Shield_Size_(size_class);

        int16_t shield_status = 0;
        int16_t offset_x;
        int16_t offset_y;

        COMBINIT::Ship_Center_Offsets_(ship_idx, &offset_x, &offset_y);

        int16_t shield_size2 = Ship_Size_To_Shield_Size_(size_class);
        int16_t shield_radius = _shield_diameters[shield_size2] / 2;

        int16_t start_x = draw_x - shield_radius;
        int16_t start_y = draw_y - shield_radius;

        s_animation_header* anim = Shield_Anims_(0, shield_size, 0, 0);
        animate::Get_Animation_Frame_Count_(anim);

        _shield_hit_now_flag = 1;

        do {
            do {
                timer::Mark_Time_();
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                graphics::Set_Window_(0, 0, 0x27F, 0x15E);
                video::Clip_On_();

                if (ship_idx == MOX::_dont_draw_ship) {
                    CMBTDRW1::Draw_Ship_(ship_idx, draw_x - offset_x, draw_y - offset_y);
                }

                Adjust_Shield_Display_Status_(&shield_status);
                Draw_Shield_Hit_(start_x, start_y, ship_idx, 0, 0, param_2, shield_status);
                Draw_Damage_Message_Queue_();

                timer::Release_Time_(1);
                RUSS::Mox_Sync_Update_();
                video::Clip_Off_();
                graphics::Reset_Window_();

                _shield_hit_now_flag = 0;
            } while (_shield_display_status != 0);
        } while (Done_Drawing_Damage_Message_Queue_() == 0);
    }

    /**
     * @address 00422a70
     */
    void __cdecl Draw_Damage_Indicator_(int16_t x, int16_t y, int16_t total_dmg, int16_t hull_dmg, int16_t shield_dmg, uint8_t side, int16_t anim_timer) {
        const uint8_t total_dmg_color_steps[27] = {
            0xb0, 0xb9, 0xba, 0xb0, 0xb8, 0xb9, 0xb0, 0xb7, 0xb8,
            0xb0, 0xb6, 0xb7, 0xb0, 0xb5, 0xb6, 0xb0, 0xb4, 0xb5,
            0xb0, 0xb3, 0xb4, 0xb0, 0xb2, 0xb3, 0xb0, 0xb1, 0xb2
        };

        const uint8_t shield_dmg_color_steps[27] = {
            0x50, 0x57, 0x57, 0x50, 0x56, 0x57, 0x50, 0x55, 0x56,
            0x50, 0x54, 0x55, 0x50, 0x53, 0x54, 0x50, 0x52, 0x53,
            0x50, 0x51, 0x52, 0x50, 0x50, 0x51, 0x50, 0x50, 0x50
        };

        const uint8_t hull_dmg_color_steps[27] = {
            0xc0, 0xcc, 0xc5, 0xc0, 0xcc, 0xc5, 0xc0, 0xcc, 0xc5,
            0xc0, 0xc4, 0xcc, 0xc0, 0xcb, 0xc4, 0xc0, 0xc3, 0xcb,
            0xc0, 0xc3, 0xcb, 0xc0, 0xca, 0xc3, 0xc0, 0xc2, 0xca
        };
        const uint8_t dmg_color_blob_template[0x58] = {
            0xb0, 0xb9, 0xba, 0xb0, 0xb8, 0xb9, 0xb0, 0xb7, 0xb8,
            0xb0, 0xb6, 0xb7, 0xb0, 0xb5, 0xb6, 0xb0, 0xb4, 0xb5,
            0xb0, 0xb3, 0xb4, 0xb0, 0xb2, 0xb3, 0xb0, 0xb1, 0xb2,
            0x00,
            0x50, 0x57, 0x57, 0x50, 0x56, 0x57, 0x50, 0x55, 0x56,
            0x50, 0x54, 0x55, 0x50, 0x53, 0x54, 0x50, 0x52, 0x53,
            0x50, 0x51, 0x52, 0x50, 0x50, 0x51, 0x50, 0x50, 0x50,
            0x00,
            0xc0, 0xcc, 0xc5, 0xc0, 0xcc, 0xc5, 0xc0, 0xcc, 0xc5,
            0xc0, 0xc4, 0xcc, 0xc0, 0xcb, 0xc4, 0xc0, 0xc3, 0xcb,
            0xc0, 0xc3, 0xcb, 0xc0, 0xca, 0xc3, 0xc0, 0xc2, 0xca,
            0x00, 0x00, 0x00, 0x00, 0x00
        };
        uint8_t dmg_color_blob[sizeof(dmg_color_blob_template)];
        s_colors font_colors;

        memcpy(dmg_color_blob, dmg_color_blob_template, sizeof(dmg_color_blob));

        y -= anim_timer;

        if (side != 1) {
            x -= anim_timer;
        } else {
            x += anim_timer;
        }

        if (anim_timer >= 9) {
            return;
        }

        graphics::Set_Window_(0, 0, 0x27f, 0x15e);
        video::Clip_On_();

        if (total_dmg > 0) {
            fonts::Set_Font_Style_Shadow_Down_(1, Build_Damage_Indicator_Color_Window_(&font_colors, &dmg_color_blob[anim_timer * 3]));
            fonts::Print_Integer_(x, y, total_dmg);
        }

        if (shield_dmg > 0) {
            fonts::Set_Font_Style_Shadow_Down_(1, Build_Damage_Indicator_Color_Window_(&font_colors, &dmg_color_blob[0x1c + anim_timer * 3]));
            fonts::Print_Integer_(x - 10, y, shield_dmg);
        }

        if (hull_dmg > 0) {
            fonts::Set_Font_Style_Shadow_Down_(1, Build_Damage_Indicator_Color_Window_(&font_colors, &dmg_color_blob[0x38 + anim_timer * 3]));
            fonts::Print_Integer_(x - 20, y, hull_dmg);
        }

        video::Clip_Off_();
    }

    /**
     * @address 00422a60
     */
    int8_t __cdecl Done_Drawing_Damage_Message_Queue_() {
        return _n_damage_msgs == 0;
    }

    /**
     * @address 004229d0
     */
    void __cdecl Draw_Damage_Message_Queue_() {
        for (int16_t i = 0; i < BEAMS::_n_damage_msgs; ++i) {
            s_beam_dmg_msg& msg = BEAMS::_msg_queue[i];

            BEAMS::Draw_Damage_Indicator_(
                msg.x,
                msg.y,
                msg.total_dmg,
                msg.hull_dmg,
                msg.shield_dmg,
                msg.side,
                msg.anim_timer
            );

            msg.anim_timer++;

            if (msg.anim_timer >= 9) {
                // Note: Possible bug in original code where incrementing i after removal
                // might skip the next message entry.
                BEAMS::Squish_Out_Msg_Queue_Slot_(i);
            }
        }

        BEAMS::Done_Drawing_Damage_Message_Queue_();
    }

    void __cdecl Adjust_Shield_Display_Status_(int16_t* shield_status) {
        s_animation_header* anim_header = BEAMS::Shield_Anims_(0, 0, 0, 0);
        int16_t total_frames = animate::Get_Animation_Frame_Count_(anim_header);

        switch (BEAMS::_shield_display_status) {
            case 0:
                if (BEAMS::_shield_hit_now_flag != 0) {
                    BEAMS::_shield_display_status = 1;
                    *shield_status = 0;
                }
                break;

            case 1:
                (*shield_status)++;
                if (*shield_status >= 4) {
                    BEAMS::_shield_display_status = 2;
                }
                break;

            case 2:
                (*shield_status)++;
                if (BEAMS::_shield_hit_now_flag != 0) {
                    if (*shield_status >= 7) {
                        *shield_status = 4;
                    }
                } else {
                    if (*shield_status >= 7) {
                        BEAMS::_shield_display_status = 3;
                    }
                }
                break;

            case 3:
                (*shield_status)++;
                if (*shield_status >= total_frames) {
                    BEAMS::_shield_display_status = 0;
                }
                break;

            default:
                break;
        }
    }

    /**
     * @address 00423370
     */
    int16_t __cdecl Ship_Size_To_Shield_Size_(int16_t ship_size) {
        int16_t shield_sizes[6];

        shield_sizes[0] = 0;
        shield_sizes[1] = 1;
        shield_sizes[2] = 1;
        shield_sizes[3] = 2;
        shield_sizes[4] = 3;
        shield_sizes[5] = 3;

        return shield_sizes[ship_size];
    }

    /**
     * @address 00422e70
     */
    void __cdecl Draw_Damage_Message_Queue_Until_Done_() {
        do {
            timer::Mark_Time_();
            video::Set_Page_Off_();
            CMBTDRW1::Draw_Main_Combat_Screen_();
            graphics::Set_Window_(0, 0, 639, 350);
            video::Clip_On_();
            BEAMS::Draw_Damage_Message_Queue_();
            timer::Release_Time_(1);
            RUSS::Mox_Sync_Update_();
            video::Clip_Off_();
            graphics::Reset_Window_();
        } while (!BEAMS::Done_Drawing_Damage_Message_Queue_());
    }

    int16_t __cdecl Get_Max_Frames_() {
        return BEAMS::_max_frames;
    }

    /**
     * @address 00420a50
     */
    int16_t __cdecl Get_Ship_Range_(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        int16_t dx = x2 - x1;
        if (dx < 0) {
            dx = -dx;
        }

        int16_t dy = y2 - y1;
        if (dy < 0) {
            dy = -dy;
        }

        if (dx <= dy) {
            dx = dy;
        }

        return dx;
    }

    /**
     * @address 004233f0
     */
    void __cdecl Draw_Shield_Hit_(int16_t x, int16_t y, int16_t ship_idx, int16_t param_4, int16_t param_5, int16_t param_6, int16_t param_7) {
        int16_t base_rot = BEAMS::Rot_To_Base_Rot_(param_6);
        int16_t rot_method = BEAMS::Rot_To_Rot_Method_(param_6);

        if (BEAMS::_shield_display_status == 0) {
            return;
        }

        int16_t shield_size_class;
        int16_t shield_size;

        if (ship_idx != 0) {
            shield_size_class = MOX::_combat_data[ship_idx].size_class;
            shield_size = BEAMS::Ship_Size_To_Shield_Size_(shield_size_class);
        } else {
            int16_t planet_idx = MOX::_colony[MOX::_cmbt_colony_id].planet;
            shield_size_class = MOX::_combat_data[0].size_class + 6;
            shield_size = BEAMS::Planet_Size_To_Shield_Size_(MOX::_planet[planet_idx].size);
        }

        uint8_t diam = BEAMS::_shield_diameters[shield_size];
        uint32_t buf_size = (uint32_t)diam * diam + 32;

        allocate::Push_Block_(MOX::_screen_seg);
        s_animation_header* sprite1 = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, buf_size);
        bitmap::Create_Blank_Bitmap_(diam, diam, sprite1);

        s_animation_header* sprite2 = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, buf_size);
        bitmap::Create_Blank_Bitmap_(diam, diam, sprite2);

        if (param_7 == 0 && BEAMS::_g_shield_sfx_handle == 0) {
            if (BEAMS::_g_reflection_state_flag == 0) {
                BEAMS::_g_shield_sfx_handle = ERICNET::Mox_Play_Sound_(38);
            } else {
                BEAMS::_g_shield_sfx_handle = ERICNET::Mox_Play_Sound_(42);
            }

            if (ship_idx != 0) {
                sound::Pitch_Bend_Sound_(BEAMS::_g_shield_sfx_handle, -(shield_size_class * 2500));
            } else {
                sound::Pitch_Bend_Sound_(BEAMS::_g_shield_sfx_handle, -15000);
            }
        } else {
            s_animation_header* anim = BEAMS::Shield_Anims_(base_rot, shield_size, param_4, param_5);
            int16_t frame_count = animate::Get_Animation_Frame_Count_(anim);

            if ((frame_count - 1 <= param_7) || (BEAMS::_g_shield_sfx_handle != 0 && BEAMS::_shield_display_status == 0)) {
                sound::Fadeout_Sound_(BEAMS::_g_shield_sfx_handle, 10);
                BEAMS::_g_shield_sfx_handle = 0;
            }
        }

        s_animation_header* anim = BEAMS::Shield_Anims_(base_rot, shield_size, param_4, param_5);
        int16_t anim_frame_count = animate::Get_Animation_Frame_Count_(anim);
        int16_t anim_frame = param_7 % anim_frame_count;

        anim = BEAMS::Shield_Anims_(base_rot, shield_size, param_4, param_5);
        animate::Set_Animation_Frame_(anim, anim_frame);

        anim = BEAMS::Shield_Anims_(base_rot, shield_size, param_4, param_5);
        bitmap::Draw_Picture_To_Bitmap_(sprite1, anim);

        s_animation_header* draw_src = sprite1;
        s_animation_header* draw_dst = sprite2;

        switch (rot_method) {
            case 0:
                bitmap::Set_Picture_Glassed_(sprite1);
                draw_dst = sprite2;
                draw_src = sprite1;
                break;
            case 1:
                bitmap::Flip_Bitmap_(sprite2, sprite1, 0);
                draw_dst = sprite1;
                draw_src = sprite2;
                break;
            case 2:
                bitmap::Flip_Bitmap_(sprite2, sprite1, 3);
                bitmap::Flip_Bitmap_(sprite1, sprite2, 0);
                draw_dst = sprite2;
                draw_src = sprite1;
                break;
            case 3:
                bitmap::Flip_Bitmap_(sprite2, sprite1, 3);
                draw_dst = sprite1;
                draw_src = sprite2;
                break;
            default:
                break;
        }

        if (rot_method <= 3) {
            bitmap::Draw_Picture_To_Bitmap_(draw_dst, draw_src);
        }

        if (ship_idx == 0) {
            int16_t planet_idx = MOX::_colony[MOX::_cmbt_colony_id].planet;
            int16_t current_shield_size = BEAMS::Planet_Size_To_Shield_Size_(MOX::_planet[planet_idx].size);
            uint8_t diam2 = BEAMS::_shield_diameters[current_shield_size];

            int16_t c_cx, c_cy;
            COMBINIT::Ship_Center_Offsets_(ship_idx, &c_cx, &c_cy);

            int16_t half_diam2 = diam2 / 2;
            int16_t x_offset = half_diam2 - c_cx;
            int16_t y_offset = half_diam2 - c_cy;

            int16_t s_cx, s_cy;
            CMBTDRW1::Get_Draw_Ship_Offsets_(ship_idx, &s_cx, &s_cy);

            int16_t draw_x = x + x_offset + s_cx;
            int16_t draw_y = y + y_offset + s_cy;

            bitmap::Draw_Picture_To_Bitmap_(sprite2, MOX::_combat_planet_seg);

            int16_t actual_x, actual_y, actual_w, actual_h;
            bitmap::Get_Bitmap_Actual_Size_(sprite2, &actual_x, &actual_y, &actual_w, &actual_h);

            bitmap::Mask_Brighten_Bitmap_(
                (actual_w / 2) - half_diam2,
                (actual_h / 2) - half_diam2,
                sprite2,
                sprite1,
                -16
            );

            graphics::Set_Window_(0, 0, 0x27F, 0x15E);
            video::Clip_On_();

            animate::Draw_(draw_x, draw_y, sprite2);
        } else {
            CMBTDRW1::Draw_Ship_To_Bitmap_(ship_idx, 0, 0, 0);

            int16_t current_shield_size = BEAMS::Ship_Size_To_Shield_Size_(MOX::_combat_data[ship_idx].size_class);
            uint8_t diam2 = BEAMS::_shield_diameters[current_shield_size];

            int16_t c_cx, c_cy;
            COMBINIT::Ship_Center_Offsets_(ship_idx, &c_cx, &c_cy);

            int16_t half_diam2 = diam2 / 2;
            int16_t x_offset = half_diam2 - c_cx;
            int16_t y_offset = half_diam2 - c_cy;

            int16_t s_cx, s_cy;
            CMBTDRW1::Get_Draw_Ship_Offsets_(ship_idx, &s_cx, &s_cy);

            int16_t draw_x = x + x_offset + s_cx;
            int16_t draw_y = y + y_offset + s_cy;

            int16_t actual_x, actual_y, actual_w, actual_h;
            bitmap::Get_Bitmap_Actual_Size_(MOX::_draw_ship_bitmap, &actual_x, &actual_y, &actual_w, &actual_h);

            bitmap::Mask_Brighten_Bitmap_(
                (actual_w / 2) - half_diam2,
                (actual_h / 2) - half_diam2,
                MOX::_draw_ship_bitmap,
                sprite1,
                -16
            );

            graphics::Set_Window_(0, 0, 0x27F, 0x15E);
            video::Clip_On_();

            animate::Draw_(draw_x, draw_y, MOX::_draw_ship_bitmap);
        }

        bitmap::Set_Picture_Glassed_(sprite1);
        animate::Draw_(x, y, sprite1);

        allocate::Pop_Block_(MOX::_screen_seg);
        fields::Get_Input_();
    }

    /**
     * @address 00422dc0
     */
    void __cdecl Add_Damage_Indicator_For_Missile_(int16_t x, int16_t y, uint8_t direction, int16_t damage) {
        int16_t adjusted_x;

        if (direction == 0) {
            adjusted_x = x - 8;
        } else {
            adjusted_x = x + 8;
        }

        int16_t adjusted_y = y - 8;

        BEAMS::Add_Damage_Message_To_Queue_(adjusted_x, adjusted_y, damage, 0, 0, direction);
    }

    /**
     * @address 00422e00
     */
    void __cdecl Add_Damage_Indicator_For_Biological_(int16_t x, int16_t y, uint8_t side_flag, int16_t damage) {
        int16_t target_x;

        if (side_flag != 0) {
            target_x = x + 8;
        } else {
            target_x = x - 8;
        }

        BEAMS::Add_Damage_Message_To_Queue_(target_x, y - 8, 0, 0, damage, side_flag);
    }

    /**
     * @address 00422770
     */
    int8_t __cdecl Draw_Fighter_Beam_(int16_t* fighter_coords, int16_t count, int16_t target_x, int16_t target_y, int16_t weapon_type, int16_t frame) {
        uint32_t original_seed = random::Get_Random_Seed_();
        
        graphics::Set_Window_(0, 0, 639, 350);
        video::Clip_On_();

        if (count > 0) {
            for (int16_t i = 0; i < count; ++i) {
                int16_t fx = fighter_coords[i * 2];
                int16_t fy = fighter_coords[i * 2 + 1];

                int16_t q1 = angle::Get_Angle_(fx - target_x, target_y - fy);
                q1 = (int16_t)(q1 / 22 + 8);
                int16_t dir1 = (q1 < 0) ? -(int16_t)((-q1) & 0xF) : (int16_t)(q1 & 0xF);

                int16_t q2 = angle::Get_Angle_(target_x - fx, fy - target_y);
                q2 = (int16_t)(q2 / 22 + 8);
                int16_t dir2 = (q2 < 0) ? -(int16_t)((-q2) & 0xF) : (int16_t)(q2 & 0xF);

                if (BEAMS::Get_Max_Frames_() < frame) {
                    return 0;
                }

                int16_t extra_flag = ((i % 3) == 0) ? 1 : 0;
                BEAMS::Fire_Weapon_(fx, fy, target_x, target_y, dir1, dir2, weapon_type, frame, 
                                    0, 0, 0, 0, 1, 0, 0, -1, 0, extra_flag);
            }
        }

        video::Clip_Off_();
        int16_t max_frames = BEAMS::Get_Max_Frames_();
        random::Set_Random_Seed_(original_seed);

        if (max_frames > frame) {
            return 1;
        }

        return 0;
    }

    /**
     * @address 0x00423980
     */
    int16_t __cdecl Rot_To_Base_Rot_(int16_t rot) {
        int16_t base_rotations[16] = {
            0, 1, 2, 3, 4, 3, 2, 1,
            0, 1, 2, 3, 4, 3, 2, 1
        };

        int16_t idx = (rot < 0) ? (int16_t)-((-rot) & 0xF) : (int16_t)(rot & 0xF);
        return *(base_rotations + idx);
    }

    /**
     * @address 0x00423A10
     */
    int16_t __cdecl Rot_To_Rot_Method_(int16_t rot) {
        int16_t methods[16] = {
            0, 0, 0, 0, 0,
            1, 1, 1, 1,
            2, 2, 2,
            3, 3, 3, 3
        };

        int16_t idx = (rot < 0) ? (int16_t)-((-rot) & 0xF) : (int16_t)(rot & 0xF);
        return *(methods + idx);
    }

    /**
     * @address 004233b0
     */
    int16_t __cdecl Planet_Size_To_Shield_Size_(int16_t planet_size) {
        int16_t sizes[6];

        sizes[0] = 4;
        sizes[1] = 5;
        sizes[2] = 6;
        sizes[3] = 7;
        sizes[4] = 7;

        return sizes[planet_size];
    }

    void __cdecl Fire_Weapon_(int16_t attacker_x, int16_t attacker_y, int16_t target_x, int16_t target_y,
                              int16_t attacker_facing, int16_t target_facing, int16_t weapon_id, int16_t current_frame,
                              int16_t fx_slot_idx, int16_t target_size_class, int16_t hit_result_flags,
                              int16_t special_flag, int16_t is_non_ship_target, int16_t fire_point_idx,
                              int16_t reflection_state, int16_t attacker_size_class, uint16_t weapon_specials,
                              int16_t unused_fighter_beam_flag)
    {
        (void)unused_fighter_beam_flag;

        int16_t v_duration;
        int16_t v_color;
        int16_t v_mount_scale_mod;
        int16_t v_ebx = 0;
        int16_t v_local_58 = 0;
        int16_t loop_count;
        int16_t v_local_54;
        int16_t v_local_54_2;
        int16_t v_local_54_4;
        int16_t v_local_54_6;
        int16_t v_local_4c;
        int16_t v_local_4a;
        int16_t v_local_4a_2;
        int16_t v_local_46;
        int16_t v_local_46_2;
        int16_t v_local_42;
        int16_t v_local_42_2;
        int16_t v_local_3e;
        int16_t v_local_3e_2;
        int16_t v_sfx_n;
        int16_t v_local_38;
        int16_t v_sfx_sprite_id;
        int16_t v_is_multi_beam = 0;

        switch (weapon_id) {
        case 1:
            v_color = 7;
            v_duration = 1;
            v_local_58 = 1;
            v_ebx = 0;
            v_sfx_sprite_id = 0x2D;
            v_local_54_6 = 10;
            v_local_3e = 0;
            v_local_42_2 = 0;
            v_local_42 = 1;
            v_local_54_4 = 0;
            v_local_54_2 = 7;
            v_local_54 = 0;
            v_local_46_2 = 1;
            v_local_46 = 1;
            v_local_38 = 1;
            v_mount_scale_mod = 1;
            v_local_3e_2 = 0;
            v_sfx_n = 12;
            v_local_4a_2 = 0;
            v_local_4a = 0;
            v_local_4c = 0;
            v_is_multi_beam = 1;
            break;
        case 2:
            v_color = 2;
            v_mount_scale_mod = 1;
            v_local_3e = 0x28;
            v_local_42_2 = 3;
            v_sfx_sprite_id = 0x2D;
            v_duration = v_color;
            v_ebx = 0;
            v_local_54_6 = 10;
            v_local_54_2 = 7;
            v_local_58 = v_mount_scale_mod;
            v_local_42 = v_mount_scale_mod;
            v_local_54_4 = v_ebx;
            v_local_54 = v_ebx;
            v_local_46_2 = v_mount_scale_mod;
            v_local_46 = v_ebx;
            v_local_38 = v_mount_scale_mod;
            v_local_3e_2 = v_mount_scale_mod;
            v_sfx_n = 15;
            v_local_4a_2 = v_ebx;
            v_local_4a = v_ebx;
            v_local_4c = v_ebx;
            v_is_multi_beam = v_mount_scale_mod;
            break;
        case 3:
            v_color = 0;
            v_local_3e = 10;
            v_duration = 0x5A;
            v_local_42_2 = 1;
            v_sfx_sprite_id = 0x37;
            v_local_54_4 = 7;
            v_local_42 = v_local_42_2;
            v_local_54_2 = v_color;
            v_local_54 = v_color;
            v_local_54_6 = v_local_3e;
            v_local_46_2 = v_color;
            v_local_46 = v_color;
            v_local_38 = v_color;
            v_ebx = 0;
            v_mount_scale_mod = 0;
            v_local_3e_2 = v_color;
            v_local_4a_2 = v_local_42_2;
            v_local_4a = v_color;
            v_local_4c = v_color;
            v_sfx_n = v_local_42_2;
            break;
        case 4:
            v_duration = 0x3C;
            v_color = 1;
            v_local_42 = v_color;
            v_local_42_2 = 3;
            v_local_3e = 0x32;
            v_sfx_sprite_id = 0x2D;
            v_ebx = 0;
            v_local_54_6 = 15;
            v_local_54_4 = 7;
            v_local_54_2 = v_local_42_2;
            v_local_54 = v_ebx;
            v_local_46_2 = v_color;
            v_local_46 = v_color;
            v_local_38 = v_color;
            v_mount_scale_mod = v_color;
            v_sfx_n = 23;
            v_local_3e_2 = v_color;
            v_local_4a_2 = v_ebx;
            v_local_4a = v_ebx;
            v_local_4c = v_ebx;
            break;
        case 5:
            v_color = 8;
            v_local_3e = 0x28;
            v_duration = 0x280;
            v_ebx = 0;
            v_local_42_2 = 5;
            v_local_54_6 = 15;
            v_local_54_4 = 10;
            v_local_54_2 = 7;
            v_sfx_sprite_id = v_local_3e;
            v_local_42 = v_ebx;
            v_local_54 = v_ebx;
            v_local_46_2 = 1;
            v_local_46 = 1;
            v_local_38 = 1;
            v_mount_scale_mod = 2;
            v_sfx_n = 9;
            v_local_3e_2 = 1;
            v_local_4a_2 = 1;
            v_local_4a = v_ebx;
            v_local_4c = 1;
            break;
        case 6:
            v_duration = 0x28;
            v_color = 3;
            v_local_42_2 = v_color;
            v_mount_scale_mod = 1;
            v_local_3e = 0x32;
            v_sfx_sprite_id = 0x2D;
            v_local_42 = v_mount_scale_mod;
            v_ebx = 0;
            v_local_54_6 = 15;
            v_local_54_4 = 7;
            v_local_54_2 = v_color;
            v_local_54 = v_ebx;
            v_local_46_2 = v_mount_scale_mod;
            v_local_46 = v_mount_scale_mod;
            v_local_38 = v_mount_scale_mod;
            v_sfx_n = 14;
            v_local_3e_2 = v_mount_scale_mod;
            v_local_4a_2 = v_mount_scale_mod;
            v_local_4a = v_ebx;
            v_local_4c = v_ebx;
            break;
        case 7:
            v_color = 6;
            v_local_42 = 1;
            v_local_3e = 0x1E;
            v_duration = 0x280;
            v_local_54_6 = 7;
            v_local_42_2 = 5;
            v_sfx_sprite_id = 0x23;
            v_ebx = 0;
            v_local_54_2 = 3;
            v_local_54_4 = v_local_54_6;
            v_local_54 = v_ebx;
            v_local_46_2 = v_local_42;
            v_local_46 = v_ebx;
            v_local_38 = v_local_42;
            v_mount_scale_mod = 2;
            v_sfx_n = 11;
            v_local_3e_2 = v_ebx;
            v_local_4a_2 = v_ebx;
            v_local_4a = v_ebx;
            v_local_4c = v_ebx;
            break;
        case 8:
            v_color = 2;
            v_local_42_2 = 3;
            v_duration = 0x5A;
            v_mount_scale_mod = 1;
            v_local_3e = 0x1E;
            v_sfx_sprite_id = 0x2D;
            v_local_54_6 = 15;
            v_ebx = 0;
            v_local_54_4 = 7;
            v_local_42 = v_mount_scale_mod;
            v_local_54_2 = v_local_42_2;
            v_local_54 = v_ebx;
            v_local_46_2 = v_mount_scale_mod;
            v_local_46 = v_mount_scale_mod;
            v_local_38 = v_mount_scale_mod;
            v_sfx_n = 22;
            v_local_3e_2 = v_mount_scale_mod;
            v_local_4a_2 = v_ebx;
            v_local_4a = v_ebx;
            v_local_4c = v_ebx;
            break;
        case 9:
        case 41:
            v_color = 0;
            v_ebx = 0;
            v_duration = 0x5A;
            v_local_3e = 15;
            v_mount_scale_mod = 1;
            v_local_42_2 = 3;
            v_sfx_sprite_id = 0x32;
            v_local_54_4 = 7;
            v_local_54_2 = 6;
            v_local_54 = 2;
            v_local_42 = v_mount_scale_mod;
            v_local_54_6 = v_local_3e;
            v_local_46_2 = v_mount_scale_mod;
            v_local_46 = v_ebx;
            v_local_38 = v_mount_scale_mod;
            v_sfx_n = 19;
            v_local_3e_2 = v_ebx;
            v_local_4a_2 = v_mount_scale_mod;
            v_local_4a = v_ebx;
            v_local_4c = v_ebx;
            break;
        case 10:
            v_color = 0;
            v_ebx = 0;
            v_duration = 0x28;
            v_local_42_2 = 3;
            v_local_3e = v_duration;
            v_mount_scale_mod = 1;
            v_sfx_sprite_id = 0x2D;
            v_local_54_6 = 15;
            v_local_54_4 = 7;
            v_local_42 = v_mount_scale_mod;
            v_local_54_2 = v_local_42_2;
            v_local_54 = v_ebx;
            v_local_46_2 = v_mount_scale_mod;
            v_local_46 = v_mount_scale_mod;
            v_local_38 = v_mount_scale_mod;
            v_sfx_n = 2;
            v_local_3e_2 = v_mount_scale_mod;
            v_local_4a_2 = v_ebx;
            v_local_4a = v_ebx;
            v_local_4c = v_ebx;
            break;
        case 11:
        case 42:
            v_color = 6;
            v_local_42_2 = 3;
            v_duration = 0x5A;
            v_local_42 = 1;
            v_local_3e = 0x28;
            v_sfx_sprite_id = 0x23;
            v_local_54_6 = 15;
            v_ebx = 0;
            v_local_54_4 = 7;
            v_local_54_2 = v_local_42_2;
            v_local_54 = v_ebx;
            v_local_46_2 = v_local_42;
            v_local_46 = v_local_42;
            v_local_38 = v_local_42;
            v_mount_scale_mod = 2;
            v_sfx_n = 21;
            v_local_3e_2 = v_local_42;
            v_local_4a_2 = v_ebx;
            v_local_4a = v_ebx;
            v_local_4c = v_ebx;
            break;
        case 12:
            v_color = 0;
            v_ebx = 0;
            v_duration = 0x3C;
            v_local_3e = 0x32;
            v_sfx_n = 13;
            v_local_42_2 = 5;
            v_local_54_4 = 10;
            v_local_54_2 = 8;
            v_sfx_sprite_id = v_local_3e;
            v_local_42 = 1;
            v_local_54_6 = v_sfx_n;
            v_local_54 = v_ebx;
            v_local_46_2 = v_local_42;
            v_local_46 = v_local_42;
            v_local_38 = v_local_42;
            v_mount_scale_mod = 2;
            v_local_3e_2 = v_local_42;
            v_local_4a_2 = v_local_42;
            v_local_4a = v_ebx;
            v_local_4c = v_ebx;
            break;
        case 13:
            v_mount_scale_mod = 2;
            v_local_42 = 1;
            v_local_3e = 0x1E;
            v_local_42_2 = 3;
            v_sfx_sprite_id = 0x37;
            v_color = v_mount_scale_mod;
            v_local_54_4 = 10;
            v_local_54_6 = 15;
            v_duration = 0x28;
            v_ebx = 0;
            v_local_54_2 = v_local_54_4;
            v_local_54 = v_ebx;
            v_local_46_2 = v_local_42;
            v_local_46 = v_local_42;
            v_local_38 = v_local_42;
            v_local_3e_2 = v_local_42;
            v_local_4a_2 = v_local_42;
            v_local_4a = v_ebx;
            v_local_4c = v_ebx;
            v_sfx_n = v_local_42;
            break;
        case 27:
        case 40:
        case 43:
            v_duration = 0x14;
            v_color = 5;
            v_local_42_2 = v_color;
            v_mount_scale_mod = 1;
            v_local_3e = 0x1E;
            v_sfx_sprite_id = 0x28;
            v_local_42 = v_mount_scale_mod;
            v_local_54 = 0;
            v_local_54_6 = 15;
            v_local_54_4 = 10;
            v_local_54_2 = 7;
            v_local_46_2 = v_mount_scale_mod;
            v_local_46 = v_mount_scale_mod;
            v_local_38 = v_mount_scale_mod;
            v_ebx = v_mount_scale_mod;
            v_local_3e_2 = v_mount_scale_mod;
            v_local_4a_2 = v_mount_scale_mod;
            v_local_4a = v_local_54;
            v_local_4c = v_local_54;
            v_is_multi_beam = v_mount_scale_mod;
            v_sfx_n = v_color;
            break;
        case 46:
            v_color = 6;
            v_local_3e = 10;
            v_duration = 0x28;
            v_local_42 = 1;
            v_local_42_2 = 3;
            v_sfx_sprite_id = 0x32;
            v_local_54_6 = 15;
            v_local_54 = 7;
            v_local_54_4 = 10;
            v_local_54_2 = 10;
            v_local_46_2 = 1;
            v_local_46 = 1;
            v_ebx = 0;
            v_local_38 = 1;
            v_mount_scale_mod = 2;
            v_sfx_n = 18;
            v_local_3e_2 = 1;
            v_local_4a_2 = 1;
            v_local_4a = 1;
            v_local_4c = 0;
            break;
        default:
            return;
        }

        if (!Is_Steve_Stuff_Weapon_(weapon_id)) {
            if (_old_color[fx_slot_idx] != v_color) {
                _old_color[fx_slot_idx] = v_color;
                Add_Weapon_Palette_Fragment_To_Current_Palette_(v_color, (fx_slot_idx + 9) << 4, 16);
            }
            if ((weapon_specials & 2) != 0) {
                v_mount_scale_mod++;
                if (v_mount_scale_mod >= 2) {
                    v_mount_scale_mod = 2;
                }
            }
            if ((weapon_specials & 0x80) != 0) {
                v_sfx_sprite_id *= 2;
                v_duration /= 2;
            }
            if ((weapon_specials & 4) != 0) {
                v_mount_scale_mod--;
                if (v_mount_scale_mod <= 0) {
                    v_mount_scale_mod = 0;
                }
            }
            if ((weapon_specials & 8) != 0) {
                v_local_38 = 1;
            }
            if ((weapon_specials & 0x10) != 0) {
                if (v_duration != 0x280) {
                    v_duration *= 2;
                }
            }
            if ((weapon_specials & 0x20) != 0) {
                v_local_3e_2 = 0;
            }
            if ((weapon_specials & 0x40) != 0) {
                v_local_38 = 1;
            }
        }

        if (v_is_multi_beam != 0 && attacker_size_class != -1) {
            loop_count = 3;
        } else {
            loop_count = 1;
        }

        _g_weapon_n = weapon_id;
        _g_sfx_n = v_sfx_n;

        s_weapon_visual_fx fx;
        s_weapon_visual_fx* pfx = nullptr;

        if (Is_Steve_Stuff_Weapon_(weapon_id)) {
            Build_Steve_Stuff_Struct_For_Weapon_(weapon_id, weapon_specials, &fx);
            v_sfx_sprite_id = static_cast<int16_t>(fx.sfx_sprite_id);
            v_duration = static_cast<int16_t>(fx.duration);
            pfx = &fx;
        }

        if (loop_count > 0) {
            int16_t frame_idx_offset = 0;
            int palette_offset = (fx_slot_idx + 9) << 4;

            do {
                int16_t frame_index = current_frame + frame_idx_offset;

                Draw_Generic_Beam_(
                    attacker_x, attacker_y, target_x, target_y, attacker_facing, target_facing,
                    v_duration, palette_offset, v_local_3e, v_local_42_2, v_sfx_sprite_id, v_local_42,
                    v_local_46_2, v_local_46, v_local_38, v_local_3e_2, v_local_4a_2, v_local_4a,
                    v_local_4c, v_ebx, v_mount_scale_mod, v_local_54_6, v_local_54_4, v_local_54_2,
                    v_local_54, frame_index, target_size_class, hit_result_flags, special_flag,
                    is_non_ship_target, v_local_58, reflection_state, attacker_size_class, pfx
                );

                frame_idx_offset++;
            } while (frame_idx_offset < loop_count);
        }

        if (fire_point_idx == current_frame || current_frame == 0) {
            _g_beam_fire_sfx_handle[fx_slot_idx * 5 + fire_point_idx] = ERICNET::Mox_Play_Sound_(v_sfx_n);
        } else if (_ship_firing_now_flag[fx_slot_idx] == 0 || Get_Max_Frames_() - 1 <= current_frame) {
            sound::Fadeout_Sound_(_g_beam_fire_sfx_handle[fx_slot_idx * 5 + fire_point_idx], 10);
            _g_beam_fire_sfx_handle[fx_slot_idx * 5 + fire_point_idx] = 0;
        }
    }

    void __cdecl Beam_SFX_(int16_t attacker_ship_id, int16_t attacker_x, int16_t attacker_y, int16_t target_id, int16_t target_x, int16_t target_y, s_beam_fx_entry* param_7, int16_t is_non_ship_target) {
        int16_t current_fx_idx[2] = { 0, -1 };
        int16_t frame_idx[2] = { 0, 0 };
        int16_t rand_offsets[6] = { 0, 0, 0, 0, 0, 0 };
        int16_t special_timer[2] = { 0, 0 };
        int16_t fx_idx = 0;

        uint32_t seed = random::Get_Random_Seed_();
        int16_t ship_range = Get_Ship_Range_(attacker_x, attacker_y, target_x, target_y);

        _g_pierced_shield_flag = 0;
        palette::Restore_Glass_Remap_Block_(MOX::_shield_remap_colors);
        Shield_Anims_(0, 0, 1, 0);
        Reset_Beam_Swapping_Colors_();

        if (param_7[0].weapon_id == 0x2e || TECHDATA::_weapons[param_7[0].weapon_id].type == 0) {
            int16_t attacker_facing_dir = MOX::_combat_data[attacker_ship_id].facing_dir;
            int16_t target_facing_dir = MOX::_combat_data[target_id].facing_dir;

            int16_t attacker_center_x, attacker_center_y;
            COMBINIT::Ship_Center_Offsets_(attacker_ship_id, &attacker_center_x, &attacker_center_y);
            attacker_center_x += attacker_x;
            attacker_center_y += attacker_y;

            int16_t target_center_x, target_center_y;
            if (is_non_ship_target == 0) {
                COMBINIT::Ship_Center_Offsets_(target_id, &target_center_x, &target_center_y);
                target_center_x += target_x;
                target_center_y += target_y;
            }
            else {
                target_center_x = target_x;
                target_center_y = target_y;
            }

            for (int32_t i = 0; i < 10; ++i) {
                _g_beam_fire_sfx_handle[i] = 0;
            }

            if ((param_7[0].specials & 0x80) != 0) {
                special_timer[0] = 3;
            }

            bool bExitLoop = false;
            do {
                int16_t has_reflected = 0;
                timer::Mark_Time_();
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                graphics::Set_Window_(0, 0, 0x27f, 0x15e);
                video::Clip_On_();

                if (MOX::_dont_draw_ship == target_id && is_non_ship_target == 0) {
                    CMBTDRW1::Draw_Ship_(target_id, target_x, target_y);
                }
                if (MOX::_dont_draw_ship == attacker_ship_id) {
                    CMBTDRW1::Draw_Ship_(attacker_ship_id, attacker_x, attacker_y);
                }

                for (int16_t i = 0; i < 2; ++i) {
                    int16_t current_fx = current_fx_idx[i];
                    if (current_fx >= 0) {
                        s_beam_fx_entry* fx = &param_7[current_fx];
                        int16_t weapon_id = fx->weapon_id;
                        int16_t shot_count = fx->shot_count;
                        int16_t attacker_picture_num = MOX::_combat_data[attacker_ship_id].picture_num;
                        int16_t attacker_owner = MOX::_combat_data[attacker_ship_id].owner;

                        int16_t target_bearing;
                        if (is_non_ship_target == 0) {
                            target_bearing = CMBTDRW1::Relative_Bearing_(attacker_ship_id, target_id);
                        }
                        else {
                            target_bearing = CMBTDRW1::Relative_Bearing_XY_(attacker_ship_id, target_x, target_y);
                        }
                        target_bearing = CMBTFIRE::Facing_Shield_(target_bearing);

                        int16_t fp_count;
                        if (attacker_ship_id <= 0 || attacker_owner >= 8 || _g_reflection_state_flag == 2) {
                            fp_count = 1;
                        }
                        else {
                            fp_count = FIREPTS::N_Fire_Points_(weapon_id, shot_count);
                        }

                        if (frame_idx[i] == 0) {
                            bool apply_random = false;
                            if (fx->result_flags == 0 && fx->special_flag == 0 && fx->is_damage_entry == 0) {
                                if (target_id != 0) {
                                    if (is_non_ship_target == 0) {
                                        int16_t shield_size = Ship_Size_To_Shield_Size_(MOX::_combat_data[target_id].size_class);
                                        int16_t max_radius = ship_range / 2;
                                        int16_t shield_radius = _shield_diameters[shield_size] / 2;
                                        if (shield_radius <= max_radius) {
                                            max_radius = shield_radius;
                                        }
                                        rand_offsets[i + 2] = (int16_t)random::Random_(max_radius) - max_radius / 2;
                                        rand_offsets[i] = (int16_t)random::Random_(max_radius) - max_radius / 2;
                                    }
                                    else {
                                        apply_random = true;
                                    }
                                }
                                else if (fx->accum_hit_x == 0) {
                                    int16_t max_radius = ship_range / 2;
                                    if (max_radius > 30) max_radius = 30;
                                    int16_t radius = (max_radius + (max_radius >> 31 & 3)) / 4;
                                    int16_t base = max_radius / 2;
                                    rand_offsets[i + 2] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i + 2] *= (int16_t)(random::Random_(2) - 1);
                                    rand_offsets[i] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i] *= (int16_t)(random::Random_(2) - 1);
                                }
                                else {
                                    apply_random = true;
                                }
                            }
                            else {
                                if (target_id == 0) {
                                    int16_t max_radius = ship_range / 2;
                                    if (max_radius > 30) max_radius = 30;
                                    int16_t radius = (max_radius + (max_radius >> 31 & 3)) / 4;
                                    int16_t base = max_radius / 2;
                                    rand_offsets[i + 2] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i + 2] *= (int16_t)(random::Random_(2) - 1);
                                    rand_offsets[i] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i] *= (int16_t)(random::Random_(2) - 1);
                                }
                                else {
                                    apply_random = true;
                                }
                            }

                            if (apply_random) {
                                if (is_non_ship_target == 0 || fx->accum_hit_x != 0) {
                                    rand_offsets[i + 2] = 0;
                                    rand_offsets[i] = 0;
                                }
                                else {
                                    int16_t max_radius = ship_range / 2;
                                    int16_t shield_radius = _shield_diameters[0] / 2;
                                    if (max_radius <= shield_radius) {
                                        shield_radius = max_radius;
                                    }
                                    int16_t radius = (shield_radius + (shield_radius >> 31 & 3)) / 4 + 1;
                                    int16_t base = shield_radius / 2;
                                    rand_offsets[i + 2] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i + 2] *= (int16_t)(random::Random_(2) - 1);
                                    rand_offsets[i] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i] *= (int16_t)(random::Random_(2) - 1);
                                }
                            }
                        }

                        if (fp_count > 0) {
                            for (int32_t fp_idx_val = 0; fp_idx_val < fp_count; ++fp_idx_val) {
                                int16_t fp_x = 0, fp_y = 0;
                                if (attacker_ship_id > 0) {
                                    FIREPTS::Calc_FP_Offsets_(&fp_x, &fp_y, weapon_id, shot_count,
                                        attacker_owner, attacker_ship_id,
                                        attacker_picture_num, (uint8_t)attacker_facing_dir,
                                        (uint8_t)target_bearing, fp_idx_val);
                                }
                                Fire_Weapon_(attacker_center_x + fp_x, attacker_center_y + fp_y,
                                    target_center_x + rand_offsets[i + 2],
                                    target_center_y + rand_offsets[i],
                                    attacker_facing_dir, target_facing_dir,
                                    fx->weapon_id, frame_idx[i], i,
                                    MOX::_combat_data[target_id].size_class,
                                    fx->result_flags, fx->special_flag,
                                    is_non_ship_target, fp_idx_val,
                                    _g_reflection_state_flag,
                                    MOX::_combat_data[attacker_ship_id].size_class,
                                    fx->specials, 1);
                            }
                        }
                    }

                    bool skip_frame_increment = false;
                    if (current_fx < 16) {
                        if (current_fx <= -1) {
                            int16_t other_i = (i == 0) ? 1 : 0;
                            int16_t other_fx = current_fx_idx[other_i];
                            if (other_fx != -1 && other_fx < 16) {
                                if (param_7[other_fx].weapon_id <= 0 || frame_idx[other_i] >= 9) {
                                    skip_frame_increment = true;
                                }
                            }
                        }
                        else {
                            int16_t max_frames = Get_Max_Frames_();
                            if (frame_idx[i] >= max_frames) {
                                skip_frame_increment = true;
                            }
                        }

                        if (skip_frame_increment) {
                            frame_idx[i] = 0;
                            special_timer[i]--;
                            if (special_timer[i] <= 0) {
                                special_timer[i] = 0;
                                s_beam_fx_entry* next_fx = &param_7[fx_idx + 1];
                                if (next_fx->weapon_id <= 0 || fx_idx + 1 >= 16) {
                                    current_fx_idx[i] = (int16_t)0xffff;
                                }
                                else {
                                    if (next_fx->is_damage_entry == 0) {
                                        fx_idx++;
                                        current_fx_idx[i] = fx_idx;
                                        if (param_7[fx_idx + 1].is_damage_entry != 0) {
                                            _g_reflection_state_flag = 1;
                                            _reflection_field_display_status = 1;
                                        }
                                        if ((param_7[fx_idx].specials & 0x80) != 0) {
                                            special_timer[i] = 3;
                                        }
                                    }
                                    else {
                                        int16_t other_i = (i == 0) ? 1 : 0;
                                        if (current_fx_idx[other_i] == (int16_t)0xfc18 &&
                                            (_ship_hit_now_flag[other_i] != 0 || current_fx_idx[i] <= -1)) {
                                            _shield_hit_now_flag = 0;
                                            _shield_display_status = 0;

                                            s_beam_fx_entry reflected_fx[16];
                                            struct_::Clear_Structure_(reflected_fx, sizeof(reflected_fx));

                                            reflected_fx[0].weapon_id = next_fx->weapon_id;
                                            reflected_fx[0].shot_count = next_fx->shot_count;
                                            reflected_fx[0].result_flags = next_fx->result_flags;
                                            reflected_fx[0].special_flag = next_fx->special_flag;
                                            reflected_fx[0].accum_hit_x = next_fx->accum_hit_x;
                                            reflected_fx[0].accum_hit_y = next_fx->accum_hit_y;
                                            reflected_fx[0].specials = next_fx->specials;
                                            reflected_fx[0].is_damage_entry = next_fx->is_damage_entry;
                                            reflected_fx[1].weapon_id = 0;
                                            reflected_fx[1].is_damage_entry = 0;

                                            _g_firing_reflected_beam_flag = 1;
                                            _g_reflection_state_flag = 2;
                                            Beam_SFX_(target_id, target_x, target_y, attacker_ship_id, attacker_x, attacker_y, reflected_fx, 0);
                                            _reflection_field_display_status = 0;
                                            _g_firing_reflected_beam_flag = 0;

                                            fx_idx++;
                                            has_reflected = 1;
                                            current_fx_idx[other_i] = (int16_t)0xfc18;

                                            timer::Mark_Time_();
                                            video::Set_Page_Off_();
                                            CMBTDRW1::Draw_Main_Combat_Screen_();
                                            graphics::Set_Window_(0, 0, 0x27f, 0x15e);
                                            video::Clip_On_();
                                            if (MOX::_dont_draw_ship == target_id && is_non_ship_target == 0) {
                                                CMBTDRW1::Draw_Ship_(target_id, target_x, target_y);
                                            }
                                            if (MOX::_dont_draw_ship == attacker_ship_id) {
                                                CMBTDRW1::Draw_Ship_(attacker_ship_id, attacker_x, attacker_y);
                                            }
                                        }
                                        else {
                                            current_fx_idx[i] = (int16_t)0xfc18;
                                            if (next_fx->is_damage_entry != 0) {
                                                _g_reflection_state_flag = 1;
                                                _reflection_field_display_status = 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (!skip_frame_increment) {
                        if (MOX::_speedx2_flag != 0 || MOX::_net_flag != 0) {
                            if (frame_idx[i] + 2 < Get_Max_Frames_()) {
                                frame_idx[i] += 2;
                            }
                            else {
                                frame_idx[i]++;
                            }
                        }
                        else {
                            frame_idx[i]++;
                        }
                    }
                } // for i loop

                if (current_fx_idx[0] <= -1 && current_fx_idx[1] <= -1) {
                    _shield_hit_now_flag = 0;
                    _reflection_field_hit_now_flag = 0;
                }

                if (current_fx_idx[0] <= -1 && current_fx_idx[1] <= -1 &&
                    _shield_display_status == 0 && _reflection_field_display_status == 0) {
                    if (Done_Drawing_Damage_Message_Queue_() != 0 && has_reflected == 0) {
                        bExitLoop = true;
                        _g_reflection_state_flag = 0;
                    }
                }

                if (is_non_ship_target == 0) {
                    Adjust_Shield_Display_Status_(&_shield_hit_draw_facing);
                    uint8_t shield_special = 0;
                    if (current_fx_idx[0] > -1) {
                        shield_special = (uint8_t)param_7[current_fx_idx[0]].special_flag;
                    }
                    if (current_fx_idx[1] > -1) {
                        shield_special = (uint8_t)(shield_special + (uint8_t)param_7[current_fx_idx[0] == 0].special_flag);
                    }
                    if (_g_pierced_shield_flag != 0) {
                        shield_special = 1;
                    }

                    if (target_id == 0) {
                        int16_t colony_id = MOX::_cmbt_colony_id;
                        int16_t planet_idx = MOX::_colony[colony_id].planet;
                        int16_t planet_size = MOX::_planet[planet_idx].size;
                        int16_t shield_size = Planet_Size_To_Shield_Size_(planet_size);
                        int16_t shield_diam = _shield_diameters[shield_size];
                        int16_t draw_facing = _shield_hit_draw_facing;
                        int16_t hit_facing = Get_SFX_Draw_Shield_Facing2_(attacker_ship_id, target_id);
                        int16_t max_radius = shield_diam / 2;
                        int16_t dx = target_center_x - max_radius;
                        int16_t dy = target_center_y - max_radius;
                        Draw_Shield_Hit_(dx, dy, target_id, shield_special != 0, 0, hit_facing, (int16_t)draw_facing);
                    }
                    else {
                        int16_t shield_size = Ship_Size_To_Shield_Size_(MOX::_combat_data[target_id].size_class);
                        int16_t shield_diam = _shield_diameters[shield_size];
                        int16_t specials = 0;
                        if (current_fx_idx[0] > -1) {
                            specials = param_7[current_fx_idx[0]].specials & 0x100;
                        }
                        int16_t draw_facing = _shield_hit_draw_facing;
                        int16_t hit_facing = Get_SFX_Draw_Shield_Facing2_(attacker_ship_id, target_id);
                        int16_t max_radius = shield_diam / 2;
                        int16_t dx = target_center_x - max_radius;
                        int16_t dy = target_center_y - max_radius;
                        Draw_Shield_Hit_(dx, dy, target_id, shield_special != 0, specials, hit_facing, (int16_t)draw_facing);
                    }

                    if (_reflection_field_display_status != 0) {
                        if (_g_reflection_state_flag == 2) {
                            Adjust_Reflection_Field_Display_Status_(&_reflection_field_hit_draw_facing);
                            int16_t refl_center_x, refl_center_y;
                            COMBINIT::Ship_Center_Offsets_(attacker_ship_id, &refl_center_x, &refl_center_y);
                            refl_center_x += attacker_x;
                            refl_center_y += attacker_y;
                            int16_t shield_size = Ship_Size_To_Shield_Size_(MOX::_combat_data[attacker_ship_id].size_class);
                            int16_t refl_diam = _reflection_field_diameters[shield_size];
                            int16_t draw_facing = _reflection_field_hit_draw_facing;
                            int16_t hit_facing = Get_SFX_Draw_Shield_Facing2_(target_id, attacker_ship_id);
                            int16_t refl_radius = refl_diam / 2;
                            Draw_Reflection_Field_Hit_(refl_center_x - refl_radius, refl_center_y - refl_radius, MOX::_combat_data[attacker_ship_id].size_class, hit_facing, draw_facing);
                        }
                        else if (_g_reflection_state_flag == 1) {
                            Adjust_Reflection_Field_Display_Status_(&_reflection_field_hit_draw_facing);
                            int16_t shield_size = Ship_Size_To_Shield_Size_(MOX::_combat_data[target_id].size_class);
                            int16_t refl_diam = _reflection_field_diameters[shield_size];
                            int16_t refl_center_x, refl_center_y;
                            COMBINIT::Ship_Center_Offsets_(target_id, &refl_center_x, &refl_center_y);
                            refl_center_x += target_x;
                            refl_center_y += target_y;
                            int16_t draw_facing = _reflection_field_hit_draw_facing;
                            int16_t hit_facing = Get_SFX_Draw_Shield_Facing2_(attacker_ship_id, target_id);
                            int16_t refl_radius = refl_diam / 2;
                            Draw_Reflection_Field_Hit_(refl_center_x - refl_radius, refl_center_y - refl_radius, MOX::_combat_data[target_id].size_class, hit_facing, draw_facing);
                        }
                    }
                }
                else {
                    _shield_display_status = 0;
                    _reflection_field_display_status = 0;
                }

                for (int16_t i = 0; i < 2; ++i) {
                    if (is_non_ship_target == 0) {
                        int16_t fx_idx_val = current_fx_idx[i];
                        if (fx_idx_val > -1 && _ship_hit_now_flag[i] != 0) {
                            s_beam_fx_entry* fx = &param_7[fx_idx_val];
                            if (fx->accum_hit_x > 0 || fx->accum_hit_y > 0) {
                                uint8_t hit_facing = (uint8_t)Do_Get_SFX_Draw_Shield_Facing_(attacker_ship_id, target_id);
                                Add_Damage_Indicator_For_Ship_(target_x, target_y, target_id, hit_facing, fx->accum_hit_x, fx->accum_hit_y);
                                ERICNET::Mox_Play_Sound_(0x1d);
                                fx->accum_hit_x = -1;
                                fx->accum_hit_y = -1;
                            }
                        }
                    }
                    else {
                        int16_t fx_idx_val = current_fx_idx[i];
                        if (fx_idx_val > -1 && _ship_hit_now_flag[i] != 0) {
                            s_beam_fx_entry* fx = &param_7[fx_idx_val];
                            if (fx->accum_hit_x > 0 || fx->accum_hit_y > 0) {
                                Add_Damage_Indicator_For_Missile_(target_center_x, target_center_y, 1, fx->accum_hit_x);
                                MOX::_missile[target_id].quantity -= fx->accum_hit_x;
                                if (MOX::_missile[target_id].quantity <= 0) {
                                    CMBTMIS::Kill_Missile_(target_id);
                                }
                                if (fx->accum_hit_x != 0) {
                                    ERICNET::Mox_Play_Sound_(0x1c);
                                }
                                fx->accum_hit_x = -1;
                            }
                        }
                    }
                }

                Draw_Damage_Message_Queue_();
                timer::Release_Time_(1);
                RUSS::Mox_Sync_Update_();
                video::Clip_Off_();
                graphics::Reset_Window_();
            } while (!bExitLoop);

            _ship_hit_now_flag[0] = 0;
            _ship_hit_now_flag[1] = 0;
            _ship_firing_now_flag[0] = 0;
            _ship_firing_now_flag[1] = 0;
            Reset_Beam_SFX_Handles_();
            _shield_display_status = 0;
            _reflection_field_display_status = 0;
            video::Clip_Off_();
            random::Set_Random_Seed_(seed);
        }
    }

    void __cdecl Draw_Ship_Burst_(int16_t x, int16_t y, int16_t dir, int16_t base_damage_class, int16_t frame, int16_t is_palette_not_90) {
        s_animation_header* anim = _laser_burst_anim[base_damage_class][is_palette_not_90][dir];
        animate::Set_Animation_Frame_(anim, frame % 3);
        ERIC::Draw_Centered_(x, y, anim);
    }

    void __cdecl Reset_Beam_SFX_Handles_() {
        _g_shield_sfx_handle = 0;
        memset(_g_beam_fire_sfx_handle, 0, sizeof(_g_beam_fire_sfx_handle));
    }

    void __cdecl Draw_Reflection_Field_Hit_(int16_t x, int16_t y, int16_t ship_size, int16_t rotation, int16_t frame) {
        int16_t base_rot = Rot_To_Base_Rot_(rotation);
        int16_t rot_method = Rot_To_Rot_Method_(rotation);
        int16_t shield_size = Ship_Size_To_Shield_Size_(ship_size);

        uint32_t diameter = _shield_diameters[shield_size];
        uint32_t size = (diameter * diameter) + 0x20;

        allocate::Push_Block_(MOX::_screen_seg);
        s_animation_header* h1 = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size);
        s_animation_header* psVar5 = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size);

        if (_reflection_field_display_status == 0) {
            allocate::Pop_Block_(MOX::_screen_seg);
            return;
        }

        if (frame == 0 && _g_reflection_field_sfx_handle == 0) {
            int16_t sfx_id = (_g_reflection_state_flag == 0) ? 0x26 : 0x2A;
            _g_reflection_field_sfx_handle = ERICNET::Mox_Play_Sound_(sfx_id);
            sound::Pitch_Bend_Sound_((int32_t)_g_reflection_field_sfx_handle, ship_size * -2500);
        } else {
            s_animation_header* anim = Reflection_Field_Anims_(base_rot, shield_size);
            if ((animate::Get_Animation_Frame_Count_(anim) - 1) <= frame) {
                sound::Fadeout_Sound_(_g_reflection_field_sfx_handle, 10);
                _g_reflection_field_sfx_handle = 0;
            }
        }

        s_animation_header* anim_frames = Reflection_Field_Anims_(base_rot, shield_size);
        int16_t frame_count = animate::Get_Animation_Frame_Count_(anim_frames);
        int16_t actual_frame = frame % frame_count;

        s_animation_header* anim_to_set = Reflection_Field_Anims_(base_rot, shield_size);
        animate::Set_Animation_Frame_(anim_to_set, actual_frame);

        s_animation_header* anim_to_draw = Reflection_Field_Anims_(base_rot, shield_size);
        bitmap::Draw_Picture_To_Bitmap_(h1, anim_to_draw);

        s_animation_header* draw_header = nullptr;
        switch (rot_method) {
            case 0:
                bitmap::Set_Picture_Glassed_(h1);
                draw_header = h1;
                break;
            case 1:
                bitmap::Flip_Bitmap_(psVar5, h1, 0);
                bitmap::Set_Picture_Glassed_(psVar5);
                draw_header = psVar5;
                break;
            case 2:
                bitmap::Flip_Bitmap_(psVar5, h1, 3);
                bitmap::Flip_Bitmap_(h1, psVar5, 0);
                bitmap::Set_Picture_Glassed_(h1);
                draw_header = h1;
                break;
            case 3:
                bitmap::Flip_Bitmap_(psVar5, h1, 3);
                bitmap::Set_Picture_Glassed_(psVar5);
                draw_header = psVar5;
                break;
            default:
                break;
        }

        if (draw_header != nullptr) {
            animate::Draw_(x, y, draw_header);
        }

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    /**
     * @address 00423aa0
     */
    void __cdecl Adjust_Reflection_Field_Display_Status_(int16_t* param_1) {
        int16_t frame_count = animate::Get_Animation_Frame_Count_(Reflection_Field_Anims_(0, 0));

        if (_g_reflection_state_flag == 0 && _reflection_field_display_status != 0) {
            _reflection_field_display_status = 0;
            return;
        }

        switch (_reflection_field_display_status) {
            case 0:
                if (_reflection_field_hit_now_flag != 0 || _g_reflection_state_flag >= 1) {
                    _reflection_field_display_status = 1;
                    *param_1 = 0;
                }
                break;
            case 1:
                (*param_1)++;
                if (*param_1 >= 4) {
                    _reflection_field_display_status = 2;
                }
                break;
            case 2:
                (*param_1)++;
                if (_reflection_field_hit_now_flag != 0) {
                    if (*param_1 >= 7) {
                        *param_1 = 4;
                    }
                } else {
                    if (*param_1 >= 7) {
                        _reflection_field_display_status = 3;
                    }
                }
                break;
            case 3:
                (*param_1)++;
                if (frame_count <= *param_1) {
                    _reflection_field_display_status = 0;
                    _g_reflection_state_flag = 0;
                }
                break;
        }
    }

    void __cdecl Build_Steve_Stuff_Struct_For_Weapon_(int16_t param_1, uint16_t param_2, s_weapon_visual_fx* param_3) {
        if (param_1 <= 0 || param_1 > 46) {
            memset(param_3, 0, sizeof(*param_3));
            return;
        }

        s_weapon_visual_fx local_2c;

        switch (param_1) {
            case 1:
                Set_Weapon_Visual_FX_Colors_(&local_2c, 0x59, 0x6b, 0xc3, 0xda, 0xfc, 0xff);
                local_2c.thickness = 1;
                local_2c.anim_style = 1;
                local_2c.sfx_sprite_id = 4;
                local_2c.duration = 20;
                local_2c.unk = 0;
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 2:
                Set_Weapon_Visual_FX_Colors_(&local_2c, 0x59, 0x67, 0x6c, 0xb2, 0xc8, 0xc8);
                local_2c.thickness = 1;
                local_2c.anim_style = 1;
                local_2c.sfx_sprite_id = 6;
                local_2c.duration = 20;
                local_2c.unk = 0;
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 3:
                if ((param_2 & 0x10) == 0) {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0xc8, 0x14, 0x20, 0xff, 0x50, 0x0a);
                    local_2c.thickness = 1;
                    local_2c.anim_style = 0;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                } else {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0xc8, 0x14, 0x20, 0xff, 0x50, 0x0a);
                    local_2c.thickness = 1;
                    local_2c.anim_style = 2;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                }
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 4:
                if ((param_2 & 0x10) == 0) {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0x5e, 0x70, 0xcd, 0xba, 0xdd, 0xff);
                    local_2c.thickness = 0;
                    local_2c.anim_style = 0;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                } else {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0x5e, 0x70, 0xcd, 0xba, 0xdd, 0xff);
                    local_2c.thickness = 0;
                    local_2c.anim_style = 2;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                }
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 5:
                if (((param_2 & 0x10) != 0) && ((param_2 & 0x100) != 0)) {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0xd2, 0x46, 0x00, 0xff, 0xa2, 0x00);
                    local_2c.thickness = 1;
                    local_2c.anim_style = 3;
                    local_2c.sfx_sprite_id = 100;
                    local_2c.duration = 25;
                    local_2c.unk = 1;
                } else if ((param_2 & 0x10) != 0) {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0xd2, 0x46, 0x00, 0xff, 0xa2, 0x00);
                    local_2c.thickness = 1;
                    local_2c.anim_style = 2;
                    local_2c.sfx_sprite_id = 120;
                    local_2c.duration = 30;
                    local_2c.unk = 0;
                } else if ((param_2 & 0x100) != 0) {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0xd2, 0x46, 0x00, 0xff, 0xa2, 0x00);
                    local_2c.thickness = 1;
                    local_2c.anim_style = 1;
                    local_2c.sfx_sprite_id = 100;
                    local_2c.duration = 25;
                    local_2c.unk = 1;
                } else {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0xd2, 0x46, 0x00, 0xff, 0xa2, 0x00);
                    local_2c.thickness = 1;
                    local_2c.anim_style = 1;
                    local_2c.sfx_sprite_id = 120;
                    local_2c.duration = 30;
                    local_2c.unk = 0;
                }
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 6:
                if ((param_2 & 0x10) == 0) {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0x94, 0x9b, 0x49, 0xfe, 0xfe, 0x94);
                    local_2c.thickness = 2;
                    local_2c.anim_style = 1;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                } else {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0x94, 0x9b, 0x49, 0xfe, 0xfe, 0x94);
                    local_2c.thickness = 2;
                    local_2c.anim_style = 2;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                }
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 7:
                if ((param_2 & 0x10) == 0) {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0x44, 0x98, 0x8f, 0x93, 0xff, 0xff);
                    local_2c.thickness = 2;
                    local_2c.anim_style = 3;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                } else {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0x44, 0x98, 0x8f, 0x93, 0xff, 0xff);
                    local_2c.thickness = 2;
                    local_2c.anim_style = 3;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                }
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 8:
                if ((param_2 & 0x10) == 0) {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0x92, 0x53, 0xba, 0xff, 0xb8, 0xff);
                    local_2c.thickness = 1;
                    local_2c.anim_style = 1;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                } else {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0x92, 0x53, 0xba, 0xff, 0xb8, 0xff);
                    local_2c.thickness = 1;
                    local_2c.anim_style = 2;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                }
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 9:
            case 41:
                if ((param_2 & 0x10) == 0) {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0xa9, 0x2b, 0x00, 0xff, 0xa6, 0x60);
                    local_2c.thickness = 1;
                    local_2c.anim_style = 0;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                } else {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0xa9, 0x2b, 0x00, 0xff, 0xa6, 0x60);
                    local_2c.thickness = 1;
                    local_2c.anim_style = 2;
                    local_2c.sfx_sprite_id = 200;
                    local_2c.duration = 50;
                    local_2c.unk = 0;
                }
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 10:
                Set_Weapon_Visual_FX_Colors_(&local_2c, 0x6b, 0xa2, 0x16, 0xf2, 0xff, 0x35);
                local_2c.thickness = 1;
                local_2c.anim_style = 1;
                local_2c.sfx_sprite_id = 40;
                local_2c.duration = 30;
                local_2c.unk = 0;
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 11:
            case 42:
                Set_Weapon_Visual_FX_Colors_(&local_2c, 0xc9, 0x53, 0xc4, 0xff, 0xb8, 0xff);
                local_2c.thickness = 1;
                local_2c.anim_style = 3;
                local_2c.sfx_sprite_id = 200;
                local_2c.duration = 50;
                local_2c.unk = 0;
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 12:
                if ((param_2 & 0x10) == 0) {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0xd2, 0x53, 0x00, 0xff, 0xff, 0x52);
                    local_2c.thickness = 2;
                    local_2c.anim_style = 1;
                    local_2c.sfx_sprite_id = 100;
                    local_2c.duration = 30;
                    local_2c.unk = 1;
                } else {
                    Set_Weapon_Visual_FX_Colors_(&local_2c, 0xd2, 0x53, 0x00, 0xff, 0xff, 0x52);
                    local_2c.thickness = 2;
                    local_2c.anim_style = 3;
                    local_2c.sfx_sprite_id = 100;
                    local_2c.duration = 30;
                    local_2c.unk = 1;
                }
                memcpy(param_3, &local_2c, sizeof(local_2c));
                break;

            case 27:
            case 40:
            case 43:
            case 46:
                memset(param_3, 0, sizeof(*param_3));
                break;

            default:
                memset(param_3, 0, sizeof(*param_3));
                return;
        }
    }

    int8_t __cdecl Is_Steve_Stuff_Weapon_(int16_t weapon_id) {
        switch (weapon_id) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 41:
            case 42:
                return 1;
            case 13:
            case 27:
            case 40:
            case 43:
            case 46:
                return 0;
            default:
                return 0;
        }
    }

    /**
     * @address 0x0041ffa0
     */
    void __cdecl Rob_Interpolate_Line_(int16_t* p_x, int16_t* p_y, int16_t target_x, int16_t target_y, int16_t step) {
        int16_t start_x = *p_x;
        int16_t start_y = *p_y;
        int32_t low_x = 0;
        int32_t low_y = 0;

        int32_t dx = target_x - start_x;
        int32_t dy = target_y - start_y;

        if (step < 1) {
            return;
        }

        int32_t max_dist = abs(dx);
        if (abs(dy) > max_dist) {
            max_dist = abs(dy);
        }

        if (max_dist <= step) {
            *p_x = target_x;
            *p_y = target_y;
            return;
        }

        if (dx == 0) {
            if (dy <= 0) {
                *p_y = start_y - step;
            } else {
                *p_y = start_y + step;
            }
            return;
        }

        if (dy == 0) {
            if (dx <= 0) {
                *p_x = start_x - step;
            } else {
                *p_x = start_x + step;
            }
            return;
        }

        int32_t high_x = dx;
        int32_t high_y = dy;
        int32_t cur_x = dx >> 1;
        int32_t cur_y = dy >> 1;

        while (true) {
            int32_t cur_dist = abs(cur_x);
            if (abs(cur_y) > cur_dist) {
                cur_dist = abs(cur_y);
            }

            if (cur_dist == step) {
                break;
            }

            if (cur_dist < step) {
                low_x = cur_x;
                low_y = cur_y;
                cur_x += (high_x - cur_x) / 2;
                cur_y += (high_y - cur_y) / 2;
            } else {
                high_x = cur_x;
                high_y = cur_y;
                cur_x += (low_x - cur_x) / 2;
                cur_y += (low_y - cur_y) / 2;
            }
        }

        *p_x = start_x + cur_x;
        *p_y += (int16_t)cur_y;
    }

    /**
     * @address 00422e40
     */
    int8_t __cdecl Do_Get_SFX_Draw_Shield_Facing_(int16_t attacker_id, int16_t target_id) {
        int32_t facing = Get_SFX_Draw_Shield_Facing2_(target_id, attacker_id);
        return ((facing + 4) % 16) < 8;
    }

    /**
     * @address 00422ed0
     */
    int16_t __cdecl Get_SFX_Draw_Shield_Facing2_(int16_t ship1_idx, int16_t ship2_idx) {
        int16_t p1_x_offset;
        int16_t p1_y_offset;
        int16_t p2_x_offset;
        int16_t p2_y_offset;

        COMBINIT::Ship_Center_Offsets_(ship1_idx, &p1_x_offset, &p1_y_offset);
        COMBINIT::Ship_Center_Offsets_(ship2_idx, &p2_x_offset, &p2_y_offset);

        int16_t x1 = MOX::_combat_data[ship1_idx].x * 20 + p1_x_offset;
        int16_t y1 = MOX::_combat_data[ship1_idx].y * 20 + p1_y_offset;
        int16_t x2 = MOX::_combat_data[ship2_idx].x * 20 + p2_x_offset;
        int16_t y2 = MOX::_combat_data[ship2_idx].y * 20 + p2_y_offset;

        int16_t angle = angle::Get_Angle_(x1 - x2, y2 - y1);

        return (angle / 22) % 16;
    }

    /**
     * @address 00420ce0
     */
    void __cdecl Add_Weapon_Palette_Fragment_To_Current_Palette_(int16_t source_fragment, int16_t dest_start, int16_t count)
    {
        if (count > 0) {
            for (int16_t i = 0; i < count; i++) {
                uint8_t red;
                uint8_t green;
                uint8_t blue;

                remap::Get_Palette_Animation_Color_(
                    _l_anims,
                    (source_fragment * 16) + i,
                    &red,
                    &green,
                    &blue
                );

                palstore::Store_Single_Palette_Color_(
                    dest_start + i,
                    red,
                    green,
                    blue
                );
            }
        }
    }

    /**
     * @address 00420110
     */
    void __cdecl Draw_Generic_Beam_(
        int16_t attacker_x, int16_t attacker_y, int16_t target_x, int16_t target_y, int16_t attacker_facing,
        int16_t target_facing, int16_t duration_frames, int16_t palette_base_index, int16_t beam_length,
        int16_t beam_width, int16_t sprite_id, int16_t draw_core_flag, int16_t draw_glow_flag,
        int16_t draw_particles_flag, int16_t is_shield_piercing, int16_t is_autofiring, int16_t param_17,
        int16_t param_18, int16_t param_19, int16_t param_20, int16_t mount_scale_mod, int16_t fx_param_1,
        int16_t fx_param_2, int16_t fx_param_3, int16_t fx_param_4, int16_t frame_index,
        int16_t target_size_class, uint16_t hit_result_flags, uint16_t special_flag,
        int16_t is_non_ship_target, int16_t base_damage_class, int16_t reflection_state,
        int16_t attacker_size_class, s_weapon_visual_fx* param_34
    ) {
        int16_t angle = angle::Get_Angle_(attacker_x - target_x, target_y - attacker_y);
        _ship_hit_now_flag[0] = 0;
        _ship_hit_now_flag[1] = 0;

        int16_t beam_dir = ((angle / 22) + 8) % 16;
        if (beam_dir < 0) {
            beam_dir = -beam_dir;
        }

        int16_t is_palette_not_90 = (palette_base_index != 0x90) ? 1 : 0;
        int16_t ship_range = Get_Ship_Range_(attacker_x, attacker_y, target_x, target_y);

        if (attacker_size_class == -1) {
            mount_scale_mod = 0;
            if (duration_frames > 20) {
                duration_frames = 20;
            }
            base_damage_class = 1;
            sprite_id = 5;
        }

        if (param_34 != nullptr) {
            duration_frames = static_cast<int16_t>(param_34->sfx_sprite_id);
            sprite_id = static_cast<int16_t>(param_34->duration);
        }

        int16_t eff_duration = duration_frames;
        if (duration_frames < 60) {
            eff_duration = 60;
        }
        if (ship_range < eff_duration) {
            eff_duration = ship_range;
        }

        int32_t min_frames = (attacker_size_class == -1) ? 2 : 3;
        int32_t frames = (int32_t)(ship_range + eff_duration) / sprite_id;

        if (frames < min_frames) {
            frames = min_frames;
        }
        if (frames > 6) {
            sprite_id *= 2;
        }

        frames = (int32_t)(ship_range + eff_duration) / sprite_id;
        if (frames < min_frames) {
            frames = min_frames;
        }
        _max_frames = static_cast<int16_t>(frames);

        if (_max_frames <= frame_index) {
            return;
        }

        if (frame_index == 0 && attacker_size_class != -1) {
            Draw_Ship_Burst_(attacker_x, attacker_y, beam_dir, base_damage_class, 0, is_palette_not_90);
            return;
        }

        int16_t target_len = sprite_id * frame_index;
        int16_t attacker_len = target_len - duration_frames;

        if (param_34 != nullptr && param_34->anim_style == 2) {
            target_len = ship_range;
            attacker_len = 0;
        }

        if (target_len < 0) {
            target_len = 0;
        }
        if (ship_range < target_len) {
            target_len = ship_range;
        }

        int16_t bx2 = attacker_x;
        int16_t by2 = attacker_y;
        Rob_Interpolate_Line_(&bx2, &by2, target_x, target_y, target_len);

        if ((bx2 == target_x && by2 == target_y) || (_max_frames - 2 <= frame_index)) {
            _ship_hit_now_flag[is_palette_not_90] = 1;
            bx2 += random::Random_(3) - 1;
            random::Random_(3);
        } else {
            _ship_hit_now_flag[is_palette_not_90] = 0;
        }

        bx2 = attacker_x;
        by2 = attacker_y;

        int16_t reduce_len;
        if (hit_result_flags == 0 || special_flag != 0 || _g_pierced_shield_flag != 0 || attacker_size_class == -1) {
            if (reflection_state == 1 && special_flag == 0 && _g_pierced_shield_flag == 0 && attacker_size_class != -1) {
                int16_t size_idx = Ship_Size_To_Shield_Size_(target_size_class);
                reduce_len = _reflection_field_diameters[size_idx] / 3;
            } else {
                _g_pierced_shield_flag = 1;
                reduce_len = 0;
            }
        } else {
            int16_t size_idx = Ship_Size_To_Shield_Size_(target_size_class);
            reduce_len = _shield_diameters[size_idx] / 3;
        }

        int16_t cap_reduce = duration_frames - 1;
        if (reduce_len < cap_reduce) {
            cap_reduce = reduce_len;
        }

        int16_t dist = target_len - cap_reduce;
        Rob_Interpolate_Line_(&bx2, &by2, target_x, target_y, dist);

        int16_t bx1 = attacker_x;
        int16_t by1 = attacker_y;

        if (attacker_len < 0) {
            attacker_len = 0;
        }
        if (ship_range < attacker_len) {
            attacker_len = ship_range;
        }

        if (param_34 != nullptr && param_34->anim_style == 2) {
            attacker_len = 0;
        }

        if (reflection_state == 2) {
            int16_t size = attacker_size_class;
            if (size < 0) {
                size = 0;
            }
            int16_t size_idx = Ship_Size_To_Shield_Size_(size);
            attacker_len += _reflection_field_diameters[size_idx] / 3;
        }

        if (attacker_len != 0) {
            Rob_Interpolate_Line_(&bx1, &by1, target_x, target_y, attacker_len);
        }

        if (param_19 != 0 && param_34 == nullptr) {
            int16_t temp_x = bx1;
            bx1 = bx2;
            bx2 = temp_x;

            int16_t temp_y = by1;
            by1 = by2;
            by2 = temp_y;
        }

        if (param_34 == nullptr) {
            if (bx1 != bx2 || by1 != by2) {
                if (mount_scale_mod == 1) {
                    bolt::Bolt_Line_(bx1, by1, bx2, by2, palette_base_index, _beam_angle, beam_length, beam_width,
                               draw_core_flag, draw_glow_flag, draw_particles_flag, is_shield_piercing,
                               is_autofiring, param_17, param_18, fx_param_1, fx_param_2, fx_param_3, fx_param_4);
                } else if (mount_scale_mod == 2) {
                    int16_t mid3 = (fx_param_3 + fx_param_1) / 2;
                    int16_t mid4 = (fx_param_4 + fx_param_2) / 2;
                    bolt::Wide_Bolt_Line_(bx1, by1, bx2, by2, palette_base_index, _beam_angle, beam_length, beam_width,
                                        draw_core_flag, draw_glow_flag, draw_particles_flag, is_shield_piercing,
                                        is_autofiring, param_17, param_18, fx_param_1, fx_param_2, mid3, mid4, fx_param_3, fx_param_4);
                } else if (mount_scale_mod == 0) {
                    if (fx_param_4 < fx_param_2) fx_param_4 = fx_param_2;
                    if (fx_param_3 < fx_param_1) fx_param_3 = fx_param_1;
                    bolt::Bolt_Line_(bx1, by1, bx2, by2, palette_base_index, _beam_angle, beam_length, beam_width,
                               draw_core_flag, draw_glow_flag, draw_particles_flag, is_shield_piercing,
                               is_autofiring, param_17, param_18, 0, 0, fx_param_3, fx_param_4);
                }

                if (param_20 != 0 && _ship_hit_now_flag[is_palette_not_90] == 0) {
                    ERIC::Draw_Centered_(bx2, by2, _mauler_ball_seg);
                }
            }
        } else {
            if (reflection_state == 2) {
                attacker_x = bx1;
                attacker_y = by1;
            }
            uint32_t hit_flag = (hit_result_flags == 0) ? 1 : 0;
            Draw_Beam2_(attacker_x, attacker_y, target_x, target_y, bx1, by1, bx2, by2, hit_flag, frame_index, palette_base_index,
                        *param_34);
        }

        if (param_19 != 0 && param_34 == nullptr) {
            int16_t temp_x = bx1;
            bx1 = bx2;
            bx2 = temp_x;

            int16_t temp_y = by1;
            by1 = by2;
            by2 = temp_y;
        }

        if (frame_index < 3 || (attacker_x == bx1 && attacker_y == by1)) {
            if (attacker_size_class != -1) {
                Draw_Ship_Burst_(attacker_x, attacker_y, beam_dir, base_damage_class, frame_index, is_palette_not_90);
            }
            _ship_firing_now_flag[is_palette_not_90] = 1;
        } else {
            _ship_firing_now_flag[is_palette_not_90] = 0;
        }

        if (_ship_hit_now_flag[is_palette_not_90] != 0 && attacker_size_class != -1) {
            int16_t exp_frame;
            if (frame_index < _max_frames - 1) {
                exp_frame = (frame_index - 1 + (-10 - ship_range) / sprite_id) % 5;
            } else {
                exp_frame = frame_index - _max_frames + 7;
            }

            if (exp_frame < 0) {
                exp_frame = -exp_frame;
            }

            if (special_flag != 0 || hit_result_flags != 0) {
                int16_t final_target_facing = target_facing;
                if (is_non_ship_target != 0) {
                    final_target_facing = 0;
                }
                Draw_Ship_Explosion_(target_x, target_y, final_target_facing, exp_frame, is_palette_not_90);
            }
        }

        if (hit_result_flags != 0 && XY_In_Shield_Radius_(bx2, by2, target_x, target_y, target_size_class)) {
            _shield_hit_now_flag = 1;
        } else {
            _shield_hit_now_flag = 0;
        }

        if (reflection_state == 1) {
            if (XY_In_Reflection_Field_Radius_(bx2, by2, target_x, target_y, target_size_class)) {
                _reflection_field_hit_now_flag = 1;
                return;
            }
        }
        _reflection_field_hit_now_flag = 0;
    }

    void __cdecl Set_Beam_Colors_(
        int32_t palette_idx,
        int32_t r1, int32_t g1, int32_t b1,
        int32_t r2, int32_t g2, int32_t b2,
        int32_t intensity
    ) {
        LocalColor colors[16];

        for (int i = 0; i < 7; ++i) {
            int32_t val = _low_ramp_values[i];
            colors[i].r = (r1 * val) / 100;
            colors[i].g = (g1 * val) / 100;
            colors[i].b = (b1 * val) / 100;
        }

        colors[7].r = r1;
        colors[7].g = g1;
        colors[7].b = b1;

        int32_t cross_factor = 115; // 0x73
        for (int i = 0; i < 4; ++i) {
            if (intensity == 0 || intensity == 1 || intensity == 2) {
                int32_t primary_weight;
                if (intensity == 0) {
                    primary_weight = _low_primary_cross_values[i];
                } else if (intensity == 1) {
                    primary_weight = _moderate_primary_cross_values[i];
                } else {
                    primary_weight = _high_primary_cross_values[i];
                }

                int32_t inv_weight = 100 - primary_weight;
                int32_t scaled_weight = primary_weight * cross_factor;

                colors[8 + i].r = ((r1 * scaled_weight) / 100 + r2 * inv_weight) / 100;
                colors[8 + i].g = ((g1 * scaled_weight) / 100 + g2 * inv_weight) / 100;
                colors[8 + i].b = ((b1 * scaled_weight) / 100 + b2 * inv_weight) / 100;
            }

            if (colors[8 + i].r > 255) colors[8 + i].r = 255;
            if (colors[8 + i].g > 255) colors[8 + i].g = 255;
            if (colors[8 + i].b > 255) colors[8 + i].b = 255;

            cross_factor += 15; // 0xF
        }

        colors[12].r = r2;
        colors[12].g = g2;
        colors[12].b = b2;

        for (int i = 0; i < 3; ++i) {
            if (intensity == 0 || intensity == 1 || intensity == 2) {
                int32_t sat_weight;
                if (intensity == 0) {
                    sat_weight = _low_saturated_values[i];
                } else if (intensity == 1) {
                    sat_weight = _moderate_saturated_values[i];
                } else {
                    sat_weight = _high_saturated_values[i];
                }

                colors[13 + i].r = (r2 * sat_weight) / 100;
                colors[13 + i].g = (g2 * sat_weight) / 100;
                colors[13 + i].b = (b2 * sat_weight) / 100;
            }

            if (colors[13 + i].r > 255) colors[13 + i].r = 255;
            if (colors[13 + i].g > 255) colors[13 + i].g = 255;
            if (colors[13 + i].b > 255) colors[13 + i].b = 255;
        }

        for (int i = 0; i < 16; ++i) {
            colors[i].r /= 4;
            colors[i].g /= 4;
            colors[i].b /= 4;
        }

        for (int i = 0; i < 16; ++i) {
            video::_current_palette[palette_idx + i].changed = 1;
            video::_current_palette[palette_idx + i].r = static_cast<uint8_t>(colors[i].r);
            video::_current_palette[palette_idx + i].g = static_cast<uint8_t>(colors[i].g);
            video::_current_palette[palette_idx + i].b = static_cast<uint8_t>(colors[i].b);
        }
    }

    void __cdecl Draw_Beam2_(int16_t start_x, int16_t start_y, int16_t end_x, int16_t end_y, int32_t x1, int32_t y1,
                             int32_t x2, int32_t y2, int32_t hit_flag, int32_t frame_index, int32_t palette_base_index,
                             s_weapon_visual_fx fx)
    {
        (void)hit_flag;

        if ((x1 != x2 || y1 != y2) && fx.anim_style < 4) {
            int32_t mod_thick = frame_index % 2;

            switch (fx.anim_style) {
                case 0: {
                    int32_t arg16;
                    int32_t arg17;
                    if (frame_index == 0) {
                        strings::Swap_(&x1, &x2);
                        strings::Swap_(&y1, &y2);
                        arg17 = 3;
                        arg16 = 10;
                    } else if (frame_index == 1) {
                        arg17 = 6;
                        arg16 = 10;
                    } else {
                        arg17 = 3;
                        arg16 = 12;
                    }
                    bolt::Bolt_Line_(x1, y1, x2, y2, palette_base_index, 12, 30, 3, frame_index, 0, 0, 1, 1, 0, 0, arg16, arg17, 4, 0);
                    break;
                }
                case 1: {
                    int32_t cond = fx.sfx_sprite_id >= 10 ? 1 : 0;
                    int32_t size = fx.unk * 2 + 7;
                    bolt::Wide_Bolt_Line_(x1, y1, x2, y2, palette_base_index, 12, 50, 3, (uint16_t)frame_index, 0, 1, cond, 1, 0, 0, 12, 3, size, 1, 2, 0);
                    break;
                }
                case 2: {
                    int32_t div_val = mod_thick + 1;
                    int32_t arg18 = 4 / div_val;
                    int32_t arg17 = 8 / div_val;
                    int32_t arg16 = 12 / div_val;
                    
                    int32_t rand_y = random::Random_(5) + end_y - 3;
                    int32_t rand_x = random::Random_(5) + end_x - 3;

                    bolt::Bolt_Line_(start_x, start_y, rand_x, rand_y, palette_base_index, 12, 100, 4, frame_index * 4, 0, 0, 0, 0, 1, 0, arg16, arg17, arg18, 0);
                    break;
                }
                case 3: {
                    int32_t div_val1 = mod_thick + 1;
                    int32_t div_val2 = mod_thick * 2 + 1;
                    int32_t div_val3 = mod_thick + 2;

                    int32_t arg20_21 = 2 / div_val1;
                    int32_t arg18_19 = 5 / div_val2 + 1;
                    int32_t arg16_17 = 21 / div_val3;

                    bolt::Wide_Bolt_Line_(start_x, start_y, end_x, end_y, palette_base_index, 16, 100, 3, (uint16_t)(frame_index * 2), 1, 0, 0, 0, 1, 0, arg16_17, arg16_17, arg18_19, arg18_19, arg20_21, arg20_21);
                    break;
                }
            }
        }

        Set_Beam_Colors_(palette_base_index,
                         fx.r1,
                         fx.g1,
                         fx.b1,
                         fx.r2,
                         fx.g2,
                         fx.b2,
                         fx.thickness);
        palette::Refresh_Palette_();
    }

    /**
     * @address 00423080
     */
    bool __cdecl XY_In_Shield_Radius_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ship_size) {
        int16_t shield_idx = Ship_Size_To_Shield_Size_(ship_size);
        int16_t radius = (int16_t)((uint8_t)_shield_diameters[shield_idx] >> 1);

        if (abs((int16_t)(x1 - x2)) <= radius &&
            abs((int16_t)(y1 - y2)) <= radius) {
            return true;
        }

        return false;
    }

    /**
     * @address 00423e10
     */
    bool __cdecl XY_In_Reflection_Field_Radius_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ship_size) {
        int16_t shield_size = Ship_Size_To_Shield_Size_(ship_size);
        int16_t radius = _reflection_field_diameters[shield_size] >> 1;

        if (abs(static_cast<int16_t>(x1 - x2)) <= radius &&
            abs(static_cast<int16_t>(y1 - y2)) <= radius) {
            return true;
        }

        return false;
    }
}