#pragma once
#include <cstdint>

namespace FMTPARA {
    inline int16_t __cdecl Get_Formatted_Paragraph_Max_Height_(int16_t max_width, char* text) {
        using Fn = int16_t(__cdecl*)(int16_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00432e70});
        return fn(max_width, text);
    }

    inline int16_t __cdecl Get_Last_Formatted_Paragraph_Max_Width_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00432ee0});
        return fn();
    }

    inline void __cdecl Print_Formatted_Paragraph_(short x, short y, short width, char* buffer, short unknown) {
        using Fn = void(__cdecl*)(short, short, short, char*, short);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00432dd0});
        fn(x, y, width, buffer, unknown);
    }
}