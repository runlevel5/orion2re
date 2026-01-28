#pragma once
#include <cstdint>

namespace MAPGEN {
    inline uint8_t& _n_marooned_heroes = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055899C});
    inline uint8_t& _n_wormholes = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005589A0});
    inline uint8_t& _make_food_planet = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4AF4});
    inline uint8_t& _n_space_monsters = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4B56});
    inline uint16_t& _is_companion_star = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A4B18});
    inline char* _companion_star = reinterpret_cast<char*>(std::uintptr_t{0x0055632C});
    inline uint8_t& _n_maps_generated = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056CD90});
    inline uint16_t (&_tests_failed)[4] = *reinterpret_cast<uint16_t(*)[4]>(std::uintptr_t{0x0056CD98});
    inline int8_t*& __evan_ptr = *reinterpret_cast<int8_t**>(std::uintptr_t{0x0056CDA8});
    inline uint32_t& _num_maps_to_generate = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0056CDD0});
    inline void*& _name_seg = *reinterpret_cast<void**>(std::uintptr_t{0x0056CDB8});
    inline uint16_t*& _name_index = *reinterpret_cast<uint16_t**>(std::uintptr_t{0x0056CDA0});
    inline void*& _fill_msg_bitmap = *reinterpret_cast<void**>(std::uintptr_t{0x005A4B20});
    inline char (&_fill_msg)[36] = *reinterpret_cast<char (*)[36]>(std::uintptr_t{0x005A4B30});
    inline int16_t& _fill_msg_ctr = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A4B54});
    inline int8_t& _first_time_map_is_generated = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005589A4});
    inline uint8_t (&_special_count)[12] = *reinterpret_cast<uint8_t(*)[12]>(std::uintptr_t{0x0056CDBC});
    inline uint32_t& _n_stars = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0056CDB0});
    inline uint32_t& DAT_0056CDC8 = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0056CDC8});
    inline uint32_t (&_satellite_stat)[6] = *reinterpret_cast<uint32_t (*)[6]>(std::uintptr_t{0x005A4B00});
    inline uint32_t (&_nebula_stat)[20] = *reinterpret_cast<uint32_t (*)[20]>(std::uintptr_t{0x005A4AE0});
    inline uint16_t& _star_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0056CDB4});
    inline s_planet_data*& _pl = *reinterpret_cast<s_planet_data**>(std::uintptr_t{0x005A4B1C});
    inline const char* s_raceopt_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558B24}); // "RACEOPT.LBX"
    inline const char* s_starbg_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00558A10}); // "starbg.lbx"
    inline const char* s_Moving_planet_d_to_planet_d = 
        reinterpret_cast<const char*>(std::uintptr_t{0x005589AC}); // "Moving planet %d to planet %d"
    inline const char* s_Colony_Planets_d_Max_d = 
        reinterpret_cast<const char*>(std::uintptr_t{0x005589CC}); // "Colony Planets: %d Max: %d Plane..."

    void __cdecl Clear_Planets_In_Star_Structure_();

    int16_t __cdecl Gas_Giants_();

    int16_t __cdecl Home_Planet_(int16_t planet_id);

    int8_t __cdecl Generate_Spectral_Class_(int16_t star_type_index);
    uint8_t __cdecl Generate_Food_Per_Farmer_(int16_t star_index, int16_t planet_index);
    uint8_t __cdecl Generate_Max_Farms_(int16_t star_idx, int16_t planet_idx);
    int8_t __cdecl Generate_Orbit_(int16_t star_id);
    void __cdecl Generate_Satellite_(int16_t star_idx);
    uint8_t __cdecl Generate_Size_();

    int8_t __cdecl Generate_Mineral_Class_(int16_t star_index, int16_t planet_count);

    int8_t __cdecl Get_Climate_Modifier_(int8_t climate_index);

    int8_t __cdecl Generate_Gravity_Class_(int16_t planet_idx);

    int16_t __cdecl Get_Planet_Group_(int16_t planet_idx);

    inline void __cdecl Update_Planet_Group_Cache() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b56a0});
        fn();
    }

    uint8_t __cdecl Generate_Climate_(int16_t star_type);

    void __cdecl Universe_Generation_(uint8_t gal_size, uint8_t gal_age, int8_t nebula_freq);

    void __cdecl Randomize_Star_Names_(uint16_t* star_name_indices);

    inline void __cdecl Init_Main_Screen_(uint8_t size) {
        using Fn = void(__cdecl*)(uint8_t);
        reinterpret_cast<Fn>(0x004B6C40)(size);
    }
    
    void __cdecl Generate_Number_Of_Nebulas_(int16_t galaxy_size, int16_t nebula_freq);

    inline void __cdecl Generate_Nebulas_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004B5D40)();
    }
    bool __cdecl Set_Star_XYs_(int16_t x_cells, int16_t y_cells, int16_t param_3, int8_t generate_spectral, int8_t use_names);

    bool __cdecl Star_XY_Invalid_(int16_t star_idx, int16_t width, int16_t height, int16_t x_scaled);

    void __cdecl Copy_Name_To_Star_(int16_t star_idx);
    int16_t __cdecl Generate_Number_Of_Satellites_(int16_t star_idx);
    inline void __cdecl Planet_Generation_(int16_t star_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(0x004B81C0)(star_idx);
    }
    void __cdecl Generate_Star_Special_(int16_t star_idx, void* param_2, uint8_t* special_counts);
    void __cdecl Black_Hole_Fix_(int16_t gal_age);

    void __cdecl Enforce_Planet_Max_(int16_t max_planets);

    void __cdecl Set_Black_Hole_Visited_Flags_();

    void __cdecl Generate_Wormhole_Links_();

    void __cdecl Assign_Marooned_Heroes_();

    void __cdecl Set_Secondary_Planet_Data_(int16_t star_idx, int16_t planet_idx);

    void __cdecl Get_A_Planet_Or_Star_Special_(int16_t star_idx, int16_t* wormhole_out, int8_t* special_out, uint8_t commit_changes, int32_t excluded_mask);

    void __cdecl Load_Nebula_Pictures_();
}