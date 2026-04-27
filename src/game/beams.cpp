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

        uint16_t shield_status = 0;
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

    void __cdecl Adjust_Shield_Display_Status_(uint16_t* shield_status) {
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

                BEAMS::Fire_Weapon_(fx, fy, target_x, target_y, dir1, dir2, weapon_type, frame, 
                                    0, 0, 0, 0, 1, 0, 0, -1, 0);
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

        return base_rotations[rot % 16];
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

        return methods[rot % 16];
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
                              int16_t reflection_state, int16_t attacker_size_class, uint16_t weapon_specials)
    {
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
        } else if (_ship_firing_now_flag[fx_slot_idx] != 0) {
            if (Get_Max_Frames_() - 1 <= current_frame) {
                // TYPE MISMATCH: Fadeout_Sound_ signature in sound.h is (int16_t, int8_t)
                sound::Fadeout_Sound_(_g_beam_fire_sfx_handle[fx_slot_idx * 5 + fire_point_idx], 10);
                _g_beam_fire_sfx_handle[fx_slot_idx * 5 + fire_point_idx] = 0;
            }
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
                        int16_t attacker_shield_max = MOX::_combat_data[attacker_ship_id].shield_arc_max;

                        int16_t target_bearing;
                        if (is_non_ship_target == 0) {
                            target_bearing = CMBTDRW1::Relative_Bearing_(attacker_ship_id, target_id);
                        }
                        else {
                            target_bearing = CMBTDRW1::Relative_Bearing_XY_(attacker_ship_id, target_x, target_y);
                        }
                        target_bearing = CMBTFIRE::Facing_Shield_(target_bearing);

                        int16_t fp_count;
                        if (attacker_ship_id <= 0 || attacker_shield_max > 7 || _g_reflection_state_flag == 2) {
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
                                        rand_offsets[i * 2 + 1] = (int16_t)random::Random_(max_radius) - max_radius / 2;
                                        rand_offsets[i * 2] = (int16_t)random::Random_(max_radius) - max_radius / 2;
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
                                    rand_offsets[i * 2 + 1] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i * 2 + 1] *= (int16_t)(random::Random_(2) - 1);
                                    rand_offsets[i * 2] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i * 2] *= (int16_t)(random::Random_(2) - 1);
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
                                    rand_offsets[i * 2 + 1] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i * 2 + 1] *= (int16_t)(random::Random_(2) - 1);
                                    rand_offsets[i * 2] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i * 2] *= (int16_t)(random::Random_(2) - 1);
                                }
                                else {
                                    apply_random = true;
                                }
                            }

                            if (apply_random) {
                                if (is_non_ship_target == 0 || fx->accum_hit_x != 0) {
                                    rand_offsets[i * 2 + 1] = 0;
                                    rand_offsets[i * 2] = 0;
                                }
                                else {
                                    int16_t max_radius = ship_range / 2;
                                    int16_t shield_radius = _shield_diameters[0] / 2;
                                    if (max_radius <= shield_radius) {
                                        shield_radius = max_radius;
                                    }
                                    int16_t radius = (shield_radius + (shield_radius >> 31 & 3)) / 4 + 1;
                                    int16_t base = shield_radius / 2;
                                    rand_offsets[i * 2 + 1] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i * 2 + 1] *= (int16_t)(random::Random_(2) - 1);
                                    rand_offsets[i * 2] = base + (int16_t)random::Random_(radius);
                                    rand_offsets[i * 2] *= (int16_t)(random::Random_(2) - 1);
                                }
                            }
                        }

                        if (fp_count > 0) {
                            for (int32_t fp_idx_val = 0; fp_idx_val < fp_count; ++fp_idx_val) {
                                int16_t fp_x = 0, fp_y = 0;
                                if (attacker_ship_id > 0) {
                                    FIREPTS::Calc_FP_Offsets_(&fp_x, &fp_y, weapon_id, shot_count,
                                        attacker_owner, attacker_ship_id,
                                        attacker_picture_num, attacker_facing_dir,
                                        target_bearing, fp_idx_val);
                                }
                                Fire_Weapon_(attacker_center_x + fp_x, attacker_center_y + fp_y,
                                    target_center_x + rand_offsets[i * 2],
                                    target_center_y + rand_offsets[i * 2 + 1],
                                    attacker_facing_dir, target_facing_dir,
                                    fx->weapon_id, frame_idx[i], i,
                                    MOX::_combat_data[target_id].size_class,
                                    fx->result_flags, fx->special_flag,
                                    is_non_ship_target, fp_idx_val,
                                    _g_reflection_state_flag,
                                    MOX::_combat_data[attacker_ship_id].size_class,
                                    fx->specials);
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
                    int16_t special_flag = 0;
                    if (current_fx_idx[0] > -1) {
                        special_flag = param_7[current_fx_idx[0]].special_flag;
                    }
                    if (current_fx_idx[1] > -1) {
                        special_flag += param_7[current_fx_idx[1]].special_flag;
                    }
                    if (_g_pierced_shield_flag != 0) {
                        special_flag = 1;
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
                        Draw_Shield_Hit_(dx, dy, target_id, (special_flag != 0), 0, hit_facing, (int16_t)draw_facing);
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
                        Draw_Shield_Hit_(dx, dy, target_id, (special_flag != 0), specials, hit_facing, (int16_t)draw_facing);
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
                                int16_t hit_facing = Do_Get_SFX_Draw_Shield_Facing_(attacker_ship_id, target_id);
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
}