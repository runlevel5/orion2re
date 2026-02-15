#pragma once
#include <cstdint>

namespace TEXTBOX {
    inline void __cdecl Get_Help_Lbx_Name_(char* buffer) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00471850});
        fn(buffer);
    }

    inline void __cdecl Do_Text_Box_(const char* param_1, const char* param_2, uint32_t param_3) {
        using Fn = void(__cdecl*)(const char*, const char*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004710b0});
        fn(param_1, param_2, param_3);
    }

    void __cdecl Text_Box_(const char* param_1, const char* param_2);
}