#pragma once
#include <cstdint>

namespace SYS {
    inline void __cdecl Clear_System_Display_Loaded_Flag_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00493f30});
        fn();
    }

    inline void __cdecl Init_System_Display_(int16_t system_id) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00496250});
        fn(system_id);
    }
}