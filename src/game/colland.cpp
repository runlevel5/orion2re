#include "pch.h"

namespace COLLAND {
    /**
     * @address 00429f50
     */
    void __cdecl Print_Planet_Data_(int16_t planet_idx) {
        char buffer[500];
        s_planet_data* planet = &MOX::_planet[planet_idx];

        COLONY::Set_Colony_Font_To_(4);

        if (planet->colony_index == -1) {
            char* planet_name = HACCESS::Get_Planet_Name_(planet_idx);
            char* fmt = ESTRINGS::E_Strings_(0x46 /* ESTR_S1 */);
            sprintf(buffer, fmt, planet_name);
        } else {
            s_colony* colony = &MOX::_colony[planet->colony_index];
            char* race_name = MOX::_player[colony->owner].race_name;
            char* planet_name = HACCESS::Get_Planet_Name_(planet_idx);
            char* fmt = ESTRINGS::E_Strings_(0x28 /* ESTR_S__S_COLONY */);
            sprintf(buffer, fmt, planet_name, race_name);
        }
        fonts::Print_(0x14, 0x28, buffer);

        char* climate_str;
        char* size_str = MOX::_planet_size_string[planet->size];

        if (planet->colony_index == -1) {
            if (planet->planet_type != 3 /* PLANET_TYPE_PLANET */) {
                char* gas_giant_msg = ESTRINGS::E_Strings_(0x132 /* ESTR_GAS_GIANT__UNINHABITABLE */);
                sprintf(buffer, gas_giant_msg);
                fonts::Print_(0x14, 0x3c, buffer);
                return;
            }
            climate_str = MOX::_planet_climate_string[planet->climate];
        } else {
            climate_str = MOX::_planet_climate_string[(uint8_t)MOX::_colony[planet->colony_index].climate];
        }

        const char* fmt_size_climate;
        if (MOX::_settings.language == 4) {
            fmt_size_climate = s_format_comma_space;
        } else {
            fmt_size_climate = ESTRINGS::E_Strings_(0x2d /* ESTR_S_S */);
        }

        sprintf(buffer, fmt_size_climate, size_str, climate_str);
        BILL::_Print_Paragraph_(0x14, 0x3c, 0x26b, buffer, 0);

        int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(planet, MOX::_PLAYER_NUM);
        char* fmt_pop = ESTRINGS::E_Strings_(0x1f);
        sprintf(buffer, fmt_pop, (int)max_pop);
        fonts::Print_(0x14, 0x50, buffer);

        char* mineral_str = MOX::_mineral_class_string[planet->mineral_class];
        char* fmt_minerals = ESTRINGS::E_Strings_(0x176);
        sprintf(buffer, fmt_minerals, mineral_str);
        fonts::Print_(0x14, 100, buffer);

        fonts::Print_(0x14, 0x78, MOX::_planet_gravity_string[planet->gravity_class]);

        if (planet->planet_special != 0) {
            char* special_str = MOX::_planet_special_string[planet->planet_special];
            char* fmt_special = ESTRINGS::E_Strings_(0x46 /* ESTR_S1 */);
            sprintf(buffer, fmt_special, special_str);
            BILL::_Print_Paragraph_(0x14, 0x8c, 0x26b, buffer, 0);
        }
    }

    /**
     * @address 00429d40
     */
    void __cdecl Load_Anims_() {
        COLLAND::_l_anims = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLONY_LBX, 6, MOX::_screen_seg);
    }

    void Add_Fields_() {
        fields::Clear_Fields_();

        int16_t sound = 0;
        char* hotkey = ESTRINGS::E_Strings_(12);

        _fields = (uint16_t)fields::Add_Hidden_Field_(
            0,
            0,
            639,
            479,
            hotkey,
            sound
        );
    }

    /**
     * @address 00429da0
     */
    void __cdecl Draw_Colony_Landing_Screen_() {
        char buffer[500];

        video::Set_Page_Off_();

        if (COLONY::_full_draw != 0) {
            graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);

            s_animation_header* anim1 = COLONY::C_Anims_(1);
            animate::Draw_(0, 0, anim1);

            s_animation_header* anim0 = COLONY::C_Anims_(0);
            animate::Draw_(0, 0, anim0);

            COLONY::Set_Colony_Font_To_(4);

            char* planet_name = HACCESS::Get_Planet_Name_((int16_t)COLONY::_planet_handle);
            char* format = ESTRINGS::E_Strings_(0xE5 /* ESTR_COLONY_BUILT_ON_S */);
            sprintf(buffer, format, planet_name);
            fonts::Print_Centered_(0x13F, 10, buffer);

            if (COLONY::_full_draw != 0) {
                video::Copy_Off_To_Back_();
                video::Set_Refresh_Stencil_();
                COLONY::_full_draw = 0;
            }
        }

        animate::Draw_(0, 0, COLLAND::_l_anims);

        int16_t current_frame = animate::Get_Current_Frame_(COLLAND::_l_anims);
        int16_t frame_count = animate::Get_Animation_Frame_Count_(COLLAND::_l_anims);

        if (((int32_t)frame_count - 2) <= (int32_t)current_frame) {
            COLLAND::Print_Planet_Data_((int16_t)COLONY::_planet_handle);
        }

        if (current_frame == 1) {
            COLLAND::_pod1d_sfx = ERICNET::Mox_Play_Sound_(0x3A);
            return;
        }

        if (current_frame == 2) {
            COLLAND::_pod2d_sfx = ERICNET::Mox_Play_Sound_(0x3A);
            return;
        }

        if (current_frame == 0x39) {
            sound::Fadeout_Sound_(COLLAND::_pod1d_sfx, 2);
            COLLAND::_pod1l_sfx = ERICNET::Mox_Play_Sound_(0x39);
            return;
        }

        if (current_frame == 0x3E) {
            sound::Fadeout_Sound_(COLLAND::_pod2d_sfx, 2);
            COLLAND::_pod2l_sfx = ERICNET::Mox_Play_Sound_(0x39);
        }
    }

    /**
     * @address 00429a90
     */
    void __cdecl Colony_Landing_Screen_() {
        MOX2::action_func_t saved_auto_fn;
        int16_t input_result;

        COLONY::Set_Colony_Font_();
        COLONY::Load_Colony_Screen_Seg_();
        video::Clip_Off_();
        COLONY::_using_colony_screen_palette = 1;

        COLONY::_star_handle = (int32_t)MOX::_screen_data;
        COLONY::_orbit_n = (int32_t)COLONY::_orbit_temp;

        COLONY::_planet_handle = (int32_t)HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
        COLONY::_colony_handle = (int32_t)MOX::_planet[COLONY::_planet_handle].colony_index;
        COLONY::_valid_colony = 1;
        COLONY::_drawing_display = 0;
        COLONY::_sys_disp_star_handle = COLONY::_star_handle;

        Load_Anims_();
        COLONY::Load_Planet_Screen_();

        if (ERIC::Fast_Fade_Out_Needed_For_Colony_Palette_()) {
            ERIC::Fast_Fade_Out_();
            graphics::Fill_(0, 0, 639, 479, 0);
            COLONY::Update_Colony_Palette_();
        }

        if (ERIC::Fast_Fade_Out_Needed_For_Colony_Palette_()) {
            palstore::Darken_Palette_(100);
            palette::Clear_Palette_Changes_(0, 255);
        }

        palette::Refresh_Palette_();
        COLONY::Reset_Field_Arrays_();
        Add_Fields_();
        COLONY::_full_draw = 1;

        MOX2::Get_Auto_Function_(&saved_auto_fn);
        fields::Assign_Auto_Function_(Draw_Colony_Landing_Screen_, 1);
        Draw_Colony_Landing_Screen_();
        ERIC::Fast_Fade_In_();
        video::Set_Refresh_Stencil_();

        if (MOX::_colony[COLONY::_colony_handle].occupation_policy == 4 /* OCC_POLICY_OCCUPATION */) {
            if (COLONY::Sum_Colonists_(COLONY::_colony_handle, -1, -1, -1, -1, 1) != 0) {
                OCPOLPUP::Occupation_Policy_Popup_();
                COLONY::Col_Calc_Wrapper_(COLONY::_colony_handle);
            }
        }

        do {
            timer::Mark_Time_();
            Add_Fields_();
            Draw_Colony_Landing_Screen_();
            animate::Get_Animation_Frame_Count_(COLLAND::_l_anims);
            animate::Get_Current_Frame_(COLLAND::_l_anims);
            RUSS::Mox_Sync_Update_();
            input_result = fields::Get_Input_();
            fields::Scan_Input_();
            timer::Release_Time_(1);
        } while (input_result == 0);

        COLONY::Reset_Field_Mode_();
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();

        sound::Fadeout_Sound_(_pod1d_sfx, 10);
        sound::Fadeout_Sound_(_pod2d_sfx, 10);
        sound::Fadeout_Sound_(_pod1l_sfx, 10);
        sound::Fadeout_Sound_(_pod2l_sfx, 10);

        int16_t planet_idx = MOX::_colony[COLONY::_colony_handle].planet;
        int16_t star_idx = MOX::_planet[planet_idx].star_index;

        if (COLCALC::N_Colonies_And_Outposts_At_Star_(star_idx) == 1 && MOX::_settings.animations_on) {
            int16_t cy = namestar::Star_Name_Popup_Screen_Center_Y_();
            int16_t cx = namestar::Star_Name_Popup_Screen_Center_X_();
            namestar::Change_Star_Name_Popup_(cx, cy, star_idx, 1);
        }

        MOX2::Put_Auto_Function_(&saved_auto_fn);
        COLONY::_using_colony_screen_palette = 0;
        MOX::_current_screen = 1; /* SCR_MAIN_VIEW */
    }
}