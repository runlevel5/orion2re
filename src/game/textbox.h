#pragma once
#include <cstdint>

namespace TEXTBOX {
    inline void __cdecl Get_Help_Lbx_Name_(char* buffer) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00471850});
        fn(buffer);
    }

    inline void __cdecl Text_Box_(const char* title, const char* content) {
        using Fn = void(__cdecl*)(const char*, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00471070});
        fn(title, content);
    }
}