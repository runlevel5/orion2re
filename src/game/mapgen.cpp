#include "pch.h"

namespace MAPGEN {
    int16_t _maximum_star_count = 99;
    uint8_t _n_marooned_heroes;
    uint8_t _n_wormholes;
    uint8_t _make_food_planet;
    uint8_t _n_space_monsters;
    uint16_t _companion_star;
    uint8_t _n_maps_generated;
    uint16_t _tests_failed[4];
    int8_t* _evan_ptr;
    uint32_t _num_maps_to_generate;
    void* _name_seg;
    uint16_t* _name_index;
    s_animation_header* _fill_msg_bitmap;
    char _fill_msg[36];
    int16_t _fill_msg_ctr;
    int8_t _first_time_map_is_generated = 1;
    uint8_t _special_count[SYSTEM_SPECIAL_COUNT];
    uint32_t _n_stars;
    uint32_t _n_planets;
    int16_t _t_roll;
    uint32_t _satellite_stat[6];
    uint32_t _nebula_stat[MAX_NEBULAS + 1];
    int16_t _star_count;
    s_planet_data* _pl;
    const char s_raceopt_lbx[12] = "raceopt.lbx";
    const char s_starname_lbx[13] = "starname.lbx";
    uint8_t _MAPGEN_TRACE = 1;

namespace {

constexpr int32_t STAR_NAME_COUNT = 829;

int16_t Generation_Progress_Interval_() {
    return MOX::_NUM_STARS <= ORIGINAL_MAX_STARS ? 1 : 16;
}

bool Should_Update_Generation_Progress_(int16_t star_idx) {
    return star_idx % Generation_Progress_Interval_() == 0;
}

int16_t Generation_Progress_Max_() {
    int16_t interval = Generation_Progress_Interval_();
    return (int16_t)(((MOX::_NUM_STARS + interval - 1) / interval) * 2);
}

int16_t Maximum_Galaxy_Grid_Y_() {
    int16_t grid_y = 8;
    while ((((grid_y * 5) + 3) / 4) * grid_y < _maximum_star_count) {
        ++grid_y;
    }
    return grid_y;
}

int16_t Maximum_Galaxy_Grid_X_() {
    return (int16_t)(((Maximum_Galaxy_Grid_Y_() * 5) + 3) / 4);
}

constexpr int16_t MAXIMUM_GALAXY_MAP_SCALE = 30;
constexpr int16_t MAXIMUM_GALAXY_CELL_SIZE = 50;

int16_t Maximum_Galaxy_Display_Scale_() {
    const int32_t raw_cell_size = (MAXIMUM_GALAXY_CELL_SIZE * MAXIMUM_GALAXY_MAP_SCALE) / 10;
    const int32_t map_width = Maximum_Galaxy_Grid_X_() * raw_cell_size;
    const int32_t map_height = Maximum_Galaxy_Grid_Y_() * raw_cell_size;
    const int32_t scale_x = (map_width * 10 + 0x1FA - 1) / 0x1FA;
    const int32_t scale_y = (map_height * 10 + 0x190 - 1) / 0x190;
    return (int16_t)((scale_x > scale_y) ? scale_x : scale_y);
}

} // namespace

bool __cdecl Set_Star_XYs_(int16_t x_cells, int16_t y_cells, int16_t galaxy_age, int8_t generate_spectral, int8_t use_names) {
    int16_t x_cell_size = MOX::_MAP_MAX_X / x_cells;
    int16_t y_cell_size = MOX::_MAP_MAX_Y / y_cells;
    int16_t scaled_max_x = HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_X);
    int16_t scaled_max_y = HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_Y);
    int8_t y_collision_padding = 0;

    s_colors font_colors_main;
    s_colors* font_colors_ptr = MISC::Font_Colors2_(4, 'w', 'y');
    font_colors_main = *font_colors_ptr;

    s_colors font_colors_alt;
    font_colors_ptr = MISC::Font_Colors2_(4, 'w', '|');
    font_colors_alt = *font_colors_ptr;

    HAROLD::Set_Player_Font_Style_(0, 0);
    HAROLD::Map_Scale_To_Zoom_Level_();

    int16_t collision_h = 30;
    y_collision_padding = 4;
    switch (MOX::_cur_map_scale) {
    case 10:
    case 15:
        collision_h = 50;
        y_collision_padding = 8;
        break;
    case 20:
        collision_h = 36;
        y_collision_padding = 6;
        break;
    case 30:
        break;
    }

    if (MOX::_NUM_STARS <= 0) return false;

    for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
        MAPGEN::_star_count = i;
        bool update_progress = (generate_spectral || use_names) && Should_Update_Generation_Progress_(i);
        if (update_progress) {
            RUSS::Mox_Update_();
        }

        if (MAPGEN::_num_maps_to_generate == 1 && update_progress) {
            fonts::Set_Font_Style_(4, &font_colors_main);
            int16_t str_w = fonts::Get_String_Width_(MAPGEN::_fill_msg);
            MISC::Draw_Advancing_Fill_Message_(4, &MAPGEN::_fill_msg_ctr, Generation_Progress_Max_(), &font_colors_main,
                &font_colors_alt, (0x280 - str_w) / 2, 0xD7,
                MAPGEN::_fill_msg, MAPGEN::_fill_msg_bitmap, 0);
            RUSS::Mox_Sync_Update_();
        }

        if (generate_spectral) {
            MOX::_star[i].spectral_class = MAPGEN::Generate_Spectral_Class_(galaxy_age);
        }

        int16_t collision_w;
        if (use_names) {
            MAPGEN::Get_Star_Name_(i);
            HAROLD::Set_Player_Font_Style_(0, 0);
            char formatted_name[32];
            snprintf(formatted_name, sizeof(formatted_name), HAROLD::s___s__00556ae4, MOX::_star[i].name);

            // Manual check for short names to append "w"
            uint32_t name_len = 0;
            while (MOX::_star[i].name[name_len] != '\0') name_len++;

            if (name_len < 7) {
                strlcat(formatted_name, "w", sizeof(formatted_name));
            }

            collision_w = fonts::Get_String_Width_(formatted_name);
            if (MAPGEN::_n_maps_generated <= 2) goto START_POS_GEN;
        }

        collision_w = (int16_t)((int16_t)game_random::Random_(8) +
                                (int16_t)game_random::Random_(8) +
                                0x20 +
                                (int16_t)game_random::Random_(8));

    START_POS_GEN:
        int16_t retry_count = 0;
        HAROLD::Set_Player_Font_Style_(0, 0);
        HAROLD::Map_Scale_To_Zoom_Level_();

        bool collision;
        do {
            int16_t rx, scaled_x;
            do {
                rx = (int16_t)(game_random::Random_(x_cell_size * 2) + (i % x_cells) * x_cell_size) % MOX::_MAP_MAX_X;
                scaled_x = HAROLD::Get_Scaled_Value_(rx);
            } while (scaled_x < 0x18 || (scaled_max_x - (MOX::_star_width / 2)) < scaled_x);
            MOX::_star[i].x = rx;

            int16_t ry, scaled_y;
            do {
                ry = (int16_t)(game_random::Random_(y_cell_size * 2) + (y_cell_size * i) / x_cells) % MOX::_MAP_MAX_Y;
                scaled_y = HAROLD::Get_Scaled_Value_(ry);
            } while (scaled_y < 0x16 || ((scaled_max_y - (MOX::_star_height / 2)) - y_collision_padding) < scaled_y);
            MOX::_star[i].y = ry;

            collision = MAPGEN::Star_XY_Invalid_(i, collision_w, collision_h, scaled_x);
            if (!collision) break;

            if (++retry_count > 0x96) {
                bool fallback_found = false;
                int32_t fallback_min_x = 0x18;
                int32_t fallback_min_y = 0x16;
                int32_t fallback_range_x = scaled_max_x - (MOX::_star_width / 2) - fallback_min_x + 1;
                int32_t fallback_range_y = scaled_max_y - (MOX::_star_height / 2) - y_collision_padding - fallback_min_y + 1;
                int32_t fallback_attempt_limit = std::max<int32_t>(0x96, MOX::_NUM_STARS * 8);

                for (int32_t fallback_attempt = 0; fallback_attempt < fallback_attempt_limit; ++fallback_attempt) {
                    int32_t fx = game_random::Random_(fallback_range_x) + fallback_min_x - 1;
                    int32_t fy = game_random::Random_(fallback_range_y) + fallback_min_y - 1;
                    MOX::_star[i].x = (int16_t)HAROLD::Get_Up_Scaled_Value_((int16_t)fx);
                    MOX::_star[i].y = (int16_t)HAROLD::Get_Up_Scaled_Value_((int16_t)fy);

                    if (!MAPGEN::Star_XY_Invalid_(i, collision_w, collision_h, (int16_t)fx)) {
                        fallback_found = true;
                        break;
                    }
                }

                if (!fallback_found) return true;
                collision = false;
            }
        } while (collision);
    }
    return false;
}

    void __cdecl Clear_Planets_In_Star_Structure_() {
        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            for (int j = 0; j < 5; ++j) {
                MOX::_star[i].planet_index[j] = -1;
            }
        }
    }

    int8_t __cdecl Generate_Spectral_Class_(int16_t star_type_index) {
        uint8_t weights[STAR_CLASS_COUNT];

        for (int32_t i = 0; i < STAR_CLASS_COUNT; ++i) {
            weights[i] = MOX::_star_class_table[i][star_type_index];
        }

        return (int8_t)RUSS::Get_Weighted_Choice_Char_(weights, STAR_CLASS_COUNT);
    }

    int8_t __cdecl Generate_Non_Black_Hole_Spectral_Class_(int16_t star_type_index) {
        for (int16_t attempt = 0; attempt < 200; ++attempt) {
            int8_t spectral_class = Generate_Spectral_Class_(star_type_index);
            if (spectral_class < STAR_CLASS_BLACK_HOLE) {
                return spectral_class;
            }
        }

        return STAR_CLASS_M;
    }

    /**
     * @brief Determines the base food production per farmer for a given planet.
     *
     * Planets in systems with non-habitable spectral classes produce no food
     * unless the system contains the Orion star special.
     */
    int8_t __cdecl Generate_Food_Per_Farmer_(int16_t star_index, int16_t planet_index) {
        bool habitable = HAROLD::Habitable_Spectral_Class_(star_index);

        // Check if the system is either habitable or is the Orion system
        if (!habitable && MOX::_star[star_index].system_special != SYSTEM_SPECIAL_ORION_STAR) {
            return 0;
        }

        int8_t climate_idx = MOX::_planet[planet_index].climate;

        return MOX::_food_per_farmer_table[climate_idx];
    }

    uint8_t __cdecl Generate_Max_Farms_(int16_t star_idx, int16_t planet_idx) {
        bool is_habitable = HAROLD::Habitable_Spectral_Class_(star_idx);

        if (!is_habitable && MOX::_star[star_idx].system_special != SYSTEM_SPECIAL_ORION_STAR) {
            return 0;
        }

        int8_t size_idx = MOX::_planet[planet_idx].size;
        return MOX::_planet_max_farms[size_idx];
    }

    int8_t __cdecl Generate_Orbit_(int16_t star_id) {
        uint8_t weights[5];

        static const uint8_t weight_table[15] = {
            25, 20, 10,
            18, 20, 22,
            17, 20, 30,
            15, 20, 33,
            25, 20, 5
        };

        for (int i = 0; i < 5; ++i) {
            weights[i] = weight_table[i * GALAXY_AGE_COUNT + MOX::_settings.galaxy_age];
        }

        int16_t choice;
        bool found = false;

        do {
            choice = RUSS::Get_Weighted_Choice_Char_(weights, 5);
            if (MOX::_star[star_id].planet_index[choice] == -1) {
                found = true;
            }
        } while (!found);

        return (int8_t)choice;
    }

    uint8_t __cdecl Generate_Size_() {
        int32_t rand_val = game_random::Random_(10);

        for (int16_t i = 0; i < PLANET_SIZE_COUNT; ++i) {
            int16_t threshold = (int16_t)(int8_t)MOX::_planet_size_table[i];

            if (threshold >= (int16_t)rand_val) {
                return (uint8_t)i;
            }
        }

        return 0xFF;
    }

    int8_t __cdecl Generate_Mineral_Class_(int16_t star_idx, int16_t unused_planet_idx) {
        (void)unused_planet_idx;
        int32_t random_roll = game_random::Random_(10);
        int8_t spectral_class = MOX::_star[star_idx].spectral_class;

        if (spectral_class >= STAR_CLASS_B && spectral_class < STAR_CLASS_BLACK_HOLE) {
            int32_t table_index = (random_roll - 1) * 6 + (int32_t)spectral_class;
            return (int8_t)MOX::_class_to_mineral[table_index];
        }

        return -1;
    }

    /**
     * @brief Generates the type of satellite (planet/gas giant/etc.) for a given orbit.
     *
     * @param star_idx The index of the star in the stars array.
     * @param orbit The orbit index (0 being closest to the star).
     * @return The generated satellite type identifier.
     */
    uint8_t __cdecl Generate_Satellite_Type_(int16_t star_idx, int8_t orbit) {
        uint8_t satellite_type;
        int16_t roll;

        do {
            // Randomly roll for a satellite type based on weightings in the data table
            roll = (int16_t)(game_random::Random_(10) - 1);
            MAPGEN::_t_roll = roll;

            satellite_type = MOX::_orbit_to_satellite_type[roll][orbit];

            if (satellite_type == MAPGEN_SATELLITE_COMPANION_STAR) {
                int chance = game_random::Random_(100);

                // If the roll is specific and it's the inner-most orbit, try to spawn a binary companion
                if (roll == 1 && chance < 11 && orbit == 0) {
                    MAPGEN::_companion_star = 1;
                    int8_t spectral = MOX::_star[star_idx].spectral_class;

                    if (spectral == STAR_CLASS_B) {
                        satellite_type = 5;
                    } else {
                        satellite_type = (uint8_t)(spectral + 4);
                    }
                } else {
                    satellite_type = MAPGEN_SATELLITE_ASTEROID;
                }
            }

        } while (satellite_type == MAPGEN_SATELLITE_GAS_GIANT && orbit == 0);

        if (satellite_type == MAPGEN_SATELLITE_INVALID) {
            int table_value = (int)(int8_t)MOX::_orbit_to_satellite_type[MAPGEN::_t_roll][orbit];
            RUSS::PFMT_(0, 0x0F, "Star %d, orbit %d, has a 0 sat type.  Roll: %d  Table: %d",
                        star_idx,
                        (int)orbit,
                        (int)MAPGEN::_t_roll,
                        table_value);
            key::Wait_For_Key_();
        }

        return satellite_type;
    }

    /**
     * @brief Generates initial data for a satellite (moon) in a star system.
     */
    void __cdecl Generate_Satellite_(int16_t star_idx, int16_t orbit, int16_t type) {
        s_planet_data* planet = &MOX::_planet[MOX::_planet_count];

        planet->star_index = star_idx;

        planet->size = Generate_Size_();

        planet->mineral_class = Generate_Mineral_Class_(star_idx, MOX::_planet_count);

        Get_Climate_Modifier_(planet->mineral_class);

        planet->gravity_class = Generate_Gravity_Class_(MOX::_planet_count);

        planet->group = (int8_t)Get_Planet_Group_(MOX::_planet_count);

        planet->climate = Generate_Climate_(planet->group);

        planet->climate_bg_type = (int8_t)(game_random::Random_(3) - 1);

        Set_Secondary_Planet_Data_(star_idx, MOX::_planet_count);
    }

    void __cdecl Set_Secondary_Planet_Data_(int16_t star_idx, int16_t planet_idx) {
        s_planet_data* planet = &MOX::_planet[planet_idx];

        planet->max_farms = Generate_Max_Farms_(star_idx, planet_idx);
        planet->food_per_farmer = Generate_Food_Per_Farmer_(star_idx, planet_idx);
        planet->colony_index = -1;
    }

    int8_t __cdecl Get_Climate_Modifier_(int8_t climate_index) {
        int8_t modifier = 0;

        if (climate_index >= MINERAL_RESOURCE_ULTRA_POOR && climate_index < MINERAL_RESOURCE_COUNT) {
            modifier = MOX::_climate_modifier_table[climate_index];

            if (modifier > 0) {
                RUSS::PFMT_(0, 0, "------CLIMATE modifier is %d  Planet: %d  Mineral: %d",
                    modifier,
                    MOX::_planet_count,
                    climate_index);
            }
        }

        return modifier;
    }

    int8_t __cdecl Generate_Gravity_Class_(int16_t planet_idx) {
        s_planet_data& planet = MOX::_planet[planet_idx];

        int16_t mineral_class = planet.mineral_class;

        if (mineral_class >= MINERAL_RESOURCE_ULTRA_POOR && mineral_class < MINERAL_RESOURCE_COUNT) {
            int16_t size = planet.size;
            return MOX::_gravity_table[mineral_class * PLANET_SIZE_COUNT + size];
        }

        return PLANET_GRAVITY_LOW_G;
    }

    void __cdecl Update_Planet_Group_Cache() {
        return;
    }

    int16_t __cdecl Get_Planet_Group_(int16_t planet_index) {
        s_planet_data& planet = MOX::_planet[planet_index];
        int16_t orbit = (int16_t)planet.orbit;
        int16_t star_index = planet.star_index;
        int16_t spectral_class = (int16_t)MOX::_star[star_index].spectral_class;

        if (_companion_star == 1) {
            orbit -= 2;
            if (orbit < 0) {
                return PLANET_GROUP_BOMBARDED;
            }
        }

        if (spectral_class < STAR_CLASS_BLACK_HOLE) {
            int8_t group = MOX::_class_to_group[spectral_class].group[orbit];

            Update_Planet_Group_Cache();

            return (int16_t)group;
        }

        return PLANET_GROUP_BOMBARDED;
    }

    /**
     * Generates a planetary climate based on star type and galaxy age.
     * If the galaxy is young or normal, it uses one weight table; if old, another.
     * If forced to be a food planet, it ensures the chosen climate supports agriculture.
     */
    int8_t __cdecl Generate_Climate_(int16_t planet_group) {
        uint8_t weights[PLANET_CLIMATE_COUNT];
        memset(&weights, 0, sizeof(weights));

        // 1. Determine weights based on planet group and galaxy age
        for (int i = 0; i < PLANET_CLIMATE_COUNT; ++i) {
            if (MOX::_g_galaxy_age <= GALAXY_AGE_AVERAGE) {
                weights[i] = MOX::_normal_gal_climate_roll_table[i][planet_group];
            }
            else if (MOX::_g_galaxy_age == GALAXY_AGE_OLD) {
                weights[i] = MOX::_old_gal_climate_roll_table[i][planet_group];
            }
        }

        int16_t result = -1; // Symbolic: NO_CLIMATE_SELECTED

        // 2. Specialized handling for food-producing planets
        if (_make_food_planet != 0) {
            int16_t match_count = 0;
            uint8_t match_index = 0;
            for (int i = 0; i < PLANET_CLIMATE_COUNT; ++i) {
                // Check if climate has a non-zero weight and supports food production
                if (weights[i] != 0 && (int8_t)MOX::_food_per_farmer_table[i] > 0) {
                    match_count++;
                    match_index = (uint8_t)i;
                }
            }

            // If only one agriculture-capable climate is valid for this star, select it immediately
            if (match_count == 1) {
                result = match_index;
            }
        }

        // 3. Perform weighted selection if no unique result was forced
        if (result == -1) {
            if (_make_food_planet == 0) {
                result = RUSS::Get_Weighted_Choice_Char_(weights, PLANET_CLIMATE_COUNT);
            }
            else {
                // If forced to be food planet but multiple options exist, loop until a valid choice is picked
                do {
                    result = RUSS::Get_Weighted_Choice_Char_(weights, PLANET_CLIMATE_COUNT);
                } while (MOX::_food_per_farmer_table[result] == 0);
            }
        }

        return (int8_t)result;
    }

    void __cdecl Enforce_Planet_Max_(int16_t max_planets) {
        int16_t original_planet_count = MOX::_planet_count;
        int16_t gas_giant_count = Gas_Giants_();

        if (max_planets <= original_planet_count - gas_giant_count) {
            uint8_t is_home_system[MAX_STARS];
            memset(is_home_system, 0, sizeof(is_home_system));

            for (int32_t i = 0; i < MAX_PLAYERS; ++i) {
                if (i < MOX::_NUM_PLAYERS) {
                    int16_t home_pid = MOX::_player[i].home_planet_id;
                    is_home_system[MOX::_planet[home_pid].star_index] = 1;
                }
            }

            if (capture::Check_Release_Version_() == 0) {
                RUSS::PFMT_(0, 1, "Colony Planets: %d  Max: %d  Planet Count: %d  Removing %d planets",
                    (int32_t)(original_planet_count - gas_giant_count),
                    (int32_t)max_planets,
                    (int32_t)original_planet_count,
                    (int32_t)((original_planet_count - gas_giant_count) - max_planets));
                key::Wait_For_Key_();
            }

            _pl = (s_planet_data*)allocate::Allocate_First_Block_(MOX::_screen_seg, MAX_PLANETS * sizeof(*MOX::_planet));
            memcpy(_pl, MOX::_planet, MAX_PLANETS * sizeof(*MOX::_planet));

            s_star_data* star_backup = (s_star_data*)allocate::Allocate_Next_Block_(MOX::_screen_seg, MAX_STARS * sizeof(*MOX::_star));
            memcpy(star_backup, MOX::_star, MAX_STARS * sizeof(*MOX::_star));

            int16_t* removable_planets = (int16_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, original_planet_count * sizeof(*removable_planets));
            int16_t removable_planet_count = 0;
            for (int16_t planet_idx = 0; planet_idx < original_planet_count; ++planet_idx) {
                s_planet_data* backup_p = &_pl[planet_idx];
                if (is_home_system[backup_p->star_index] == 0 &&
                    !HOMEGEN::Is_Added_Food_World_(backup_p->star_index) &&
                    backup_p->planet_type != PLANET_TYPE_GAS_GIANT) {
                    removable_planets[removable_planet_count++] = planet_idx;
                }
            }

            while (max_planets <= MOX::_planet_count - gas_giant_count) {
                if (removable_planet_count == 0) break;

                int16_t choice = (int16_t)(game_random::Random_(removable_planet_count) - 1);
                int16_t planet_idx = removable_planets[choice];
                removable_planets[choice] = removable_planets[--removable_planet_count];
                _pl[planet_idx].colony_index = -2;
                MOX::_planet_count--;
            }

            int16_t new_idx = 0;
            Clear_Planets_In_Star_Structure_();

            for (int16_t old_idx = 0; old_idx < original_planet_count; ++old_idx) {
                if (_pl[old_idx].colony_index != -2) {
                    int16_t player_home_idx = Home_Planet_(old_idx);
                    if (player_home_idx != -1) {
                        MOX::_player[player_home_idx].home_planet_id = new_idx;
                    }

                    RUSS::PFMT_(0, 10, "Moving planet %d to planet %d", old_idx, new_idx);

                    MOX::_planet[new_idx] = _pl[old_idx];
                    int16_t colony_idx = MOX::_planet[new_idx].colony_index;
                    if (colony_idx >= 0) {
                        MOX::_colony[colony_idx].planet = new_idx;
                    }
                    MOX::_star[MOX::_planet[new_idx].star_index].planet_index[(uint8_t)MOX::_planet[new_idx].orbit] = new_idx;

                    new_idx++;
                }
            }
        }
    }

    void __cdecl Set_Black_Hole_Visited_Flags_() {
        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            if (MOX::_star[i].spectral_class == STAR_CLASS_BLACK_HOLE) {
                MOX::_star[i].visited = -1;
            }
        }
    }

    void __cdecl Generate_Wormhole_Links_() {
        uint8_t star_used[MAX_STARS];
        memset(star_used, 0, sizeof(star_used));

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                int16_t home_planet_id = MOX::_player[i].home_planet_id;
                int16_t star_index = MOX::_planet[home_planet_id].star_index;
                star_used[star_index] = 1;
            }
        }

        if (_n_wormholes != 0) {
            for (int16_t wormhole_idx = 0; wormhole_idx < (int16_t)_n_wormholes; ++wormhole_idx) {
                int16_t found_star_idx = -1;
                for (int attempt = 0; attempt <= 200; ++attempt) {
                    int16_t star_idx = game_random::Random_(MOX::_NUM_STARS - 1);
                    if (star_idx != 0 &&
                        star_used[star_idx] == 0 &&
                        MOX::_star[star_idx].system_special == SYSTEM_SPECIAL_NO_SPECIAL &&
                        !HAROLD::Empty_System_(star_idx) &&
                        MOX::_star[star_idx].spectral_class != STAR_CLASS_BLACK_HOLE &&
                        MOX::_star[star_idx].wormhole_star_id == -1)
                    {
                        found_star_idx = star_idx;
                        break;
                    }
                }

                if (found_star_idx != -1) {
                    HAROLD::Set_Wormhole_Id_(found_star_idx, star_used);
                }
            }
        }
    }

    void __cdecl Get_A_Planet_Or_Star_Special_(int16_t star_idx, int16_t* wormhole_out, int8_t* special_out, uint8_t commit_changes, int32_t excluded_mask) {
        uint16_t valid_planets[5] = { 0 };
        uint16_t large_planets[5] = { 0 };
        uint16_t habitable_planets[5] = { 0 };
        bool has_large = false;
        bool has_habitable = false;

        for (int i = 0; i < 5; ++i) {
            int16_t p_idx = MOX::_star[star_idx].planet_index[i];
            if (p_idx >= 0) {
                s_planet_data* planet = &MOX::_planet[p_idx];
                if (planet->planet_type == PLANET_TYPE_PLANET && planet->planet_special == SYSTEM_SPECIAL_NO_SPECIAL) {
                    valid_planets[i] = 1;
                    int8_t climate = planet->climate;
                    // Habitable climates: Desert, Arid, Tundra, Ocean, Swamp, Terran, Gaia
                    if (climate == PLANET_CLIMATE_DESERT || climate == PLANET_CLIMATE_ARID ||
                        climate == PLANET_CLIMATE_TUNDRA || climate == PLANET_CLIMATE_OCEAN ||
                        climate == PLANET_CLIMATE_SWAMP || climate == PLANET_CLIMATE_TERRAN ||
                        climate == PLANET_CLIMATE_GAIA) {
                        habitable_planets[i] = 1;
                        has_habitable = true;
                        if (planet->size >= PLANET_SIZE_MEDIUM) { // Medium or larger
                            large_planets[i] = 1;
                            has_large = true;
                        }
                    }
                }
            }
        }

        int16_t tries = 0;
        uint16_t system_weights[SYSTEM_SPECIAL_COUNT];
        system_weights[SYSTEM_SPECIAL_NO_SPECIAL] = 0;

        while (true) {
            for (int s = SYSTEM_SPECIAL_STABLE_WORMHOLE; s < SYSTEM_SPECIAL_COUNT; ++s) {
                if ((excluded_mask >> s) & 1) {
                    system_weights[s] = 0;
                }
                else if (s == SYSTEM_SPECIAL_MAROONED_HERO) { // Marooned Hero
                    system_weights[s] = (MOX::_zoom_count + 2 >= _n_marooned_heroes) ?
                        MOX::_planet_special_weighted_chance[s] : 0;
                }
                else if (s == SYSTEM_SPECIAL_NATIVES || s == SYSTEM_SPECIAL_SPLINTER_COLONY) { // Natives, Splinter Colony
                    system_weights[s] = (has_large) ?
                        MOX::_planet_special_weighted_chance[s] : 0;
                }
                else if (s == SYSTEM_SPECIAL_ANCIENT_ARTIFACTS) { // Ancient Artifacts
                    system_weights[s] = (has_habitable) ?
                        MOX::_planet_special_weighted_chance[s] : 0;
                }
                else if (s == SYSTEM_SPECIAL_SPACE_MONSTER) { // Space Monster
                    system_weights[s] = (MOX2::_user_wants_n_space_monsters) ?
                        MOX::_planet_special_weighted_chance[s] : 0;
                }
                else {
                    system_weights[s] = MOX::_planet_special_weighted_chance[s];
                }
            }

            int16_t choice = RUSS::Get_Weighted_Choice_Short_(system_weights, SYSTEM_SPECIAL_COUNT);
            *special_out = (int8_t)choice;
            bool success = false;

            if (choice >= SYSTEM_SPECIAL_STABLE_WORMHOLE && choice <= SYSTEM_SPECIAL_ANCIENT_ARTIFACTS) {
                switch (choice) {
                case SYSTEM_SPECIAL_STABLE_WORMHOLE:
                    if (commit_changes && _n_wormholes < (MOX::_settings.galaxy_size * 4 + 4)) {
                        *wormhole_out = 1;
                        _n_wormholes++;
                        success = true;
                    }
                    break;

                case SYSTEM_SPECIAL_SPACE_DEBRIS:
                case SYSTEM_SPECIAL_PIRATE_CACHE:
                case SYSTEM_SPECIAL_SPACE_MONSTER:
                    if (commit_changes) {
                        MOX::_star[star_idx].system_special = (int8_t)choice;
                    }
                    success = true;
                    break;

                case SYSTEM_SPECIAL_GOLD_DEPOSITS:
                case SYSTEM_SPECIAL_GEM_DEPOSITS:
                {
                    int16_t p_idx = RUSS::Get_Weighted_Choice_Short_(valid_planets, 5);
                    if (p_idx >= 0) {
                        if (commit_changes) {
                            int16_t actual_p_idx = MOX::_star[star_idx].planet_index[p_idx];
                            MOX::_planet[actual_p_idx].planet_special = (int8_t)choice;
                            MOX::_star[star_idx].system_special = (int8_t)choice;
                        }
                        success = true;
                    }
                    break;
                }

                case SYSTEM_SPECIAL_NATIVES:
                case SYSTEM_SPECIAL_SPLINTER_COLONY:
                {
                    int16_t p_idx = RUSS::Get_Weighted_Choice_Short_(large_planets, 5);
                    if (p_idx >= 0) {
                        if (commit_changes) {
                            int16_t actual_p_idx = MOX::_star[star_idx].planet_index[p_idx];
                            MOX::_planet[actual_p_idx].planet_special = (int8_t)choice;
                            MOX::_star[star_idx].system_special = (int8_t)choice;
                        }
                        success = true;
                    }
                    break;
                }

                case SYSTEM_SPECIAL_MAROONED_HERO:
                    _n_marooned_heroes++; // Increment global attempt counter
                    if (commit_changes) {
                        MOX::_star[star_idx].system_special = (int8_t)choice;
                    }
                    success = true;
                    break;

                case SYSTEM_SPECIAL_ANCIENT_ARTIFACTS:
                {
                    int16_t p_idx = RUSS::Get_Weighted_Choice_Short_(habitable_planets, 5);
                    if (p_idx >= 0) {
                        if (commit_changes) {
                            int16_t actual_p_idx = MOX::_star[star_idx].planet_index[p_idx];
                            MOX::_planet[actual_p_idx].planet_special = (int8_t)choice;
                            MOX::_star[star_idx].system_special = (int8_t)choice;
                        }
                        success = true;
                    }
                    break;
                }
                }
            }

            if (success) break;

            tries++;
            if (tries > 100) {
                *special_out = SYSTEM_SPECIAL_NO_SPECIAL;
                break;
            }
        }
    }

    void __cdecl Generate_Star_Special_(int16_t star_idx, int16_t* wormhole_out, uint8_t* special_counts) {
        int8_t special_type = SYSTEM_SPECIAL_NO_SPECIAL;

        if ((int16_t)MOX::_planet_special < (int16_t)game_random::Random_(100)) {
            Get_A_Planet_Or_Star_Special_(star_idx, wormhole_out, &special_type, 1, 0);
        } else {
            special_type = SYSTEM_SPECIAL_NO_SPECIAL;
        }

        int16_t monster_chance;
        if (MOX::_settings.game_difficulty == GAME_DIFFICULTY_HARD) {
            monster_chance = 8;
        } else if (MOX::_settings.game_difficulty == GAME_DIFFICULTY_IMPOSSIBLE) {
            monster_chance = 12;
        } else {
            monster_chance = 5;
        }

        if ((int32_t)game_random::Random_(100) - 1 < (int32_t)monster_chance) {
            _n_space_monsters++;
        }

        special_counts[special_type]++;
    }

    void __cdecl Assign_Marooned_Heroes_() {
        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            if (MOX::_star[star_idx].system_special == SYSTEM_SPECIAL_MAROONED_HERO) {
                int16_t attempts = 0;
                bool stop_searching = false;
                int16_t selected_hero_idx = -1;

                do {
                    int16_t hero_candidate = (int16_t)game_random::Random_(38) - 1;

                    if (hero_candidate == 0x41 || MOX::_leaders[hero_candidate].status != -1) {
                        attempts++;
                    }
                    else {
                        selected_hero_idx = hero_candidate;
                        stop_searching = true;
                    }

                    if (attempts == 100) {
                        stop_searching = true;
                    }
                } while (!stop_searching);

                if (selected_hero_idx == -1) {
                    MOX::_star[star_idx].system_special = SYSTEM_SPECIAL_NO_SPECIAL;
                }
                else {
                    MOX::_leaders[selected_hero_idx].status = 5;
                    MOX::_leaders[selected_hero_idx].location = star_idx;
                }
            }
        }
    }

    void __cdecl Generate_Number_Of_Nebulas_(int16_t galaxy_size, int16_t unused_nebula_freq) {
        (void)unused_nebula_freq;
        MOX::_NUM_NEBULAS = 0;

        if (MOX::_nebula_screen_seg == nullptr) {
            return;
        }

        switch (galaxy_size) {
        case GALAXY_SIZE_SMALL:
            MOX::_NUM_NEBULAS = (uint8_t)(game_random::Random_(2) - 1);
            break;
        case GALAXY_SIZE_MEDIUM:
            MOX::_NUM_NEBULAS = (uint8_t)game_random::Random_(2);
            break;
        case GALAXY_SIZE_LARGE:
            MOX::_NUM_NEBULAS = (uint8_t)game_random::Random_(3);
            break;
        case GALAXY_SIZE_HUGE:
            MOX::_NUM_NEBULAS = (uint8_t)(game_random::Random_(3) + 1);
            break;
#if MAX_STARS > ORIGINAL_MAX_STARS
        case GALAXY_SIZE_MAXIMUM:
            MOX::_NUM_NEBULAS = (uint8_t)(game_random::Random_(3) + 1);
            MOX::_NUM_NEBULAS = (uint8_t)std::min<int32_t>(
                MAX_NEBULAS,
                ((int32_t)MOX::_NUM_NEBULAS * MOX::_NUM_STARS + ORIGINAL_MAX_STARS * 2 - 1) /
                    (ORIGINAL_MAX_STARS * 2));
            break;
#endif
        default:
            break;
        }
    }

    void __cdecl Generate_Nebulas_() {
        int16_t widths[12];
        int16_t heights[12];
        uint8_t nebula_types[MAX_NEBULAS];
        int16_t min_dist_param = 0x14;
        int16_t attempts = 0;
        const int32_t point_buffer_value_count = 30000 * 2;
        const int32_t coord_buffer_value_count = 10000 * 2;
        const int32_t weight_buffer_value_count = 10000;

        allocate::Push_Block_(MOX::_screen_seg);
        int16_t* point_buffer = (int16_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, (int32_t)(sizeof(*point_buffer) * point_buffer_value_count));
        int16_t* coord_buffer = (int16_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, (int32_t)(sizeof(*coord_buffer) * coord_buffer_value_count));
        uint16_t* weight_buffer = (uint16_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, (int32_t)(sizeof(*weight_buffer) * weight_buffer_value_count));

        RUSS::Mox_Update_();

        for (int i = 0; i < 12; ++i) {
            s_animation_header* anim = (s_animation_header*)farload::Far_Reload_("starbg.lbx", i * 4 + 6, MOX::_nebula_screen_seg);
            widths[i] = animate::Get_Width_(anim);
            heights[i] = animate::Get_Height_(anim);
        }

        if (MOX::_NUM_NEBULAS != 0) {
            for (int16_t i = 0; i < (int16_t)(uint16_t)MOX::_NUM_NEBULAS; ++i) {
                RUSS::Mox_Update_();
                // The original reads an uninitialized stack byte array before accepting
                // a type. Use a direct random choice here instead of encoding UB.
                nebula_types[i] = (uint8_t)((int16_t)game_random::Random_(12) - 1);
                RUSS::Mox_Update_();
            }
        }

        if (MOX::_NUM_STARS > ORIGINAL_MAX_STARS) {
            int16_t scaled_map_max_x = HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_X);
            int16_t scaled_map_max_y = HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_Y);
            int16_t nebula_width = HAROLD::Get_Scaled_Value_(0xA5);
            int16_t nebula_height = HAROLD::Get_Scaled_Value_(0x82);
            int16_t min_x = 10;
            int16_t min_y = 10;
            int16_t max_x = scaled_map_max_x - nebula_width;
            int16_t max_y = scaled_map_max_y - nebula_height;
            int16_t placed_x[MAX_NEBULAS];
            int16_t placed_y[MAX_NEBULAS];
            int32_t initial_dx = nebula_width + 40;
            int32_t initial_dy = nebula_height + 40;
            int32_t initial_min_dist_sq = initial_dx * initial_dx + initial_dy * initial_dy;

            for (int16_t i = 0; i < (int16_t)(uint16_t)MOX::_NUM_NEBULAS; ++i) {
                bool placed = false;
                int32_t min_dist_sq = initial_min_dist_sq;

                for (int16_t relaxation = 0; relaxation < 8 && !placed; ++relaxation) {
                    for (int16_t attempt = 0; attempt < 500 && !placed; ++attempt) {
                        int16_t x = (int16_t)(game_random::Random_(max_x - min_x + 1) + min_x - 1);
                        int16_t y = (int16_t)(game_random::Random_(max_y - min_y + 1) + min_y - 1);
                        bool too_close = false;

                        for (int16_t j = 0; j < i; ++j) {
                            if (HOMEGEN::Dist_Between_Points_(x, y, placed_x[j], placed_y[j]) < min_dist_sq) {
                                too_close = true;
                                break;
                            }
                        }

                        if (!too_close) {
                            placed_x[i] = x;
                            placed_y[i] = y;
                            placed = true;
                        }
                    }

                    min_dist_sq = min_dist_sq * 3 / 4;
                }

                if (!placed) {
                    placed_x[i] = (int16_t)(game_random::Random_(max_x - min_x + 1) + min_x - 1);
                    placed_y[i] = (int16_t)(game_random::Random_(max_y - min_y + 1) + min_y - 1);
                }

                MOX::_nebula[i].type = (int8_t)nebula_types[i];
                MOX::_nebula[i].x = (int16_t)HAROLD::Get_Up_Scaled_Value_(placed_x[i]);
                MOX::_nebula[i].y = (int16_t)HAROLD::Get_Up_Scaled_Value_(placed_y[i]);
            }

            RUSS::Mox_Update_();
            allocate::Pop_Block_(MOX::_screen_seg);
            Load_Nebula_Pictures_();
            return;
        }

        int16_t scaled_x_limit = HAROLD::Get_Scaled_Value_(0xA5);
        int16_t scaled_y_limit = HAROLD::Get_Scaled_Value_(0x82);
        int16_t scaled_x_limit_plus_20 = scaled_x_limit + 0x14;
        int16_t scaled_y_limit_plus_20 = scaled_y_limit + 0x14;

        while (true) {
            RUSS::Mox_Update_();
            ++attempts;
            if (attempts > 100) {
                MOX::_NUM_NEBULAS = 1;
                break;
            }

            memset(point_buffer, 0, sizeof(*point_buffer) * point_buffer_value_count);
            memset(coord_buffer, 0, sizeof(*coord_buffer) * coord_buffer_value_count);
            memset(weight_buffer, 0, sizeof(*weight_buffer) * weight_buffer_value_count);
            RUSS::Mox_Update_();

            int16_t start_x = (int16_t)game_random::Random_(0x1FA - widths[0]);
            int16_t start_y = (int16_t)game_random::Random_(0x190 - heights[0]);

            MOX::_nebula[0].type = (int8_t)nebula_types[0];
            MOX::_nebula[0].x = HAROLD::Get_Up_Scaled_Value_(start_x);
            MOX::_nebula[0].y = HAROLD::Get_Up_Scaled_Value_(start_y);

            if (MOX::_NUM_NEBULAS < 2) break;

            int16_t valid_points_count = 0;
            int32_t dx = (int32_t)(scaled_x_limit_plus_20 + min_dist_param);
            int32_t dy = (int32_t)(scaled_y_limit_plus_20 + min_dist_param);
            int32_t dist_sq_threshold = dx * dx + dy * dy;

            Build_Min_Point_Distances_(start_x, start_y, 10, 10, 0x1FA - scaled_x_limit, 0x190 - scaled_y_limit, dist_sq_threshold, point_buffer, &valid_points_count, 30000);
            RUSS::Mox_Update_();

            if (valid_points_count < (int16_t)(uint16_t)MOX::_NUM_NEBULAS) {
                min_dist_param -= 10;
                continue;
            }

            coord_buffer[0] = start_x;
            coord_buffer[1] = start_y;
            weight_buffer[0] = 1;
            int16_t placed_count = 1;
            int16_t buffer_full = 0;

            for (int16_t i = 0; i < valid_points_count && buffer_full == 0; ++i) {
                int16_t px = point_buffer[i * 2];
                int16_t py = point_buffer[i * 2 + 1];

                int16_t too_close = 0;
                for (int16_t j = 0; j < placed_count; ++j) {
                    int16_t coord_x = coord_buffer[j * 2];
                    int16_t coord_y = coord_buffer[j * 2 + 1];

                    if (px != coord_x || py != coord_y) {
                        if (HOMEGEN::Dist_Between_Points_(px, py, coord_x, coord_y) < dist_sq_threshold) {
                            too_close = 1;
                            break;
                        }
                    }
                }

                if (too_close == 0) {
                    coord_buffer[placed_count * 2] = point_buffer[i * 2];
                    coord_buffer[placed_count * 2 + 1] = point_buffer[i * 2 + 1];
                    weight_buffer[placed_count] = 1;
                    ++placed_count;
                    if (placed_count >= 10000) {
                        buffer_full = 1;
                    }
                }
            }

            RUSS::Mox_Update_();
            if ((int16_t)(uint16_t)MOX::_NUM_NEBULAS <= placed_count) {
                for (int16_t i = 0; i < (int16_t)(uint16_t)MOX::_NUM_NEBULAS; ++i) {
                    int16_t choice = RUSS::Get_Weighted_Choice_Short_(weight_buffer, placed_count);

                    MOX::_nebula[i].type = (int8_t)nebula_types[i];
                    MOX::_nebula[i].x = HAROLD::Get_Up_Scaled_Value_(coord_buffer[choice * 2]);
                    MOX::_nebula[i].y = HAROLD::Get_Up_Scaled_Value_(coord_buffer[choice * 2 + 1]);
                    weight_buffer[choice] = 0;
                }
                break;
            }

            if (attempts % 20 == 0) {
                min_dist_param -= 10;
            }
        }

        RUSS::Mox_Update_();
        allocate::Pop_Block_(MOX::_screen_seg);
        Load_Nebula_Pictures_();
    }

    /**
     * @brief Initializes the main screen map dimensions and zoom levels based on galaxy size.
     * 
     * @param size The galaxy size (0-3). Note: the parameter is ignored in favor of 
     *             HAROLD::Galaxy_Size_From_N_Stars_().
     */
    void __cdecl Init_Main_Screen_(uint8_t gal_size) {
        (void)gal_size;

        int16_t size = HAROLD::Galaxy_Size_From_N_Stars_();

        switch (size) {
        case GALAXY_SIZE_SMALL:
            MOX::_MAP_MAX_X = 0x1FA;
            MOX::_MAP_MAX_Y = 0x190;
            MOX::_cur_map_scale = 10;
            MOX::_max_map_scale = 10;
            MOX::_max_zoom_count = 0;
            MOX::_zoom_count = 0;
            break;
        case GALAXY_SIZE_MEDIUM:
            MOX::_MAP_MAX_X = 0x2F7;
            MOX::_MAP_MAX_Y = 0x258;
            MOX::_cur_map_scale = 15;
            MOX::_max_map_scale = 15;
            MOX::_max_zoom_count = 1;
            MOX::_zoom_count = 1;
            break;
        case GALAXY_SIZE_LARGE:
            MOX::_MAP_MAX_X = 0x3F4;
            MOX::_MAP_MAX_Y = 0x320;
            MOX::_cur_map_scale = 20;
            MOX::_max_map_scale = 20;
            MOX::_max_zoom_count = 2;
            MOX::_zoom_count = 2;
            break;
        case GALAXY_SIZE_HUGE:
            MOX::_MAP_MAX_X = 0x5EE;
            MOX::_MAP_MAX_Y = 0x4B0;
            MOX::_cur_map_scale = 30;
            MOX::_max_map_scale = 30;
            MOX::_max_zoom_count = 3;
            MOX::_zoom_count = 3;
            break;
#if MAX_STARS > ORIGINAL_MAX_STARS
        case GALAXY_SIZE_MAXIMUM: {
            const int16_t raw_cell_size = (MAXIMUM_GALAXY_CELL_SIZE * MAXIMUM_GALAXY_MAP_SCALE) / 10;
            MOX::_MAP_MAX_X = (int16_t)(Maximum_Galaxy_Grid_X_() * raw_cell_size);
            MOX::_MAP_MAX_Y = (int16_t)(Maximum_Galaxy_Grid_Y_() * raw_cell_size);
            MOX::_cur_map_scale = MAXIMUM_GALAXY_MAP_SCALE;
            MOX::_max_map_scale = Maximum_Galaxy_Display_Scale_();
            MOX::_max_zoom_count = 3;
            MOX::_zoom_count = 3;
            break;
        }
#endif
        default:
            break;
        }
    }

    void __cdecl Load_Nebula_Pictures_() {
        bool first_load = true;

        if (MOX::_NUM_NEBULAS == 0) {
            return;
        }

        for (int16_t type_val = 0; type_val < 12; ++type_val) {
            for (int16_t frame_idx = 0; frame_idx < 4; ++frame_idx) {
                RUSS::Mox_Update_();
                int entry_num = type_val * 4 + frame_idx + 6;

                if (first_load) {
                    MOX::_nebula_pict_seg[type_val][frame_idx] =
                        (s_animation_header*)farload::Far_Reload_("starbg.lbx", entry_num, MOX::_nebula_screen_seg);
                    first_load = false;
                }
                else {
                    MOX::_nebula_pict_seg[type_val][frame_idx] =
                        (s_animation_header*)farload::Far_Reload_Next_("starbg.lbx", entry_num, MOX::_nebula_screen_seg);
                }
            }
        }
    }

    int16_t __cdecl Gas_Giants_() {
        int16_t gas_giant_count = 0;

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            for (int16_t planet_idx = 0; planet_idx < 5; ++planet_idx) {
                if (MOX::_star[star_idx].planet_index[planet_idx] > -1) {
                    if (HAROLD::Planet_Type_(star_idx, planet_idx) == PLANET_TYPE_GAS_GIANT) {
                        gas_giant_count++;
                    }
                }
            }
        }

        return gas_giant_count;
    }

    /**
     * Rolls the number of planetary orbits to populate for a star system.
     * @param star_idx The star whose spectral class is used for the roll.
     * @return The generated satellite count.
     */
    int16_t __cdecl Generate_Number_Of_Satellites_(int16_t star_idx) {
        int32_t random_val = game_random::Random_(10);
        int16_t result = 0;

        if (MOX::_star[star_idx].spectral_class < STAR_CLASS_BLACK_HOLE) {
            int32_t row_index = random_val - 1;
            int8_t spectral_class = MOX::_star[star_idx].spectral_class;
            result = (int16_t)MOX::_class_to_num_satellites[row_index][spectral_class];
        }

        return result;
    }

    void __cdecl Planet_Generation_(int16_t star_idx) {
        int16_t orbit_res = Generate_Orbit_(star_idx);
        int8_t orbit = (int8_t)orbit_res;
        int16_t type = Generate_Satellite_Type_(star_idx, orbit);

        if (type == MAPGEN_SATELLITE_ASTEROID || type == MAPGEN_SATELLITE_GAS_GIANT || type == MAPGEN_SATELLITE_PLANET) {
            MOX::_star[star_idx].planet_index[orbit] = MOX::_planet_count;

            s_planet_data& planet = MOX::_planet[MOX::_planet_count];
            planet.orbit = orbit;
            planet.planet_type = (int8_t)type;

            Generate_Satellite_(star_idx, orbit, type);

            MOX::_planet_count++;
            _n_planets++;
        }
    }

    int16_t __cdecl Home_Planet_(int16_t planet_id) {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].home_planet_id == planet_id) {
                return i;
            }
        }
        return -1;
    }

    bool __cdecl Star_XY_Invalid_(int16_t target_idx, int16_t limit_x, int16_t limit_y, int16_t offset_val) {
        int16_t scaled_max_x = HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_X);
        HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_Y);

        bool is_invalid = false;
        if (target_idx <= 0) {
            return false;
        }

        for (int16_t i = 0; i < target_idx; ++i) {
            if (is_invalid) {
                break;
            }

            const s_star_data& star_current = MOX::_star[i];
            const s_star_data& star_target = MOX::_star[target_idx];

            if (star_current.spectral_class == STAR_CLASS_BLACK_HOLE || star_target.spectral_class == STAR_CLASS_BLACK_HOLE) {
                if (GEO::Parsecs_Between_Stars_(target_idx, i) < 5) {
                    is_invalid = true;
                }
            } else {
                int32_t x1 = HAROLD::Get_Scaled_Value_(star_current.x);
                int32_t x2 = HAROLD::Get_Scaled_Value_(star_target.x);
                int32_t y1 = HAROLD::Get_Scaled_Value_(star_current.y);
                int32_t y2 = HAROLD::Get_Scaled_Value_(star_target.y);

                int32_t dx = x1 - x2;
                int32_t dy = y1 - y2;
                int32_t dist_sq = dx * dx + dy * dy;

                int32_t abs_dx = (dx < 0) ? -dx : dx;
                int32_t abs_dy = (dy < 0) ? -dy : dy;

                int16_t scaled_60 = HAROLD::Get_Scaled_Value_(60);

                bool test0 = (dist_sq <= 800);
                bool test1 = (abs_dy < limit_y && abs_dx < limit_x);
                bool test2 = (abs_dy < 15 && abs_dx < scaled_60);
                bool test3 = ((limit_x * 2 / 3) + offset_val > scaled_max_x);

                if (test0 || test1 || test2 || test3) {
                    _tests_failed[0] += (uint16_t)test0;
                    _tests_failed[1] += (uint16_t)test1;
                    _tests_failed[2] += (uint16_t)test2;
                    _tests_failed[3] += (uint16_t)test3;
                    is_invalid = true;
                }
            }
        }

        return is_invalid;
    }

    /**
     * @brief Initializes and then shuffles an array of star name indices.
     * 
     * This function fills the star name index array with every available name,
     * then performs one random swap per name to shuffle the order.
     */
    void __cdecl Randomize_Star_Names_(uint16_t* star_name_indices) {
        for (uint16_t i = 0; i < STAR_NAME_COUNT; ++i) {
            star_name_indices[i] = i;
        }

        for (int i = 0; i < STAR_NAME_COUNT; ++i) {
            int32_t idx1 = game_random::Random_(STAR_NAME_COUNT) - 1;
            int32_t idx2 = game_random::Random_(STAR_NAME_COUNT) - 1;

            if (idx1 != idx2) {
                uint16_t swapped_name_idx = star_name_indices[idx1];
                star_name_indices[idx1] = star_name_indices[idx2];
                star_name_indices[idx2] = swapped_name_idx;
            }
        }
    }

    /**
     * Corrects black holes that overlap nebulae by converting them to habitable stars.
     */
    void __cdecl Black_Hole_Fix_(int16_t galaxy_age) {
        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            s_star_data& star = MOX::_star[i];

            if (star.spectral_class == STAR_CLASS_BLACK_HOLE) {
                int16_t x = star.x;
                int16_t y = star.y;

                bool in_nebula_1 = GEO::Point_Is_In_Some_Nebula_(x, y);
                bool in_nebula_2 = GEO::Point_Is_In_Some_Nebula_(x + 24, y);
                bool in_nebula_3 = GEO::Point_Is_In_Some_Nebula_(x, y + 24);
                bool in_nebula_4 = GEO::Point_Is_In_Some_Nebula_(x + 24, y + 24);
                bool in_nebula_5 = GEO::Point_Is_In_Some_Nebula_(x + 12, y);
                bool in_nebula_6 = GEO::Point_Is_In_Some_Nebula_(x + 24, y + 12);
                bool in_nebula_7 = GEO::Point_Is_In_Some_Nebula_(x + 12, y + 24);
                bool in_nebula_8 = GEO::Point_Is_In_Some_Nebula_(x, y + 12);

                if (in_nebula_1 || in_nebula_2 || in_nebula_3 || in_nebula_4 ||
                    in_nebula_5 || in_nebula_6 || in_nebula_7 || in_nebula_8) {
                    star.spectral_class = Generate_Non_Black_Hole_Spectral_Class_(galaxy_age);

                    int16_t satellite_count = Generate_Number_Of_Satellites_(i);
                    for (int16_t satellite = 0; satellite < satellite_count; ++satellite) {
                        Planet_Generation_(i);
                    }
                    star.visited = 0;
                }
            }
        }
    }

    void __cdecl Build_Min_Point_Distances_(int16_t x, int16_t y, int16_t min_x, int16_t min_y, int16_t max_x, int16_t max_y, int32_t dist_sq, int16_t* out_points, int16_t* count, int16_t max_count) {
        int16_t x_coords[640];
        int16_t y_coords[480];
        bool finished = false;

        memset(x_coords, 0xFF, sizeof(x_coords));
        memset(y_coords, 0xFF, sizeof(y_coords));

        for (int16_t i = min_x; i < max_x; ++i) {
            x_coords[i] = i;
        }
        for (int16_t i = min_y; i < max_y; ++i) {
            y_coords[i] = i;
        }

        HAROLD::Randomize_List_(x_coords, 640, 0);
        HAROLD::Randomize_List_(y_coords, 480, 0);

        for (int i = 0; i < 640; ++i) {
            if (finished) break;

            int16_t rx = x_coords[i];
            if (rx > -1) {
                for (int j = 0; j < 480; ++j) {
                    int16_t ry = y_coords[j];
                    if (ry > -1) {
                        int32_t dist = HOMEGEN::Dist_Between_Points_(x, y, rx, ry);
                        if (dist >= dist_sq) {
                            int16_t current_count = *count;
                            out_points[current_count * 2] = rx;
                            out_points[current_count * 2 + 1] = ry;

                            *count = current_count + 1;

                            if (*count >= max_count) {
                                finished = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    void __cdecl Get_Star_Name_(int16_t star_idx) {
        int32_t name_id = (int16_t)_name_index[star_idx % STAR_NAME_COUNT];
        int32_t numeral_id = star_idx / STAR_NAME_COUNT;
        const char (*name_table)[15] = (const char (*)[15])_name_seg;
        const char* src_name = name_table[name_id];
        char (&dest_name)[15] = MOX::_star[star_idx].name;

        if (numeral_id == 0) {
            snprintf(dest_name, sizeof(dest_name), "%.14s", src_name);
            return;
        }

        const char* numeral = MOX::_roman_literals[numeral_id];
        int32_t base_name_length = (int32_t)sizeof(dest_name) - (int32_t)strlen(numeral) - 2;
        snprintf(dest_name, sizeof(dest_name), "%.*s %s", base_name_length, src_name, numeral);
    }

    void __cdecl Universe_Generation_(uint8_t gal_size, uint8_t gal_age, int8_t nebula_freq) {
        s_colors local_colors1;
        s_colors local_colors2;
        uint32_t last_stat_clock;
        uint32_t stats_start_clock = 0;
        char planets_per_star[MAX_STARS];
        char error_buf[80];
        int16_t grid_x, grid_y;
        int16_t unused_wormhole_flag;
        int32_t galaxy_size = (int32_t)gal_size;

        s_colors* font_colors_ptr = MISC::Font_Colors2_(4, 'w', 'y');
        local_colors1 = *font_colors_ptr;

        font_colors_ptr = MISC::Font_Colors2_(4, 'w', '|');
        local_colors2 = *font_colors_ptr;
        RUSS::Mox_Update_();

        last_stat_clock = clock();
        MOX::_g_galaxy_size = gal_size;
        MOX::_g_galaxy_age = gal_age;
        _n_maps_generated = 0;
        memset(_tests_failed, 0, sizeof(_tests_failed));
        RUSS::Mox_Update_();

        while (true) {
            _evan_ptr = MOX::_climate_modifier_table;
            _make_food_planet = 0;
            _num_maps_to_generate = 1;
            ++_n_maps_generated;
            RUSS::Mox_Update_();
            RUSS::Mox_Update_();
            RUSS::Mox_Update_();

            MOX::_star_width = 37;
            MOX::_star_height = 37;
            _companion_star = 0;
            memset(MOX::_planet, 0, MAX_PLANETS * sizeof(*MOX::_planet));
            RUSS::Mox_Update_();

            _name_seg = farload::Far_Reload_Data_("starname.lbx", 1, MOX::_screen_seg, 0, 0x33D, 0xF);
            RUSS::Mox_Update_();
            _name_index = (uint16_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xCF4);
            RUSS::Mox_Update_();
            RUSS::Mox_Update_();

            if (MOX::_n_stats_to_generate != 0) {
                _num_maps_to_generate = MOX::_n_stats_to_generate;
                stats_start_clock = clock();
            }

            RUSS::Mox_Update_();
            Randomize_Star_Names_(_name_index);

            if (_num_maps_to_generate == 1) {
                if (ERIC::Get_Fade_Percent_() != 0) {
                    ERIC::Fast_Fade_In_();
                }
                video::Set_Page_Back_();
                RUSS::Mox_Update_();
                s_animation_header* raceopt_ptr = (s_animation_header*)buffer::Buffer_Reload_("raceopt.lbx", 0, MOX::_global_cache_seg);
                animate::Draw_(0, 0, raceopt_ptr);
                RUSS::Mox_Update_();
                video::Copy_Back_To_Off_();

                char* msg = (char*)HAROLD::H_Message_(0xF2);
                strlcpy(_fill_msg, msg, sizeof(_fill_msg));

                uint32_t bsize = bitmap::Get_Bitmap_Size_(325, 30);
                _fill_msg_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bsize);
                bitmap::Create_Blank_Bitmap_(325, 30, _fill_msg_bitmap);
                _fill_msg_ctr = 0;
                RUSS::Mox_Update_();
                fonts::Set_Font_Style_Outline_Heavy_(4, &local_colors1);
                fonts::Set_Outline_Color_(2, 1);
                fonts::Print_To_Bitmap_(0, 0, _fill_msg, _fill_msg_bitmap);
                RUSS::Mox_Update_();
            }

            if (_first_time_map_is_generated != 0) {
                game_random::Set_Random_Seed_(MOX::_settings.start_of_game_seed);
                _first_time_map_is_generated = 0;
            }

            switch (galaxy_size) {
            case GALAXY_SIZE_SMALL:
                MOX::_NUM_STARS = 20;
                grid_x = 5;
                grid_y = 4;
                break;
            case GALAXY_SIZE_MEDIUM:
                MOX::_NUM_STARS = 36;
                grid_x = 6;
                grid_y = 6;
                break;
            case GALAXY_SIZE_LARGE:
                MOX::_NUM_STARS = 54;
                grid_x = 9;
                grid_y = 6;
                break;
            case GALAXY_SIZE_HUGE:
                MOX::_NUM_STARS = 71;
                grid_x = 9;
                grid_y = 8;
                break;
#if MAX_STARS > ORIGINAL_MAX_STARS
            case GALAXY_SIZE_MAXIMUM:
                MOX::_NUM_STARS = _maximum_star_count;
                grid_x = Maximum_Galaxy_Grid_X_();
                grid_y = Maximum_Galaxy_Grid_Y_();
                break;
#endif
            default:
                snprintf(error_buf, sizeof(error_buf), "Galaxy size %d is undefined", galaxy_size);
                exitm::Exit_With_Message(error_buf);
                return;
            }

            Init_Main_Screen_(gal_size);
            RUSS::Mox_Update_();
            RUSS::Mox_Update_();

            MOX::_planet_count = 0;
            Generate_Number_Of_Nebulas_(gal_size, nebula_freq);
            if (MOX::_NUM_NEBULAS != 0) {
                Generate_Nebulas_();
            }

            HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_X);
            HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_Y);

            int16_t map_idx = 0;
            bool restart_setup = false;

            while ((int32_t)map_idx < (int32_t)_num_maps_to_generate) {
                if (_num_maps_to_generate != 1) {
                    MOX::_settings.start_of_game_seed = game_random::Get_Random_Seed_();
                }

                _n_space_monsters = 0;
                if (MOX::_n_stats_to_generate != 0) {
                    Generate_Number_Of_Nebulas_(gal_size, nebula_freq);
                }

                _n_planets = 0;
                _n_stars = 0;
                _n_marooned_heroes = 0;
                _n_wormholes = 0;
                MOX::_NUM_COLONIES = 0;
                memset(_special_count, 0, sizeof(_special_count));
                MOX::_planet_count = 0;
                memset(MOX::_planet, 0, MAX_PLANETS * sizeof(*MOX::_planet));
                memset(MOX::_star, 0, MAX_STARS * sizeof(*MOX::_star));
                memset(planets_per_star, 0, sizeof(planets_per_star));

                if (Set_Star_XYs_(grid_x, grid_y, gal_age, 1, 1)) {
                    fonts::Set_Font_Style_(4, &local_colors1);
                    int16_t swidth = fonts::Get_String_Width_(_fill_msg);
                    MISC::Draw_Advancing_Fill_Message_(4, &_fill_msg_ctr, MOX::_NUM_STARS * 2, &local_colors1,
                        &local_colors2, (640 - swidth) / 2, 215, _fill_msg, _fill_msg_bitmap, 1);
                    RUSS::Mox_Sync_Update_();
                    restart_setup = true;
                    break;
                }

                for (_star_count = 0; _star_count < MOX::_NUM_STARS; ++_star_count) {
                    RUSS::Mox_Update_();
                    memset(MOX::_star[_star_count].relocate_ship_to, 0xFF, sizeof(MOX::_star[_star_count].relocate_ship_to));
                    memset(MOX::_star[_star_count].officer_index, 0xFF, sizeof(MOX::_star[_star_count].officer_index));
                    MOX::_star[_star_count].wormhole_star_id = -1;

                    if (_num_maps_to_generate == 1 && map_idx < 1) {
                        if (Should_Update_Generation_Progress_(_star_count)) {
                            fonts::Set_Font_Style_(4, &local_colors1);
                            int16_t swidth = fonts::Get_String_Width_(_fill_msg);
                            MISC::Draw_Advancing_Fill_Message_(4, &_fill_msg_ctr, Generation_Progress_Max_(), &local_colors1,
                                &local_colors2, (640 - swidth) / 2, 215, _fill_msg, _fill_msg_bitmap, 0);
                            RUSS::Mox_Sync_Update_();
                        }
                    }

                    MOX::_star[_star_count].owner = -1;
                    int16_t r_size = (int16_t)(game_random::Random_(10) - 1);
                    if (r_size < 3) {
                        MOX::_star[_star_count].size = 0;
                    }
                    else if (r_size < 7) {
                        MOX::_star[_star_count].size = 1;
                    }
                    else {
                        MOX::_star[_star_count].size = 2;
                    }

                    MOX::_star[_star_count].pict_type = (int8_t)(game_random::Random_(3) - 1);
                    memset(MOX::_star[_star_count].planet_index, 0xFF, sizeof(MOX::_star[_star_count].planet_index));

                    _companion_star = 0;
                    if (HAROLD::Habitable_Spectral_Class_(_star_count)) {
                        int8_t n_sat = (int8_t)Generate_Number_Of_Satellites_(_star_count);
                        planets_per_star[_star_count] = n_sat;
                        if (n_sat > 0) {
                            int16_t planet_ctr = n_sat;
                            do {
                                RUSS::Mox_Update_();
                                Planet_Generation_(_star_count);
                                --planet_ctr;
                            } while (planet_ctr != 0);
                        }
                        if (MOX::_n_stats_to_generate != 0) {
                            ++_satellite_stat[(uint8_t)n_sat];
                        }
                    }

                    if (MOX::_star[_star_count].spectral_class != STAR_CLASS_BLACK_HOLE && planets_per_star[_star_count] > 0 && _star_count > 0) {
                        Generate_Star_Special_(_star_count, &unused_wormhole_flag, _special_count);
                    }

                    MOX::_star[_star_count].wormhole_star_id = -1;
                    ++_n_stars;
                }

                bool connected = false;
                do {
                    if (MOX::_NUM_NEBULAS != 0) {
                        Black_Hole_Fix_(gal_age);
                    }
                    GEO::Initialize_Black_Hole_Blocks_(45);
                    if (GEO::Map_Is_Connected_()) {
                        connected = true;
                    }
                    else {
                        Set_Star_XYs_(grid_x, grid_y, gal_age, 0, 0);
                    }
                } while (!connected);

                if (MOX::_n_stats_to_generate != 0) {
                    ++_nebula_stat[MOX::_NUM_NEBULAS];

                    if (MOX::_n_stats_to_generate < 100 ||
                        (map_idx % (MOX::_n_stats_to_generate / 100)) == 0) {
                        uint32_t prev_stat_clock = last_stat_clock;
                        last_stat_clock = clock();
                        RUSS::PFMT_(0, 0, "                                                                      ");

                        uint32_t total_time = last_stat_clock - stats_start_clock;
                        RUSS::PFMT_(0, 0, "Stat %d  (start: %d)  (diff %d)  (total: %d.%d)",
                            (int32_t)map_idx,
                            (int32_t)stats_start_clock,
                            (int32_t)(last_stat_clock - prev_stat_clock),
                            (int32_t)(total_time / 100),
                            (int32_t)(total_time % 100));
                    }
                }

                ++map_idx;
            }

            if (!restart_setup) {
                return;
            }
        }
    }
}