#pragma once

#include <cstdint>

namespace CMBTFIR2 {
    int16_t __cdecl Helmsman_Bonus_(int16_t leader_idx);

    int16_t __cdecl Weaponry_Bonus_(int16_t leader_idx);

    inline int8_t __cdecl Apply_Combat_Artemis_Net_(int8_t flag) {
        using Fn = int8_t(__cdecl*)(int8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e88b0});
        return fn(flag);
    }

    inline int16_t __cdecl Used_Teleporter_(int16_t ship) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004e8af0})(ship);
    }

    inline void __cdecl Set_Boarded_Flag_(int16_t ship) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004e8b80})(ship);
    }

    inline void __cdecl Resolve_Raid_(int16_t ship, int16_t a, int16_t* b) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004e7120})(ship, a, b);
    }

    inline void __cdecl Resolve_Capture_(int16_t ship, int16_t a, int16_t b, int32_t x, int32_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int32_t, int32_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004e7ea0})(ship, a, b, x, y);
    }

    inline int16_t __cdecl Already_Boarded_(int16_t ship) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004e8b50})(ship);
    }

    inline void __cdecl Set_Combat_Ship_Done_(int16_t ship) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004e8ac0})(ship);
    }

    inline int32_t __cdecl Max_Defensive_Fire_Range_(int16_t ship) {
        using Fn = int32_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004e6f40})(ship);
    }

    inline void __cdecl Init_Internal_Space_(s_combat_data* combat_ship) {
        using Fn = void(__cdecl*)(s_combat_data*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e6180});
        fn(combat_ship);
    }
}