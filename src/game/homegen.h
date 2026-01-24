#pragma once
#include <cstdint>

struct s_planet_data;

namespace HOMEGEN {
    inline uint32_t& _HOME_WORLD_GEN_MAX_CTR = *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00572390 });
    inline uint8_t*& _good_stars = *reinterpret_cast<uint8_t**>(std::uintptr_t{ 0x00572398 });
    inline uint32_t* _nearest_food_worlds = reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x005723A0 });

    uint32_t __cdecl Generate_Home_Worlds_();

    #pragma pack(push, 1)
    struct CandidateList {
        uint16_t stars[20];
        uint16_t count;
    };
    #pragma pack(pop)

    uint8_t __cdecl Build_Home_Star_List_(uint16_t* home_stars_out, int32_t min_dist);

    int32_t __cdecl Dist_Between_Stars_(uint16_t star_a, uint16_t star_b);

    inline void __cdecl Set_Up_Home_Gen_Msg_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051BC90 })();
    }

    inline void __cdecl Build_Min_Star_Distances_(int32_t min_dist_sq) {
        using Fn = void(__cdecl*)(int32_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051ACA0 })(min_dist_sq);
    }

    inline void __cdecl Randomize_Home_Worlds_(int16_t* home_stars) {
        using Fn = void(__cdecl*)(int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051BBC0 })(home_stars);
    }

    inline void __cdecl Modify_Home_Worlds_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051B300 })();
    }

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