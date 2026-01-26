#pragma once
#include <cstdint>

namespace MAPGEN {
    inline uint8_t& _make_food_planet = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4AF4});
    inline uint8_t& _n_space_monsters = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4B56});
    inline char* _companion_star = reinterpret_cast<char*>(std::uintptr_t{0x0055632C});
    inline void*& _fill_msg_bitmap = *reinterpret_cast<void**>(std::uintptr_t{0x005A4B20});
    inline char (&_fill_msg)[36] = *reinterpret_cast<char (*)[36]>(std::uintptr_t{0x005A4B30});
    inline int16_t& _fill_msg_ctr = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A4B54});
    inline const char* s_raceopt_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558B24}); // "RACEOPT.LBX"

    uint8_t __cdecl Generate_Spectral_Class_(int16_t star_type_index);
    uint8_t __cdecl Generate_Food_Per_Farmer_(int16_t star_index, int16_t planet_index);
    uint8_t __cdecl Generate_Max_Farms_(int16_t star_idx, int16_t planet_idx);
    int8_t __cdecl Generate_Orbit_(int16_t star_id);
    void __cdecl Generate_Satellite_(int16_t star_idx);

    inline int8_t __cdecl Generate_Size_() {
        using Fn = int8_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b65d0});
        return fn();
    }

    inline int8_t __cdecl Generate_Mineral_Class_(int16_t star_index, int16_t planet_count) {
        using Fn = int8_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b5cf0});
        return fn(star_index, planet_count);
    }

    inline void __cdecl Get_Climate_Modifier_(int8_t mineral_class) {
        using Fn = void(__cdecl*)(int8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b6b20});
        fn(mineral_class);
    }

    inline int8_t __cdecl Generate_Gravity_Class_(int16_t planet_count) {
        using Fn = int8_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b5c50});
        return fn(planet_count);
    }

    inline int16_t __cdecl Get_Planet_Group_(int16_t planet_count) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b6b60});
        return fn(planet_count);
    }

    inline int8_t __cdecl Generate_Climate_(int16_t group) {
        using Fn = int8_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b5af0});
        return fn(group);
    }

    inline void __cdecl Universe_Generation_(int16_t arg_1c, int32_t arg_20, int16_t arg_18) {
        using Fn = void(__cdecl*)(int16_t, int32_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004B7120})(arg_1c, arg_20, arg_18);
    }

    inline void __cdecl Enforce_Planet_Max_(int limit) {
        using Fn = void(__cdecl*)(int);
        reinterpret_cast<Fn>(std::uintptr_t{0x004B57E0})(limit);
    }

    inline void __cdecl Set_Black_Hole_Visited_Flags_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B6DB0})();
    }

    inline void __cdecl Generate_Wormhole_Links_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B69F0})();
    }

    inline void __cdecl Assign_Marooned_Heroes_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B56B0})();
    }

    inline void __cdecl Set_Secondary_Planet_Data_(int16_t star_idx, int16_t planet_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B6490});
        fn(star_idx, planet_idx);
    }

    inline void __cdecl Get_A_Planet_Or_Star_Special_(int16_t star_idx, uint8_t* need_new_flag, MOX::SYSTEM_SPECIAL* system_special, uint8_t unknown, int32_t monster_mask) {
        using Fn = void(__cdecl*)(int16_t, uint8_t*, MOX::SYSTEM_SPECIAL*, uint8_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b6650});
        fn(star_idx, need_new_flag, system_special, unknown, monster_mask);
    }

    inline void __cdecl Load_Nebula_Pictures_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B7040})();
    }
}