#include "deps.h"
#include <cstdlib>

namespace SYS {
    // Suggested name: Need_To_Reload_System_Display
    bool __cdecl Need_To_Reload_System_Display_(int16_t star_index) {
        char buffer[120];

        _sprintf(buffer, s_fmt_00557F18, (int)star_index, &MOX::_star[star_index]);

        size_t len = strlen(buffer);
        if (len >= 20) {
            buffer[20] = '\0';
        }

        if (MOX::_star_name_loaded_string != nullptr) {
            // Re-calculate length after potential truncation for the string compare
            size_t current_len = strlen(buffer);
            if (_strncmp(buffer, MOX::_star_name_loaded_string, current_len) == 0) {
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

    void __cdecl Set_System_Display_Loaded_Flag_(short star_idx) {
        char temp_buffer[120];

        char* allocated = (char*)allocate::Allocate_First_Block_((void*)SYS::_sys_data_buffer, 20);
        MOX::_star_name_loaded_string = allocated;

        memset(allocated, 0, 20);

        s_star_data* star = &MOX::_star[star_idx];

        _sprintf(temp_buffer, SYS::s_fmt_star_display, (int)star_idx, star->name);

        if (strlen(temp_buffer) > 19) {
            temp_buffer[19] = '\0';
        }

        _sprintf(MOX::_star_name_loaded_string, temp_buffer);
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
        MOX::Mox_Update_();

        MOX::_sys_disp = (s_sys_disp*)allocate::Allocate_Next_Block_((void*)SYS::_sys_data_buffer, 0x46);

        s_animation_header* background_pic = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, 0x49, (void*)MOX::_global_cache_seg);
        MOX::_moveable_box[0].width = animate::Get_Width_(background_pic);
        MOX::_moveable_box[0].height = animate::Get_Height_(background_pic);

        s_star_data* star = &MOX::_star[star_idx];
        int8_t spectral_class = star->spectral_class;

        s_animation_header* sun_anim = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, spectral_class + 0x53, (void*)MOX::_global_cache_seg);
        MOX::_sun_seg[spectral_class] = sun_anim;
        _system_display_sun_seg = sun_anim;

        MOX::Mox_Update_();
        _draw_sun_before_planet = 1;

        for (int16_t orbit_idx = 0; orbit_idx < 5; ++orbit_idx) {
            int16_t planet_idx = star->planet_index[orbit_idx];
            if (planet_idx < 0) {
                MOX::_sys_disp[orbit_idx].orbit = -1;
                MOX::_sys_disp[orbit_idx].x = 999;
                MOX::_sys_disp[orbit_idx].y = 999;
            } else {
                s_planet_data* planet = &MOX::_planet[planet_idx];
                int16_t planet_size = planet->size;
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

        MOX::Mox_Update_();

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
        MOX::Mox_Update_();

        _qsort(MOX::_sys_disp, 5, 0xe, qsort_sys_disp_);

        if (MOX::_look_at_colonies == 0) {
            if (!Ok_To_View_System_(star_idx, MOX::_PLAYER_NUM)) {
                MISC::Get_LBX_Message_(spectral_class + 0xf, &DAT_00566440);
            }
        }
    }

    void __cdecl Init_System_Display_(int16_t star_id) {
        if (SYS::Need_To_Reload_System_Display_(star_id)) {
            MOX::_system_star_id = star_id;
            SYS::Set_System_Display_Loaded_Flag_(star_id);
            SYS::_skip_draw_system_planets = 1;

            if (MOX::_moveable_box[0].off_flag < MOX::_box_off_flag[0]) {
                SYS::_skip_draw_system_display = 1;
            }

            SYS::Load_New_System_Display_(star_id);
            MOX::_system_icon_scanned = -1;

            if (HAROLD::Player_Sighted_Monster_(MOX::_PLAYER_NUM, star_id)) {
                MAINPUPS::Clear_System_Special_Slots_();
            }
        }
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

        _sprintf(fmt_text, s_m25_30_0s, HAROLD::H_Message_(HESTR_170_MAX_POPULATION));
        strcat(fmt_text, s_1);
        strcat(fmt_text, s_d_nl);

        int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(&MOX::_planet[planet_id], MOX::_PLAYER_NUM);
        _sprintf(main_text, fmt_text, (int)max_pop);

        if (info.food == 0) {
            _sprintf(line_text, s_0s_10_nl, HAROLD::H_Message_(HESTR_171_FOOD_PER_FARMER));
        } else {
            int food_val = (int)info.food / 2;
            _sprintf(line_text, s_0s_1d_nl, HAROLD::H_Message_(HESTR_171_FOOD_PER_FARMER), food_val);
        }
        strcat(main_text, line_text);

        _sprintf(line_text, s_0s_1d_nl, HAROLD::H_Message_(HESTR_172_INDUSTRY_PER_WORKER), (int)info.industry);
        strcat(main_text, line_text);

        _sprintf(line_text, s_0s_1d_nl, HAROLD::H_Message_(HESTR_173_RESEARCH_PER_SCIENTIST), (int)info.research);
        strcat(main_text, line_text);

        if (info.maintenance == 0) {
            strcat(main_text, s_m25_27);
            _sprintf(line_text, s_0s_1_dash, HAROLD::H_Message_(HESTR_174_NO_MAINTENANCE_BONUS));
            strcat(main_text, line_text);
        } else {
            strcat(main_text, s_m25_17);
            _sprintf(line_text, s_0s, HAROLD::H_Message_(HESTR_175_MAINTENANCE));

            int16_t msg_id = (info.maintenance <= 0) ? HESTR_189_MAINTENANCE_BONUS : HESTR_18A_MAINTENANCE_PENALTY;
            _sprintf(line_text, s_0s_1, HAROLD::H_Message_(msg_id));

            int abs_maint = abs((int)info.maintenance);
            _sprintf(val_text, s_percent_d_nl, abs_maint);

            strcat(line_text, val_text);
            strcat(main_text, line_text);
        }

        if (info.worker_bonus == 0) {
            strcat(main_text, s_m25_27);
            _sprintf(line_text, s_0s_1_dash, HAROLD::H_Message_(HESTR_176_NO_WORKER_BONUS));
            strcat(main_text, line_text);
        } else {
            strcat(main_text, s_m25_17);
            _sprintf(line_text, s_0s, HAROLD::H_Message_(HESTR_177_WORKER));

            int16_t msg_id = (info.worker_bonus > 0) ? HESTR_18B_WORKER_BONUS : HESTR_18C_WORKER_PENALTY;
            _sprintf(line_text, s_0s_1, HAROLD::H_Message_(msg_id));

            int abs_bonus = abs((int)info.worker_bonus);
            _sprintf(val_text, s_percent_d_nl, abs_bonus);

            strcat(line_text, val_text);
            strcat(main_text, line_text);
        }

        strcpy(fmt_text, HAROLD::H_Message_(HESTR_178_COLONY_INFO_FOR_S));
        char* planet_name = HACCESS::Get_Planet_Name_No_Prime_(planet_id);
        _sprintf(title_text, fmt_text, planet_name);

        TEXTBOX::Text_Box_(title_text, main_text);
    }
}