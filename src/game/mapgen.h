#pragma once
#include <cstdint>

namespace MAPGEN {
    extern uint8_t _n_marooned_heroes;
    extern uint8_t _n_wormholes;
    extern uint8_t _make_food_planet;
    extern uint8_t _n_space_monsters;
    extern uint16_t _companion_star;
    extern uint8_t _n_maps_generated;
    extern uint16_t _tests_failed[4];
    extern int8_t* _evan_ptr;
    extern uint32_t _num_maps_to_generate;
    extern void* _name_seg;
    extern uint16_t* _name_index;
    extern s_animation_header* _fill_msg_bitmap;
    extern char _fill_msg[36];
    extern int16_t _fill_msg_ctr;
    extern int8_t _first_time_map_is_generated;
    extern uint8_t _special_count[SYSTEM_SPECIAL_COUNT];
    extern uint32_t _n_stars;
    extern uint32_t _n_planets;
    extern int16_t _t_roll;
    extern uint32_t _satellite_stat[6];
    extern uint32_t _nebula_stat[5];
    extern int16_t _star_count;
    extern s_planet_data* _pl;
    extern const char s_raceopt_lbx[12];
    extern const char s_starname_lbx[13];
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

    int8_t __cdecl Generate_Mineral_Class_(int16_t star_index, int16_t unused_planet_idx);

    int8_t __cdecl Get_Climate_Modifier_(int8_t climate_index);

    int8_t __cdecl Generate_Gravity_Class_(int16_t planet_idx);

    int16_t __cdecl Get_Planet_Group_(int16_t planet_idx);

    void __cdecl Update_Planet_Group_Cache();

    int8_t __cdecl Generate_Climate_(int16_t planet_group);

    /**
     * Generates a full galaxy map for the requested size, age, and nebula frequency.
     * @param gal_size Requested galaxy size preset.
     * @param gal_age Requested galaxy age preset.
     * @param nebula_freq Requested nebula frequency setting.
     */
    void __cdecl Universe_Generation_(uint8_t gal_size, uint8_t gal_age, int8_t nebula_freq);

    /** Shuffles the star-name index table in place. */
    void __cdecl Randomize_Star_Names_(uint16_t* star_name_indices);

    /** Initializes map bounds and zoom settings for the current galaxy size. */
    void __cdecl Init_Main_Screen_(uint8_t gal_size);
    
    void __cdecl Generate_Number_Of_Nebulas_(int16_t galaxy_size, int16_t unused_nebula_freq);

    void __cdecl Generate_Nebulas_();

    void __cdecl Build_Min_Point_Distances_(int16_t x, int16_t y, int16_t min_x, int16_t min_y, int16_t max_x, int16_t max_y, int32_t dist_sq, int16_t* out_points, int16_t* count, int16_t max_count);

    /**
     * Places stars into the galaxy grid and optionally assigns spectral classes and names.
     * @param x_cells Horizontal placement cell count.
     * @param y_cells Vertical placement cell count.
     * @param galaxy_age Spectral-generation column to use.
     * @param generate_spectral Non-zero to generate spectral classes.
     * @param use_names Non-zero to assign names during placement.
     */
    bool __cdecl Set_Star_XYs_(int16_t x_cells, int16_t y_cells, int16_t galaxy_age, int8_t generate_spectral, int8_t use_names);

    bool __cdecl Star_XY_Invalid_(int16_t star_idx, int16_t width, int16_t height, int16_t x_scaled);

    /** Copies the loaded name-table entry into the target star record. */
    void __cdecl Get_Star_Name_(int16_t star_idx);
    int16_t __cdecl Generate_Number_Of_Satellites_(int16_t star_idx);
    void __cdecl Planet_Generation_(int16_t star_idx);
    /** Assigns one generated special to a star system and updates summary counts. */
    void __cdecl Generate_Star_Special_(int16_t star_idx, int16_t* wormhole_out, uint8_t* special_counts);
    /** Re-rolls black holes that overlap nebulae into habitable stars. */
    void __cdecl Black_Hole_Fix_(int16_t galaxy_age);

    /** Removes excess non-home, non-gas-giant planets until the map reaches the requested cap. */
    void __cdecl Enforce_Planet_Max_(int16_t max_planets);

    void __cdecl Set_Black_Hole_Visited_Flags_();

    void __cdecl Generate_Wormhole_Links_();

    void __cdecl Assign_Marooned_Heroes_();

    /** Recomputes dependent farm, food, and colony fields for a generated planet. */
    void __cdecl Set_Secondary_Planet_Data_(int16_t star_idx, int16_t planet_idx);

    /**
     * Chooses an eligible planet or system special for a star.
     * @param star_idx Star to evaluate.
     * @param wormhole_out Set to non-zero when a wormhole special is chosen.
     * @param special_out Receives the chosen special id, or 0 on failure.
     * @param commit_changes Non-zero to write the special back into map state.
     * @param excluded_mask Bitmask of specials to skip during selection.
     */
    void __cdecl Get_A_Planet_Or_Star_Special_(int16_t star_idx, int16_t* wormhole_out, int8_t* special_out, uint8_t commit_changes, int32_t excluded_mask);

    /** Loads animation frames for each generated nebula type. */
    void __cdecl Load_Nebula_Pictures_();
}