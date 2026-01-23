#pragma once

#include <cstdint>

namespace file_ani {
    inline void __cdecl Set_File_Animation_Buffer_(uint32_t buffer) {
        using Fn = void(__cdecl*)(uint32_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x005317a0})(buffer);
    }

    inline void __cdecl Reset_File_Animation_Buffer_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00531780})();
    }

    inline void __cdecl Open_File_Animation_(const char* filename, int16_t index) {
        using Fn = void(__cdecl*)(const char*, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00530bf0})(filename, index);
    }

    inline void __cdecl Draw_File_Animation_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00530db0})();
    }

    inline void __cdecl Draw_File_Animation_Stencil_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00531540})();
    }

    inline int16_t __cdecl Get_File_Animation_Frame_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00531740})();
    }

    inline int16_t __cdecl Get_File_Animation_Total_Frames_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x004e8c60})();
    }
}