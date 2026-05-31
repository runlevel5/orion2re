#include "pch.h"

namespace SYS {
    char s__00551100[1] = "";
    void* _sys_data_buffer;
    const char s_fmt_star_display[7] = "%d> %s";
    const char s_1[3] = "\x1A" "1";
    const char s_percent_d_nl[6] = "%d%%\n";
    const char s_0s_1[7] = "\x1A" "0%s" "\x1A" "1";
    const char s_0s[5] = "\x1A" "0%s";
    const char s_m25_17[9] = "\a" "M25,17.";
    const char s_0s_1_dash[10] = "\x1A" "0%s" "\x1A" "1--\n";
    const char s_m25_27[9] = "\a" "M25,27.";
    const char s_0s_1d_nl[10] = "\x1A" "0%s" "\x1A" "1%d\n";
    const char s_0s_10_nl[9] = "\x1A" "0%s" "\x1A" "10\n";
    const char s_d_nl[4] = "%d\n";
    const char s_m25_30_0s[13] = "\a" "M25,30." "\x1A" "0%s";

    uint8_t _draw_sun_before_planet;
    int8_t _skip_draw_system_planets;
    const char s__00551120[2] = { '\x1B', '\0' };
    uint8_t _skip_draw_system_display;
    s_animation_header* _system_display_sun_seg;
    void* _sys_disp_bitmap;
    int8_t _remap_gray_percentages[10] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    uint8_t _remap_gray_mask_palette[10][256];
    s_animation_header* _mask_header;
    char _star_description_string[256];

    void __cdecl Clear_System_Display_Ship_Fields_() {
        memset(MOX::_system_display_ships, 0, sizeof(MOX::_system_display_ships));

        for (int i = 0; i < 15; ++i) {
            MOX::_system_display_ships[i].button = -1000;
            MOX::_system_display_ships[i].ship_idx = -1;
        }
    }

    // Suggested name: Need_To_Reload_System_Display
    bool __cdecl Need_To_Reload_System_Display_(int16_t star_index) {
        char buffer[120];

        sprintf(buffer, SYS::s_fmt_star_display, (int)star_index, &MOX::_star[star_index]);

        size_t len = strlen(buffer);
        if (len >= 20) {
            buffer[20] = '\0';
        }

        if (MOX::_star_name_loaded_string != nullptr) {
            // Re-calculate length after potential truncation for the string compare
            size_t current_len = strlen(buffer);
            if (strncmp(buffer, MOX::_star_name_loaded_string, current_len) == 0) {
                return false;
            }
        }

        return true;
    }

    void __cdecl Clear_System_Display_Loaded_Flag_() {
        if (MOX::_star_name_loaded_string != nullptr) {
            memset(MOX::_star_name_loaded_string, 0, 20);
        }
    }

    void __cdecl Set_System_Display_Loaded_Flag_(int16_t star_idx) {
        char temp_buffer[120];

        char* allocated = (char*)allocate::Allocate_First_Block_((void*)SYS::_sys_data_buffer, 20);
        MOX::_star_name_loaded_string = allocated;

        memset(allocated, 0, 20);

        s_star_data* star = &MOX::_star[star_idx];

        sprintf(temp_buffer, SYS::s_fmt_star_display, (int)star_idx, star->name);

        if (strlen(temp_buffer) >= 20) {
            temp_buffer[20] = '\0';
        }

        sprintf(MOX::_star_name_loaded_string, temp_buffer);
    }

    bool __cdecl Ok_To_View_System_(int16_t star_idx, int16_t player_idx) {
        // Collect visibility conditions
        bool has_visited = HAROLD::Player_Has_Visited_(player_idx, star_idx);
        bool is_omniscient = HAROLD::Player_Is_Omniscient_(player_idx);

        // System cannot be viewed if it is a black hole
        if (MOX::_star[star_idx].spectral_class == STAR_CLASS_BLACK_HOLE) {
            return false;
        }

        // Otherwise, it is viewable if the player has visited it or has omniscience
        return (has_visited || is_omniscient);
    }

    void __cdecl Load_New_System_Display_(int16_t star_idx) {
        bool has_planets = false;
        palette::Set_Palette_Changes_(0, 0xff);
        reduce::Clear_Reduced_Palette_Groups_();
        RUSS::Mox_Update_();

        MOX::_sys_disp = (s_sys_disp*)allocate::Allocate_Next_Block_((void*)SYS::_sys_data_buffer, 0x46);

        s_animation_header* background_pic = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x49, (void*)MOX::_global_cache_seg);
        MOX::_moveable_box[0].width = animate::Get_Width_(background_pic);
        MOX::_moveable_box[0].height = animate::Get_Height_(background_pic);

        s_star_data* star = &MOX::_star[star_idx];
        int8_t spectral_class = star->spectral_class;

        s_animation_header* sun_anim = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, spectral_class + 0x53, (void*)MOX::_global_cache_seg);
        MOX::_sun_seg[spectral_class] = sun_anim;
        _system_display_sun_seg = MOX::_sun_seg[spectral_class];

        RUSS::Mox_Update_();
        _draw_sun_before_planet = 1;

        for (int16_t orbit_idx = 0; orbit_idx < 5; ++orbit_idx) {
            int16_t planet_idx = star->planet_index[orbit_idx];
            if (planet_idx < 0) {
                MOX::_sys_disp[orbit_idx].orbit = -1;
                MOX::_sys_disp[orbit_idx].x = 999;
                MOX::_sys_disp[orbit_idx].y = 999;
            } else {
                s_planet_data* planet = &MOX::_planet[planet_idx];
                int8_t planet_size = planet->size;
                if (planet->planet_type == PLANET_TYPE_GAS_GIANT) {
                    planet_size = 4;
                }

                int32_t x_out, y_out, orbit_pos_out;
                GEO::Get_Orbit_Display_X_Y_(star_idx, (int32_t)planet->orbit, &x_out, &y_out, &orbit_pos_out);

                uint8_t dim = MOX::_rot_plan_dim[planet_size];
                int32_t offset = dim >> 1;

                if (orbit_idx == 0) {
                    if ((orbit_pos_out >= 0 && orbit_pos_out <= 2) || (orbit_pos_out >= 12 && orbit_pos_out <= 15)) {
                        _draw_sun_before_planet = 0;
                    }
                }

                MOX::_sys_disp[orbit_idx].x = (int16_t)x_out - (int16_t)offset;
                MOX::_sys_disp[orbit_idx].y = (int16_t)y_out - (int16_t)offset;
                MOX::_sys_disp[orbit_idx].orbit = planet_idx;
                MOX::_sys_disp[orbit_idx].orbit_idx = (int8_t)orbit_idx;
                MOX::_sys_disp[orbit_idx].oribit_pos = (int8_t)orbit_pos_out;

                allocate::Get_Remaining_Block_Space_((void*)_sys_data_buffer);
                Get_Rotating_Planet_Segment_(&MOX::_sys_disp[orbit_idx].pic, planet_idx, star_idx, 0);

                int16_t frame_count = animate::Get_Animation_Frame_Count_(MOX::_sys_disp[orbit_idx].pic);
                MOX::_sys_disp[orbit_idx].current_frame = (int16_t)((orbit_idx * 5) % frame_count);

                reduce::Add_Reduced_Palette_Group_(MOX::_sys_disp[orbit_idx].pic);
                has_planets = true;
            }
        }

        RUSS::Mox_Update_();

        if (has_planets) {
            reduce::Compress_Reduced_Palette_Groups_(0xc0, 0x40);
            remap::Update_Glass_Remap_Colors_();
            Calculate_Remap_Gray_Palettes_();

            if (MOX::_current_screen == SCREEN_MAIN && _skip_draw_system_planets == 0) {
                if (MOVEBOX::Moveable_Box_Selected_(2) != 0) {
                    MOVEBOX::Vanish_Fleet_Movement_Box_();
                    fields::Disable_Draw_All_Fields_();
                    video::Set_Page_Off_();
                    MAINSCR::Draw_Mini_Main_Screen_();
                    RUSS::Mox_Sync_Update_();
                    fields::Enable_Draw_All_Fields_();
                }
            }
        }

        allocate::Get_Remaining_Block_Space_((void*)_sys_data_buffer);
        MOX::_mainscr_palette = 0;
        RUSS::Mox_Update_();

        qsort(MOX::_sys_disp, 5, 0xe, qsort_sys_disp_);

        if (MOX::_look_at_colonies == 0) {
            if (!Ok_To_View_System_(star_idx, MOX::_PLAYER_NUM)) {
                MISC::Get_LBX_Message_(MOX::_star[star_idx].spectral_class + 0xf, SYS::_star_description_string);
            }
        }
    }

    void __cdecl Init_System_Display_(int16_t star_id) {
        if (SYS::Need_To_Reload_System_Display_(star_id)) {
            MOX::_system_star_id = star_id;
            SYS::Set_System_Display_Loaded_Flag_(star_id);
            SYS::_skip_draw_system_planets = 1;

            if (MOX::_moveable_box[0].id < MOX::_box_off_flag[0]) {
                SYS::_skip_draw_system_display = 1;
            }

            SYS::Load_New_System_Display_(star_id);
            MOX::_system_icon_scanned = -1;

            if (HAROLD::Player_Sighted_Monster_(MOX::_PLAYER_NUM, star_id)) {
                MAINPUPS::Clear_System_Special_Slots_();
            }
        }
    }

    void __cdecl Get_Rotating_Planet_Segment_(s_animation_header** out_ptr, int16_t planet_idx, int16_t arg3, int32_t arg4) {
        (void)arg3;
        (void)arg4;

        s_planet_data* planet = &MOX::_planet[planet_idx];
        int16_t colony_idx = planet->colony_index;
        uint16_t climate = (uint16_t)(uint8_t)planet->climate;
        int8_t size = planet->size;

        if (size > 3) {
            size = 4;
        }

        if (colony_idx > -1 && colony_idx < (int16_t)MOX::_NUM_COLONIES && planet->planet_type != PLANET_TYPE_GAS_GIANT) {
            if (MOX::_colony[colony_idx].outpost_flag == 0) {
                climate = (uint16_t)(uint8_t)MOX::_colony[colony_idx].climate;
            }
        }

        int16_t entry_idx;
        if (planet->planet_type == PLANET_TYPE_GAS_GIANT) {
            entry_idx = 50;
        } else {
            entry_idx = (int16_t)climate * 5 + (int16_t)size;
        }

        *out_ptr = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, (int)entry_idx + 0x5c, MOX::_global_cache_seg);
    }

    void __cdecl Potential_Colony_Info_Popup_(int16_t planet_id) {
        char bonus_msg_buf[32];
        char penalty_msg_buf[32];
        char fmt_text[200];
        char line_text[100];
        char val_text[32];
        char title_text[80];
        char main_text[1000];
        s_potential_colony_info info;

        strcpy(bonus_msg_buf, HAROLD::H_Message_(HESTR_16E_BONUS));
        strcpy(penalty_msg_buf, HAROLD::H_Message_(HESTR_16F_PENALTY));

        COLCALC::Compute_Potential_Colony_Info_(
            &MOX::_player[MOX::_PLAYER_NUM],
            &MOX::_planet[planet_id],
            &info
        );

        sprintf(fmt_text, s_m25_30_0s, HAROLD::H_Message_(HESTR_170_MAX_POPULATION));
        strcat(fmt_text, s_1);
        strcat(fmt_text, s_d_nl);

        int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(&MOX::_planet[planet_id], MOX::_PLAYER_NUM);
        sprintf(main_text, fmt_text, (int)max_pop);

        if (info.food == 0) {
            sprintf(line_text, s_0s_10_nl, HAROLD::H_Message_(HESTR_171_FOOD_PER_FARMER));
        } else {
            int food_val = (int)info.food / 2;
            sprintf(line_text, s_0s_1d_nl, HAROLD::H_Message_(HESTR_171_FOOD_PER_FARMER), food_val);
        }
        strcat(main_text, line_text);

        sprintf(line_text, s_0s_1d_nl, HAROLD::H_Message_(HESTR_172_INDUSTRY_PER_WORKER), (int)info.industry);
        strcat(main_text, line_text);

        sprintf(line_text, s_0s_1d_nl, HAROLD::H_Message_(HESTR_173_RESEARCH_PER_SCIENTIST), (int)info.research);
        strcat(main_text, line_text);

        if (info.maintenance == 0) {
            strcat(main_text, s_m25_27);
            sprintf(line_text, s_0s_1_dash, HAROLD::H_Message_(HESTR_174_NO_MAINTENANCE_BONUS));
            strcat(main_text, line_text);
        } else {
            strcat(main_text, s_m25_17);
            sprintf(line_text, s_0s, HAROLD::H_Message_(HESTR_175_MAINTENANCE));

            int16_t msg_id = (info.maintenance <= 0) ? HESTR_189_MAINTENANCE_BONUS : HESTR_18A_MAINTENANCE_PENALTY;
            sprintf(line_text, s_0s_1, HAROLD::H_Message_(msg_id));

            int abs_maint = abs((int)info.maintenance);
            sprintf(val_text, s_percent_d_nl, abs_maint);

            strcat(line_text, val_text);
            strcat(main_text, line_text);
        }

        if (info.worker_bonus == 0) {
            strcat(main_text, s_m25_27);
            sprintf(line_text, s_0s_1_dash, HAROLD::H_Message_(HESTR_176_NO_WORKER_BONUS));
            strcat(main_text, line_text);
        } else {
            strcat(main_text, s_m25_17);
            sprintf(line_text, s_0s, HAROLD::H_Message_(HESTR_177_WORKER));

            int16_t msg_id = (info.worker_bonus > 0) ? HESTR_18B_WORKER_BONUS : HESTR_18C_WORKER_PENALTY;
            sprintf(line_text, s_0s_1, HAROLD::H_Message_(msg_id));

            int abs_bonus = abs((int)info.worker_bonus);
            sprintf(val_text, s_percent_d_nl, abs_bonus);

            strcat(line_text, val_text);
            strcat(main_text, line_text);
        }

        strcpy(fmt_text, HAROLD::H_Message_(HESTR_178_COLONY_INFO_FOR_S));
        char* planet_name = HACCESS::Get_Planet_Name_No_Prime_(planet_id);
        sprintf(title_text, fmt_text, planet_name);

        TEXTBOX::Text_Box_(title_text, main_text);
    }

    int __cdecl qsort_sys_disp_(const void* a, const void* b) {
        auto p1 = static_cast<const s_sys_disp*>(a);
        auto p2 = static_cast<const s_sys_disp*>(b);
        return (int)p1->y - (int)p2->y;
    }

    void __cdecl Draw_System_Display_Border_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t x_offset, int16_t y_offset, int16_t mode) {
        (void)content_h;
        (void)x_offset;
        (void)y_offset;
        MOX::_moveable_box_pict[0] = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x49, (void*)MOX::_global_cache_seg);
        animate::Draw_(content_w + base_x, mode + base_y, MOX::_moveable_box_pict[0]);
    }

    void __cdecl Draw_Colony_Marker_(int16_t x, int16_t y, int16_t owner) {
        uint8_t color = (uint8_t)MOX::_player[owner].color;
        s_animation_header* anim = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, (int16_t)color + 0x23, (void*)MOX::_global_cache_seg);
        MOX::_colony_marker_seg[color] = anim;
        int16_t width = animate::Get_Width_(anim);
        animate::Draw_(x - width + 2, y, anim);
    }

    void __cdecl Draw_Outpost_Marker_(int16_t x, int16_t y, int16_t owner) {
        uint8_t color = (uint8_t)MOX::_player[owner].color;
        s_animation_header* anim = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, (int16_t)color + 0x2b, (void*)MOX::_global_cache_seg);
        MOX::_outpost_marker_seg[color] = anim;
        int16_t width = animate::Get_Width_(anim);
        animate::Draw_(x - width, y, anim);
    }

    void __cdecl Remap_Draw_With_Mask_(int16_t x, int16_t y, s_animation_header* anim_hdr, s_animation_header* mask_hdr) {
        bool is_clipped = false;
        int16_t x2;
        int16_t y2;

        if (animate::_clipping_flag == 1) {
            if (video::_screen_window_x2 < x) return;
            if (video::_screen_window_y2 < y) return;

            x2 = animate::_animation_header->width + x - 1;
            if (x2 < video::_screen_window_x1) return;

            y2 = animate::_animation_header->height + y - 1;
            if (y2 < video::_screen_window_y1) return;

            if (x < video::_screen_window_x1 ||
                y < video::_screen_window_y1 ||
                x2 > video::_screen_window_x2 ||
                y2 > video::_screen_window_y2) {
                is_clipped = true;
            }
        }

        animate::_animation_header = anim_hdr;
        SYS::_mask_header = mask_hdr;

        uint8_t* anim_frame = reinterpret_cast<uint8_t*>(anim_hdr) + anim_hdr->frame_offsets[anim_hdr->current_frame];
        int8_t* mask_frame = reinterpret_cast<int8_t*>(mask_hdr) + mask_hdr->frame_offsets[mask_hdr->current_frame];

        if (!is_clipped) {
            if (video::screen_refresh_flag >= 2) {
                setvesa::Add_Square_To_Scan_List_(x, y, anim_hdr->width + x - 1, anim_hdr->height + y - 1);
                setvesa::Add_Square_To_Scan_List_(x + 100, y + 100, anim_hdr->width + x + 99, anim_hdr->height + y + 99);
            }

            switch (static_cast<uint8_t>(anim_hdr->flags) & 3) {
            case 0:
                SYS::Remap_Draw_Animated_Sprite_Mask_(x, y, anim_frame, mask_frame);
                break;
            case 1:
                exitm::Exit_With_Message(s_Remap_draw_not_support_bitmaps_00557fc4);
                break;
            case 2:
                exitm::Exit_With_Message(s_Remap_draw_not_support_screen_00557fa4);
                break;
            case 3:
                exitm::Exit_With_Message(s_Remap_draw_not_support_map_00557f88);
                break;
            }
        } else {
            if (animate::_dot_stencil_flag == 0) {
                if (video::screen_refresh_flag >= 2) {
                    setvesa::Add_Square_To_Scan_List_(x, y, x2, y2);
                }

                switch (static_cast<uint8_t>(anim_hdr->flags) & 3) {
                case 0:
                    SYS::Remap_Clipped_Draw_Animated_Sprite_Mask_(x, y, anim_frame, mask_frame);
                    break;
                case 1:
                    exitm::Exit_With_Message(s_Remap_draw_not_support_bitmaps_00557fc4);
                    break;
                case 2:
                    exitm::Exit_With_Message(s_Remap_draw_not_support_screen_00557fa4);
                    break;
                case 3:
                    exitm::Exit_With_Message(s_Remap_draw_not_support_map_00557f88);
                    break;
                }
            }
        }

        SYS::_mask_header->current_frame++;
        animate::_animation_header->current_frame++;

        if (static_cast<uint8_t>(animate::_animation_header->flags) & 0x20) {
            int16_t loop_frame = animate::_animation_header->loop_frame;
            if (animate::_animation_header->current_frame > loop_frame) {
                if (loop_frame - animate::_animation_header->current_frame == -1) {
                    animate::_animation_header->current_frame = 0;
                } else if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                    animate::_animation_header->current_frame = animate::_animation_header->frame_count - 1;
                }
            }
        } else {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                animate::_animation_header->current_frame = animate::_animation_header->loop_frame;
            }
        }

        if (static_cast<uint8_t>(SYS::_mask_header->flags) & 0x20) {
            int16_t loop_frame = SYS::_mask_header->loop_frame;
            if (SYS::_mask_header->current_frame > loop_frame) {
                if (loop_frame - SYS::_mask_header->current_frame == -1) {
                    SYS::_mask_header->current_frame = 0;
                } else if (SYS::_mask_header->current_frame >= SYS::_mask_header->frame_count) {
                    SYS::_mask_header->current_frame = SYS::_mask_header->frame_count - 1;
                }
            }
        } else {
            if (SYS::_mask_header->current_frame >= SYS::_mask_header->frame_count) {
                SYS::_mask_header->current_frame = SYS::_mask_header->loop_frame;
            }
        }
    }

    void __cdecl Draw_Planet_With_Pierced_Ring_(
        int16_t planet_idx,
        int16_t sys_disp_idx,
        int16_t x_offset_1,
        int16_t y_offset_1,
        int16_t x_offset_2,
        int16_t y_offset_2,
        int16_t y_offset_3,
        int16_t x_offset_3) {
        const int32_t sys_slot = static_cast<int32_t>(sys_disp_idx);
        const int16_t orbit_idx = static_cast<int16_t>(MOX::_sys_disp[sys_slot].orbit_idx);
        const int16_t orbit_pos = static_cast<int16_t>(MOX::_sys_disp[sys_slot].oribit_pos);
        const int16_t planet_type = static_cast<int16_t>(MOX::_planet[planet_idx].planet_type);
        const int32_t planet_x = static_cast<int32_t>(MOX::_sys_disp[sys_slot].x);
        const int32_t planet_y = static_cast<int32_t>(MOX::_sys_disp[sys_slot].y);

        allocate::Push_Block_(MOX::_screen_seg);

        int16_t planet_mask_index;
        if (planet_type == PLANET_TYPE_PLANET) {
            planet_mask_index = static_cast<int16_t>(MOX::_planet[planet_idx].size);
        } else {
            planet_mask_index = 4;
        }

        int32_t bitmap_size = bitmap::Get_Bitmap_Size_(0x120, 0x9a);
        SYS::_sys_disp_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size + 10);

        bitmap_size = bitmap::Get_Bitmap_Size_(0x22, 0x22);
        void* outline_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size + 10);

        const int16_t ring_base_x = static_cast<int16_t>(x_offset_2 + x_offset_3 + x_offset_1);
        const int16_t ring_draw_x = static_cast<int16_t>(ring_base_x + 0x10);
        const int16_t ring_base_y = static_cast<int16_t>(y_offset_1 + y_offset_3 + y_offset_2);
        const int16_t ring_draw_y = static_cast<int16_t>(ring_base_y + 0x0c);

        MOX::_o_ring_seg[orbit_idx] =
            (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, orbit_idx + 0x4d, MOX::_global_cache_seg);
        animate::Get_Width_(MOX::_o_ring_seg[orbit_idx]);
        animate::Get_Height_(MOX::_o_ring_seg[orbit_idx]);

        s_animation_header* ring_picture =
            (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, orbit_idx + 0x4d, MOX::_global_cache_seg);
        bitmap::Draw_Picture_To_Bitmap_((s_animation_header*)SYS::_sys_disp_bitmap, ring_picture);

        const int16_t planet_screen_x = static_cast<int16_t>(ring_base_x + planet_x + 0x9f);
        const int16_t planet_screen_y = static_cast<int16_t>(ring_base_y + planet_y + 0x58);

        MOX::_planet_mask_seg[planet_mask_index] =
            (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, planet_mask_index + 0x8f, MOX::_global_cache_seg);
        animate::Set_Animation_Frame_(MOX::_planet_mask_seg[planet_mask_index], orbit_pos);

        SYS::Get_Rotating_Planet_Segment_(
            &MOX::_sys_disp[sys_slot].pic,
            MOX::_sys_disp[sys_slot].orbit,
            orbit_pos,
            0);
        remap::Calculate_Picture_Remap_Colors_(MOX::_sys_disp[sys_slot].pic);

        bitmap::Create_Blank_Bitmap_(0x23, 0x23, outline_bitmap);
        bitmap::Add_Picture_To_Bitmap_(2, 2, MOX::_sys_disp[sys_slot].pic, (s_animation_header*)outline_bitmap);
        bitmap::Outline_Bitmap_((s_animation_header*)outline_bitmap, (s_animation_header*)outline_bitmap, 0x1f, 0);
        bitmap::Outline_Bitmap_((s_animation_header*)outline_bitmap, (s_animation_header*)outline_bitmap, 0x1e, 0);
        bitmap::Mask_Out_Bitmap_With_Bitmap_(
            static_cast<int16_t>(planet_screen_x - ring_draw_x - 2),
            static_cast<int16_t>(planet_screen_y - ring_draw_y - 2),
            (s_animation_header*)SYS::_sys_disp_bitmap,
            (s_animation_header*)outline_bitmap,
            0);
        animate::Draw_(ring_draw_x, ring_draw_y, (s_animation_header*)SYS::_sys_disp_bitmap);

        MOX::_planet_mask_seg[planet_mask_index] =
            (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, planet_mask_index + 0x8f, MOX::_global_cache_seg);
        animate::Set_Animation_Frame_(MOX::_planet_mask_seg[planet_mask_index], orbit_pos);

        SYS::Get_Rotating_Planet_Segment_(
            &MOX::_sys_disp[sys_slot].pic,
            MOX::_sys_disp[sys_slot].orbit,
            orbit_pos,
            0);

        const int16_t frame_count = animate::Get_Animation_Frame_Count_(MOX::_sys_disp[sys_slot].pic);
        animate::Set_Animation_Frame_(
            MOX::_sys_disp[sys_slot].pic,
            static_cast<int32_t>(MOX::_ship_frame % frame_count));

        SYS::Remap_Draw_With_Mask_(
            planet_screen_x,
            planet_screen_y,
            MOX::_sys_disp[sys_slot].pic,
            MOX::_planet_mask_seg[planet_mask_index]);

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Draw_System_Display_Planets_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t y_offset, int16_t x_offset) {
        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_sys_disp[i].orbit;
            int8_t orbit_idx = MOX::_sys_disp[i].orbit_idx;
            if (planet_idx > -1) {
                if (MOX::_planet[planet_idx].planet_type == 3 || MOX::_planet[planet_idx].planet_type == 2) {
                    Draw_Planet_With_Pierced_Ring_(planet_idx, i, base_x, base_y, content_w, content_h, y_offset, x_offset);
                } else {
                    MOX::_asteroid_seg = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x5b, (void*)MOX::_global_cache_seg);
                    animate::Set_Animation_Frame_(MOX::_asteroid_seg, (int)orbit_idx);
                    animate::Draw_(x_offset + content_w + base_x + 0x10, y_offset + content_h + base_y + 0xc, (s_animation_header*)MOX::_asteroid_seg);
                }
            }
        }
    }

    void __cdecl Draw_Sun_Seg_(int16_t star_idx, int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t y_offset, int16_t x_offset) {
        _system_display_sun_seg = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, MOX::_star[star_idx].spectral_class + 0x53, (void*)MOX::_global_cache_seg);

        int16_t box_w = MOX::_moveable_box[0].width;
        int16_t box_h = MOX::_moveable_box[0].height;
        int16_t anim_h = animate::Get_Height_(_system_display_sun_seg);
        int16_t anim_w = animate::Get_Width_(_system_display_sun_seg);

        if (MOX::_current_screen == 0x1d) {
            animate::Draw_(0x1c0, 0x5c, _system_display_sun_seg);
        } else {
            // BUG: original code cross-references widths/heights
            int16_t draw_x = (int16_t)((box_w - anim_h) / 2) + content_w + base_x;
            int16_t draw_y = (int16_t)((box_h - anim_w) / 2) + content_h + base_y;
            animate::Draw_(draw_x, draw_y, _system_display_sun_seg);
        }
    }

    void __cdecl Draw_System_Special_Popup_(int16_t x, int16_t y, int16_t special_idx, int16_t anim_idx) {
        const char* desc = MAINPUPS::System_Special_Desc_(special_idx);
        const char* name = MOX::_planet_special_string[special_idx];
        s_animation_header* anim = MAINPUPS::System_Special_Anim_(anim_idx, special_idx);

        MAINPUPS::Draw_Special_Info_Popup_(x, y, anim, name, desc);
    }

    void __cdecl Print_Planet_Info_Short_(int16_t planet_idx, int16_t* box_width, int16_t* box_height) {
        Do_Print_Planet_Info_(0, 0, 0, 0, planet_idx, 0, 0, box_width, box_height, 0);
    }

    void __cdecl Print_Planet_Info_(int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7) {
        int16_t out_width;
        int16_t out_height;

        Do_Print_Planet_Info_(p1, p2, p3, p4, p5, p6, p7, &out_width, &out_height, 1);
    }

    void __cdecl Draw_System_Highlight_(short planet_idx, short rect_w, short rect_h, short x_offset, short y_offset) {
        int16_t colony_idx = MOX::_planet[planet_idx].colony_index;
        MOX::_o_highlight_seg = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x4b, (void*)MOX::_global_cache_seg);
        int16_t w = animate::Get_Width_(MOX::_o_highlight_seg);
        int16_t h = animate::Get_Height_(MOX::_o_highlight_seg);

        bool replaced = false;
        uint8_t player_color = 0;

        if (colony_idx < 0) {
            for (int i = 0; i < 9; ++i) {
                replace::Replace_Color_(MOX::_o_highlight_seg, (int16_t)(0x49 + i), (int8_t)(0x06 + i));
            }
            animate::Draw_((short)(((int)rect_w - (int)w) / 2) + x_offset, (short)(((int)rect_h - (int)h) / 2) + y_offset, MOX::_o_highlight_seg);
            for (int i = 0; i < 9; ++i) {
                replace::Replace_Color_(MOX::_o_highlight_seg, (int16_t)(0x06 + i), (int8_t)(0x49 + i));
            }
        } else {
            player_color = (uint8_t)MOX::_player[MOX::_colony[colony_idx].owner].color;
            if (player_color != 0) {
                uint8_t base_color = MOX::_main_palette_player_colors[player_color];
                for (int i = 0; i < 9; ++i) {
                    replace::Replace_Color_(MOX::_o_highlight_seg, (int16_t)(0x49 + i), (int8_t)(base_color + i));
                }
                replaced = true;
            }
            animate::Draw_((short)(((int)rect_w - (int)w) / 2) + x_offset, (short)(((int)rect_h - (int)h) / 2) + y_offset, MOX::_o_highlight_seg);
            if (replaced) {
                uint8_t base_color = MOX::_main_palette_player_colors[player_color];
                for (int i = 0; i < 9; ++i) {
                    replace::Replace_Color_(MOX::_o_highlight_seg, (int16_t)(base_color + i), (int8_t)(0x49 + i));
                }
            }
        }
    }

    void __cdecl Hide_System_Display_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t x_offset, int16_t y_offset, char mode)
    {
        int16_t x1, y1, x2, y2;

        if (MOX::_current_screen == 0x1d) {
            x1 = 0x132;
            y1 = 0x11;
            x2 = 0x270;
            y2 = 0xc5;
        } else {
            x2 = x_offset + content_w + base_x + 0x13f;
            x1 = x_offset + content_w + base_x - 1;
            y2 = content_h + base_y + y_offset + 0xb6;
            y1 = content_h + base_y + y_offset - 1;
        }

        s_colors font_colors;
        memcpy(&font_colors, MISC::Font_Colors2_(4, 'n', 's'), sizeof(font_colors));

        if (mode != 0) {
            int16_t clipped_y2 = video::_screen_window_y2;
            if (y2 <= clipped_y2) {
                clipped_y2 = y2;
            }
            MISC::Diagonal_Line_Fill_(x1, y1, x2 - 5, clipped_y2, 0x10, '=', ';');
            fonts::Set_Font_Style_(4, &font_colors);
        }
    }

    void __cdecl Calculate_Remap_Gray_Palettes_() {
        remap::Sort_Palette_List_();

        for (int i = 0; i < 10; ++i) {
            int scale = 100 - _remap_gray_percentages[i];
            for (int j = 0; j < 256; ++j) {
                const s_palette_entry& entry = video::_current_palette[j];
                _remap_gray_mask_palette[i][j] = remap::Find_Closest_Color_(
                    static_cast<uint8_t>((entry.r * scale) / 100),
                    static_cast<uint8_t>((entry.g * scale) / 100),
                    static_cast<uint8_t>((entry.b * scale) / 100)
                );
            }
        }
    }

    void __cdecl Draw_System_Display_Popup_(int16_t star_idx, int8_t draw_border, const char* title_text, int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t x_offset, int16_t y_offset) {
        char local_64[100];

        MOX::_ship_frame = (short)((MOX::_ship_frame + 1) % 32000);
        if (MOX::_current_screen == 0) {
            graphics::Set_Window_(0x16, 0x16, 0x20f, 0x1a5);
            video::Clip_On_();
        }

        MOX::_system_display_base_x = base_x;
        MOX::_system_display_base_y = base_y;

        if ((MOX::_look_at_colonies == '\0') && !Ok_To_View_System_(star_idx, MOX::_PLAYER_NUM)) {
            if (!HAROLD::Player_Sighted_Monster_(MOX::_PLAYER_NUM, star_idx)) {
                Hide_System_Display_(base_x, base_y, content_w, content_h, x_offset, y_offset, '\0');
                if (draw_border == '\0') {
                    return;
                }
                Draw_System_Display_Border_(base_x, base_y, content_w, content_h, x_offset, y_offset, 0);
                const char* pcVar5;
                if (HAROLD::Contact_With_One_Colony_(star_idx) == '\0') {
                    pcVar5 = HAROLD::H_Message_(0x16b);
                } else {
                    pcVar5 = HAROLD::H_Message_(0x16a);
                    sprintf(local_64, pcVar5, &MOX::_star[star_idx]);
                    pcVar5 = local_64;
                }
                Print_System_Display_Box_Title_(base_x, base_y, content_w, content_h, pcVar5);
                Print_Empty_System_Data_(star_idx);
                return;
            }

            Hide_System_Display_(base_x, base_y, content_w, content_h, x_offset, y_offset, '\x01');
            if (draw_border != '\0') {
                Draw_System_Display_Border_(base_x, base_y, content_w, content_h, x_offset, y_offset, 1);
            }

            if (MOX::_current_screen == 0x1d) {
                graphics::Save_Window_();
                graphics::Set_Window_(0, 0, 0x27f, 0x1df);
                video::Clip_On_();
                Draw_System_Special_Popup_(content_w + base_x - 5, y_offset + content_h + base_y + 0x28, 9, star_idx);
                graphics::Restore_Window_();
                video::Clip_Off_();
                return;
            }

            Draw_System_Special_Popup_(content_w + base_x + 5, y_offset + content_h + base_y, 9, star_idx);
            const char* pcVar5 = HAROLD::H_Message_(0x16a);
            sprintf(local_64, pcVar5, &MOX::_star[star_idx]);
            Print_System_Display_Box_Title_(base_x, base_y, content_w, content_h, local_64);
            return;
        }

        if (draw_border != '\0') {
            Draw_System_Display_Border_(base_x, base_y, content_w, content_h, x_offset, y_offset, 1);
        }

        MOX::_system_star_id_for_auto_select = star_idx;
        int iVar6 = (int)star_idx;
        if (MOX::_star[iVar6].wormhole_star_id > -1) {
            short sVar15 = (short)MOX::_star[iVar6].wormhole_star_id;
            s_colors* psVar7 = MISC::Font_Colors2_(1, 'n', 's');
            s_colors local_bc;
            memcpy(&local_bc, psVar7, sizeof(s_colors));
            fonts::Set_Font_Style_(1, &local_bc);

            short local_c2;
            short sStack_c0;
            if (MOX::_current_screen == 0x1d) {
                local_c2 = 0x270;
                sStack_c0 = 0xbb;
            } else {
                local_c2 = base_x + 0x149 + content_w;
                sStack_c0 = content_h;
                short sVar11 = fonts::Get_Font_Height_();
                sStack_c0 = (sStack_c0 - sVar11) + 0xe4 + base_y;
            }

            char local_b4[100];
            local_b4[0] = '\0';

            if (MOX::_star[iVar6].wormhole_star_id <= -1) {
                if (HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, star_idx)) {
                    const char* pcVar5 = MOX::_planet_special_string[MOX::_star[iVar6].system_special];
                    strcpy(local_b4, pcVar5);
                }
            } else {
                if (HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, sVar15)) {
                    const char* pcVar5 = HAROLD::H_Message_(0x16c);
                    sprintf(local_b4, pcVar5, &MOX::_star[sVar15]);
                } else {
                    const char* pcVar5 = HAROLD::H_Message_(0x16d);
                    strcpy(local_b4, pcVar5);
                }
            }

            if (local_b4[0] != '\0') {
                fonts::Print_Right_(local_c2, sStack_c0, local_b4);
            }
        }

        if (_draw_sun_before_planet != '\0') {
            Draw_Sun_Seg_(star_idx, base_x, base_y, content_w, content_h, y_offset, x_offset);
        }

        if (_skip_draw_system_planets == '\0') {
            Draw_System_Display_Planets_(base_x, base_y, content_w, content_h, y_offset, x_offset);
            animate::_clipping_flag = 1;
        } else {
            _skip_draw_system_planets = '\0';
        }

        if (_draw_sun_before_planet == '\0') {
            Draw_Sun_Seg_(star_idx, base_x, base_y, content_w, content_h, y_offset, x_offset);
        }

        for (short sVar15 = 0; sVar15 < 5; ++sVar15) {
            s_sys_disp* psVar1 = &MOX::_sys_disp[sVar15];
            if (psVar1->orbit > -1) {
                s_planet_data* psVar8 = &MOX::_planet[psVar1->orbit];
                int8_t planet_size = psVar8->size;
                if (psVar8->planet_type == '\x02') {
                    planet_size = 4;
                }
                short sVar11 = psVar1->x + x_offset + content_w + 0x9f + base_x;
                short sVar_y = psVar1->y + y_offset + content_h + 0x58 + base_y;

                if (psVar8->colony_index > -1) {
                    if (psVar8->planet_type == '\x01') {
                        short offset = (short)(MOX::_rot_plan_dim[planet_size] >> 1);
                        sVar11 += offset;
                        sVar_y += offset;
                    }
                    short sVar4 = (short)MOX::_colony[psVar8->colony_index].owner;
                    if (MOX::_colony[psVar8->colony_index].outpost_flag == '\0') {
                        Draw_Colony_Marker_(sVar11, sVar_y, sVar4);
                    } else {
                        Draw_Outpost_Marker_(sVar11, sVar_y, sVar4);
                    }
                }
            }
        }

        for (short sVar15 = 0; sVar15 < 5; ++sVar15) {
            short sVar11 = MOX::_sys_disp[sVar15].orbit;
            if (sVar11 > -1) {
                int8_t planet_size = MOX::_planet[sVar11].size;
                if (MOX::_planet[sVar11].planet_type == '\x02') {
                    planet_size = 4;
                }
                short sVar4 = (short)(MOX::_sys_disp[sVar15].x + x_offset + content_w + 0x9f + base_x);
                short sVar_y = (short)(MOX::_sys_disp[sVar15].y + y_offset + content_h + 0x58 + base_y);

                if (MOX::_sys_disp[sVar15].orbit_idx == MOX::_system_icon_scanned) {
                    if (MOX::_planet[sVar11].planet_type != '\x01') {
                        uint16_t dim = (uint16_t)MOX::_rot_plan_dim[planet_size];
                        Draw_System_Highlight_(sVar11, dim, dim, sVar4, sVar_y);
                    }
                    short local_c4, local_c6;
                    Print_Planet_Info_Short_(sVar11, &local_c4, &local_c6);
                    MISC::Draw_Darkened_Box_(x_offset + content_w + base_x, y_offset + content_h + base_y, local_c4 + 9, local_c6 + 4, 0x3f);
                    Print_Planet_Info_(base_x, base_y, content_w, content_h, sVar11, x_offset, y_offset);
                }
            }
        }

        Draw_System_Ship_Icons_(base_x, base_y, content_w, content_h, y_offset, x_offset);
        Draw_Gate_Icons_(star_idx, base_x, base_y, content_w, content_h, x_offset);

        if ((MOX::_system_icon_scanned > 1999) && (MOX::_system_icon_scanned < 2004)) {
            Print_Gate_Info_(star_idx, MOX::_system_icon_scanned - 2000, base_x, base_y, content_w, content_h, x_offset, y_offset);
        }

        if (MOX::_current_screen != 0x1d) {
            Print_System_Display_Box_Title_(base_x, base_y, content_w, content_h, title_text);
        }
    }

    void __cdecl N_Ships_By_Type_At_Location_(int16_t x, int16_t y, int16_t target_owner, int16_t *total_count, int16_t *bases_count, int16_t *colony_ships_count, int16_t *transports_count, int16_t *ship_sizes) {
        *total_count = 0;
        *bases_count = 0;
        *colony_ships_count = 0;
        *transports_count = 0;
        memset(ship_sizes, 0, 6 * sizeof(int16_t));

        if (MOX::_NUM_SHIPS <= 0) {
            return;
        }

        for (int16_t ship_idx = 0; ship_idx < (int16_t)MOX::_NUM_SHIPS; ++ship_idx) {
            if (HACCESS::Ship_Owner_(ship_idx) != target_owner) {
                continue;
            }

            s_ship_data* ship = &MOX::_ship[ship_idx];

            if (ship->status >= 3) {
                continue;
            }

            if (ship->x != x || ship->y != y) {
                continue;
            }

            int16_t size = (int16_t)ship->d.size;
            if (size > 5) {
                size = 5;
            }

            switch (ship->d.ship_type) {
                case 0:
                    ship_sizes[size]++;
                    break;
                case 1:
                    (*bases_count)++;
                    break;
                case 2:
                    (*transports_count)++;
                    break;
                case 4:
                    (*colony_ships_count)++;
                    break;
                default:
                    break;
            }

            (*total_count)++;
        }
    }

    void __cdecl Get_Gate_Icon_XY_(int16_t x, int16_t y, int16_t off_x, int16_t off_y, int16_t unused, int16_t* out_x, int16_t* out_y) {
        *out_x = x + off_x + 0xE9;
        *out_y = y + off_y + 0xF3;

        if (MOX::_current_screen == 0x1D) {
            *out_x += 0x3C;
            *out_y -= 0x46;
        }
    }

    void __cdecl Draw_Gate_Icons_(int16_t star_idx, int16_t x, int16_t y, int16_t width, int16_t height, int32_t unknown) {
        int16_t out_x;
        int16_t out_y;

        SYS::Get_Gate_Icon_XY_(x, y, width, height, (int16_t)unknown, &out_x, &out_y);

        if (MOX::_current_screen == 29 /* SCREEN_GALAXY */) {
            out_y -= 6;
        }

        s_star_data* star = &MOX::_star[star_idx];

        if (HAROLD::Star_Has_Artemis_Net_(star)) {
            out_x -= 12;
            MOX::_gate_icon_seg[14] = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 33 /* LBX_ENTRY_ARTEMIS */, MOX::_global_cache_seg);
            animate::Draw_(out_x, out_y, MOX::_gate_icon_seg[14]);
        }

        if (HAROLD::Star_Has_Dimensional_Portal_(star)) {
            out_y += 1;
            out_x -= 12;
            MOX::_gate_icon_seg[2] = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 21 /* LBX_ENTRY_DIMENSIONAL_PORTAL */, MOX::_global_cache_seg);
            animate::Draw_(out_x, out_y, MOX::_gate_icon_seg[2]);
        }
    }

    void __cdecl Print_System_Display_Box_Title_(int16_t x_offset, int16_t y_offset, int16_t box_x, int16_t box_y, const char* text) {
        s_colors colors;
        int16_t text_width;
        int16_t current_spacing;
        int16_t current_style;
        int16_t font_height;
        int16_t image_width;
        int16_t print_x;
        int16_t print_y;

        s_colors* temp_colors = MISC::Font_Colors2_(4, 0x3c /* '<' */, 0x41 /* 'A' */);
        colors = *temp_colors;

        fonts::Set_Font_Style_(4, &colors);
        text_width = fonts::Get_String_Width_(text);

        if (text_width > 265 /* 0x109 */) {
            fonts::Set_Font_Style_(3, &colors);
        }

        if (text_width < 60 /* 0x3c */) {
            fonts::Set_Font_Spacing_(4);
        } else {
            fonts::Set_Font_Spacing_(2);
        }

        current_spacing = MISC::Get_Font_Spacing_();
        current_style = fonts::_current_font_style;
        font_height = fonts::Get_Font_Height_();

        MOX::_moveable_box_pict[0] = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x49, (void*)MOX::_global_cache_seg);
        image_width = animate::Get_Width_(MOX::_moveable_box_pict[0]);

        print_x = x_offset + box_x + (image_width / 2);
        print_y = ((20 - (font_height + 1)) / 2) + box_y + y_offset + 17;

        MISC::Set_Fitted_Font_Style_((char*)text, &colors, current_style, current_spacing, 0, 0x131, 1);

        current_style = MISC::Get_Font_Style_();
        current_spacing = MISC::Get_Font_Spacing_();

        temp_colors = MISC::Font_Colors2_(current_style, 0x3c /* '<' */, 0x41 /* 'A' */);
        colors = *temp_colors;

        fonts::Set_Font_Style_(current_style, &colors);
        fonts::Set_Font_Spacing_(current_spacing);

        if (current_style < 3) {
            print_y = (print_y - current_style) + 3;
        }

        MISC::_Print_Centered_Shadow_Down_(print_x, print_y, 0x3d /* '=' */, text);
    }

    /**
     * @address 00497f80
     */
    s_animation_header* __cdecl Buffer_Reload_System_Ships_(int16_t display_idx) {
        int16_t node_ship_idx = MOX::_ship_node[MOX::_system_display_ships[display_idx].ship_idx].ship_idx;
        int16_t owner_idx = HACCESS::Ship_Owner_(node_ship_idx);

        if (owner_idx < 8) {
            // Reload buffer based on player color
            uint8_t player_color = static_cast<uint8_t>(MOX::_player[owner_idx].color);
            return (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, player_color + 273 /* 0x111 */, MOX::_global_cache_seg);
        }

        if (owner_idx == 8) {
            // Neutral or special owner case
            return (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 281 /* 0x119 */, MOX::_global_cache_seg);
        }

        if (owner_idx > 8 && owner_idx < 15) {
            // Other special owner ranges
            return (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, (owner_idx - 9) + 282 /* 0x11a */, MOX::_global_cache_seg);
        }

        return nullptr;
    }

    void __cdecl Print_System_Ship_Data_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t ship_idx) {
        int16_t v1;
        int16_t v2;

        SYS::Do_Print_System_Ship_Data_(base_x, base_y, content_w, content_h, ship_idx, &v1, &v2, 1);
    }

    void __cdecl Draw_System_Ship_Icons_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t y_offset, int16_t x_offset) {
        int16_t icon_scanned = (int16_t)MOX::_system_icon_scanned;

        for (int16_t i = 0; i < 15; ++i) {
            int16_t ship_idx = MOX::_system_display_ships[i].ship_idx;
            if (ship_idx > -1) {
                int16_t target_x = MOX::_system_display_ships[i].x + content_w + base_x;
                int16_t target_y = MOX::_system_display_ships[i].y + content_h + base_y;

                if (icon_scanned > 999 && (ship_idx - icon_scanned == -1000)) {
                    int16_t text_w, text_h;
                    SYS::Measure_System_Ship_Data_(base_x, base_y, content_w, content_h, ship_idx, &text_w, &text_h);

                    int16_t box_x = x_offset + content_w + base_x;
                    int16_t box_y = y_offset + content_h + base_y;
                    MISC::Draw_Darkened_Box_(box_x, box_y, text_w + 9, text_h + 4, 0x3F /* '?' */);

                    SYS::Print_System_Ship_Data_(base_x, base_y, content_w, content_h, ship_idx);

                    MOX::_system_display_ships[i].pic = SYS::Buffer_Reload_System_Ships_(i);
                    animate::Get_Current_Frame_(MOX::_system_display_ships[i].pic);
                } else {
                    MOX::_system_display_ships[i].pic = SYS::Buffer_Reload_System_Ships_(i);
                    animate::Reset_Animation_Frame_(MOX::_system_display_ships[i].pic);
                    animate::Get_Current_Frame_(MOX::_system_display_ships[i].pic);
                }

                animate::Draw_(target_x, target_y, MOX::_system_display_ships[i].pic);
                icon_scanned = (int16_t)MOX::_system_icon_scanned;
            }
        }
    }

    /**
     * @address 00495bf0
     */
    void __cdecl Measure_System_Ship_Data_(int16_t x, int16_t y, int16_t w, int16_t h, int16_t p5, int16_t* out_w, int16_t* out_h) {
        // Mode 0 is used for measurement calculation instead of actual drawing.
        SYS::Do_Print_System_Ship_Data_(x, y, w, h, p5, out_w, out_h, 0);
    }

    int16_t __cdecl Print_Special_Building_Data_(
            int16_t colony_idx,
            int16_t x,
            int16_t* line_pos,
            int16_t* max_text_width,
            int16_t* total_height,
            int16_t line_spacing,
            bool draw) {
        int16_t visible_count = 0;
        const int16_t special_building_ids[7] = { 0x29, 0x28, 0x08, 0x1A, 0x1B, 0x2A, 0x2F };

        s_colony* colony = &MOX::_colony[colony_idx];

        for (int16_t i = 0; i < 7; ++i) {
            const int16_t building_id = special_building_ids[i];

            if (colony->buildings[building_id] != 0) {
                if (visible_count == 0) {
                    *line_pos = static_cast<int16_t>(*line_pos + MISC::Get_Font_Style_() + 3);
                    *total_height = static_cast<int16_t>(*total_height + MISC::Get_Font_Style_() + 3);
                }

                *line_pos = static_cast<int16_t>(*line_pos + line_spacing);

                if (draw) {
                    fonts::Print_(x, *line_pos, TECHDATA::_buildings[building_id].name);
                } else {
                    *total_height = static_cast<int16_t>(*total_height + line_spacing);

                    const int16_t text_width =
                        fonts::Get_String_Width_(TECHDATA::_buildings[building_id].name);

                    if (*max_text_width < text_width) {
                        *max_text_width = text_width;
                    }
                }

                ++visible_count;
            }
        }

        return visible_count;
    }

    void __cdecl Do_Print_System_Ship_Data_(
        int16_t base_x,
        int16_t base_y,
        int16_t x_offset,
        int16_t y_offset,
        int16_t ship_node_num,
        int16_t* width_out,
        int16_t* height_out,
        bool draw_text
    ) {
        int16_t print_x;
        int16_t print_y;
        int16_t ship_idx;
        int16_t owner;
        int16_t group_a_count;
        int16_t group_b_count;
        int16_t group_c_count;
        int16_t unknown_count;
        int16_t hull_counts[6];
        s_colors font_colors;
        char line_buffer[200];
        char ship_name[80];

        ship_name[0] = '\0';
        *width_out = 0;
        *height_out = 4;

        if (MOX::_current_screen == 0x1d) {
            print_x = static_cast<int16_t>(base_x + x_offset + 8);
            print_y = static_cast<int16_t>(base_y + y_offset + 10);
        } else {
            print_x = static_cast<int16_t>(base_x + x_offset + 0x12);
            print_y = static_cast<int16_t>(base_y + y_offset + 0x36);
        }

        if (ship_node_num >= 5000) {
            ship_idx = MOX::_ship_node[static_cast<int16_t>(ship_node_num - 5000)].ship_idx;
            sprintf(ship_name, MOX::_ship[ship_idx].d.name);
        } else {
            ship_idx = MOX::_ship_node[ship_node_num].ship_idx;
            owner = HACCESS::Ship_Owner_(ship_idx);
            if (owner >= 8) {
                sprintf(ship_name, MOX::_ship[ship_idx].d.name);
            }
        }

        if (static_cast<int8_t>(ship_name[0]) > 0) {
            font_colors = *MISC::Font_Colors2_(1, 0x6e, 0x73);
            fonts::Set_Font_Style_(2, &font_colors);
            fonts::Set_Font_Spacing_(1);
            (void)fonts::Get_Font_Height_();

            if (draw_text) {
                fonts::Print_(print_x, print_y, ship_name);
                return;
            }

            {
                int16_t text_width = fonts::Get_String_Width_(ship_name);
                if (*width_out < text_width) {
                    *width_out = text_width;
                }
            }

            *height_out = fonts::Get_Font_Height_();
            return;
        }

        MOX::_moveable_box_pict[0] = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x49, MOX::_global_cache_seg);
        (void)animate::Get_Width_(MOX::_moveable_box_pict[0]);
        (void)animate::Get_Height_(MOX::_moveable_box_pict[0]);

        ship_idx = MOX::_ship_node[ship_node_num].ship_idx;
        owner = HACCESS::Ship_Owner_(ship_idx);

        {
            s_ship_data* const ship = &MOX::_ship[ship_idx];
            s_player* const player = &MOX::_player[owner];
            const uint8_t palette_color =
                MOX::_main_palette_player_colors[static_cast<uint8_t>(player->color)];
            int16_t visible_detail_lines;

            font_colors = *MISC::Font_Colors2_(1, palette_color, static_cast<int32_t>(palette_color) + 5);
            fonts::Set_Font_Style_(2, &font_colors);
            fonts::Set_Font_Spacing_(1);
            const int8_t line_step =
                static_cast<int8_t>(static_cast<uint8_t>(fonts::Get_Font_Height_()) + 1);

            SYS::N_Ships_By_Type_At_Location_(
                ship->x,
                ship->y,
                owner,
                &unknown_count,
                &group_a_count,
                &group_b_count,
                &group_c_count,
                hull_counts
            );

            visible_detail_lines = 0;
            if (group_c_count > 0) {
                ++visible_detail_lines;
            }
            if (group_b_count > 0) {
                ++visible_detail_lines;
            }
            if (group_a_count > 0) {
                ++visible_detail_lines;
            }

            for (int16_t hull_idx = 0; hull_idx < 6; ++hull_idx) {
                if (hull_counts[hull_idx] > 0) {
                    ++visible_detail_lines;
                }
            }

            sprintf(line_buffer, HAROLD::H_Message_(0x163), player->race_name);
            fonts::Set_Font_Style_(2, &font_colors);
            fonts::Set_Font_Spacing_(1);

            if (draw_text) {
                fonts::Print_(print_x, print_y, line_buffer);
            } else {
                int16_t text_width = fonts::Get_String_Width_(line_buffer);
                if (*width_out < text_width) {
                    *width_out = text_width;
                }
            }

            for (int16_t hull_idx = 0; hull_idx < 6; ++hull_idx) {
                const int16_t hull_count = hull_counts[hull_idx];
                if (hull_count > 0) {
                    const char* hull_name =
                        (hull_count == 1)
                            ? TECHDATA::_hull_data[hull_idx].name
                            : TECHDATA::_hull_data[hull_idx].size_name;

                    sprintf(line_buffer, s__2d__s_00557f10, static_cast<int32_t>(hull_count), hull_name);
                    print_y = static_cast<int16_t>(print_y + static_cast<int16_t>(line_step));

                    if (draw_text) {
                        fonts::Print_(static_cast<int16_t>(print_x + 3), print_y, line_buffer);
                    } else {
                        int16_t text_width =
                            static_cast<int16_t>(fonts::Get_String_Width_(line_buffer) + 3);
                        if (*width_out < text_width) {
                            *width_out = text_width;
                        }
                    }
                }
            }

            if (group_a_count > 0) {
                if (group_a_count == 1) {
                    sprintf(line_buffer, HAROLD::H_Message_(0x164), static_cast<int32_t>(group_a_count));
                } else {
                    sprintf(line_buffer, HAROLD::H_Message_(0x165));
                }

                print_y = static_cast<int16_t>(print_y + static_cast<int16_t>(line_step));

                if (draw_text) {
                    fonts::Print_(static_cast<int16_t>(print_x + 3), print_y, line_buffer);
                } else {
                    int16_t text_width =
                        static_cast<int16_t>(fonts::Get_String_Width_(line_buffer) + 3);
                    if (*width_out < text_width) {
                        *width_out = text_width;
                    }
                }
            }

            if (group_b_count > 0) {
                sprintf(
                    line_buffer,
                    HAROLD::H_Message_(group_b_count == 1 ? 0x166 : 0x167),
                    static_cast<int32_t>(group_b_count)
                );

                print_y = static_cast<int16_t>(print_y + static_cast<int16_t>(line_step));

                if (draw_text) {
                    fonts::Print_(static_cast<int16_t>(print_x + 3), print_y, line_buffer);
                } else {
                    int16_t text_width =
                        static_cast<int16_t>(fonts::Get_String_Width_(line_buffer) + 3);
                    if (*width_out < text_width) {
                        *width_out = text_width;
                    }
                }
            }

            if (group_c_count > 0) {
                sprintf(
                    line_buffer,
                    HAROLD::H_Message_(group_c_count == 1 ? 0x168 : 0x169),
                    static_cast<int32_t>(group_c_count)
                );

                print_y = static_cast<int16_t>(print_y + static_cast<int16_t>(line_step));

                if (draw_text) {
                    fonts::Print_(static_cast<int16_t>(print_x + 3), print_y, line_buffer);
                } else {
                    int16_t text_width =
                        static_cast<int16_t>(fonts::Get_String_Width_(line_buffer) + 3);
                    if (*width_out < text_width) {
                        *width_out = text_width;
                    }
                }
            }

            *height_out = static_cast<int16_t>(
                static_cast<int16_t>(line_step) * static_cast<int16_t>(visible_detail_lines + 1)
            );
        }
    }

    void __cdecl Do_Print_Planet_Info_(
            int16_t left,
            int16_t top,
            int16_t inner_left,
            int16_t inner_top,
            int16_t planet_idx,
            int16_t text_left,
            int16_t text_top,
            int16_t* max_width,
            int16_t* total_height,
            bool draw) {
        int16_t special_building_data_height = 0;
        int16_t colony_owner = -1;
        s_colors font_colors;
        char title_buffer[60];
        char detail_buffer[80];

        s_planet_data* planet = &MOX::_planet[planet_idx];
        const bool can_show_special_buildings =
                (MOX::_player_has_ship_in_system != 0) ||
                HAROLD::Player_Has_Colony_(
                        MOX::_PLAYER_NUM,
                        static_cast<int16_t>(static_cast<int8_t>(planet->star_index))) ||
                HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM);

        *max_width = 0;
        *total_height = 12;

        MOX::_moveable_box_pict[0] =
                (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x49, MOX::_global_cache_seg);
        (void)animate::Get_Width_(MOX::_moveable_box_pict[0]);
        (void)animate::Get_Height_(MOX::_moveable_box_pict[0]);

        memcpy(&font_colors, MISC::Font_Colors2_(2, 0x3c, 0x43), sizeof(font_colors));

        const int16_t colony_idx = planet->colony_index;
        s_colony* colony = nullptr;
        if (colony_idx > -1) {
            colony = &MOX::_colony[colony_idx];
            colony_owner = static_cast<int8_t>(colony->owner);
        }

        fonts::Set_Font_Style_(2, &font_colors);
        fonts::Set_Font_Spacing_(1);

        const int16_t line_advance =
                static_cast<int16_t>(static_cast<uint8_t>(fonts::Get_Font_Height_()) + 1);
        const int16_t text_x = static_cast<int16_t>(text_left + inner_left + left + 6);
        int16_t current_y = static_cast<int16_t>(text_top + inner_top + top + 7);

        if (planet->planet_type == 2) {
            strcpy(title_buffer, HAROLD::H_Message_(0x15f));

            if (draw) {
                fonts::Print_(text_x, current_y, title_buffer);
            } else {
                const int16_t width = fonts::Get_String_Width_(title_buffer);
                if (*max_width < width) {
                    *max_width = width;
                }
            }
        } else if (planet->planet_type == 1) {
            strcpy(title_buffer, HAROLD::H_Message_(0x160));

            if (draw) {
                fonts::Print_(text_x, current_y, title_buffer);
            } else {
                const int16_t width = fonts::Get_String_Width_(title_buffer);
                if (*max_width < width) {
                    *max_width = width;
                }
            }
        } else {
            const uint8_t climate_idx =
                    static_cast<uint8_t>(colony != nullptr ? colony->climate : planet->climate);

            sprintf(title_buffer, s__s_005569e8, HACCESS::Get_Planet_Name_No_Prime_(planet_idx));

            if (colony != nullptr) {
                sprintf(detail_buffer, s___s_00557f0c, MOX::_player[colony_owner].race_name);
                strcat(title_buffer, detail_buffer);

                if (colony->outpost_flag != 0) {
                    strcat(title_buffer, HAROLD::H_Message_(0x161));
                } else {
                    strcat(title_buffer, s_rparen_00557f08);
                }
            }

            if (draw) {
                fonts::Print_(text_x, current_y, title_buffer);
                fonts::Set_Font_Style_(2, &font_colors);
                fonts::Set_Font_Spacing_(1);
            } else {
                const int16_t width = fonts::Get_String_Width_(title_buffer);
                if (*max_width < width) {
                    *max_width = width;
                }
            }

            sprintf(
                    detail_buffer,
                    s__s___s_0055238c,
                    MOX::_planet_size_string[static_cast<int8_t>(planet->size)],
                    MOX::_planet_climate_string[climate_idx]);

            if (draw) {
                current_y = static_cast<int16_t>(current_y + line_advance);
                fonts::Print_(text_x, current_y, detail_buffer);
            } else {
                *total_height = static_cast<int16_t>(*total_height + line_advance);
                const int16_t width = fonts::Get_String_Width_(detail_buffer);
                if (*max_width < width) {
                    *max_width = width;
                }
            }

            const int16_t max_population =
                    COLCALC::Planet_Max_Population_For_Player_(planet, MOX::_PLAYER_NUM);

            if (colony != nullptr) {
                sprintf(
                        detail_buffer,
                        HAROLD::H_Message_(0x181),
                        static_cast<uint32_t>(static_cast<uint8_t>(colony->n_pops)),
                        static_cast<int32_t>(max_population));
            } else {
                sprintf(
                        detail_buffer,
                        HAROLD::H_Message_(0x162),
                        static_cast<int32_t>(max_population));
            }

            if (draw) {
                current_y = static_cast<int16_t>(current_y + line_advance);
                fonts::Print_(text_x, current_y, detail_buffer);
            } else {
                *total_height = static_cast<int16_t>(*total_height + line_advance);
                const int16_t width = fonts::Get_String_Width_(detail_buffer);
                if (*max_width < width) {
                    *max_width = width;
                }
            }

            current_y = static_cast<int16_t>(current_y + line_advance);
            *total_height = static_cast<int16_t>(*total_height + line_advance);

            if (static_cast<int8_t>(planet->gravity_class) == 1) {
                sprintf(
                        detail_buffer,
                        s__s_00551130,
                        MOX::_mineral_class_string[static_cast<int8_t>(planet->mineral_class)]);
            } else {
                sprintf(
                        detail_buffer,
                        s__s___s_0055238c,
                        MOX::_mineral_class_string[static_cast<int8_t>(planet->mineral_class)],
                        MOX::_planet_gravity_string[static_cast<int8_t>(planet->gravity_class)]);
            }

            if (draw) {
                fonts::Print_(text_x, current_y, detail_buffer);
            } else {
                const int16_t width = fonts::Get_String_Width_(detail_buffer);
                if (*max_width < width) {
                    *max_width = width;
                }
            }

            if ((colony != nullptr) && can_show_special_buildings) {
                special_building_data_height = SYS::Print_Special_Building_Data_(
                        colony_idx,
                        text_x,
                        &current_y,
                        max_width,
                        total_height,
                        line_advance,
                        draw);
            }

            const int32_t planet_special = static_cast<int8_t>(planet->planet_special);
            if ((planet_special != 0) && (planet_special != 0xb)) {
                if (special_building_data_height == 0) {
                    current_y = static_cast<int16_t>(current_y + 4);
                    *total_height = static_cast<int16_t>(*total_height + 4);
                }

                current_y = static_cast<int16_t>(current_y + line_advance);
                *total_height = static_cast<int16_t>(*total_height + line_advance);

                if (draw) {
                    fonts::Print_(text_x, current_y, MOX::_planet_special_string[planet_special]);
                } else {
                    const int16_t width =
                            fonts::Get_String_Width_(MOX::_planet_special_string[planet_special]);
                    if (*max_width < width) {
                        *max_width = width;
                    }
                }
            }
        }

        *total_height = static_cast<int16_t>(*total_height + 2);
    }

    void __cdecl Print_Empty_System_Data_(int16_t star_idx) {
        (void)star_idx;
        int16_t local_4_stack;
        int16_t paragraph_top;
        int16_t box_width;
        int16_t paragraph_width;
        int16_t paragraph_height;
        int16_t left_padding;
        int16_t origin_x;
        int16_t origin_y;
        uint16_t saved_window_status;

        if (MOX::_current_screen == 0) {
            left_padding = 0x0D;
            paragraph_top = 0x2F;
            box_width = static_cast<int16_t>(MOX::_moveable_box[0].width - 0x1A);
            paragraph_width = static_cast<int16_t>(box_width - 0x19);
            paragraph_height = static_cast<int16_t>(MOX::_moveable_box[0].height - 0x5B);
            origin_x = static_cast<int16_t>(MOX::_moveable_box[0].content_offset_x + MOX::_moveable_box[0].x);
            origin_y = static_cast<int16_t>(MOX::_moveable_box[0].content_offset_y + MOX::_moveable_box[0].y);
        } else if (MOX::_current_screen == 0x1D) {
            box_width = 0x013E;
            paragraph_top = 0;
            paragraph_width = 0x0125;
            paragraph_height = 0x00B4;
            origin_x = 0x0132;
            origin_y = 0x0011;
            left_padding = 0;
        } else {
            // The binary seeds these layout values from one uninitialized stack slot here.
            // Use a defined fallback while preserving the single-source dataflow shape.
            const int16_t safe_fallback = 0;
            paragraph_width = safe_fallback;
            box_width = safe_fallback;
            paragraph_height = safe_fallback;
            left_padding = safe_fallback;
            paragraph_top = safe_fallback;
            origin_x = safe_fallback;
            origin_y = safe_fallback;
        }

        local_4_stack = static_cast<int16_t>(left_padding + origin_x);
        const int16_t text_y = static_cast<int16_t>(paragraph_top + origin_y);

        MISC::Save_Window_Status_(&saved_window_status);
        graphics::Set_Window_(0x16, 0x16, 0x020F, 0x01A5);
        video::Clip_On_();
        MISC::Change_Font_Style_(3);

        const int16_t max_width =
            BILL::_Get_Paragraph_Max_Width_(paragraph_width, SYS::_star_description_string, 3);
        int32_t delta = (int32_t)(int16_t)box_width - (int32_t)(int16_t)max_width;
        int32_t centered = delta / 2;

        ERIC::Print_Paragraph_Centered_Vertically_(
            (int16_t)(centered + (int32_t)(int16_t)local_4_stack),
            text_y,
            paragraph_width,
            paragraph_height,
            SYS::_star_description_string,
            3);

        MISC::Restore_Window_Status_(saved_window_status);
    }

    void __cdecl Print_Gate_Info_(
            int16_t star_idx,
            uint16_t gate_type,
            int16_t x_base,
            int16_t y_base,
            int16_t x_offset,
            int16_t y_offset,
            int16_t x_extra,
            int16_t y_extra) {
        int16_t box_x = static_cast<int16_t>(
                static_cast<uint16_t>(x_offset) +
                static_cast<uint16_t>(x_base) +
                static_cast<uint16_t>(x_extra));
        int16_t box_y = static_cast<int16_t>(
                static_cast<uint16_t>(y_offset) +
                static_cast<uint16_t>(y_base) +
                static_cast<uint16_t>(y_extra));

        uint8_t player_color =
                MOX::_main_palette_player_colors[static_cast<uint8_t>(MOX::_player[MOX::_PLAYER_NUM].color)];

        s_colors font_colors = *MISC::Font_Colors2_(1, player_color, player_color + 6);
        fonts::Set_Font_Style_(1, &font_colors);

        int16_t gate_label_width;
        char info_text[200];
        info_text[0] = '\0';

        switch (gate_type) {
        case 0:
            strcpy(info_text, HAROLD::H_Message_(0x17C));
            gate_label_width = fonts::Get_String_Width_(info_text);

            if (MOX::_NUM_PLAYERS > 0) {
                for (int16_t player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
                    if (HAROLD::Player_Has_Dimensional_Portal_(&MOX::_player[player_idx], &MOX::_star[star_idx])) {
                        strcat(info_text, MOX::_player[player_idx].name);
                        strcat(info_text, s_comma_space_00556168);
                    }
                }
            }

            info_text[static_cast<int32_t>(strlen(info_text)) - 2] = '\0';
            strcat(info_text, s_rparen_00557f08);
            break;

        case 1:
            strcpy(info_text, HAROLD::H_Message_(0x17A));
            gate_label_width = fonts::Get_String_Width_(info_text);
            break;

        case 2:
            strcpy(info_text, HAROLD::H_Message_(0x179));
            gate_label_width = fonts::Get_String_Width_(info_text);
            break;

        case 3:
            strcpy(info_text, HAROLD::H_Message_(0x17B));
            gate_label_width = fonts::Get_String_Width_(info_text);

            if (MOX::_NUM_PLAYERS > 0) {
                for (int16_t player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
                    if (HAROLD::Player_Has_Artemis_Net_(&MOX::_player[player_idx], &MOX::_star[star_idx])) {
                        strcat(info_text, MOX::_player[player_idx].race_name);
                        strcat(info_text, s_comma_space_00556168);
                    }
                }
            }

            info_text[static_cast<int32_t>(strlen(info_text)) - 2] = '\0';
            strcat(info_text, s_rparen_00557f08);
            break;

        default:
            break;
        }

        int16_t line_width = static_cast<int16_t>(gate_label_width + 2);
        int16_t paragraph_width = FMTPARA::Get_Formatted_Paragraph_Max_Width_(line_width, info_text, 0);
        int16_t paragraph_height = FMTPARA::Get_Formatted_Paragraph_Max_Height_(line_width, info_text);

        fonts::Get_Font_Height_();

        paragraph_width = static_cast<int16_t>(paragraph_width + 0x0C);
        paragraph_height = static_cast<int16_t>(paragraph_height + 5);

        MISC::Draw_Darkened_Box_(box_x, box_y, paragraph_width, paragraph_height, 0x3F);
        BILL::_Print_Paragraph_(
                static_cast<int16_t>(box_x + 7),
                static_cast<int16_t>(box_y + 5),
                line_width,
                info_text,
                0);
    }

    void __cdecl Wrapper_For_Normal_System_Popup_() {
        const int16_t star_idx = MOX::_moveable_box[0].id;
        const int16_t base_x = MOX::_moveable_box[0].x;
        const int16_t base_y = MOX::_moveable_box[0].y;
        const int16_t content_offset_x = MOX::_moveable_box[0].content_offset_x;
        const int16_t content_offset_y = MOX::_moveable_box[0].content_offset_y;
        s_star_data* const star = &MOX::_star[star_idx];
        char popup_text[80];

        const char* const format = HAROLD::H_Message_(0x16A);
        sprintf(popup_text, format, star);

        SYS::Init_System_Display_(star_idx);
        SYS::Draw_System_Display_Popup_(
            star_idx,
            1,
            popup_text,
            base_x,
            base_y,
            content_offset_x,
            content_offset_y,
            0x0D,
            0x2F
        );
    }

    void __cdecl Wrapper_For_Informational_System_Fields_(
        int16_t param_1,
        int16_t param_2,
        int16_t param_3,
        int16_t param_4,
        int16_t param_5,
        int16_t* param_6) {
        int16_t local_4;
        int16_t local_2;

        SYS::Init_System_Display_(param_1);
        SYS::Add_System_Display_Popup_Fields_(
            param_1,
            param_2,
            param_3,
            0,
            0,
            param_4,
            param_5,
            &local_4,
            param_6,
            &local_2);
    }

    void __cdecl Add_System_Display_Popup_Fields_(
        int16_t star_idx,
        int16_t base_x,
        int16_t base_y,
        int16_t popup_offset_x,
        int16_t popup_offset_y,
        int16_t orbit_offset_x,
        int16_t orbit_offset_y,
        int16_t* out_popup_hidden_field,
        int16_t* out_popup_title_button,
        int16_t* out_system_field) {
        int16_t gate_icon_x = 0;
        int16_t gate_icon_y = 0;
        int32_t orbit_display_x = 0;
        int32_t orbit_display_y = 0;
        int32_t orbit_display_frame = 0;

        if (star_idx <= -1) {
            return;
        }

        if (star_idx >= static_cast<int16_t>(MOX::_NUM_STARS)) {
            return;
        }

        if (SYS::_skip_draw_system_display != 0) {
            return;
        }

        SYS::Build_System_Popup_Ships_(star_idx);

        if (out_popup_title_button != nullptr) {
            const int16_t popup_left = static_cast<int16_t>(base_x + popup_offset_x);
            const int16_t popup_top = static_cast<int16_t>(base_y + popup_offset_y);

            *out_popup_title_button = fields::Add_Clipped_Button_Field_(
                static_cast<int16_t>(popup_left + 0x107),
                static_cast<int16_t>(popup_top + 0xEC),
                s__00551100,
                MOX::_moveable_box_pict[4],
                SYS::s__00551120,
                0x28);

            *out_popup_hidden_field = fields::Add_Hidden_Field_(
                popup_left,
                popup_top,
                static_cast<int16_t>(popup_left + MOX::_moveable_box[0].width),
                static_cast<int16_t>(popup_top + 0x2F),
                s__00551100,
                0);
        } else {
            *out_popup_hidden_field = -1000;
        }

        for (int16_t orbit_slot = 4; orbit_slot >= 0; --orbit_slot) {
            const int16_t planet_idx = MOX::_sys_disp[orbit_slot].orbit;

            if (planet_idx <= -1) {
                MOX::_planet_buttons[orbit_slot] = -1000;
                continue;
            }

            const s_planet_data& planet = MOX::_planet[planet_idx];
            if (planet.planet_type == 1) {
                MOX::_planet_buttons[orbit_slot] = -1000;
                continue;
            }

            int8_t planet_size = planet.size;
            if (planet.planet_type == 2) {
                planet_size = 4;
            }

            GEO::Get_Orbit_Display_X_Y_(
                star_idx,
                static_cast<int32_t>(planet.orbit),
                &orbit_display_x,
                &orbit_display_y,
                &orbit_display_frame);

            const uint8_t planet_diameter = MOX::_rot_plan_dim[planet_size];
            const int16_t half_diameter = static_cast<int16_t>(planet_diameter / 2);

            const int16_t left = static_cast<int16_t>(
                base_x + popup_offset_x + orbit_offset_x + static_cast<int16_t>(orbit_display_x) + 0xA1 -
                half_diameter);
            const int16_t top = static_cast<int16_t>(
                base_y + popup_offset_y + orbit_offset_y + static_cast<int16_t>(orbit_display_y) + 0x5A -
                half_diameter);

            MOX::_planet_buttons[orbit_slot] = fields::Add_Hidden_Field_(
                left,
                top,
                static_cast<int16_t>(left + planet_diameter),
                static_cast<int16_t>(top + planet_diameter),
                s__00551100,
                0x29);
        }

        for (int16_t ship_slot = 0; ship_slot < 15; ++ship_slot) {
            if (MOX::_system_display_ships[ship_slot].ship_idx <= -1) {
                continue;
            }

            const int16_t ship_left = static_cast<int16_t>(
                base_x + popup_offset_x + MOX::_system_display_ships[ship_slot].x);
            const int16_t ship_top = static_cast<int16_t>(
                base_y + popup_offset_y + MOX::_system_display_ships[ship_slot].y);

            MOX::_system_display_ships[ship_slot].pic =
                SYS::Buffer_Reload_System_Ships_(ship_slot);

            const int16_t ship_bottom = static_cast<int16_t>(
                ship_top + animate::Get_Height_(MOX::_system_display_ships[ship_slot].pic));
            const int16_t ship_right = static_cast<int16_t>(
                ship_left + animate::Get_Width_(MOX::_system_display_ships[ship_slot].pic));

            MOX::_system_display_ships[ship_slot].button = fields::Add_Hidden_Field_(
                ship_left,
                ship_top,
                ship_right,
                ship_bottom,
                s__00551100,
                0x29);
        }

        for (int16_t i = 0; i < 4; ++i) {
            MOX::_gate_icon_field[i] = -1000;
        }

        SYS::Get_Gate_Icon_XY_(
            base_x,
            base_y,
            popup_offset_x,
            popup_offset_y,
            orbit_offset_x,
            &gate_icon_x,
            &gate_icon_y);

        if (MOX::_current_screen == 0x1D) {
            gate_icon_y = static_cast<int16_t>(gate_icon_y - 6);
        }

        if (HAROLD::Star_Has_Artemis_Net_(&MOX::_star[star_idx])) {
            gate_icon_x = static_cast<int16_t>(gate_icon_x - 0x0C);
            MOX::_gate_icon_field[3] = fields::Add_Hidden_Field_(
                gate_icon_x,
                gate_icon_y,
                static_cast<int16_t>(gate_icon_x + 0x0F),
                static_cast<int16_t>(gate_icon_y + 0x0F),
                s__00551100,
                0);
        }

        if (HAROLD::Star_Has_Dimensional_Portal_(&MOX::_star[star_idx])) {
            gate_icon_x = static_cast<int16_t>(gate_icon_x - 0x0C);
            MOX::_gate_icon_field[0] = fields::Add_Hidden_Field_(
                gate_icon_x,
                gate_icon_y,
                static_cast<int16_t>(gate_icon_x + 0x0F),
                static_cast<int16_t>(gate_icon_y + 0x0F),
                s__00551100,
                0);
        }

        if (MOX::_current_screen == 0x1D) {
            *out_system_field = fields::Add_Hidden_Field_(
                static_cast<int16_t>(base_x + popup_offset_x),
                static_cast<int16_t>(base_y + popup_offset_y),
                static_cast<int16_t>(base_x + popup_offset_x + 0x13E),
                static_cast<int16_t>(base_y + popup_offset_y + 0xB4),
                s__00551100,
                0);
            return;
        }

        MOX::_system_display_grid_field = fields::Add_Grid_Field_(
            static_cast<int16_t>(base_x + popup_offset_x + 0x0D),
            static_cast<int16_t>(base_y + popup_offset_y + 0x2F),
            1,
            1,
            0x140,
            0xB7,
            &MOX::_system_display_grid_x[0],
            &MOX::_system_display_grid_y[0],
            0);

        *out_system_field = fields::Add_Hidden_Field_(
            static_cast<int16_t>(base_x + popup_offset_x),
            static_cast<int16_t>(base_y + popup_offset_y),
            static_cast<int16_t>(base_x + popup_offset_x + MOX::_moveable_box[0].width),
            static_cast<int16_t>(base_y + popup_offset_y + MOX::_moveable_box[0].height),
            s__00551100,
            0);
    }

    void __cdecl Remap_Draw_Animated_Sprite_Mask_(
        int16_t x,
        int16_t y,
        const uint8_t* sprite_data,
        const int8_t* mask_data) {
      const int16_t* const sprite_words =
          reinterpret_cast<const int16_t*>(sprite_data);

      const int16_t sprite_width = animate::_animation_header->width;
      const int32_t sprite_height = animate::_animation_header->height;

      int32_t current_row = static_cast<uint16_t>(sprite_words[1]);
      int32_t word_index = 2;
      int32_t dest_offset =
          (static_cast<int32_t>(y) + current_row) * static_cast<int32_t>(video::g_screen_res_x) +
          static_cast<int32_t>(x);
      int32_t row_start_dest_offset = dest_offset;

      while (current_row < sprite_height) {
        const int16_t run_length = sprite_words[word_index];
        const int16_t delta = sprite_words[word_index + 1];
        word_index += 2;

        if (run_length == 0) {
          current_row += static_cast<int32_t>(delta);
          row_start_dest_offset += static_cast<int32_t>(delta) * 640;
          dest_offset = row_start_dest_offset;
        } else {
          dest_offset += static_cast<int32_t>(delta);

          int32_t sprite_byte_index = word_index * 2;
          int32_t pixel_index = 0;
          const int32_t run_end = dest_offset + static_cast<int32_t>(run_length);

          if (run_end > dest_offset) {
            const int32_t mask_offset =
                static_cast<int32_t>(sprite_width) * current_row + static_cast<int32_t>(delta);

            do {
              const int8_t mask_value = mask_data[mask_offset + pixel_index];

              uint8_t color;
              if (mask_value != 0) {
                const uint8_t remapped_color =
                    remap::_picture_remap_color_list[sprite_data[sprite_byte_index]];
                color =
                    SYS::_remap_gray_mask_palette[mask_value][remapped_color];
              } else {
                color = remap::_picture_remap_color_list[sprite_data[sprite_byte_index]];
              }

              video::_current_video_page[dest_offset] = color;

              ++sprite_byte_index;
              ++dest_offset;
              ++pixel_index;
            } while (run_end > dest_offset);
          }

          word_index += (run_length + 1) / 2;
        }
      }
    }

    void __cdecl Remap_Clipped_Draw_Animated_Sprite_Mask_(
        int16_t x,
        int16_t y,
        const uint8_t* encoded_sprite,
        const int8_t* mask_bitmap) {
        const uint16_t* const sprite_words =
            reinterpret_cast<const uint16_t*>(encoded_sprite);
        const int32_t sprite_width = animate::_animation_header->width;
        const int32_t sprite_height = animate::_animation_header->height;

        int16_t draw_x;
        int32_t left_clip;
        if (x < video::_screen_window_x1) {
            left_clip = static_cast<int32_t>(video::_screen_window_x1) - static_cast<int32_t>(x);
            draw_x = video::_screen_window_x1;
        } else {
            left_clip = 0;
            draw_x = x;
        }

        int16_t draw_y;
        int32_t top_clip;
        if (video::_screen_window_y1 > y) {
            top_clip = static_cast<int32_t>(video::_screen_window_y1) - static_cast<int32_t>(y);
            draw_y = video::_screen_window_y1;
        } else {
            top_clip = 0;
            draw_y = y;
        }

        int16_t right = static_cast<int16_t>(static_cast<int32_t>(x) + sprite_width - 1);
        if (video::_screen_window_x2 <= right) {
            right = video::_screen_window_x2;
        }

        int16_t bottom = static_cast<int16_t>(static_cast<int32_t>(y) + sprite_height - 1);
        if (video::_screen_window_y2 <= bottom) {
            bottom = video::_screen_window_y2;
        }

        int32_t word_index = 2;
        int32_t current_row = static_cast<uint16_t>(sprite_words[1]);

        int32_t current_dest_row =
            static_cast<int32_t>(draw_y) * static_cast<int32_t>(video::g_screen_res_x) +
            static_cast<int32_t>(x);

        if (current_row < top_clip) {
            do {
                const int16_t run_length = static_cast<int16_t>(sprite_words[word_index]);
                if (run_length == 0) {
                    current_row += static_cast<uint16_t>(sprite_words[word_index + 1]);
                } else {
                    word_index += (static_cast<int32_t>(run_length) + 1) / 2;
                }
                word_index += 2;
            } while (current_row < top_clip);
        }

        if (current_row > top_clip) {
            current_dest_row +=
                (current_row - top_clip) * static_cast<int32_t>(video::g_screen_res_x);
        }

        int32_t row_index = current_row - top_clip;
        int32_t row_clip_start = current_dest_row + left_clip;
        const int32_t visible_width =
            static_cast<int32_t>(right) - static_cast<int32_t>(draw_x) + 1;
        int32_t row_clip_end = row_clip_start + visible_width;
        const int32_t visible_height =
            static_cast<int32_t>(bottom) - static_cast<int32_t>(draw_y) + 1;

        while (row_index < visible_height) {
            const int16_t run_length = static_cast<int16_t>(sprite_words[word_index]);
            const int16_t run_x = static_cast<int16_t>(sprite_words[word_index + 1]);
            word_index += 2;

            if (run_length == 0) {
                row_index += static_cast<int32_t>(run_x);
                current_dest_row += static_cast<int32_t>(run_x) * 640;
                row_clip_start = current_dest_row + left_clip;
                row_clip_end = row_clip_start + visible_width;
                continue;
            }

            int32_t dest_x = current_dest_row + static_cast<int32_t>(run_x);
            int32_t pixel_byte_index = word_index * 2;
            const int32_t run_end = dest_x + static_cast<int32_t>(run_length);
            int32_t skipped_pixels = 0;

            if (dest_x < row_clip_end) {
                if (dest_x < row_clip_start) {
                    int32_t clip_count = row_clip_start - dest_x;
                    if (clip_count > static_cast<int32_t>(run_length)) {
                        clip_count = static_cast<int32_t>(run_length);
                    }
                    skipped_pixels = clip_count;
                    pixel_byte_index += clip_count;
                    dest_x += clip_count;
                }

                const int32_t clipped_run_end =
                    (row_clip_end < run_end) ? row_clip_end : run_end;

                if (clipped_run_end > dest_x) {
                    int32_t mask_index =
                        sprite_width * row_index + static_cast<int32_t>(run_x) + skipped_pixels;

                    while (dest_x < clipped_run_end) {
                        const int8_t mask_value = mask_bitmap[mask_index];
                        const uint8_t remapped_color =
                            remap::_picture_remap_color_list[encoded_sprite[pixel_byte_index]];

                        uint8_t out_color;
                        if (mask_value != 0) {
                            out_color = SYS::_remap_gray_mask_palette[mask_value][remapped_color];
                        } else {
                            out_color = remapped_color;
                        }

                        video::_current_video_page[dest_x] = out_color;
                        ++pixel_byte_index;
                        ++dest_x;
                        ++skipped_pixels;
                        ++mask_index;
                    }
                }
            }

            word_index += (static_cast<int32_t>(run_length) + 1) / 2;
        }
    }

    void __cdecl Get_System_Display_Ship_Icon_XY_(
        int32_t unused,
        int16_t display_slot,
        int16_t* x,
        int16_t* y,
        int16_t* layout_state
    ) {
        (void)unused;

        MOX::_moveable_box_pict[0] =
            (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x49, MOX::_global_cache_seg);
        (void)animate::Get_Width_(MOX::_moveable_box_pict[0]);
        (void)animate::Get_Height_(MOX::_moveable_box_pict[0]);

        s_animation_header** display_ship_pic = &MOX::_system_display_ships[display_slot].pic;

        if (MOX::_current_screen == 0x1d) {
            *display_ship_pic = SYS::Buffer_Reload_System_Ships_(display_slot);
            *x = static_cast<int16_t>(*layout_state + 5);
            *y = static_cast<int16_t>(0xb1 - animate::Get_Height_(*display_ship_pic));
            *layout_state = static_cast<int16_t>(*layout_state + animate::Get_Width_(*display_ship_pic) + 3);
            return;
        }

        *display_ship_pic = SYS::Buffer_Reload_System_Ships_(display_slot);
        *x = static_cast<int16_t>(*layout_state + 0x12);
        *y = static_cast<int16_t>(((0x19 - animate::Get_Height_(*display_ship_pic)) / 2) + 0xec);
        *layout_state = static_cast<int16_t>(*layout_state + animate::Get_Width_(*display_ship_pic) + 3);
    }

    void __cdecl Build_System_Popup_Ships_(int16_t system_idx) {
        int16_t displayed_ship_count = 0;
        int16_t layout_state = 0;

        SYS::Clear_System_Display_Ship_Fields_();
        MOX::_player_has_ship_in_system = 0;

        if (MOX::_ship_icon_count > 0) {
            for (int16_t ship_icon_index = 0; ship_icon_index < MOX::_ship_icon_count; ++ship_icon_index) {
                const int16_t node_idx = MOX::_ship_icon[ship_icon_index].node_idx;
                const int16_t ship_idx = MOX::_ship_node[node_idx].ship_idx;
                s_ship_data* const ship = &MOX::_ship[ship_idx];

                if (ship->location == system_idx && ship->status < 3) {
                    const int16_t owner_idx = ship->owner;
                    MOX::_system_display_ships[owner_idx].ship_idx = node_idx;

                    if (owner_idx == MOX::_PLAYER_NUM) {
                        MOX::_player_has_ship_in_system = 1;
                    }

                    const int16_t display_index = displayed_ship_count;
                    ++displayed_ship_count;

                    SYS::Get_System_Display_Ship_Icon_XY_(
                        display_index,
                        owner_idx,
                        &MOX::_system_display_ships[owner_idx].x,
                        &MOX::_system_display_ships[owner_idx].y,
                        &layout_state);
                }
            }
        }
    }
}