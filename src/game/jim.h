#pragma once

struct s_animation_header;
struct s_mouse_list;

namespace JIM {
    inline int16_t& _jim_current_stream =
        *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005558E0 });

    inline int16_t& _jim_old_music_id =
        *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005558E8 });

    // Potential string literal: "STREAM.LBX"
    inline const char* s_STREAM_LBX =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x005558FC });

    inline s_mouse_list& _logo_mouse_list = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{ 0x005558F0 });
    inline int16_t& _darken_palette_value = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005A7D96 });

    inline const char* jimtext_lbx = reinterpret_cast<const char*>(0x00552360);

    inline const char* s_intro_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x00555960 });   // "intro.lbx"
    inline const char* s_simtex_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055596C });  // "simtex.lbx"
    inline const char* s_logo_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x00555978 });    // "logo.lbx"
    inline const char* s_fmt_ssimtex_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x00555984 }); // "%ssimtex.lbx"

    // External functions
    bool __cdecl Check_For_Simtex_Logo_();

    void __cdecl Logo_Fadeout_();

    void __cdecl Draw_Logos_(void);
    void Play_Background_Music_();

    void __cdecl Stop_Background_Music_();

    void __cdecl Play_Combat_Music_();

    inline void __cdecl Play_Streaming_Music_(int32_t music_id, int32_t p2, int32_t p3) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00424FB0});
        fn(music_id, p2, p3);
    }

    void __cdecl Decode_Text_Name_And_Name_(char* buffer, int16_t id1, int16_t id2, int16_t id3);

    void __cdecl Get_Text_Message_(const char* filename, int16_t msg_idx, char* dest, int16_t max_len);

    inline char* s_Message_too_long = reinterpret_cast<char*>(std::uintptr_t{0x00555918});

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

    inline const char* s_d_comma_d_0055239c = reinterpret_cast<const char*>(0x0055239C);
    inline const char* s_d_dot_d_005523a4 = reinterpret_cast<const char*>(0x005523A4);
    inline const char* s_Decode_message_too_long_0055593c = reinterpret_cast<const char*>(0x0055593C);

    void __cdecl Decode_Text_Year_(char* buffer, int16_t year);

    void __cdecl Decode_Text_Field_(char* text, int16_t field_id, int16_t max_len);
}