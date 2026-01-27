#pragma once
#include <cstdint>

namespace MAPGEN {
    inline uint8_t& _n_marooned_heroes = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055899C});
    inline uint8_t& _n_wormholes = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005589A0});
    inline uint8_t& _make_food_planet = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4AF4});
    inline uint8_t& _n_space_monsters = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4B56});
    inline uint16_t& _is_companion_star = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A4B18});
    inline char* _companion_star = reinterpret_cast<char*>(std::uintptr_t{0x0055632C});
    inline void*& _fill_msg_bitmap = *reinterpret_cast<void**>(std::uintptr_t{0x005A4B20});
    inline char (&_fill_msg)[36] = *reinterpret_cast<char (*)[36]>(std::uintptr_t{0x005A4B30});
    inline int16_t& _fill_msg_ctr = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A4B54});
    inline s_planet_data*& _pl = *reinterpret_cast<s_planet_data**>(std::uintptr_t{0x005A4B1C});
    inline const char* s_raceopt_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558B24}); // "RACEOPT.LBX"
    inline const char* s_starbg_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558A10}); // "starbg.lbx"
    inline const char* s_Moving_planet_d_to_planet_d = 
        reinterpret_cast<const char*>(std::uintptr_t{0x005589AC}); // "Moving planet %d to planet %d"
    inline const char* s_Colony_Planets_d_Max_d = 
        reinterpret_cast<const char*>(std::uintptr_t{0x005589CC}); // "Colony Planets: %d Max: %d Plane..."

    inline void __cdecl Clear_Planets_In_Star_Structure_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b5790});
        fn();
    }

    inline int16_t __cdecl Gas_Giants_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b5a80});
        return fn();
    }

    inline int16_t __cdecl Home_Planet_(int16_t planet_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b6be0});
        return fn(planet_idx);
    }

    uint8_t __cdecl Generate_Spectral_Class_(int16_t star_type_index);
    uint8_t __cdecl Generate_Food_Per_Farmer_(int16_t star_index, int16_t planet_index);
    uint8_t __cdecl Generate_Max_Farms_(int16_t star_idx, int16_t planet_idx);
    int8_t __cdecl Generate_Orbit_(int16_t star_id);
    void __cdecl Generate_Satellite_(int16_t star_idx);
    uint8_t __cdecl Generate_Size_();

    inline int8_t __cdecl Generate_Mineral_Class_(int16_t star_index, int16_t planet_count) {
        using Fn = int8_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b5cf0});
        return fn(star_index, planet_count);
    }

    int8_t __cdecl Get_Climate_Modifier_(int8_t climate_index);

    int8_t __cdecl Generate_Gravity_Class_(int16_t planet_idx);

    int16_t __cdecl Get_Planet_Group_(int16_t planet_idx);

    inline void __cdecl Update_Planet_Group_Cache() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b56a0});
        fn();
    }

    uint8_t __cdecl Generate_Climate_(int16_t star_type);

    inline void __cdecl Universe_Generation_(int8_t gal_size, int8_t gal_age, int8_t difficulty) {
        using Fn = void(__cdecl*)(int8_t, int8_t, int8_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004B7120})(gal_size, gal_age, difficulty);
    }

    void __cdecl Enforce_Planet_Max_(int16_t max_planets);

    void __cdecl Set_Black_Hole_Visited_Flags_();

    void __cdecl Generate_Wormhole_Links_();

    inline void __cdecl Create_Wormhole_End_(int16_t star_idx, uint8_t* used_stars) {
        using Fn = void(__cdecl*)(int16_t, uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b6e00});
        fn(star_idx, used_stars);
    }

    void __cdecl Assign_Marooned_Heroes_();

    void __cdecl Set_Secondary_Planet_Data_(int16_t star_idx, int16_t planet_idx);

    void __cdecl Get_A_Planet_Or_Star_Special_(int16_t star_idx, int16_t* wormhole_out, int8_t* special_out, uint8_t commit_changes, int32_t excluded_mask);

    void __cdecl Load_Nebula_Pictures_();
}