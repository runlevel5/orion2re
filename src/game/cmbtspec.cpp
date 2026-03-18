#include "pch.h"

namespace CMBTSPEC {
    void __cdecl Draw_Cloak_(int16_t ship_idx, int16_t cloak_type) {
        int16_t unused_x, unused_y;
        int16_t offset_x, offset_y;
        int16_t rel_x, rel_y;
        int16_t duration;
        int16_t step;
        int32_t sound_handle;

        if (CMBTDRW1::Ship_On_Screen_(ship_idx) == 0) {
            return;
        }

        s_combat_data& ship = MOX::_combat_data[ship_idx];

        // This call's results are not used but maintained for semantic equivalence.
        CMBTDRW1::Get_Draw_Ship_Offsets_(ship.size_class, &unused_x, &unused_y);

        switch (cloak_type) {
            case 1:
            case 2:
                duration = 24;
                break;
            case 4:
                duration = 60;
                break;
            case 5:
                duration = 36;
                break;
            default:
                duration = ship_idx;
                break;
        }

        MOX::_dont_draw_ship = ship_idx;
        rel_x = (ship.x - MOX::_cur_combat_x) * 20;
        rel_y = (ship.y - MOX::_cur_combat_y) * 20;

        if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
            step = 2;
        } else {
            step = 1;
        }

        sound_handle = ERICNET::Mox_Play_Sound_(10);

        if (duration > 0) {
            for (int16_t progress = 0; progress < duration; progress += step) {
                timer::Mark_Time_();
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                CMBTDRW1::Draw_Ship_To_Bitmap_(ship_idx, 0, cloak_type, progress);
                CMBTDRW1::Get_Draw_Ship_Offsets_(ship_idx, &offset_x, &offset_y);
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();
                animate::Draw_(offset_x + rel_x, offset_y + rel_y, MOX::_draw_ship_bitmap);
                video::Clip_Off_();
                graphics::Reset_Window_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(1);
            }
        }

        MOX::_dont_draw_ship = -1;
        sound::Fadeout_Sound_(sound_handle, 10);
    }

    void __cdecl Start_Ship_Engine_SFX_() {
        if (CMBTSPEC::_g_engine_sfx_handle == 0) {
            CMBTSPEC::_g_engine_sfx_handle = ERICNET::Mox_Play_Sound_(0x1f);
            sound::Loop_Sound_(CMBTSPEC::_g_engine_sfx_handle, -1);
        }
    }

    void __cdecl End_Ship_Engine_SFX_() {
        sound::Fadeout_Sound_(CMBTSPEC::_g_engine_sfx_handle, 10);
        CMBTSPEC::_g_engine_sfx_handle = 0;
    }

    void __cdecl Draw_Continuing_Tractor_(int16_t ship_idx) {
        int16_t x1, y1, x2, y2;
        int16_t target_idx;

        target_idx = static_cast<int16_t>(MOX::_combat_data[ship_idx].special_cooldown);

        COMBINIT::Ship_Center_Offsets_(ship_idx, &x1, &y1);
        x1 += (MOX::_combat_data[ship_idx].x - MOX::_cur_combat_x) * 20;
        y1 += (MOX::_combat_data[ship_idx].y - MOX::_cur_combat_y) * 20;

        COMBINIT::Ship_Center_Offsets_(target_idx, &x2, &y2);
        x2 += (MOX::_combat_data[target_idx].x - MOX::_cur_combat_x) * 20;
        y2 += (MOX::_combat_data[target_idx].y - MOX::_cur_combat_y) * 20;

        animate::Set_Animation_Frame_(static_cast<s_animation_header*>(MOX::_small_tractor_seg), 6);
        palette::Restore_Glass_Remap_Block_(MOX::_tractor_remap_colors);
        allocate::Push_Block_(MOX::_screen_seg);

        int32_t remaining_space = allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);
        void* temp_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, remaining_space - 100);

        bitmap::Draw_Picture_To_Bitmap_(static_cast<s_animation_header*>(temp_bitmap), MOX::_small_tractor_seg);
        bitmap::Set_Picture_Glassed_(static_cast<s_animation_header*>(temp_bitmap));

        graphics::Set_Window_(0, 0, 639, 350);
        video::Clip_On_();
        bitmap::Draw_Bitmap_Line_(x1, y1, x2, y2, temp_bitmap);
        video::Clip_Off_();

        palette::Restore_Glass_Remap_Block_(MOX::_shield_remap_colors);
        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Draw_Moving_Tractor_(int16_t ship_idx, int16_t center_x, int16_t center_y) {
        int16_t target_ship_idx = static_cast<int16_t>(MOX::_combat_data[ship_idx].special_cooldown);
        int16_t x1, y1, x2, y2;

        COMBINIT::Ship_Center_Offsets_(ship_idx, &x1, &y1);
        if (ship_idx == MOX::_cur_ship) {
            x1 += center_x - (MOX::_cur_combat_x * 20);
            y1 += center_y - (MOX::_cur_combat_y * 20);
        } else {
            x1 += (MOX::_combat_data[ship_idx].x - MOX::_cur_combat_x) * 20;
            y1 += (MOX::_combat_data[ship_idx].y - MOX::_cur_combat_y) * 20;
        }

        COMBINIT::Ship_Center_Offsets_(target_ship_idx, &x2, &y2);
        if (target_ship_idx == MOX::_cur_ship) {
            x2 += center_x - (MOX::_cur_combat_x * 20);
            y2 += center_y - (MOX::_cur_combat_y * 20);
        } else {
            x2 += (MOX::_combat_data[target_ship_idx].x - MOX::_cur_combat_x) * 20;
            y2 += (MOX::_combat_data[target_ship_idx].y - MOX::_cur_combat_y) * 20;
        }

        animate::Set_Animation_Frame_(static_cast<s_animation_header*>(MOX::_small_tractor_seg), (MOX::_ship_frame % 3) + 4);
        palette::Restore_Glass_Remap_Block_(MOX::_tractor_remap_colors);
        allocate::Push_Block_(MOX::_screen_seg);

        int32_t remaining = allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);
        void* temp_bmp = allocate::Allocate_Next_Block_(MOX::_screen_seg, remaining - 100);

        bitmap::Draw_Picture_To_Bitmap_(static_cast<s_animation_header*>(temp_bmp), MOX::_small_tractor_seg);
        bitmap::Set_Picture_Glassed_(static_cast<s_animation_header*>(temp_bmp));
        bitmap::Draw_Bitmap_Line_(x1, y1, x2, y2, temp_bmp);

        allocate::Pop_Block_(MOX::_screen_seg);
        palette::Restore_Glass_Remap_Block_(MOX::_shield_remap_colors);
    }

    void __cdecl Lightning_Field_(int16_t ship_idx, int16_t missile_idx, int16_t target_x, int16_t target_y) {
        int16_t off_x, off_y;
        int16_t target_x_rel_center, target_y_rel_center;
        int16_t missile_draw_x, missile_draw_y;
        int16_t draw_x, draw_y;
        int16_t old_dont_draw;
        int32_t sound_handle;
        s_animation_header* anim;

        COMBINIT::Ship_Center_Offsets_(ship_idx, &off_x, &off_y);
        target_x_rel_center = target_x - off_x;
        target_y_rel_center = target_y - off_y;

        if (CMBTDRW1::Ship_Completely_On_Screen_(ship_idx) == 0) {
            COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[ship_idx].x, MOX::_combat_data[ship_idx].y);
        }

        missile_draw_x = MOX::_missile[missile_idx].x - MOX::_cur_combat_x * 20;
        missile_draw_y = MOX::_missile[missile_idx].y - MOX::_cur_combat_y * 20;

        target_x_rel_center -= MOX::_cur_combat_x * 20;
        target_y_rel_center -= MOX::_cur_combat_y * 20;

        int16_t param_3_rel = target_x - MOX::_cur_combat_x * 20;
        int16_t param_4_rel = target_y - MOX::_cur_combat_y * 20;

        draw_x = (target_x - MOX::_cur_combat_x * 20) - 36;
        draw_y = (target_y - MOX::_cur_combat_y * 20) - 36;

        int16_t range = special::Range_(param_3_rel, param_4_rel, missile_draw_x, missile_draw_y);
        if (range > 16) {
            special::Interpolate_Line_(&param_3_rel, &param_4_rel, missile_draw_x, missile_draw_y, range - 16);
        }

        old_dont_draw = MOX::_dont_draw_ship;
        MOX::_dont_draw_ship = ship_idx;
        sound_handle = ERICNET::Mox_Play_Sound_(4);

        for (int i = 15; i > 0; --i) {
            timer::Mark_Time_();
            video::Set_Page_Off_();
            CMBTDRW1::Draw_Main_Combat_Screen_();
            graphics::Set_Window_(0, 0, 639, 350);
            video::Clip_On_();
            CMBTDRW1::Draw_Ship_(ship_idx, target_x_rel_center, target_y_rel_center);
            anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 0, MOX::_global_cache_seg);
            animate::Draw_(draw_x, draw_y, anim);
            video::Clip_Off_();
            graphics::Reset_Window_();
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);
        }

        MOX::_dont_draw_ship = old_dont_draw;
        sound::Fadeout_Sound_(sound_handle, 10);
    }

    void __cdecl Draw_Stasis_Ball_(int16_t ship_idx, int16_t scale_percent, int16_t off_x, int16_t off_y) {
        int16_t center_x;
        int16_t center_y;
        int16_t actual_w;
        int16_t actual_h;
        int16_t min_x;
        int16_t min_y;
        s_animation_header* h_target;
        s_animation_header* h_temp;
        s_animation_header* final_img;
        int32_t resource_idx;

        COMBINIT::Ship_Center_Offsets_(ship_idx, &center_x, &center_y);

        center_x += (MOX::_combat_data[ship_idx].x - MOX::_cur_combat_x) * 20 + off_x;
        center_y += (MOX::_combat_data[ship_idx].y - MOX::_cur_combat_y) * 20 + off_y;

        allocate::Push_Block_(MOX::_screen_seg);
        h_target = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x57E4);
        h_temp = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x57E4);

        int16_t facing = MOX::_combat_data[ship_idx].facing_dir;
        int32_t dir_idx = facing;

        if (facing > 4 && facing < 9) {
            dir_idx = abs(facing - 8);
        } else if (facing > 8 && facing < 13) {
            dir_idx = abs(facing + 8) & 0xF;
        } else if (facing > 12 && facing < 16) {
            dir_idx = abs(facing - 16);
        } else {
            dir_idx = abs(dir_idx);
        }

        switch (MOX::_combat_data[ship_idx].size_class) {
        case 0:
            resource_idx = dir_idx + 0x35;
            break;
        case 1:
        case 2:
            resource_idx = dir_idx + 0x3A;
            break;
        case 3:
            resource_idx = dir_idx + 0x3F;
            break;
        case 4:
        case 5:
            resource_idx = dir_idx + 9;
            break;
        default:
            goto skip_load;
        }

        {
            char* path = ESTRINGS::E_Strings_(0xE0);
            s_animation_header* loaded_img = (s_animation_header*)Buffer_Reload_(path, resource_idx, MOX::_global_cache_seg);
            bitmap::Draw_Picture_To_Bitmap_(h_temp, loaded_img);
        }

    skip_load:
        if (facing > 4 && facing < 9) {
            bitmap::Flip_Bitmap_(h_target, h_temp, 0);
        } else if (facing > 12 && facing < 16) {
            bitmap::Flip_Bitmap_(h_target, h_temp, 3);
        } else {
            bitmap::Draw_Picture_To_Bitmap_(h_target, h_temp);
        }

        if (scale_percent == 100) {
            bitmap::Set_Picture_Glassed_(h_target);
            bitmap::Get_Bitmap_Actual_Size_(h_target, &min_x, &min_y, &actual_w, &actual_h);
            final_img = h_target;
        } else {
            bitmap::Scale_Bitmap_(h_temp, h_target, scale_percent, scale_percent);
            bitmap::Set_Picture_Glassed_(h_temp);
            bitmap::Get_Bitmap_Actual_Size_(h_temp, &min_x, &min_y, &actual_w, &actual_h);
            final_img = h_temp;
        }

        animate::Draw_(center_x - actual_w / 2, center_y - actual_h / 2, final_img);

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Draw_Plasma_Web_(int16_t ship_idx, int16_t x_off, int16_t y_off) {
        int16_t draw_x;
        int16_t draw_y;
        int16_t sub_dir;
        int32_t asset_id;
        s_animation_header* seg;
        void* temp_bitmap;
        int8_t facing;

        allocate::Push_Block_(MOX::_screen_seg);
        uint32_t bitmap_size = bitmap::Get_Bitmap_Size_(80, 80);
        temp_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);

        COMBINIT::Ship_Center_Offsets_(ship_idx, &draw_x, &draw_y);

        draw_x += x_off;
        draw_y += y_off;

        facing = MOX::_combat_data[ship_idx].facing_dir;
        sub_dir = (int16_t)(facing % 4);

        if (facing == 4 || facing == 12) {
            sub_dir = 4;
        }

        bool is_even_facing = ((facing / 4) & 1) == 0;

        switch (MOX::_combat_data[ship_idx].size_class) {
        case 0:
            if (is_even_facing) {
                asset_id = sub_dir + 31;
            } else {
                asset_id = 35 - sub_dir;
            }
            break;

        case 1:
        case 2:
            if (is_even_facing) {
                asset_id = sub_dir + 26;
            } else {
                asset_id = 30 - sub_dir;
            }
            break;

        case 3:
            if (is_even_facing) {
                asset_id = sub_dir + 21;
            } else {
                asset_id = 25 - sub_dir;
            }
            break;

        case 4:
        case 5:
            if (is_even_facing) {
                asset_id = sub_dir + 21;
            } else {
                asset_id = 20 - sub_dir;
            }
            break;

        default:
            goto finish_drawing;
        }

        seg = static_cast<s_animation_header*>(Buffer_Reload_(ESTRINGS::E_Strings_(224), asset_id, MOX::_global_cache_seg));
        {
            int16_t frame_count = animate::Get_Animation_Frame_Count_(seg);
            animate::Set_Animation_Frame_(seg, MOX::_ship_frame % frame_count);
        }
        bitmap::Draw_Picture_To_Bitmap_(static_cast<s_animation_header*>(temp_bitmap), seg);

    finish_drawing:
        int8_t flip_dir = facing / 4;
        if (flip_dir != 0) {
            if (flip_dir == 1) {
                bitmap::Flip_Bitmap_(static_cast<s_animation_header*>(temp_bitmap), static_cast<s_animation_header*>(temp_bitmap), 0);
            } else if (flip_dir == 3) {
                bitmap::Flip_Bitmap_(static_cast<s_animation_header*>(temp_bitmap), static_cast<s_animation_header*>(temp_bitmap), 3);
            }
        }

        palette::Restore_Glass_Remap_Block_(MOX::_plasma_web_remap_colors);

        draw_x -= animate::Get_Width_(static_cast<s_animation_header*>(temp_bitmap)) / 2;
        draw_y -= animate::Get_Height_(static_cast<s_animation_header*>(temp_bitmap)) / 2;

        bitmap::Set_Picture_Glassed_(static_cast<s_animation_header*>(temp_bitmap));
        animate::Draw_(draw_x, draw_y, static_cast<s_animation_header*>(temp_bitmap));

        allocate::Pop_Block_(MOX::_screen_seg);
        palette::Restore_Glass_Remap_Block_(MOX::_shield_remap_colors);
    }
    void __cdecl Draw_Stasis_Beam_(int16_t source_ship_idx, int16_t target_ship_idx) {
        int16_t x1, y1, x2, y2;
        int16_t frame_count;
        int16_t step;
        int32_t sound_handle;
        s_animation_header* anim;
        void* temp_bitmap;

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            if (!CMBTDRW1::Ship_Completely_On_Screen_(target_ship_idx)) {
                COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[target_ship_idx].x, MOX::_combat_data[target_ship_idx].y);
            }

            anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 1, MOX::_global_cache_seg);
            animate::Draw_Palette_(anim);
            frame_count = animate::Get_Animation_Frame_Count_(anim);

            allocate::Push_Block_(MOX::_screen_seg);
            temp_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, 50000 /* 0xC350 */);

            COMBINIT::Ship_Center_Offsets_(source_ship_idx, &x1, &y1);
            x1 += (MOX::_combat_data[source_ship_idx].x - MOX::_cur_combat_x) * 20;
            y1 += (MOX::_combat_data[source_ship_idx].y - MOX::_cur_combat_y) * 20;

            COMBINIT::Ship_Center_Offsets_(target_ship_idx, &x2, &y2);
            x2 += (MOX::_combat_data[target_ship_idx].x - MOX::_cur_combat_x) * 20;
            y2 += (MOX::_combat_data[target_ship_idx].y - MOX::_cur_combat_y) * 20;

            if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
                step = 2;
            } else {
                step = 1;
            }

            MOX::_dont_draw_ship = source_ship_idx;
            sound_handle = ERICNET::Mox_Play_Sound_(10);

            if (frame_count > 0) {
                for (int16_t frame_idx = 0; frame_idx < frame_count; frame_idx += step) {
                    timer::Mark_Time_();
                    video::Set_Page_Off_();
                    CMBTDRW1::Draw_Main_Combat_Screen_();
                    
                    anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 1, MOX::_global_cache_seg);
                    animate::Set_Animation_Frame_(anim, frame_idx);
                    bitmap::Draw_Picture_To_Bitmap_(static_cast<s_animation_header*>(temp_bitmap), anim);
                    
                    graphics::Set_Window_(0, 0, 639 /* 0x27F */, 350 /* 0x15E */);
                    video::Clip_On_();
                    bitmap::Draw_Bitmap_Line_(x1, y1, x2, y2, temp_bitmap);
                    CMBTDRW1::Draw_Ship_(source_ship_idx, (MOX::_combat_data[source_ship_idx].x - MOX::_cur_combat_x) * 20, (MOX::_combat_data[source_ship_idx].y - MOX::_cur_combat_y) * 20);
                    graphics::Reset_Window_();
                    video::Clip_Off_();
                    RUSS::Mox_Sync_Update_();
                    timer::Release_Time_(2);
                }
            }

            MOX::_dont_draw_ship = -1;
            sound::Fadeout_Sound_(sound_handle, 10);

            sound_handle = ERICNET::Mox_Play_Sound_(10);
            int16_t ball_frame = 0;
            int16_t ball_step_increment = step * 10;
            for (int16_t ball_counter = 0; ball_counter < 10; ball_counter += step) {
                timer::Mark_Time_();
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                
                graphics::Set_Window_(0, 0, 639 /* 0x27F */, 350 /* 0x15E */);
                video::Clip_On_();
                Draw_Stasis_Ball_(target_ship_idx, ball_frame, 0, 0);
                video::Clip_Off_();
                graphics::Reset_Window_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(1);
                
                ball_frame += ball_step_increment;
            }

            sound::Fadeout_Sound_(sound_handle, 10);
            allocate::Pop_Block_(MOX::_screen_seg);
        }
    }

    void __cdecl Gyro_Destablizer_(int16_t source_idx, int16_t target_idx, int16_t attack_count) {
        int16_t rotations_left = 0;
        int16_t saved_dont_draw;
        int32_t sound_handle = 0;

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            if (!CMBTDRW1::Ship_Completely_On_Screen_(target_idx)) {
                COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[target_idx].x, MOX::_combat_data[target_idx].y);
            }

            saved_dont_draw = MOX::_dont_draw_ship;
            MOX::_dont_draw_ship = target_idx;

            video::Set_Page_Back_();
            CMBTDRW1::Full_Draw_Combat_Screen_();
            CMBTDRW1::Draw_Main_Combat_Screen_();
            video::Copy_Back_To_Off_();
            video::Set_Page_Off_();
            graphics::Set_Window_(0, 0, 639, 350);
            video::Clip_On_();

            int16_t screen_x = (MOX::_combat_data[target_idx].x - MOX::_cur_combat_x) * 20;
            int16_t screen_y = (MOX::_combat_data[target_idx].y - MOX::_cur_combat_y) * 20;

            CMBTDRW1::Draw_Ship_(target_idx, screen_x, screen_y);
            video::Clip_Off_();
            graphics::Reset_Window_();

            RUSS::Mox_Sync_Update_();
            CMBTSPEC::Draw_Gyro_Special_Effect_(source_idx, target_idx);

            for (int i = 0; i < 19; ++i) {
                rotations_left += (int16_t)random::Random_(13);
            }

            while (true) {
                int16_t temp = abs(rotations_left) % 16;
                if (temp != 15 && temp >= 2) break;
                rotations_left += (int16_t)random::Random_(13);
            }

            graphics::Set_Window_(0, 0, 639, 350);
            video::Clip_On_();
            sound_handle = ERICNET::Mox_Play_Sound_(3);

            if (rotations_left > 0) {
                do {
                    video::Set_Page_Off_();
                    int16_t next_dir = MOX::_combat_data[target_idx].facing_dir + 1;
                    MOX::_combat_data[target_idx].facing_dir = (int8_t)(abs(next_dir) % 16);

                    graphics::Set_Window_(0, 0, 639, 350);
                    video::Clip_On_();
                    CMBTDRW1::Draw_Ship_(target_idx, screen_x, screen_y);
                    graphics::Reset_Window_();
                    video::Clip_Off_();
                    RUSS::Mox_Sync_Update_();
                    rotations_left--;
                } while (rotations_left != 0);
            }

            video::Clip_Off_();
            graphics::Reset_Window_();
            video::Set_Page_Back_();
            CMBTDRW1::Full_Draw_Combat_Screen_();
            video::Set_Page_Off_();
            MOX::_dont_draw_ship = saved_dont_draw;
        }

        int32_t total_damage_accum = 0;
        if (attack_count > 0) {
            int16_t range = TECHDATA::_weapons[34].damage - TECHDATA::_weapons[34].min_damage + 1;
            for (int i = 0; i < attack_count; ++i) {
                int32_t roll = random::Random_(range);
                int16_t base_dmg = TECHDATA::_weapons[34].min_damage + (int16_t)roll - 1;
                total_damage_accum += base_dmg * (MOX::_combat_data[target_idx].size_class + 1);
            }
        }

        int16_t bonus = MOX::_ordnance_bonus[MOX::_combat_data[source_idx].owner];
        int16_t scaled_damage = (int16_t)((bonus * total_damage_accum) / 100);

        int16_t current_damage = MOX::_combat_data[target_idx].structure_damage;
        int16_t max_structure = MOX::_combat_data[target_idx].structure_max;
        int16_t applied_damage = scaled_damage;

        if (current_damage + scaled_damage > max_structure) {
            applied_damage = max_structure - current_damage;
        }

        MOX::_combat_data[target_idx].structure_damage += applied_damage;

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            int16_t screen_x = (MOX::_combat_data[target_idx].x - MOX::_cur_combat_x) * 20;
            int16_t screen_y = (MOX::_combat_data[target_idx].y - MOX::_cur_combat_y) * 20;

            BEAMS::Add_Damage_Indicator_For_Ship_(screen_x, screen_y, target_idx, 1, applied_damage, 0);
            BEAMS::Draw_Damage_Message_Queue_Until_Done_();
            sound::Fadeout_Sound_((int16_t)sound_handle, 10);
        }
    }

    void __cdecl Tractor_Beam_(int16_t param_1, int16_t param_2) {
        int16_t src_x;
        int16_t src_y;
        int16_t dst_x;
        int16_t dst_y;

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            allocate::Push_Block_(MOX::_screen_seg);
            s_animation_header* h1 = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 50000);

            COMBINIT::Ship_Center_Offsets_(param_1, &src_x, &src_y);
            src_x += (MOX::_combat_data[param_1].x - MOX::_cur_combat_x) * 20;
            src_y += (MOX::_combat_data[param_1].y - MOX::_cur_combat_y) * 20;

            COMBINIT::Ship_Center_Offsets_(param_2, &dst_x, &dst_y);
            dst_x += (MOX::_combat_data[param_2].x - MOX::_cur_combat_x) * 20;
            dst_y += (MOX::_combat_data[param_2].y - MOX::_cur_combat_y) * 20;

            for (int16_t frame = 0; frame < 8; ++frame) {
                timer::Mark_Time_();
                video::Set_Page_Off_();
                animate::Set_Animation_Frame_(static_cast<s_animation_header*>(MOX::_small_tractor_seg), frame);
                bitmap::Draw_Picture_To_Bitmap_(h1, MOX::_small_tractor_seg);
                bitmap::Set_Picture_Glassed_(h1);
                palette::Restore_Glass_Remap_Block_(MOX::_tractor_remap_colors);
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();
                bitmap::Draw_Bitmap_Line_(src_x, src_y, dst_x, dst_y, h1);
                graphics::Reset_Window_();
                video::Clip_Off_();
                palette::Restore_Glass_Remap_Block_(MOX::_shield_remap_colors);
                CMBTDRW1::Draw_Main_Combat_Screen_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(1);
            }

            allocate::Pop_Block_(MOX::_screen_seg);
        }

        MOX::_combat_data[param_1].special_cooldown = (int8_t)param_2;
        int16_t current_speed = COMBAT1::Calc_Current_Speed_(param_2);
        MOX::_combat_data[param_2].current_speed = current_speed;
        CMBTDRW1::Refresh_Combat_Screen_Full_();
    }

    void __cdecl Draw_Anti_Missile_Rockets_(int16_t ship_idx, int16_t target_x, int16_t target_y, int16_t missile_idx) {
        int16_t x_offset = MOX::_cur_combat_x * 20;
        int16_t y_offset = MOX::_cur_combat_y * 20;

        int16_t missile_x_rel = MOX::_missile[missile_idx].x - x_offset;
        int16_t missile_y_rel = MOX::_missile[missile_idx].y - y_offset;

        int16_t target_x_rel = target_x - x_offset;
        int16_t target_y_rel = target_y - y_offset;

        int16_t off_x = 0;
        int16_t off_y = 0;
        COMBINIT::Ship_Center_Offsets_(ship_idx, &off_x, &off_y);

        int16_t curr_x = off_x + target_x_rel;
        int16_t curr_y = off_y + target_y_rel;
        int16_t start_x = curr_x;
        int16_t start_y = curr_y;

        int16_t speed;
        if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
            speed = 30;
        } else {
            speed = 20;
        }

        graphics::Save_Window_();

        while (missile_x_rel != curr_x || missile_y_rel != curr_y) {
            timer::Mark_Time_();
            video::Set_Page_Off_();
            CMBTDRW1::Draw_Main_Combat_Screen_();
            graphics::Set_Window_(0, 0, 639, 350);
            video::Clip_On_();

            if (ship_idx == MOX::_dont_draw_ship) {
                CMBTDRW1::Draw_Ship_(ship_idx, target_x_rel, target_y_rel);
            }

            special::Absolute_Interpolate_Line_(&curr_x, &curr_y, start_x, start_y, missile_x_rel, missile_y_rel, speed);
            CMBTSPEC::Draw_Anti_Missile_Rocket_Picture_(curr_x, curr_y);
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);
        }
    }

    void __cdecl Anti_Missile_Rockets_(int16_t ship_idx, int16_t ship_x, int16_t ship_y, int16_t missile_idx, int16_t weapon_slot_idx, int16_t tech_level) {
        int16_t cur_combat_x_offset = MOX::_cur_combat_x * 20;
        int16_t cur_combat_y_offset = MOX::_cur_combat_y * 20;
        int16_t count_destroyed = 0;

        int16_t target_x = MOX::_missile[missile_idx].x - cur_combat_x_offset;
        int16_t target_y = MOX::_missile[missile_idx].y - cur_combat_y_offset;

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            Draw_Anti_Missile_Rockets_(ship_idx, ship_x, ship_y, missile_idx);
        }

        int16_t hit_chance = (int16_t)(((-2 - tech_level) * 10) / 3) + 71;
        int16_t shots_avail = MOX::_combat_data[ship_idx].weapons[weapon_slot_idx].shots_left;

        if (shots_avail > 0) {
            for (int16_t i = 0; i < shots_avail; ++i) {
                if (MOX::_missile[missile_idx].type == 0) {
                    break;
                }

                int roll = random::Random_(100);
                MOX::_combat_data[ship_idx].weapons[weapon_slot_idx].shots_left--;
                MOX::_combat_data[ship_idx].weapons[weapon_slot_idx].ammo--;

                if (roll < hit_chance) {
                    MOX::_missile[missile_idx].quantity--;
                    if (MOX::_missile[missile_idx].quantity <= 0) {
                        CMBTMIS::Kill_Missile_(missile_idx);
                    }
                    count_destroyed++;
                }
            }
        }

        if (count_destroyed > 0 && MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            ERICNET::Mox_Play_Sound_(0x1c);
            s_animation_header* anim_seg = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 7, MOX::_global_cache_seg);
            int16_t frame_count = animate::Get_Animation_Frame_Count_(anim_seg);

            BEAMS::Add_Damage_Indicator_For_Missile_(target_x, target_y, 1, count_destroyed);

            if (frame_count > 0) {
                for (int16_t frame = 0; frame < frame_count; ++frame) {
                    timer::Mark_Time_();
                    video::Set_Page_Off_();
                    CMBTDRW1::Draw_Main_Combat_Screen_();
                    graphics::Set_Window_(0, 0, 639, 350);
                    video::Clip_On_();

                    if (MOX::_dont_draw_ship == ship_idx) {
                        CMBTDRW1::Draw_Ship_(ship_idx, ship_x - cur_combat_x_offset, ship_y - cur_combat_y_offset);
                    }

                    s_animation_header* anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 7, MOX::_global_cache_seg);
                    animate::Set_Animation_Frame_(anim, frame);
                    animate::Draw_(target_x - 12, target_y - 12, anim);

                    video::Clip_Off_();
                    graphics::Reset_Window_();
                    BEAMS::Draw_Damage_Message_Queue_();
                    RUSS::Mox_Sync_Update_();
                    timer::Release_Time_(1);
                }
            }
        }
    }

    // Suggested name: Draw_Transporter_Bomb_Beam
    void __cdecl Draw_Transporter_Bomb_Beam_(int16_t ship_idx, int16_t target_x, int16_t target_y) {
        int16_t start_x, start_y;
        s_animation_header* anim;
        s_animation_header* buffer;
        int16_t frame_idx = 0;
        int32_t sound_handle;

        CMBTDRW1::Get_Ship_Center_Draw_XY_(ship_idx, &start_x, &start_y);
        graphics::Save_Window_();

        anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 15, MOX::_global_cache_seg);
        animate::Draw_Palette_(anim);

        allocate::Push_Block_(MOX::_screen_seg);
        buffer = reinterpret_cast<s_animation_header*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, 50000 /* 0xC350 */));

        MOX::_dont_draw_ship = ship_idx;
        sound_handle = ERICNET::Mox_Play_Sound_(0x14);
        palette::Restore_Glass_Remap_Block_(MOX::_stasis_bubble_remap_colors);

        for (int16_t i = 8; i > 0; --i) {
            timer::Mark_Time_();
            video::Set_Page_Off_();
            CMBTDRW1::Draw_Main_Combat_Screen_();
            
            graphics::Set_Window_(0, 0, 639 /* 0x27f */, 350 /* 0x15e */);
            video::Clip_On_();

            anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 15, MOX::_global_cache_seg);
            animate::Set_Animation_Frame_(anim, frame_idx % 3);
            bitmap::Draw_Picture_To_Bitmap_(buffer, anim);
            bitmap::Set_Picture_Glassed_(buffer);
            bitmap::Draw_Bitmap_Line_(start_x, start_y, target_x, target_y, buffer);

            graphics::Set_Window_(0, 0, 639 /* 0x27f */, 350 /* 0x15e */);
            video::Clip_On_();

            int16_t ship_draw_x = (MOX::_combat_data[ship_idx].x - MOX::_cur_combat_x) * 20;
            int16_t ship_draw_y = (MOX::_combat_data[ship_idx].y - MOX::_cur_combat_y) * 20;

            frame_idx++;
            CMBTDRW1::Draw_Ship_(ship_idx, ship_draw_x, ship_draw_y);

            video::Clip_Off_();
            graphics::Reset_Window_();
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);
        }

        sound::Fadeout_Sound_(static_cast<int16_t>(sound_handle), 10);
        MOX::_dont_draw_ship = -1;
        graphics::Restore_Window_();
        allocate::Pop_Block_(MOX::_screen_seg);
        palette::Restore_Glass_Remap_Block_(MOX::_shield_remap_colors);
    }

    void __cdecl Missile_Launch_FX_(int16_t missile_idx, int16_t tx, int16_t ty, int16_t is_targeting_missile) {
        int16_t max_dist;
        int16_t step;
        int16_t dist_acc;
        uint8_t hit_flag;
        int sound_handle;
        s_missile* missile;
        int16_t target_unit_idx;

        hit_flag = 0;
        max_dist = 8;
        missile = &MOX::_missile[missile_idx];
        target_unit_idx = missile->target_unit_idx;

        if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
            step = 9;
        } else {
            step = 3;
        }

        if (missile->type == 0x13 || missile->type == 0x28) {
            max_dist = 10000;
            missile->travel_dist = 10000;
        }

        if (MOX::_super_fast_flag == 0) {
            switch (missile->type) {
            case 0x0E:
                sound_handle = ERICNET::Mox_Play_Sound_(0x20);
                sound::Pitch_Bend_Sound_(sound_handle, 500);
                break;
            case 0x0F:
                ERICNET::Mox_Play_Sound_(0x20);
                break;
            case 0x10:
                sound_handle = ERICNET::Mox_Play_Sound_(0x20);
                sound::Pitch_Bend_Sound_(sound_handle, -1500);
                break;
            case 0x11:
                sound_handle = ERICNET::Mox_Play_Sound_(0x20);
                sound::Pitch_Bend_Sound_(sound_handle, -2500);
                break;
            case 0x12:
                ERICNET::Mox_Play_Sound_(0x3D);
                break;
            case 0x13:
                ERICNET::Mox_Play_Sound_(0x3C);
                break;
            case 0x14:
                ERICNET::Mox_Play_Sound_(0x3B);
                break;
            default:
                ERICNET::Mox_Play_Sound_(0x20);
                break;
            }
        }

        dist_acc = 0;
        if (max_dist > 0) {
            do {
                if (hit_flag != 0) {
                    break;
                }

                hit_flag = (uint8_t)CMBTMIS::Move_Missile_(missile_idx, tx, ty, step << 2);

                if (MOX::_super_fast_flag == 0) {
                    video::Set_Page_Off_();
                    CMBTDRW1::Draw_Main_Combat_Screen_();
                    RUSS::Mox_Sync_Update_();
                }

                if (is_targeting_missile == 1) {
                    if (MOX::_missile[target_unit_idx].quantity <= 0) {
                        hit_flag = 0;
                        CMBTMIS::Retarget_Missile_(missile_idx);
                    }
                } else {
                    if (MOX::_combat_data[target_unit_idx].unit_status == 0) {
                        hit_flag = 0;
                        CMBTMIS::Retarget_Missile_(missile_idx);
                    }
                }

                if (hit_flag != 0) {
                    CMBTMIS::Resolve_Missile_(missile_idx, tx, ty);
                }

                dist_acc += step;
            } while (dist_acc < max_dist);
        }
    }

    void __cdecl Stellar_Converter_(int16_t attacker_idx, int16_t target_idx, int16_t num_shots) {
        int16_t x1, y1, x2, y2;
        int16_t x_off, y_off;
        s_animation_header* beam_anim;
        s_animation_header* impact_anim;
        s_animation_header* temp_bitmap;
        int16_t frame_count;
        int16_t frame_step;
        uint32_t sound_handle;

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            if (CMBTDRW1::Ship_Completely_On_Screen_(target_idx) == 0) {
                COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[target_idx].x, MOX::_combat_data[target_idx].y);
            }

            Buffer_Reload_(COLONY::_CMBTSFX_LBX, 41, MOX::_global_cache_seg);
            beam_anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 40, MOX::_global_cache_seg);
            animate::Draw_Palette_(beam_anim);
            palette::Refresh_Palette_();

            allocate::Push_Block_(MOX::_screen_seg);
            temp_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 50000);

            COMBINIT::Ship_Center_Offsets_(attacker_idx, &x_off, &y_off);
            x1 = x_off + (MOX::_combat_data[attacker_idx].x - MOX::_cur_combat_x) * 20;
            y1 = y_off + (MOX::_combat_data[attacker_idx].y - MOX::_cur_combat_y) * 20;

            COMBINIT::Ship_Center_Offsets_(target_idx, &x_off, &y_off);
            x2 = x_off + (MOX::_combat_data[target_idx].x - MOX::_cur_combat_x) * 20;
            y2 = y_off + (MOX::_combat_data[target_idx].y - MOX::_cur_combat_y) * 20;

            frame_count = animate::Get_Animation_Frame_Count_(beam_anim);
            if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
                frame_step = 2;
            } else {
                frame_step = 1;
            }

            sound_handle = ERICNET::Mox_Play_Sound_(20);

            for (int16_t frame_idx = 0; frame_idx < frame_count; frame_idx += frame_step) {
                timer::Mark_Time_();
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();

                beam_anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 40, MOX::_global_cache_seg);
                animate::Set_Animation_Frame_(beam_anim, (int32_t)frame_idx);
                bitmap::Draw_Picture_To_Bitmap_(temp_bitmap, beam_anim);

                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();
                bitmap::Draw_Bitmap_Line_(x1, y1, x2, y2, temp_bitmap);

                if (frame_idx > 8) {
                    impact_anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 41, MOX::_global_cache_seg);
                    animate::Set_Animation_Frame_(impact_anim, frame_idx - 9);
                    palette::Restore_Glass_Remap_Block_(MOX::_stellar_converter_remap_colors);
                    animate::Draw_(x2 - 60, y2 - 60, impact_anim);
                }

                graphics::Reset_Window_();
                video::Clip_Off_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(2);
            }

            sound::Fadeout_Sound_(sound_handle, 10);
            allocate::Pop_Block_(MOX::_screen_seg);
        }

        int16_t total_damage = 0;
        int16_t total_struct_damage = 0;
        int16_t bonus = MOX::_ordnance_bonus[MOX::_combat_data[attacker_idx].owner];
        int16_t base_damage = TECHDATA::_weapons[38].damage;
        int16_t damage_per_hit = (bonus * num_shots * base_damage) / 100;

        for (int16_t i = 0; i < 4; ++i) {
            int16_t current_damage = damage_per_hit;
            int16_t current_struct_damage = 0;
            int16_t has_analyzer = COMBINIT::Does_Combat_Ship_Have_Special_(attacker_idx, 1);
            int16_t has_focus = COMBINIT::Does_Combat_Ship_Have_Special_(attacker_idx, 0x20);

            CMBTFIRE::Apply_Ship_Damage_(target_idx, &current_damage, &current_struct_damage, i, 0, 0, has_focus, has_analyzer, 0);
            total_damage += current_damage;
            total_struct_damage += current_struct_damage;
        }

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            palette::Restore_Glass_Remap_Block_(MOX::_shield_remap_colors);
            BEAMS::Add_Damage_Indicator_For_Ship_(
                (MOX::_combat_data[target_idx].x - MOX::_cur_combat_x) * 20,
                (MOX::_combat_data[target_idx].y - MOX::_cur_combat_y) * 20,
                target_idx, 1, total_damage, total_struct_damage
            );
            BEAMS::Draw_Damage_Message_Queue_Until_Done_();
        }
    }

    void __cdecl Plasma_Web_(int16_t param_1, int16_t param_2, int16_t param_3) {
        int16_t accumulated_damage = 0;
        s_combat_data* source_ship = &MOX::_combat_data[param_1];
        s_combat_data* target_ship = &MOX::_combat_data[param_2];
        int16_t shots_left = source_ship->weapons[param_3].shots_left;
        int16_t weapon_id = source_ship->weapons[param_3].weapon_id;

        if (MOX::_apply_damage_to_planet != 1 && MOX::_super_fast_flag == 0) {
            if (!CMBTDRW1::Ship_Completely_On_Screen_(param_2)) {
                COMBINIT::Snap_Center_Combat_Screen_(target_ship->x, target_ship->y);
            }

            s_animation_header* travel_seg = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 0xE /* Travel sprite */, MOX::_global_cache_seg);
            int16_t travel_frames = animate::Get_Animation_Frame_Count_(travel_seg);

            int16_t start_x, start_y;
            COMBINIT::Ship_Center_Offsets_(param_1, &start_x, &start_y);
            start_x += (source_ship->x - MOX::_cur_combat_x) * 20 - 20;
            start_y += (source_ship->y - MOX::_cur_combat_y) * 20 - 20;

            int16_t dest_x, dest_y;
            COMBINIT::Ship_Center_Offsets_(param_2, &dest_x, &dest_y);
            dest_x += (target_ship->x - MOX::_cur_combat_x) * 20 - 20;
            dest_y += (target_ship->y - MOX::_cur_combat_y) * 20 - 20;

            int32_t travel_sound = ERICNET::Mox_Play_Sound_(0x1B);
            int16_t step = (MOX::_auto_combat == 0) ? 6 : 24;

            int16_t current_x = start_x;
            int16_t current_y = start_y;
            int16_t travel_frame_idx = 0;

            while (current_x != dest_x || current_y != dest_y) {
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();

                s_animation_header* anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 0xE, MOX::_global_cache_seg);
                animate::Set_Animation_Frame_(anim, (int32_t)travel_frame_idx);
                animate::Draw_(current_x, current_y, anim);
                video::Clip_Off_();

                travel_frame_idx = (travel_frame_idx + 1) % travel_frames;
                graphics::Reset_Window_();
                RUSS::Mox_Sync_Update_();
                special::Absolute_Interpolate_Line_(&current_x, &current_y, start_x, start_y, dest_x, dest_y, step);
            }

            sound::Fadeout_Sound_(travel_sound, 10);

            int16_t impact_entry;
            switch (target_ship->size_class) {
                case 0: impact_entry = 0x24; break;
                case 1:
                case 2: impact_entry = 0x25; break;
                case 3: impact_entry = 0x26; break;
                case 4:
                case 5: impact_entry = 0x27; break;
                default: impact_entry = 0x25; break;
            }

            s_animation_header* impact_seg = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, impact_entry, MOX::_global_cache_seg);
            int16_t impact_frames = animate::Get_Animation_Frame_Count_(impact_seg);
            int16_t impact_x = current_x + (20 - animate::Get_Width_(impact_seg) / 2);
            int16_t impact_y = current_y + (20 - animate::Get_Height_(impact_seg) / 2);

            int32_t hit_sound = ERICNET::Mox_Play_Sound_(0x44);
            int16_t impact_frame_idx = 0;
            int16_t frame_inc = (MOX::_auto_combat == 0) ? 1 : 2;

            while (impact_frame_idx < impact_frames) {
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();

                palette::Restore_Glass_Remap_Block_(MOX::_plasma_web_remap_colors);

                int16_t cur_impact_entry;
                switch (target_ship->size_class) {
                    case 0: cur_impact_entry = 0x24; break;
                    case 1:
                    case 2: cur_impact_entry = 0x25; break;
                    case 3: cur_impact_entry = 0x26; break;
                    case 4:
                    case 5: cur_impact_entry = 0x27; break;
                    default: cur_impact_entry = 0x25; break;
                }

                s_animation_header* anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, cur_impact_entry, MOX::_global_cache_seg);
                animate::Set_Animation_Frame_(anim, (int32_t)impact_frame_idx);
                animate::Draw_(impact_x, impact_y, anim);

                palette::Restore_Glass_Remap_Block_(MOX::_shield_remap_colors);
                video::Clip_Off_();
                graphics::Reset_Window_();
                RUSS::Mox_Sync_Update_();

                impact_frame_idx += frame_inc;
            }

            sound::Fadeout_Sound_(hit_sound, 10);
        }

        int16_t ord_bonus = MOX::_ordnance_bonus[source_ship->owner];
        int16_t min_dmg = (TECHDATA::_weapons[weapon_id].min_damage * ord_bonus) / 100;
        int16_t max_dmg = (TECHDATA::_weapons[weapon_id].damage * ord_bonus) / 100;
        int16_t diff = max_dmg - min_dmg;

        if (shots_left > 0) {
            for (int16_t i = 0; i < shots_left; ++i) {
                accumulated_damage += (random::Random_(100) * diff / 100) + min_dmg;
            }
        }

        target_ship->plasma_web_damage += accumulated_damage;
    }

    void __cdecl Destroy_Ship_FX_(int16_t ship_idx, int16_t x, int16_t y, int16_t explosion_type) {
        int16_t off_x = 0;
        int16_t off_y = 0;
        int16_t frame_count;
        int16_t step;
        int16_t i;
        int16_t old_dont_draw;
        s_animation_header* anim = nullptr;
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        if (CMBTDRW1::Ship_Completely_On_Screen_(ship_idx) == 0) {
            timer::Mark_Time_();
            COMBINIT::Snap_Center_Combat_Screen_(x / 20, y / 20);
            timer::Release_Time_(7);
        }

        COMBINIT::Ship_Center_Offsets_(ship_idx, &off_x, &off_y);

        if (ship->previous_owner < 10) {
            if (explosion_type != 1) {
                int16_t damage = CMBTFIR2::Damage_From_Ship_Exploding_(ship_idx);
                Do_Spherical_SFX_(ship_idx, x, y, explosion_type, damage);
                ship->unit_status = 5 /* DESTROYED */;
                return;
            }

            old_dont_draw = MOX::_dont_draw_ship;

            switch (ship->size_class) {
                case 0:
                    off_x -= 12;
                    off_y -= 9;
                    anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 3, MOX::_global_cache_seg);
                    break;
                case 1:
                case 2:
                    off_x -= 23;
                    off_y -= 20;
                    anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 4, MOX::_global_cache_seg);
                    break;
                case 3:
                case 4:
                case 5:
                    off_x -= 47;
                    off_y -= 44;
                    anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 5, MOX::_global_cache_seg);
                    break;
            }

            frame_count = animate::Get_Animation_Frame_Count_(anim);

            int16_t screen_off_x = (MOX::_cur_combat_x * -20) + off_x + x;
            int16_t screen_off_y = (MOX::_cur_combat_y * -20) + off_y + y;

            ship->plasma_web_damage = 0;
            MOX::_dont_draw_ship = ship_idx;

            video::Set_Page_Back_();
            CMBTDRW1::Full_Draw_Combat_Screen_();
            video::Copy_Back_To_Off_();

            if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
                step = 2;
            } else {
                step = 1;
            }

            Play_Ship_Explosion_SFX_((int16_t)ship->size_class);

            int16_t half_frame = frame_count / 2;

            for (i = 0; i < frame_count; i += step) {
                timer::Mark_Time_();
                if (i >= half_frame) {
                    ship->unit_status = 5 /* DESTROYED */;
                }

                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();

                if (ship->unit_status == 0) {
                    CMBTDRW1::Draw_Ship_(ship_idx, (MOX::_cur_combat_x * -20) + x, (MOX::_cur_combat_y * -20) + y);
                }

                animate::Set_Animation_Frame_(anim, i);
                animate::Draw_(screen_off_x, screen_off_y, anim);

                video::Clip_Off_();
                graphics::Reset_Window_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(1);
            }

            MOX::_dont_draw_ship = old_dont_draw;
            COMBAT1::Assign_Combat_Grids_(MOX::_cur_ship);
            CMBTDRW1::Refresh_Combat_Screen_Full_();
        } else {
            int16_t facing_dir = ship->facing_dir;
            CMBTDRW1::Get_Draw_Ship_Offsets_(ship_idx, &off_x, &off_y);

            int16_t screen_off_x = (MOX::_cur_combat_x * -20) + off_x + x;
            int16_t screen_off_y = (MOX::_cur_combat_y * -20) + off_y + y;

            ship->plasma_web_damage = 0;
            ship->unit_status = 5 /* DESTROYED */;

            video::Set_Page_Back_();
            CMBTDRW1::Full_Draw_Combat_Screen_();

            if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
                step = 2;
            } else {
                step = 1;
            }

            uint8_t color_map[32];
            for (int k = 0; k < 32; ++k) {
                color_map[k] = (uint8_t)(k + 0x60);
            }

            int quadrant = (facing_dir - 1) / 4;
            if (quadrant < 0) quadrant = 0;

            for (i = 0; i < 16; i += step) {
                timer::Mark_Time_();
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();

                int entry_id;
                int16_t sfx_id;
                char* monster_lbx = ESTRINGS::E_Strings_(0xE0);

                switch (ship->previous_owner) {
                    case 10:
                        entry_id = (ship->size_class == 2) ? 0x48 : 0x47;
                        sfx_id = 0x3F;
                        break;
                    case 11:
                        entry_id = (ship->size_class == 2) ? 0x4A : 0x49;
                        sfx_id = 0x42;
                        break;
                    case 12:
                        entry_id = (ship->size_class == 2) ? 0x46 : 0x45;
                        sfx_id = 0x41;
                        break;
                    case 13:
                        entry_id = (ship->size_class == 2) ? 0x4C : 0x4B;
                        sfx_id = 0x40;
                        break;
                    case 14:
                        entry_id = (ship->size_class == 2) ? 0x4E : 0x4D;
                        sfx_id = 0x3E;
                        break;
                    default:
                        entry_id = 0;
                        sfx_id = -1;
                        break;
                }

                anim = (s_animation_header*)Buffer_Reload_(monster_lbx, entry_id, MOX::_global_cache_seg);

                if (i == 0 && sfx_id != -1) {
                    ERICNET::Mox_Play_Sound_(sfx_id);
                }

                switch (quadrant) {
                    case 0:
                        animate::Set_Animation_Frame_(anim, (facing_dir << 4) + i);
                        bitmap::Draw_Picture_To_Bitmap_(MOX::_draw_ship_bitmap, anim);
                        break;
                    case 1:
                        animate::Set_Animation_Frame_(anim, ((8 - facing_dir) << 4) + i);
                        bitmap::Draw_Picture_To_Bitmap_(MOX::_ship_bitmap, anim);
                        bitmap::Flip_Bitmap_(MOX::_draw_ship_bitmap, MOX::_ship_bitmap, 0);
                        break;
                    case 2:
                        animate::Set_Animation_Frame_(anim, ((facing_dir - 8) << 4) + i);
                        bitmap::Draw_Picture_To_Bitmap_(MOX::_draw_ship_bitmap, anim);
                        bitmap::Flip_Bitmap_(MOX::_ship_bitmap, MOX::_draw_ship_bitmap, 0);
                        bitmap::Flip_Bitmap_(MOX::_draw_ship_bitmap, MOX::_ship_bitmap, 3);
                        break;
                    case 3:
                        animate::Set_Animation_Frame_(anim, ((16 - facing_dir) << 4) + i);
                        bitmap::Draw_Picture_To_Bitmap_(MOX::_ship_bitmap, anim);
                        bitmap::Flip_Bitmap_(MOX::_draw_ship_bitmap, MOX::_ship_bitmap, 3);
                        break;
                }

                if (ship->owner == MOX::_combat_defender_player) {
                    replace::Replace_Color_Range_(MOX::_draw_ship_bitmap, color_map, 32, 32);
                }

                animate::Draw_(screen_off_x, screen_off_y, MOX::_draw_ship_bitmap);

                video::Clip_Off_();
                graphics::Reset_Window_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(1);
            }

            video::Set_Page_Off_();
            CMBTDRW1::Draw_Main_Combat_Screen_();
            RUSS::Mox_Sync_Update_();
            COMBAT1::Assign_Combat_Grids_(MOX::_cur_ship);
            CMBTDRW1::Refresh_Combat_Screen_Full_();
        }
    }

    /**
     * @address 004a59c0
     */
    void __cdecl Draw_Anti_Missile_Rocket_Picture_(int16_t x, int16_t y) {
        int16_t x1 = x - 1;
        int16_t x2 = x + 1;
        int16_t y1 = y - 1;
        int16_t y2 = y + 1;

        // Fill background square
        graphics::Fill_(x1, y1, x2, y2, 25);

        // Draw accented corners
        fields::Clipped_Dot_(x1, y1, 16);
        fields::Clipped_Dot_(x1, y2, 16);
        fields::Clipped_Dot_(x2, y1, 16);
        fields::Clipped_Dot_(x2, y2, 16);
    }

    void __cdecl Play_Ship_Explosion_SFX_(int16_t param_1) {
        int32_t random_val = random::Random_(3);
        int16_t switch_idx = static_cast<int16_t>(random_val + param_1 - 1);

        switch (switch_idx) {
        case 0:
            ERICNET::Mox_Play_Sound_(0x1d);
            break;
        case 1:
            ERICNET::Mox_Play_Sound_(0x18);
            break;
        case 2:
            ERICNET::Mox_Play_Sound_(0x18);
            break;
        case 3:
            ERICNET::Mox_Play_Sound_(0x18);
            break;
        case 4:
            ERICNET::Mox_Play_Sound_(0x18);
            break;
        case 5:
            ERICNET::Mox_Play_Sound_(0x1e);
            ERICNET::Mox_Play_Sound_(0x2b);
            break;
        default:
            ERICNET::Mox_Play_Sound_(0x18);
            break;
        }
    }

    void __cdecl Draw_Gyro_Special_Effect_(int16_t param_1, int16_t param_2) {
        int16_t x1, y1;
        int16_t x2, y2;
        int16_t step;

        s_animation_header* anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 0x44, MOX::_global_cache_seg);
        animate::Draw_Palette_(anim);

        anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 8, MOX::_global_cache_seg);
        int16_t frame_count = animate::Get_Animation_Frame_Count_(anim);

        allocate::Push_Block_(MOX::_screen_seg);
        uint8_t* block = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 50000);

        COMBINIT::Ship_Center_Offsets_(param_1, &x1, &y1);
        x1 += (MOX::_combat_data[param_1].x - MOX::_cur_combat_x) * 20;
        y1 += (MOX::_combat_data[param_1].y - MOX::_cur_combat_y) * 20;

        COMBINIT::Ship_Center_Offsets_(param_2, &x2, &y2);
        x2 += (MOX::_combat_data[param_2].x - MOX::_cur_combat_x) * 20;
        y2 += (MOX::_combat_data[param_2].y - MOX::_cur_combat_y) * 20;

        if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
            step = 2;
        } else {
            step = 1;
        }

        int16_t frame = 0;
        int32_t sound_id = ERICNET::Mox_Play_Sound_(11);

        if (frame_count > 0) {
            do {
                timer::Mark_Time_();
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();

                anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 8, MOX::_global_cache_seg);
                animate::Set_Animation_Frame_(anim, (int32_t)frame);
                bitmap::Draw_Picture_To_Bitmap_((s_animation_header*)block, anim);

                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();

                bitmap::Draw_Bitmap_Line_(x1, y1, x2, y2, block);

                CMBTDRW1::Draw_Ship_(
                    param_1,
                    (int16_t)((MOX::_combat_data[param_1].x - MOX::_cur_combat_x) * 20),
                    (int16_t)((MOX::_combat_data[param_1].y - MOX::_cur_combat_y) * 20)
                );

                CMBTDRW1::Draw_Ship_(
                    param_2,
                    (int16_t)((MOX::_combat_data[param_2].x - MOX::_cur_combat_x) * 20),
                    (int16_t)((MOX::_combat_data[param_2].y - MOX::_cur_combat_y) * 20)
                );

                graphics::Reset_Window_();
                video::Clip_Off_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(1);

                frame += step;
            } while (frame < frame_count);
        }

        sound::Fadeout_Sound_(sound_id, 10);

        frame = 0;
        sound_id = ERICNET::Mox_Play_Sound_(11);

        do {
            timer::Mark_Time_();
            video::Set_Page_Off_();
            CMBTDRW1::Draw_Main_Combat_Screen_();

            graphics::Set_Window_(0, 0, 639, 350);
            video::Clip_On_();

            anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 8, MOX::_global_cache_seg);
            animate::Set_Animation_Frame_(anim, (int32_t)((frame % 3) + frame_count - 3));
            bitmap::Draw_Picture_To_Bitmap_((s_animation_header*)block, anim);

            bitmap::Draw_Bitmap_Line_(x1, y1, x2, y2, block);

            int32_t dir = MOX::_combat_data[param_2].facing_dir + 1;
            int32_t sign = dir >> 31;
            MOX::_combat_data[param_2].facing_dir = (int8_t)(((((dir ^ sign) - sign) & 0xF) ^ sign) - sign);

            CMBTDRW1::Draw_Ship_(
                param_2,
                (int16_t)((MOX::_combat_data[param_2].x - MOX::_cur_combat_x) * 20),
                (int16_t)((MOX::_combat_data[param_2].y - MOX::_cur_combat_y) * 20)
            );

            CMBTDRW1::Draw_Ship_(
                param_1,
                (int16_t)((MOX::_combat_data[param_1].x - MOX::_cur_combat_x) * 20),
                (int16_t)((MOX::_combat_data[param_1].y - MOX::_cur_combat_y) * 20)
            );

            video::Clip_Off_();
            graphics::Reset_Window_();
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(2);

            frame += step;
        } while (frame < 8);

        allocate::Pop_Block_(MOX::_screen_seg);
        sound::Fadeout_Sound_(sound_id, 10);
    }

    void __cdecl Draw_Ship_Collapsing_(int16_t ship_idx, int16_t x, int16_t y, int16_t scale, int8_t rotation_offset) {
        int16_t center_off_x;
        int16_t center_off_y;

        if (scale > 99) {
            scale = 99;
        }

        if (scale < 2) {
            // Mark ship status as destroyed/finished collapsing
            MOX::_combat_data[ship_idx].unit_status = 5;
            return;
        }

        // 0x62 = 98 pixels
        bitmap::Create_Blank_Bitmap_(98, 98, MOX::_combat_bitmap);
        bitmap::Create_Blank_Bitmap_(98, 98, MOX::_draw_ship_bitmap);

        // Temporarily rotate the ship for this draw call
        MOX::_combat_data[ship_idx].facing_dir += rotation_offset;
        CMBTDRW1::Draw_Ship_To_Bitmap_(ship_idx, 0, 0, 0);
        MOX::_combat_data[ship_idx].facing_dir -= rotation_offset;

        // Scale the source ship bitmap into the combat buffer
        bitmap::Scale_Bitmap_(MOX::_combat_bitmap, MOX::_draw_ship_bitmap, scale, scale);

        // Calculate frame offsets based on ship size/picture and draw to screen
        COMBINIT::Ship_Center_Offsets_(ship_idx, &center_off_x, &center_off_y);
        ERIC::Draw_Centered_(center_off_x + x, center_off_y + y, MOX::_combat_bitmap);
    }

    void __cdecl Do_Spherical_SFX_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4, int16_t param_5) {
        static const int16_t anim_frames[125] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0xd, 0xd, 0x16, 0x2e, 0x3d, 0x3d, 0x56, 0x5f, 0x6b, 0x72, 0x76,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0xd, 0x10, 0x19, 0x1e, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23,
            0, 0,
            0xb, 0x15, 0x2d, 0x40, 0x41, 0x55, 0x61, 0x6c, 0x73, 0x73,
            0, 0, 0
        };

        int16_t center_dx_2;
        int16_t center_dy_2;
        int16_t dummy_hits;

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0 && CMBTDRW1::Ship_Completely_On_Screen_(param_1) == 0) {
            COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[param_1].x, MOX::_combat_data[param_1].y);
        }

        int16_t center_x, center_y;
        COMBINIT::Ship_Center_Offsets_(param_1, &center_x, &center_y);
        center_x = center_x + MOX::_cur_combat_x * -20 + param_2;
        center_y = center_y + MOX::_cur_combat_y * -20 + param_3;

        allocate::Push_Block_(MOX::_screen_seg);
        COLONY::Load_Ship_Bitm_();

        int16_t sfx_type = param_4;
        bool draw_ships_collapsing_flag = true;
        void* remap_colors = nullptr;

        switch (sfx_type) {
        case 0:
        case 1:
            draw_ships_collapsing_flag = true;
            remap_colors = MOX::_pulsar_remap_colors;
            ERICNET::Mox_Play_Sound_(0x14);
            break;
        case 2:
            draw_ships_collapsing_flag = true;
            ERICNET::Mox_Play_Sound_(0x14);
            break;
        case 3:
            remap_colors = MOX::_warp_core_remap_colors;
            draw_ships_collapsing_flag = false;
            ERICNET::Mox_Play_Sound_(0x2F);
            ERICNET::Mox_Play_Sound_(8);
            if (random::Random_(10) == 1) {
                ERICNET::Mox_Play_Sound_(0x21);
            }
            break;
        case 4:
            remap_colors = MOX::_warp_core_remap_colors;
            draw_ships_collapsing_flag = false;
            ERICNET::Mox_Play_Sound_(0x19);
            ERICNET::Mox_Play_Sound_(8);
            break;
        }

        s_animation_header* anim = nullptr;
        switch (sfx_type) {
        case 0:
        case 1:
            anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 0x34, MOX::_global_cache_seg);
            break;
        case 2:
            anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 6, MOX::_global_cache_seg);
            break;
        case 3:
            anim = (s_animation_header*)Buffer_Reload_(COLONY::_SPHERSFX_LBX, 0, MOX::_global_cache_seg);
            break;
        case 4:
            anim = (s_animation_header*)Buffer_Reload_(COLONY::_SPHERSFX_LBX, 1, MOX::_global_cache_seg);
            break;
        }

        int16_t width = animate::Get_Width_(anim);
        int16_t frame_count = animate::Get_Animation_Frame_Count_(anim);
        int32_t radius_sq = (width / 2) * (width / 2);

        int16_t ship_indices[50];
        int16_t ship_dist_ratios[50];
        int32_t ship_dist_sq[50];
        int32_t ships_in_range_count = 0;

        if (MOX::_combat_total_ship_count > 1) {
            for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[i].unit_status == 0 &&
                    CMBTMOV1::Ship_Is_In_Stasis_(i) == 0 &&
                    i != param_1 &&
                    MOX::_combat_data[i].unit_status != 5)
                {
                    int16_t cx, cy;
                    CMBTDRW1::Get_Ship_Center_Draw_XY_(i, &cx, &cy);
                    int32_t dx = (int32_t)cx - center_x;
                    int32_t dy = (int32_t)cy - center_y;
                    int32_t dist_sq = dx * dx + dy * dy;

                    if (dist_sq <= radius_sq) {
                        int32_t abs_dy = dy;
                        if (abs_dy < 0) abs_dy = -abs_dy;
                        int32_t abs_dx = dx;
                        if (abs_dx < 0) abs_dx = -abs_dx;

                        int32_t max_d = abs_dy;
                        if (abs_dx > abs_dy) {
                            max_d = abs_dx;
                        }

                        ship_indices[ships_in_range_count] = i;
                        ship_dist_sq[ships_in_range_count] = dist_sq;
                        ship_dist_ratios[ships_in_range_count] = (int16_t)(max_d / 20);
                        ships_in_range_count++;
                    }
                }
            }
        }

        COMBINIT::Ship_Center_Offsets_(param_1, &center_dx_2, &center_dy_2);

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            MOX::_dont_draw_ship = param_1;
            video::Set_Page_Back_();
            CMBTDRW1::Draw_Main_Combat_Screen_();
            video::Copy_Back_To_Off_();

            int16_t step = 1;
            if (MOX::_net_flag != 0 || MOX::_speedx2_flag == 1) {
                step = 2;
            }

            int16_t ship_collapse_3_val = 100;
            int16_t ship_collapse_4_val = 132;

            for (int16_t frame = 0; frame < frame_count; frame += step) {
                graphics::Reset_Window_();
                timer::Mark_Time_();
                video::Set_Page_Off_();
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();

                if (!draw_ships_collapsing_flag && MOX::_combat_data[param_1].unit_status != 5) {
                    if (sfx_type == 1 || sfx_type == 0) {
                        CMBTDRW1::Draw_Ship_(param_1, center_x - center_dx_2, center_y - center_dy_2);
                    }
                    else if (sfx_type == 3) {
                        Draw_Ship_Collapsing_(param_1, center_x - center_dx_2, center_y - center_dy_2, ship_collapse_3_val, (int8_t)frame);
                    }
                    else if (sfx_type == 4) {
                        Draw_Ship_Collapsing_(param_1, center_x - center_dx_2, center_y - center_dy_2, ship_collapse_4_val, (int8_t)frame);
                    }
                }

                int32_t frame_offset = sfx_type * 25;
                int16_t frame_val = anim_frames[frame_offset + frame];
                int32_t frame_val_sq = (int32_t)frame_val * frame_val;

                for (int32_t j = 0; j < ships_in_range_count; ++j) {
                    bool draw_ship = true;
                    if ((sfx_type == 0 || sfx_type == 1 || sfx_type == 2) &&
                        ((frame < 5 && ship_dist_ratios[j] == 1) ||
                            (frame > 0 && frame < 7 && ship_dist_ratios[j] == 2) ||
                            (frame > 4 && frame < 13 && ship_dist_ratios[j] == 3) ||
                            (frame > 8 && ship_dist_ratios[j] == 4)))
                    {
                        int16_t ship_idx = ship_indices[j];
                        int16_t cx = MOX::_combat_data[ship_idx].x - MOX::_cur_combat_x;
                        cx = cx * 20 - 3 + (int16_t)random::Random_(5);
                        int16_t cy = MOX::_combat_data[ship_idx].y - MOX::_cur_combat_y;
                        cy = cy * 20 - 3 + (int16_t)random::Random_(5);
                        CMBTDRW1::Draw_Ship_(ship_idx, cx, cy);
                        draw_ship = false;
                    }
                    else if (sfx_type != 0 && sfx_type != 1) {
                        int32_t dist_sq = ship_dist_sq[j];
                        int32_t size_bonus = (int32_t)MOX::_combat_data[ship_indices[j]].size_class * 25;
                        int32_t max_dist = frame_val_sq + size_bonus;

                        if (dist_sq <= max_dist) {
                            if (frame_val < 85 || (frame_val >= 85 && dist_sq <= frame_val_sq - 625)) {
                                int16_t ship_idx = ship_indices[j];
                                int16_t cx = MOX::_combat_data[ship_idx].x - MOX::_cur_combat_x;
                                cx = cx * 20 - 3 + (int16_t)random::Random_(5);
                                int16_t cy = MOX::_combat_data[ship_idx].y - MOX::_cur_combat_y;
                                cy = cy * 20 - 3 + (int16_t)random::Random_(5);
                                CMBTDRW1::Draw_Ship_(ship_idx, cx, cy);
                                draw_ship = false;
                            }
                        }
                    }

                    if (draw_ship) {
                        int16_t ship_idx = ship_indices[j];
                        int16_t cx = (MOX::_combat_data[ship_idx].x - MOX::_cur_combat_x) * 20;
                        int16_t cy = (MOX::_combat_data[ship_idx].y - MOX::_cur_combat_y) * 20;
                        CMBTDRW1::Draw_Ship_(ship_idx, cx, cy);
                    }
                }

                if (sfx_type != 2) {
                    palette::Restore_Glass_Remap_Block_(remap_colors);
                }

                switch (sfx_type) {
                case 0:
                case 1:
                    anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 0x34, MOX::_global_cache_seg);
                    break;
                case 2:
                    anim = (s_animation_header*)Buffer_Reload_(COLONY::_CMBTSFX_LBX, 6, MOX::_global_cache_seg);
                    break;
                case 3:
                    anim = (s_animation_header*)Buffer_Reload_(COLONY::_SPHERSFX_LBX, 0, MOX::_global_cache_seg);
                    break;
                case 4:
                    anim = (s_animation_header*)Buffer_Reload_(COLONY::_SPHERSFX_LBX, 1, MOX::_global_cache_seg);
                    break;
                }

                animate::Set_Animation_Frame_(anim, (int32_t)frame);
                ERIC::Draw_Centered_(center_x, center_y, anim);

                if (sfx_type != 2) {
                    palette::Restore_Glass_Remap_Block_(MOX::_shield_remap_colors);
                }

                if (draw_ships_collapsing_flag && MOX::_combat_data[param_1].unit_status != 5) {
                    if (sfx_type == 1 || sfx_type == 0 || sfx_type == 2) {
                        CMBTDRW1::Draw_Ship_(param_1, center_x - center_dx_2, center_y - center_dy_2);
                    }
                    else if (sfx_type == 3) {
                        Draw_Ship_Collapsing_(param_1, center_x - center_dx_2, center_y - center_dy_2, ship_collapse_3_val, (int8_t)frame);
                    }
                    else if (sfx_type == 4) {
                        Draw_Ship_Collapsing_(param_1, center_x - center_dx_2, center_y - center_dy_2, ship_collapse_3_val + 40, (int8_t)frame);
                    }
                }

                video::Clip_Off_();
                graphics::Reset_Window_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(1);

                ship_collapse_3_val -= step * 8;
                ship_collapse_4_val -= step * 10;
            }

            MOX::_dont_draw_ship = -1;
        }

        for (int32_t i = 0; i < 300; ++i) {
            if (MOX::_missile[i].type > 0 &&
                TECHDATA::_weapons[MOX::_missile[i].type].rack != 2 &&
                (TECHDATA::_weapons[MOX::_missile[i].type].rack != 4 || random::Random_(2) != 1))
            {
                int32_t mx = (int32_t)MOX::_missile[i].x + (int32_t)MOX::_cur_combat_x * -20;
                int32_t my = (int32_t)MOX::_missile[i].y + (int32_t)MOX::_cur_combat_y * -20;
                int32_t dx = mx - center_x;
                if (dx < 0) dx = -dx;
                int32_t dy = my - center_y;
                if (dy < 0) dy = -dy;
                int32_t dist_sq = dx * dx + dy * dy;

                if (dist_sq <= radius_sq) {
                    int16_t damage = 0;
                    int16_t owner = CMBTMIS::Missile_Owner_((int16_t)i);
                    int32_t kill_ratio = 0;

                    if (sfx_type == 0) {
                        for (int16_t k = 0; k < param_5; ++k) {
                            damage += TECHDATA::_weapons[44].min_damage + (int16_t)random::Random_(TECHDATA::_weapons[44].damage -
                                TECHDATA::_weapons[44].min_damage + 1) - 1;
                        }
                        damage = (int16_t)((MOX::_ordnance_bonus[MOX::_combat_data[param_1].owner] * damage) / 100);
                    }
                    else if (sfx_type == 1) {
                        for (int16_t k = 0; k < param_5; ++k) {
                            damage += TECHDATA::_weapons[36].min_damage + (int16_t)random::Random_(TECHDATA::_weapons[36].damage -
                                TECHDATA::_weapons[36].min_damage + 1) - 1;
                        }
                        damage = (int16_t)((MOX::_ordnance_bonus[MOX::_combat_data[param_1].owner] * damage) / 100);
                    }
                    else if (sfx_type == 2) {
                        for (int16_t k = 0; k < param_5; ++k) {
                            damage += TECHDATA::_weapons[13].min_damage + (int16_t)random::Random_(TECHDATA::_weapons[13].damage -
                                TECHDATA::_weapons[13].min_damage + 1) - 1;
                        }
                        damage = (int16_t)((MOX::_ordnance_bonus[MOX::_combat_data[param_1].owner] * damage) / 100);
                    }
                    else {
                        kill_ratio = (dist_sq * 100) / radius_sq;
                        damage = param_5;
                    }

                    int32_t chance = ((100 - kill_ratio) * damage) / 100;
                    int16_t missile_hits = CMBTMIS::Get_Missile_Hits_(MOX::_missile[i].type, owner, MOX::_missile[i].specials);
                    int16_t qty = MOX::_missile[i].quantity;

                    if (qty > 0) {
                        for (int16_t k = 0; k < qty; ++k) {
                            if (random::Random_(100) <= (int32_t)(chance * 25) / missile_hits) {
                                MOX::_missile[i].quantity--;
                            }
                        }
                    }

                    if (MOX::_missile[i].quantity <= 0) {
                        CMBTMIS::Kill_Missile_((int16_t)i);
                    }
                }
            }
        }

        for (int32_t j = 0; j < ships_in_range_count; ++j) {
            int16_t damage = 0;
            int32_t kill_ratio = 0;

            if (sfx_type == 0) {
                for (int16_t k = 0; k < param_5; ++k) {
                    damage += TECHDATA::_weapons[44].min_damage + (int16_t)random::Random_(TECHDATA::_weapons[44].damage -
                        TECHDATA::_weapons[44].min_damage + 1) - 1;
                }
                damage = (int16_t)((MOX::_ordnance_bonus[MOX::_combat_data[param_1].owner] * damage) / 100);
            }
            else if (sfx_type == 1) {
                for (int16_t k = 0; k < param_5; ++k) {
                    damage += TECHDATA::_weapons[36].min_damage + (int16_t)random::Random_(TECHDATA::_weapons[36].damage -
                        TECHDATA::_weapons[36].min_damage + 1) - 1;
                }
                damage = (int16_t)((MOX::_ordnance_bonus[MOX::_combat_data[param_1].owner] * damage) / 100);
            }
            else if (sfx_type == 2) {
                for (int16_t k = 0; k < param_5; ++k) {
                    damage += TECHDATA::_weapons[13].min_damage + (int16_t)random::Random_(TECHDATA::_weapons[13].damage -
                        TECHDATA::_weapons[13].min_damage + 1) - 1;
                }
                damage = (int16_t)((MOX::_ordnance_bonus[MOX::_combat_data[param_1].owner] * damage) / 100);
            }
            else {
                kill_ratio = (ship_dist_sq[j] * 100) / radius_sq;
                damage = param_5;
            }

            int32_t chance = ((100 - kill_ratio) * (int32_t)damage) / 100;
            if (chance < 2) {
                chance = 1;
            }

            dummy_hits = 0;
            int16_t struct_damage = 0;
            int16_t ship_idx = ship_indices[j];

            if (MOX::_combat_data[ship_idx].size_class != -1 && MOX::_combat_data[ship_idx].size_class + 1 > 0) {
                for (int16_t k = 0; k < MOX::_combat_data[ship_idx].size_class + 1; ++k) {
                    int32_t rnd = random::Random_((int32_t)chance);
                    if (rnd <= 1) {
                        dummy_hits++; // Original game bug: increments an unused stack variable here
                    }
                    else {
                        center_dx_2 += (int16_t)random::Random_((int32_t)chance); // Original game bug: accumulates in center_dx_2
                    }
                }
            }

            if (sfx_type == 2) {
                MOX::_combat_data[ship_idx].structure_damage += (int16_t)chance;
                struct_damage = 0;
                dummy_hits = (int16_t)chance;
            }
            else {
                int16_t facing = CMBTDRW1::Relative_Bearing_(ship_idx, param_1);
                facing = CMBTFIRE::Facing_Shield_(facing);
                CMBTFIRE::Apply_Ship_Damage_(ship_idx, &center_dx_2, &struct_damage, facing, 0, 0, 0, 0, 0);
            }

            if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                BEAMS::Add_Damage_Indicator_For_Ship_(
                    (MOX::_combat_data[ship_idx].x - MOX::_cur_combat_x) * 20,
                    (MOX::_combat_data[ship_idx].y - MOX::_cur_combat_y) * 20,
                    ship_idx, 1, center_dx_2, struct_damage);
            }
        }

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            BEAMS::Draw_Damage_Message_Queue_Until_Done_();
        }

        for (int32_t j = 0; j < ships_in_range_count; ++j) {
            int16_t ship_idx = ship_indices[j];
            if (MOX::_combat_data[ship_idx].unit_status == 0) {
                int16_t dead = CMBTFIRE::Is_Combat_Ship_Dead_(ship_idx);
                if (dead != 0) {
                    CMBTFIRE::Destroy_Ship_(ship_idx, (int16_t)MOX::_combat_data[ship_idx].x * 20, (int16_t)MOX::_combat_data[ship_idx].y * 20,
                        dead);
                }
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            CMBTDRW1::Refresh_Combat_Screen_Full_();
        }
    }
}