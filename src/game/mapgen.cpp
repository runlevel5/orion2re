#include "deps.h"

namespace MAPGEN {

bool __cdecl Set_Star_XYs_(int16_t x_cells, int16_t y_cells, int16_t param_3, int8_t generate_spectral, int8_t use_names) {
    int16_t x_cell_size = MOX::_MAP_MAX_X / x_cells;
    int16_t y_cell_size = MOX::_MAP_MAX_Y / y_cells;
    int16_t scaled_max_x = HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_X);
    int16_t scaled_max_y = HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_Y);
    int8_t y_collision_padding = 0;

    s_colors font_colors_main;
    s_colors* puVar = MISC::Font_Colors2_(4, 'w', 'y');
    font_colors_main = *puVar;

    s_colors font_colors_alt;
    puVar = MISC::Font_Colors2_(4, 'w', '|');
    font_colors_alt = *puVar;

    HAROLD::Set_Player_Font_Style_(0, 0);
    HAROLD::Map_Scale_To_Zoom_Level_();

    int16_t collision_h = 0;
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
        collision_h = 30;
        y_collision_padding = 4;
        break;
    }

    if (MOX::_NUM_STARS <= 0) return false;

    for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
        MAPGEN::_star_count = i;
        MOX::Mox_Update_();

        if (MAPGEN::_num_maps_to_generate == 1) {
            fonts::Set_Font_Style_(4, &font_colors_main);
            int16_t str_w = fonts::Get_String_Width_(MAPGEN::_fill_msg);
            MISC::Draw_Advancing_Fill_Message_(4, &MAPGEN::_fill_msg_ctr, MOX::_NUM_STARS * 2, &font_colors_main,
                &font_colors_alt, (0x280 - str_w) / 2, 0xD7,
                MAPGEN::_fill_msg, MAPGEN::_fill_msg_bitmap, 0);
            RUSS::Mox_Sync_Update_();
        }

        if (generate_spectral) {
            MOX::_star[i].spectral_class = MAPGEN::Generate_Spectral_Class_(param_3);
        }

        int16_t collision_w;
        if (use_names) {
            MAPGEN::Copy_Name_To_Star_(i);
            HAROLD::Set_Player_Font_Style_(0, 0);
            char formatted_name[32];
            _sprintf(formatted_name, HAROLD::s___s__00556ae4, MOX::_star[i].name);

            // Manual check for short names to append DAT_00556dac
            uint32_t name_len = 0;
            while (MOX::_star[i].name[name_len] != '\0') name_len++;

            if (name_len < 7) {
                char* src = &DAT_00556dac;
                uint32_t src_len = 0;
                while (src[src_len] != '\0') src_len++;

                char* dst_end = formatted_name;
                while (*dst_end != '\0') dst_end++;

                for (uint32_t j = 0; j <= src_len; ++j) {
                    dst_end[j] = src[j];
                }
            }

            collision_w = fonts::Get_String_Width_(formatted_name);
            if (MAPGEN::_n_maps_generated <= 2) goto START_POS_GEN;
        }

        collision_w = random::Random_(8) + random::Random_(8) + 0x20 + random::Random_(8);

    START_POS_GEN:
        int16_t retry_count = 0;
        HAROLD::Set_Player_Font_Style_(0, 0);
        HAROLD::Map_Scale_To_Zoom_Level_();

        bool collision;
        do {
            int16_t rx, scaled_x;
            do {
                rx = (int16_t)(random::Random_(x_cell_size * 2) + (i % x_cells) * x_cell_size) % MOX::_MAP_MAX_X;
                scaled_x = HAROLD::Get_Scaled_Value_(rx);
            } while (scaled_x < 0x18 || (scaled_max_x - (MOX::_star_width / 2)) < scaled_x);
            MOX::_star[i].x = rx;

            int16_t ry, scaled_y;
            do {
                ry = (int16_t)(random::Random_(y_cell_size * 2) + (y_cell_size * i) / x_cells) % MOX::_MAP_MAX_Y;
                scaled_y = HAROLD::Get_Scaled_Value_(ry);
            } while (scaled_y < 0x16 || ((scaled_max_y - (MOX::_star_height / 2)) - y_collision_padding) < scaled_y);
            MOX::_star[i].y = ry;

            collision = MAPGEN::Star_XY_Invalid_(i, collision_w, collision_h, scaled_x);
            if (!collision) break;

            if (++retry_count > 0x96) {
                int16_t fallback_retries = 0;
                bool fallback_found = false;
                do {
                    MOX::Mox_Update_();
                    int16_t fx = (int16_t)random::Random_(0x189) + 0x14;
                    int16_t fy = (int16_t)random::Random_(0x155) + 0x14;
                    MOX::_star[i].x = HAROLD::Get_Upscaled_Value_(fx);
                    MOX::_star[i].y = HAROLD::Get_Upscaled_Value_(fy);

                    if (!MAPGEN::Star_XY_Invalid_(i, collision_w, collision_h, fx)) {
                        fallback_found = true;
                    }
                    else {
                        if (++fallback_retries > 0x96) return true;
                    }
                } while (!fallback_found);
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
        int8_t weights[7];

        for (int32_t i = 0; i < 7; ++i) {
            weights[i] = MOX::_star_class_table[i * 3 + star_type_index];
        }

        return (int8_t)RUSS::Get_Weighted_Choice_Char_(weights, 7);
    }

    /**
     * @brief Determines the base food production per farmer for a given planet.
     *
     * Planets in systems with non-habitable spectral classes produce no food
     * unless the system contains the Orion star special.
     */
    uint8_t __cdecl Generate_Food_Per_Farmer_(int16_t star_index, int16_t planet_index) {
        bool habitable = HAROLD::Habitable_Spectral_Class_(star_index);

        // Check if the system is either habitable or is the Orion system
        if (!habitable && MOX::_star[star_index].system_special != SYSTEM_SPECIAL_ORION_STAR) {
            return 0;
        }

        s_planet_data* planet = &MOX::_planet[planet_index];
        uint8_t climate_idx = static_cast<uint8_t>(planet->climate);

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
        int8_t weights[5];

        static const uint8_t weight_table[15] = {
            25, 20, 10,
            18, 20, 22,
            17, 20, 30,
            15, 20, 33,
            25, 20, 5
        };

        for (int i = 0; i < 5; ++i) {
            weights[i] = weight_table[i * 3 + MOX::_settings_galaxy_age];
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
        int32_t rand_val = random::Random_(10);

        for (int16_t i = 0; i < 5; ++i) {
            int16_t threshold = (int16_t)(int8_t)MOX::_planet_size_table[i];

            if (threshold >= (int16_t)rand_val) {
                return (uint8_t)i;
            }
        }

        return 0xFF;
    }

    int8_t __cdecl Generate_Mineral_Class_(int16_t star_idx, int16_t param_2) {
        int32_t random_roll = random::Random_(10);
        int8_t spectral_class = MOX::_star[star_idx].spectral_class;

        if (spectral_class >= 0 && spectral_class < 6) {
            // Access mineral class table: (random_roll - 1) * 6 + spectral_class
            // The table is an array of mineral types mapped by (Roll-1, SpectralClass)
            int32_t table_index = (random_roll - 1) * 6 + static_cast<int32_t>(spectral_class);
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
            roll = static_cast<int16_t>(random::Random_(10) - 1);
            MAPGEN::_t_roll = roll;

            // Access weight table: _orbit_to_satellite_type[roll][orbit] (row size is 5)
            satellite_type = MOX::_orbit_to_satellite_type[roll * 5 + static_cast<uint8_t>(orbit)];

            if (satellite_type == 4) { // Special logic for type 4 (potential companion star)
                int chance = random::Random_(100);

                // If the roll is specific and it's the inner-most orbit, try to spawn a binary companion
                if (roll == 1 && chance < 11 && orbit == 0) {
                    MAPGEN::_is_companion_star = 1;
                    int8_t spectral = MOX::_star[star_idx].spectral_class;

                    if (spectral == 0) {
                        satellite_type = 5;
                    } else {
                        satellite_type = static_cast<uint8_t>(spectral + 4);
                    }
                } else {
                    // Fallback to type 1 if companion conditions aren't met
                    satellite_type = 1;
                }
            }

            // Validation: Cannot have a Gas Giant (type 2) in the 0th orbit (too close to star)
        } while (satellite_type == 2 && orbit == 0);

        // Debug assertion: Check for invalid satellite type 0
        if (satellite_type == 0) {
            RUSS::PFMT_(0, 0x0F, s_Star_d_orbit_d_has_a_0_sat_t_00558a1c,
                        static_cast<int>(star_idx),
                        static_cast<int>(orbit),
                        static_cast<int>(MAPGEN::_t_roll),
                        static_cast<int>(satellite_type));
            assert::hgetch_();
        }

        return satellite_type;
    }

    /**
     * @brief Generates initial data for a satellite (moon) in a star system.
     */
    void __cdecl Generate_Satellite_(int16_t star_idx, int16_t orbit, int16_t type) {
        s_planet_data* planet = &MOX::_planet[MOX::_planet_count];

        planet->star_index = (int8_t)star_idx;

        planet->size = Generate_Size_();

        planet->mineral_class = Generate_Mineral_Class_(star_idx, MOX::_planet_count);

        Get_Climate_Modifier_(planet->mineral_class);

        planet->gravity_class = (int8_t)Generate_Gravity_Class_(MOX::_planet_count);

        planet->group = (int8_t)Get_Planet_Group_(MOX::_planet_count);

        planet->climate = Generate_Climate_((int16_t)planet->group);

        planet->climate_bg_type = (int8_t)(random::Random_(3) - 1);

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

        // Climate index must be between 0 and 4 inclusive
        if (climate_index >= 0 && climate_index < 5) {
            modifier = MOX::_climate_modifier_table[climate_index];

            if (modifier > 0) {
                // Debug logging: arg1=0, arg2=0, fmt, modifier, planet_count, index
                RUSS::PFMT_(0, 0, s_______CLIMATE_modifier_is__d_Pla_00558a58,
                    static_cast<int32_t>(modifier),
                    static_cast<int32_t>(MOX::_planet_count),
                    static_cast<int32_t>(climate_index));
            }
        }

        return modifier;
    }

    int8_t __cdecl Generate_Gravity_Class_(int16_t planet_idx) {
        s_planet_data& planet = MOX::_planet[planet_idx];

        int16_t mineral_class = static_cast<int16_t>(planet.mineral_class);

        if (mineral_class >= 0 && mineral_class < 5) {
            int16_t size = static_cast<int16_t>(planet.size);
            return MOX::_gravity_table[mineral_class * 5 + size];
        }

        return 0;
    }

    void __cdecl Update_Planet_Group_Cache() {
        return;
    }

    int16_t __cdecl Get_Planet_Group_(int16_t planet_index) {
        s_planet_data& planet = MOX::_planet[planet_index];
        int16_t orbit = static_cast<int16_t>(planet.orbit);

        if (_is_companion_star == 1) {
            orbit -= 2;
            if (orbit < 0) {
                return 0;
            }
        }

        s_star_data& star = MOX::_star[static_cast<uint8_t>(planet.star_index)];

        if (star.spectral_class >= 0 && star.spectral_class < 6) {
            uint8_t group = MOX::_class_to_group[star.spectral_class].group[orbit];

            Update_Planet_Group_Cache();

            return static_cast<int16_t>(group);
        }

        return 0;
    }

    /**
     * Generates a planetary climate based on star type and galaxy age.
     * If the galaxy is young or normal, it uses one weight table; if old, another.
     * If forced to be a food planet, it ensures the chosen climate supports agriculture.
     */
    int8_t __cdecl Generate_Climate_(int16_t star_type) {
        int8_t weights[10];
        memset(&weights, 0, sizeof(weights));

        // 1. Determine weights based on star type and galaxy age
        for (int i = 0; i < 10; ++i) {
            if (MOX::_g_galaxy_age <= 1) { // 0: Young, 1: Normal
                weights[i] = MOX::_normal_gal_climate_roll_table[i][star_type];
            }
            else if (MOX::_g_galaxy_age == 2) { // 2: Old
                weights[i] = MOX::_old_gal_climate_roll_table[i][star_type];
            }
        }

        int16_t result = -1; // Symbolic: NO_CLIMATE_SELECTED

        // 2. Specialized handling for food-producing planets
        if (_make_food_planet != 0) {
            int16_t match_count = 0;
            uint8_t match_index = 0;
            for (int i = 0; i < 10; ++i) {
                // Check if climate has a non-zero weight and supports food production
                if (weights[i] != 0 && static_cast<int8_t>(MOX::_food_per_farmer_table[i]) > 0) {
                    match_count++;
                    match_index = static_cast<uint8_t>(i);
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
                result = RUSS::Get_Weighted_Choice_Char_(weights, 10);
            }
            else {
                // If forced to be food planet but multiple options exist, loop until a valid choice is picked
                do {
                    result = RUSS::Get_Weighted_Choice_Char_(weights, 10);
                } while (MOX::_food_per_farmer_table[result] == 0);
            }
        }

        return (int8_t)result;
    }

    void __cdecl Enforce_Planet_Max_(int16_t max_planets) {
        int16_t original_planet_count = MOX::_planet_count;
        int16_t gas_giant_count = Gas_Giants_();

        if (max_planets <= original_planet_count - gas_giant_count) {
            uint8_t is_home_system[72];
            for (int32_t i = 0; i < 18; ++i) {
                ((uint32_t*)is_home_system)[i] = 0;
            }

            _pl = (s_planet_data*)allocate::Allocate_First_Block_(MOX::_screen_seg, 360 * 17);
            for (int32_t i = 0; i < (360 * 17) / 4; ++i) {
                ((uint32_t*)_pl)[i] = ((uint32_t*)MOX::_planet)[i];
            }

            allocate::Allocate_Next_Block_(MOX::_screen_seg, 72 * 113);

            for (int32_t i = 0; i < 8; ++i) {
                if (i < MOX::_NUM_PLAYERS) {
                    int16_t home_pid = MOX::_player[i].home_planet_id;
                    is_home_system[(uint8_t)MOX::_planet[home_pid].star_index] = 1;
                }
            }

            if (capture::Check_Release_Version_() == 0) {
                RUSS::PFMT_(0, 1, s_Colony_Planets_d_Max_d, (int32_t)(original_planet_count - gas_giant_count), (int32_t)max_planets);
                assert::hgetch_();
            }

            while (max_planets <= MOX::_planet_count - gas_giant_count) {
                int32_t random_idx = random::Random_(static_cast<int32_t>(MOX::_planet_count)) - 1;
                s_planet_data* backup_p = &_pl[random_idx];

                if (backup_p->colony_index != -2 && is_home_system[(uint8_t)backup_p->star_index] == 0) {
                    if (backup_p->planet_type != 2 /* PLANET_TYPE_GAS_GIANT */) {
                        backup_p->colony_index = -2;
                        MOX::_planet_count--;
                    }
                }
            }

            int16_t new_idx = 0;
            Clear_Planets_In_Star_Structure_();

            for (int16_t old_idx = 0; old_idx < original_planet_count; ++old_idx) {
                if (_pl[old_idx].colony_index != -2) {
                    int16_t player_home_idx = Home_Planet_(old_idx);
                    if (player_home_idx != -1) {
                        MOX::_player[player_home_idx].home_planet_id = new_idx;
                    }

                    RUSS::PFMT_(0, 10, s_Moving_planet_d_to_planet_d, (int32_t)old_idx, (int32_t)new_idx);

                    MOX::_planet[new_idx] = _pl[old_idx];
                    MOX::_star[(uint8_t)MOX::_planet[new_idx].star_index].planet_index[(uint8_t)MOX::_planet[new_idx].orbit] = new_idx;

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
        uint8_t star_used[72];
        for (int i = 0; i < 18; ++i) {
            reinterpret_cast<uint32_t*>(star_used)[i] = 0;
        }

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                int16_t home_planet_id = MOX::_player[i].home_planet_id;
                int8_t star_index = MOX::_planet[home_planet_id].star_index;
                star_used[star_index] = 1;
            }
        }

        if (_n_wormholes != 0) {
            for (int wormhole_idx = 0; wormhole_idx < (int)_n_wormholes; ++wormhole_idx) {
                int16_t found_star_idx = -1;
                for (int attempt = 0; attempt <= 200; ++attempt) {
                    int32_t star_idx = random::Random_(static_cast<int32_t>(MOX::_NUM_STARS) - 1);
                    if (star_idx != 0 &&
                        star_used[star_idx] == 0 &&
                        MOX::_star[star_idx].system_special == SYSTEM_SPECIAL_NO_SPECIAL &&
                        !HAROLD::Empty_System_(static_cast<int16_t>(star_idx)) &&
                        MOX::_star[star_idx].spectral_class != STAR_CLASS_BLACK_HOLE &&
                        MOX::_star[star_idx].wormhole_star_id == 0xFF)
                    {
                        found_star_idx = (int16_t)star_idx;
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
        int16_t valid_planets[5] = { 0 };
        int16_t large_planets[5] = { 0 };
        int16_t habitable_planets[5] = { 0 };
        bool has_large = false;
        bool has_habitable = false;

        for (int i = 0; i < 5; ++i) {
            int16_t p_idx = MOX::_star[star_idx].planet_index[i];
            if (p_idx >= 0) {
                s_planet_data* planet = &MOX::_planet[p_idx];
                if (planet->planet_type == 3 && planet->planet_special == 0) {
                    valid_planets[i] = 1;
                    int8_t climate = planet->climate;
                    // Habitable climates: Desert, Arid, Tundra, Ocean, Swamp, Terran, Gaia
                    if (climate == 3 || climate == 7 || climate == 4 || climate == 5 ||
                        climate == 6 || climate == 8 || climate == 9) {
                        habitable_planets[i] = 1;
                        has_habitable = true;
                        if (planet->size >= 2) { // Medium or larger
                            large_planets[i] = 1;
                            has_large = true;
                        }
                    }
                }
            }
        }

        int16_t tries = 0;
        int16_t system_weights[12];
        system_weights[0] = 0;

        while (true) {
            for (int s = 1; s < 12; ++s) {
                if ((excluded_mask >> s) & 1) {
                    system_weights[s] = 0;
                }
                else if (s == 8) { // Marooned Hero
                    system_weights[s] = (MOX::_zoom_count + 2 >= _n_marooned_heroes) ?
                        MOX::_planet_special_weighted_chance[s] : 0;
                }
                else if (s == 6 || s == 7) { // Natives, Splinter Colony
                    system_weights[s] = (has_large) ?
                        MOX::_planet_special_weighted_chance[s] : 0;
                }
                else if (s == 10) { // Ancient Artifacts
                    system_weights[s] = (has_habitable) ?
                        MOX::_planet_special_weighted_chance[s] : 0;
                }
                else if (s == 9) { // Space Monster
                    system_weights[s] = (MOX2::_user_wants_n_space_monsters) ?
                        MOX::_planet_special_weighted_chance[s] : 0;
                }
                else {
                    system_weights[s] = MOX::_planet_special_weighted_chance[s];
                }
            }

            int16_t choice = RUSS::Get_Weighted_Choice_Short_(system_weights, 12);
            *special_out = (int8_t)choice;
            bool success = false;

            if ((uint32_t)choice - 1 <= 9) { // Switch only handles specials 1-10
                switch (choice) {
                case 1: // Stable Wormhole
                    if (commit_changes && _n_wormholes < (MOX::_settings.galaxy_size * 4 + 4)) {
                        *wormhole_out = 1;
                        _n_wormholes++;
                        success = true;
                    }
                    break;

                case 2: // Space Debris
                case 3: // Pirate Cache
                case 4: // System Special placeholder
                case 9: // Space Monster
                    if (commit_changes) {
                        MOX::_star[star_idx].system_special = (int8_t)choice;
                    }
                    success = true;
                    break;

                case 5: // Gold Deposits (and Gem Deposits if it were in the switch)
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

                case 6: // Natives
                case 7: // Splinter Colony
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

                case 8: // Marooned Hero
                    _n_marooned_heroes++; // Increment global attempt counter
                    if (commit_changes) {
                        MOX::_star[star_idx].system_special = (int8_t)choice;
                    }
                    success = true;
                    break;

                case 10: // Ancient Artifacts
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
                *special_out = 0;
                break;
            }
        }
    }

    void __cdecl Generate_Star_Special_(int16_t star_idx, void* param_2, uint8_t* special_counts) {
        uint8_t special_type = 0;

        if ((int16_t)MOX::_planet_special < (int16_t)random::Random_(100)) {
            Get_A_Planet_Or_Star_Special_(star_idx, (int16_t*)param_2, (int8_t*)&special_type, 1, 0);
        } else {
            special_type = 0;
        }

        int16_t monster_chance;
        if (MOX::_settings.game_difficulty == 3) {
            monster_chance = 8;
        } else if (MOX::_settings.game_difficulty == 4) {
            monster_chance = 12;
        } else {
            monster_chance = 5;
        }

        if (random::Random_(100) - 1 < (int32_t)monster_chance) {
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
                    int16_t hero_candidate = (int16_t)random::Random_(38) - 1;

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

    void __cdecl Generate_Number_Of_Nebulas_(int16_t galaxy_size, int16_t nebula_freq) {
        MOX::_NUM_NEBULAS = 0;

        if (MOX::_nebula_screen_seg == nullptr) {
            return;
        }

        switch (galaxy_size) {
        case 0:
            MOX::_NUM_NEBULAS = static_cast<uint8_t>(random::Random_(2) - 1);
            break;
        case 1:
            MOX::_NUM_NEBULAS = static_cast<uint8_t>(random::Random_(2));
            break;
        case 2:
            MOX::_NUM_NEBULAS = static_cast<uint8_t>(random::Random_(3));
            break;
        case 3:
            MOX::_NUM_NEBULAS = static_cast<uint8_t>(random::Random_(3) + 1);
            break;
        default:
            break;
        }
    }

    void __cdecl Generate_Nebulas_() {
        int16_t widths[12];
        int16_t heights[12];
        int16_t nebula_types[12];
        int16_t min_dist_param = 0x14;
        int16_t attempts = 0;

        allocate::Push_Block_(MOX::_screen_seg);
        // Buffers used for map generation geometry
        int32_t* point_buffer = (int32_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 120000);
        int16_t* coord_buffer = (int16_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40000);
        uint16_t* weight_buffer = (uint16_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 20000);

        MOX::Mox_Update_();

        for (int i = 0; i < 12; ++i) {
            s_animation_header* anim = (s_animation_header*)farload::Far_Reload_(s_starbg_lbx, i * 4 + 6, MOX::_nebula_screen_seg);
            widths[i] = animate::Get_Width_(anim);
            heights[i] = animate::Get_Height_(anim);
        }

        if (MOX::_NUM_NEBULAS != 0) {
            for (int i = 0; i < MOX::_NUM_NEBULAS; ++i) {
                int16_t type;
                int16_t inner_attempts = 0;
                do {
                    MOX::Mox_Update_();
                    type = (int16_t)random::Random_(12) - 1;
                    inner_attempts++;
                    // Try to pick a type, loop until something allows it or too many attempts
                    if (inner_attempts > 0xF0) break;
                } while (false); // Simplification of disassembly logic
                nebula_types[i] = type;
                MOX::Mox_Update_();
            }
        }

        int16_t scaled_x_limit = HAROLD::Get_Scaled_Value_(0xA5);
        int16_t scaled_y_limit = HAROLD::Get_Scaled_Value_(0x82);

        while (true) {
            MOX::Mox_Update_();
            attempts++;
            if (attempts > 100) {
                MOX::_NUM_NEBULAS = 1;
                break;
            }

            // Clear buffers
            for (int i = 0; i < 30000; ++i) point_buffer[i] = 0;
            for (int i = 0; i < 20000; ++i) coord_buffer[i] = 0;
            for (int i = 0; i < 10000; ++i) weight_buffer[i] = 0;
            MOX::Mox_Update_();

            int16_t start_x = (int16_t)random::Random_(0x1FA - widths[0]);
            int16_t start_y = (int16_t)random::Random_(400 - heights[0]);

            MOX::_nebula[0].type = (int8_t)nebula_types[0];
            MOX::_nebula[0].x = HAROLD::Get_Upscaled_Value_(start_x);
            MOX::_nebula[0].y = HAROLD::Get_Upscaled_Value_(start_y);

            if (MOX::_NUM_NEBULAS < 2) break;

            int16_t valid_points_count = 0;
            int32_t dx = (scaled_x_limit + 0x14) + min_dist_param;
            int32_t dy = min_dist_param + (scaled_y_limit + 0x14);
            int32_t dist_sq_threshold = dx * dx + dy * dy;

            Build_Min_Point_Distances_(start_x, start_y, 10, 10, 0x1FA - scaled_x_limit, 400 - scaled_y_limit, dist_sq_threshold, point_buffer, &valid_points_count, 30000);
            MOX::Mox_Update_();

            if (valid_points_count < MOX::_NUM_NEBULAS) {
                min_dist_param -= 10;
                continue;
            }

            coord_buffer[0] = start_x;
            coord_buffer[1] = start_y;
            weight_buffer[0] = 1;
            int16_t placed_count = 1;
            bool buffer_full = false;

            for (int i = 0; i < valid_points_count && !buffer_full; ++i) {
                int16_t px = (int16_t)(point_buffer[i] & 0xFFFF);
                int16_t py = (int16_t)(point_buffer[i] >> 16);

                bool too_close = false;
                for (int j = 0; j < placed_count; ++j) {
                    if (px == coord_buffer[j * 2] && py == coord_buffer[j * 2 + 1]) continue;
                    if (HOMEGEN::Dist_Between_Points_(px, py, coord_buffer[j * 2], coord_buffer[j * 2 + 1]) < dist_sq_threshold) {
                        too_close = true;
                        break;
                    }
                }

                if (!too_close) {
                    coord_buffer[placed_count * 2] = px;
                    coord_buffer[placed_count * 2 + 1] = py;
                    weight_buffer[placed_count] = 1;
                    placed_count++;
                    if (placed_count >= 10000) buffer_full = true;
                }
            }

            MOX::Mox_Update_();
            if (MOX::_NUM_NEBULAS <= placed_count) {
                for (int i = 0; i < MOX::_NUM_NEBULAS; ++i) {
                    int16_t choice = MOX::Get_Weighted_Choice_Short_(weight_buffer, placed_count);
                    MOX::_nebula[i].type = (int8_t)nebula_types[i];
                    MOX::_nebula[i].x = HAROLD::Get_Upscaled_Value_(coord_buffer[choice * 2]);
                    MOX::_nebula[i].y = HAROLD::Get_Upscaled_Value_(coord_buffer[choice * 2 + 1]);
                    weight_buffer[choice] = 0;
                }
                break;
            }

            if (attempts % 20 == 0) {
                min_dist_param -= 10;
            }
        }

        MOX::Mox_Update_();
        allocate::Pop_Block_(MOX::_screen_seg);
        Load_Nebula_Pictures_();
    }

    /**
     * @brief Initializes the main screen map dimensions and zoom levels based on galaxy size.
     * 
     * @param size The galaxy size (0-3). Note: the parameter is ignored in favor of 
     *             HAROLD::Galaxy_Size_From_N_Stars_().
     */
    void __cdecl Init_Main_Screen_(uint8_t size) {
        int16_t galaxy_size = HAROLD::Galaxy_Size_From_N_Stars_();

        switch (galaxy_size) {
        case 0:
            MOX::_MAP_MAX_X = 0x1FA;
            MOX::_MAP_MAX_Y = 0x190;
            MOX::_cur_map_scale = 10;
            MOX::_max_map_scale = 10;
            MOX::_max_zoom_count = 0;
            MOX::_zoom_count = 0;
            break;
        case 1:
            MOX::_MAP_MAX_X = 0x2F7;
            MOX::_MAP_MAX_Y = 0x258;
            MOX::_cur_map_scale = 15;
            MOX::_max_map_scale = 15;
            MOX::_max_zoom_count = 1;
            MOX::_zoom_count = 1;
            break;
        case 2:
            MOX::_MAP_MAX_X = 0x3F4;
            MOX::_MAP_MAX_Y = 0x320;
            MOX::_cur_map_scale = 20;
            MOX::_max_map_scale = 20;
            MOX::_max_zoom_count = 2;
            MOX::_zoom_count = 2;
            break;
        case 3:
            MOX::_MAP_MAX_X = 0x5EE;
            MOX::_MAP_MAX_Y = 0x4B0;
            MOX::_cur_map_scale = 30;
            MOX::_max_map_scale = 30;
            MOX::_max_zoom_count = 3;
            MOX::_zoom_count = 3;
            break;
        default:
            break;
        }
    }

    void __cdecl Load_Nebula_Pictures_() {
        bool first_load = true;
        uint8_t num_nebulas = MOX::_NUM_NEBULAS;

        if (num_nebulas == 0) {
            return;
        }

        for (int8_t neb_idx = 0; neb_idx < num_nebulas; ++neb_idx) {
            int16_t entry_counter = 0;
            for (int16_t type_val = 0; type_val < 12; ++type_val) {
                // Update system twice per inner loop set
                MOX::Mox_Update_();
                for (int16_t frame_idx = 0; frame_idx < 4; ++frame_idx) {
                    MOX::Mox_Update_();

                    // If the current nebula matches this nebula type index (0-11)
                    if (MOX::_nebula[neb_idx].type == (int8_t)type_val) {
                        s_animation_header* loaded_segment;
                        int entry_num = entry_counter + 6;

                        if (first_load) {
                            loaded_segment = reinterpret_cast<s_animation_header*>(farload::Far_Reload_(s_starbg_lbx, entry_num, MOX::_nebula_screen_seg));
                            first_load = false;
                        }
                        else {
                            loaded_segment = reinterpret_cast<s_animation_header*>(farload::Far_Reload_Next_(s_starbg_lbx, entry_num, MOX::_nebula_screen_seg));
                        }

                        // Store the segment for the specific frame of this nebula
                        MOX::_nebula_pict_seg[neb_idx][frame_idx] = loaded_segment;
                    }
                    entry_counter++;
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
     * Finds the index of the player whose home planet matches the given planet ID.
     * @param planet_id The ID of the planet to search for.
     * @return The player index if found, otherwise -1.
     */
    int16_t __cdecl Generate_Number_Of_Satellites_(int16_t star_idx) {
        int32_t random_val = random::Random_(10);
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

        // Check if satellite type is Small (1), Medium (2), or Large (3)
        if (type == 1 || type == 2 || type == 3) {
            MOX::_star[star_idx].planet_index[orbit] = (int16_t)MOX::_planet_count;

            s_planet_data& planet = MOX::_planet[MOX::_planet_count]; // size: 17
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
     * This function fills the star name index array with sequential values from 0 to 828,
     * then performs 829 random swaps to shuffle the order.
     */
    void __cdecl Randomize_Star_Names_(uint16_t* star_name_indices) {
        for (uint16_t i = 0; i < 829; ++i) {
            star_name_indices[i] = i;
        }

        for (int i = 0; i < 829; ++i) {
            int32_t idx1 = random::Random_(829) - 1;
            int32_t idx2 = random::Random_(829) - 1;

            if (idx1 != idx2) {
                uint16_t temp = star_name_indices[idx1];
                star_name_indices[idx1] = star_name_indices[idx2];
                star_name_indices[idx2] = temp;
            }
        }
    }

    /**
     * Corrects black holes that overlap nebulae by converting them to habitable stars.
     */
    void __cdecl Black_Hole_Fix_(int16_t param_1) {
        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            s_star_data& star = MOX::_star[i];

            if (star.spectral_class == STAR_CLASS_BLACK_HOLE) {
                int16_t x = star.x;
                int16_t y = star.y;

                // Check points around the star for nebula presence
                bool in_nebula = GEO::Point_Is_In_Some_Nebula_(x, y) ||
                                 GEO::Point_Is_In_Some_Nebula_(x + 24, y) ||
                                 GEO::Point_Is_In_Some_Nebula_(x, y + 24) ||
                                 GEO::Point_Is_In_Some_Nebula_(x + 24, y + 24) ||
                                 GEO::Point_Is_In_Some_Nebula_(x + 12, y) ||
                                 GEO::Point_Is_In_Some_Nebula_(x + 24, y + 12) ||
                                 GEO::Point_Is_In_Some_Nebula_(x + 12, y + 24) ||
                                 GEO::Point_Is_In_Some_Nebula_(x, y + 12);

                if (in_nebula) {
                    int16_t attempts = 0;
                    do {
                        star.spectral_class = MAPGEN::Generate_Spectral_Class_(param_1);

                        // Failsafe: if we can't find a habitable class after 200 attempts, force class M
                        if (attempts > 200) {
                            star.spectral_class = STAR_CLASS_M;
                        }
                        attempts++;
                    } while (!HAROLD::Habitable_Spectral_Class_(i));

                    Planet_Generation_(i);
                    star.visited = 0;
                }
            }
        }
    }

    void __cdecl Build_Min_Point_Distances_(int16_t x, int16_t y, int16_t min_x, int16_t min_y, int16_t max_x, int16_t max_y, int32_t dist_sq, void* out_points, int16_t* count, int32_t max_count) {
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
                            int16_t* out = (int16_t*)out_points;
                            out[current_count * 2] = rx;
                            out[current_count * 2 + 1] = ry;

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

    void __cdecl Copy_Name_To_Star_(int16_t star_idx) {
        int32_t name_id = static_cast<int16_t>(_name_index[star_idx]);
        const char* src_name = reinterpret_cast<const char*>(reinterpret_cast<std::uintptr_t>(_name_seg) + (name_id * 15));
        char* dest_name = MOX::_star[star_idx].name;

        while ((*dest_name++ = *src_name++) != '\0');
    }

    void __cdecl Universe_Generation_(uint8_t gal_size, uint8_t gal_age, int8_t nebula_freq) {
        s_colors local_colors1;
        s_colors local_colors2;
        uint32_t start_time;
        char planets_per_star[80];
        char error_buf[80];
        int16_t grid_x, grid_y;

        s_colors* p = MISC::Font_Colors2_(4, 'w', 'y');
        MOX::_g_galaxy_age = gal_age;
        _n_maps_generated = 0;
        local_colors1 = *p;

        p = MISC::Font_Colors2_(4, 'w', '|');
        local_colors2 = *p;
        memset(_tests_failed, 0, 8);
        MOX::Mox_Update_();

        _make_food_planet = 0;
        MOX::Mox_Update_();
        start_time = _clock();
        _num_maps_to_generate = 1;
        _n_maps_generated++;
        MOX::_g_galaxy_size = gal_size;
        MOX::Mox_Update_();
        MOX::Mox_Update_();
        MOX::Mox_Update_();

        MOX::_star_width = 37;
        MOX::_star_height = 37;
        _is_companion_star = 0;

        memset(MOX::_planet, 0, 0x5fa * 4);
        MOX::Mox_Update_();

        _name_seg = farload::Far_Reload_Data_("starname.lbx", 1, MOX::_screen_seg, 0, 0x33D, 0xF);
        MOX::Mox_Update_();
        _name_index = reinterpret_cast<uint16_t*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xCF4));
        MOX::Mox_Update_();
        MOX::Mox_Update_();

        if (MOX::_n_stats_to_generate != 0) {
            _num_maps_to_generate = MOX::_n_stats_to_generate;
            start_time = _clock();
        }

        MOX::Mox_Update_();
        Randomize_Star_Names_(_name_index);

        if (_num_maps_to_generate == 1) {
            if (ERIC::Get_Fade_Percent_() != 0) ERIC::Fast_Fade_In_();
            video::Set_Page_Back_();
            MOX::Mox_Update_();
            s_animation_header* raceopt_ptr = reinterpret_cast<s_animation_header*>(Buffer_Reload_("raceopt.lbx", 0, MOX::_global_cache_seg));
            animate::Draw_(0, 0, raceopt_ptr);
            MOX::Mox_Update_();
            video::Copy_Back_To_Off_();

            char* msg = (char*)HAROLD::H_Message_(0xF2);
            strncpy(_fill_msg, msg, sizeof(_fill_msg));

            uint32_t bsize = bitmap::Get_Bitmap_Size_(325, 30);
            _fill_msg_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, bsize);
            bitmap::Create_Blank_Bitmap_(325, 30, _fill_msg_bitmap);
            _fill_msg_ctr = 0;
            MOX::Mox_Update_();
            fonts::Set_Font_Style_Outline_Heavy_(4, &local_colors1);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Print_To_Bitmap_(0, 0, _fill_msg, _fill_msg_bitmap);
            MOX::Mox_Update_();
        }

        if (_first_time_map_is_generated != 0) {
            random::Set_Random_Seed_(MOX::_settings.start_of_game_seed);
            _first_time_map_is_generated = 0;
        }

        switch (gal_size) {
        case 0:
            MOX::_NUM_STARS = 20;
            grid_x = 5; grid_y = 4;
            break;
        case 1:
            MOX::_NUM_STARS = 36;
            grid_x = 6; grid_y = 6;
            break;
        case 2:
            MOX::_NUM_STARS = 54;
            grid_x = 9; grid_y = 6;
            break;
        case 3:
            MOX::_NUM_STARS = 71;
            grid_x = 9; grid_y = 8;
            break;
        default:
            _sprintf(error_buf, "Galaxy size %d is undefined", gal_size);
            exitm::Exit_With_Message(error_buf);
            return;
        }

        Init_Main_Screen_(gal_size);
        MOX::Mox_Update_();
        MOX::Mox_Update_();

        MOX::_planet_count = 0;
        Generate_Number_Of_Nebulas_(gal_size, nebula_freq);
        if (MOX::_NUM_NEBULAS != 0) Generate_Nebulas_();

        HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_X);
        HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_Y);

        uint32_t map_idx = 0;
        while (map_idx < _num_maps_to_generate) {
            if (_num_maps_to_generate != 1) {
                MOX::_settings.start_of_game_seed = random::Get_Random_Seed_();
            }

            _n_space_monsters = 0;
            if (MOX::_n_stats_to_generate != 0) {
                Generate_Number_Of_Nebulas_(gal_size, nebula_freq);
            }

            memset(_special_count, 0, 12);
            _n_stars = 0;
            _n_marooned_heroes = 0;
            _n_wormholes = 0;
            MOX::_NUM_COLONIES = 0;
            MOX::_planet_count = 0;

            memset(MOX::_planet, 0, 306 * sizeof(s_planet_data));
            for (int i = 0; i < 80; ++i) {
                memset(MOX::_star[i].name, 0, 15);
            }
            memset(planets_per_star, 0, 80);

            if (Set_Star_XYs_(grid_x, grid_y, gal_age, 1, 1)) {
                fonts::Set_Font_Style_(4, &local_colors1);
                int16_t swidth = fonts::Get_String_Width_(_fill_msg);
                MISC::Draw_Advancing_Fill_Message_(4, &_fill_msg_ctr, MOX::_NUM_STARS * 2, &local_colors1, &local_colors2, (640 - swidth) / 2, 215, _fill_msg, _fill_msg_bitmap, 1);
                RUSS::Mox_Sync_Update_();
                continue;
            }

            for (_star_count = 0; _star_count < MOX::_NUM_STARS; ++_star_count) {
                MOX::Mox_Update_();
                for (int i = 0; i < 8; ++i) MOX::_star[_star_count].relocate_ship_to[i] = -1;
                for (int i = 0; i < 8; ++i) MOX::_star[_star_count].officer_index[i] = -1;
                MOX::_star[_star_count].wormhole_star_id = -1;

                if (_num_maps_to_generate == 1 && map_idx == 0) {
                    fonts::Set_Font_Style_(4, &local_colors1);
                    int16_t swidth = fonts::Get_String_Width_(_fill_msg);
                    MISC::Draw_Advancing_Fill_Message_(4, &_fill_msg_ctr, MOX::_NUM_STARS * 2, &local_colors1, &local_colors2, (640 - swidth) / 2, 215, _fill_msg, _fill_msg_bitmap, 0);
                    RUSS::Mox_Sync_Update_();
                }

                MOX::_star[_star_count].owner = -1;
                int r_size = random::Random_(10) - 1;
                if (r_size < 3) MOX::_star[_star_count].size = 0;
                else if (r_size < 7) MOX::_star[_star_count].size = 1;
                else MOX::_star[_star_count].size = 2;

                MOX::_star[_star_count].pict_type = (int8_t)random::Random_(3) - 1;
                for (int i = 0; i < 5; ++i) MOX::_star[_star_count].planet_index[i] = -1;

                _is_companion_star = 0;
                if (HAROLD::Habitable_Spectral_Class_(_star_count)) {
                    int16_t n_sat = Generate_Number_Of_Satellites_(_star_count);
                    planets_per_star[_star_count] = (char)n_sat;
                    for (int i = 0; i < n_sat; ++i) {
                        MOX::Mox_Update_();
                        Planet_Generation_(_star_count);
                    }
                    if (MOX::_n_stats_to_generate != 0) {
                        _satellite_stat[n_sat]++;
                    }
                }

                if (MOX::_star[_star_count].spectral_class != 6 && planets_per_star[_star_count] > 0 && _star_count > 0) {
                    Generate_Star_Special_(_star_count, &_special_count[4], &_special_count[4]);
                }
                _n_stars++;
            }

            bool connected = false;
            do {
                if (MOX::_NUM_NEBULAS != 0) Black_Hole_Fix_(gal_age);
                GEO::Initialize_Black_Hole_Blocks_(45);
                if (GEO::Map_Is_Connected_()) {
                    connected = true;
                }
                else {
                    Set_Star_XYs_(grid_x, grid_y, gal_age, 0, 0);
                }
            } while (!connected);

            if (MOX::_n_stats_to_generate != 0) {
                _nebula_stat[MOX::_NUM_NEBULAS]++;
            }

            map_idx++;
        }
        return;
    }
}