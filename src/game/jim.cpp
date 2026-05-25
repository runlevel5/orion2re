#include "pch.h"

namespace JIM {

    bool __cdecl Check_For_Simtex_Logo_() {
        char find_buffer[80];
        char secondary_path[80];

        sprintf(secondary_path, s_fmt_ssimtex_lbx, farload::_secondary_drive_path);

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
        mouse::Set_Mouse_List_(&_logo_mouse_list, 1);
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
            mouse::Set_Mouse_List_(&_logo_mouse_list, 1);
            palette::Clear_Palette_Changes_(0, 0xFF);
            platform::Toggle_Pages_Stub_();

            int16_t total_frames = file_ani::Get_File_Animation_Total_Frames_();
            file_ani::Draw_File_Animation_();
            video::Copy_Off_To_Back_();
            RUSS::Mox_Update_();
            platform::Toggle_Pages_Stub_();
            _darken_palette_value = 0;

            // Slow fade in for the first logo
            skip_flag = palstore::Slow_Fade_In_();

            int16_t end_delay_counter = 15; // 0xF
            mouse::Set_Mouse_List_(&_logo_mouse_list, 1);

            while (!user_interrupted) {
                if (skip_flag != 0) break;

                timer::Mark_Time_();
                if (key::Keyboard_Status_() == 1) {
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
                RUSS::Mox_Update_();
                timer::Release_Time_(1);
            }
        } else {
            // Play Simtex cinematic if Simtex logo is present
            SMACK::Play_Cinematic_(s_simtex_lbx, 0, 0);
            sound::Stop_Stream_();
            _jim_current_stream = -1;
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
            RUSS::Mox_Update_();

            int16_t end_delay_counter = 5;
            platform::Toggle_Pages_Stub_();

            user_interrupted = false;
            ERIC::Fast_Fade_In_();
            mouse::Set_Mouse_List_(&_logo_mouse_list, 1);

            while (!user_interrupted) {
                timer::Mark_Time_();
                if (key::Keyboard_Status_() == 1) {
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
                RUSS::Mox_Update_();
                timer::Release_Time_(1);
            }
        }

        allocate::Pop_Block_(MOX::_global_screen_seg);
        file_ani::Reset_File_Animation_Buffer_();
        Logo_Fadeout_();
        RUSS::Mox_Update_();

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
            RUSS::Mox_Update_();
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

        _jim_old_music_id = -1;

        // If one of the background tracks (1, 2, or 3) is already playing, do nothing
        if (_jim_current_stream == 1 || _jim_current_stream == 2 || _jim_current_stream == 3) {
            return;
        }

        // Fade out existing audio
        sound::Fade_Stream_Down_(25);

        // Wait for volume to reach zero while updating engine state
        if (sound::Get_Stream_Volume_() > 0) {
            int16_t current_volume;
            do {
                current_volume = (int16_t)sound::Get_Stream_Volume_();
                RUSS::Mox_Update_();
            } while (current_volume > 0);
        }

        sound::Stop_Stream_();

        // Randomly select next track from "STREAM.LBX" (indices 1, 2, or 3)
        _jim_current_stream = (int16_t)((clock() % 3) + 1);

        sound::Set_Stream_Filename_(s_STREAM_LBX);
        sound::Play_Stream_(_jim_current_stream);
        sound::Loop_Stream_(-1);

        _jim_old_music_id = -1;
        sound::Set_Stream_Volume_(0);
        sound::Fade_Stream_Up_(30);
    }

    void __cdecl Decode_Text_Stat_(char* buffer, int16_t stat_a, int16_t stat_b) {
        Decode_Text_Message_(buffer, -1, -1, -1, stat_a, -1, -1, -1, stat_b);
    }

    void __cdecl Decode_Text_Year_(char* buffer, int16_t year) {
        JIM::Decode_Text_Message_(buffer, -1, -1, -1, -1, -1, -1, -1, year);
    }

    void __cdecl Decode_Text_Race2_(char* buffer, int16_t race_idx1, int16_t race_idx2) {
        JIM::Decode_Text_Message_(buffer, race_idx1, -1, -1, -1, -1, -1, -1, race_idx2);
    }

    void __cdecl Decode_Text_Race_(char* out_buffer, int16_t msg_idx, int16_t race_idx) {
        JIM::Decode_Text_Message_(out_buffer, msg_idx, -1, -1, -1, -1, -1, -1, race_idx);
    }

    void __cdecl Decode_Text_Name_And_Stat_(char* text, int16_t val1, int16_t val2, int16_t val3) {
        JIM::Decode_Text_Message_(text, val1, -1, -1, val2, -1, -1, -1, val3);
    }

    void __cdecl Decode_Text_Name_Tech_And_Other_Tech_(char* text, int16_t val1, int16_t val2, int16_t val3, int16_t val4, int16_t val5) {
        JIM::Decode_Text_Message_(text, val1, -1, val3, -1, val2, val4, -1, val5);
    }

    void __cdecl Decode_Text_Name_And_System_(char* buffer, int16_t name_idx, int16_t system_idx, int16_t unknown_idx) {
        JIM::Decode_Text_Message_(buffer, name_idx, system_idx, -1, -1, -1, -1, -1, unknown_idx);
    }

    void __cdecl Decode_Text_System_(char* buffer, int16_t message_id, int16_t system_id) {
        JIM::Decode_Text_Message_(buffer, -1, message_id, -1, -1, -1, -1, -1, system_id);
    }

    void __cdecl Decode_Text_Name_And_Other_Tech_(char* buffer, int16_t id1, int16_t id2, int16_t id3, int16_t id4) {
        JIM::Decode_Text_Message_(buffer, id1, -1, -1, -1, id2, id3, -1, id4);
    }

    void __cdecl Decode_Text_Name_And_Tech_(char* text, int16_t id1, int16_t id2, int16_t id3) {
        JIM::Decode_Text_Message_(text, id1, -1, id2, -1, -1, -1, -1, id3);
    }

    void __cdecl Decode_Text_Name_And_Name_(char* buffer, int16_t id1, int16_t id2, int16_t id3) {
        Decode_Text_Message_(buffer, id1, -1, -1, -1, id2, -1, -1, id3);
    }

    void __cdecl Decode_Text_Tech_And_Other_Name_(char* text, int16_t tech_id, int16_t other_name_1, int16_t other_name_2, int16_t other_name_3) {
        JIM::Decode_Text_Message_(text, tech_id, -1, other_name_1, -1, other_name_2, -1, -1, other_name_3);
    }

    void __cdecl Decode_Text_Field_(char* text, int16_t field_id, int16_t max_len) {
        Decode_Text_Message_(text, -1, -1, -1, -1, -1, -1, field_id, max_len);
    }

    void __cdecl Decode_Text_Message_(char* text, int16_t player1_id, int16_t star_id, int16_t tech_app1_id, int16_t number, int16_t player2_id, int16_t tech_app2_id, int16_t tech_field_id, int16_t max_len) {
        int16_t len = 0;
        char temp_buf[40];
        char dest_buf[200];

        for (int i = 0; i < 200; ++i) {
            char c = text[i];
            if (c == '\0') {
                break;
            }

            char* src_ptr = nullptr;
            if (static_cast<uint8_t>(c) >= 0x80 && static_cast<uint8_t>(c) <= 0x8A) {
                dest_buf[len] = '\0';

                switch (static_cast<uint8_t>(c)) {
                    case 0x80:
                        src_ptr = MOX::_player[player1_id].name;
                        break;
                    case 0x81:
                        src_ptr = MOX::_star[star_id].name;
                        break;
                    case 0x82:
                        itoa(number, temp_buf, 10);
                        src_ptr = temp_buf;
                        break;
                    case 0x83:
                        strcpy(temp_buf, TECH::Technology_Applications_Name_(tech_app1_id, player1_id));
                        src_ptr = temp_buf;
                        break;
                    case 0x84:
                        src_ptr = MOX::_player[player2_id].name;
                        break;
                    case 0x85:
                        strcpy(temp_buf, TECH::Technology_Applications_Name_(tech_app2_id, player2_id));
                        src_ptr = temp_buf;
                        break;
                    case 0x86:
                        src_ptr = MOX::_player[player1_id].race_name;
                        break;
                    case 0x87:
                        src_ptr = MOX::_player[player2_id].race_name;
                        break;
                    case 0x88:
                        if (MOX::_settings.language == 0) {
                            sprintf(temp_buf, s_d_dot_d_005523a4, MOX::_stardate / 10, MOX::_stardate % 10);
                        } else {
                            sprintf(temp_buf, s_d_comma_d_0055239c, MOX::_stardate / 10, MOX::_stardate % 10);
                        }
                        src_ptr = temp_buf;
                        break;
                    case 0x89:
                        strcpy(temp_buf, TECH::Technology_Fields_Name_(tech_field_id));
                        src_ptr = temp_buf;
                        break;
                    case 0x8A:
                        strcpy(temp_buf, HACCESS::Get_Planet_Name_(star_id));
                        src_ptr = temp_buf;
                        break;
                }

                strcpy(&dest_buf[len], src_ptr);
                len = static_cast<int16_t>(strlen(dest_buf));
            } else {
                dest_buf[len++] = c;
            }
        }

        dest_buf[len] = '\0';

        if (strlen(dest_buf) >= static_cast<uint32_t>(max_len - 1)) {
            sprintf(dest_buf, s_Decode_message_too_long_0055593c, max_len, text);
            exitm::Exit_With_Message(dest_buf);
        } else {
            strcpy(text, dest_buf);
        }
    }

    void __cdecl Get_Text_Message_(const char* filename, int16_t msg_idx, char* dest, int16_t max_len) {
        char buffer[100];

        uint16_t lang = static_cast<uint8_t>(MOX::_settings.language);
        if (lang > 5) {
            lang = 0;
        }

        RUSS::Mox_Update_();

        farload::Farload_Data_Static_(filename, static_cast<int16_t>(msg_idx * 6 + lang), buffer, 0, 1, 100);

        RUSS::Mox_Update_();

        int16_t len = static_cast<int16_t>(strlen(buffer));

        if (max_len - 1 <= len || len >= 99) {
            sprintf(buffer, s_Message_too_long, msg_idx, filename, len);
            exitm::Exit_With_Message(buffer);
            return;
        }

        strcpy(dest, buffer);
    }

    void __cdecl Bill_Picture_To_Mouse_(s_animation_header* anim) {
        if (animate::Get_Width_(anim) <= 24) {
            animate::Get_Height_(anim);
        }

        bitmap::Draw_Picture_To_Bitmap_(COLONY::_mouse_bitm, anim);
        bitmap::Clear_Mouse_Image_(0);
        bitmap::Draw_Bitmap_To_Mouse_(0, 0, COLONY::_mouse_bitm, 0);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
    }

    void __cdecl Play_Combat_Music_() {
        if (MOX::_settings.music_on == 1) {
            _jim_old_music_id = -1;
            sound::Fade_Stream_Down_(35);

            if (sound::Get_Stream_Volume_() > 0) {
                int16_t current_volume;
                do {
                    current_volume = static_cast<int16_t>(sound::Get_Stream_Volume_());
                    RUSS::Mox_Update_();
                } while (current_volume > 0);
            }

            sound::Stop_Stream_();

            int32_t ticks = clock();
            _jim_current_stream = static_cast<uint16_t>((ticks % 3) + 4);

            sound::Set_Stream_Filename_(s_STREAM_LBX);
            sound::Play_Stream_(_jim_current_stream);
            sound::Loop_Stream_(-1);
            sound::Set_Stream_Volume_(0);
            sound::Fade_Stream_Up_(30);
        }
    }

    void __cdecl Stop_Background_Music_() {
        _jim_old_music_id = -1;
        _jim_current_stream = -1;
        sound::Set_Stream_Volume_(0);
        sound::Stop_Stream_();
    }

    void __cdecl Play_Streaming_Music_(int16_t music_id, int16_t transition_id, int16_t loop_flag) {
        if (MOX::_settings.music_on != 1) {
            return;
        }

        if (music_id != _jim_old_music_id) {
            _jim_old_music_id = music_id;
            _jim_old_transition_id = transition_id;
            _jim_fade_music_flag = 1;
        }

        if (_jim_fade_music_flag == 1) {
            sound::Fade_Stream_Down_(30);
            _jim_fade_music_flag = 2;
            return;
        }

        if (_jim_fade_music_flag == 2) {
            if (sound::Get_Stream_Volume_() != 0) {
                return;
            }

            sound::Stop_Stream_();

            if (_jim_old_music_id <= 100) {
                sound::Set_Stream_Filename_(s_STREAM_LBX_005558fc);
                sound::Play_Stream_(_jim_old_music_id);
            } else {
                sound::Set_Stream_Filename_(s_STREAMHD_LBX_00555908);
                sound::Play_Stream_(_jim_old_music_id - 100);
            }

            _jim_current_stream = _jim_old_music_id;

            if (_jim_old_transition_id == -1 && loop_flag == 1) {
                sound::Loop_Stream_(-1);
            }

            sound::Set_Stream_Volume_(0);
            sound::Fade_Stream_Up_(30);

            if (_jim_old_transition_id == -1) {
                _jim_fade_music_flag = 4;
            } else {
                _jim_fade_music_flag = 3;
            }
        } else if (_jim_fade_music_flag == 3) {
            if (sound::Is_Stream_Playing_() != 0) {
                return;
            }

            sound::Stop_Stream_();

            if (_jim_old_transition_id == -2) {
                int32_t ticks = static_cast<int32_t>(clock());
                _jim_current_stream = static_cast<int16_t>((ticks % 3) + 1);
                sound::Set_Stream_Filename_(s_STREAM_LBX_005558fc);
                sound::Play_Stream_(_jim_current_stream);
            } else {
                if (_jim_old_transition_id <= 100) {
                    sound::Set_Stream_Filename_(s_STREAM_LBX_005558fc);
                    sound::Play_Stream_(_jim_old_transition_id);
                } else {
                    sound::Set_Stream_Filename_(s_STREAMHD_LBX_00555908);
                    sound::Play_Stream_(_jim_old_transition_id - 100);
                }
                _jim_current_stream = _jim_old_transition_id;
            }

            if (loop_flag == 1) {
                sound::Loop_Stream_(-1);
            }

            sound::Set_Stream_Volume_(0);
            sound::Fade_Stream_Up_(30);
            _jim_fade_music_flag = 4;
        }
    }
}
