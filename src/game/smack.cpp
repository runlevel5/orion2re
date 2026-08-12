#include "pch.h"

namespace SMACK {
    s_mouse_list _smack_mouse = {0, 0, 0, 0, 639, 479};
    MOX2::action_func_t _smack_auto_function;
    int16_t _smack_x;
    int16_t _smack_y;

    void __cdecl Smack_Fade_(int16_t fade_mode) {
        int16_t fade_step;
        int16_t palette_level;
        int16_t fade_percent;

        if (fade_mode == 3) {
            fade_step = -10;
            fade_percent = ERIC::Get_Fade_Percent_();
            palette_level = 100;
            if (fade_percent == 0) {
                return;
            }
        } else {
            palette_level = 0;
            fade_percent = ERIC::Get_Fade_Percent_();
            fade_step = 10;
            if (fade_percent == 100) {
                return;
            }
        }

        do {
            if (palette_level >= 0 && palette_level < 100) {
                palstore::Darken_Palette_((int)palette_level);
                video::Submit_Palette_();
                RUSS::Mox_Update_();
            }
            palette_level = (int16_t)(palette_level + fade_step);
        } while (palette_level != 110 && palette_level != -10);

        palette::Clear_Palette_Changes_(0, 0xff);
    }

    void __cdecl Play_Cinematic_(const char* smacker_name, uint32_t play_flags, char use_fade)
    {
        int16_t exit_requested = 0;
        int32_t animation_width;
        int32_t animation_height;
        int16_t total_frames;
        int16_t current_frame;

        buffer::Free_Buffer_Space_();
        video::Set_Page_Off_();
        sound::Fade_Stream_Down_(0x19);

        if (use_fade == 1) {
            SMACK::Smack_Fade_(4);
            mouse::Set_Mouse_List_(&SMACK::_smack_mouse, 1);
        }
        else {
            MOX2::Get_Auto_Function_(&SMACK::_smack_auto_function);
            fields::Deactivate_Auto_Function_();
            video::Set_Refresh_Stencil_();
            fields::Enable_Draw_All_Fields_();
            fields::Draw_Visible_Fields_();
            video::Copy_Off_To_Back_();
        }

        video::Set_Refresh_Differential_();
        fields::Assign_Auto_Function_(smckplay::Draw_Smacker_Animation_, 0);
        fields::Clear_Fields_();
        fields::Deactivate_Help_List_();
        graphics::Set_Window_(0, 0, 0x27f, 0x1df);
        video::Clip_On_();
        sound::Stop_Stream_();

        const char audio_enabled =
            (MOX::_settings.music_on != 0 || MOX::_settings.sound_fx_on != 0) ? 1 : 0;
        smckplay::Open_Smacker_Animation_(smacker_name, audio_enabled, play_flags);

        animation_width = smckplay::Get_Smacker_Animation_Width_();
        SMACK::_smack_x = (int16_t)((0x280 - animation_width) / 2);

        animation_height = smckplay::Get_Smacker_Animation_Height_();
        if ((play_flags & 0x100000U) == 0) {
            SMACK::_smack_y = (int16_t)((0x1e0 - animation_height) / 2);
        }
        else {
            SMACK::_smack_y = (int16_t)((0x1e0 - animation_height) / 4);
        }

        if (use_fade == 1) {
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 0x27f, 0x1df, 0);
            platform::Toggle_Pages_Stub_();
            graphics::Fill_(0, 0, 0x27f, 0x1df, 0);
            smckplay::Draw_Smacker_Animation_();
            palette::Refresh_Palette_();
            palstore::Darken_Palette_(100);
            platform::Toggle_Pages_Stub_();
            ERIC::Set_Fade_Percent_(100);
            SMACK::Smack_Fade_(3);
        }

        total_frames = smckplay::Get_Smacker_Animation_Total_Frames_();

        do {
            if (key::Keyboard_Status_() == 1) {
                exit_requested = 1;
                key::Read_Key_();
            }

            if (mouse::Mouse_Button_() != 0) {
                exit_requested = 1;
            }

            video::Set_Page_Off_();
            smckplay::Draw_Smacker_Animation_();
            platform::Toggle_Pages_Stub_();
            RUSS::Mox_Update_();

            current_frame = smckplay::Get_Smacker_Animation_Frame_();
            if (total_frames - current_frame == 1) {
                exit_requested = 1;
            }
        } while (exit_requested == 0);

        fields::Deactivate_Auto_Function_();

        if (use_fade == 1) {
            SMACK::Smack_Fade_(4);
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 0x27f, 0x1df, 0);
            platform::Toggle_Pages_Stub_();
            ERIC::Set_Fade_Percent_(0);
            ERIC::Set_Fade_Speed_Percent_(10);
            ERIC::Fast_Fade_Out_();
        }

        smckplay::Close_Smacker_Animation_();
        MOX::_global_cache_seg = (void*)buffer::Allocate_Buffer_Space_(MOX::_global_cache_size);
        RUSS::Mox_Update_();
        JIM::_jim_current_stream = (uint16_t)-1;
        JIM::Play_Background_Music_();

        if (use_fade == 0) {
            MOX::_FULL_DRAW_SCREEN_FLAG = 1;
            MOX2::Put_Auto_Function_(&SMACK::_smack_auto_function);
        }

        video::Set_Refresh_Stencil_();
    }
}