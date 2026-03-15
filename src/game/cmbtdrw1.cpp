#include "pch.h"

namespace CMBTDRW1 {

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
            MOX::_detail_fleet_icon_seg = (uint32_t)farload::Far_Reload_Next_("flticons.lbx", race, MOX::_screen_seg);
        } else {
            int16_t mode = is_editable ? 0 : 2;
            MOX::_detail_fleet_icon_seg = OFFICER::Load_Officer_Picture_(0, MOX::_global_combat_data.officer_idx, (uint32_t)MOX::_screen_seg, 0, mode);
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
                    uint8_t* p_bmp = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 400);
                    bitmap::Draw_Picture_To_Bitmap_((s_animation_header*)p_bmp, MOX::_combat_map_planet_seg);

                    for (int16_t i = 0x60; i < 0x70; ++i) {
                        int16_t shade = i - blip_frame;
                        if (shade < 0x60) shade = 0x60;
                        replace::Replace_Color_(p_bmp, i, shade);
                    }

                    animate::Draw_(blip_x + p_size - 7, blip_y + p_size - 7, reinterpret_cast<s_animation_header*>(p_bmp));
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
                int32_t absorption = TECHDATA::_armor[armor_idx].armor_absorption_percent;
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

            int16_t hull_armor = TECHDATA::_hull_data[hull_size].armor_absorption;
            int16_t armor_percent = TECHDATA::_armor[MOX::_combat_data[ship_idx].armor_type].armor_absorption_percent;
            
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

    uint8_t __cdecl Get_Weapon_Arc_(int16_t bearing) {
        uint8_t arc_flags = 0;

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
}