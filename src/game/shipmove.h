#pragma once
#include <cstdint>
#include "types.h"

namespace SHIPMOVE {
    // Suggested name: Allocate_New_Ship_Slots_
    inline void __cdecl Allocate_New_Ship_Slots_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00436F50});
        fn();
    }

    inline void __cdecl Update_All_Ship_Ranges_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00437170});
        fn();
    }

    inline bool __cdecl Star_In_Normal_Range_Of_Player_(s_star_data* star, int16_t player_idx) {
        using Fn = bool(__cdecl*)(s_star_data*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00436230});
        return fn(star, player_idx);
    }
}