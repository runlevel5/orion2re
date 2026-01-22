#pragma once

namespace JIM {
    inline void Draw_Logos_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00425A70 });
        fn();
    }
    inline void Play_Background_Music_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004251D0 });
        fn();
    }
}