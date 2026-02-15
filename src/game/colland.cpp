#include "deps.h"

namespace COLLAND {
    /**
     * @address 00429d40
     */
    void __cdecl Load_Anims_() {
        COLLAND::_l_anims = (uint32_t)(uintptr_t)farload::Far_Reload_Next_(COLONY::_COLONY_LBX, 6, MOX::_screen_seg);
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
     * @address 00429a90
     */
    void __cdecl Colony_Landing_Screen_() {
        void* saved_auto_fn;
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
        JIM::Fast_Fade_In_();
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
            animate::Get_Animation_Frame_Count_((s_animation_header*)(uintptr_t)_l_anims);
            animate::Get_Current_Frame_((s_animation_header*)(uintptr_t)_l_anims);
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