#include "pch.h"

namespace JIM {

    int16_t _jim_current_stream = -1;
    int16_t _jim_old_music_id = -1;
    int16_t _jim_fade_music_flag = -1;
    int16_t _jim_old_transition_id = -1;
    const char s_STREAM_LBX[] = "STREAM.LBX";
    const char s_STREAMHD_LBX_00555908[] = "STREAMHD.LBX";
    s_mouse_list _logo_mouse_list = {0, 0, 0, 0, 639, 479};
    int16_t _darken_palette_value;
    const char jimtext_lbx[] = "jimtext.lbx";
    const char s_intro_lbx[] = "intro.lbx";
    const char s_simtex_lbx[] = "simtex.lbx";
    const char s_logo_lbx[] = "logo.LBX";
    const char s_fmt_ssimtex_lbx[] = "%ssimtex.lbx";
    const char s_Message_too_long[] = "Message #%d in %s too long (%d).";
    const char s_d_comma_d_0055239c[] = "%d,%d";
    const char s_d_dot_d_005523a4[] = "%d.%d";
    const char s_Decode_message_too_long_0055593c[] = "Decode message too long (%d).\n%s";

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

        int16_t skip_requested = 0;
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

            skip_requested = palstore::Slow_Fade_In_();

            int16_t end_delay_counter = 15;
            mouse::Set_Mouse_List_(&_logo_mouse_list, 1);

            while (!user_interrupted) {
                if (skip_requested != 0) break;

                timer::Mark_Time_();
                if (key::Keyboard_Status_() == 1) {
                    key::Read_Key_();
                    skip_requested = 1;
                    user_interrupted = true;
                }

                if (mouse::Mouse_Button_() != 0) {
                    skip_requested = 1;
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
        if (skip_requested == 0) {
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
                    skip_requested = 1;
                    user_interrupted = true;
                }

                if (mouse::Mouse_Button_() != 0) {
                    skip_requested = 1;
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

        if (skip_requested == 0) {
            SMACK::Play_Cinematic_(s_intro_lbx, 0, 1);
        }
    }

    void __cdecl Logo_Fadeout_() {
        video::Set_Refresh_Full_();

        for (int16_t current_fade = _darken_palette_value; current_fade <= 100; current_fade += 10) {
            palstore::Darken_Palette_((int32_t)current_fade);
            video::Submit_Palette_();
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
        if (MOX::_settings.music_on != 1) {
            return;
        }

        _jim_old_music_id = -1;

        if (_jim_current_stream == 1 || _jim_current_stream == 2 || _jim_current_stream == 3) {
            return;
        }

        sound::Fade_Stream_Down_(25);

        if (sound::Get_Stream_Volume_() > 0) {
            int16_t current_volume;
            do {
                current_volume = (int16_t)sound::Get_Stream_Volume_();
                RUSS::Mox_Update_();
            } while (current_volume > 0);
        }

        sound::Stop_Stream_();

        _jim_current_stream = (int16_t)((clock() % 3) + 1);

        sound::Set_Stream_Filename_(s_STREAM_LBX);
        sound::Play_Stream_(_jim_current_stream);
        sound::Loop_Stream_(-1);

        _jim_old_music_id = -1;
        sound::Set_Stream_Volume_(0);
        sound::Fade_Stream_Up_(30);
    }

    void __cdecl Decode_Text_Stat_(char* buffer, int16_t numeric_value, int16_t max_len) {
        Decode_Text_Message_(buffer, -1, -1, -1, numeric_value, -1, -1, -1, max_len);
    }

    void __cdecl Decode_Text_Year_(char* buffer, int16_t max_len) {
        JIM::Decode_Text_Message_(buffer, -1, -1, -1, -1, -1, -1, -1, max_len);
    }

    void __cdecl Decode_Text_Race2_(char* buffer, int16_t race_idx, int16_t max_len) {
        JIM::Decode_Text_Message_(buffer, race_idx, -1, -1, -1, -1, -1, -1, max_len);
    }

    void __cdecl Decode_Text_Race_(char* buffer, int16_t race_idx, int16_t max_len) {
        JIM::Decode_Text_Message_(buffer, race_idx, -1, -1, -1, -1, -1, -1, max_len);
    }

    void __cdecl Decode_Text_Name_And_Stat_(char* buffer, int16_t name_idx, int16_t numeric_value, int16_t max_len) {
        JIM::Decode_Text_Message_(buffer, name_idx, -1, -1, numeric_value, -1, -1, -1, max_len);
    }

    void __cdecl Decode_Text_Name_Tech_And_Other_Tech_(char* buffer, int16_t name_idx, int16_t other_name_idx, int16_t first_tech_idx, int16_t second_tech_idx, int16_t max_len) {
        JIM::Decode_Text_Message_(buffer, name_idx, -1, first_tech_idx, -1, other_name_idx, second_tech_idx, -1, max_len);
    }

    void __cdecl Decode_Text_Name_And_System_(char* buffer, int16_t name_idx, int16_t system_idx, int16_t max_len) {
        JIM::Decode_Text_Message_(buffer, name_idx, system_idx, -1, -1, -1, -1, -1, max_len);
    }

    void __cdecl Decode_Text_System_(char* buffer, int16_t system_idx, int16_t max_len) {
        JIM::Decode_Text_Message_(buffer, -1, system_idx, -1, -1, -1, -1, -1, max_len);
    }

    void __cdecl Decode_Text_Name_And_Other_Tech_(char* buffer, int16_t name_idx, int16_t other_name_idx, int16_t tech_idx, int16_t max_len) {
        JIM::Decode_Text_Message_(buffer, name_idx, -1, -1, -1, other_name_idx, tech_idx, -1, max_len);
    }

    void __cdecl Decode_Text_Name_And_Tech_(char* buffer, int16_t name_idx, int16_t tech_idx, int16_t max_len) {
        JIM::Decode_Text_Message_(buffer, name_idx, -1, tech_idx, -1, -1, -1, -1, max_len);
    }

    void __cdecl Decode_Text_Name_And_Name_(char* buffer, int16_t name_idx1, int16_t name_idx2, int16_t max_len) {
        Decode_Text_Message_(buffer, name_idx1, -1, -1, -1, name_idx2, -1, -1, max_len);
    }

    void __cdecl Decode_Text_Tech_And_Other_Name_(char* buffer, int16_t tech_id, int16_t other_name_1, int16_t other_name_2, int16_t max_len) {
        JIM::Decode_Text_Message_(buffer, tech_id, -1, other_name_1, -1, other_name_2, -1, -1, max_len);
    }

    void __cdecl Decode_Text_Field_(char* buffer, int16_t field_id, int16_t max_len) {
        Decode_Text_Message_(buffer, -1, -1, -1, -1, -1, -1, field_id, max_len);
    }

    void __cdecl Decode_Text_Message_(char* message_buffer, int16_t player1_id, int16_t star_id, int16_t tech_app1_id, int16_t number, int16_t player2_id, int16_t tech_app2_id, int16_t tech_field_id, int16_t max_len) {
        int16_t dest_len = 0;
        char temp_buf[40];
        char dest_buf[200];

        for (int32_t i = 0; i < (int32_t)sizeof(dest_buf); ++i) {
            char current_char = message_buffer[i];
            if (current_char == '\0') {
                break;
            }

            char* src_ptr = nullptr;
            if ((uint8_t)current_char >= 0x80 && (uint8_t)current_char <= 0x8A) {
                dest_buf[dest_len] = '\0';

                switch ((uint8_t)current_char) {
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

                strcpy(&dest_buf[dest_len], src_ptr);
                dest_len = (int16_t)strlen(dest_buf);
            } else {
                dest_buf[dest_len++] = current_char;
            }
        }

        dest_buf[dest_len] = '\0';

        if (strlen(dest_buf) >= (uint32_t)(max_len - 1)) {
            sprintf(dest_buf, s_Decode_message_too_long_0055593c, max_len, message_buffer);
            exitm::Exit_With_Message(dest_buf);
        } else {
            strcpy(message_buffer, dest_buf);
        }
    }

    void __cdecl Get_Text_Message_(const char* filename, int16_t message_index, char* dest, int16_t max_len) {
        char buffer[100];

        uint16_t lang = (uint8_t)MOX::_settings.language;
        if (lang > 5) {
            lang = 0;
        }

        RUSS::Mox_Update_();

        farload::Farload_Data_Static_(filename, (int16_t)(message_index * 6 + lang), buffer, 0, 1, (int16_t)sizeof(buffer));

        RUSS::Mox_Update_();

        int16_t message_len = (int16_t)strlen(buffer);

        if (max_len - 1 <= message_len || message_len >= (int16_t)(sizeof(buffer) - 1)) {
            sprintf(buffer, s_Message_too_long, message_index, filename, message_len);
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
                    current_volume = (int16_t)sound::Get_Stream_Volume_();
                    RUSS::Mox_Update_();
                } while (current_volume > 0);
            }

            sound::Stop_Stream_();

            int32_t ticks = clock();
            _jim_current_stream = (int16_t)((ticks % 3) + 4);

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
                sound::Set_Stream_Filename_(s_STREAM_LBX);
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
                int32_t ticks = (int32_t)clock();
                _jim_current_stream = (int16_t)((ticks % 3) + 1);
                sound::Set_Stream_Filename_(s_STREAM_LBX);
                sound::Play_Stream_(_jim_current_stream);
            } else {
                if (_jim_old_transition_id <= 100) {
                    sound::Set_Stream_Filename_(s_STREAM_LBX);
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
