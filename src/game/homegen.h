#pragma once
#include <cstdint>

struct s_planet_data;

namespace HOMEGEN {
    inline int32_t& _HOME_WORLD_GEN_MAX_CTR = *reinterpret_cast<int32_t*>(std::uintptr_t{ 0x00572390 });
    inline s_good_star*& _good_stars = *reinterpret_cast<s_good_star**>(std::uintptr_t{ 0x00572398 });
    inline uint32_t* _nearest_food_worlds = reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x005723A0 });

    uint32_t __cdecl Generate_Home_Worlds_();

    #pragma pack(push, 1)
    struct CandidateList {
        uint16_t stars[20];
        uint16_t count;
    };
    #pragma pack(pop)

    uint8_t __cdecl Build_Home_Star_List_(uint16_t* home_stars_out, int32_t min_dist);

    uint32_t __cdecl Dist_Between_Stars_(uint16_t star_a, uint16_t star_b);

    void __cdecl Set_Up_Home_Gen_Msg_();

    void __cdecl Build_Min_Star_Distances_(int32_t min_dist_sq);

    void __cdecl Randomize_Home_Worlds_(int16_t* arr);

    void __cdecl Modify_Home_Worlds_();

    inline void __cdecl FUN_0051ad50() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051AD50 })();
    }

    inline void __cdecl Make_System_Monsters_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0051C3F0})();
    }

    inline void __cdecl Assign_Home_Star_Names_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00519ea0 })();
    }

    inline void __cdecl Enforce_Climate_(int16_t star_idx, int16_t climate_threshold, int16_t* out_planet_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051a170 })(star_idx, climate_threshold, out_planet_idx);
    }

    inline void __cdecl Remove_Specials_(int16_t star_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051bc20 })(star_idx);
    }

    inline void __cdecl Set_Planet_Size(int16_t planet_idx, int16_t size) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051b570 })(planet_idx, size);
    }

    inline void __cdecl Enforce_Medium_Planet_Size_(int16_t planet_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051a490 })(planet_idx);
    }

    inline void __cdecl Enforce_Planet_Type_(int16_t star_idx, int16_t planet_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051a5c0 })(star_idx, planet_idx);
    }

    inline void __cdecl Enforce_Planet_Abundant_Mineral_Class(int16_t planet_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051a150 })(planet_idx);
    }

    inline void __cdecl Enforce_Mineral_Richness_(int16_t planet_idx, int16_t richness) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051a4d0 })(planet_idx, richness);
    }

    inline void __cdecl Set_System_Planet_Artifact_Special(int16_t planet_idx, int16_t star_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051b530 })(planet_idx, star_idx);
    }

    inline void __cdecl Enforce_Gravity_(int16_t planet_idx, int8_t gravity_type, int16_t owner_idx) {
        using Fn = void(__cdecl*)(int16_t, int8_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051a410 })(planet_idx, gravity_type, owner_idx);
    }

    inline void __cdecl Guarantee_A_Planet_At_Min_Dist_(int16_t star_idx, int32_t min_dist, int16_t* home_star_ids) {
        using Fn = void(__cdecl*)(int16_t, int32_t, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051b8c0 })(star_idx, min_dist, home_star_ids);
    }

    inline void __cdecl Ensure_At_Least_1_Food_Planet_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051b590 })();
    }

    inline int16_t __cdecl Usable_Orbit_(int16_t star_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(0x0051c2b0)(star_idx);
    }

    inline void __cdecl Make_One_Planet_(int16_t star_idx, uint16_t planet_idx, int16_t orbit, int8_t special) {
        using Fn = void(__cdecl*)(int16_t, uint16_t, int16_t, int8_t);
        reinterpret_cast<Fn>(0x0051b1d0)(star_idx, planet_idx, orbit, special);
    }

    void __cdecl Enforce_3_Planet_Minimum_(int16_t star_idx);

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