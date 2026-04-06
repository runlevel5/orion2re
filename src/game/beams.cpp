#include "pch.h"
#include "beams.h"
#include "animate.h"
#include "palette.h"
#include "firepts.h"
#include "farload.h"
#include "colony.h"
#include "mox.h"

namespace BEAMS {
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

    s_animation_header* __cdecl Reflection_Field_Anims_(int32_t p1, int32_t p2) {
        // The index calculation is performed in 16-bit registers in disassembly
        int16_t entry_num = (int16_t)p2 * 5 + (int16_t)p1 + 89;

        return (s_animation_header*)Buffer_Reload_(
            COLONY::_BEAMS_LBX,
            static_cast<int32_t>(entry_num),
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
            return (s_animation_header*)Buffer_Reload_(COLONY::_BEAMS_LBX, size_idx * 5 + beam_idx + 113 /* 0x71 */, MOX::_global_cache_seg);
        }

        if (is_heavy != 0) {
            // Heavy variant offset
            return (s_animation_header*)Buffer_Reload_(COLONY::_BEAMS_LBX, size_idx + 129 /* 0x81 */, MOX::_global_cache_seg);
        }

        if (size_idx < 4 && is_special == 0) {
            // Return from the pre-cached shield animations
            return BEAMS::_shield_anims[beam_idx][size_idx][0];
        }

        // Default reload path
        return (s_animation_header*)Buffer_Reload_(COLONY::_BEAMS_LBX, size_idx * 5 + beam_idx + 109 /* 0x6D */, MOX::_global_cache_seg);
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
                uint16_t current_index = BEAMS::_n_damage_msgs;
                s_beam_dmg_msg& msg = BEAMS::_msg_queue[current_index];

                msg.anim_timer = 0;
                msg.x = x;
                msg.y = y;
                msg.total_dmg = (uint16_t)total_dmg;
                msg.hull_dmg = (uint16_t)struct_dmg;
                msg.shield_dmg = (uint16_t)shield_dmg;
                msg.side = side;

                BEAMS::_n_damage_msgs++;
            }
        }
    }

    void __cdecl Add_Damage_Indicator_For_Ship_(int32_t dx, int32_t dy, int16_t ship_idx, int32_t flag, int16_t total_damage, int16_t struct_damage) {
        int16_t x_center;
        int16_t y_center;
        int16_t size_offset;

        // Size class: 0=Tiny, 1=Small, 2=Medium, 3=Large, 4=Huge, 5=Titan/Starbase
        size_offset = static_cast<int16_t>(MOX::_combat_data[ship_idx].size_class) * 8;

        COMBINIT::Ship_Center_Offsets_(ship_idx, &x_center, &y_center);

        x_center += static_cast<int16_t>(dx);
        y_center += static_cast<int16_t>(dy);

        if (flag != 0) {
            x_center += size_offset;
        } else {
            x_center -= size_offset;
        }

        y_center -= size_offset;

        BEAMS::Add_Damage_Message_To_Queue_(x_center, y_center, total_damage, struct_damage, 0, (uint8_t)(flag != 0));
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
        uint8_t total_dmg_colors[27] = {
            0xb0, 0xb9, 0xba, 0xb0, 0xb8, 0xb9, 0xb0, 0xb7, 0xb8,
            0xb0, 0xb6, 0xb7, 0xb0, 0xb5, 0xb6, 0xb0, 0xb4, 0xb5,
            0xb0, 0xb3, 0xb4, 0xb0, 0xb2, 0xb3, 0xb0, 0xb1, 0xb2
        };

        uint8_t shield_dmg_colors[27] = {
            0x50, 0x57, 0x57, 0x50, 0x56, 0x57, 0x50, 0x55, 0x56,
            0x50, 0x54, 0x55, 0x50, 0x53, 0x54, 0x50, 0x52, 0x53,
            0x50, 0x51, 0x52, 0x50, 0x50, 0x51, 0x50, 0x50, 0x50
        };

        uint8_t hull_dmg_colors[27] = {
            0xc0, 0xcc, 0xc5, 0xc0, 0xcc, 0xc5, 0xc0, 0xcc, 0xc5,
            0xc0, 0xc4, 0xcc, 0xc0, 0xcb, 0xc4, 0xc0, 0xc3, 0xcb,
            0xc0, 0xc3, 0xcb, 0xc0, 0xca, 0xc3, 0xc0, 0xc2, 0xca
        };

        y -= anim_timer;

        if (side == 1) {
            x += anim_timer;
        } else {
            x -= anim_timer;
        }

        if (anim_timer >= 9) {
            return;
        }

        graphics::Set_Window_(0, 0, 0x27f, 0x15e);
        video::Clip_On_();

        if (total_dmg > 0) {
            fonts::Set_Font_Style_Shadow_Down_(1, reinterpret_cast<s_colors*>(&total_dmg_colors[anim_timer * 3]));
            fonts::Print_Integer_(x, y, total_dmg);
        }

        if (shield_dmg > 0) {
            fonts::Set_Font_Style_Shadow_Down_(1, reinterpret_cast<s_colors*>(&shield_dmg_colors[anim_timer * 3]));
            fonts::Print_Integer_(x - 10, y, shield_dmg);
        }

        if (hull_dmg > 0) {
            fonts::Set_Font_Style_Shadow_Down_(1, reinterpret_cast<s_colors*>(&hull_dmg_colors[anim_timer * 3]));
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

                int q1 = angle::Get_Angle_(fx - target_x, target_y - fy) / 22 + 8;
                int16_t dir1 = (abs(q1) & 0xF);
                if (q1 < 0) dir1 = -dir1;

                int q2 = angle::Get_Angle_(target_x - fx, fy - target_y) / 22 + 8;
                int16_t dir2 = (abs(q2) & 0xF);
                if (q2 < 0) dir2 = -dir2;

                if (BEAMS::Get_Max_Frames_() < frame) {
                    return 0;
                }

                bool is_mod3 = (i % 3 == 0);
                BEAMS::Fire_Weapon_(fx, fy, target_x, target_y, dir1, dir2, weapon_type, frame, 
                                    0, 0, 0, 0, 1, 0, 0, -1, 0, is_mod3);
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
}