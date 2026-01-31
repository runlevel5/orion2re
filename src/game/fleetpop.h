#pragma once
#include <cstdint>

namespace FLEETPOP {
    inline void __cdecl Reload_Fleet_Box_Data_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00405990})();
    }

    inline void __cdecl Set_Fleet_Movement_Icon_Selections_(uint8_t p1, uint8_t p2) {
        using Fn = void(__cdecl*)(uint8_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00406330});
        fn(p1, p2);
    }
}