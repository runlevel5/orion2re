#pragma once
#include <cstdint>

namespace BILL {
    inline void __cdecl Bill_Init_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00406780})();
    }

    void __cdecl Cache_Player_Scientist_Anim_();

    inline void __cdecl Copy_File_To_File_(const char* src, const char* dest) {
        using Fn = void(__cdecl*)(const char*, const char*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004077E0})(src, dest);
    }

    char* __cdecl Cache_Player_Scientist_Anim_Imp_(int16_t player_index);

    inline void __cdecl Bill_Default_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004067b0});
        fn();
    }

    int16_t __cdecl Get_Their_Spy_Number_(int16_t player_idx, int16_t target_idx);

    inline void __cdecl Squeeze_Print_(int16_t x, int16_t y, char* text, int16_t limit) {
        using Fn = void(__cdecl*)(int16_t, int16_t, char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004073E0});
        fn(x, y, text, limit);
    }

    inline void __cdecl Squeeze_Paragraph_Centered_(int16_t x1, int16_t x2, int16_t y, int16_t height, char* text, int16_t unknown) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004076b0});
        fn(x1, x2, y, height, text, unknown);
    }

    inline void __cdecl Squeeze_Print_Formatted_Paragraph_(int x, int y, int x2, int y2, char* buffer, int unknown) {
        using Fn = void(__cdecl*)(int, int, int, int, char*, int);
        reinterpret_cast<Fn>(std::uintptr_t{0x00407680})(x, y, x2, y2, buffer, unknown);
    }

    inline void __cdecl Squeeze_Formatted_Paragraph_Centered_(int16_t a1, int16_t y, uint16_t type, int16_t a4, void* buffer, int32_t a6) {
        using Fn = void(__cdecl*)(int16_t, int16_t, uint16_t, int16_t, void*, int32_t);
        reinterpret_cast<Fn>(0x004076f0)(a1, y, type, a4, buffer, a6);
    }

    inline void __cdecl Squeeze_Print_Centered_(int16_t x, int16_t y, const char* text, int16_t width) {
        using Fn = void(__cdecl*)(int16_t, int16_t, const char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00407410});
        fn(x, y, text, width);
    }

    inline void __cdecl _Print_Paragraph_(uint16_t x, int16_t y, uint16_t width, const char* text, int shadow_style) {
        using Fn = void(__cdecl*)(uint16_t, int16_t, uint16_t, const char*, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00408090});
        fn(x, y, width, text, shadow_style);
    }

    inline int16_t __cdecl _Get_Paragraph_Max_Width_(int32_t max_width, char* text, int32_t param_3) {
        using Fn = int16_t(__cdecl*)(int32_t, char*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00408120});
        return fn(max_width, text, param_3);
    }

    inline char* _dest_file_name = reinterpret_cast<char*>(std::uintptr_t{0x00564738});
    inline const char* s_SR_R_x_SC_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x005512B0}); // "SR_R%x_SC.LBX"
    inline const char* s_SR_R_wildcard_SC_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x005512E0}); // "SR_R?_SC.LBX"
}