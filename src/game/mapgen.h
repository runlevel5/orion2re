#pragma once
#include <cstdint>

namespace MAPGEN {
    extern uint8_t _n_marooned_heroes;
    extern uint8_t _n_wormholes;
    extern uint8_t _make_food_planet;
    extern uint8_t _n_space_monsters;
    extern uint16_t _companion_star_flag;
    extern const char _companion_star[2];
    extern uint8_t _n_maps_generated;
    extern uint16_t _tests_failed[4];
    extern int8_t* __evan_ptr;
    extern uint32_t _num_maps_to_generate;
    extern void* _name_seg;
    extern uint16_t* _name_index;
    extern s_animation_header* _fill_msg_bitmap;
    extern char _fill_msg[36];
    extern int16_t _fill_msg_ctr;
    extern int8_t _first_time_map_is_generated;
    extern uint8_t _special_count[12];
    extern uint32_t _n_stars;
    extern uint32_t _n_planets;
    extern int16_t _t_roll;
    extern uint32_t _satellite_stat[6];
    extern uint32_t _nebula_stat[5];
    extern uint16_t _star_count;
    extern s_planet_data* _pl;
    extern const char s_raceopt_lbx[12]; // "raceopt.lbx"
    extern const char s_starname_lbx[13]; // "starname.lbx"
    extern const char s_starbg_lbx[11]; // "starbg.lbx"
    extern const char s_Stat_d__start_d__diff_d__total_d_d[48]; // "Stat %d  (start: %d)  (diff %d)  (total: %d.%d)"
    extern const char s_Blank_Stat_Line[71]; // "                                                                      "
    extern const char s_Moving_planet_d_to_planet_d[30]; // "Moving planet %d to planet %d"
    extern const char s_Colony_Planets_d_Max_d[67]; // "Colony Planets: %d  Max: %d  Planet Count: %d  Removing %d planets"
    extern const char s_Star_d_orbit_d_has_a_0_sat_t_00558a1c[58]; // "Star %d, orbit %d, has a 0 sat type.  Roll: %d  Table: %d"
    extern uint8_t _MAPGEN_TRACE;

    void __cdecl Clear_Planets_In_Star_Structure_();

    int16_t __cdecl Gas_Giants_();

    int16_t __cdecl Home_Planet_(int16_t planet_id);

    int8_t __cdecl Generate_Spectral_Class_(int16_t star_type_index);
    int8_t __cdecl Generate_Food_Per_Farmer_(int16_t star_index, int16_t planet_index);
    uint8_t __cdecl Generate_Max_Farms_(int16_t star_idx, int16_t planet_idx);
    int8_t __cdecl Generate_Orbit_(int16_t star_id);
    uint8_t __cdecl Generate_Satellite_Type_(int16_t star_idx, int8_t orbit);
    void __cdecl Generate_Satellite_(int16_t star_idx, int16_t orbit, int16_t type);
    uint8_t __cdecl Generate_Size_();

    int8_t __cdecl Generate_Mineral_Class_(int16_t star_index, int16_t planet_count);

    int8_t __cdecl Get_Climate_Modifier_(int8_t climate_index);

    int8_t __cdecl Generate_Gravity_Class_(int16_t planet_idx);

    int16_t __cdecl Get_Planet_Group_(int16_t planet_idx);

    void __cdecl Update_Planet_Group_Cache();

    int8_t __cdecl Generate_Climate_(int16_t star_type);

    void __cdecl Universe_Generation_(uint8_t gal_size, uint8_t gal_age, int8_t nebula_freq);

    void __cdecl Randomize_Star_Names_(uint16_t* star_name_indices);

    void __cdecl Init_Main_Screen_(uint8_t gal_size);
    
    void __cdecl Generate_Number_Of_Nebulas_(int16_t galaxy_size, int16_t nebula_freq);

    void __cdecl Generate_Nebulas_();

    void __cdecl Build_Min_Point_Distances_(int16_t x, int16_t y, int16_t min_x, int16_t min_y, int16_t max_x, int16_t max_y, int32_t dist_sq, int16_t* out_points, int16_t* count, int16_t max_count);

    bool __cdecl Set_Star_XYs_(int16_t x_cells, int16_t y_cells, int16_t param_3, int8_t generate_spectral, int8_t use_names);

    bool __cdecl Star_XY_Invalid_(int16_t star_idx, int16_t width, int16_t height, int16_t x_scaled);

    void __cdecl Get_Star_Name_(int16_t star_idx);
    int16_t __cdecl Generate_Number_Of_Satellites_(int16_t star_idx);
    void __cdecl Planet_Generation_(int16_t star_idx);
    void __cdecl Generate_Star_Special_(int16_t star_idx, int16_t* wormhole_out, uint8_t* special_counts);
    void __cdecl Black_Hole_Fix_(int16_t gal_age);

    void __cdecl Enforce_Planet_Max_(int16_t max_planets);

    void __cdecl Set_Black_Hole_Visited_Flags_();

    void __cdecl Generate_Wormhole_Links_();

    void __cdecl Assign_Marooned_Heroes_();

    void __cdecl Set_Secondary_Planet_Data_(int16_t star_idx, int16_t planet_idx);

    void __cdecl Get_A_Planet_Or_Star_Special_(int16_t star_idx, int16_t* wormhole_out, int8_t* special_out, uint8_t commit_changes, int32_t excluded_mask);

    void __cdecl Load_Nebula_Pictures_();
}