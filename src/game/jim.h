#pragma once

namespace JIM {
    inline uint16_t& _jim_current_stream =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x005558E0 });

    inline uint16_t& _jim_old_music_id =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x005558E8 });

    // Potential string literal: "STREAM.LBX"
    inline const char* s_STREAM_LBX =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x005558FC });

    inline void* _logo_mouse_list = reinterpret_cast<void*>(std::uintptr_t{ 0x005558F0 });
    inline int16_t& _darken_palette_value = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005A7D96 });

    inline const char* s_intro_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x00555960 });   // "intro.lbx"
    inline const char* s_simtex_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055596C });  // "simtex.lbx"
    inline const char* s_logo_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x00555978 });    // "logo.lbx"
    inline const char* s_fmt_ssimtex_lbx = reinterpret_cast<const char*>(std::uintptr_t{ 0x00555984 }); // "%ssimtex.lbx"

    // External functions
    bool __cdecl Check_For_Simtex_Logo_();

    void __cdecl Logo_Fadeout_();

    void __cdecl Fast_Fade_In_();

    void __cdecl Draw_Logos_(void);
    void Play_Background_Music_();
}