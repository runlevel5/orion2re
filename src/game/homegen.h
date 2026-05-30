#pragma once
#include <cstdint>

struct s_planet_data;

namespace HOMEGEN {
#pragma pack(push, 1)
    struct s_star_dist {
        int16_t dist;
        int16_t index;
    };
#pragma pack(pop)    
    
    inline const char* s_Found_no_satellites_after_3_MIN_e_0055adc8 =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055ADC8}); // "Found no satellites after 3_MIN enforcement: Star: %d"

    inline const char* s_Orion = reinterpret_cast<const char*>(std::uintptr_t{0x0055a738}); // "Orion"

    extern int32_t _HOME_WORLD_GEN_MAX_CTR;
    extern s_good_star* _good_stars;
    extern int16_t _nearest_food_worlds[8];
    extern s_star_dist* _star_list;

    bool __cdecl Is_Added_Food_World_(int16_t world_id);

    uint16_t __cdecl Generate_Home_Worlds_();

    #pragma pack(push, 1)
    struct CandidateList {
        int16_t stars[20];
        int16_t count;
    };

    struct s_orion_candidate {
        int32_t dist;
        int16_t star_idx;
    };

    struct StarCandidate {
        int32_t distance;
        int16_t star_id;
    };
    #pragma pack(pop)

    int __cdecl qsort_1_(const void* candidate_a, const void* candidate_b);

    uint8_t __cdecl Build_Home_Star_List_(int16_t* home_stars_out, int32_t min_dist);

    int32_t __cdecl qsort_star_dist_list_(const void* param_1, const void* param_2);

    int32_t __cdecl Dist_Between_Stars_(int16_t star_a, int16_t star_b);

    int32_t __cdecl Dist_Between_Points_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Set_Up_Home_Gen_Msg_();

    void __cdecl Build_Min_Star_Distances_(int32_t min_dist_sq);

    void __cdecl Randomize_Home_Worlds_(int16_t* arr);

    void __cdecl Modify_Home_Worlds_();

    int16_t __cdecl Generate_Orion_();

    int16_t __cdecl Get_Orion_Star_Id_();

    void __cdecl Make_System_Monsters_();

    void __cdecl Assign_Home_Star_Names_();

    void __cdecl Enforce_Climate_(int16_t star_idx, int16_t climate_id, int16_t* out_planet_idx);

    void __cdecl Remove_Specials_(int16_t star_idx);

    void __cdecl Set_Planet_Size(int16_t planet_index, int8_t size);

    void __cdecl Enforce_Medium_Planet_Size_(int16_t planet_idx);

    void __cdecl Enforce_Planet_Type_(int16_t star_idx, int16_t planet_idx);

    void __cdecl Enforce_Planet_Abundant_Mineral_Class(int16_t planet_idx);

    void __cdecl Enforce_Mineral_Richness_(int16_t planet_index, int16_t mineral_class);

    void __cdecl Set_System_Planet_Artifact_Special(int16_t planet_idx, int16_t star_idx);

    void __cdecl Enforce_Gravity_(int16_t planet_idx, int8_t gravity_class, int16_t player_idx);

    void __cdecl Guarantee_A_Planet_At_Min_Dist_(int16_t star_idx, int16_t min_dist, int16_t* home_star_ids);

    uint8_t __cdecl Ensure_At_Least_1_Food_Planet_();

    void __cdecl Add_Non_Food_Planets_To_List_(int16_t star_idx, int16_t* planet_list, int16_t* count);

    int16_t __cdecl Usable_Orbit_(int16_t star_idx);

    void __cdecl Make_One_Planet_(int16_t star_idx, int16_t planet_idx, int16_t orbit_idx, int8_t is_food_planet);

    void __cdecl Enforce_3_Planet_Minimum_(int16_t star_idx);

    int16_t __cdecl Random_Planet_(int16_t star_idx);

    void __cdecl Set_Special_For_Star_Guarded_By_Monster_(s_star_data* star, int16_t monster_type);

    void __cdecl Upgrade_Planet_Guarded_By_Monster_(s_planet_data* planet, int16_t monster_type);

    bool __cdecl Star_Has_Planet_(int16_t star_index);
}