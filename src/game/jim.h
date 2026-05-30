#pragma once

struct s_animation_header;
struct s_mouse_list;

namespace JIM {
    extern int16_t _jim_current_stream;

    extern int16_t _jim_old_music_id;

    extern int16_t _jim_fade_music_flag;
    extern int16_t _jim_old_transition_id;

    extern const char s_STREAM_LBX[];
    extern const char s_STREAMHD_LBX_00555908[];

    extern s_mouse_list _logo_mouse_list;
    extern int16_t _darken_palette_value;

    extern const char jimtext_lbx[];

    extern const char s_intro_lbx[];
    extern const char s_simtex_lbx[];
    extern const char s_logo_lbx[];
    extern const char s_fmt_ssimtex_lbx[];

    // External functions
    bool __cdecl Check_For_Simtex_Logo_();

    void __cdecl Logo_Fadeout_();

    void __cdecl Draw_Logos_(void);
    void Play_Background_Music_();

    void __cdecl Stop_Background_Music_();

    void __cdecl Play_Combat_Music_();

    void __cdecl Play_Streaming_Music_(int16_t music_id, int16_t transition_id, int16_t loop_flag);

    void __cdecl Decode_Text_Name_And_Name_(char* buffer, int16_t id1, int16_t id2, int16_t id3);

    void __cdecl Get_Text_Message_(const char* filename, int16_t msg_idx, char* dest, int16_t max_len);

    extern const char s_Message_too_long[];

    void __cdecl Decode_Text_Name_And_Tech_(char* text, int16_t id1, int16_t id2, int16_t id3);

    void __cdecl Decode_Text_Name_And_Other_Tech_(char* buffer, int16_t id1, int16_t id2, int16_t id3, int16_t id4);

    void __cdecl Decode_Text_System_(char* buffer, int16_t message_id, int16_t system_id);

    void __cdecl Decode_Text_Name_And_System_(char* buffer, int16_t name_idx, int16_t system_idx, int16_t unknown_idx);

    void __cdecl Decode_Text_Name_Tech_And_Other_Tech_(char* text, int16_t val1, int16_t val2, int16_t val3, int16_t val4, int16_t val5);

    void __cdecl Bill_Picture_To_Mouse_(s_animation_header* anim);

    void __cdecl Decode_Text_Name_And_Stat_(char* text, int16_t val1, int16_t val2, int16_t val3);

    void __cdecl Decode_Text_Tech_And_Other_Name_(char* text, int16_t tech_id, int16_t other_name_1, int16_t other_name_2, int16_t other_name_3);

    void __cdecl Decode_Text_Race2_(char* buffer, int16_t race_idx1, int16_t race_idx2);

    void __cdecl Decode_Text_Race_(char* out_buffer, int16_t msg_idx, int16_t race_idx);

    void __cdecl Decode_Text_Stat_(char* buffer, int16_t stat_a, int16_t stat_b);

    void __cdecl Decode_Text_Message_(char* text, int16_t player1_id, int16_t star_id, int16_t tech_app1_id, int16_t number, int16_t player2_id, int16_t tech_app2_id, int16_t tech_field_id, int16_t max_len);

    extern const char s_d_comma_d_0055239c[];
    extern const char s_d_dot_d_005523a4[];
    extern const char s_Decode_message_too_long_0055593c[];

    void __cdecl Decode_Text_Year_(char* buffer, int16_t year);

    void __cdecl Decode_Text_Field_(char* text, int16_t field_id, int16_t max_len);
}