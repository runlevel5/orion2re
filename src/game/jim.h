#pragma once

namespace JIM {
    inline uint16_t& _jim_current_stream =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x005558E0 });

    inline uint16_t& _jim_old_music_id =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x005558E8 });

    // Potential string literal: "STREAM.LBX"
    inline const char* s_STREAM_LBX =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x005558FC });

    inline void Draw_Logos_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00425A70 });
        fn();
    }

    void Play_Background_Music_();
}