#pragma once

#include <cstdint>

namespace SMACK {
    inline void __cdecl Play_Cinematic_(const char* lbx_file, int16_t arg2, int16_t arg3) {
        using Fn = void(__cdecl*)(const char*, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004d7500})(lbx_file, arg2, arg3);
    }
}