#pragma once
#include <cstdint>

struct s_planet_data;

namespace HOMEGEN {
    inline const char* s_Found_no_satellites_after_3_MIN_e_0055adc8 =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055ADC8}); // "Found no satellites after 3 MIN epochs for star %d"

    inline int32_t& _HOME_WORLD_GEN_MAX_CTR = *reinterpret_cast<int32_t*>(std::uintptr_t{ 0x00572390 });
    inline s_good_star*& _good_stars = *reinterpret_cast<s_good_star**>(std::uintptr_t{ 0x00572398 });
    inline int16_t* _nearest_food_worlds = reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005723A0 });
    inline StarDist*& _star_list = *reinterpret_cast<StarDist**>(0x005723B0);

    bool __cdecl Is_Added_Food_World_(int16_t world_id);

    uint32_t __cdecl Generate_Home_Worlds_();

    #pragma pack(push, 1)
    struct CandidateList {
        uint16_t stars[20];
        uint16_t count;
    };
    #pragma pack(pop)

    uint8_t __cdecl Build_Home_Star_List_(uint16_t* home_stars_out, int32_t min_dist);

    uint32_t __cdecl Dist_Between_Stars_(int16_t star_a, int16_t star_b);

    void __cdecl Set_Up_Home_Gen_Msg_();

    void __cdecl Build_Min_Star_Distances_(int32_t min_dist_sq);

    void __cdecl Randomize_Home_Worlds_(int16_t* arr);

    void __cdecl Modify_Home_Worlds_();

    inline void __cdecl FUN_0051ad50() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051AD50 })();
    }

    void __cdecl Make_System_Monsters_();

    void __cdecl Assign_Home_Star_Names_();

    void __cdecl Enforce_Climate_(int16_t star_idx, int16_t climate_id, int16_t* out_planet_idx);

    void __cdecl Remove_Specials_(int16_t star_idx);

    void __cdecl Set_Planet_Size(int16_t planet_index, int8_t size);

    void __cdecl Enforce_Medium_Planet_Size_(int16_t planet_idx);

    void __cdecl Enforce_Planet_Type_(int16_t star_idx, int16_t planet_idx);

    /**
     * @address 0051a150
     */
    void __cdecl Enforce_Planet_Abundant_Mineral_Class(int16_t planet_idx);

    void __cdecl Enforce_Mineral_Richness_(int16_t planet_index, int16_t mineral_class);

    inline void __cdecl Set_System_Planet_Artifact_Special(int16_t planet_idx, int16_t star_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0051b530 })(planet_idx, star_idx);
    }

    void __cdecl Enforce_Gravity_(int16_t planet_idx, int8_t gravity_class, int16_t player_idx);

    int __cdecl compare_star_dist(const void* a, const void* b);

    void __cdecl Guarantee_A_Planet_At_Min_Dist_(int16_t star_idx, int16_t min_dist, int16_t* home_star_ids);

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