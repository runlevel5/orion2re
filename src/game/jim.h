#pragma once

namespace JIM {
    inline uint16_t& _jim_current_stream =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x005558E0 });

    inline uint16_t& _jim_old_music_id =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x005558E8 });

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

    inline void __cdecl Stop_Background_Music_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004251b0});
        fn();
    }

    inline void __cdecl Play_Combat_Music_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004252a0 });
        fn();
    }

    inline void __cdecl Play_Streaming_Music_(int32_t music_id, int32_t p2, int32_t p3) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00424FB0});
        fn(music_id, p2, p3);
    }

    void __cdecl Decode_Text_Name_And_Name_(char* buffer, int16_t id1, int16_t id2, int16_t id3);

    inline void __cdecl Get_Text_Message_(const char* lbx_name, int16_t index, char* buffer, int16_t max_len) {
        using Fn = void(__cdecl*)(const char*, int16_t, char*, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00425340})(lbx_name, index, buffer, max_len);
    }

    void __cdecl Decode_Text_Name_And_Tech_(char* text, int16_t id1, int16_t id2, int16_t id3);

    void __cdecl Decode_Text_Name_And_Other_Tech_(char* buffer, int16_t id1, int16_t id2, int16_t id3, int16_t id4);

    void __cdecl Decode_Text_System_(char* buffer, int16_t message_id, int16_t system_id);

    void __cdecl Decode_Text_Name_And_System_(char* buffer, int16_t name_idx, int16_t system_idx, int16_t unknown_idx);

    void __cdecl Decode_Text_Name_Tech_And_Other_Tech_(char* text, int16_t val1, int16_t val2, int16_t val3, int16_t val4, int16_t val5);

    inline void __cdecl Bill_Picture_To_Mouse_(s_animation_header* sprite) {
        using Fn = void(__cdecl*)(s_animation_header*);
        reinterpret_cast<Fn>(0x00425A10)(sprite);
    }

    void __cdecl Decode_Text_Name_And_Stat_(char* text, int16_t val1, int16_t val2, int16_t val3);

    void __cdecl Decode_Text_Tech_And_Other_Name_(char* text, int16_t tech_id, int16_t other_name_1, int16_t other_name_2, int16_t other_name_3);

    void __cdecl Decode_Text_Race2_(char* buffer, int16_t race_idx1, int16_t race_idx2);

    void __cdecl Decode_Text_Race_(char* out_buffer, int16_t msg_idx, int16_t race_idx);

    void __cdecl Decode_Text_Stat_(char* buffer, int16_t stat_a, int16_t stat_b);

    inline void __cdecl Decode_Text_Message_(char* buffer, int16_t a1, int16_t a2, int16_t a3, int16_t a4, int16_t a5, int16_t a6, int16_t a7, int16_t a8) {
        using Fn = void(__cdecl*)(char*, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00425420});
        fn(buffer, a1, a2, a3, a4, a5, a6, a7, a8);
    }

    void __cdecl Decode_Text_Year_(char* buffer, int16_t year);

    inline void __cdecl Decode_Text_Field_(char* text_buffer, uint16_t field_id, int max_len) {
        using Fn = void(__cdecl*)(char*, uint16_t, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00425980 });
        fn(text_buffer, field_id, max_len);
    }
}