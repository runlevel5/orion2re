#pragma once
#include <cstdint>

namespace capture {
    inline void __cdecl Screen_Flic_Capture_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00534A60})();
    }

    inline void __cdecl Check_Release_Version_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00520210})();
    }
}