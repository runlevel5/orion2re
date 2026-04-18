#pragma once
#include <cstdint>

namespace SHIPSTAK {
    inline int16_t __cdecl Ship_Has_Special_(int16_t ship_idx, int16_t special_id) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00460600});
        return fn(ship_idx, special_id);
    }

    bool __cdecl Ship_Has_Stealth_Device_(int16_t ship_idx);

    short __cdecl Ship_Stack_Star_Id_(short node_idx);

    void __cdecl Find_Ship_Stacks_();

    inline void __cdecl Remove_Non_Detected_Ships_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004608f0})();
    }

    inline void __cdecl Sort_Ships_In_Stack_(int16_t stack_start_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00460d40})(stack_start_idx);
    }
}