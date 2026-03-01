#include "pch.h"

namespace ADVCIV {
    bool __cdecl Enemy_Has_Colony_At_Star_(int16_t player_idx, int16_t star_idx) {
        int8_t owner = _star_colony_owner[star_idx];

        // Checks if the star is owned (owner > -1) and if the owner is not the specified player
        if (owner > -1 && (int16_t)owner != player_idx) {
            return true;
        }

        return false;
    }

    void __cdecl Init_Advanced_Civilization_Fleet_(int16_t player_index) {
        return;
    }

    void __cdecl Assign_Advanced_Civilization_Starting_Ships_() {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            Init_Advanced_Civilization_Fleet_(i);
        }
    }

    int32_t __cdecl Uncolonized_Planet_Worth_To_Player_(s_planet_data* planet, s_player* plr) {
        int32_t worth = 0;

        if (planet->planet_type == PLANET_TYPE_PLANET) {
            int16_t subterranean_bonus = 0;
            if (plr->evolutionary_upgrade_traits[TRAIT_SUBTERRANEAN] != 0) {
                subterranean_bonus = COLCALC::Double_Add_Two_(planet->size);
            }

            int16_t player_idx = static_cast<int16_t>(plr - MOX::_player);
            int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(planet, player_idx);

            int32_t total_pop_sum = max_pop + subterranean_bonus;
            if (plr->tech_applications[TECH_APP_ADVANCED_CITY_PLANNING] == 3) {
                total_pop_sum += 5;
            }

            int8_t mineral_bonus = COLCALC::_minerals_per_mine[planet->mineral_class];
            int32_t scaled_pop = (mineral_bonus + 1) * total_pop_sum / 3;

            int8_t pop_growth_bonus = plr->traits[TRAIT_POP_GROWTH_BONUS];
            if (pop_growth_bonus == -50) {
                worth = 80 * mineral_bonus + scaled_pop;
            } else if (pop_growth_bonus == 0) {
                worth = 75 * mineral_bonus + 2 * scaled_pop;
            } else if (pop_growth_bonus == 50) {
                worth = 70 * mineral_bonus + 3 * scaled_pop;
            } else if (pop_growth_bonus == 100) {
                worth = 65 * mineral_bonus + 4 * scaled_pop;
            }

            int32_t food_per_farmer = planet->food_per_farmer;
            bool has_grav_gen_tech = (plr->tech_applications[TECH_APP_PLANETARY_GRAVITY_GENERATOR] == 3);
            if (food_per_farmer == 0 && plr->tech_applications[TECH_APP_BIOMORPHIC_FUNGI] == 3) {
                food_per_farmer = 1;
            }

            if (plr->evolutionary_upgrade_traits[TRAIT_EATS_MINERALS] == 0) {
                if (plr->evolutionary_upgrade_traits[TRAIT_CYBERNETIC] == 0) {
                    worth += food_per_farmer * 150;
                } else {
                    worth += food_per_farmer * 75;
                }
            }

            switch (planet->planet_special) {
            case SYSTEM_SPECIAL_GOLD_DEPOSITS: worth += 1280; break;
            case SYSTEM_SPECIAL_GEM_DEPOSITS: worth += 2560; break;
            case SYSTEM_SPECIAL_ANCIENT_ARTIFACTS: worth += 640; break;
            case SYSTEM_SPECIAL_ORION_STAR: worth += 1600; break;
            default: break;
            }

            worth = (mineral_bonus + 7) * total_pop_sum * worth / 10;

            bool is_swamp_ocean = (planet->climate == PLANET_CLIMATE_OCEAN || planet->climate == PLANET_CLIMATE_SWAMP);
            bool is_aquatic = (plr->evolutionary_upgrade_traits[TRAIT_AQUATIC] != 0);

            if (!is_swamp_ocean || !is_aquatic) {
                int32_t maintenance_reduction = COLCALC::_climate_maintenance_modifiers[planet->climate] >> 2;
                worth = worth * (100 - maintenance_reduction) / 100;
                if (is_swamp_ocean) {
                    worth = (worth * 3) / 4;
                }
            }

            int32_t divisor = 1;
            int32_t penalty_factor = has_grav_gen_tech ? 1 : 0;
            bool apply_penalty = false;

            if (planet->gravity_class == PLANET_GRAVITY_LOW_G) {
                if (plr->evolutionary_upgrade_traits[TRAIT_LOW_G_WORLD] == 0) {
                    divisor = 20;
                    penalty_factor += 18;
                    apply_penalty = true;
                }
            } else if (planet->gravity_class == PLANET_GRAVITY_NORMAL_G) {
                if (plr->evolutionary_upgrade_traits[TRAIT_LOW_G_WORLD] != 0) {
                    divisor = 20;
                    penalty_factor += 18;
                    apply_penalty = true;
                }
            } else if (planet->gravity_class == PLANET_GRAVITY_HEAVY_G) {
                if (plr->evolutionary_upgrade_traits[TRAIT_LOW_G_WORLD] == 0) {
                    divisor = 6;
                    penalty_factor += 4;
                    apply_penalty = true;
                }
            }

            if (apply_penalty) {
                worth = (worth * penalty_factor) / divisor;
            }
        }

        int32_t final_worth = worth >> 6;
        if (final_worth > 0xFFFE) {
            final_worth = 0xFFFF;
        }
        return final_worth;
    }

    int __cdecl qsort_planet_worth_(const void* a, const void* b) {
        const auto* planet_1 = static_cast<const s_adv_civ_player_planet*>(a);
        const auto* planet_2 = static_cast<const s_adv_civ_player_planet*>(b);
        return planet_2->worth - planet_1->worth;
    }

    void __cdecl Get_Adv_Civ_Planet_Worthiness_(int16_t player_idx, int16_t planet_idx, int32_t* worth, int8_t* has_colony, int16_t* proximity_bonus) {
        *proximity_bonus = 0;
        *worth = 0;
        *has_colony = 0;

        if (planet_idx <= -1) {
            *worth = -1;
            return;
        }

        s_planet_data* planet = &MOX::_planet[planet_idx];
        s_player* player = &MOX::_player[player_idx];

        // 1. Base worthiness calculation
        int32_t raw_worth = Uncolonized_Planet_Worth_To_Player_(planet, player);
        *worth = (int32_t)(raw_worth & 0xFFFF);

        // 2. Proximity bonus scaling
        int16_t base_proximity = Proximity_Bonus_(player_idx, planet_idx);
        *proximity_bonus = (int16_t)((int32_t)base_proximity * (*worth) / 100);

        // 3. Penalty if player already has a colony at this star
        if (Player_Has_Colony_At_Star_(player_idx, (int16_t)planet->star_index)) {
            // Worth reduced to 67%
            *worth = (*worth * 67) / 100;
            *has_colony = 1;
        }

        // 4. Combine base worth with proximity bonus
        *worth += (int32_t)(*proximity_bonus);
    }

    void __cdecl Get_Worth_For_All_Planets_() {
        for (int16_t player_idx = 0; player_idx < (int16_t)MOX::_NUM_PLAYERS; ++player_idx) {
            s_adv_civ_player& player = _adv_civ_player[player_idx];

            for (int16_t i = 0; i < player.planet_count; ++i) {
                s_adv_civ_player_planet& planet = player.planets[i];

                int32_t worth = 0;
                int8_t has_colony = 0;
                int16_t proximity_bonus = 0;

                if (_planet_selected_flag[planet.planet_idx] == -1) {
                    Get_Adv_Civ_Planet_Worthiness_(player_idx, planet.planet_idx, &worth, &has_colony, &proximity_bonus);
                }

                planet.worth = worth;
                planet.has_colony = has_colony;
                planet.proximity_bonus = proximity_bonus;
            }

            _qsort(player.planets, 360, sizeof(s_adv_civ_player_planet), qsort_planet_worth_);
        }
    }

    void __cdecl Build_Adv_Civ_Star_List_() {
        int16_t max_dist_so_far = 0;

        for (int16_t plr_idx = 0; plr_idx < (int16_t)MOX::_NUM_PLAYERS; ++plr_idx) {
            _adv_civ_player[plr_idx].candidate_star_count = 0;

            int16_t home_star_idx = (int16_t)MOX::_planet[MOX::_player[plr_idx].home_planet_id].star_index;
            for (int16_t star_idx = 1; star_idx < 72; ++star_idx) {
                int16_t d = (int16_t)_parsec[home_star_idx].dist[star_idx];
                if (max_dist_so_far < d) {
                    max_dist_so_far = d;
                }
            }

            _adv_civ_player[plr_idx].furthest_star_dist_from_home = max_dist_so_far;

            for (int16_t colonized_star = 1; colonized_star < 72; ++colonized_star) {
                if (colonized_star < MOX::_NUM_STARS &&
                    MOX::_star[colonized_star].system_special != SYSTEM_SPECIAL_SPACE_MONSTER &&
                    Player_Has_Colony_At_Star_(plr_idx, colonized_star)) {

                    for (int16_t target_star = 1; target_star < MOX::_NUM_STARS; ++target_star) {
                        int16_t d = (int16_t)_parsec[colonized_star].dist[target_star];
                        if (d < 10 && MOX::_star[target_star].system_special != SYSTEM_SPECIAL_SPACE_MONSTER) {
                            auto& star_proximity = _adv_civ_player[plr_idx].stars[target_star];
                            if (d <= star_proximity.min_dist_to_colony) {
                                star_proximity.min_dist_to_colony = d;
                            }
                            star_proximity.star_idx = target_star;
                        }
                    }
                }
            }
        }

        Build_Adv_Civ_Planet_List_();
        Get_Worth_For_All_Planets_();
    }

    void __cdecl Build_Adv_Civ_Planet_List_() {
        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int16_t p_idx = 0; p_idx < (int16_t)MOX::_NUM_PLAYERS; ++p_idx) {
            s_adv_civ_player* player = &ADVCIV::_adv_civ_player[p_idx];

            for (int16_t star_list_idx = 0; star_list_idx < 72; ++star_list_idx) {
                s_adv_civ_player_star* adv_star = &player->stars[star_list_idx];

                if (adv_star->min_dist_to_colony < 10 && adv_star->star_idx > 0) {
                    player->candidate_star_count++;
                    int16_t star_idx = adv_star->star_idx;
                    s_star_data* star = &MOX::_star[star_idx];

                    for (int16_t i = 0; i < 5; ++i) {
                        int16_t planet_idx = star->planet_index[i];

                        if (planet_idx != -1) {
                            s_planet_data* planet = &MOX::_planet[planet_idx];

                            if (planet->planet_type == PLANET_TYPE_PLANET &&
                                player->has_planet[planet_idx] == 0 &&
                                planet->colony_index == -1)
                            {
                                int16_t count = player->planet_count;
                                s_adv_civ_player_planet* adv_planet = &player->planets[count];

                                memset(adv_planet, 0, sizeof(s_adv_civ_player_planet));
                                adv_planet->planet_idx = planet_idx;

                                player->planet_count++;
                                player->has_planet[planet_idx] = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Determines the next suitable planet index for an advanced civilization AI player to expand to.
     * Evaluates planets based on proximity to home world and existence of enemy colonies.
     */
    int16_t __cdecl Get_Next_Adv_Civ_Planet_(int16_t player_idx, int8_t* availability_map) {
        s_adv_civ_player& adv_player = _adv_civ_player[player_idx];
        int16_t home_planet_id = MOX::_player[player_idx].home_planet_id;
        int16_t home_star_idx = (int16_t)MOX::_planet[home_planet_id].star_index;

        int16_t expansion_cutoff = Get_AdvCiv_Planet_Picks_Cutoff();

        if (adv_player.assigned_planet_count < expansion_cutoff) {
            int16_t total_planet_count = adv_player.planet_count;

            // Calculate maximum allowed expansion distance based on furthest current reach
            int32_t dist_calc = (static_cast<int32_t>(adv_player.furthest_star_dist_from_home) * 4) / 10;
            int32_t galaxy_limit = static_cast<int32_t>(MOX::_settings.galaxy_size) + 9;
            int16_t max_dist = static_cast<int16_t>((dist_calc > galaxy_limit) ? dist_calc : galaxy_limit);

            for (int16_t i = 0; i < total_planet_count; ++i) {
                int16_t p_idx = adv_player.planets[i].planet_idx;

                // Check if planet is valid and not already assigned/claimed (availability_map entry -1)
                if (p_idx > -1 && availability_map[p_idx] == -1) {
                    int16_t star_idx = (int16_t)MOX::_planet[p_idx].star_index;
                    int16_t distance = static_cast<int16_t>(_parsec[home_star_idx].dist[star_idx]);

                    if (!Enemy_Has_Colony_At_Star_(player_idx, star_idx)) {
                        if (distance <= max_dist) {
                            return i; // Return the local index in the AI's planet list
                        }
                    }
                }
            }
        }

        return -1;
    }

    int16_t Get_AdvCiv_Planet_Picks_Cutoff() {
        int32_t stars_half = static_cast<int32_t>(MOX::_NUM_STARS) / 2;
        int32_t scaled_stars = static_cast<int16_t>(stars_half) * 10;
        int32_t players = static_cast<int32_t>(MOX::_NUM_PLAYERS);

        int32_t result_val = (scaled_stars / players) + 9;
        return static_cast<int16_t>(result_val / 10) - 1;
    }

    void __cdecl Build_Parsec_Star_Distances_(s_parsec* parsec) {
        for (int16_t i = 0; i < 72; ++i) {
            memset(&parsec[i].dist, 0, sizeof(parsec[i].dist));

            if (i < MOX::_NUM_STARS) {
                for (int16_t j = 0; j < MOX::_NUM_STARS; ++j) {
                    if (i == j) {
                        parsec[i].dist[j] = 0;
                    } else {
                        parsec[i].dist[j] = GEO::Parsecs_Between_Stars_(i, j);
                    }
                }
            }
        }
    }

    void __cdecl Choose_Adv_Civ_Planets_() {
        short cutoff = Get_AdvCiv_Planet_Picks_Cutoff();
        short active_players_count = (short)MOX::_NUM_PLAYERS;

        short player_status[16];

        for (int i = 0; i < 8; ++i) {
            player_status[i] = -1;
        }
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            player_status[i] = (short)i;
        }

        while (active_players_count > 0) {
            for (int i = 8; i < 16; ++i) {
                player_status[i] = -1;
            }

            Build_Adv_Civ_Star_List_();
            HAROLD::Randomize_List_(player_status, 8, 0);

            for (int i = 0; i < 8; ++i) {
                short plr_idx = player_status[i];
                if (plr_idx == -1) continue;

                short cand_idx = Get_Next_Adv_Civ_Planet_(plr_idx, _planet_selected_flag);
                if (cand_idx == -1) {
                    active_players_count--;
                    player_status[plr_idx] = -1;
                } else {
                    short global_p_idx = _adv_civ_player[plr_idx].planets[cand_idx].planet_idx;
                    int8_t star_idx = MOX::_planet[global_p_idx].star_index;
                    player_status[plr_idx + 8] = cand_idx;
                    _planet_selected_flag[global_p_idx] = (int8_t)plr_idx;
                    _star_colony_owner[star_idx] = (int8_t)plr_idx;
                }
            }

            short round_pick_count = 0;
            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                if (player_status[i + 8] != -1) {
                    round_pick_count++;
                }
            }

            if (round_pick_count == (short)MOX::_NUM_PLAYERS) {
                for (short p = 0; p < (short)MOX::_NUM_PLAYERS; ++p) {
                    short cand_idx = player_status[p + 8];
                    if (cand_idx == -1) continue;

                    s_adv_civ_player& plr = _adv_civ_player[p];
                    s_adv_civ_player_planet& cand_planet = plr.planets[cand_idx];
                    short global_p_idx = cand_planet.planet_idx;

                    s_adv_planet_worth& worth_rec = _planet_worth_to_player[p];
                    if (worth_rec.planet_idx < 0) {
                        worth_rec.planet_idx = 0;
                    }

                    int8_t w_idx = worth_rec.planet_idx;
                    s_adv_planet_worth_s1* planets_ptr = (s_adv_planet_worth_s1*)worth_rec.planets;
                    planets_ptr[w_idx].planet_idx = global_p_idx;
                    planets_ptr[w_idx].worth = cand_planet.worth;

                    worth_rec.planet_idx++;
                    if (worth_rec.planet_idx >= 20) {
                        worth_rec.planet_idx = 19;
                    }

                    plr.assigned_planet_count++;
                    cand_planet.field4_0x9 = 1;
                    cand_planet.worth = -32766;

                    if (plr.planet_count > 1) {
                        plr.planets[cand_idx] = plr.planets[plr.planet_count - 1];
                    }
                    plr.planet_count--;

                    if (plr.assigned_planet_count >= cutoff) {
                        player_status[p] = -1;
                        active_players_count--;
                    }
                }
            } else {
                // If round picking fails, undo current flags and terminate choosing process
                for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                    short cand_idx = player_status[i + 8];
                    // BUG: uses candidate list index as global planet index for undo
                    if (cand_idx >= 0 && cand_idx < 360) {
                        _planet_selected_flag[cand_idx] = -1;
                    }
                }
                active_players_count = 0;
            }
        }
    }

    int __cdecl Get_Players_Planet_Worthiness_Average_(int16_t player_idx) {
        using Fn = int(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(0x004ac710);
        return fn(player_idx);
    }

    void __cdecl Advanced_Civilization_Colonies_() {
        random::Set_Random_Seed_(MOX::_settings.start_of_game_seed);
        random::Get_Random_Seed_();
        allocate::Push_Block_(MOX::_screen_seg);

        _adv_civ_player = (s_adv_civ_player*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x8500);
        _parsec = (s_parsec*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x5100);
        _planet_selected_flag = (int8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x168);
        _star_colony_owner = (int8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x48);
        _planet_worth_to_player = (s_adv_planet_worth*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x3c8);

        memset(_adv_civ_player, 0, 0x8500);
        memset(_parsec, 0, 0x5100);
        memset(_planet_selected_flag, 0xFF, 0x168);
        memset(_star_colony_owner, 0xFF, 0x48);
        memset(_planet_worth_to_player, 0xFF, 0x3c8);

        for (int16_t p_idx = 0; p_idx < (int16_t)MOX::_NUM_PLAYERS; ++p_idx) {
            int16_t hw_idx = MOX::_player[p_idx].home_planet_id;
            int16_t star_idx = (int16_t)MOX::_planet[hw_idx].star_index;

            // Initialize all planets for this player to -1 index
            for (int i = 0; i < 360; ++i) {
                _adv_civ_player[p_idx].planets[i].planet_idx = -1;
            }

            // Initialize all stars for this player
            for (int i = 0; i < 72; ++i) {
                _adv_civ_player[p_idx].stars[i].star_idx = -1;
                _adv_civ_player[p_idx].stars[i].min_dist_to_colony = 9999;
            }

            // Setup homeworld star and colony
            _adv_civ_player[p_idx].stars[star_idx].star_idx = star_idx;
            _star_colony_owner[star_idx] = (int8_t)p_idx;

            s_adv_civ_player_planet& hw_planet = _adv_civ_player[p_idx].planets[hw_idx];
            hw_planet.has_colony = 1;
            hw_planet.planet_idx = hw_idx;
            hw_planet.field4_0x9 = 1;
            hw_planet.worth = -32766;
        }

        MOX::Mox_Update_();
        Build_Parsec_Star_Distances_(_parsec);
        MOX::Mox_Update_();
        Choose_Adv_Civ_Planets_();
        MOX::Mox_Update_();
        Twiddle_Selected_Adv_Civ_Planets_();
        MOX::Mox_Update_();
        Init_Non_Homeworld_Colonies_();
        MOX::Mox_Update_();

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    /**
     * Balances planet qualities for advanced civilizations during galaxy generation.
     * Compares all players' start worthiness and improves under-performing players' planets
     * until they are within 90% of the best player's starting quality.
     */
    void __cdecl Twiddle_Selected_Adv_Civ_Planets_() {
        MAPGEN::_MAPGEN_TRACE = 0;
        int16_t player_special_count[8] = { 0 };
        int max_worthiness = 0;
        int16_t best_player_idx = 0;

        // Step 1: Identify the player with the highest average planet worthiness
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            int current_worth = Get_Players_Planet_Worthiness_Average_(i);
            if (current_worth > max_worthiness) {
                max_worthiness = current_worth;
                best_player_idx = i;
            }
        }

        // Step 2: Count the "special" planets (Gems, Gold, Artifacts) for each player
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = 0; j < (int16_t)MOX::_planet_count; ++j) {
                if (_planet_selected_flag[j] == i) {
                    int8_t spec = MOX::_planet[j].planet_special;
                    if (spec == SYSTEM_SPECIAL_GEM_DEPOSITS || spec == SYSTEM_SPECIAL_GOLD_DEPOSITS || spec == SYSTEM_SPECIAL_ANCIENT_ARTIFACTS) {
                        player_special_count[i]++;
                    }
                }
            }
        }

        // Step 3: Improve planets for any player performing worse than the leader
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            if (i == best_player_idx) continue;

            bool player_satisfied = false;
            for (int16_t j = 0; j < (int16_t)MOX::_planet_count && !player_satisfied; ++j) {
                if (_planet_selected_flag[j] == i) {
                    int8_t targets[3];
                    targets[0] = 4 - MOX::_planet[j].climate;
                    int size_diff = 8 - MOX::_planet[j].size;
                    targets[1] = (size_diff < 0) ? 0 : (int8_t)size_diff;
                    targets[2] = 4 - MOX::_planet[j].mineral_class;

                    // If player has fewer special systems than the leader, attempt to generate one
                    if (player_special_count[i] < player_special_count[best_player_idx] &&
                        MOX::_planet[j].planet_special == 0 &&
                        MOX::_star[MOX::_planet[j].star_index].system_special == 0)
                    {
                        int8_t generated_special = 0;
                        // Skip special generation for home planets of "Fantastic Traders" trait (trait 16)
                        if (!(MOX::_player[i].home_planet_id == j && MOX::_player[i].traits[TRAIT_ANCIENT_HOME_WORLD] != 0)) {
                            int attempts = 0;
                            do {
                                int16_t dummy_buf;
                                MAPGEN::Get_A_Planet_Or_Star_Special_(MOX::_planet[j].star_index, &dummy_buf, &generated_special, 0, 0);
                                if (generated_special == SYSTEM_SPECIAL_GEM_DEPOSITS || generated_special == SYSTEM_SPECIAL_GOLD_DEPOSITS || generated_special == SYSTEM_SPECIAL_ANCIENT_ARTIFACTS) {
                                    attempts = 100; // Success
                                } else {
                                    generated_special = 0;
                                }
                                attempts++;
                            } while (attempts < 100);
                        }

                        if (generated_special > 0) {
                            MOX::_planet[j].planet_special = generated_special;
                            player_special_count[i]++;
                            MOX::_star[MOX::_planet[j].star_index].system_special = generated_special;
                        }

                        if (Get_Players_Planet_Worthiness_Average_(i) >= (max_worthiness * 90 / 100)) {
                            player_satisfied = true;
                        }
                    }

                    // Perform iterative quality improvements (max 6 iterations per planet)
                    for (int k = 0; k < 6 && !player_satisfied; ++k) {
                        if (Twiddle_Planet_(j, targets) != 0) {
                            // If Twiddle_Planet returns non-zero, no more improvements are possible for this planet
                            break;
                        }
                        MAPGEN::Set_Secondary_Planet_Data_(MOX::_planet[j].star_index, j);
                        if (Get_Players_Planet_Worthiness_Average_(i) >= (max_worthiness * 90 / 100)) {
                            player_satisfied = true;
                        }
                    }
                }
            }
        }
    }

    /**
     * @brief Modifies a planet's attributes based on weighted random selection.
     * 
     * @param planet_idx The index of the planet in the global planet array.
     * @param weight_choices An array of weights for size, climate, and mineral improvements.
     * @return int8_t Returns 1 if no choice was made (-1), otherwise 0.
     */
    int8_t __cdecl Twiddle_Planet_(int16_t planet_idx, int8_t* weight_choices) {
        int16_t choice = RUSS::Get_Weighted_Choice_Char_(weight_choices, 3);

        if (choice == -1) {
            return 1;
        }

        s_planet_data& planet = MOX::_planet[planet_idx];

        if (choice == 0) {
            // Increase planet size, capped at PLANET_SIZE_HUGE
            int32_t new_size = (int32_t)planet.size + 1;
            if (new_size > PLANET_SIZE_HUGE) {
                new_size = PLANET_SIZE_HUGE;
            }
            planet.size = (int8_t)new_size;
            weight_choices[0]--;
        }
        else if (choice == 1) {
            // Increase planet climate quality, capped at PLANET_CLIMATE_GAIA
            int32_t new_climate = (int32_t)planet.climate + 1;
            if (new_climate > PLANET_CLIMATE_GAIA) {
                new_climate = PLANET_CLIMATE_GAIA;
            }
            planet.climate = (int8_t)new_climate;
            weight_choices[1]--;
        }
        else if (choice == 2) {
            // Increase planet mineral class, capped at MINERAL_RESOURCE_ULTRA_RICH
            int32_t new_minerals = (int32_t)planet.mineral_class + 1;
            if (new_minerals > MINERAL_RESOURCE_ULTRA_RICH) {
                new_minerals = MINERAL_RESOURCE_ULTRA_RICH;
            }
            planet.mineral_class = (int8_t)new_minerals;
            weight_choices[2]--;
        }

        return 0;
    }

    void __cdecl Init_Non_Homeworld_Colonies_() {
        for (int16_t i = 0; i < (int16_t)MOX::_planet_count; ++i) {
            // Check if the planet at index 'i' has a valid selection flag (not -1).
            // A flag of -1 indicates the planet is not selected for colony initialization.
            int8_t flag = _planet_selected_flag[i];
            if (flag != -1) {
                ERICNET::Init_Non_Homeworld_Colony_(i, static_cast<int16_t>(flag));
            }
        }
    }
}