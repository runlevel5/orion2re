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

    inline const char* s_intro_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x00555960 });   // "intro.lbx"
    inline const char* s_simtex_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055596C });  // "simtex.lbx"
    inline const char* s_logo_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x00555978 });    // "logo.lbx"
    inline const char* s_fmt_ssimtex_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x00555984 }); // "%ssimtex.lbx"

    // External functions
    bool __cdecl Check_For_Simtex_Logo_();

    void __cdecl Logo_Fadeout_();

    void __cdecl Draw_Logos_(void);
    void Play_Background_Music_();

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

    inline void __cdecl Get_Text_Message_(const char* lbx_name, int16_t index, char* buffer, int16_t max_len) {
        using Fn = void(__cdecl*)(const char*, int16_t, char*, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00425340})(lbx_name, index, buffer, max_len);
    }

    inline void __cdecl Bill_Picture_To_Mouse_(s_animation_header* sprite) {
        using Fn = void(__cdecl*)(s_animation_header*);
        reinterpret_cast<Fn>(0x00425A10)(sprite);
    }

    inline void __cdecl Decode_Text_Race_(char* buffer, int16_t player_idx, int16_t max_len) {
        using Fn = void(__cdecl*)(char*, int16_t, int16_t);
        reinterpret_cast<Fn>(0x00425770)(buffer, player_idx, max_len);
    }
}