#include "deps.h"

namespace MAPGEN {
    uint8_t __cdecl Generate_Spectral_Class_(int16_t star_type_index) {
        uint8_t weights[7];

        for (int32_t i = 0; i < 7; ++i) {
            weights[i] = MOX::_star_class_table[i * 3 + star_type_index];
        }

        return RUSS::Get_Weighted_Choice_Char_(weights, 7);
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
        uint8_t weights[5];

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

        int8_t choice;
        bool found = false;

        do {
            choice = RUSS::Get_Weighted_Choice_Char_(weights, 5);
            if (MOX::_star[star_id].planet_index[choice] == -1) {
                found = true;
            }
        } while (!found);

        return choice;
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

    /**
     * @brief Generates initial data for a satellite (moon) in a star system.
     * @param star_index Index of the star this satellite orbits.
     */
    void __cdecl Generate_Satellite_(int16_t star_index) {
        s_planet_data* planet = &MOX::_planet[MOX::_planet_count];

        planet->star_index = (int8_t)star_index;

        planet->size = Generate_Size_();

        planet->mineral_class = Generate_Mineral_Class_(star_index, MOX::_planet_count);

        Get_Climate_Modifier_(planet->mineral_class);

        planet->gravity_class = (int8_t)Generate_Gravity_Class_(MOX::_planet_count);

        planet->group = (int8_t)Get_Planet_Group_(MOX::_planet_count);

        planet->climate = Generate_Climate_((int16_t)planet->group);

        planet->climate_bg_type = (int8_t)(random::Random_(3) - 1);

        Set_Secondary_Planet_Data_(star_index, MOX::_planet_count);
    }

    void __cdecl Set_Secondary_Planet_Data_(int16_t star_idx, int16_t planet_idx) {
        s_planet_data* planet = &MOX::_planet[planet_idx];

        planet->max_farms = Generate_Max_Farms_(star_idx, planet_idx);
        planet->food_per_farmer = Generate_Food_Per_Farmer_(star_idx, planet_idx);
        planet->colony_index = -1;
    }

    /**
     * Retrieves the climate modifier for a given climate index and logs debug information
     * if the modifier is positive.
     * 
     * @param climate_index The index into the climate modifier table (0-4).
     * @return The climate modifier value, or 0 if out of bounds.
     */
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
    uint8_t __cdecl Generate_Climate_(int16_t star_type) {
        uint8_t weights[10];

        // 1. Determine weights based on star type and galaxy age
        for (int i = 0; i < 10; ++i) {
            if (MOX::_g_galaxy_age <= 1) { // 0: Young, 1: Normal
                weights[i] = MOX::_normal_gal_climate_roll_table[i][star_type];
            } else if (MOX::_g_galaxy_age == 2) { // 2: Old
                weights[i] = MOX::_old_gal_climate_roll_table[i][star_type];
            }
        }

        uint8_t result = 0xFF; // Symbolic: NO_CLIMATE_SELECTED

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
        if (result == 0xFF) {
            if (_make_food_planet == 0) {
                result = RUSS::Get_Weighted_Choice_Char_(weights, 10);
            } else {
                // If forced to be food planet but multiple options exist, loop until a valid choice is picked
                do {
                    result = RUSS::Get_Weighted_Choice_Char_(weights, 10);
                } while (MOX::_food_per_farmer_table[result] == 0);
            }
        }

        return result;
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
                    Create_Wormhole_End_(found_star_idx, star_used);
                }
            }
        }
    }

    void __cdecl Get_A_Planet_Or_Star_Special_(int16_t star_idx, int16_t* wormhole_out, int8_t* special_out, uint8_t commit_changes, int32_t excluded_mask) {
        int16_t valid_planets[5] = {0};
        int16_t large_planets[5] = {0};
        int16_t habitable_planets[5] = {0};
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
                } else if (s == 8) { // Marooned Hero
                    system_weights[s] = (MOX::_zoom_count + 2 >= _n_marooned_heroes) ? 
                                        MOX::_planet_special_weighted_chance[s] : 0;
                } else if (s == 6 || s == 7) { // Natives, Splinter Colony
                    system_weights[s] = (has_large) ? 
                                        MOX::_planet_special_weighted_chance[s] : 0;
                } else if (s == 10) { // Ancient Artifacts
                    system_weights[s] = (has_habitable) ? 
                                        MOX::_planet_special_weighted_chance[s] : 0;
                } else if (s == 9) { // Space Monster
                    system_weights[s] = (MOX2::_user_wants_n_space_monsters) ? 
                                        MOX::_planet_special_weighted_chance[s] : 0;
                } else {
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
                    } else {
                        selected_hero_idx = hero_candidate;
                        stop_searching = true;
                    }

                    if (attempts == 100) {
                        stop_searching = true;
                    }
                } while (!stop_searching);

                if (selected_hero_idx == -1) {
                    MOX::_star[star_idx].system_special = SYSTEM_SPECIAL_NO_SPECIAL;
                } else {
                    MOX::_leaders[selected_hero_idx].status = 5;
                    MOX::_leaders[selected_hero_idx].location = star_idx;
                }
            }
        }
    }

    void __cdecl Load_Nebula_Pictures_() {
        bool first_load = true;
        uint8_t num_nebulas = MOX::_NUM_NEBULAS;

        if (num_nebulas == 0) {
            return;
        }

        for (int16_t neb_idx = 0; neb_idx < (int16_t)num_nebulas; ++neb_idx) {
            int16_t entry_counter = 0;
            for (int16_t type_val = 0; type_val < 12; ++type_val) {
                // Update system twice per inner loop set
                MOX::Mox_Update_();
                for (int16_t frame_idx = 0; frame_idx < 4; ++frame_idx) {
                    MOX::Mox_Update_();

                    // If the current nebula matches this nebula type index (0-11)
                    if (MOX::_nebula[neb_idx].type == (uint8_t)type_val) {
                        void* loaded_segment;
                        int entry_num = entry_counter + 6;

                        if (first_load) {
                            loaded_segment = farload::Far_Reload_(s_starbg_lbx, entry_num, MOX::_nebula_screen_seg);
                            first_load = false;
                        } else {
                            loaded_segment = farload::Far_Reload_Next_(s_starbg_lbx, entry_num, MOX::_nebula_screen_seg);
                        }

                        // Store the segment for the specific frame of this nebula
                        MOX::_nebula_pict_seg[neb_idx * 4 + frame_idx] = loaded_segment;
                    }
                    entry_counter++;
                }
            }
        }
    }
}