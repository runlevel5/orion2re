#pragma once
#include <cstdint>

struct s_planet_data;

namespace HOMEGEN {
    inline int16_t __cdecl Generate_Home_Worlds_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x0051A620})();
    }

    inline void __cdecl Make_System_Monsters_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0051C3F0})();
    }

    inline void __cdecl Assign_Home_Star_Names_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00519ea0 })();
    }

    inline void __cdecl Init_Diplomatic_Relations_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00450480 })();
    }

    inline int16_t __cdecl Random_Planet_(int16_t star_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x0051BEA0 })(star_idx);
    }

    inline void __cdecl Upgrade_Planet_Guarded_By_Monster_(s_planet_data* planet, int16_t ship_type) {
        using Fn = void(__cdecl*)(s_planet_data*, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051BF50 })(planet, ship_type);
    }
}