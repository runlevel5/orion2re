#include "pch.h"

namespace JIM {

    bool __cdecl Check_For_Simtex_Logo_() {
        char find_buffer[80];
        char secondary_path[80];

        _sprintf(secondary_path, s_fmt_ssimtex_lbx, farload::_secondary_drive_path);

        if (dos::DIR_(s_simtex_lbx, find_buffer)) {
            return true;
        }

        if (dos::DIR_(secondary_path, find_buffer)) {
            return true;
        }

        return false;
    }

    void __cdecl Draw_Logos_(void) {
        fields::Deactivate_Auto_Function_();
        mouse::Set_Mouse_List_(_logo_mouse_list, 1);
        _darken_palette_value = 0;
        fields::Clear_Fields_();
        video::Set_Refresh_Stencil_();
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);

        allocate::Push_Block_(MOX::_global_screen_seg);
        int32_t remaining_space = allocate::Get_Remaining_Block_Space_(MOX::_global_screen_seg);
        void* ani_buffer = allocate::Allocate_Next_Block_(MOX::_global_screen_seg, remaining_space - 100);
        file_ani::Set_File_Animation_Buffer_(ani_buffer);

        int16_t skip_flag = 0;
        bool user_interrupted = false;

        if (Check_For_Simtex_Logo_() == 0) {
            file_ani::Open_File_Animation_(s_logo_lbx, 0);
            video::Set_Page_Off_();
            palstore::Darken_Palette_(100);
            mouse::Set_Mouse_List_(_logo_mouse_list, 1);
            palette::Clear_Palette_Changes_(0, 0xFF);
            platform::Toggle_Pages_Stub_();

            int16_t total_frames = file_ani::Get_File_Animation_Total_Frames_();
            file_ani::Draw_File_Animation_();
            video::Copy_Off_To_Back_();
            MOX::Mox_Update_();
            platform::Toggle_Pages_Stub_();
            _darken_palette_value = 0;

            // Slow fade in for the first logo
            palstore::Slow_Fade_In_();

            int16_t end_delay_counter = 15; // 0xF
            mouse::Set_Mouse_List_(_logo_mouse_list, 1);

            while (!user_interrupted) {
                if (skip_flag != 0) break;

                timer::Mark_Time_();
                if (key::Keyboard_Status_()) {
                    key::Read_Key_();
                    skip_flag = 1;
                    user_interrupted = true;
                }

                if (mouse::Mouse_Button_() != 0) {
                    skip_flag = 1;
                    user_interrupted = true;
                }

                if (user_interrupted) break;

                video::Set_Page_Off_();
                if (file_ani::Get_File_Animation_Frame_() == total_frames) {
                    if (--end_delay_counter < 1) {
                        user_interrupted = true;
                    }
                } else {
                    file_ani::Draw_File_Animation_();
                }

                platform::Toggle_Pages_Stub_();
                MOX::Mox_Update_();
                timer::Release_Time_(1);
            }
        } else {
            // Play Simtex cinematic if check failed
            SMACK::Play_Cinematic_(s_simtex_lbx, 0, 0);
            sound::Stop_Stream_();
            _jim_current_stream = 0xFFFF;
        }

        // Second Logo (Master of Orion / Microprose)
        if (skip_flag == 0) {
            Logo_Fadeout_();
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);

            file_ani::Open_File_Animation_(s_logo_lbx, 1);
            int16_t total_frames = file_ani::Get_File_Animation_Total_Frames_();
            file_ani::Draw_File_Animation_();
            video::Copy_Off_To_Back_();
            MOX::Mox_Update_();

            int16_t end_delay_counter = 5;
            platform::Toggle_Pages_Stub_();

            user_interrupted = false;
            ERIC::Fast_Fade_In_();
            mouse::Set_Mouse_List_(_logo_mouse_list, 1);

            while (!user_interrupted) {
                timer::Mark_Time_();
                if (key::Keyboard_Status_()) {
                    key::Read_Key_();
                    skip_flag = 1;
                    user_interrupted = true;
                }

                if (mouse::Mouse_Button_() != 0) {
                    skip_flag = 1;
                    user_interrupted = true;
                }

                if (user_interrupted) break;

                video::Set_Page_Off_();
                if (file_ani::Get_File_Animation_Frame_() == total_frames) {
                    if (--end_delay_counter < 1) {
                        user_interrupted = true;
                    }
                } else {
                    file_ani::Draw_File_Animation_Stencil_();
                }

                platform::Toggle_Pages_Stub_();
                MOX::Mox_Update_();
                timer::Release_Time_(1);
            }
        }

        allocate::Pop_Block_(MOX::_global_screen_seg);
        file_ani::Reset_File_Animation_Buffer_();
        Logo_Fadeout_();
        MOX::Mox_Update_();

        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
        video::Copy_Off_To_Back_();

        // Finally play the intro cinematic unless the user requested a skip early on
        if (skip_flag == 0) {
            SMACK::Play_Cinematic_(s_intro_lbx, 0, 1);
        }
    }

    void __cdecl Logo_Fadeout_() {
        video::Set_Refresh_Full_();

        for (int16_t current_fade = _darken_palette_value; current_fade <= 100; current_fade += 10) {
            palstore::Darken_Palette_(static_cast<int32_t>(current_fade));
            MOX::Mox_Update_();
        }

        video::Set_Page_Back_();
        graphics::Fill_(0, 0, 639, 479, 0);
        video::Copy_Back_To_Off_();
        palette::Clear_Palette_Changes_(0, 255);
        RUSS::Mox_Sync_Update_();
        video::Set_Refresh_Stencil_();
    }

    void Play_Background_Music_() {
        // Only proceed if music is enabled in settings
        if (MOX::_settings.music_on != 1) {
            return;
        }

        _jim_old_music_id = 0xFFFF;

        // If one of the background tracks (1, 2, or 3) is already playing, do nothing
        if (_jim_current_stream == 1 || _jim_current_stream == 2 || _jim_current_stream == 3) {
            return;
        }

        // Fade out existing audio
        sound::Fade_Stream_Down_(25);

        // Wait for volume to reach zero while updating engine state
        while (sound::Get_Stream_Volume_() > 0) {
            MOX::Mox_Update_();
        }

        sound::Stop_Stream_();

        // Randomly select next track from "STREAM.LBX" (indices 1, 2, or 3)
        _jim_current_stream = static_cast<uint16_t>((_clock() % 3) + 1);

        sound::Set_Stream_Filename_(s_STREAM_LBX);
        sound::Play_Stream_(static_cast<int32_t>(_jim_current_stream));
        sound::Loop_Stream_(-1);

        _jim_old_music_id = 0xFFFF;
        sound::Set_Stream_Volume_(0);
        sound::Fade_Stream_Up_(30);
    }
}
