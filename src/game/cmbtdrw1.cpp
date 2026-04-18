#include "pch.h"

namespace CMBTDRW1 {

    void __cdecl Draw_Shield_Arcs_() {
        if (MOX::_cur_ship == 0) {
            return;
        }

        char str[4][4] = {0};
        if (MOX::_settings.language == 4) {
            strcpy(str[0], s_A_00557ed4);
            strcpy(str[1], s_D_005569dc);
            strcpy(str[2], s_I_0055801c);
            strcpy(str[3], s_S_005569cc);
        } else {
            strcpy(str[0], s_F_005598f4);
            strcpy(str[1], s_R_0055824c);
            strcpy(str[2], s_B_005560e4);
            strcpy(str[3], s_L_005560e8);
        }

        int16_t local_3a[7];
        local_3a[1] = 15;
        local_3a[2] = 22;
        local_3a[3] = 25;
        local_3a[4] = 30;
        local_3a[5] = 33;
        local_3a[6] = 33;

        int16_t cy;
        COMBINIT::Ship_Center_Offsets_(MOX::_cur_ship, local_3a, &cy);

        int16_t cx = local_3a[0];
        cx += MOX::_combat_data[MOX::_cur_ship].x * 20;
        cy += MOX::_combat_data[MOX::_cur_ship].y * 20;

        int8_t facing = MOX::_combat_data[MOX::_cur_ship].facing_dir;

        int16_t x2[4];
        int16_t y2[4];
        int16_t dx[4];
        int16_t dy[4];

        int16_t sz = local_3a[MOX::_combat_data[MOX::_cur_ship].size_class + 1];

        int idx0 = (facing / 4) % 4;
        int idx1 = (idx0 + 1) % 4;
        int idx2 = (idx0 + 2) % 4;
        int idx3 = (idx0 + 3) % 4;

        switch (facing) {
            case 0:
            case 4:
            case 8:
            case 12: {
                x2[0] = 2000;  y2[0] = 2000;
                x2[1] = -2000; y2[1] = -2000;
                x2[2] = 2000;  y2[2] = -2000;
                x2[3] = -2000; y2[3] = 2000;

                dx[idx0] = sz;  dy[idx0] = 0;
                dx[idx1] = 0;   dy[idx1] = sz;
                dx[idx2] = -sz; dy[idx2] = 0;
                dx[idx3] = 0;   dy[idx3] = -sz;
                break;
            }
            case 1:
            case 5:
            case 9:
            case 13: {
                x2[0] = 3827;  y2[0] = -9239;
                x2[1] = -9239; y2[1] = -3827;
                x2[2] = -3827; y2[2] = 9239;
                x2[3] = 9239;  y2[3] = 3827;

                int16_t v23 = (sz * 23) / 25;
                int16_t v19 = (sz * 19) / 50;

                dx[idx0] = v23;  dy[idx0] = -v19;
                dx[idx1] = v19;  dy[idx1] = v23;
                dx[idx2] = -v23; dy[idx2] = v19;
                dx[idx3] = -v19; dy[idx3] = -v23;
                break;
            }
            case 2:
            case 6:
            case 10:
            case 14: {
                x2[0] = 0;     y2[0] = 2000;
                x2[1] = -2000; y2[1] = 0;
                x2[2] = 0;     y2[2] = -2000;
                x2[3] = 2000;  y2[3] = 0;

                dx[idx0] = sz;  dy[idx0] = -sz;
                dx[idx1] = sz;  dy[idx1] = sz;
                dx[idx2] = -sz; dy[idx2] = sz;
                dx[idx3] = -sz; dy[idx3] = -sz;
                break;
            }
            case 3:
            case 7:
            case 11:
            case 15: {
                x2[0] = 9239;  y2[0] = -3827;
                x2[1] = -3827; y2[1] = -9239;
                x2[2] = -9239; y2[2] = 3827;
                x2[3] = 3827;  y2[3] = 9239;

                int16_t v23 = (sz * 23) / 25;
                int16_t v19 = (sz * 19) / 50;

                dx[idx0] = v19;  dy[idx0] = -v23;
                dx[idx1] = v23;  dy[idx1] = v19;
                dx[idx2] = -v19; dy[idx2] = v23;
                dx[idx3] = -v23; dy[idx3] = -v19;
                break;
            }
            default:
                break;
        }

        fonts::Set_Font_Style_(0, &MOX::_combat_green);

        for (int i = 0; i < 4; i++) {
            dx[i] -= 2;
            dy[i] -= 3;

            int x1 = cx - MOX::_cur_combat_x * 20;
            int y1 = cy - MOX::_cur_combat_y * 20;
            int x2_coord = x2[i] - MOX::_cur_combat_x * 20 + cx;
            int y2_coord = y2[i] - MOX::_cur_combat_y * 20 + cy;

            line::Line_(x1, y1, x2_coord, y2_coord, 0x54);

            fonts::Print_(x1 + dx[i], y1 + dy[i], str[i]);
        }
    }

    void __cdecl Detailed_View_Ship_(int16_t ship_idx) {
        char ship_name[16];
        bool is_editable = (ship_idx != -1);

        EVANHELP::Save_Help_List_();
        MOX2::Get_Auto_Function_(&_k_save);
        video::Set_Page_Back_();
        MOX::_ship_frame--;
        MOX2::Execute_Auto_Function_(&_k_save);
        fields::Draw_Visible_Fields_Back_();
        video::Copy_Back_To_Off_();
        RUSS::Mox_Sync_Update_();
        fields::Assign_Auto_Function_(Combat_Draw_View_Ship_, 1);
        allocate::Push_Block_(MOX::_screen_seg);

        if (is_editable) {
            MOX::_ship_data_id = -1;
            COMBINIT::Load_Display_Combat_Ship_(ship_idx);
            
            // BUG: Potential buffer overflow if name is longer than 15 characters
            strcpy(ship_name, MOX::_global_combat_data.name);

            INITSHIP::Get_Ship_Combat_Bonuses_(
                ship_idx, 
                &MOX::_global_combat_data.ship_defense, 
                &MOX::_global_combat_data.ship_missle_defense, 
                &MOX::_global_combat_data.ship_attack
            );

            uint32_t bitmap_size = bitmap::Get_Bitmap_Size_(98, 98);
            MOX::_draw_ship_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);
            MOX::_combat_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);

            s_animation_header* anim = (s_animation_header*)Buffer_Reload_("Combat.lbx", 0xB, MOX::_global_cache_seg);
            remap::Calculate_Picture_Remap_Colors_(anim);

            NEWGAME::_color_ken.colors[0] = remap::Get_Remap_Palette_Color_(0x84);
            NEWGAME::_color_ken.colors[1] = remap::Get_Remap_Palette_Color_(0x8C);
            NEWGAME::_color_ken.colors[2] = remap::Get_Remap_Palette_Color_(0x8A);
            NEWGAME::_color_ken.colors[3] = remap::Get_Remap_Palette_Color_(0x8C);
            NEWGAME::_color_ken.colors[4] = remap::Get_Remap_Palette_Color_(0x8C);
            NEWGAME::_color_ken.colors[5] = remap::Get_Remap_Palette_Color_(0x8C);
            NEWGAME::_color_ken.colors[6] = remap::Get_Remap_Palette_Color_(0x8C);
            NEWGAME::_color_ken.colors[7] = remap::Get_Remap_Palette_Color_(0x8C);
        }

        if (MOX::_global_combat_data.officer_idx == -1) {
            int16_t p_idx = (int16_t)MOX::_global_combat_data.player_idx;
            if (p_idx > 7) p_idx = 0;
            
            uint8_t race = (uint8_t)MOX::_player[p_idx].race;
            MOX::_detail_fleet_icon_seg = (s_animation_header*)farload::Far_Reload_Next_("flticons.lbx", race, MOX::_screen_seg);
        } else {
            int16_t mode = is_editable ? 0 : 2;
            MOX::_detail_fleet_icon_seg = (s_animation_header*)OFFICER::Load_Officer_Picture_(0, MOX::_global_combat_data.officer_idx, MOX::_screen_seg, 0, mode);
        }

        fields::Mark_Fields_();

        if (is_editable) {
            fonts::Set_Font_Style_(4, &NEWGAME::_color_ken);
            fields::Add_Continuous_String_Input_Field_(0x106, 0x32, 0x67, 0xC, ship_name, 0xF, 0, 0, 0, 0);
            fields::Add_Hidden_Field_(0x10C, 0x32, 0x17F, 0x40, "", 0);
        }

        int16_t exit_field_id = fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, "", 0);
        bool exit_triggered = false;
        do {
            int16_t input = fields::Get_Input_();
            if (input == exit_field_id) {
                exit_triggered = true;
            }
            video::Set_Page_Off_();
            Combat_Draw_View_Ship_();
            RUSS::Mox_Sync_Update_();
        } while (!exit_triggered);

        if (is_editable) {
            strings::Trim_(ship_name);
            if (ship_name[0] != '\0') {
                strcpy(MOX::_ship[ship_idx].d.name, ship_name);
            }
        }

        fields::Release_Fields_();
        EVANHELP::Restore_Help_List_();
        MOX2::Put_Auto_Function_(&_k_save);
        allocate::Pop_Block_(MOX::_screen_seg);
    }

    /**
     * Calculates the relative bearing from ship_a to ship_b, adjusted for ship_a's current facing direction.
     * The result is in degrees [0, 359].
     */
    void __cdecl Draw_Legal_Boxes_() {
        if (MOX::_cur_ship == 0) {
            return;
        }

        s_combat_data* ship = &MOX::_combat_data[MOX::_cur_ship];
        bool can_move = false;

        if (ship->movement_left > 0) {
            can_move = true;
        } else {
            if (COMBINIT::Does_Combat_Ship_Have_Special_(MOX::_cur_ship, 0x21 /* 33 */) ||
                COMBINIT::Does_Combat_Ship_Have_Special_(MOX::_cur_ship, 0x16 /* 22 */)) {
                if (CMBTFIR2::Used_Teleporter_(MOX::_cur_ship) == 0) {
                    can_move = true;
                }
            }
        }

        if (can_move) {
            int16_t box_size;
            switch (ship->size_class) {
            case 0:
                box_size = 20; /* 0x14 */
                break;
            case 1:
            case 2:
                box_size = 40; /* 0x28 */
                break;
            case 3:
            case 4:
            case 5:
                box_size = 60; /* 0x3C */
                break;
            default:
                // Note: Fallback to uninitialized stack variable DI in assembly
                break;
            }

            uint8_t color = (MOX::_in_nebula_flag == 1) ? 0 : 0x81 /* 129 */;

            for (int16_t x = MOX::_cur_combat_x; x < MOX::_cur_combat_x + 32; ++x) {
                for (int16_t y = MOX::_cur_combat_y; y < MOX::_cur_combat_y + 18; ++y) {
                    if (MOX::_combat_grid[x][y] == 2000 /* 0x7D0 */) {
                        int16_t x1 = (x - MOX::_cur_combat_x) * 20;
                        int16_t y1 = (y - MOX::_cur_combat_y) * 20;
                        graphics::Fill_(x1, y1, x1 + box_size, y1 + box_size, color);
                    }
                }
            }
        }
    }

    int16_t __cdecl Relative_Bearing_(int16_t ship_a_idx, int16_t ship_b_idx) {
        int16_t x1, y1;
        int16_t x2, y2;
        int16_t bearing;
        int8_t facing;

        // Get coordinates for ship A
        COMBINIT::Ship_Center_Offsets_(ship_a_idx, &x1, &y1);
        x1 += MOX::_combat_data[ship_a_idx].x * 20;
        y1 += MOX::_combat_data[ship_a_idx].y * 20;

        // Get coordinates for ship B
        COMBINIT::Ship_Center_Offsets_(ship_b_idx, &x2, &y2);
        x2 += MOX::_combat_data[ship_b_idx].x * 20;
        y2 += MOX::_combat_data[ship_b_idx].y * 20;

        // Calculate angle between ships. Y-axis is inverted in screen space calculation here.
        bearing = angle::Get_Angle_(x2 - x1, y1 - y2);

        facing = MOX::_combat_data[ship_a_idx].facing_dir;

        // Subtract ship A's heading from the absolute bearing. 
        // Heading is in 16 discrete directions (0-15), where each unit is 22.5 degrees.
        // 22.5 * facing = 22 * facing + facing / 2
        bearing = bearing - (facing / 2) - (facing * 22);

        // Normalize angle to [0, 359]
        if (bearing < 0) {
            bearing += 360;
        }

        return bearing;
    }

    int16_t __cdecl Relative_Bearing_XY_(int16_t ship_idx, int16_t target_x, int16_t target_y) {
        int16_t off_x;
        int16_t off_y;
        int16_t result_bearing;
        s_combat_data* ship;

        COMBINIT::Ship_Center_Offsets_(ship_idx, &off_x, &off_y);

        ship = &MOX::_combat_data[ship_idx];

        off_x += (int16_t)ship->x * 20;
        off_y += (int16_t)ship->y * 20;

        result_bearing = angle::Get_Angle_((int32_t)(target_x - off_x), (int32_t)(off_y - target_y));

        result_bearing -= (int16_t)(ship->facing_dir / 2);
        result_bearing += (int16_t)(ship->facing_dir * -22);

        if (result_bearing < 0) {
            result_bearing += 360;
        }

        return result_bearing;
    }

    void __cdecl Draw_Internals_Display_(int16_t x_off, int16_t y_off) {
        char msg_buffer[80];
        char itoa_buffer[80];

        if (MOX::_cur_ship != 0) {
            s_combat_data* ship = &MOX::_combat_data[MOX::_cur_ship];

            fonts::Set_Font_Style_Shadow_Down_(2, &MOX::_combat_green);
            fonts::Set_Outline_Color_(0, 0);
            KEN::Ken_Get_Text_Message_(36, msg_buffer, 80);
            fonts::Print_Centered_(445, 362, msg_buffer);

            fonts::Set_Font_Style_Shadow_Down_(1, &MOX::_combat_green);
            fonts::Set_Outline_Color_(0, 0);

            int16_t current_y = y_off + 4;
            if (ship->ftl_type == 0) {
                KEN::Ken_Get_Text_Message_(37, msg_buffer, 80);
                fonts::Print_(x_off + 4, current_y, msg_buffer);
            } else {
                KEN::Ken_Get_Text_Message_(14, msg_buffer, 80);
                fonts::Print_(x_off + 4, current_y, msg_buffer);
                __itoa(ship->drive_current_hits, itoa_buffer, 10);
                fonts::Print_Right_(x_off + 85, current_y, itoa_buffer);
            }

            current_y += 10;
            if (ship->shield_type == 0) {
                KEN::Ken_Get_Text_Message_(38, msg_buffer, 80);
                fonts::Print_(x_off + 4, current_y, msg_buffer);
            } else {
                KEN::Ken_Get_Text_Message_(39, msg_buffer, 80);
                fonts::Print_(x_off + 4, current_y, msg_buffer);
                __itoa(ship->shield_current_hits, itoa_buffer, 10);
                fonts::Print_Right_(x_off + 85, current_y, itoa_buffer);
            }

            current_y += 10;
            KEN::Ken_Get_Text_Message_(21, msg_buffer, 80);
            fonts::Print_(x_off + 4, current_y, msg_buffer);
            __itoa(ship->computer_current_hits, itoa_buffer, 10);
            fonts::Print_Right_(x_off + 85, current_y, itoa_buffer);

            current_y += 10;
            KEN::Ken_Get_Text_Message_(16, msg_buffer, 80);
            fonts::Print_(x_off + 4, current_y, msg_buffer);
            __itoa(ship->structure_max - ship->structure_damage, itoa_buffer, 10);
            fonts::Print_Right_(x_off + 85, current_y, itoa_buffer);

            current_y += 10;
            KEN::Ken_Get_Text_Message_(18, msg_buffer, 80);
            fonts::Print_(x_off + 4, current_y, msg_buffer);
            __itoa(ship->armor_remaining, itoa_buffer, 10);
            fonts::Print_Right_(x_off + 85, current_y, itoa_buffer);

            current_y = y_off + 64;
            KEN::Ken_Get_Text_Message_(40, msg_buffer, 80);
            fonts::Print_(x_off + 4, current_y, msg_buffer);
            fonts::Print_Integer_Right_(x_off + 85, current_y, ship->current_speed);

            current_y = y_off + 75;
            KEN::Ken_Get_Text_Message_(90, msg_buffer, 80);
            fonts::Print_(x_off + 4, current_y, msg_buffer);
            fonts::Print_Integer_Right_(x_off + 85, current_y, ship->movement_left);
        }
    }

    void __cdecl Full_Draw_Combat_Screen_() {
        char local_ship_name[32];

        if (MOX::_apply_damage_to_planet != 0 && MOX::_super_fast_flag == 0) {
            if (MOX::_in_nebula_flag == 0) {
                graphics::Fill_(0, 0, 639, 479, 0);
            }
            RUSS::Mox_Update_();
            
            graphics::Interlaced_Fill_(15, 362, 102, 374, 0, 199);
            graphics::Interlaced_Fill_(401, 362, 489, 374, 0, 199);
            graphics::Interlaced_Fill_(401, 379, 488, 467, 0, 199);
            RUSS::Mox_Update_();

            if (MOX::_cur_ship > -1) {
                Draw_Internals_Display_(401, 379);
            }
            RUSS::Mox_Update_();

            MOX::_combat_background_seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx, 0, MOX::_global_cache_seg);
            RUSS::Mox_Update_();
            animate::Draw_(0, 351, MOX::_combat_background_seg);

            bool is_auto_combat = false;
            if (MOX::_PLAYER_NUM == MOX::_combat_attacker_player) {
                if (MOX::_auto_combat_flag[0] != 0) is_auto_combat = true;
            } else {
                if (MOX::_auto_combat_flag[1] != 0) is_auto_combat = true;
            }

            if (is_auto_combat) {
                animate::Set_Animation_Frame_((s_animation_header*)MOX::_auto_button_seg, 1);
                animate::Draw_(273, 364, (s_animation_header*)MOX::_auto_button_seg);
            }
            RUSS::Mox_Update_();

            graphics::Set_Window_(0, 0, 639, 350);
            video::Clip_On_();

            int16_t star_idx = (MOX::_in_nebula_flag == 1) ? 35 : 46;
            MOX::_combat_far_star = (s_animation_header*)Buffer_Reload_(s_combat_lbx, star_idx, MOX::_global_cache_seg);
            RUSS::Mox_Update_();

            animate::Draw_(MOX::_combat_far_x, MOX::_combat_far_y, MOX::_combat_far_star);
            animate::Draw_(MOX::_combat_far_x - 640, MOX::_combat_far_y, MOX::_combat_far_star);
            RUSS::Mox_Update_();
            animate::Draw_(MOX::_combat_far_x - 640, MOX::_combat_far_y - 480, MOX::_combat_far_star);
            animate::Draw_(MOX::_combat_far_x, MOX::_combat_far_y - 480, MOX::_combat_far_star);
            RUSS::Mox_Update_();

            if (MOX::_draw_legal_boxes_flag == 1 && MOX::_cur_ship > -1) {
                int8_t owner = MOX::_combat_data[MOX::_cur_ship].owner;
                if (owner < 8 && MOX::_player[owner].objectives == 100) {
                    Draw_Legal_Boxes_();
                }
            }
            RUSS::Mox_Update_();

            MOX::_combat_mid_star = (s_animation_header*)Buffer_Reload_(s_combat_lbx, 47, MOX::_global_cache_seg);
            RUSS::Mox_Update_();
            animate::Draw_(MOX::_combat_mid_x, MOX::_combat_mid_y, MOX::_combat_mid_star);
            animate::Draw_(MOX::_combat_mid_x - 640, MOX::_combat_mid_y, MOX::_combat_mid_star);
            RUSS::Mox_Update_();
            animate::Draw_(MOX::_combat_mid_x - 640, MOX::_combat_mid_y - 480, MOX::_combat_mid_star);
            animate::Draw_(MOX::_combat_mid_x, MOX::_combat_mid_y - 480, MOX::_combat_mid_star);
            RUSS::Mox_Update_();

            MOX::_combat_near_star = (s_animation_header*)Buffer_Reload_(s_combat_lbx, 48, MOX::_global_cache_seg);
            RUSS::Mox_Update_();
            animate::Draw_(MOX::_combat_near_x, MOX::_combat_near_y, MOX::_combat_near_star);
            animate::Draw_(MOX::_combat_near_x - 640, MOX::_combat_near_y, MOX::_combat_near_star);
            RUSS::Mox_Update_();
            animate::Draw_(MOX::_combat_near_x - 640, MOX::_combat_near_y - 480, MOX::_combat_near_star);
            animate::Draw_(MOX::_combat_near_x, MOX::_combat_near_y - 480, MOX::_combat_near_star);
            RUSS::Mox_Update_();

            video::Clip_Off_();
            graphics::Reset_Window_();

            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[i].unit_status == 0 && i != MOX::_dont_draw_ship) {
                    int8_t cooldown = MOX::_combat_data[i].special_cooldown;
                    if (cooldown != -1 && (int16_t)(uint8_t)cooldown != MOX::_dont_draw_ship) {
                        CMBTSPEC::Draw_Continuing_Tractor_(i);
                    }
                }
            }

            if (MOX::_cur_ship != -1) {
                fonts::Set_Font_Style_Shadow_Down_(2, &MOX::_combat_green);
                fonts::Set_Outline_Color_(0, 0);
                strcpy(local_ship_name, MOX::_combat_data[MOX::_cur_ship].name);
                BILL::Squeeze_Print_Centered_(59, 362, local_ship_name, 88);
                Draw_Ship_Status_(14, 376, MOX::_cur_ship);
                Draw_Grey_Damage_Bars_(14, 376, MOX::_cur_ship);
                Draw_Crew_Level_Stars_(MOX::_cur_ship, 15, 379);
            }
            RUSS::Mox_Update_();

            if (MOX::_grid_flag == 1) {
                for (int16_t x = 0; x < 640; x += 20) {
                    line::Line_(x, 0, x, 350, 4);
                }
                for (int16_t y = 0; y < 360; y += 20) {
                    line::Line_(0, y, 640, y, 4);
                }
            }
            RUSS::Mox_Update_();

            Draw_Reduced_Combat_Map_();
            MOX::_last_ship_status = -2;
        }
    }

    void __cdecl Draw_Special_Status_Display_() {
        s_colors color_active;
        s_colors color_inactive;
        s_colors color_damaged;
        char msg_buffer[80];
        int16_t visible_special_count = 0;
        int16_t y_pos = 0x17c;

        // Manual setup of color palettes based on disassembled byte writes
        memset(color_active.colors, 0x53 /* 'S' */, 8);
        color_active.colors[0] = 0x50; /* 'P' */

        memset(color_inactive.colors, 0x53 /* 'S' */, 8);
        color_inactive.colors[0] = 0xdf;

        memset(color_damaged.colors, 0xdc, 8);
        color_damaged.colors[0] = 0x48; /* 'H' */
        color_damaged.colors[7] = 0x4a; /* 'J' */

        graphics::Interlaced_Fill_(0x72, 0x17a, 0x107, 0x1d2, 0, 0xc7 /* 199 */);

        for (int16_t bit = 0; bit < 39; ++bit) {
            s_combat_data *ship = &MOX::_combat_data[MOX::_cur_ship];
            if (struct_::Test_Bit_Field_(ship->special_device_flags, bit)) {
                s_colors *current_color;
                
                if (struct_::Test_Bit_Field_(ship->special_device_damage_flags, bit)) {
                    current_color = &color_damaged;
                } else {
                    if (CMBTFIRE::Special_Is_On_(MOX::_cur_ship, visible_special_count)) {
                        current_color = &color_active;
                    } else {
                        current_color = &color_inactive;
                    }
                }

                visible_special_count++;

                fonts::Set_Font_Style_Shadow_Down_(1, current_color);
                fonts::Set_Outline_Color_(0, 0);
                fonts::Set_Font_Spacing_(1);
                fonts::Print_(0x7b, y_pos, TECHDATA::_specials[bit].name);
                
                y_pos += 11;
            }
        }

        if (visible_special_count == 0) {
            fonts::Set_Font_Style_Shadow_Down_(2, &MOX::_combat_green);
            fonts::Set_Outline_Color_(0, 0);
            KEN::Ken_Get_Text_Message_(7, msg_buffer, 0x50);
            fonts::Print_Centered_(187 /* 0xbb */, 418 /* 0x1a2 */, msg_buffer);
        }
    }

    void __cdecl Draw_Main_Combat_Screen_() {
        MOX::_combat_clock_frame++;
        if (MOX::_combat_clock_frame > 18) {
            MOX::_combat_clock_frame = 15;
        }

        uint8_t page_flag = video::Get_Current_Video_Page_Flag_();
        MOX::_auto_cmbt_button_value = MOX::_auto_combat;

        if (MOX::_apply_damage_to_planet == 0) return;
        if (MOX::_super_fast_flag != 0) return;

        int16_t ptr_x = mouse::Pointer_X_();
        int16_t ptr_y = mouse::Pointer_Y_();
        int16_t ptr_img = mouse::Get_Pointer_Image_Number_();
        int16_t input_idx = fields::Scan_Input_();
        int16_t hovered_missile = -1000;

        if (input_idx > 0 && MOX::_cur_missile_count >= input_idx) {
            int16_t active_missile_count = 0;
            for (int16_t i = 0; i < 300; ++i) {
                if (MOX::_cur_missile_count <= active_missile_count) {
                    break;
                }
                if (MOX::_missile[i].type > 0) {
                    if (CMBTMIS::On_Combat_Screen_((MOX::_missile[i].x - 8) / 20, (MOX::_missile[i].y - 8) / 20)) {
                        active_missile_count++;
                        if (active_missile_count == input_idx) {
                            hovered_missile = i;
                            break;
                        }
                    }
                }
            }
        }

        if (MOX::_cur_ship >= 0) {
            if (ptr_x / 20 < 32 && ptr_y / 20 < 18) {
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();

                int16_t size_class = MOX::_combat_data[MOX::_cur_ship].size_class;
                int16_t ship_draw_size;
                if (size_class == 0) {
                    ship_draw_size = 0;
                } else if (size_class > 2) {
                    ship_draw_size = 20;
                } else {
                    ship_draw_size = 10;
                }

                int16_t grid_x = (ptr_x - ship_draw_size + 12) / 20;
                int16_t grid_y = (ptr_y - ship_draw_size + 12) / 20;

                if (MOX::_combat_grid[grid_x + MOX::_cur_combat_x][grid_y + MOX::_cur_combat_y] == 2000 &&
                    MOX::_skan_flag == 0 &&
                    (MOX::_game_type == 1 || MOX::_combat_data[MOX::_cur_ship].owner == MOX::_PLAYER_NUM) &&
                    MOX::_order_info_flag == 1) {
                    
                    int16_t box_size;
                    switch (size_class) {
                        case 0:
                            box_size = 20;
                            break;
                        case 1:
                        case 2:
                            box_size = 40;
                            break;
                        case 3:
                        case 4:
                        case 5:
                            box_size = 60;
                            break;
                        default:
                            box_size = 0;
                            break;
                    }
                    
                    if (box_size > 0) {
                        graphics::Box_(grid_x * 20, grid_y * 20, box_size, box_size, 1, 177 /* 0xb1 */);
                    }
                }
                video::Clip_Off_();
                graphics::Reset_Window_();

                int16_t ptr_offset = mouse::Get_Pointer_Offset_();
                int16_t ptr_offset_y = mouse::Get_Pointer_Offset_();
                int16_t hov_grid_x = (ptr_x + ptr_offset) / 20 + MOX::_cur_combat_x;
                int16_t hov_grid_y = (ptr_y + ptr_offset_y) / 20 + MOX::_cur_combat_y;
                int16_t hov_ship = MOX::_combat_grid[hov_grid_x][hov_grid_y];

                if (MOX::_ship_status_display != 0) {
                    Draw_Special_Status_Display_();
                } else {
                    if (hovered_missile > -1) {
                        Draw_Weapon_Status_Display_(-1, hovered_missile);
                        graphics::Interlaced_Fill_(401, 362, 489, 374, 0, 199);
                        fonts::Set_Font_Style_Shadow_Down_(1, &MOX::_combat_green);
                        fonts::Set_Outline_Color_(0, 0);

                        char* weapon_name = TECHDATA::_weapons[MOX::_missile[hovered_missile].type].name;
                        BILL::Squeeze_Print_Centered_(445, 365, weapon_name, 89);

                        CMBTMIS::Draw_Missile_Status_(401, 379, hovered_missile);
                        MOX::_last_ship_status = -1;
                    } else {
                        if ((ptr_img == 12 || ptr_img == 1) && hov_ship != -1 && hov_ship < MOX::_combat_total_ship_count) {
                            Draw_Weapon_Status_Display_(hov_ship, -1);
                            if (hov_ship != MOX::_last_ship_status) {
                                if (MOX::_combat_data[hov_ship].unit_status == 0) {
                                    video::Set_Page_Back_();
                                    fonts::Set_Font_Style_Shadow_Down_(2, &MOX::_combat_green);
                                    fonts::Set_Outline_Color_(0, 0);
                                    graphics::Interlaced_Fill_(401, 362, 489, 374, 0, 199);
                                    
                                    char ship_name[32];
                                    strcpy(ship_name, MOX::_combat_data[hov_ship].name);
                                    BILL::Squeeze_Print_Centered_(445, 362, ship_name, 89);
                                    
                                    Draw_Ship_Status_(400, 376, hov_ship);
                                    Draw_Grey_Damage_Bars_(400, 376, hov_ship);
                                    Draw_Crew_Level_Stars_(hov_ship, 401, 379);

                                    if (page_flag == 1) {
                                        video::Set_Page_Off_();
                                    }
                                    MOX::_last_ship_status = hov_ship;
                                } else {
                                    video::Set_Page_Back_();
                                    graphics::Interlaced_Fill_(401, 362, 489, 374, 0, 199);
                                    graphics::Interlaced_Fill_(401, 379, 488, 467, 0, 199);
                                    Draw_Internals_Display_(401, 379);

                                    if (page_flag == 1) {
                                        video::Set_Page_Off_();
                                    }
                                    MOX::_last_ship_status = hov_ship;
                                }
                            }
                        } else {
                            if (MOX::_last_ship_status != 666 /* 0x29A */) {
                                video::Set_Page_Back_();
                                Draw_Weapon_Status_Display_(-1, -1);
                                graphics::Interlaced_Fill_(401, 362, 489, 374, 0, 199);
                                graphics::Interlaced_Fill_(401, 379, 488, 467, 0, 199);
                                Draw_Internals_Display_(401, 379);

                                if (page_flag == 1) {
                                    video::Set_Page_Off_();
                                }
                                MOX::_last_ship_status = 666;
                            }
                        }
                    }
                }
            } else {
                if (MOX::_last_ship_status != 666 /* 0x29A */) {
                    video::Set_Page_Back_();
                    if (MOX::_ship_status_display == 0) {
                        Draw_Weapon_Status_Display_(-1, -1);
                    } else {
                        Draw_Special_Status_Display_();
                    }
                    graphics::Interlaced_Fill_(401, 362, 489, 374, 0, 199);
                    graphics::Interlaced_Fill_(401, 379, 488, 467, 0, 199);
                    Draw_Internals_Display_(401, 379);

                    if (page_flag == 1) {
                        video::Set_Page_Off_();
                    }
                    MOX::_last_ship_status = 666;
                }
            }

            if (MOX::_shields_flag == 1) {
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();
                Draw_Shield_Arcs_();
                video::Clip_Off_();
                graphics::Reset_Window_();
            }
        }

        graphics::Set_Window_(0, 0, 639, 350);
        video::Clip_On_();

        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[i].unit_status == 0 && MOX::_dont_draw_ship != i) {
                    if (Ship_On_Screen_(i)) {
                        Draw_Ship_(i, (MOX::_combat_data[i].x - MOX::_cur_combat_x) * 20,
                                            (MOX::_combat_data[i].y - MOX::_cur_combat_y) * 20);
                    }
                }
            }
        }

        if (MOX::_cur_ship >= 0 && MOX::_dont_draw_ship != MOX::_cur_ship) {
            if (MOX::_combat_data[MOX::_cur_ship].unit_status == 0) {
                if (COMBAT1::Ship_Controlled_By_Computer_(MOX::_cur_ship) == 0) {
                    if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_PLAYER_NUM || MOX::_game_type == 1) {
                        int16_t frame = (MOX::_ship_frame / 2) % 4;
                        int16_t draw_x, draw_y;
                        s_animation_header* cursor_anim = nullptr;

                        switch (MOX::_combat_data[MOX::_cur_ship].size_class) {
                            case 0:
                                cursor_anim = MOX::_ship_cursor[0];
                                draw_x = (MOX::_combat_data[MOX::_cur_ship].x - MOX::_cur_combat_x) * 20 - 3;
                                draw_y = (MOX::_combat_data[MOX::_cur_ship].y - MOX::_cur_combat_y) * 20 - 3;
                                break;
                            case 1:
                            case 2:
                                cursor_anim = MOX::_ship_cursor[1];
                                draw_x = (MOX::_combat_data[MOX::_cur_ship].x - MOX::_cur_combat_x) * 20 - 5;
                                draw_y = (MOX::_combat_data[MOX::_cur_ship].y - MOX::_cur_combat_y) * 20 - 4;
                                break;
                            case 3:
                            case 4:
                            case 5:
                                cursor_anim = MOX::_ship_cursor[2];
                                draw_x = (MOX::_combat_data[MOX::_cur_ship].x - MOX::_cur_combat_x) * 20 - 8;
                                draw_y = (MOX::_combat_data[MOX::_cur_ship].y - MOX::_cur_combat_y) * 20 - 8;
                                break;
                        }

                        if (cursor_anim != nullptr) {
                            animate::Set_Animation_Frame_(cursor_anim, frame);
                            animate::Draw_(draw_x, draw_y, cursor_anim);
                        }
                    }
                }
            }
        }

        CMBTMIS::Draw_Missiles_();
        video::Clip_Off_();
        graphics::Reset_Window_();

        fonts::Set_Font_Style_Shadow_Down_(2, &MOX::_combat_green);
        fonts::Set_Outline_Color_(0, 0);

        if (MOX::_missile_warning > 0 && 
            MOX::_cur_ship >= 0 && 
            MOX::_combat_data[MOX::_cur_ship].owner < 8 && 
            MOX::_player[MOX::_combat_data[MOX::_cur_ship].owner].objectives == 100 /* 'd' */) {
            
            if (MOX::_missile_warning == 1) {
                int16_t ms_frame = animate::Get_Current_Frame_((s_animation_header*)MOX::_msl_warning_on_seg);
                animate::Reset_Animation_Frame_((s_animation_header*)MOX::_msl_warning_on_seg);
                if (ms_frame > 0) {
                    for (int16_t i = 0; i < ms_frame; ++i) {
                        animate::Draw_(500, 363, (s_animation_header*)MOX::_msl_warning_on_seg);
                    }
                }
                animate::Draw_(500, 363, (s_animation_header*)MOX::_msl_warning_on_seg);
                animate::Draw_(529, 407, (s_animation_header*)MOX::_warning_missile_seg);
            } else {
                Draw_Reduced_Combat_Map_();
                Draw_Reduced_Map_Box_();
            }
        } else {
            int16_t blip_x = (MOX::_combat_data[MOX::_cur_ship].x * 3) / 2 + 500;
            int16_t blip_y = (MOX::_combat_data[MOX::_cur_ship].y * 3) / 2 + 364;

            int16_t blip_frame_arr[4] = {0, 2, 5, 2};
            int16_t blip_frame = blip_frame_arr[(MOX::_ship_frame / 2) % 4];

            if (MOX::_cur_ship == 0) {
                int8_t p_size = MOX::_planet[MOX::_colony[MOX::_cmbt_colony_id].planet].size;
                if (p_size == 0) {
                    Draw_Blip4_(blip_x, blip_y, 0x60, blip_frame);
                } else if (p_size > 0 && p_size <= 4) {
                    allocate::Push_Block_(MOX::_screen_seg);
                    s_animation_header* p_bmp = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 400);
                    bitmap::Draw_Picture_To_Bitmap_(p_bmp, MOX::_combat_map_planet_seg);

                    for (int16_t i = 0x60; i < 0x70; ++i) {
                        int16_t shade = i - blip_frame;
                        if (shade < 0x60) shade = 0x60;
                        replace::Replace_Color_(p_bmp, i, shade);
                    }

                    animate::Draw_(blip_x + p_size - 7, blip_y + p_size - 7, p_bmp);
                    allocate::Pop_Block_(MOX::_screen_seg);
                }
            } else {
                int16_t blip_color = (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_combat_attacker_player) ? 0x20 : 0x60;

                switch (MOX::_combat_data[MOX::_cur_ship].size_class) {
                    case 0:
                        Draw_Blip2_(blip_x, blip_y, blip_color, blip_frame);
                        break;
                    case 1:
                    case 2:
                        Draw_Blip3_(blip_x, blip_y, blip_color, blip_frame);
                        break;
                    case 3:
                        Draw_Blip4_(blip_x, blip_y, blip_color, blip_frame);
                        break;
                    case 4:
                    case 5:
                        Draw_Blip5_(blip_x, blip_y, blip_color, blip_frame);
                        break;
                }
            }
            Draw_Reduced_Map_Box_();
        }

        MOX::_ship_frame++;
        if (MOX::_ship_frame == 32000) {
            MOX::_ship_frame = 0;
        }

        if (MOX::_order_info_flag == 0) {
            Draw_Fake_Combat_Buttons_();
            MOX::_combat_options_background_seg = (s_animation_header*)Buffer_Reload_("combat.lbx", 9, MOX::_global_cache_seg);
            animate::Draw_(248, 298, MOX::_combat_options_background_seg);
            Draw_Option_Panel_Lights_();
        }

        if (MOX::_auto_combat == 1) {
            Draw_Fake_Combat_Buttons_();
        }
    }

    void __cdecl Draw_Grey_Damage_Bars_(int16_t x, int16_t y, int16_t ship_idx)
    {
        s_animation_header* seg;

        seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x2B, MOX::_global_cache_seg);
        animate::Draw_(x - 1, y - 1, seg);

        seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x2C, MOX::_global_cache_seg);
        animate::Draw_(x, y + 0x4E, seg);

        if (ship_idx == 0)
        {
            seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
            animate::Draw_(x + 1, y + 0x4F, seg);

            if (CMBTFIRE::Does_Combat_Planet_Have_Defenses_() != 0)
            {
                int16_t struct_dmg = MOX::_combat_data[ship_idx].structure_damage;

                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x58, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x55, seg);

                int16_t armor_idx = INITSHIP::Best_Armor_(MOX::_combat_data[0].owner);
                int32_t absorption = TECHDATA::_armor[armor_idx].ships_bonus;
                int32_t denom = ((absorption * 4 + 400) * 25) / 100;
                int16_t bar_len = ((struct_dmg * 88) / denom) / 2;

                graphics::Set_Window_(0, 0, x + bar_len + 1, 0x1DF);
                video::Clip_On_();

                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x55, seg);

                graphics::Set_Window_(x - bar_len + 0x58, 0, 0x27F, 0x1DF);

                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x55, seg);

                video::Clip_Off_();
                graphics::Reset_Window_();
            }
            else
            {
                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x55, seg);
            }
            return;
        }

        int8_t previous_owner = MOX::_combat_data[ship_idx].previous_owner;
        
        if (previous_owner <= 9)
        {
            int16_t hull_size;
            
            if (MOX::_combat_data[ship_idx].ftl_type == 0)
            {
                if (MOX::_combat_defender_player == 8)
                {
                    hull_size = 8;
                }
                else
                {
                    hull_size = COMBINIT::Type_Of_Space_Station_(MOX::_cmbt_colony_id);
                }
            }
            else
            {
                hull_size = MOX::_combat_data[ship_idx].size_class;
            }

            int16_t hull_armor = TECHDATA::_hull_data[hull_size].armor_hp;
            int16_t armor_percent = TECHDATA::_armor[MOX::_combat_data[ship_idx].armor_type].ships_bonus;
            
            int32_t armor_total = hull_armor + (armor_percent * hull_armor) / 100;
            
            if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0xE) != 0)
            {
                armor_total *= 3;
            }

            int16_t armor_rem = MOX::_combat_data[ship_idx].armor_remaining;
            int32_t armor_dmg = armor_total - armor_rem;
            
            if (armor_dmg != 0)
            {
                int16_t bar_len = ((armor_dmg * 88) / armor_total) / 2;

                graphics::Set_Window_(0, 0, x + bar_len + 1, 0x1DF);
                video::Clip_On_();

                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x4F, seg);

                graphics::Set_Window_(x - bar_len + 0x58, 0, 0x27F, 0x1DF);

                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x4F, seg);

                video::Clip_Off_();
                graphics::Reset_Window_();
            }

            int16_t struct_dmg = MOX::_combat_data[ship_idx].structure_damage;
            
            if (struct_dmg > 0)
            {
                int16_t bar_len = (struct_dmg * 44) / MOX::_combat_data[ship_idx].structure_max;
                if (bar_len < 2)
                {
                    bar_len = 2;
                }

                graphics::Set_Window_(x + 1, 0, x + bar_len + 1, 0x1DF);
                video::Clip_On_();

                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x55, seg);

                video::Clip_Off_();
                graphics::Reset_Window_();
            }

            int16_t max_space = CMBTFIR2::Max_Total_Internal_Space_(ship_idx);
            int16_t total_space = CMBTFIR2::Total_Internal_Space_(ship_idx);
            int32_t space_dmg = max_space - total_space;

            if (space_dmg != 0)
            {
                int16_t bar_len = (space_dmg * 44) / max_space;
                if (bar_len < 2)
                {
                    bar_len = 2;
                }

                graphics::Set_Window_(x - bar_len + 0x58, 0, x + 0x58, 0x1DF);
                video::Clip_On_();

                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x55, seg);

                video::Clip_Off_();
                graphics::Reset_Window_();
            }
        }
        else
        {
            seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x59, MOX::_global_cache_seg);
            animate::Draw_(x + 1, y + 0x4F, seg);

            seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x58, MOX::_global_cache_seg);
            animate::Draw_(x + 1, y + 0x55, seg);

            int16_t struct_dmg = MOX::_combat_data[ship_idx].structure_damage;
            
            if (struct_dmg > 0)
            {
                int16_t bar_len = ((struct_dmg * 88) / MOX::_combat_data[ship_idx].structure_max) / 2;

                graphics::Set_Window_(0, 0, x + bar_len + 1, 0x1DF);
                video::Clip_On_();

                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x55, seg);

                graphics::Set_Window_(x - bar_len + 0x58, 0, 0x27F, 0x1DF);

                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x55, seg);

                video::Clip_Off_();
                graphics::Reset_Window_();
            }

            int16_t max_space = CMBTFIR2::Max_Total_Internal_Space_(ship_idx);
            int16_t total_space = CMBTFIR2::Total_Internal_Space_(ship_idx);
            int32_t space_dmg = max_space - total_space;

            if (space_dmg != 0)
            {
                int16_t bar_len = ((space_dmg * 88) / max_space) / 2;

                graphics::Set_Window_(0, 0, x + bar_len + 1, 0x1DF);
                video::Clip_On_();

                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x4F, seg);

                graphics::Set_Window_(x - bar_len + 0x58, 0, 0x27F, 0x1DF);

                seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x57, MOX::_global_cache_seg);
                animate::Draw_(x + 1, y + 0x4F, seg);

                video::Clip_Off_();
                graphics::Reset_Window_();
            }
        }
    }

    void __cdecl Refresh_Combat_Screen_Full_() {
        COMBAT1::Add_Tactical_Combat_Fields_();
        COMBAT1::Assign_Combat_Grids_(MOX::_cur_ship);

        if (MOX::_super_fast_flag == 0) {
            video::Set_Page_Back_();
            CMBTDRW1::Full_Draw_Combat_Screen_();

            if (MOX::_ship_status_display == 0) {
                CMBTDRW1::Draw_Weapon_Status_Display_(-1, -1);
            } else {
                CMBTDRW1::Draw_Special_Status_Display_();
            }

            graphics::Interlaced_Fill_(0x191, 0x16a, 0x1e9, 0x176, 0, 199 /* 0xc7 */);
            graphics::Interlaced_Fill_(0x191, 0x17b, 0x1e8, 0x1d3, 0, 199 /* 0xc7 */);

            CMBTDRW1::Draw_Internals_Display_(0x191, 0x17b);
            MOX::_last_ship_status = 666 /* 0x29a */;

            video::Copy_Back_To_Off_();
            video::Set_Page_Off_();
            CMBTDRW1::Draw_Main_Combat_Screen_();
            RUSS::Mox_Sync_Update_();
        }
    }

    void __cdecl Draw_Reduced_Combat_Map_() {
        int16_t x_pos, y_pos;
        int8_t planet_size;

        if (MOX::_cmbt_colony_id != -1) {
            y_pos = (static_cast<int16_t>(MOX::_combat_data[0].y) * 3 >> 1) + 364 /* 0x16c */;
            x_pos = (static_cast<int16_t>(MOX::_combat_data[0].x) * 3 >> 1) + 500 /* 0x1f4 */;

            planet_size = MOX::_planet[MOX::_colony[MOX::_cmbt_colony_id].planet].size;

            if (planet_size == 0) {
                Draw_Blip4_(x_pos, y_pos, 0x60, 0);
            } else if (planet_size >= 1 && planet_size <= 4) {
                animate::Draw_(x_pos + planet_size - 7, y_pos + planet_size - 7, MOX::_combat_map_planet_seg);
            }
        }

        for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
            if (MOX::_combat_data[i].unit_status == 0) {
                uint8_t color = (MOX::_combat_data[i].owner == MOX::_combat_attacker_player) ? 0x20 : 0x60;

                y_pos = (static_cast<int16_t>(MOX::_combat_data[i].y) * 3 >> 1) + 364 /* 0x16c */;
                x_pos = (static_cast<int16_t>(MOX::_combat_data[i].x) * 3 >> 1) + 500 /* 0x1f4 */;

                switch (MOX::_combat_data[i].size_class) {
                case 0:
                    Draw_Blip2_(x_pos, y_pos, color, 0);
                    break;
                case 1:
                case 2:
                    Draw_Blip3_(x_pos, y_pos, color, 0);
                    break;
                case 3:
                    Draw_Blip4_(x_pos, y_pos, color, 0);
                    break;
                case 4:
                case 5:
                    Draw_Blip5_(x_pos, y_pos, color, 0);
                    break;
                default:
                    break;
                }
            }
        }

        for (int16_t i = 0; i < 300; ++i) {
            if (MOX::_missile[i].type > 0) {
                y_pos = (static_cast<int16_t>(MOX::_missile[i].y / 20) * 3 >> 1) + 364 /* 0x16c */;
                x_pos = (static_cast<int16_t>(MOX::_missile[i].x / 20) * 3 >> 1) + 500 /* 0x1f4 */;
                graphics::Dot_(x_pos, y_pos, 0x1c);
            }
        }
    }

    void __cdecl Draw_Crew_Level_Stars_(int16_t ship_idx, int16_t x, int16_t y) {
        int16_t current_x;
        int16_t counter;
        char buffer[80];

        if (ship_idx != 0) {
            counter = 0;
            current_x = x;
            if (MOX::_combat_data[ship_idx].crew_quality > 0) {
                do {
                    counter++;
                    animate::Draw_(current_x, y, (s_animation_header*)MOX::_level_symbol_seg);
                    current_x += 11;
                } while (counter < MOX::_combat_data[ship_idx].crew_quality);
            }
        }

        fonts::Set_Font_Style_(2, &MOX::_combat_green);

        if (MOX::_combat_data[ship_idx].officer_idx > -1) {
            KEN::Ken_Get_Text_Message_(86, buffer, 80);
            fonts::Print_Right_(x + 87, y + 2, buffer);
        }
    }

    void __cdecl Draw_Reduced_Map_Box_() {
        int16_t base_y = (MOX::_cur_combat_y * 3) / 2;
        int16_t base_x = (MOX::_cur_combat_x * 3) / 2;

        // Top-left corner
        line::Line_(base_x + 499 /* 0x1f3 */, base_y + 363 /* 0x16b */, base_x + 506 /* 0x1fa */, base_y + 363 /* 0x16b */, 138 /* 0x8a */);
        line::Line_(base_x + 499 /* 0x1f3 */, base_y + 363 /* 0x16b */, base_x + 499 /* 0x1f3 */, base_y + 370 /* 0x172 */, 138 /* 0x8a */);

        // Bottom-left corner
        line::Line_(base_x + 499 /* 0x1f3 */, base_y + 392 /* 0x188 */, base_x + 506 /* 0x1fa */, base_y + 392 /* 0x188 */, 138 /* 0x8a */);
        line::Line_(base_x + 499 /* 0x1f3 */, base_y + 392 /* 0x188 */, base_x + 499 /* 0x1f3 */, base_y + 385 /* 0x181 */, 138 /* 0x8a */);

        // Top-right corner
        line::Line_(base_x + 549 /* 0x225 */, base_y + 363 /* 0x16b */, base_x + 542 /* 0x21e */, base_y + 363 /* 0x16b */, 138 /* 0x8a */);
        line::Line_(base_x + 549 /* 0x225 */, base_y + 363 /* 0x16b */, base_x + 549 /* 0x225 */, base_y + 370 /* 0x172 */, 138 /* 0x8a */);

        // Bottom-right corner
        line::Line_(base_x + 549 /* 0x225 */, base_y + 392 /* 0x188 */, base_x + 542 /* 0x21e */, base_y + 392 /* 0x188 */, 138 /* 0x8a */);
        line::Line_(base_x + 549 /* 0x225 */, base_y + 392 /* 0x188 */, base_x + 549 /* 0x225 */, base_y + 385 /* 0x181 */, 138 /* 0x8a */);
    }

    void __cdecl Draw_Fake_Combat_Buttons_() {
        animate::Set_Animation_Frame_((s_animation_header*)MOX::_weapons_button_seg, 1 - MOX::_ship_status_display);
        animate::Draw_(0x72, 0x169, (s_animation_header*)MOX::_weapons_button_seg);

        animate::Set_Animation_Frame_((s_animation_header*)MOX::_specials_button_seg, MOX::_ship_status_display);
        animate::Draw_(0xbb, 0x169, (s_animation_header*)MOX::_specials_button_seg);

        if (MOX::_order_info_flag == 1) {
            animate::Set_Animation_Frame_((s_animation_header*)MOX::_auto_button_seg, MOX::_auto_combat);
            animate::Draw_(0x111, 0x16c, (s_animation_header*)MOX::_auto_button_seg);

            animate::Set_Animation_Frame_((s_animation_header*)MOX::_scan_button_seg, MOX::_skan_flag);
            animate::Draw_(0x151, 0x16c, (s_animation_header*)MOX::_scan_button_seg);

            animate::Reset_Animation_Frame_((s_animation_header*)MOX::_board_button_seg);
            animate::Draw_(0x111, 0x188, (s_animation_header*)MOX::_board_button_seg);

            animate::Reset_Animation_Frame_((s_animation_header*)MOX::_flee_button_seg);
            animate::Draw_(0x151, 0x188, (s_animation_header*)MOX::_flee_button_seg);

            animate::Reset_Animation_Frame_((s_animation_header*)MOX::_wait_button_seg);
            animate::Draw_(0x111, 0x1a4, (s_animation_header*)MOX::_wait_button_seg);

            animate::Reset_Animation_Frame_((s_animation_header*)MOX::_cmbt_done_button_seg);
            animate::Draw_(0x151, 0x1a4, (s_animation_header*)MOX::_cmbt_done_button_seg);

            animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_info_button_seg, 0);
            animate::Draw_(0x12a, 0x1c0, (s_animation_header*)MOX::_cmbt_info_button_seg);
        } else {
            CMBTDRW1::Draw_Option_Panel_Lights_();

            animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_info_button_seg, 0);
            animate::Draw_(0x12a, 0x12d, (s_animation_header*)MOX::_cmbt_info_button_seg);

            animate::Set_Animation_Frame_((s_animation_header*)MOX::_self_destruct_seg, 0);
            animate::Draw_(0x11c, 0x15c, (s_animation_header*)MOX::_self_destruct_seg);
        }
    }

    void __cdecl Draw_Option_Panel_Lights_() {
        animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_option_seg, MOX::_msl_impact_warning_flag);
        animate::Draw_(0x10B, 0x182, (s_animation_header*)MOX::_cmbt_option_seg);

        animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_option_seg, MOX::_speedx2_flag);
        animate::Draw_(0x10B, 0x192, (s_animation_header*)MOX::_cmbt_option_seg);

        animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_option_seg, MOX::_draw_legal_boxes_flag);
        animate::Draw_(0x10B, 0x1A2, (s_animation_header*)MOX::_cmbt_option_seg);

        animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_option_seg, MOX::_shields_flag);
        animate::Draw_(0x10B, 0x1B2, (s_animation_header*)MOX::_cmbt_option_seg);

        animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_option_seg, MOX::_grid_flag);
        animate::Draw_(0x10B, 0x1C2, (s_animation_header*)MOX::_cmbt_option_seg);
    }

    /**
     * Draws a 2x2 pixel "blip" with varying intensities.
     * 
     * @param x Screen X coordinate
     * @param y Screen Y coordinate
     * @param intensity_a First component of intensity/color
     * @param intensity_b Second component of intensity/color
     */
    void __cdecl Draw_Blip2_(int16_t x, int16_t y, int16_t intensity_a, int16_t intensity_b) {
        int16_t base_intensity = intensity_a + intensity_b;

        // Top-left pixel
        graphics::Dot_(x, y, (uint8_t)(base_intensity + 9));

        // Top-right pixel
        graphics::Dot_(x + 1, y, (uint8_t)(base_intensity + 6));

        // Bottom-left pixel
        graphics::Dot_(x, y + 1, (uint8_t)(base_intensity + 6));

        // Bottom-right pixel
        graphics::Dot_(x + 1, y + 1, (uint8_t)(base_intensity + 4));
    }

    /**
     * Draws a 3x3 blip on the screen using multiple dots with varying color offsets.
     * The color of each dot is relative to the sum of p3 and p4.
     */
    void __cdecl Draw_Blip3_(int16_t x, int16_t y, int16_t p3, int16_t p4) {
        int16_t color_base = p3 + p4;

        // Row 0
        graphics::Dot_(x, y, (uint8_t)(color_base + 9));
        graphics::Dot_(x + 1, y, (uint8_t)(color_base + 6));
        graphics::Dot_(x + 2, y, (uint8_t)(color_base + 4));

        // Row 1
        graphics::Dot_(x, y + 1, (uint8_t)(color_base + 6));
        graphics::Dot_(x + 1, y + 1, (uint8_t)(color_base + 6));
        graphics::Dot_(x + 2, y + 1, (uint8_t)(color_base + 4));

        // Row 2
        graphics::Dot_(x, y + 2, (uint8_t)(color_base + 4));
        graphics::Dot_(x + 1, y + 2, (uint8_t)(color_base + 4));
        graphics::Dot_(x + 2, y + 2, (uint8_t)(color_base + 1));
    }

    void __cdecl Draw_Blip4_(int16_t x, int16_t y, int16_t base_color, int16_t color_offset) {
        int16_t color_sum = base_color + color_offset;

        graphics::Dot_(x, y, (uint8_t)(color_sum + 1));
        graphics::Dot_(x + 1, y, (uint8_t)(color_sum + 4));
        graphics::Dot_(x + 2, y, (uint8_t)(color_sum + 4));
        graphics::Dot_(x + 3, y, (uint8_t)(color_sum + 1));

        graphics::Dot_(x, y + 1, (uint8_t)(color_sum + 4));
        graphics::Dot_(x + 1, y + 1, (uint8_t)(color_sum + 9));
        graphics::Dot_(x + 2, y + 1, (uint8_t)(color_sum + 6));
        graphics::Dot_(x + 3, y + 1, (uint8_t)(color_sum + 4));

        graphics::Dot_(x, y + 2, (uint8_t)(color_sum + 4));
        graphics::Dot_(x + 1, y + 2, (uint8_t)(color_sum + 6));
        graphics::Dot_(x + 2, y + 2, (uint8_t)(color_sum + 6));
        graphics::Dot_(x + 3, y + 2, (uint8_t)(color_sum + 4));

        graphics::Dot_(x, y + 3, (uint8_t)(color_sum + 1));
        graphics::Dot_(x + 1, y + 3, (uint8_t)(color_sum + 4));
        graphics::Dot_(x + 2, y + 3, (uint8_t)(color_sum + 4));
        graphics::Dot_(x + 3, y + 3, (uint8_t)(color_sum + 1));
    }

    /**
     * Draws a 5x5 sprite blip on the screen using a calculated color base.
     * The color of each pixel is determined by the sum of two color parameters and a local offset.
     */
    void __cdecl Draw_Blip5_(int16_t x, int16_t y, int16_t color_base_a, int16_t color_base_b) {
        uint8_t base = static_cast<uint8_t>(color_base_a + color_base_b);

        // Row 0
        graphics::Dot_(x, y, base + 2);
        graphics::Dot_(x + 1, y, base + 4);
        graphics::Dot_(x + 2, y, base + 4);
        graphics::Dot_(x + 3, y, base + 4);
        graphics::Dot_(x + 4, y, base + 1);

        // Row 1
        graphics::Dot_(x, y + 1, base + 4);
        graphics::Dot_(x + 1, y + 1, base + 9);
        graphics::Dot_(x + 2, y + 1, base + 9);
        graphics::Dot_(x + 3, y + 1, base + 6);
        graphics::Dot_(x + 4, y + 1, base + 4);

        // Row 2
        graphics::Dot_(x, y + 2, base + 4);
        graphics::Dot_(x + 1, y + 2, base + 9);
        graphics::Dot_(x + 2, y + 2, base + 6);
        graphics::Dot_(x + 3, y + 2, base + 6);
        graphics::Dot_(x + 4, y + 2, base + 4);

        // Row 3
        graphics::Dot_(x, y + 3, base + 4);
        graphics::Dot_(x + 1, y + 3, base + 6);
        graphics::Dot_(x + 2, y + 3, base + 6);
        graphics::Dot_(x + 3, y + 3, base + 6);
        graphics::Dot_(x + 4, y + 3, base + 4);

        // Row 4
        graphics::Dot_(x, y + 4, base + 1);
        graphics::Dot_(x + 1, y + 4, base + 4);
        graphics::Dot_(x + 2, y + 4, base + 4);
        graphics::Dot_(x + 3, y + 4, base + 4);
        graphics::Dot_(x + 4, y + 4, base + 1);
    }

    void __cdecl Draw_Super_Fast_Screen_() {
        MOX::_super_fast_flag = 0;
        video::Set_Page_Off_();
        CMBTDRW1::Full_Draw_Combat_Screen_();

        if (MOX::_ship_status_display == 0) {
            CMBTDRW1::Draw_Weapon_Status_Display_(-1, -1);
        } else {
            CMBTDRW1::Draw_Special_Status_Display_();
        }

        graphics::Interlaced_Fill_(0x191, 0x16a, 0x1e9, 0x176, 0x0, 199 /* 0xc7 */);
        graphics::Interlaced_Fill_(0x191, 0x17b, 0x1e8, 0x1d3, 0x0, 199 /* 0xc7 */);

        CMBTDRW1::Draw_Internals_Display_(0x191, 0x17b);
        MOX::_last_ship_status = 666 /* 0x29a */;

        CMBTDRW1::Draw_Main_Combat_Screen_();
        fields::Draw_Visible_Fields_();
        video::Copy_Off_To_Back_();
        RUSS::Mox_Sync_Update_();

        MOX::_super_fast_flag = 1;
    }

    void __cdecl Move_Options_Background_() {
        int16_t target_y;
        int16_t current_y;
        int16_t step;

        if (MOX::_order_info_flag == 0) {
            MOX::_order_info_flag = 1;
            current_y = 445;
            target_y = 298;
            step = -7;
            video::Set_Page_Back_();
            Full_Draw_Combat_Screen_();
            Draw_Main_Combat_Screen_();
            Draw_Fake_Combat_Buttons_();
            video::Copy_Back_To_Off_();
            RUSS::Mox_Sync_Update_();
            MOX::_order_info_flag = 0;
        } else {
            current_y = 298;
            target_y = 445;
            step = 7;
            video::Set_Page_Back_();
            Full_Draw_Combat_Screen_();
            Draw_Main_Combat_Screen_();
            Draw_Fake_Combat_Buttons_();
            video::Copy_Back_To_Off_();
            video::Set_Page_Off_();
            MOX::_combat_options_background_seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx, 9, MOX::_global_cache_seg);
            animate::Draw_(248, 298, MOX::_combat_options_background_seg);
            RUSS::Mox_Sync_Update_();
        }

        fields::Disable_Draw_All_Fields_();

        if (current_y != target_y) {
            do {
                current_y += step;

                if (step < 0) {
                    if (current_y < target_y) {
                        current_y = target_y;
                    }
                } else if (current_y > target_y) {
                    current_y = target_y;
                }

                video::Set_Page_Off_();
                animate::Set_Animation_Frame_((s_animation_header*)MOX::_weapons_button_seg, 1 - MOX::_ship_status_display);
                animate::Draw_(114, 361, (s_animation_header*)MOX::_weapons_button_seg);
                animate::Set_Animation_Frame_((s_animation_header*)MOX::_specials_button_seg, MOX::_ship_status_display);
                animate::Draw_(187, 361, (s_animation_header*)MOX::_specials_button_seg);

                graphics::Set_Window_(0, 0, 639, 479);
                video::Clip_On_();

                MOX::_combat_options_background_seg = (s_animation_header*)Buffer_Reload_(s_combat_lbx, 9, MOX::_global_cache_seg);
                animate::Draw_(248, current_y, MOX::_combat_options_background_seg);

                animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_option_seg, (int32_t)MOX::_msl_impact_warning_flag);
                animate::Draw_(267, current_y + 88, (s_animation_header*)MOX::_cmbt_option_seg);

                animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_option_seg, (int32_t)MOX::_speedx2_flag);
                animate::Draw_(267, current_y + 104, (s_animation_header*)MOX::_cmbt_option_seg);

                animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_option_seg, (int32_t)MOX::_draw_legal_boxes_flag);
                animate::Draw_(267, current_y + 120, (s_animation_header*)MOX::_cmbt_option_seg);

                animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_option_seg, (int32_t)MOX::_shields_flag);
                animate::Draw_(267, current_y + 136, (s_animation_header*)MOX::_cmbt_option_seg);

                animate::Set_Animation_Frame_((s_animation_header*)MOX::_cmbt_option_seg, (int32_t)MOX::_grid_flag);
                animate::Draw_(267, current_y + 152, (s_animation_header*)MOX::_cmbt_option_seg);

                video::Clip_Off_();
                graphics::Reset_Window_();
                RUSS::Mox_Sync_Update_();

            } while (current_y != target_y);
        }

        COMBAT1::Add_Tactical_Combat_Fields_();
        COMBAT1::Assign_Combat_Grids_(MOX::_cur_ship);
        fields::Enable_Draw_All_Fields_();
        Refresh_Combat_Screen_Full_();
    }

    void __cdecl Display_Combat_View_Ship_(int16_t ship_idx) {
        struct_::Copy_Structure_(&MOX::_global_combat_data, &MOX::_combat_data[ship_idx], 313 /* 0x139 */);
        MOX::_ship_data_id = ship_idx;

        if (ship_idx == 0) {
            MOX::_global_combat_data.drive_effectiveness_pct = -666;
        }

        video::Set_Page_Off_();
        MOX::_last_ship_status = 1;
        CMBTDRW1::Full_Draw_Combat_Screen_();

        if (MOX::_ship_status_display == 0) {
            CMBTDRW1::Draw_Weapon_Status_Display_(-1, -1);
        } else {
            CMBTDRW1::Draw_Special_Status_Display_();
        }

        CMBTDRW1::Draw_Main_Combat_Screen_();
        video::Copy_Off_To_Back_();
        RUSS::Mox_Sync_Update_();
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

        memset(MOX::_combat_grid, 0xff, sizeof(MOX::_combat_grid));

        CMBTDRW1::Detailed_View_Ship_(-1);
        COMBAT1::Add_Tactical_Combat_Fields_();
        COMBAT1::Assign_Combat_Grids_(MOX::_cur_ship);
        CMBTDRW1::Refresh_Combat_Screen_Full_();
        fields::Assign_Auto_Function_(CMBTDRW1::Draw_Main_Combat_Screen_, 1);
    }

    int16_t __cdecl Board_Popup_(int16_t min_val, int16_t max_val, char* msg, int16_t* out_val) {
        int16_t result = 1;
        int16_t done = 0;

        _g_min = min_val;
        _g_max = max_val;
        _g_msg = msg;
        _g_var = max_val;

        allocate::Push_Block_(MOX::_screen_seg);

        _board_left_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_combat_lbx_005599c8, 6, MOX::_screen_seg);
        _board_right_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_combat_lbx_005599c8, 40, MOX::_screen_seg);
        _board_raid_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_combat_lbx_005599c8, 39, MOX::_screen_seg);
        _board_capture_button_seg = (s_animation_header*)farload::Far_Reload_Next_(s_combat_lbx_005599c8, 41, MOX::_screen_seg);

        video::Set_Page_Back_();
        Full_Draw_Combat_Screen_();
        Draw_Main_Combat_Screen_();
        fields::Draw_Visible_Fields_Back_();
        video::Copy_Back_To_Off_();
        RUSS::Mox_Sync_Update_();
        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(Draw_Board_Popup_, 1);
        fields::Clear_Fields_();

        fields::Add_Scroll_Field_(0xbb, 0xf9, min_val, max_val + 1, min_val, max_val, 0xcf, 0xd, (short*)&_g_var, s__00551100, 0);

        int16_t btn_left = fields::Add_Button_Field_(0xa4, 0xfd, s__00551100, _board_left_button_seg, s__00551100, 0);
        int16_t btn_right = fields::Add_Button_Field_(0x196, 0xfd, s__00551100, _board_right_button_seg, s__00551100, 0);
        int16_t btn_raid = fields::Add_Button_Field_(0xa3, 0x111, s__00551100, _board_raid_button_seg, s__00551100, 0);
        int16_t btn_capture = fields::Add_Button_Field_(0x159, 0x111, s__00551100, _board_capture_button_seg, s__00551100, 0);
        int16_t btn_cancel = fields::Add_Hot_Key_(s__00551120);

        video::Set_Page_Off_();
        Draw_Board_Popup_();
        RUSS::Mox_Sync_Update_();
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

        while (!done) {
            int16_t input = fields::Get_Input_();
            fields::Scan_Input_();

            if (input == btn_capture) {
                done = 1;
                result = 2;
            }

            if (input == btn_raid) {
                done = 1;
                result = 1;
            }

            if (input == btn_right && _g_var < max_val) {
                _g_var++;
            }

            if (input == btn_left && _g_var > 0) {
                _g_var--;
            }

            if (input == btn_cancel) {
                result = 0;
                done = 1;
            }

            if (!done) {
                video::Set_Page_Off_();
                Draw_Board_Popup_();
                RUSS::Mox_Sync_Update_();
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
        fields::Clear_Fields_();
        fields::Assign_Auto_Function_(Draw_Main_Combat_Screen_, 1);
        Refresh_Combat_Screen_Full_();
        COMBAT1::Assign_Combat_Grids_(MOX::_cur_ship);

        *out_val = _g_var;

        return result;
    }

    int16_t __cdecl Ship_Completely_On_Screen_(int16_t ship_idx) {
        int idx = (int)ship_idx;
        int16_t x = (int16_t)MOX::_combat_data[idx].x;
        int16_t y = (int16_t)MOX::_combat_data[idx].y;

        int16_t result = (int16_t)(CMBTMIS::On_Combat_Screen_(x, y) & 0x00FF);

        if (result == 0) {
            return 0;
        }

        if (ship_idx == 0) {
            int16_t planet_size;
            if (MOX::_combat_defender_player == 8) {
                planet_size = 2;
            } else {
                int colony_idx = (int)MOX::_cmbt_colony_id;
                int planet_idx = (int)MOX::_colony[colony_idx].planet;
                planet_size = (int16_t)MOX::_planet[planet_idx].size;
            }

            int16_t dx = 0, dy = 0;
            bool do_planet_check = true;

            switch (planet_size) {
                case 0: dx = 2; dy = 2; break;
                case 1: dx = 3; dy = 3; break;
                case 2: dx = 5; dy = 5; break;
                case 3: dx = 6; dy = 6; break;
                case 4: dx = 7; dy = 7; break;
                default:
                    do_planet_check = false;
                    break;
            }

            if (do_planet_check) {
                result &= (int16_t)(CMBTMIS::On_Combat_Screen_(x + dx, y + dy) & 0x00FF);
            }
        }

        int8_t size_class = MOX::_combat_data[idx].size_class;
        switch (size_class) {
            case 1:
            case 2:
                result &= (int16_t)(CMBTMIS::On_Combat_Screen_(x + 1, y + 1) & 0x00FF);
                break;
            case 3:
            case 4:
            case 5:
                result = (int16_t)(CMBTMIS::On_Combat_Screen_(x + 2, y + 2) & 0x00FF);
                break;
            default:
                break;
        }

        return result;
    }

    int16_t __cdecl Ship_On_Screen_(int16_t ship_idx) {
        s_combat_data* ship = &MOX::_combat_data[ship_idx];
        int16_t x = ship->x;
        int16_t y = ship->y;

        int16_t combined_result = (int16_t)CMBTMIS::On_Combat_Screen_(x, y);

        if (ship_idx == 0) {
            int16_t planet_size = 2;

            if (MOX::_combat_defender_player != 8) {
                int16_t colony_id = MOX::_cmbt_colony_id;
                int16_t planet_idx = MOX::_colony[colony_id].planet;
                planet_size = MOX::_planet[planet_idx].size;
            }

            switch (planet_size) {
                case 0:
                    combined_result |= (int16_t)CMBTMIS::On_Combat_Screen_(x + 2, y + 2);
                    break;
                case 1:
                    combined_result |= (int16_t)CMBTMIS::On_Combat_Screen_(x + 3, y + 3);
                    break;
                case 2:
                    combined_result |= (int16_t)CMBTMIS::On_Combat_Screen_(x + 5, y + 5);
                    break;
                case 3:
                    combined_result |= (int16_t)CMBTMIS::On_Combat_Screen_(x + 6, y + 6);
                    break;
                case 4:
                    combined_result |= (int16_t)CMBTMIS::On_Combat_Screen_(x + 7, y + 7);
                    break;
                default:
                    break;
            }
        } else {
            int8_t size_class = ship->size_class;

            switch (size_class) {
                case 1:
                case 2:
                    combined_result |= (int16_t)CMBTMIS::On_Combat_Screen_(x + 1, y + 1);
                    break;
                case 3:
                case 4:
                case 5:
                    combined_result |= (int16_t)CMBTMIS::On_Combat_Screen_(x + 2, y + 2);
                    break;
                default:
                    break;
            }
        }

        return combined_result;
    }

    void __cdecl Scroll_Draw_Combat_Screen_() {
        graphics::Set_Window_(0, 0, 639, 350);
        video::Clip_On_();

        if (MOX::_in_nebula_flag == 0) {
            graphics::Fill_(0, 0, 639, 350, 0);
        }

        int index = (MOX::_in_nebula_flag == 1) ? 35 /* 0x23 */ : 46 /* 0x2e */;
        MOX::_combat_far_star = (s_animation_header*)Buffer_Reload_(MOX::s_combat_lbx, index, MOX::_global_cache_seg);

        animate::Draw_(MOX::_combat_far_x, MOX::_combat_far_y, MOX::_combat_far_star);
        animate::Draw_(MOX::_combat_far_x - 640, MOX::_combat_far_y, MOX::_combat_far_star);
        animate::Draw_(MOX::_combat_far_x - 640, MOX::_combat_far_y - 480, MOX::_combat_far_star);
        animate::Draw_(MOX::_combat_far_x, MOX::_combat_far_y - 480, MOX::_combat_far_star);

        MOX::_combat_mid_star = (s_animation_header*)Buffer_Reload_(MOX::s_combat_lbx, 47 /* 0x2f */, MOX::_global_cache_seg);
        animate::Draw_(MOX::_combat_mid_x, MOX::_combat_mid_y, MOX::_combat_mid_star);
        animate::Draw_(MOX::_combat_mid_x - 640, MOX::_combat_mid_y, MOX::_combat_mid_star);
        animate::Draw_(MOX::_combat_mid_x - 640, MOX::_combat_mid_y - 480, MOX::_combat_mid_star);
        animate::Draw_(MOX::_combat_mid_x, MOX::_combat_mid_y - 480, MOX::_combat_mid_star);

        MOX::_combat_near_star = (s_animation_header*)Buffer_Reload_(MOX::s_combat_lbx, 48 /* 0x30 */, MOX::_global_cache_seg);
        animate::Draw_(MOX::_combat_near_x, MOX::_combat_near_y, MOX::_combat_near_star);
        animate::Draw_(MOX::_combat_near_x - 640, MOX::_combat_near_y, MOX::_combat_near_star);
        animate::Draw_(MOX::_combat_near_x - 640, MOX::_combat_near_y - 480, MOX::_combat_near_star);
        animate::Draw_(MOX::_combat_near_x, MOX::_combat_near_y - 480, MOX::_combat_near_star);

        video::Clip_Off_();
        graphics::Reset_Window_();

        if (MOX::_grid_flag == 1) {
            for (int16_t x = 0; x < 640; x += 20) {
                line::Line_(x, 0, x, 350, 4);
            }
            for (int16_t y = 0; y < 360; y += 20) {
                line::Line_(0, y, 640, y, 4);
            }
        }

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            if (MOX::_combat_data[i].unit_status == 0) {
                if (MOX::_dont_draw_ship != i) {
                    uint8_t cooldown = MOX::_combat_data[i].special_cooldown;
                    if (cooldown != 0xFF && (int16_t)cooldown != MOX::_dont_draw_ship) {
                        CMBTSPEC::Draw_Continuing_Tractor_(i);
                    }
                }
            }
        }

        graphics::Set_Window_(0, 0, 639, 350);
        video::Clip_On_();

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            if (MOX::_combat_data[i].unit_status == 0) {
                int16_t sx = (MOX::_combat_data[i].x - MOX::_cur_combat_x) * 20;
                int16_t sy = (MOX::_combat_data[i].y - MOX::_cur_combat_y) * 20;
                Draw_Ship_(i, sx, sy);
            }
        }

        CMBTMIS::Draw_Missiles_();
        video::Clip_Off_();
        graphics::Reset_Window_();

        if (MOX::_missile_warning == 1) {
            int16_t frames = animate::Get_Current_Frame_(MOX::_msl_warning_on_seg);
            animate::Reset_Animation_Frame_(MOX::_msl_warning_on_seg);
            while (frames > 0) {
                animate::Draw_(500, 363, MOX::_msl_warning_on_seg);
                frames--;
            }
            animate::Draw_(500, 363, MOX::_msl_warning_on_seg);
            animate::Draw_(529, 407, MOX::_warning_missile_seg);
        } else {
            Draw_Reduced_Combat_Map_();
            Draw_Reduced_Map_Box_();
        }

        if (MOX::_last_ship_status != 666 /* 0x29a */) {
            if (MOX::_ship_status_display == 0) {
                Draw_Weapon_Status_Display_(-1, -1);
            } else {
                Draw_Special_Status_Display_();
            }
            graphics::Interlaced_Fill_(401, 362, 489, 374, 0, 199);
            graphics::Interlaced_Fill_(401, 379, 488, 467, 0, 199);
            Draw_Internals_Display_(401, 379);
        }

        if (MOX::_order_info_flag == 0) {
            Draw_Fake_Combat_Buttons_();
            MOX::_combat_options_background_seg = (s_animation_header*)Buffer_Reload_(MOX::s_combat_lbx, 9, MOX::_global_cache_seg);
            animate::Draw_(248, 298, MOX::_combat_options_background_seg);
            Draw_Option_Panel_Lights_();
        }
    }

    int8_t __cdecl Get_Weapon_Arc_(int16_t bearing) {
        int8_t arc_flags = 0;

        // Front arc bit (approx. +/- 60 degrees)
        if (bearing <= 60 || bearing >= 300) {
            arc_flags = 1;
        }

        // Side-extended front arc bit (approx. +/- 120 degrees)
        if (bearing <= 120 || bearing >= 240) {
            arc_flags += 2;
        }

        // Rear-extended back arc bit (approx. 60 to 300 degrees)
        if (bearing >= 60 && bearing <= 300) {
            arc_flags += 4;
        }

        // Back arc bit (approx. 120 to 240 degrees)
        if (bearing >= 120 && bearing <= 240) {
            arc_flags += 8;
        }

        return arc_flags;
    }

    void __cdecl Get_Draw_Ship_Offsets_(int16_t ship_idx, int16_t* out_x, int16_t* out_y) {
        *out_x = 0;
        *out_y = 0;

        if (ship_idx == 0) {
            *out_x = 0;
            *out_y = 0;

            if (MOX::_cmbt_colony_id == -1) {
                *out_x = 9;
                *out_y = 7;
                return;
            }

            if (MOX::_planet[MOX::_colony[MOX::_cmbt_colony_id].planet].size != 2) {
                return;
            }

            *out_x = 9;
            *out_y = 7;
            return;
        }

        int8_t facing_dir = MOX::_combat_data[ship_idx].facing_dir;

        switch ((MOX::_combat_data[ship_idx].size_class + 1) / 2) {
        case 0:
            *out_x = -16;
            *out_y = -16;
            break;
        case 1:
            *out_x = -6;
            *out_y = -6;
            break;
        case 2:
        case 3:
            *out_x = 4;
            *out_y = 4;
            break;
        default:
            break;
        }

        int iVar2 = (facing_dir - 1) / 4;
        if (iVar2 <= 0) {
            iVar2 = 0;
        }

        if (iVar2 == 1) {
            *out_x -= 2;
        } else if (iVar2 == 2) {
            *out_x -= 2;
            *out_y -= 1;
        } else if (iVar2 == 3) {
            *out_y -= 1;
        }

        *out_x -= 2;
        *out_y -= 3;
    }

    void __cdecl Draw_Ship_To_Bitmap_(int16_t ship_idx, int16_t layer, int16_t x, int16_t y) {
        int16_t facing_dir = MOX::_combat_data[ship_idx].facing_dir;
        s_animation_header* anim = MOX::_combat_data[ship_idx].ship_image_seg;
        int8_t previous_owner = MOX::_combat_data[ship_idx].previous_owner;

        if (anim == nullptr || MOX::_combat_data[ship_idx].owner != (uint8_t)previous_owner) {
            uint8_t local_40[32];
            uint8_t local_20[32];

            for (int16_t i = 0; i < 32; ++i) {
                local_40[i] = i + 0x20;
                local_20[i] = i + 0x60;
            }

            char lbx_name[12];
            int32_t picture_num;

            if ((uint8_t)previous_owner < 8 && MOX::_combat_data[ship_idx].picture_num != 0x2C) {
                _memcpy(lbx_name, s_cmbtshp_lbx_0055125c, 12);
                uint8_t color = MOX::_player[(uint8_t)previous_owner].color;
                picture_num = color * 45 + MOX::_combat_data[ship_idx].picture_num;
            } else {
                _memcpy(lbx_name, s_monster_lbx_00551250, 12);
                if (MOX::_combat_data[ship_idx].picture_num == 0x2C) {
                    picture_num = 25;
                } else {
                    picture_num = MOX::_combat_data[ship_idx].picture_num;
                }
            }

            anim = (s_animation_header*)Buffer_Reload_(lbx_name, picture_num, MOX::_global_cache_seg);

            if (MOX::_combat_data[ship_idx].owner == MOX::_combat_attacker_player) {
                replace::Replace_Color_Range_(anim, local_40, 0x60, 0x20);
            } else {
                replace::Replace_Color_Range_(anim, local_20, 0x20, 0x20);
            }
        }

        int32_t dir_group = (facing_dir - 1) / 4;
        if (dir_group <= 0) {
            dir_group = 0;
        }

        switch (dir_group) {
            case 0:
                animate::Set_Animation_Frame_(anim, facing_dir * 4);
                bitmap::Draw_Picture_To_Bitmap_(MOX::_draw_ship_bitmap, anim);
                break;
            case 1:
                animate::Set_Animation_Frame_(anim, (8 - facing_dir) * 4);
                bitmap::Draw_Picture_To_Bitmap_(MOX::_ship_bitmap, anim);
                bitmap::Flip_Bitmap_(MOX::_draw_ship_bitmap, MOX::_ship_bitmap, 0);
                break;
            case 2:
                animate::Set_Animation_Frame_(anim, (facing_dir - 8) * 4);
                bitmap::Draw_Picture_To_Bitmap_(MOX::_draw_ship_bitmap, anim);
                bitmap::Flip_Bitmap_(MOX::_ship_bitmap, MOX::_draw_ship_bitmap, 0);
                bitmap::Flip_Bitmap_(MOX::_draw_ship_bitmap, MOX::_ship_bitmap, 3);
                break;
            case 3:
                animate::Set_Animation_Frame_(anim, (16 - facing_dir) * 4);
                bitmap::Draw_Picture_To_Bitmap_(MOX::_ship_bitmap, anim);
                bitmap::Flip_Bitmap_(MOX::_draw_ship_bitmap, MOX::_ship_bitmap, 3);
                break;
            default:
                break;
        }

        if (x != 0) {
            Plasma_Darken_(MOX::_draw_ship_bitmap, y, x);
        }
    }

    /**
     * Calculates the screen drawing coordinates for the center of a ship in combat.
     * It combines the world-to-screen coordinate transformation (multiplied by 20)
     * with local ship-specific centering offsets.
     */
    void __cdecl Draw_Ship_Status_(int16_t param_1, int16_t param_2, int16_t param_3) {
        if (param_1 < 0x14) {
            graphics::Fill_(0xf, 0x17b, 0x66, 0x1c5, 0);
            s_animation_header* anim = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x2d, reinterpret_cast<void*>(MOX::_global_cache_seg));
            animate::Draw_(0xf, 0x17b, anim);
        } else if (param_1 > 0x17c) {
            graphics::Fill_(0x191, 0x17b, 0x1e8, 0x1d3, 0);
            s_animation_header* anim = (s_animation_header*)Buffer_Reload_(s_combat_lbx_00551244, 0x2d, reinterpret_cast<void*>(MOX::_global_cache_seg));
            animate::Draw_(0x191, 0x17b, anim);
        }

        if (param_3 == 0) {
            animate::Draw_(param_1 + 0x10, param_2 + 0xf, MOX::_combat_planet_seg2);
            s_combat_data* planet_combat = &MOX::_combat_data[0];

            if (planet_combat->owner != 8) {
                char local_50[80];
                char local_a0[80];
                char local_b4[12];

                fonts::Set_Font_Style_(0, &MOX::_combat_green);
                KEN::Ken_Get_Text_Message_(0, local_50, 0x50);
                strcpy(local_a0, local_50);

                if (MOX::_cmbt_colony_id == -1) {
                    __itoa(0x23, local_b4, 10);
                } else {
                    __itoa(MOX::_colony[MOX::_cmbt_colony_id].n_pops, local_b4, 10);
                }
                
                strcat(local_a0, local_b4);
                strcat(local_a0, s__00559998);

                if (MOX::_cmbt_colony_id != -1) {
                    if (MOX::_colony[MOX::_cmbt_colony_id].military[1] > 0) {
                        KEN::Ken_Get_Text_Message_(1, local_50, 0x50);
                        strcat(local_a0, local_50);
                        __itoa(MOX::_colony[MOX::_cmbt_colony_id].military[1], local_b4, 10);
                        strcat(local_a0, local_b4);
                        strcat(local_a0, s__00559998);
                    }
                    if (MOX::_colony[MOX::_cmbt_colony_id].military[0] > 0) {
                        KEN::Ken_Get_Text_Message_(2, local_50, 0x50);
                        strcat(local_a0, local_50);
                        __itoa(MOX::_colony[MOX::_cmbt_colony_id].military[0], local_b4, 10);
                        strcat(local_a0, local_b4);
                        strcat(local_a0, s__00559998);
                    }
                }

                fonts::Print_(param_1 + 2, param_2 + 0x48, local_a0);
                local_a0[0] = '\0';

                if (MOX::_cmbt_colony_id != -1) {
                    int16_t y = param_2 + 4;
                    if (MOX::_colony[MOX::_cmbt_colony_id].buildings[26] != 0) {
                        fonts::Print_(param_1 + 2, y, TECHDATA::_buildings[26].name);
                        y += 7;
                    }
                    if (MOX::_colony[MOX::_cmbt_colony_id].buildings[27] != 0) {
                        fonts::Print_(param_1 + 2, y, TECHDATA::_buildings[27].name);
                        y += 7;
                    }
                    if (MOX::_colony[MOX::_cmbt_colony_id].buildings[47] != 0) {
                        fonts::Print_(param_1 + 2, y, TECHDATA::_buildings[47].name);
                        y += 7;
                    }
                    if (MOX::_colony[MOX::_cmbt_colony_id].buildings[42] != 0) {
                        fonts::Print_(param_1 + 2, y, TECHDATA::_buildings[42].name);
                    }
                }
            }
        } else {
            Draw_Ship_To_Bitmap_(param_3, 0, 0, 0);
            bitmap::Create_Blank_Bitmap_(0x62, 0x62, MOX::_combat_bitmap);
            
            s_combat_data* ship = &MOX::_combat_data[param_3];
            int16_t facing = ship->facing_dir;
            int dir = (facing - 1) / 4;
            if (dir < 0) {
                dir = 0;
            }

            int16_t x_offset = (dir == 1 || dir == 2) ? 14 : 16;
            bitmap::Add_Picture_To_Bitmap_(x_offset, 11, MOX::_draw_ship_bitmap, MOX::_combat_bitmap);

            if (ship->shield_type > 0 && ship->shield_current_hits > 0) {
                bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 1, 0);
                bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 2, 0);
                
                int shield_val = (ship->shield_arc_max + 19) / 10;
                if (shield_val > 10) {
                    shield_val = 10;
                }
                
                if (shield_val - 1 <= 9) {
                    switch (shield_val - 1) {
                        case 0:
                        case 1:
                        case 2:
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 1, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 2, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x51, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x52, 0);
                            for (int i = 0; i < 4; i++) {
                                if (ship->shield_arc_current[i] < 11) {
                                    bitmap::Copy_Bitmap_To_Bitmap_(MOX::_draw_ship_bitmap, MOX::_combat_bitmap);
                                    int mask_idx = ((i - (facing / 4) + 4) % 4) + (facing % 4) * 4;
                                    bitmap::Mask_Out_Bitmap_With_Bitmap_(0, 0, MOX::_draw_ship_bitmap, MOX::_shield_mask_bitmap[mask_idx], 1);
                                    replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x52, 1);
                                    if (ship->shield_arc_current[i] <= 0) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x51, 1);
                                    }
                                    bitmap::Add_Picture_To_Bitmap_(0, 0, MOX::_draw_ship_bitmap, MOX::_combat_bitmap);
                                }
                            }
                            break;
                        case 3:
                        case 4:
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 1, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 2, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x50, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x52, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x54, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x56, 0);
                            for (int i = 0; i < 4; i++) {
                                if (ship->shield_arc_current[i] < 31) {
                                    bitmap::Copy_Bitmap_To_Bitmap_(MOX::_draw_ship_bitmap, MOX::_combat_bitmap);
                                    int mask_idx = ((i - (facing / 4) + 4) % 4) + (facing % 4) * 4;
                                    bitmap::Mask_Out_Bitmap_With_Bitmap_(0, 0, MOX::_draw_ship_bitmap, MOX::_shield_mask_bitmap[mask_idx], 1);
                                    replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x56, 1);
                                    if (ship->shield_arc_current[i] <= 21) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x54, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 11) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x52, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 0) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x50, 1);
                                    }
                                    bitmap::Add_Picture_To_Bitmap_(0, 0, MOX::_draw_ship_bitmap, MOX::_combat_bitmap);
                                }
                            }
                            break;
                        case 5:
                        case 6:
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 1, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 2, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x50, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x51, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x53, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x54, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x55, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x56, 0);
                            for (int i = 0; i < 4; i++) {
                                if (ship->shield_arc_current[i] < 51) {
                                    bitmap::Copy_Bitmap_To_Bitmap_(MOX::_draw_ship_bitmap, MOX::_combat_bitmap);
                                    int mask_idx = ((i - (facing / 4) + 4) % 4) + (facing % 4) * 4;
                                    bitmap::Mask_Out_Bitmap_With_Bitmap_(0, 0, MOX::_draw_ship_bitmap, MOX::_shield_mask_bitmap[mask_idx], 1);
                                    replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x56, 1);
                                    if (ship->shield_arc_current[i] <= 41) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x55, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 31) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x54, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 21) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x53, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 11) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x51, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 0) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x50, 1);
                                    }
                                    bitmap::Add_Picture_To_Bitmap_(0, 0, MOX::_draw_ship_bitmap, MOX::_combat_bitmap);
                                }
                            }
                            break;
                        case 7:
                        case 8:
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x50, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x51, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x52, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x53, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x54, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x55, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x56, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x57, 0);
                            for (int i = 0; i < 4; i++) {
                                if (ship->shield_arc_current[i] < 71) {
                                    bitmap::Copy_Bitmap_To_Bitmap_(MOX::_draw_ship_bitmap, MOX::_combat_bitmap);
                                    int mask_idx = ((i - (facing / 4) + 4) % 4) + (facing % 4) * 4;
                                    bitmap::Mask_Out_Bitmap_With_Bitmap_(0, 0, MOX::_draw_ship_bitmap, MOX::_shield_mask_bitmap[mask_idx], 1);
                                    replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x57, 1);
                                    if (ship->shield_arc_current[i] <= 61) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x56, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 51) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x55, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 41) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x54, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 31) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x53, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 21) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x52, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 11) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x51, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 0) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x50, 1);
                                    }
                                    bitmap::Add_Picture_To_Bitmap_(0, 0, MOX::_draw_ship_bitmap, MOX::_combat_bitmap);
                                }
                            }
                            break;
                        case 9:
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x50, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x51, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x52, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x53, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x53, 0); // Duplicate as in assembly
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x54, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x54, 0); // Duplicate as in assembly
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x55, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x56, 0);
                            bitmap::Outline_Bitmap_(MOX::_combat_bitmap, MOX::_combat_bitmap, 0x57, 0);
                            for (int i = 0; i < 4; i++) {
                                if (ship->shield_arc_current[i] < 91) {
                                    bitmap::Copy_Bitmap_To_Bitmap_(MOX::_draw_ship_bitmap, MOX::_combat_bitmap);
                                    int mask_idx = ((i - (facing / 4) + 4) % 4) + (facing % 4) * 4;
                                    bitmap::Mask_Out_Bitmap_With_Bitmap_(0, 0, MOX::_draw_ship_bitmap, MOX::_shield_mask_bitmap[mask_idx], 1);
                                    replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x57, 1);
                                    if (ship->shield_arc_current[i] <= 81) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x56, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 71) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x55, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 61) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x54, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 41) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x53, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 21) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x52, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 11) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x51, 1);
                                    }
                                    if (ship->shield_arc_current[i] <= 0) {
                                        replace::Replace_Color_(MOX::_draw_ship_bitmap, 0x50, 1);
                                    }
                                    bitmap::Add_Picture_To_Bitmap_(0, 0, MOX::_draw_ship_bitmap, MOX::_combat_bitmap);
                                }
                            }
                            break;
                    }
                }

                bitmap::Mask_Out_Bitmap_With_Bitmap_(0, 0, MOX::_combat_bitmap, (s_animation_header*)MOX::_shield_div_bitmap[facing % 4], 0);
                Draw_Ship_To_Bitmap_(param_3, 1, 0, 0);

                int dir2 = (facing - 1) / 4;
                if (dir2 < 0) {
                    dir2 = 0;
                }

                int16_t dx2 = (dir2 == 1 || dir2 == 2) ? 14 : 16;
                bitmap::Add_Picture_To_Bitmap_(dx2, 11, MOX::_draw_ship_bitmap, MOX::_combat_bitmap);
            }

            replace::Replace_Color_(MOX::_combat_bitmap, 1, 0);
            replace::Replace_Color_(MOX::_combat_bitmap, 2, 0);
            animate::Draw_(param_1, param_2, MOX::_combat_bitmap);

            if (ship->current_speed <= 0) {
                struct s_colors shadow_color;
                memcpy(shadow_color.colors, "HJJJJJJJ", 8);
                fonts::Set_Font_Style_Shadow_Down_(1, &shadow_color);
                
                char local_50[80];
                KEN::Ken_Get_Text_Message_(0x47, local_50, 0x50);
                fonts::Print_Centered_(param_1 + 45, param_2 + 70, local_50);
            }
        }
    }

    void __cdecl Get_Ship_Center_Draw_XY_(int16_t combat_idx, int16_t* x_out, int16_t* y_out) {
        int16_t offset_x;
        int16_t offset_y;

        // Retrieve ship-specific centering offsets based on ship image dimensions
        COMBINIT::Ship_Center_Offsets_(combat_idx, &offset_x, &offset_y);

        // Calculate screen X coordinate: (world_x - viewport_x) * grid_size + local_offset
        // 0x14 is the grid cell size (20 pixels)
        *x_out = (int16_t)(MOX::_combat_data[combat_idx].x - MOX::_cur_combat_x) * 20 + offset_x;

        // Calculate screen Y coordinate: (world_y - viewport_y) * grid_size + local_offset
        *y_out = (int16_t)(MOX::_combat_data[combat_idx].y - MOX::_cur_combat_y) * 20 + offset_y;
    }

    void __cdecl Draw_Retreating_Ship_(int16_t ship_idx) {
        int8_t owner = MOX::_combat_data[ship_idx].owner;
        int16_t facing = (MOX::_combat_attacker_player == owner) ? 8 : 0;
        int16_t x_offset = (MOX::_combat_attacker_player == owner) ? 28 : 31;

        int16_t move_step_fast = x_offset / 3;
        int16_t move_step_slow = x_offset / 4 + 1;
        int16_t local_e = 0;
        int16_t frame_step = 1;

        COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[ship_idx].x, MOX::_combat_data[ship_idx].y);

        int16_t ship_center_x;
        int16_t ship_center_y;
        Get_Ship_Center_Draw_XY_(ship_idx, &ship_center_x, &ship_center_y);

        int16_t base_x = MOX::_combat_data[ship_idx].x * 20;
        int16_t base_y = MOX::_combat_data[ship_idx].y * 20;

        if (MOX::_combat_data[ship_idx].facing_dir != facing) {
            CMBTMOV1::Rotate_Ship_(ship_idx, facing,
                                   MOX::_cur_combat_x * -20 + base_x,
                                   MOX::_cur_combat_y * -20 + base_y);
        }

        if (CMBTFIRE::Is_Combat_Ship_Dead_(ship_idx) != 0) {
            return;
        }

        s_animation_header* effect_seg = (s_animation_header*)Buffer_Reload_(s_cmbtsfx_lbx_00551228, 42, MOX::_global_cache_seg);
        int16_t effect_width = animate::Get_Width_(effect_seg);
        int16_t effect_height = animate::Get_Height_(effect_seg);
        int16_t frame_count = animate::Get_Animation_Frame_Count_(effect_seg);

        int16_t ship_actual_width;
        int16_t ship_actual_height;
        Get_Ship_Actual_Size_(ship_idx, &ship_actual_width, &ship_actual_height);

        MOX::_dont_draw_ship = ship_idx;

        Get_Ship_Center_Draw_XY_(ship_idx, &ship_center_x, &ship_center_y);

        video::Set_Page_Off_();

        int16_t draw_x;
        if (MOX::_combat_attacker_player == owner) {
            draw_x = ship_center_x - (ship_actual_width / 2) - x_offset - effect_width;
        } else {
            draw_x = ship_center_x + (ship_actual_width / 2) + x_offset;
        }

        int16_t draw_y = ship_center_y - (effect_height / 2);

        if (draw_x < 4) {
            draw_x = 4;
        }
        if (draw_x + effect_width > 0x27F) {
            draw_x = 0x276 - effect_width;
        }

        if (draw_y < 4) {
            draw_y = 4;
        }
        // BUG: Comparisons of draw_y should ideally account for height to prevent overflow.
        if (draw_y > 0x15E) {
            draw_y = 0x15A - effect_height;
        }

        int16_t window_status;
        MISC::Save_Window_Status_(&window_status);

        int16_t x1, y1, x2, y2;
        if (MOX::_combat_attacker_player == owner) {
            x1 = draw_x + 0x21;
            y1 = draw_y + 0x35;
            x2 = ship_center_x + ship_actual_width;
            y2 = ship_center_y + ship_actual_height;
        } else {
            x1 = ship_center_x - ship_actual_width;
            y1 = draw_y + 0x35;
            x2 = draw_x + 0x1E;
            y2 = draw_y + effect_height;
        }

        int16_t sound_id = (int16_t)ERICNET::Mox_Play_Sound_(52 /* 0x34 */);
        sound::Pitch_Bend_Sound_(sound_id, (2 - MOX::_combat_data[ship_idx].size_class) * 2500);

        if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
            frame_step = 3;
        }

        if (frame_count > 0) {
            for (int16_t frame = 0; frame < frame_count; frame += frame_step) {
                int16_t step = (frame < 5) ? move_step_fast : move_step_slow;
                step *= frame_step;

                timer::Mark_Time_();
                Draw_Main_Combat_Screen_();
                video::Clip_On_();
                graphics::Set_Window_(0, 0, 0x27F, 0x15E);

                effect_seg = (s_animation_header*)Buffer_Reload_(s_cmbtsfx_lbx_00551228, 42, MOX::_global_cache_seg);
                animate::Set_Animation_Frame_(effect_seg, frame);
                animate::Draw_(draw_x, draw_y, effect_seg);

                graphics::Set_Window_(x1, y1, x2, y2);
                Draw_Ship_(ship_idx, MOX::_cur_combat_x * -20 + local_e + base_x,
                                     MOX::_cur_combat_y * -20 + base_y);

                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(1);

                if (MOX::_combat_attacker_player == owner) {
                    local_e -= step;
                } else {
                    local_e += step;
                }
            }
        }

        MISC::Restore_Window_Status_(window_status);
    }

    void __cdecl Draw_Ship_(int16_t param_1, int16_t param_2, int16_t param_3) {
        int16_t frame_lookup[4] = {1, 2, 3, 2};
        const int16_t local_56 = 4;
        int16_t frame_idx;
        uint8_t previous_owner = MOX::_combat_data[param_1].previous_owner;

        if (previous_owner <= 9) {
            frame_idx = frame_lookup[((MOX::_ship_frame / 2) + param_1) % 4];
        } else {
            frame_idx = (MOX::_ship_frame / 2) % 4;
        }

        int16_t x_offset, y_offset;
        Get_Draw_Ship_Offsets_(param_1, &x_offset, &y_offset);

        if (param_1 == 0) {
            animate::Draw_((MOX::_combat_data[0].x - MOX::_cur_combat_x) * 20 + x_offset,
                           (MOX::_combat_data[0].y - MOX::_cur_combat_y) * 20 + y_offset,
                           MOX::_combat_planet_seg);
            COMBINIT::Ship_Center_Offsets_(0, &param_2, &param_3);
            return;
        }

        int8_t facing_dir = MOX::_combat_data[param_1].facing_dir;
        s_animation_header *anim = MOX::_combat_data[param_1].ship_image_seg;
        int8_t owner = MOX::_combat_data[param_1].owner;

        if (anim == nullptr || previous_owner != owner) {
            uint8_t local_40[32];
            uint8_t local_20[32];
            for (int i = 0; i < 32; ++i) {
                local_40[i] = i + 32;
                local_20[i] = i + 96;
            }

            char lbx_name[12];
            int16_t picture_num;
            if (previous_owner < 8 && MOX::_combat_data[param_1].picture_num != 44 /* 0x2c */) {
                strcpy(lbx_name, s_cmbtshp_lbx_0055125c);
                int16_t color = MOX::_player[previous_owner].color;
                picture_num = color * 45 + MOX::_combat_data[param_1].picture_num;
            } else {
                strcpy(lbx_name, s_monster_lbx_00551250);
                if (MOX::_combat_data[param_1].picture_num == 44 /* 0x2c */) {
                    picture_num = 25; /* 0x19 */
                } else {
                    picture_num = MOX::_combat_data[param_1].picture_num;
                }
            }
            
            anim = (s_animation_header*)Buffer_Reload_(lbx_name, picture_num, MOX::_global_cache_seg);
            
            if (owner == MOX::_combat_attacker_player) {
                replace::Replace_Color_Range_(anim, local_40, 96 /* 0x60 */, 32 /* 0x20 */);
            } else {
                replace::Replace_Color_Range_(anim, local_20, 32 /* 0x20 */, 32 /* 0x20 */);
            }
        }

        int quadrant = (facing_dir - 1) / 4;
        if (quadrant <= 0) {
            quadrant = 0;
        }

        switch (quadrant) {
            case 0: {
                int16_t done = CMBTFIR2::Combat_Ship_Done_(param_1);
                int16_t draw_frame;
                if (done == 0 && MOX::_combat_data[param_1].is_captured == 0) {
                    draw_frame = local_56 * facing_dir + frame_idx;
                } else {
                    draw_frame = local_56 * facing_dir;
                }
                animate::Set_Animation_Frame_(anim, draw_frame);
                bitmap::Draw_Picture_To_Bitmap_(MOX::_draw_ship_bitmap, anim);
                break;
            }
            case 1: {
                int16_t done = CMBTFIR2::Combat_Ship_Done_(param_1);
                int16_t draw_frame;
                if (done == 0 && MOX::_combat_data[param_1].is_captured == 0) {
                    draw_frame = local_56 * (8 - facing_dir) + frame_idx;
                } else {
                    draw_frame = local_56 * (8 - facing_dir);
                }
                animate::Set_Animation_Frame_(anim, draw_frame);
                bitmap::Draw_Picture_To_Bitmap_(MOX::_ship_bitmap, anim);
                bitmap::Flip_Bitmap_(MOX::_draw_ship_bitmap, MOX::_ship_bitmap, 0);
                break;
            }
            case 2: {
                int16_t done = CMBTFIR2::Combat_Ship_Done_(param_1);
                int16_t draw_frame;
                if (done == 0 && MOX::_combat_data[param_1].is_captured == 0) {
                    draw_frame = local_56 * (facing_dir - 8) + frame_idx;
                } else {
                    draw_frame = local_56 * (facing_dir - 8);
                }
                animate::Set_Animation_Frame_(anim, draw_frame);
                bitmap::Draw_Picture_To_Bitmap_(MOX::_draw_ship_bitmap, anim);
                bitmap::Flip_Bitmap_(MOX::_ship_bitmap, MOX::_draw_ship_bitmap, 0);
                bitmap::Flip_Bitmap_(MOX::_draw_ship_bitmap, MOX::_ship_bitmap, 3);
                break;
            }
            case 3: {
                int16_t done = CMBTFIR2::Combat_Ship_Done_(param_1);
                int16_t draw_frame;
                if (done == 0 && MOX::_combat_data[param_1].is_captured == 0) {
                    draw_frame = local_56 * (16 - facing_dir) + frame_idx;
                } else {
                    draw_frame = local_56 * (16 - facing_dir);
                }
                animate::Set_Animation_Frame_(anim, draw_frame);
                bitmap::Draw_Picture_To_Bitmap_(MOX::_ship_bitmap, anim);
                bitmap::Flip_Bitmap_(MOX::_draw_ship_bitmap, MOX::_ship_bitmap, 3);
                break;
            }
        }

        uint8_t status_flag = MOX::_combat_data[param_1].special_status_flag;
        if (status_flag == 4) {
            if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_PLAYER_NUM || MOX::_game_type == 1) {
                bitmap::Outline_Bitmap_(MOX::_ship_bitmap, MOX::_draw_ship_bitmap, 5, 0);
                bitmap::Mask_Out_Bitmap_With_Bitmap_(0, 0, MOX::_ship_bitmap, MOX::_draw_ship_bitmap, 0);
                animate::Draw_(x_offset + param_2, y_offset + param_3, MOX::_ship_bitmap);
            }
            return;
        }

        if (status_flag == 1 || status_flag == 5) {
            int val = (MOX::_ship_frame + param_1 * 2) % 16;
            Plasma_Darken_(MOX::_draw_ship_bitmap, val, 3);
        }

        if (MOX::_combat_data[param_1].reflected_damage_pool > 0 &&
            !CMBTMOV1::Ship_Is_In_Stasis_(param_1) &&
            !CMBTMOV1::Ship_In_Black_Hole_(param_1) &&
            MOX::_combat_data[param_1].plasma_web_damage == 0) {
            CMBTFIRE::Draw_Energy_Absorber_(param_1, param_2, param_3);
        }

        animate::Draw_(x_offset + param_2, y_offset + param_3, MOX::_draw_ship_bitmap);

        if (CMBTMOV1::Ship_Is_In_Stasis_(param_1)) {
            CMBTSPEC::Draw_Stasis_Ball_(param_1, 100, 0, 0);
            return;
        }

        if (MOX::_combat_data[param_1].plasma_web_damage > 0) {
            CMBTSPEC::Draw_Plasma_Web_(param_1, param_2, param_3);
            return;
        }

        if (CMBTMOV1::Ship_In_Black_Hole_(param_1)) {
            CMBTFIRE::Draw_BHG_(param_1, 0, 0);
        }
    }

    void __cdecl Get_Ship_Actual_Size_(int16_t ship_id, int16_t* width, int16_t* height) {
        int16_t x_offset;
        int16_t y_offset;

        CMBTDRW1::Draw_Ship_To_Bitmap_(ship_id, 0, 0, 0);

        bitmap::Get_Bitmap_Actual_Size_(
            MOX::_draw_ship_bitmap,
            &x_offset,
            &y_offset,
            width,
            height
        );
    }

    void __cdecl Create_Ship_Darken_Map_()
    {
        for (int i = 0; i < 0x20; ++i) {
            for (int j = 0; j < 0x10; ++j) {
                uint8_t val = static_cast<uint8_t>(i - j * 2);
                MOX::_ship_darken_map[i][j] = val;
                if (val > 0x20) {
                    MOX::_ship_darken_map[i][j] = 0;
                }
            }
        }

        for (int i = 0x20; i < 0x30; ++i) {
            for (int j = 0; j < 0x10; ++j) {
                uint8_t val = static_cast<uint8_t>(i - j);
                MOX::_ship_darken_map[i][j] = val;
                if (val < 0x20) {
                    MOX::_ship_darken_map[i][j] = 0;
                }
            }
        }

        for (int i = 0x30; i < 0x40; ++i) {
            for (int j = 0; j < 0x10; ++j) {
                uint8_t val = static_cast<uint8_t>(i - j);
                MOX::_ship_darken_map[i][j] = val;
                if (val < 0x30) {
                    MOX::_ship_darken_map[i][j] = 0;
                }
            }
        }

        for (int i = 0x40; i < 0x48; ++i) {
            for (int j = 0; j < 0x10; ++j) {
                uint8_t val = static_cast<uint8_t>(i - (j / 2));
                MOX::_ship_darken_map[i][j] = val;
                if (val < 0x40) {
                    MOX::_ship_darken_map[i][j] = 0;
                }
            }
        }

        for (int i = 0x48; i < 0x50; ++i) {
            for (int j = 0; j < 0x10; ++j) {
                uint8_t val = static_cast<uint8_t>(i - (j / 2));
                MOX::_ship_darken_map[i][j] = val;
                if (val < 0x48) {
                    MOX::_ship_darken_map[i][j] = 0;
                }
            }
        }

        for (int i = 0x60; i < 0x70; ++i) {
            for (int j = 0; j < 0x10; ++j) {
                uint8_t val = static_cast<uint8_t>(i - j);
                MOX::_ship_darken_map[i][j] = val;
                if (val < 0x60) {
                    MOX::_ship_darken_map[i][j] = 0;
                }
            }
        }

        for (int i = 0x70; i < 0x80; ++i) {
            for (int j = 0; j < 0x10; ++j) {
                uint8_t val = static_cast<uint8_t>(i - j);
                MOX::_ship_darken_map[i][j] = val;
                if (val < 0x70) {
                    MOX::_ship_darken_map[i][j] = 0;
                }
            }
        }
    }

    void __cdecl Draw_Board_Popup_() {
        fonts::Set_Font_Style_(2, &MOX::_combat_green);

        s_animation_header* anim = (s_animation_header*)Buffer_Reload_(s_combat_lbx_005599c8, 36, MOX::_global_cache_seg);
        animate::Draw_(150, 100, anim);

        BILL::_Print_Paragraph_(165, 153, 261, _g_msg, 2);

        fonts::Print_Integer_Centered_(182, 231, _g_min);
        fonts::Print_Integer_Centered_(296, 231, _g_var);
        fonts::Print_Integer_Centered_(408, 231, _g_max);

        int16_t offset = (int16_t)((((int32_t)_g_var - (int32_t)_g_min) * 216) / ((int32_t)_g_max - (int32_t)_g_min) + 36);

        graphics::Fill_(186, 253, offset + 149, 263, 193);

        offset += 150;

        line::Line_(offset, 253, offset, 263, 0);
    }

    void __cdecl Draw_Weapon_Status_Display_(int16_t param_1, int16_t param_2) {
        uint8_t color_b0[8] = { 0x53, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56 };
        uint8_t color_b8[8] = { 0x50, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53 };
        uint8_t color_a8[8] = { 0xdf, 0xdc, 0xdc, 0xdc, 0xdc, 0xdc, 0xdc, 0xdc };
        uint8_t color_6c[8] = { 0x48, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a };

        char local_a0[52];
        char local_64[20];
        char local_50[80];

        graphics::Interlaced_Fill_(114, 378, 263, 466, 0, 199);

        short y_pos = 380; // 0x17c

        for (int weapon_slot_idx = 0; weapon_slot_idx < 8; ++weapon_slot_idx) {
            int16_t w_id = MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].weapon_id;
            
            if (w_id != 0) {
                uint8_t arc = MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].arc;
                short is_ai = COMBAT1::Ship_Controlled_By_Computer_(MOX::_cur_ship);
                short display_color_mode = 0;

                if (is_ai == 0) {
                    if (COMBAT1::Weapon_Is_On_(MOX::_cur_ship, weapon_slot_idx) == 0) {
                        display_color_mode = MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].active == 1 ? 3 : 2;
                    } else if (param_2 < 0) {
                        if (param_1 != -1) {
                            short rel_bearing = Relative_Bearing_(MOX::_cur_ship, param_1);
                            uint8_t required_arc = Get_Weapon_Arc_(rel_bearing);

                            if ((arc & required_arc) != 0 &&
                                CMBTFIRE::Weapon_In_Range_(weapon_slot_idx, MOX::_cur_ship, param_1, 0, 0) != 0 &&
                                CMBTFIRE::Target_Is_Legal_For_Weapon_(
                                    MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].weapon_id,
                                    MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].specials,
                                    param_1, 0) != 0) {
                                display_color_mode = 1;
                            } else {
                                display_color_mode = 0;
                            }
                        } else {
                            display_color_mode = 0;
                        }
                    } else {
                        short rel_bearing = Relative_Bearing_XY_(MOX::_cur_ship, MOX::_missile[param_2].x, MOX::_missile[param_2].y);
                        uint8_t required_arc = Get_Weapon_Arc_(rel_bearing);

                        if ((arc & required_arc) == 0 ||
                            CMBTFIRE::Weapon_In_Range_(weapon_slot_idx, MOX::_cur_ship, param_2, 1, 0) == 0 ||
                            CMBTFIRE::Target_Is_Legal_For_Weapon_(
                                MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].weapon_id,
                                MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].specials,
                                param_2, 1) == 0) {
                            display_color_mode = 0;
                        } else {
                            display_color_mode = 1;
                        }
                    }
                } else {
                    display_color_mode = 0;
                }

                int8_t rack = TECHDATA::_weapons[w_id].type;
                int16_t ammo = MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].ammo;
                int8_t shots_left = MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].shots_left;
                uint8_t status = MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].status;

                if (((rack == 1 || rack == 3 || rack == 4) && ammo == 0) ||
                    (rack == 2 && ammo > 0) ||
                    (shots_left == 0 && status != 1)) {
                    display_color_mode = 2;
                }

                uint8_t* font_color;
                if (display_color_mode == 0) {
                    font_color = color_b8;
                } else if (display_color_mode == 1) {
                    font_color = color_b0;
                } else if (display_color_mode == 2) {
                    font_color = color_a8;
                } else {
                    font_color = color_6c;
                }

                fonts::Set_Font_Style_Shadow_Down_(1, (s_colors*)font_color);
                fonts::Set_Outline_Color_(0, 0);
                fonts::Set_Font_Spacing_(1);

                uint8_t count_to_print = (uint8_t)shots_left;
                if (count_to_print == 0 && status == 1) {
                    count_to_print = (uint8_t)MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].count;
                }

                fonts::Print_Integer_Right_(127, y_pos, count_to_print);

                uint16_t specials = MOX::_combat_data[MOX::_cur_ship].weapons[weapon_slot_idx].specials;

                if ((specials & 2) == 0) {
                    if ((specials & 4) == 0) {
                        local_a0[0] = '\0';
                    } else {
                        strcpy(local_a0, TECHDATA::_weapon_modifications[2].name);
                        strcat(local_a0, s_space_005510f0);
                    }
                } else {
                    strcpy(local_a0, TECHDATA::_weapon_modifications[1].name);
                    strcat(local_a0, s_space_005510f0);
                }

                if (shots_left == 1) {
                    strcat(local_a0, TECHDATA::_weapons[w_id].name);
                } else {
                    strcat(local_a0, TECHDATA::_weapons[w_id].name_plural);
                }

                if (display_color_mode == 0) {
                    fonts::Set_Font_Style_Shadow_Down_(1, (s_colors*)color_b8);
                } else if (display_color_mode == 1) {
                    fonts::Set_Font_Style_Shadow_Down_(1, (s_colors*)color_b0);
                } else if (display_color_mode == 2) {
                    fonts::Set_Font_Style_Shadow_Down_(1, (s_colors*)color_a8);
                }

                if (rack == 1 || rack == 3 || rack == 4 || w_id == 0x21) {
                    strcat(local_a0, s_space_x_005599a0);
                    _itoa(ammo, local_64, 10);
                    strcat(local_a0, local_64);
                } else if (rack == 2) {
                    if (ammo > 0) {
                        strcat(local_a0, s_space_005510f0);
                        _itoa(ammo, local_64, 10);
                        strcat(local_a0, local_64);
                        KEN::Ken_Get_Text_Message_(85, local_50, 80);
                        strcat(local_a0, local_50);
                    }
                } else if (rack == 0) {
                    strcat(local_a0, s_space_lparen_0055999c);
                    switch (arc) {
                        case 1:
                            KEN::Ken_Get_Text_Message_(3, local_50, 80);
                            strcat(local_a0, local_50);
                            break;
                        case 2:
                            KEN::Ken_Get_Text_Message_(4, local_50, 80);
                            strcat(local_a0, local_50);
                            break;
                        case 4:
                            KEN::Ken_Get_Text_Message_(6, local_50, 80);
                            strcat(local_a0, local_50);
                            break;
                        case 8:
                            KEN::Ken_Get_Text_Message_(5, local_50, 80);
                            strcat(local_a0, local_50);
                            break;
                        case 15:
                            strcat(local_a0, s_360_00556164);
                            break;
                    }
                    strcat(local_a0, s_rparen_00557f08);
                }

                fonts::Set_Outline_Color_(0, 0);
                fonts::Set_Font_Spacing_(1);
                fonts::Print_(131, y_pos, local_a0);
            }
            y_pos += 11;
        }
    }

    void __cdecl Plasma_Darken_(s_animation_header* param_1, int16_t param_2, int16_t param_3) {
        int frame = param_2 % 16;
        animate::Set_Animation_Frame_(MOX::_darken_plasma_map, frame);

        int32_t* darken_frame_offsets = reinterpret_cast<int32_t*>(MOX::_darken_plasma_map + 1);
        uint8_t* darken_data = reinterpret_cast<uint8_t*>(MOX::_darken_plasma_map) + darken_frame_offsets[MOX::_darken_plasma_map->current_frame];

        int32_t* param_1_frame_offsets = reinterpret_cast<int32_t*>(param_1 + 1);
        uint8_t* param_1_data = reinterpret_cast<uint8_t*>(param_1) + param_1_frame_offsets[param_1->current_frame];

        int16_t darkness = 0;
        switch (param_3) {
            case 0:
                darkness = 0;
                break;
            case 1:
                darkness = param_2 / 4;
                if (darkness > 6) darkness = 6;
                break;
            case 2:
                darkness = 6 - (param_2 / 4);
                if (darkness < 0) darkness = 0;
                break;
            case 3:
                darkness = 6;
                break;
            case 4:
                darkness = param_2 / 3;
                if (darkness > 20) darkness = 20;
                break;
            case 5:
                darkness = (param_2 / -3) + 12;
                if (darkness < 0) darkness = 0;
                break;
            default:
                // The original code has a bug where it reads an uninitialized variable for darkness.
                // Due to compiler stack layout mapping, it accidentally reads the lower 16 bits of the param_1_data pointer.
                darkness = static_cast<int16_t>(reinterpret_cast<std::uintptr_t>(param_1_data) & 0xFFFF);
                break;
        }

        if (darkness == 0) return;

        int16_t darken_w = animate::Get_Width_(MOX::_darken_plasma_map);
        int16_t darken_h = animate::Get_Height_(MOX::_darken_plasma_map);
        int16_t param_1_w = animate::Get_Width_(param_1);
        int16_t param_1_h = animate::Get_Height_(param_1);

        if (param_1_h <= 0) return;

        int32_t dst_offset = 0;
        int32_t src_offset = 0;

        for (int16_t y = 0; y < param_1_h; ++y) {
            if (y < darken_h) {
                if (param_1_w > 0) {
                    for (int16_t x = 0; x < param_1_w; ++x) {
                        if (x < darken_w) {
                            uint8_t darken_pixel = darken_data[src_offset + x];
                            uint32_t val;
                            if (darkness < 5) {
                                val = static_cast<uint32_t>((darken_pixel * darkness) >> 2);
                            } else {
                                val = static_cast<uint32_t>(darken_pixel + darkness - 4);
                            }
                            if (val > 15) {
                                val = 15;
                            }
                            uint8_t param_1_pixel = param_1_data[dst_offset + x];
                            param_1_data[dst_offset + x] = MOX::_ship_darken_map[param_1_pixel][val];
                        }
                    }
                }
            }
            dst_offset += param_1_w;
            src_offset += darken_w;
        }
    }

    void __cdecl Draw_Bombs_(int16_t attacker_idx, int16_t target_idx, int16_t weapon_id, int16_t damage) {
        bool is_bio = (TECHDATA::_weapons[weapon_id].natural_mods & 0x80) != 0;
        bool has_transporter = COMBINIT::Does_Combat_Ship_Have_Special_(attacker_idx, 0x23 /* Transporter */) != 0;
        int16_t speed = (MOX::_speedx2_flag != 0 || MOX::_net_flag != 0) ? 12 : 6;

        if (Ship_Completely_On_Screen_(attacker_idx) == 0) {
            COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[attacker_idx].x, MOX::_combat_data[attacker_idx].y);
        }

        int16_t target_x, target_y;
        Get_Ship_Center_Draw_XY_(target_idx, &target_x, &target_y);

        int16_t start_x, start_y;
        if (has_transporter) {
            int16_t mid_x = animate::Get_Width_(MOX::_combat_planet_seg) / 2;
            int16_t mid_y = animate::Get_Height_(MOX::_combat_planet_seg) / 2;
            Get_Ship_Center_Draw_XY_(attacker_idx, &start_x, &start_y);
            
            int16_t range = special::Range_(start_x, start_y, target_x, target_y);
            int32_t limit_sq = (int32_t)mid_x * mid_x + (int32_t)mid_y * mid_y + 3600;
            
            if ((int32_t)range * range > limit_sq) {
                do {
                    special::Absolute_Interpolate_Line_(&start_x, &start_y, start_x, start_y, target_x, target_y, 20);
                    range = special::Range_(start_x, start_y, target_x, target_y);
                } while ((int32_t)range * range > limit_sq);
            }
            CMBTSPEC::Draw_Transporter_Bomb_Beam_(attacker_idx, start_x, start_y);
        } else {
            Get_Ship_Center_Draw_XY_(attacker_idx, &start_x, &start_y);
        }

        int16_t facing = CMBTMOV1::Get_Facing_(start_x, start_y, target_x, target_y);
        int32_t projectile_entry = facing + (is_bio ? 160 /* 0xA0 */ : 144 /* 0x90 */);

        video::Set_Page_Back_();
        Full_Draw_Combat_Screen_();

        int16_t curr_x = start_x;
        int16_t curr_y = start_y;
        while (curr_x != target_x || curr_y != target_y) {
            timer::Mark_Time_();
            video::Set_Page_Off_();
            Draw_Main_Combat_Screen_();
            graphics::Set_Window_(0, 0, 639 /* 0x27F */, 350 /* 0x15E */);
            video::Clip_On_();

            special::Absolute_Interpolate_Line_(&curr_x, &curr_y, start_x, start_y, target_x, target_y, speed);
            
            s_animation_header* projectile_anim = (s_animation_header*)Buffer_Reload_("cmbtmisl.lbx", projectile_entry, MOX::_global_cache_seg);
            animate::Draw_(curr_x - 17, curr_y - 17, projectile_anim);
            
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);
        }

        if (is_bio) {
            BEAMS::Add_Damage_Indicator_For_Biological_(target_x, target_y, 1, damage);
        } else {
            BEAMS::Add_Damage_Indicator_For_Missile_(target_x, target_y, 1, damage);
        }

        int16_t sound_handle = (int16_t)ERICNET::Mox_Play_Sound_(24 /* 0x18 */);
        s_animation_header* explosion_anim = (s_animation_header*)Buffer_Reload_("cmbtsfx.lbx", 7, MOX::_global_cache_seg);
        int16_t frame_count = animate::Get_Animation_Frame_Count_(explosion_anim);

        for (int16_t i = 0; i < frame_count; ++i) {
            timer::Mark_Time_();
            video::Set_Page_Off_();
            Draw_Main_Combat_Screen_();
            graphics::Set_Window_(0, 0, 639 /* 0x27F */, 350 /* 0x15E */);
            video::Clip_On_();

            explosion_anim = (s_animation_header*)Buffer_Reload_("cmbtsfx.lbx", 7, MOX::_global_cache_seg);
            animate::Set_Animation_Frame_(explosion_anim, i);
            animate::Draw_(target_x - 12, target_y - 12, explosion_anim);
            
            BEAMS::Draw_Damage_Message_Queue_();
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);
        }

        BEAMS::Draw_Damage_Message_Queue_Until_Done_();
        sound::Fadeout_Sound_(sound_handle, 10);
    }

}