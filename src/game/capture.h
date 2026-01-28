#pragma once
#include <cstdint>

namespace capture {
    inline uint16_t& _release_flag =
            *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055AFC8});

    void Release_Version_();

    inline void __cdecl Screen_Flic_Capture_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00534A60})();
    }

    inline int16_t __cdecl Check_Release_Version_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00520210});
        return fn();
    }
}