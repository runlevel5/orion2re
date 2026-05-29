#include "pch.h"

namespace COLCALC {
    int16_t __cdecl Get_System_Population_(int16_t player_idx, int16_t star_idx) {
        int16_t total_pop = 0;

        for (int i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];

            if (planet_idx != -1) {
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

                if (colony_idx != -1) {
                    // Check if colony belongs to the specified player
                    if (MOX::_colony[colony_idx].owner == player_idx) {
                        // n_pops is accumulated as an unsigned byte value
                        total_pop += (uint8_t)MOX::_colony[colony_idx].n_pops;
                    }
                }
            }
        }

        return total_pop;
    }

    void __cdecl Show_Command_Points_(int player_idx) {
        char msg_buffer[3000];

        // Assembly calculates offset: player_idx * 3753 (0xEA9)
        Compute_Player_Maintenance_(&MOX::_player[player_idx], msg_buffer);

        TEXTBOX::Text_Box_(_command_summary_msg, msg_buffer);
    }

    int __cdecl Calc_Player_Climate_(int player_idx, int climate) {
        if (player_idx < 8 && MOX::_player[player_idx].traits[TRAIT_AQUATIC] != 0) {
            switch (climate) {
                case PLANET_CLIMATE_TUNDRA: // 4
                case PLANET_CLIMATE_SWAMP: // 6
                    return PLANET_CLIMATE_TERRAN; // 8
                case PLANET_CLIMATE_OCEAN: // 5
                case PLANET_CLIMATE_TERRAN: // 8
                    return PLANET_CLIMATE_GAIA; // 9
                default:
                    break;
            }
        }
        return climate;
    }

    int __cdecl Colony_Officer_(s_colony* colony) {
        int8_t star_idx = MOX::_planet[colony->planet].star_index;

        int8_t officer_idx = (int8_t)MOX::_star[star_idx].officer_index[colony->owner];

        if (officer_idx != -1) {
            if (MOX::_leaders[officer_idx].eta != 0) {
                return -1;
            }
        }

        return (int)officer_idx;
    }

    void __cdecl Twiddle_Initial_Homeworlds_() {
        int min_command_points = 30000;

        CONTACT::Compute_Contacts_();
        RUSS::Mox_Update_();

        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            Twiddle_Initial_Homeworld_(&MOX::_colony[i]);
        }

        SHIPMOVE::Update_All_Ship_Ranges_();
        All_Colony_Calculations_();

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            s_player* player = &MOX::_player[i];
            int star_idx = MOX::_planet[player->home_planet_id].star_index;
            MOX::_star[star_idx].colonize_player = (int8_t)i;

            if (player->command_points < (int16_t)min_command_points) {
                min_command_points = player->command_points;
            }
        }

        if (MOX::_settings.starting_civilization_level == 2) {
            int starting_ship_points = (min_command_points - 2) / 2;

            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                s_player* player = &MOX::_player[i];
                RUSS::Mox_Update_();

                int star_idx = MOX::_planet[player->home_planet_id].star_index;
                MOX::_star[star_idx].colonize_player = (int8_t)i;

                player->bc = (player->traits[TRAIT_TAX_BONUS] * 100 + 200);

                for (int s = 0; s < MOX::_NUM_STARS; ++s) {
                    s_star_data* star = &MOX::_star[s];
                    if (star->system_special < SYSTEM_SPECIAL_SPLINTER_COLONY ||
                        star->system_special > SYSTEM_SPECIAL_MAROONED_HERO) {

                        if (SHIPMOVE::Star_In_Normal_Range_Of_Player_(star, (int16_t)i)) {
                            star->visited |= (1 << i);

                            if (star->system_special > SYSTEM_SPECIAL_STABLE_WORMHOLE &&
                                star->system_special < SYSTEM_SPECIAL_GOLD_DEPOSITS) {
                                if (Monster_At_Star_(s) == 0) {
                                    star->system_special = SYSTEM_SPECIAL_NO_SPECIAL;
                                }
                            }
                        }
                    }
                }

                int freighter_pool = 10;
                if (player->surplus_freighters < 0) freighter_pool -= player->surplus_freighters;
                if (player->surplus_food < 0) freighter_pool -= player->surplus_food;
                player->n_freighters = (int16_t)((freighter_pool + 4) / 5) * 5;

                short first_colony = Get_First_Player_Colony_Idx(i);
                int pool_remaining = min_command_points;
                if (pool_remaining < 0) {
                    pool_remaining = 0;
                }

                int max_design_idx;
                if (starting_ship_points < 0) {
                    max_design_idx = 0;
                } else {
                    max_design_idx = starting_ship_points;
                    if (max_design_idx >= 4) {
                        max_design_idx = 4;
                        if (player->tech_applications[TECH_APP_TITAN_CONSTRUCTION] != 3) {
                            max_design_idx = 3;
                        }
                    }
                }

                for (;;) {
                    RUSS::Mox_Update_();
                    if (pool_remaining == 0) {
                        break;
                    }

                    if (pool_remaining - 1 < max_design_idx) {
                        max_design_idx = pool_remaining - 1;
                    }

                    MOX::_ship[MOX::_NUM_SHIPS].owner = (int8_t)i;
                    INITSHIP::Update_Player_Ship_Designs_((int16_t)i);

                    pool_remaining -= (max_design_idx + 1);
                }
            }
            All_Colony_Calculations_();
        }
    }

    void __cdecl Compute_All_Star_Colony_Stuff_() {
        int32_t num_stars = static_cast<int32_t>(MOX::_NUM_STARS);
        s_star_data* star_ptr = MOX::_star;
        s_star_data* end_ptr = &MOX::_star[num_stars];

        if (star_ptr < end_ptr) {
            do {
                Compute_Star_Colony_Stuff_(star_ptr);
                star_ptr++;
            } while (star_ptr < end_ptr);
        }
    }

    void __cdecl Star_Colony_Calculation_(short star_idx, short player_idx) {
        s_star_data* star = &MOX::_star[star_idx];

        for (int i = 0; i < 5; ++i) {
            short planet_idx = star->planet_index[i];
            if (planet_idx != -1) {
                short colony_idx = MOX::_planet[planet_idx].colony_index;
                if (colony_idx != -1) {
                    s_colony* colony = &MOX::_colony[colony_idx];

                    if (colony->owner == player_idx && colony->outpost_flag == 0) {
                        Recalculate_Colony_(colony);
                    }
                }
            }
        }

        Update_Player_Stats_(&MOX::_player[player_idx]);
    }

    void __cdecl Compute_Star_Colony_Stuff_(s_star_data* col) {
        uint8_t old_interdictor_state = col->has_warp_field_interdictor;

        col->has_colony = 0;
        col->has_warp_field_interdictor = 0;
        col->has_tachyon = 0;
        col->has_subspace = 0;
        col->has_stargate = 0;
        col->has_jumpgate = 0;
        col->has_artemis_net = 0;
        col->has_dimensional_portal = 0;

        for (int16_t i = 0; i < 5; ++i) {
            int16_t p_idx = col->planet_index[i];
            if (p_idx == -1) continue;

            int16_t c_idx = MOX::_planet[p_idx].colony_index;
            if (c_idx == -1) continue;

            s_colony* colony = &MOX::_colony[c_idx];
            int8_t owner = colony->owner;
            uint8_t owner_bit = (uint8_t)(1 << (owner & 0x1F));

            col->has_colony |= owner_bit;

            if (colony->buildings[BUILDING_WARP_INTERDICTOR] != 0) {
                col->has_warp_field_interdictor |= owner_bit;
            }

            if (colony->buildings[BUILDING_STAR_BASE] != 0 ||
                colony->buildings[BUILDING_BATTLESTATION] != 0 ||
                colony->buildings[BUILDING_STAR_FORTRESS] != 0) {

                if (MOX::_player[owner].tech_applications[TECH_APP_TACHYON_COMMUNICATIONS] == 3) {
                    col->has_tachyon |= owner_bit;
                }
                if (MOX::_player[owner].tech_applications[TECH_APP_SUB_SPACE_COMMUNICATIONS] == 3) {
                    col->has_subspace |= owner_bit;
                }
            }

            if (colony->buildings[BUILDING_ARTEMIS_SYSTEM_NET] != 0) {
                col->has_artemis_net |= owner_bit;
            }

            if (colony->buildings[BUILDING_DIMENSIONAL_PORTAL] != 0) {
                col->has_dimensional_portal |= owner_bit;
            }

            if (MOX::_player[owner].tech_applications[TECH_APP_JUMP_GATE] == 3) {
                col->has_jumpgate |= owner_bit;
            }

            if (MOX::_player[owner].tech_applications[TECH_APP_STAR_GATE] == 3) {
                col->has_stargate |= owner_bit;
            }
        }

        if ((col->has_warp_field_interdictor != 0) != (old_interdictor_state != 0)) {
            No_Op1_();
        }
    }

    void __cdecl Do_Colony_Owner_Change_Star_Colony_Stuff_(s_colony* colony) {
        s_planet_data* planet = &MOX::_planet[colony->planet];
        s_star_data* star = &MOX::_star[planet->star_index];

        COLCALC::Compute_Star_Colony_Stuff_(star);

        if (colony->buildings[BUILDING_WARP_INTERDICTOR] != 0) {
            SHIPMOVE::Update_Ship_ETAs_(-1);
        }
    }

    void __cdecl Surrender_Star_(int16_t star_idx, int16_t player_idx, int16_t to_player_idx) {
        s_star_data* star = &MOX::_star[star_idx];

        star->surrender_to[player_idx] = (int8_t)to_player_idx;

        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            if (star->surrender_to[i] == (int8_t)player_idx) {
                star->surrender_to[i] = (int8_t)to_player_idx;
            }
        }

        if (star->surrender_to[to_player_idx] != (int8_t)-1) {
            int8_t indirect_target = star->surrender_to[to_player_idx];
            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                if (star->surrender_to[i] == (int8_t)to_player_idx) {
                    star->surrender_to[i] = indirect_target;
                }
            }
        }
    }

    void __cdecl Surrender_Empire_(int16_t player_id, int16_t target_id) {
        MOX::_player[player_id].surrender_to = (int8_t)target_id;

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                if (MOX::_player[i].surrender_to == (int8_t)player_id) {
                    MOX::_player[i].surrender_to = (int8_t)target_id;
                }
            }
        }

        if (MOX::_player[target_id].surrender_to != -1) {
            if (MOX::_NUM_PLAYERS > 0) {
                for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                    if (MOX::_player[i].surrender_to == (int8_t)target_id) {
                        MOX::_player[i].surrender_to = MOX::_player[target_id].surrender_to;
                    }
                }
            }
        }
    }

    void __cdecl Init_Surrenders_() {
        for (int i = 0; i < (int)MOX::_NUM_STARS; ++i) {
            for (int j = 0; j < (int)MOX::_NUM_PLAYERS; ++j) {
                MOX::_star[i].surrender_to[j] = -1;
            }
        }

        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            MOX::_player[i].surrender_to = -1;
        }
    }

    bool __cdecl May_Surrender_Empire_(int16_t player_idx, int16_t target_idx) {
        if (MOX::_player[player_idx].surrender_to != (int8_t)target_idx && MOX::_player[target_idx].surrender_to == -1) {
            return true;
        }
        return false;
    }

    void __cdecl No_Op1_() {
        return;
    }

    int16_t __cdecl Player_N_Turns_Until_Research_Complete_(s_player* plr) {
        uint8_t field_idx = static_cast<uint8_t>(plr->current_research_field);

        if (plr->tech_fields[field_idx] == 3) {
            return 0;
        }

        int32_t produced = (int32_t)plr->research_produced;
        int32_t cost = Player_Research_Cost_(plr, field_idx);
        int32_t accumulated = plr->research_accumulated;

        if (produced == 0 && accumulated <= cost) {
            return -1;
        }

        int16_t turns = 0;

        if (accumulated <= cost) {
            do {
                turns++;
                accumulated += produced;
            } while (accumulated <= cost);
        }

        int32_t total_chance = 0;
        do {
            turns++;
            accumulated += produced;
            int32_t current_turn_chance = Chance_For_Research_Breakthrough_Aux_(accumulated, produced, cost);
            total_chance += current_turn_chance;
        } while (total_chance < 100);

        return turns;
    }

    int32_t __cdecl Chance_For_Research_Breakthrough_(s_player* plr) {
        int32_t cost = Player_Research_Cost_(plr, static_cast<uint16_t>(static_cast<uint8_t>(plr->current_research_field)));

        int32_t produced = static_cast<int32_t>(plr->research_produced);
        int32_t accumulated_total = plr->research_accumulated + produced;

        return Chance_For_Research_Breakthrough_Aux_(accumulated_total, produced, cost);
    }

    int32_t __cdecl Chance_For_Research_Breakthrough_Aux_(int32_t accumulated_total, int32_t produced, int32_t cost) {
        int32_t chance = 0;

        if (cost > 0 && cost < accumulated_total) {
            chance = ((accumulated_total - cost) * 100) / cost;

            if (chance > 100) {
                return 100;
            }

            if (chance == 0) {
                chance = 1;
            }
        }

        return chance;
    }

    void __cdecl Pre_Import_Computing_(s_colony* colony) {
        // Reset maintenance accumulators
        colony->maintenance[ECON_FOOD] = 0;
        colony->maintenance[ECON_INDUSTRY] = 0;
        colony->maintenance[ECON_RESEARCH] = 0;
        colony->maintenance[ECON_BC] = 0;

        colony->food2_per_farmer = (int8_t)Colony_Food2_Per_Farmer_(colony, (int16_t)colony->owner, nullptr);
        Colony_Environmental_Stuff_(colony);
        colony->industry_per_worker = (int8_t)Colony_Industry_Per_Worker_(colony, static_cast<int16_t>(static_cast<int8_t>(colony->owner)), nullptr);
        colony->research_per_scientist = (int8_t)Colony_Research_Per_Scientist_(colony, static_cast<int16_t>(static_cast<int8_t>(colony->owner)), nullptr);

        Colony_Morale_(colony, nullptr);
        Colony_Industry_Maintenance_(colony);
        Colony_Industry_Production_(colony, nullptr);
        Colony_Food_Production_(colony, nullptr);
        Colony_Food_Maintenance_(colony);
        Colony_Replicators_(colony);

        // Calculate industry imports needed to cover maintenance, capped by production capacity
        int16_t maintenance_val = static_cast<int16_t>(static_cast<uint8_t>(colony->maintenance[ECON_INDUSTRY]));
        colony->imports[ECON_INDUSTRY] = maintenance_val;
        if (colony->production[ECON_INDUSTRY] <= maintenance_val) {
            colony->imports[ECON_INDUSTRY] = colony->production[ECON_INDUSTRY];
        }

        Colony_Research_Production_(colony, nullptr);
        Colony_Industry_To_Tax_(colony);
        Colony_BC_Production_(colony, nullptr);
        Colony_BC_Maintenance_(colony);
    }

    void __cdecl Recalculate_Colony_(s_colony* colony) {
        Pre_Import_Computing_(colony);

        int16_t owner_id = static_cast<int16_t>(colony->owner);
        Pass_Out_Imports_(owner_id, colony);
    }

    int __cdecl Player_Research_Cost_(s_player* plr, int16_t tech_field) {
        uint32_t total_cost = 0;

        if (tech_field > 0) {
            total_cost = TECHDATA::_technology_fields[tech_field].cost;

            if (tech_field >= 75) {
                uint32_t modifier = static_cast<uint32_t>(plr->hyper_advanced_tech[tech_field - 75]);

                total_cost += modifier * 10000;
            }
        }

        return static_cast<int>(total_cost);
    }

    int16_t __cdecl Colony_Food2_Per_Farmer_(s_colony* colony, int16_t player_idx, s_colony_job_production* out) {
        // TECH_APP_BIOMORPHIC_FUNGI maps to tech_applications[29].
        int16_t food_total = (int16_t)MOX::_planet[colony->planet].food_per_farmer * 2;

        if (out != nullptr) {
            out->base_prod = food_total;
        }

        if (food_total == 0) {
            // Check if player has Biomorphic Fungi (level 3)
            if (MOX::_player[player_idx].tech_applications[TECH_APP_BIOMORPHIC_FUNGI] == 3) {
                food_total = 2;
                if (out != nullptr) {
                    out->tech_apps[TECH_APP_BIOMORPHIC_FUNGI] = 2;
                }
            }
        }

        // Apply building bonuses if current base production is above zero
        if (food_total > 0) {
            // Weather Controller bonus: +4 food
            if (colony->buildings[BUILDING_WEATHER_CONTROLLER] != 0) {
                food_total += 4;
                if (out != nullptr) {
                    out->buildings_prod[BUILDING_WEATHER_CONTROLLER] = 4;
                }
            }
            // Astro University bonus: +2 food
            if (colony->buildings[BUILDING_ASTRO_UNIVERSITY] != 0) {
                food_total += 2;
                if (out != nullptr) {
                    out->buildings_prod[BUILDING_ASTRO_UNIVERSITY] = 2;
                }
            }
        }

        return food_total;
    }

    // Suggested name: Colony_Industry_Per_Worker
    int16_t __cdecl Colony_Industry_Per_Worker_(s_colony* colony, int16_t player_idx, s_colony_job_production* out) {
        int16_t industry;

        int8_t mineral_class = MOX::_planet[colony->planet].mineral_class;
        industry = _minerals_per_mine[mineral_class];

        if (out != nullptr) {
            out->base_prod = industry;
        }

        if (colony->buildings[BUILDING_ASTRO_UNIVERSITY]) {
            industry += 1;
            if (out != nullptr) {
                out->buildings_prod[BUILDING_ASTRO_UNIVERSITY] = 1;
            }
        }

        if (colony->buildings[BUILDING_AUTOMATED_FACTORY]) {
            industry += 1;
            if (out != nullptr) {
                out->buildings_prod[BUILDING_AUTOMATED_FACTORY] = 1;
            }
        }

        if (colony->buildings[BUILDING_ROBO_MINER_PLANT]) {
            industry += 2;
            if (out != nullptr) {
                out->buildings_prod[BUILDING_ROBO_MINER_PLANT] = 2;
            }
        }

        if (colony->buildings[BUILDING_DEEP_CORE_MINE]) {
            industry += 3;
            if (out != nullptr) {
                out->buildings_prod[BUILDING_DEEP_CORE_MINE] = 3;
            }
        }

        if (MOX::_player[player_idx].tech_applications[TECH_APP_MICROLITE_CONSTRUCTION] == 3) {
            industry += 1;
            if (out != nullptr) {
                out->tech_apps[TECH_APP_MICROLITE_CONSTRUCTION] = 1;
            }
        }

        return (int16_t)industry;
    }

    // Suggested name: Colony_Research_Per_Scientist
    int16_t __cdecl Colony_Research_Per_Scientist_(s_colony* colony, int16_t player_idx, s_colony_job_production* out) {
        int16_t research_per_scientist = 3;

        if (out != nullptr) {
            out->base_prod = 3;
        }

        if (colony->buildings[BUILDING_ASTRO_UNIVERSITY] != 0) {
            research_per_scientist = 4;
            if (out != nullptr) {
                out->buildings_prod[BUILDING_ASTRO_UNIVERSITY] = 1;
            }
        }

        if (colony->buildings[BUILDING_SUPERCOMPUTER] != 0) {
            research_per_scientist += 2;
            if (out != nullptr) {
                out->buildings_prod[BUILDING_SUPERCOMPUTER] = 2;
            }
        }

        if (colony->buildings[BUILDING_GALACTIC_CYBERNET] != 0) {
            research_per_scientist += 3;
            if (out != nullptr) {
                out->buildings_prod[BUILDING_GALACTIC_CYBERNET] = 3;
            }
        }

        if (colony->buildings[BUILDING_RESEARCH_LAB] != 0) {
            research_per_scientist += 1;
            if (out != nullptr) {
                out->buildings_prod[BUILDING_RESEARCH_LAB] = 1;
            }
        }

        int8_t special = MOX::_planet[colony->planet].planet_special;

        if (special == SYSTEM_SPECIAL_ANCIENT_ARTIFACTS) {
            research_per_scientist += 2;
            if (out != nullptr) {
                out->artifacts_bonus += 2;
            }
        } else if (special == SYSTEM_SPECIAL_ORION_STAR) {
            research_per_scientist += 5;
            if (out != nullptr) {
                out->artifacts_bonus += 5;
            }
        }

        return research_per_scientist;
    }

    void __cdecl Colony_Environmental_Stuff_(s_colony* colony) {
        int8_t climate = MOX::_planet[colony->planet].climate;
        colony->climate = climate;

        if (climate == PLANET_CLIMATE_RADIATED) {
            if (colony->buildings[BUILDING_BARRIER_SHIELD] != 0 ||
                colony->buildings[BUILDING_FLUX_SHIELD] != 0 ||
                colony->buildings[BUILDING_RADIATION_SHIELD] != 0)
            {
                colony->climate = PLANET_CLIMATE_BARREN;
            }
        }

        int16_t food_per_farmer = Colony_Food2_Per_Farmer_(colony, colony->owner, nullptr);
        if (food_per_farmer != 0) {
            colony->max_farms = -1;
        } else {
            colony->max_farms = 0;
        }
    }

    int32_t __cdecl Mixed_Race_Morale_Penalty_(s_colony* colony) {
        uint8_t n_pops = static_cast<uint8_t>(colony->n_pops);

        if (n_pops > 1 && colony->buildings[BUILDING_ALIEN_CONTROL_CENTER] == 0) {
            uint32_t first_biological_race = 0xFFFFFFFF;

            for (uint32_t i = 0; i < static_cast<uint32_t>(n_pops); ++i) {
                uint32_t race_id = colony->pop[i] & 0x0F;

                if (race_id < 8) {
                    if (first_biological_race == 0xFFFFFFFF) {
                        first_biological_race = race_id;
                    } else if (race_id != first_biological_race) {
                        return -4;
                    }
                }
            }
        }

        return 0;
    }

    bool __cdecl Colony_Is_Blockaded(s_colony* colony) {
        int16_t planet_idx = colony->planet;

        int8_t star_idx = MOX::_planet[planet_idx].star_index;

        uint8_t blockaded_mask = MOX::_star[star_idx].blockaded;

        return (bool)((blockaded_mask >> (colony->owner & 0x1F)) & 1);
    }

    void __cdecl Colony_Specialty_(s_colony* colony) {
        int16_t total_production = 0;
        int16_t max_production = 0;
        int16_t best_index = 0;
        int16_t* production = colony->production;

        for (int16_t i = 0; i < ECON_BC; ++i) {
            int16_t current_val = production[i];
            total_production += current_val;

            if (current_val > max_production) {
                max_production = current_val;
                best_index = i;
            }
        }

        // Logic equivalent to: max_production > floor(total_production / 2)
        if ((total_production / 2) < max_production) {
            colony->specialty = static_cast<int8_t>(best_index);
        } else {
            colony->specialty = -1;
        }
    }

    void __cdecl Post_Import_Computing_(s_colony* colony) {
        COLCALC::Colony_Pop_Grows_(colony);
        COLCALC::Colony_Specialty_(colony);
    }

    int16_t __cdecl Planet_Max_Population_For_Player_(s_planet_data* planet, int16_t player_idx) {
        if (planet->colony_index != -1) {
            s_colony* colony = &MOX::_colony[planet->colony_index];

            if (colony->outpost_flag == 0) {
                if (colony->n_pops == 0) {
                    return Colony_Race_Pop_Limit_(colony, player_idx);
                }

                uint8_t races_present[10];
                memset(races_present, 0, sizeof(races_present));

                for (int i = 0; i < (int)colony->n_pops; ++i) {
                    races_present[colony->pop[i] & 0x0F] = 1;
                }

                int16_t max_limit = 0;
                for (int race = 0; race < 10; ++race) {
                    if (races_present[race]) {
                        int16_t race_limit = Colony_Race_Pop_Limit_(colony, (int16_t)race);
                        if (max_limit < race_limit) {
                            max_limit = race_limit;
                        }
                    }
                }
                return max_limit;
            }
        }

        int16_t pop_limit = Size_And_Climate_Race_Pop_Limit_(planet->size, (int)planet->climate, player_idx);

        if (MOX::_player[player_idx].tech_applications[TECH_APP_ADVANCED_CITY_PLANNING] == 3) {
            pop_limit += 5;
        }

        return pop_limit;
    }

    int16_t __cdecl Size_And_Climate_Race_Pop_Limit_(int8_t size, int climate, int16_t player_idx) {
        bool is_immune = false;

        if (player_idx < 8) {
            if (MOX::_player[player_idx].traits[TRAIT_ENVIRONMENT_IMMUNE] != 0) {
                is_immune = true;
            }
        } else if (player_idx == 8) {
            // Player index 8 is considered immune (e.g., Neutral/Orion)
            is_immune = true;
        }

        int adjusted_climate = COLCALC::Calc_Player_Climate_(player_idx, climate);
        int max_pop = COLCALC::Size_And_Climate_Max_Population_(size, adjusted_climate, is_immune);

        if (player_idx < 8 && MOX::_player[player_idx].traits[TRAIT_SUBTERRANEAN] != 0) {
            max_pop += COLCALC::Double_Add_Two_(size);
        }

        return static_cast<short>(max_pop);
    }

    void __cdecl All_Colony_Calculations_() {
        RUSS::Assert_Settings_();

        for (int32_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];
            if (colony->owner != -1 && colony->outpost_flag == 0) {
                Pre_Import_Computing_(colony);
            }
        }

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            Pass_Out_Imports_(i, nullptr);
        }

        for (int32_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];
            if (colony->owner != -1 && colony->outpost_flag == 0) {
                Post_Import_Computing_(colony);
            }
        }

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            Update_Player_Stats_(&MOX::_player[i]);
        }

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            s_player* player = &MOX::_player[i];
            player->total_research = 0;
            for (int32_t j = 0; j < 83; ++j) {
                if (player->tech_fields[j] == 3) {
                    player->total_research++;
                }
            }
            player->total_ships = 0;
            player->total_colonies = 0;
        }

        for (int32_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];
            if (colony->owner >= 0 && (uint8_t)colony->n_pops > 4) {
                MOX::_player[colony->owner].total_colonies++;
            }
        }

        for (int32_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            if (ship->owner < 8 && ship->status < 5 && ship->d.ship_type == 0) {
                MOX::_player[ship->owner].total_ships += (1 << (ship->d.size & 0x1F));
            }
        }
    }

    short __cdecl Colony_Race_Pop_Limit_(s_colony* colony, int16_t player_idx) {
        // Calculate base limit from planet size and climate
        int16_t pop_limit = (int)Size_And_Climate_Race_Pop_Limit_(
            MOX::_planet[colony->planet].size,
            (uint8_t)colony->climate,
            player_idx
        );

        // Tech bonus: Advanced City Planning
        if (MOX::_player[colony->owner].tech_applications[TECH_APP_ADVANCED_CITY_PLANNING] == 3) {
            pop_limit += 5;
        }

        // Building bonus: Biospheres
        if (colony->buildings[BUILDING_BIOSPHERES] != 0) {
            pop_limit += 2;
        }

        return pop_limit;
    }

    int16_t __cdecl Get_First_Player_Colony_Idx(int32_t player_idx) {
        int16_t index = 0;
        while (MOX::_colony[index].owner != player_idx) {
            ++index;
        }
        return index;
    }

    void __cdecl Colony_Industry_Maintenance_(s_colony* colony) {
        int16_t colony_idx = (int16_t)(((uintptr_t)colony - (uintptr_t)MOX::_colony) / sizeof(s_colony));

        if (EVENTS::Event_Check_Space_Anomaly_(colony_idx) != 0) {
            colony->maintenance[ECON_INDUSTRY] = 0;
            return;
        }

        // Initialize industry requirement fields
        colony->industry2_needed_for_our_empire = 0;
        colony->industry2_needed_for_androids = 0;
        colony->industry2_needed_for_assimilated = 0;
        colony->industry2_needed_for_conquered = 0;
        for (int i = 0; i < 8; ++i) {
            colony->industry2_needed_for_empire[i] = 0;
        }

        int16_t owner_race_idx = (int8_t)colony->owner;
        int n_pops = (uint8_t)colony->n_pops;

        if (n_pops != 0) {
            for (int i = 0; i < n_pops; ++i) {
                uint32_t pop_data = colony->pop[i];
                int16_t race = (int16_t)(pop_data & 0xF);

                if (race < 8) {
                    int8_t cost = (MOX::_player[race].traits[TRAIT_CYBERNETIC] != 0) ? 1 : 0;

                    if (race == owner_race_idx) {
                        colony->industry2_needed_for_our_empire += cost;
                    } else if (((pop_data >> 10) & 1) == 0) {
                        // Bit 10 determines non-conquered (assimilated) vs conquered status
                        colony->industry2_needed_for_assimilated += cost;
                    } else {
                        colony->industry2_needed_for_conquered += cost;
                    }
                    colony->industry2_needed_for_empire[race] += cost;
                } else if (race == 8) {
                    // Androids (race index 8) always contribute 2 to industry maintenance
                    colony->industry2_needed_for_androids += 2;
                }
            }
        }

        // Final colony maintenance is (total_industry_needed + 1) / 2
        int total_needed = (uint8_t)colony->industry2_needed_for_our_empire +
                           (uint8_t)colony->industry2_needed_for_androids +
                           (uint8_t)colony->industry2_needed_for_assimilated +
                           (uint8_t)colony->industry2_needed_for_conquered;

        colony->maintenance[ECON_INDUSTRY] = (int8_t)((total_needed + 1) / 2);
    }

    void __cdecl Colony_Industry_To_Tax_(s_colony* colony) {
        int16_t industry_prod = colony->production[ECON_INDUSTRY];
        int32_t taxable_industry = (int32_t)industry_prod - (int32_t)(uint8_t)colony->maintenance[ECON_INDUSTRY];

        if (taxable_industry < 1) {
            colony->n_industry_taxed = 0;
            return;
        }

        uint8_t tax_rate = MOX::_player[(int8_t)colony->owner].tax_rate;
        int16_t taxed_amount = (int16_t)(((int32_t)tax_rate * taxable_industry) / 100);

        colony->n_industry_taxed = taxed_amount;
        colony->production[ECON_INDUSTRY] = industry_prod - taxed_amount;
    }

    bool __cdecl Monster_At_Star_(int star_idx) {
        s_ship_data* ship_list = MOX::_ship;
        int num_ships = static_cast<int>(MOX::_NUM_SHIPS);

        for (int i = 0; i < num_ships; ++i) {
            s_ship_data* current_ship = &ship_list[i];

            // In MoO2, owner indices 8 and above typically represent monsters or guardians
            if (current_ship->owner >= 8 && current_ship->location == star_idx) {
                return true;
            }
        }

        return false;
    }

    void __cdecl Show_Industry_Production_(short colony_idx) {
        char buffer[3000];

        // MOX::_colony is a pointer to an array of s_colony structures (size 0x169 / 361)
        s_colony* colony = &MOX::_colony[colony_idx];

        Colony_Industry_Production_(colony, buffer);
        TEXTBOX::Text_Box_(_industry_summary_msg, buffer);
    }

    void __cdecl Show_Research_Production_(int16_t colony_idx) {
        char buffer[3000];

        Colony_Research_Production_(&MOX::_colony[colony_idx], buffer);
        TEXTBOX::Text_Box_(_research_summary_msg, buffer);
    }

    void __cdecl Colony_Food_Maintenance_(s_colony* colony) {
        int16_t colony_idx = static_cast<int16_t>(colony - MOX::_colony);

        if (EVENTS::Event_Check_Space_Anomaly_(colony_idx) != 0) {
            colony->maintenance[ECON_FOOD] = 0;
            return;
        }

        // Initialize food requirement trackers
        for (int i = 0; i < 8; ++i) {
            colony->food2_needed_for_empire[i] = 0;
        }
        colony->food2_needed_for_our_empire = 0;
        colony->food2_needed_for_assimilated = 0;
        colony->food2_needed_for_conquered = 0;
        colony->food2_needed_for_natives = 0;

        for (int i = 0; i < (int)(uint8_t)colony->n_pops; ++i) {
            // Read 4-byte pop structure to access bitfields efficiently
            uint32_t pop_bits = *reinterpret_cast<uint32_t*>(&colony->pop[i]);
            uint32_t race = pop_bits & 0xF;
            uint32_t bit_10_flag = (pop_bits >> 10) & 1;

            if (race < 8) {
                s_player* player_ptr = &MOX::_player[race];
                uint8_t food_rate_half;

                if (player_ptr->traits[TRAIT_CYBERNETIC] != 0) {
                    food_rate_half = 1;
                } else if (player_ptr->traits[TRAIT_EATS_MINERALS] != 0) {
                    food_rate_half = 0;
                } else {
                    food_rate_half = 2;
                }

                if (race == (uint32_t)(uint8_t)colony->owner) {
                    colony->food2_needed_for_our_empire += food_rate_half;
                } else if (bit_10_flag == 0) {
                    colony->food2_needed_for_assimilated += food_rate_half;
                } else {
                    colony->food2_needed_for_conquered += food_rate_half;
                }

                colony->food2_needed_for_empire[race] += food_rate_half;
            } else if (race == 9) {
                // Natives consume fixed amount (2 units half-food = 1.0 food)
                colony->food2_needed_for_natives += 2;
            }
        }

        int32_t sum_food2 = (uint8_t)colony->food2_needed_for_our_empire +
                            (uint8_t)colony->food2_needed_for_assimilated +
                            (uint8_t)colony->food2_needed_for_conquered +
                            (uint8_t)colony->food2_needed_for_natives;

        // Final food maintenance is half the total required half-units, rounded up
        colony->maintenance[ECON_FOOD] = (int8_t)((sum_food2 + 1) / 2);
    }

    uint8_t __cdecl Leader_At_Anomaly_(s_leader_data* leader) {
        uint8_t result = 0;

        // Status 1 = Active/At Location, Type 1 = Scientist? (Context dependent)
        if (leader->status == 1 && leader->eta == 0 && leader->type == 1) {
            if (EVENTS::Event_Check_Space_Anomaly_At_Star_(leader->location) != 0) {
                result = 1;
            }
        }

        return result;
    }

    int32_t __cdecl Tribute_Received_From_(int32_t receiver_idx, int32_t payer_idx) {
        s_player* payer = &MOX::_player[payer_idx];

        int32_t tribute_percent = static_cast<int32_t>(payer->tribute_treaty[receiver_idx]);

        int32_t net_production = static_cast<int32_t>(payer->bc_produced) - payer->tribute_income;

        int32_t result = (tribute_percent * net_production) / 20;

        if (result < 0) {
            result = 0;
        }

        return result;
    }

    bool __cdecl Player_Owns_Officer_(s_player* plr, s_leader_data* leader) {
        if ((plr - MOX::_player) == static_cast<intptr_t>(leader->player_index)) {
            if (leader->status == 0 || leader->status == 1) {
                return true;
            }
        }
        return false;
    }

    void __cdecl Colony_BC_Maintenance_(s_colony* colony) {
        if (colony->n_pops == 0) {
            colony->maintenance[ECON_BC] = 0;
        } else {
            int16_t colony_idx = (int16_t)(((int32_t)colony - (int32_t)MOX::_colony) / 0x169);
            if (EVENTS::Event_Check_Space_Anomaly_(colony_idx) != 0) {
                colony->maintenance[ECON_BC] = 0;
            } else {
                int32_t total_building_maintenance = 0;
                for (int32_t i = 0; i < 49; ++i) {
                    if (colony->buildings[i] != 0) {
                        total_building_maintenance += TECHDATA::_buildings[i].maintenance;
                    }
                }

                int32_t climate_modifier = _climate_maintenance_modifiers[(uint8_t)colony->climate] + 100;
                int32_t base_cost = (uint8_t)colony->n_food_replicated + total_building_maintenance;

                int32_t calculated_bc = (((climate_modifier * base_cost) * 4 + 200) * 25) / 10000;
                colony->maintenance[ECON_BC] = (byte)calculated_bc;
            }
        }

        colony->imports[ECON_BC] = static_cast<int16_t>(static_cast<uint8_t>(colony->maintenance[ECON_BC])) - colony->production[ECON_BC];
    }

    short __cdecl Double_Add_Two_(short value) {
        return (value * 2) + 2;
    }

    int __cdecl Size_And_Climate_Max_Population_(int8_t size, int climate, uint8_t is_immune) {
        // Calculate population factor based on size and climate
        int32_t population_factor = (int32_t)is_immune * 25 + _size_climate_max_pop_lookup[climate];

        // Cap the factor at 100%
        if (population_factor > 100) {
            population_factor = 100;
        }

        // Multiply by the planet's specific max population and round to nearest integer
        // Equivalent to: (factor * max_pop + 50) / 100
        int32_t planet_max_val = (int32_t)MOX::_planet_max_population[size];
        int32_t result = (int32_t)(population_factor * planet_max_val + 50) / 100;

        return result;
    }

    void __cdecl Do_Player_Colony_Calculations_(int16_t player_idx) {
        RUSS::Mox_Update_();

        s_colony* colony = MOX::_colony;
        s_colony* end = &MOX::_colony[MOX::_NUM_COLONIES];

        while (colony < end) {
            if (colony->owner == player_idx && colony->outpost_flag == 0) {
                COLCALC::Pre_Import_Computing_(colony);
                RUSS::Mox_Update_();
            }

            colony++;
            end = &MOX::_colony[MOX::_NUM_COLONIES];
        }

        COLCALC::Do_Player_Colony_Post_Production_(player_idx);
        RUSS::Mox_Update_();
    }

    void __cdecl Do_Player_Colony_Post_Production_(int16_t player_idx) {
        Pass_Out_Imports_(player_idx, nullptr);

        s_colony* colony = MOX::_colony;
        s_colony* end = &MOX::_colony[MOX::_NUM_COLONIES];

        while (colony < end) {
            if (colony->owner == player_idx && colony->outpost_flag == 0) {
                Post_Import_Computing_(colony);
                RUSS::Mox_Update_();
            }

            colony++;
            end = &MOX::_colony[MOX::_NUM_COLONIES];
        }

        Update_Player_Stats_(&MOX::_player[player_idx]);
    }

    bool __cdecl Player_May_Get_Tech_App_(s_player* plr, int tech_app) {
        if (plr->tech_applications[tech_app] == 3) {
            return false;
        }

        switch (tech_app) {
            case TECH_APP_NO_TECH:
            case TECH_APP_CAPITOL:
                return false;

            case TECH_APP_CONFEDERATION:
                if (plr->traits[TRAIT_CURRENT_GOVERNMENT] != 0) {
                    return false;
                }
                break;

            case TECH_APP_DIMENSIONAL_PORTAL:
                return (bool)MOX::_settings.xenons_exist;

            case TECH_APP_FEDERATION:
                if (plr->traits[TRAIT_CURRENT_GOVERNMENT] != 4) {
                    return false;
                }
                break;

            case TECH_APP_GALACTIC_UNIFICATION:
                if (plr->traits[TRAIT_CURRENT_GOVERNMENT] != 6) {
                    return false;
                }
                break;

            case TECH_APP_IMPERIUM:
                if (plr->traits[TRAIT_CURRENT_GOVERNMENT] != 2) {
                    return false;
                }
                break;

            default:
                if (MOX::_settings.strategic_combat_flag != 0) {
                    if (TECHDATA::_technology_applications[tech_app].in_strategic == 0) {
                        return false;
                    }
                }

                if (tech_app >= 204) {
                    return false;
                }

                if (TECHDATA::_technology_applications[tech_app].tech_field_id == -1) {
                    return false;
                }
                break;
        }

        return true;
    }

    int16_t __cdecl Gravity_Player_Production_Factor_(const s_colony* colony, int16_t player_idx) {
        if (colony->buildings[BUILDING_GRAVITY_GENERATOR] != 0 || player_idx >= 8) {
            return 4;
        }

        int8_t gravity_class = MOX::_planet[colony->planet].gravity_class;

        if (gravity_class == PLANET_GRAVITY_LOW_G) { // Low Gravity
            const s_player& player = MOX::_player[player_idx];
            return (player.traits[TRAIT_LOW_G_WORLD] == 0) ? 3 : 4;
        }

        if (gravity_class == PLANET_GRAVITY_NORMAL_G) { // Normal Gravity
            const s_player& player = MOX::_player[player_idx];
            return (player.traits[TRAIT_LOW_G_WORLD] == 0) ? 4 : 3;
        }

        if (gravity_class == PLANET_GRAVITY_HEAVY_G) { // Heavy Gravity
            const s_player& player = MOX::_player[player_idx];
            return (player.traits[TRAIT_HEAVY_G_WORLD] == 0) ? 2 : 4;
        }

        // The original binary falls through with an undefined register value here.
        // Valid game data should only use the 0..2 gravity classes above.
        return 4;
    }

    int16_t __cdecl Colony_Pop_Prod_Produced_(const s_colony* colony, const uint32_t* pop_unit, int16_t mode) {
        uint32_t pop_mask = *pop_unit;

        // Call the empire-specific base production function
        int16_t base_prod = _colony_empire_base_functions[mode](
            colony,
            static_cast<int16_t>(colony->owner),
            (int16_t)(pop_mask & 0x0000000F),
            nullptr
        );

        // Apply multipliers if base production is positive
        if (base_prod > 0) {
            if ((pop_mask & 0x400) != 0) {
                base_prod *= 3;
            } else {
                base_prod *= 4;
            }

            // Apply the production factor (e.g., gravity modifier)
            int16_t gravity_factor = Gravity_Player_Production_Factor_(colony, (int16_t)(pop_mask & 0x0000000F));
            base_prod = (int16_t)(gravity_factor * base_prod);
        }

        // If mode is 0, the result is halved
        if (mode == 0) {
            base_prod /= 2;
        }

        return base_prod;
    }

    int16_t __cdecl Colony_Pop_Base_Prod_Produced_(const s_colony* colony, const uint32_t* pop, int16_t job_type, s_colony_job_production* out) {
        int16_t base_produced;
        int16_t min_limit = 0;

        base_produced = _colony_empire_base_functions[job_type](
            colony, 
            static_cast<int16_t>(colony->owner), 
            (int16_t)(*pop & 0x0000000f), 
            out
        );

        if (base_produced < 1) {
            if (job_type != 0 || colony->food2_per_farmer != 0) {
                min_limit = 1;
            }
        }

        if (base_produced < min_limit) {
            base_produced = min_limit;
        }

        return base_produced;
    }

    int32_t __cdecl Colony_Producing_Product_Cost_(s_colony* colony) {
        return Colony_Product_Cost_(colony, colony->producing[0]);
    }

    int32_t __cdecl Colony_Cost_To_Buy_Given_Product_(s_colony* colony, int32_t product, int32_t production_spent) {
        int32_t buy_cost = 0;

        switch (product) {
            case -38:
            case -37:
            case -3:
            case -2:
            case -1:
                // Certain special production projects (e.g., Trade, Housing) cannot be bought.
                break;

            default: {
                int32_t total_cost = (int16_t)Colony_Product_Cost_(colony, (int16_t)product);
                int32_t half_cost = total_cost / 2;
                int32_t tenth_cost = total_cost / 10;

                if (production_spent < total_cost) {
                    if (production_spent >= half_cost) {
                        return (total_cost - production_spent) * 2;
                    }

                    // Accumulate cost for the portion from 50% to 100%
                    buy_cost = (total_cost - half_cost) * 2;

                    if (production_spent >= tenth_cost) {
                        // Progress is between 10% and 50%
                        return buy_cost + (half_cost - production_spent) * 5;
                    }

                    // Progress is below 10%, accumulate cost for 10%-50% and then 0%-10%
                    buy_cost += (half_cost - tenth_cost) * 5;
                    buy_cost += (tenth_cost - production_spent) * 10;
                }
                break;
            }
        }

        return buy_cost;
    }

    int32_t __cdecl Ship_Type_Cost_For_Player_(int32_t ship_type, int16_t player_idx) {
        int32_t cost = 0;

        if (ship_type == 1) {
            cost = 500;
        } else if (ship_type == 2 || ship_type == 4) {
            cost = 100;
        }

        DESIGN::Cost_Reduction_For_Govt_Type_(player_idx, &cost);

        return cost;
    }

    bool __cdecl Colony_Can_Afford_To_Buy_Product_(int16_t colony_idx) {
        s_colony* colony = &MOX::_colony[colony_idx];

        int32_t current_bc = MOX::_player[(int)colony->owner].bc;

        int32_t cost = Colony_Cost_To_Buy_Producing_Product_(colony);

        return current_bc >= cost;
    }

    int32_t __cdecl Colony_Cost_To_Buy_Producing_Product_(s_colony* colony) {
        return COLCALC::Colony_Cost_To_Buy_Given_Product_(
            colony,
            static_cast<int32_t>(colony->producing[0]),
            static_cast<int32_t>(colony->production_spent)
        );
    }

    void __cdecl Colony_Buy_Outright_(int16_t colony_idx) {
        s_colony* colony = &MOX::_colony[colony_idx];
        s_player* player = &MOX::_player[colony->owner];

        int32_t buy_cost = Colony_Cost_To_Buy_Producing_Product_(colony);
        player->bc -= buy_cost;

        int32_t product_total_cost = Colony_Producing_Product_Cost_(colony);
        colony->bought_outright = (int16_t)product_total_cost - colony->production_spent;
    }

    int16_t __cdecl Get_Govt_Type_Tech_App_(int16_t govt_type) {
        switch (govt_type) {
            case 1:
                return TECH_APP_CONFEDERATION;
            case 3:
                return TECH_APP_IMPERIUM;
            case 5:
                return TECH_APP_FEDERATION;
            case 7:
                return TECH_APP_GALACTIC_UNIFICATION;
            default:
                return 0;
        }
    }

    int16_t __cdecl Colony_N_Turns_To_Produce_(s_colony* colony) {
        int32_t cost = Colony_Product_Cost_(colony, colony->producing[0]);
        int32_t spent = (int32_t)colony->production_spent;

        if (spent >= cost) {
            return 1;
        }

        int32_t net_production = (int32_t)colony->production[ECON_INDUSTRY] - static_cast<int32_t>(static_cast<uint8_t>(colony->maintenance[ECON_INDUSTRY]));

        if (net_production <= 0) {
            return -1;
        }

        int32_t remaining_cost = cost - spent;

        int32_t turns = (remaining_cost + net_production - 1) / net_production;

        if (turns <= 0) {
            return 1;
        }

        return (int16_t)turns;
    }

    bool __cdecl Housing_Is_Useless_(s_colony* colony) {
        int16_t max_pop = Planet_Max_Population_For_Player_(&MOX::_planet[colony->planet], colony->owner);

        return max_pop == (uint8_t)colony->n_pops;
    }

    void __cdecl Colony_Calculation_(int16_t colony_idx) {
        s_colony* colony = &MOX::_colony[colony_idx];

        if (colony->outpost_flag == 0) {
            Recalculate_Colony_(colony);
        }

        Update_Player_Stats_(&MOX::_player[colony->owner]);
    }

    int16_t __cdecl N_Colonies_And_Outposts_At_Star_(int16_t star_index) {
        int16_t count = 0;
        for (int i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_index].planet_index[i];

            // Check if planet exists in this orbit and if it has a colony/outpost
            if (planet_idx > -1) {
                if (MOX::_planet[planet_idx].colony_index > -1) {
                    count++;
                }
            }
        }
        return count;
    }

    void __cdecl Ensure_Uncreative_Field_OK_(s_player* player, int field_idx) {
        int selected_tech_id = -1;
        int candidates_count = 0;
        int owned_count = 0;

        s_tech_field_data* field = &TECHDATA::_technology_fields[field_idx];
        int16_t* techs = field->tech;

        for (int i = 0; i < 4; ++i) {
            int tech_id = (int)(int16_t)techs[i];

            if (tech_id > 0) {
                if (player->tech_applications[tech_id] == 1) {
                    // Player already owns a tech in this field
                    owned_count++;
                }
                else if (player->tech_applications[tech_id] == 0) {
                    // Candidate selection logic
                    bool is_valid = true;

                    // Tech 0x34 (52) is restricted if xenons (Antarans) do not exist
                    if (MOX::_settings.xenons_exist == 0 && tech_id == TECH_APP_DIMENSIONAL_PORTAL) {
                        is_valid = false;
                    }

                    // Special field index 6 (Advanced Science) is excluded
                    if (field_idx == 6) {
                        is_valid = false;
                    }

                    // If strategic combat is enabled, restricted to strategic techs
                    if (is_valid && MOX::_settings.strategic_combat_flag != 0) {
                        if (TECHDATA::_technology_applications[tech_id].in_strategic == 0) {
                            is_valid = false;
                        }
                    }

                    if (is_valid) {
                        candidates_count++;
                        // Reservoir sampling: pick new candidate with 1/N probability
                        if (random::Random_(candidates_count) == 1) {
                            selected_tech_id = tech_id;
                        }
                    }
                }
            }
        }

        // If no tech is owned in this field and we found valid candidates, unlock the picked one
        if (owned_count == 0 && candidates_count > 0) {
            player->tech_applications[selected_tech_id] = 1;
        }
    }

    void __cdecl Advance_Player_Field_(s_player* player, int field_idx, bool skip_first_advance) {
        int loop_count = 0;
        int techs_known = 0;

        uint8_t* player_tech_fields = player->tech_fields;
        uint8_t* player_tech_applications = player->tech_applications;

        do {
            if (!skip_first_advance || loop_count != 0) {
                player_tech_fields[field_idx] = 3;
                field_idx = (int)TECHDATA::_technology_fields[field_idx].next_field_id;
                player_tech_fields[field_idx] = 2;
            }

            techs_known = 0;
            int16_t* techs_in_field = TECHDATA::_technology_fields[field_idx].tech;
            for (int i = 0; i < 4; ++i) {
                int16_t tech_id = techs_in_field[i];
                if (tech_id > 0 && player_tech_applications[tech_id] == 1) {
                    techs_known++;
                }
            }

            loop_count++;
        } while (techs_known == 0 && field_idx < 75);
    }

    bool __cdecl Field_Is_Gray_Box_(int32_t field_id) {
        switch (field_id) {
            case 0x16:
            case 0x17:
            case 0x1c:
            case 0x1d:
            case 0x37:
            case 0x39:
                return true;
            default:
                return false;
        }
    }

    void __cdecl Compute_Potential_Colony_Info_(s_player* plr, s_planet_data* planet, s_potential_colony_info* results) {
        results->pop_growth_bonus = 0;
        results->maintenance = 0;
        results->field2_0x4 = 0;
        results->worker_bonus = 0;
        results->food = 0;
        results->industry = 0;
        results->research = 0;

        results->pop_growth_bonus = static_cast<int16_t>(plr->traits[TRAIT_POP_GROWTH_BONUS]);

        if (plr->traits[TRAIT_ENVIRONMENT_IMMUNE] == 0) {
            results->maintenance += static_cast<int16_t>(_climate_maintenance_modifiers[planet->climate]);
        }

        if (planet->gravity_class == PLANET_GRAVITY_LOW_G) {
            if (plr->traits[TRAIT_LOW_G_WORLD] == 0) {
                results->worker_bonus = -25;
            }
        } else if (planet->gravity_class == PLANET_GRAVITY_NORMAL_G) {
            if (plr->traits[TRAIT_LOW_G_WORLD] != 0) {
                results->worker_bonus = -25;
            }
        } else if (planet->gravity_class == PLANET_GRAVITY_HEAVY_G) {
            if (plr->traits[TRAIT_HEAVY_G_WORLD] == 0) {
                results->worker_bonus = -50;
            }
        }

        int16_t food_val = static_cast<int16_t>(planet->food_per_farmer) * 2;

        if (food_val == 0 && plr->tech_applications[TECH_APP_BIOMORPHIC_FUNGI] == 3) {
            food_val = 2;
        }

        results->food = food_val;

        if (food_val > 0) {
            results->food += static_cast<int16_t>(plr->traits[TRAIT_FOOD_BONUS]);

            if (plr->traits[TRAIT_AQUATIC] != 0) {
                int8_t bonus_base = 0;
                bool has_bonus = true;
                switch (planet->climate) {
                    case PLANET_CLIMATE_TUNDRA:
                    case PLANET_CLIMATE_SWAMP:
                        bonus_base = MOX::food_bonus_base_4_6;
                        break;
                    case PLANET_CLIMATE_OCEAN:
                    case PLANET_CLIMATE_TERRAN:
                        bonus_base = MOX::food_bonus_base_5_8;
                        break;
                    default:
                        has_bonus = false;
                        break;
                }
                if (has_bonus) {
                    results->food += (static_cast<int16_t>(bonus_base) - static_cast<int16_t>(MOX::_food_per_farmer_table[planet->climate])) * 2;
                }
            }

            if (results->food < 1) {
                results->food = 1;
            }
        }

        results->industry = static_cast<int16_t>(_minerals_per_mine[planet->mineral_class]);

        if (plr->tech_applications[TECH_APP_MICROLITE_CONSTRUCTION] == 3) {
            results->industry += 1;
        }

        results->industry += static_cast<int16_t>(plr->traits[TRAIT_PRODUCTION_BONUS]);

        if (results->industry < 1) {
            results->industry = 1;
        }

        results->research = 3 + static_cast<int16_t>(plr->traits[TRAIT_RESEARCH_BONUS]);
        if (plr->tech_applications[TECH_APP_HEIGHTENED_INTELLIGENCE] == 3) {
            results->research += 1;
        }

        if (planet->planet_special == SYSTEM_SPECIAL_ANCIENT_ARTIFACTS) {
            results->research += 2;
        } else if (planet->planet_special == SYSTEM_SPECIAL_ORION_STAR) {
            results->research += 5;
        }
    }

    int16_t N_Living_Human_Players_() {
        int16_t human_count = 0;

        for (int32_t i = 0; i < static_cast<int32_t>(MOX::_NUM_PLAYERS); ++i) {
            s_player& player = MOX::_player[i];

            if (player.eliminated == 0 && player.objectives == 100) {
                human_count++;
            }
        }

        return human_count;
    }

    int16_t N_Living_Players_() {
        int16_t living_count = 0;
        int32_t num_players = (int32_t)MOX::_NUM_PLAYERS;

        for (int32_t i = 0; i < num_players; ++i) {
            if (MOX::_player[i].eliminated == 0) {
                living_count++;
            }
        }

        return living_count;
    }

    void __cdecl Apply_All_Player_Changes_() {
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            s_player* plr = &MOX::_player[i];

            if (plr->eliminated == 0) {
                maintain::Player_Maintenance_(plr);

                // Add surplus BC to player treasury
                plr->bc += static_cast<int>(plr->surplus_bc);

                Update_Player_Stats_(plr);
                Check_For_Research_Breakthrough_(plr);
                Compute_Player_Total_Known_Tech_Cost_(plr);

                // Notify if the player is running out of money
                if (plr->surplus_bc < 0 && plr->bc < 50) {
                    s_MSG msg = {};
                    msg.msg_type = MSG_TYPE_FINANCIAL;
                    msg.u.financial.a = (int16_t)plr->bc;
                    msg.u.financial.b = plr->surplus_bc;
                    MSG_::Add_Msg_(msg, (int16_t)i);
                }

                // Flag interesting events like food or freighter shortages
                if (plr->surplus_food < 0 || plr->surplus_freighters < 0) {
                    REPORT::_something_interesting_happened[i] = 1;
                }
            }
        }

        SHIPMOVE::Unmark_Just_Built_AI_Ships_();
    }

    void __cdecl Do_Surrenders_() {
        if (MOX::_NUM_STARS > 0) {
            for (int16_t star_idx = 0; star_idx < (int16_t)MOX::_NUM_STARS; ++star_idx) {
                if (MOX::_NUM_PLAYERS > 0) {
                    for (int16_t player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
                        int8_t target = MOX::_star[star_idx].surrender_to[player_idx];
                        if (target != -1) {
                            Do_Surrender_Star_(star_idx, player_idx, target);
                        }
                    }
                }
            }
        }

        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t player_idx = 0; player_idx < (int16_t)MOX::_NUM_PLAYERS; ++player_idx) {
                int8_t target = MOX::_player[player_idx].surrender_to;
                if (target != -1) {
                    Do_Surrender_Empire_(player_idx, target);
                }
            }
        }
    }

    void __cdecl Compute_Blockades_() {
        // Step 1: Initialize blockade statuses for all systems
        for (int i = 0; i < MOX::_NUM_STARS; ++i) {
            MOX::_star[i].blockaded = 0;
            memset(MOX::_star[i].blockaded_by, 0, 8);
        }

        // Step 2: Calculate blockades based on ship positions
        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];

            // Only active combat ships inside known star systems can blockade
            if (ship->status == 0 && ship->location < 0x48 && ship->d.ship_type == 0) {
                int16_t star_id = (int16_t)ship->location;
                int8_t ship_owner = ship->owner;
                s_star_data* star = &MOX::_star[star_id];

                if (ship_owner < 8) {
                    for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
                        // A blockade occurs if the ship owner is at war with the colony owner
                        if (p != ship_owner && (star->has_colony & (1 << p))) {
                            int8_t treaty = MOX::_player[ship_owner].treaty[p];
                            if (treaty >= 4 && treaty <= 6) { // Hostile/War state
                                star->blockaded |= (1 << p);
                                star->blockaded_by[p] |= (1 << ship_owner);
                            }
                        }
                    }
                } else {
                    // Special entities (monsters/Antarans) blockade all colonies in the system
                    star->blockaded = star->has_colony;
                }
            }
        }

        // Step 3: Apply diplomatic penalties for ongoing blockades
        for (int i = 0; i < MOX::_NUM_STARS; ++i) {
            s_star_data* star = &MOX::_star[i];
            if (star->blockaded != 0) {
                for (int p_target = 0; p_target < MOX::_NUM_PLAYERS; ++p_target) {
                    if ((star->blockaded >> p_target) & 1) {
                        for (int p_attacker = 0; p_attacker < MOX::_NUM_PLAYERS; ++p_attacker) {
                            // Penalty occurs if p_attacker is blockading p_target,
                            // and p_attacker doesn't own a colony here themselves
                            if (((star->blockaded_by[p_target] >> p_attacker) & 1) &&
                                !((star->has_colony >> p_attacker) & 1)) {

                                int16_t penalty = -static_cast<int16_t>(random::Random_(5));
                                diplomac::Change_Relations_(penalty, p_attacker, p_target, 7 /* RELATION_BLOCKADE */, i, 0);
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Finds the highest diplomatic or charisma bonus provided by any leader currently owned by the player.
     * Bit 0x20 in general_skills corresponds to the Diplomat skill, while 0x10 is Charisma.
     */
    int __cdecl Best_Leader_Diplomat_Bonus_(int player_idx) {
        int max_bonus = 0;
        s_player* plr = &MOX::_player[player_idx];

        for (int i = 0; i < 67; ++i) {
            s_leader_data* leader = &MOX::_leaders[i];

            if (Player_Owns_Officer_(plr, leader)) {
                int bonus = 0;

                if (leader->general_skills & 0x20 /* DIPLOMAT */) {
                    int16_t level = OFFICER::Owned_Officer_Level_(i);
                    bonus = (level * 3 + 3) * 5;
                }
                else if (leader->general_skills & 0x10 /* CHARISMA */) {
                    int16_t level = OFFICER::Owned_Officer_Level_(i);
                    bonus = (level * 5 + 5) * 2;
                }

                if (bonus > max_bonus) {
                    max_bonus = bonus;
                }
            }
        }

        return max_bonus;
    }

    int16_t __cdecl Get_Player_Max_Population_(int16_t player_idx) {
        int16_t total_max_pop = 0;
        s_colony* colony = MOX::_colony;

        s_colony* end = &MOX::_colony[MOX::_NUM_COLONIES];

        while (colony < end) {
            if (colony->owner == player_idx && colony->outpost_flag == 0) {
                total_max_pop += Colony_Race_Pop_Limit_(colony, player_idx);
            }
            colony++;

            end = &MOX::_colony[MOX::_NUM_COLONIES];
        }

        return total_max_pop;
    }

    void __cdecl Do_Surrender_Empire_(int16_t surrender_id, int16_t receiver_id) {
        s_player* surrender_plr = &MOX::_player[surrender_id];
        s_player* receiver_plr = &MOX::_player[receiver_id];

        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            if (ship->owner == (int8_t)surrender_id) {
                ship->owner = (int8_t)receiver_id;
                if (ship->officer_index != -1) {
                    OFFICER::Deassign_Officer_(ship->officer_index);
                    ship->officer_index = -1;
                }
            }
        }

        MAINSCR::Update_Ship_Icons_In_Between_Turns_();

        for (int i = 0; i < 83; ++i) {
            if (surrender_plr->tech_applications[i] == 3 /* TECH_OWNED */) {
                if (Player_May_Get_Tech_App_(receiver_plr, (int16_t)i)) {
                    Player_Gets_Tech_App_(receiver_plr, (int16_t)i);
                }
            }
        }

        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];
            if (colony->owner == (int8_t)surrender_id) {
                Do_Surrender_Colony_(colony, receiver_id);
                int8_t star_idx = MOX::_planet[colony->planet].star_index;
                Compute_Star_Colony_Stuff_(&MOX::_star[star_idx]);
            }
        }

        int total_spies = BILL::Get_Their_Spy_Number_(receiver_id, receiver_id);
        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            total_spies += BILL::Get_Their_Spy_Number_(surrender_id, (int16_t)i);
        }

        if (total_spies > 63) {
            total_spies = 63;
        }

        BILL::Set_Their_Spy_Number_(receiver_id, receiver_id, (uint8_t)total_spies);

        receiver_plr->n_freighters += surrender_plr->n_freighters;
        receiver_plr->bc += surrender_plr->bc;
        surrender_plr->surrender_to = -1;

        Eliminate_Player_(surrender_plr);
    }

    void __cdecl Produce_Ground_Military_(s_colony* colony) {
        if (colony->buildings[22] != 0) {
            if (invasion::Colony_Infantry_Barracks_Limit_(colony) > colony->military[0]) {
                ++colony->infantry_roundoff;
                if (colony->infantry_roundoff == 5) {
                    colony->infantry_roundoff = 0;
                    ++colony->military[0];
                }
            } else {
                if (invasion::Colony_Infantry_Limit_(colony) < colony->military[0]) {
                    --colony->military[0];
                }
            }
        }

        if (colony->buildings[2] != 0) {
            if (invasion::Colony_Tank_Barracks_Limit_(colony) > colony->military[1]) {
                ++colony->tank_roundoff;
                if (colony->tank_roundoff == 5) {
                    colony->tank_roundoff = 0;
                    ++colony->military[1];
                    return;
                }
            } else {
                if (invasion::Colony_Tank_Limit_(colony) < colony->military[1]) {
                    --colony->military[1];
                }
            }
        }
    }

    void __cdecl Apply_Colony_Changes_(s_colony* colony) {
        if (EVENTS::Event_Check_Space_Anomaly_(static_cast<int>(colony - MOX::_colony)) == 0) {
            if (colony->occupation_policy == 4) {
                colony->occupation_policy = 3;
            }
            if (colony->n_turns_existed != -1) {
                colony->n_turns_existed++;
            }
            if (colony->n_pops != 0) {
                Apply_Colony_Pop_Growth_(colony);
                Apply_Assimilation_(colony);
                Produce_Ground_Military_(colony);
                Apply_Production_(colony);
            } else {
                Ghost_Colony_Stuff_(colony);
            }
        }
    }

    void __cdecl Apply_Colony_Pop_Growth_(s_colony* colony) {
        const int32_t colony_idx = static_cast<int32_t>(colony - MOX::_colony);

        if (EVENTS::Event_Check_Space_Anomaly_(colony_idx) != 0) {
            return;
        }

        if (colony->n_pops == 0) {
            return;
        }

        if (colony->occupation_policy == 0) {
            int16_t removable_count = 0;
            uint32_t* selected_pop = nullptr;
            const uint8_t pop_count = static_cast<uint8_t>(colony->n_pops);

            for (int32_t i = 0; i < pop_count; ++i) {
                uint32_t* pop = &colony->pop[i];
                const uint32_t race_idx = *pop & 0x0f;

                if (race_idx == 9 ||
                    (race_idx < 8 &&
                     static_cast<int32_t>(static_cast<int8_t>(colony->owner)) != static_cast<int32_t>(race_idx) &&
                     ((*pop & 0x400) != 0))) {
                    ++removable_count;
                    if (random::Random_(removable_count) == 1) {
                        selected_pop = pop;
                    }
                }
            }

            if (removable_count == 0) {
                colony->occupation_policy = 4;
            } else {
                if (removable_count == 1) {
                    colony->occupation_policy = 4;

                    s_MSG msg = {};
                    msg.msg_type = MSG_TYPE_COLONY;
                    msg.u.colony.planet_idx = static_cast<uint8_t>(colony->planet);
                    msg.u.colony.subtype = 2;
                    msg.u.colony.c.assimilation.val = static_cast<uint8_t>(*selected_pop & 0x0f);

                    MSG_::Add_Msg_(msg, static_cast<int16_t>(static_cast<int8_t>(colony->owner)));
                }

                --colony->n_pops;
                *selected_pop = colony->pop[static_cast<uint8_t>(colony->n_pops)];
            }
        }

        if (colony->n_pops == 0) {
            return;
        }

        const int32_t owner_idx = static_cast<int8_t>(colony->owner);
        s_player* const owner_player = &MOX::_player[owner_idx];
        int16_t race_counts[10] = {};
        uint8_t protected_race[10] = {};
        bool had_negative_growth = false;

        {
            const uint8_t pop_count = static_cast<uint8_t>(colony->n_pops);
            for (int32_t i = 0; i < pop_count; ++i) {
                ++race_counts[colony->pop[i] & 0x0f];
            }
        }

        if (race_counts[owner_idx] > 0) {
            protected_race[owner_idx] = 1;
        } else {
            int16_t best_race = -1;
            int16_t best_count = 0;

            for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                if (best_count < race_counts[i]) {
                    best_race = i;
                    best_count = race_counts[i];
                }
            }

            if (best_race != -1) {
                protected_race[best_race] = 1;
            } else if (race_counts[8] > 0) {
                protected_race[8] = 1;
            } else {
                protected_race[9] = 1;
            }
        }

        for (int32_t race_idx = 0; race_idx < 10; ++race_idx) {
            if (race_idx >= MOX::_NUM_PLAYERS && race_idx != 8 && race_idx != 9) {
                continue;
            }

            if (colony->pop_growth[race_idx] >= 0) {
                continue;
            }

            had_negative_growth = true;
            colony->pop_roundoff[race_idx] =
                    static_cast<int16_t>(colony->pop_roundoff[race_idx] + colony->pop_growth[race_idx]);

            while (colony->pop_roundoff[race_idx] < 0) {
                if (static_cast<int32_t>(protected_race[race_idx]) >= race_counts[race_idx]) {
                    colony->pop_roundoff[race_idx] = 0;
                } else {
                    int16_t selected_special = -1;
                    int16_t selected_normal = -1;
                    int16_t special_count = 0;
                    int16_t normal_count = 0;
                    const uint8_t pop_count = static_cast<uint8_t>(colony->n_pops);

                    for (int16_t pop_idx = 0; pop_idx < static_cast<int16_t>(pop_count); ++pop_idx) {
                        const uint32_t pop = colony->pop[pop_idx];
                        if ((pop & 0x0f) != static_cast<uint32_t>(race_idx)) {
                            continue;
                        }

                        if ((pop & 0x180) == 0) {
                            ++normal_count;
                            if (random::Random_(normal_count) == 1) {
                                selected_normal = pop_idx;
                            }
                        } else {
                            ++special_count;
                            if (random::Random_(special_count) == 1) {
                                selected_special = pop_idx;
                            }
                        }
                    }

                    const int16_t remove_idx = (special_count > 0) ? selected_special : selected_normal;

                    --race_counts[race_idx];
                    --colony->n_pops;
                    colony->pop[remove_idx] = colony->pop[static_cast<uint8_t>(colony->n_pops)];
                    colony->pop_roundoff[race_idx] =
                            static_cast<int16_t>(colony->pop_roundoff[race_idx] + 1000);
                }
            }
        }

        if (had_negative_growth) {
            int16_t food_shortage =
                    static_cast<int16_t>(static_cast<uint8_t>(colony->maintenance[0])) -
                    colony->production[0];
            food_shortage = static_cast<int16_t>(food_shortage - colony->imports[0]);

            const int16_t industry_shortage =
                    static_cast<int16_t>(static_cast<uint8_t>(colony->maintenance[1])) -
                    colony->production[1];

            if (food_shortage > 0 || industry_shortage > 0) {
                s_MSG msg = {};
                msg.msg_type = MSG_TYPE_COLONY;
                msg.u.colony.planet_idx = static_cast<uint8_t>(colony->planet);
                msg.u.colony.subtype = 4;

                if (food_shortage > 0) {
                    msg.u.colony.c.starvation.a = (int8_t)food_shortage;
                }
                if (industry_shortage > 0) {
                    msg.u.colony.c.starvation.b = (int8_t)industry_shortage;
                }

                MSG_::Add_Msg_(msg, static_cast<int16_t>(static_cast<int8_t>(colony->owner)));
            }
        }

        int16_t player_order[10] = {};

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            player_order[i] = i;
        }

        {
            const int16_t temp = player_order[0];
            player_order[0] = player_order[owner_idx];
            player_order[owner_idx] = temp;
        }

        RUSS::Shuffle_Sint_(&player_order[1], static_cast<int16_t>(MOX::_NUM_PLAYERS - 1));

        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int16_t order_idx = 0; order_idx < MOX::_NUM_PLAYERS; ++order_idx) {
            const int16_t race_idx = player_order[order_idx];

            if (colony->pop_growth[race_idx] <= 0) {
                continue;
            }

            colony->pop_roundoff[race_idx] =
                    static_cast<int16_t>(colony->pop_roundoff[race_idx] + colony->pop_growth[race_idx]);

            while (colony->pop_roundoff[race_idx] >= 1000) {
                const uint8_t pop_count = static_cast<uint8_t>(colony->n_pops);

                if (COLCALC::Colony_Race_Pop_Limit_(colony, race_idx) <= static_cast<int16_t>(pop_count)) {
                    colony->pop_roundoff[race_idx] = 999;
                    break;
                }

                int32_t job_class = -1;

                if (colony->food2_per_farmer != 0) {
                    const int32_t food_balance =
                            static_cast<uint8_t>(colony->food2_needed_for_our_empire) -
                            static_cast<int32_t>(colony->production[0]) * 2 +
                            static_cast<uint8_t>(colony->food2_needed_for_assimilated) +
                            static_cast<uint8_t>(colony->food2_needed_for_conquered) +
                            static_cast<uint8_t>(colony->food2_needed_for_natives);

                    if (food_balance > 0) {
                        job_class = 0;
                    } else if (food_balance == 0) {
                        if (MOX::_player[race_idx].traits[TRAIT_EATS_MINERALS] == 0) {
                            job_class = 0;
                        }
                    } else if (food_balance == -1) {
                        if (MOX::_player[race_idx].traits[TRAIT_EATS_MINERALS] == 0 &&
                            MOX::_player[race_idx].traits[TRAIT_CYBERNETIC] == 0) {
                            job_class = 0;
                        }
                    }

                    if (job_class != 0) {
                        if (owner_player->surplus_food < 0) {
                            job_class = 0;
                        } else if (owner_player->surplus_food == 0 && owner_player->traits[TRAIT_EATS_MINERALS] == 0) {
                            bool found_later_owned_colony = false;

                            for (s_colony* scan = colony + 1;
                                 scan < MOX::_colony + MOX::_NUM_COLONIES;
                                 ++scan) {
                                if (scan->owner == colony->owner && scan->outpost_flag == 0) {
                                    found_later_owned_colony = true;
                                    break;
                                }
                            }

                            if (found_later_owned_colony) {
                                job_class = 0;
                            }
                        }
                    }
                }

                if (job_class == -1) {
                    const int16_t industry_output = colony->production[1];
                    const int16_t industry_needed = static_cast<uint8_t>(colony->maintenance[1]);

                    if (industry_needed > industry_output) {
                        job_class = 1;
                    } else if (industry_needed == industry_output &&
                               MOX::_player[race_idx].traits[TRAIT_CYBERNETIC] != 0) {
                        const int32_t industry_balance =
                                static_cast<uint8_t>(colony->industry2_needed_for_our_empire) +
                                static_cast<uint8_t>(colony->industry2_needed_for_androids) +
                                static_cast<uint8_t>(colony->industry2_needed_for_assimilated) +
                                static_cast<uint8_t>(colony->industry2_needed_for_conquered);

                        if ((industry_balance & 1) == 0) {
                            job_class = 1;
                        }
                    }
                }

                if (job_class == -1 &&
                    EVENTS::Event_Check_Colony_Researching_(colony_idx) != 0) {
                    job_class = 2;
                }

                if (job_class == -1) {
                    uint32_t job_counts[3] = {};

                    for (int32_t i = 0; i < pop_count; ++i) {
                        ++job_counts[(colony->pop[i] & 0x180) >> 7];
                    }

                    job_class = 1;
                    if (job_counts[1] + job_counts[2] != 0) {
                        job_counts[0] = 0;
                        job_class = RUSS::Get_Weighted_Choice_Int_(job_counts, 3);
                    }
                }

                colony->pop[pop_count] =
                        (static_cast<uint32_t>(job_class) << 7) |
                        0x200 |
                        (static_cast<uint32_t>(race_idx) & 0x0f) |
                        ((static_cast<uint32_t>(static_cast<uint8_t>(colony->owner)) << 4) & 0x70);

                ++colony->n_pops;
                colony->pop_roundoff[race_idx] =
                        static_cast<int16_t>(colony->pop_roundoff[race_idx] - 1000);
            }
        }
    }

    void __cdecl Apply_All_Colony_Changes_() {
        s_colony* current = MOX::_colony;
        int16_t num_colonies = MOX::_NUM_COLONIES;
        if (num_colonies <= 0) {
            return;
        }

        do {
            if (current->owner != -1 && current->outpost_flag == 0) {
                Apply_Colony_Changes_(current);
            }

            ++current;
            num_colonies = MOX::_NUM_COLONIES;
            if (num_colonies <= 0) {
                break;
            }
        } while (current < &MOX::_colony[num_colonies]);
    }

    void __cdecl Check_For_Research_Breakthrough_(s_player* player) {
        uint8_t field = (uint8_t)player->current_research_field;

        if (field == 0) {
            return;
        }

        int32_t breakthrough_chance = Chance_For_Research_Breakthrough_(player);
        player->research_breakthrough = 0;
        player->research_accumulated += (int32_t)player->research_produced;

        if ((int32_t)random::Random_(100) <= breakthrough_chance) {
            player->research_breakthrough = 1;
            player->research_accumulated = 0;
            Give_Player_Field_(player, field, 0);
        }
    }

    bool __cdecl May_Surrender_Star_(int16_t star_idx, int16_t player_idx, int16_t recipient_idx) {
        if (MOX::_player[player_idx].home_planet_id != -1) {
            if (MOX::_planet[MOX::_player[player_idx].home_planet_id].star_index == star_idx) {
                return false;
            }
        }

        // Check if the system is already set to surrender to this recipient
        if (MOX::_star[star_idx].surrender_to[recipient_idx] == (int8_t)player_idx) {
            return false;
        }

        // Check if the player already has a surrender order for this system
        if (MOX::_star[star_idx].surrender_to[player_idx] != -1) {
            return false;
        }

        // Recipient must be able to reach the star
        if (!SHIPMOVE::Star_In_Normal_Range_Of_Player_(&MOX::_star[star_idx], (int16_t)recipient_idx)) {
            return false;
        }

        bool has_other_colony = false;
        // Verify player has colonies in other star systems
        for (int i = 0; i < (int)MOX::_NUM_STARS; ++i) {
            // has_colony is a bitmask of players present in the system
            if (((uint8_t)MOX::_star[i].has_colony >> (uint8_t)player_idx) & 1) {
                if (&MOX::_star[i] != &MOX::_star[star_idx]) {
                    has_other_colony = true;
                }
            }
        }

        return has_other_colony;
    }

    void __cdecl Do_Surrender_Colony_(s_colony* colony, int16_t new_owner) {
        int8_t old_owner = colony->owner;

        // Check for Capitol building and remove it upon surrender
        if (colony->buildings[BUILDING_CAPITOL] != 0) {
            colony->buildings[BUILDING_CAPITOL] = 0;
        }

        colony->owner = (char)new_owner;
        colony->auto_building = 0;

        // Update ownership of individual population units
        for (int i = 0; i < (int)(uint8_t)colony->n_pops; ++i) {
            uint32_t pop_val = colony->pop[i];
            // Race/Owner bits are 4-6
            if (((pop_val & 0x70) >> 4) == (uint32_t)old_owner) {
                colony->pop[i] = (pop_val & ~0x70) | (((uint32_t)new_owner << 4) & 0x70);
            }
        }

        Do_Colony_Owner_Change_Star_Colony_Stuff_(colony);
        Colony_Environmental_Stuff_(colony);
        invasion::Enforce_Population_Limits_At_Colony_(colony);

        int16_t colony_idx = (int16_t)(colony - MOX::_colony);
        COLBLDG::Clear_Colony_Queue_(colony_idx);
        COLCALC::Colony_Calculation_(colony_idx);

        colony->n_turns_existed = 0;

        // Update the 'visited' bitmask for the star system to reflect the new owner
        int star_idx = MOX::_planet[colony->planet].star_index;
        MOX::_star[star_idx].visited |= (1 << (uint8_t)new_owner);
    }

    void __cdecl Do_Surrender_Star_(int16_t star_idx, int16_t old_owner, int16_t new_owner) {
        s_star_data& star = MOX::_star[star_idx];

        for (int i = 0; i < 5; ++i) {
            int16_t planet_idx = star.planet_index[i];
            if (planet_idx != -1) {
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;
                if (colony_idx != -1) {
                    s_colony& colony = MOX::_colony[colony_idx];
                    if (colony.owner == (int8_t)old_owner) {
                        COLCALC::Do_Surrender_Colony_(&colony, new_owner);
                    }
                }
            }
        }

        // Update ownership bitmasks
        star.has_colony &= ~(1 << (uint8_t)old_owner);
        star.has_colony |= (1 << (uint8_t)new_owner);
        star.visited |= (1 << (uint8_t)new_owner);

        COLCALC::Compute_Star_Colony_Stuff_(&star);

        star.colonize_player = -1;

        if (star.officer_index[old_owner] != -1) {
            OFFICER::Move_Officer_To_Pool_((int16_t)star.officer_index[old_owner]);
            star.officer_index[old_owner] = -1;
        }

        star.surrender_to[old_owner] = -1;
    }

    void __cdecl Give_Player_Field_(s_player* player, int field_id, char get_all_flag) {
        if (field_id < 75 /* HYPER_ADVANCED_THRESHOLD */) {
            Advance_Player_Field_(player, field_id, false);

            bool get_all = (player->traits[TRAIT_CREATIVE] != 0) || (get_all_flag != 0);
            bool update_ship_designs = false;

            // Certain fields automatically grant all technologies or require ship design recalculation
            if (field_id >= 22 && field_id <= 57) {
                switch (field_id) {
                    case 22: /* CONSTRUCTION_22 - e.g., Battleship hulls */
                    case 55: /* CONSTRUCTION_55 - e.g., Doom Star hulls */
                    case 57: /* CONSTRUCTION_57 - e.g., Core Waste Dump? */
                        update_ship_designs = true;
                        get_all = true;
                        break;
                    case 23:
                    case 28:
                    case 29:
                        get_all = true;
                        break;
                    default:
                        break;
                }
            }

            if (get_all) {
                // Creative players or forced breakthrough: get all available technologies in the field
                int16_t* techs = TECHDATA::_technology_fields[field_id].tech;
                for (int i = 0; i < 4; ++i) {
                    int16_t tech_id = techs[i];
                    // tech_applications[id] == 1 usually means it was just researched but not yet applied
                    if (tech_id > 0 && player->tech_applications[tech_id] == 1) {
                        Player_Gets_Tech_App_(player, (int16_t)tech_id);
                    }
                }

                if (update_ship_designs) {
                    short player_idx = static_cast<short>(player - MOX::_player);
                    INITSHIP::Init_Ship_Designs_(player_idx);
                    return;
                }
            } else {
                // Non-Creative: only get the specific technology they were researching
                uint16_t tech_id = static_cast<uint16_t>(static_cast<uint8_t>(player->current_research_application));
                Player_Gets_Tech_App_(player, (int16_t)tech_id);
            }
        } else {
            // Hyper-advanced technology increment
            player->hyper_advanced_tech[field_id - 75]++;
        }
    }

    int32_t __cdecl Colony_Product_Cost_(s_colony* colony, int16_t product_id) {
        if (product_id >= 1 && product_id < BUILDING_COUNT) {
            int32_t base_cost = TECHDATA::_buildings[product_id].cost;

            switch (product_id) {
                case BUILDING_BARRIER_SHIELD:
                    if (colony->buildings[BUILDING_FLUX_SHIELD]) {
                        return base_cost - TECHDATA::_buildings[BUILDING_FLUX_SHIELD].cost;
                    }
                    if (colony->buildings[BUILDING_RADIATION_SHIELD]) {
                        return base_cost - TECHDATA::_buildings[BUILDING_RADIATION_SHIELD].cost;
                    }
                    return base_cost;

                case BUILDING_FLUX_SHIELD:
                    if (colony->buildings[BUILDING_RADIATION_SHIELD]) {
                        return base_cost - TECHDATA::_buildings[BUILDING_RADIATION_SHIELD].cost;
                    }
                    return base_cost;

                case BUILDING_STAR_FORTRESS:
                    if (colony->buildings[BUILDING_BATTLESTATION]) {
                        return base_cost - TECHDATA::_buildings[BUILDING_BATTLESTATION].cost;
                    }
                    if (colony->buildings[BUILDING_STAR_BASE]) {
                        return base_cost - TECHDATA::_buildings[BUILDING_STAR_BASE].cost;
                    }
                    return base_cost;

                case BUILDING_BATTLESTATION:
                    if (colony->buildings[BUILDING_STAR_BASE]) {
                        return base_cost - TECHDATA::_buildings[BUILDING_STAR_BASE].cost;
                    }
                    return base_cost;

                case BUILDING_TERRAFORMING:
                {
                    int32_t n_terraforms = (int8_t)MOX::_planet[colony->planet].n_terraforms;
                    return base_cost + (n_terraforms * 250);
                }

                default:
                    return base_cost;
            }
        }

        if (product_id <= -100) {
            int ship_index = -100 - product_id;
            return MOX::_ship[ship_index].d.cost;
        }

        if (product_id <= -50) {
            int design_index = -50 - product_id;
            return MOX::_player[colony->owner].ship_designs[design_index].cost;
        }

        switch (product_id) {
            case -17: // Transport
                return Ship_Type_Cost_For_Player_(4, colony->owner);

            case -12: // Colony Ship
                return Ship_Type_Cost_For_Player_(1, colony->owner);

            case -11: // Freighter
                return Ship_Type_Cost_For_Player_(2, colony->owner);

            case -7: // Spy
                return 100;

            case -15: // Outpost?
            case -6:
            case -5:
            case -4:
                return 50;

            default:
                return 0;
        }
    }

    void __cdecl Compute_Player_Total_Known_Tech_Cost_(s_player* player) {
        player->total_known_tech_cost = 0;

        // Process standard technology fields (0 to 74)
        for (int i = 0; i < 75; ++i) {
            s_tech_field_data& field = TECHDATA::_technology_fields[i];

            int divisor = 1;
            int numerator = (player->tech_fields[i] == 3) ? 1 : 0;

            int16_t* techs = field.tech;

            for (int j = 0; j < 4; ++j) {
                int16_t tech_id = techs[j];
                if (tech_id > 0) {
                    divisor++;
                    if (player->tech_applications[tech_id] == 3) {
                        numerator++;
                    }
                }
            }

            player->total_known_tech_cost += (int32_t)((uint32_t)field.cost * (uint32_t)numerator) / divisor;
        }

        // Process hyper-advanced technology fields (75 to 82)
        for (int i = 75; i < 83; ++i) {
            uint8_t level = player->hyper_advanced_tech[i - 75];
            if (level > 0) {
                const s_tech_field_data& field = TECHDATA::_technology_fields[i];
                // Cost calculation: Base Cost + (Level - 1) * 10000
                player->total_known_tech_cost += field.cost + (level - 1) * 10000;
            }
        }
    }

    bool __cdecl Colony_Can_Build_Large_Ships_(s_colony *colony) {
        if (colony->buildings[BUILDING_STAR_BASE] != 0 ||
            colony->buildings[BUILDING_BATTLESTATION] != 0 ||
            colony->buildings[BUILDING_STAR_FORTRESS] != 0) {
            return true;
        }
        return false;
    }

    /**
     * @address 004fb870
     */
    void __cdecl Check_For_Eliminated_Players_() {
        uint8_t has_colonies[8];
        memset(has_colonies, 0, sizeof(has_colonies));

        // Mark players that currently own at least one colony (excluding outposts)
        for (int i = 0; i < (int)MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];
            if ((uint8_t)colony->owner < 8 && colony->outpost_flag == 0) {
                has_colonies[(uint8_t)colony->owner] = 1;
            }
        }

        // Check each player to see if they should be eliminated
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            s_player* player = &MOX::_player[i];

            if (player->eliminated == 0) {
                // If the player has no colonies left, eliminate them from the game
                if (has_colonies[i] == 0) {
                    Eliminate_Player_(player);
                    EVENTS::Event_Race_Eliminated_((int8_t)i);
                }

                // Reset the last attacker info for this turn
                player->last_attacker = -1;
            }
        }
    }

    void __cdecl Delete_Obsolete_Artifical_Planets_Under_Construction_(int16_t star_idx) {
        if (!COLCALC::Star_Has_Asteroid_Or_Gas_Giant_(star_idx)) {
            for (int32_t planet_slot = 0; planet_slot < 5; ++planet_slot) {
                const int16_t planet_idx = MOX::_star[star_idx].planet_index[planet_slot];
                if (planet_idx != -1) {
                    const int16_t colony_idx = MOX::_planet[planet_idx].colony_index;
                    if (colony_idx != -1) {
                        COLBLDG::Delete_Prod_From_Colony_Queue_(colony_idx, 0x30);
                    }
                }
            }
        }
    }

    char* __cdecl Format_Quotient_(int32_t numerator, int32_t denominator) {
        int32_t val = numerator;

        if (val < 0) {
            val = (int32_t)(0u - (uint32_t)val);
            _tmp_quotient[0] = '-';
        } else {
            _tmp_quotient[0] = '+';
        }

        int32_t n_chars = sprintf(&_tmp_quotient[1], DESIGN::s_pct_d, val / denominator);
        char* fraction_ptr = &_tmp_quotient[1] + n_chars;

        int32_t remainder = val % denominator;
        if (remainder != 0) {
            int32_t multiplier = 10;
            int32_t count = 1;
            int32_t scaled_remainder = (int32_t)((uint32_t)remainder * (uint32_t)multiplier);

            while (((scaled_remainder / denominator) * denominator) != scaled_remainder) {
                if (count >= 5) {
                    break;
                }

                multiplier = (int32_t)((uint32_t)multiplier * 10u);
                count++;
                scaled_remainder = (int32_t)((uint32_t)remainder * (uint32_t)multiplier);
            }

            if (multiplier != 0) {
                sprintf(fraction_ptr, s_dot_d_0055a528, scaled_remainder / denominator);
            }
        }

        return _tmp_quotient;
    }

    short __cdecl N_Prods_In_Colony_Queue_(short colony_idx, short item_id) {
        short count = 0;
        for (short i = 0; i < 7; i++) {
            if (MOX::_colony[colony_idx].producing[i] == item_id) {
                count++;
            }
        }
        return count;
    }

    bool __cdecl Star_Has_Asteroid_Or_Gas_Giant_(int star_index) {
        bool found = false;
        
        // s_star_data size is 113 (0x71). planet_index is at offset 0x4a.
        for (int i = 0; i < 5; ++i) {
            short planet_index = MOX::_star[star_index].planet_index[i];
            
            if (planet_index != -1) {
                // s_planet_data size is 17 (0x11). planet_type is at offset 0x4.
                int8_t planet_type = MOX::_planet[planet_index].planet_type;
                
                // Checks for specific planet types (likely 1 = Asteroids, 2 = Gas Giant)
                if (planet_type == PLANET_TYPE_ASTEROID || planet_type == PLANET_TYPE_GAS_GIANT) {
                    found = true;
                }
            }
        }
        
        return found;
    }

    void __cdecl Eliminate_Player_(s_player* player) {
        int16_t player_idx = static_cast<int16_t>(player - MOX::_player);

        player->eliminated = 1;

        // Destroy all outposts owned by the eliminated player
        for (int i = 0; i < (int)MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];
            if (colony->owner == player_idx && colony->outpost_flag != 0) {
                BOMB::Destroy_Colony_(colony, -1);
            }
        }

        // Release all leaders hired by the eliminated player
        for (int i = 0; i < 67; ++i) {
            s_leader_data* leader = &MOX::_leaders[i];
            if (leader->player_index == player_idx) {
                leader->status = -2 /* STATUS_RELEASED? */;
                leader->eta = 0;
                if (leader->location != -1) {
                    if (leader->type == 0 /* LEADER_TYPE_SHIP */) {
                        MOX::_ship[leader->location].officer_index = -1;
                    } else {
                        MOX::_star[leader->location].officer_index[player_idx] = -1;
                    }
                    leader->location = -1;
                }
                leader->player_index = -1;
            }
        }

        // Destroy all ships owned by the eliminated player
        for (int i = 0; i < (int)MOX::_NUM_SHIPS; ++i) {
            if (MOX::_ship[i].owner == player_idx) {
                SHIPS::Kill_Ship_(static_cast<int16_t>(i));
            }
        }

        MAINSCR::Update_Ship_Icons_In_Between_Turns_();

        // Clean up diplomatic relations and spies for/against this player
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            BILL::Set_Their_Spy_Number_(player_idx, static_cast<int16_t>(i), 0);
            if (player_idx != i) {
                spy::Bring_Home_Spies_Vs_(static_cast<int16_t>(i), (int16_t)player_idx);
            }

            // Clear treaties from others towards the eliminated player
            MOX::_player[i].treaty[player_idx] = 0;
            MOX::_player[i].trade_treaty[player_idx] = 0;
            MOX::_player[i].research_treaty[player_idx] = 0;
            MOX::_player[i].tribute_treaty[player_idx] = 0;

            // Clear treaties from the eliminated player towards others
            MOX::_player[player_idx].treaty[i] = 0;
            MOX::_player[player_idx].trade_treaty[i] = 0;
            MOX::_player[player_idx].research_treaty[i] = 0;
            MOX::_player[player_idx].tribute_treaty[i] = 0;
        }

        // Reset economic/logistic stats
        player->n_freighters = 0;
        player->bc = 0;
        player->settlers_freighted = 0;

        // Record the date of defeat for the attacker
        if (player->last_attacker >= 0 && player->last_attacker < 8) {
            MOX::_player[player->last_attacker].player_defeated[player_idx] = static_cast<uint16_t>(MOX::_stardate);
        }
    }

    char* __cdecl Player_Name_(int32_t player_idx) {
        if (player_idx < 8) {
            return MOX::_player[player_idx].race_name;
        }

        return MOX::_race_names[player_idx + 5];
    }

    int16_t __cdecl Colony_Planetary_Shield_(s_colony* colony) {
        if (colony->buildings[BUILDING_BARRIER_SHIELD] != 0) {
            return 3;
        }

        if (colony->buildings[BUILDING_FLUX_SHIELD] != 0) {
            return 2;
        }

        if (colony->buildings[BUILDING_RADIATION_SHIELD] != 0) {
            return 1;
        }

        return 0;
    }

    uint8_t __cdecl Colony_Can_Build_Product_(s_colony* colony, int16_t product) {
        uint8_t owner = colony->owner;

        if (product > 0 && product < 49) {
            if (MOX::_player[owner].tech_applications[TECHDATA::_buildings[product].tech_app] == 3 &&
                colony->buildings[product] == 0) {
                
                switch (product) {
                    case 1: {
                        if (colony->n_pops > 0) {
                            int32_t first_race = -1;
                            for (int i = 0; i < colony->n_pops; ++i) {
                                if (colony->pop[i] & 0x400) {
                                    return 1;
                                }
                                uint32_t race = colony->pop[i] & 0xF;
                                if (race < 8) {
                                    if (first_race == -1) {
                                        first_race = race;
                                    } else if (first_race != race) {
                                        if (MOX::_player[owner].traits[0] / 2 != 3) {
                                            return 1;
                                        }
                                    }
                                }
                            }
                        }
                        return 2;
                    }
                    case 3: {
                        uint8_t retval = 1;
                        int16_t star_index = MOX::_planet[colony->planet].star_index;
                        for (int i = 0; i < 5; ++i) {
                            int16_t p = MOX::_star[star_index].planet_index[i];
                            if (p != -1) {
                                int16_t c = MOX::_planet[p].colony_index;
                                if (c != -1 && MOX::_colony[c].owner == owner) {
                                    if (MOX::_colony[c].buildings[3] != 0 || 
                                        COLCALC::N_Prods_In_Colony_Queue_(c, 3) >= 1) {
                                        retval = 2;
                                    }
                                }
                            }
                        }
                        return retval;
                    }
                    case 5:
                    case 32: {
                        if (colony->buildings[13] != 0) return 0;
                        return 1;
                    }
                    case 9: {
                        if (MOX::_player[owner].traits[0] / 2 == 3) return 0;
                        int16_t c = MOX::_planet[MOX::_player[owner].home_planet_id].colony_index;
                        if (MOX::_colony[c].buildings[9] != 0) return 0;
                        return 1;
                    }
                    case 11: {
                        bool has_planet_type_3 = false;
                        int16_t star_index = MOX::_planet[colony->planet].star_index;
                        for (int i = 0; i < 5; ++i) {
                            int16_t p = MOX::_star[star_index].planet_index[i];
                            if (p != -1) {
                                if (MOX::_planet[p].planet_type == 3) {
                                    has_planet_type_3 = true;
                                    if (MOX::_planet[p].colony_index == -1) {
                                        return 1;
                                    }
                                }
                            }
                        }
                        if (has_planet_type_3) return 2;
                        return 0;
                    }
                    case 14: {
                        return 1;
                    }
                    case 17: {
                        if (colony->climate == 8) return 1;
                        return 0;
                    }
                    case 18: {
                        return 0;
                    }
                    case 20:
                    case 31: {
                        if (MOX::_player[owner].traits[0] / 2 == 3) return 0;
                        return 1;
                    }
                    case 25: {
                        if (colony->n_pops > 0) {
                            int8_t gravity = MOX::_planet[colony->planet].gravity_class;
                            for (int i = 0; i < colony->n_pops; ++i) {
                                uint32_t race = colony->pop[i] & 0xF;
                                if (race < 8) {
                                    if (gravity == 0) {
                                        if (MOX::_player[race].traits[10] == 0) return 1;
                                    } else if (gravity == 1) {
                                        if (MOX::_player[race].traits[10] != 0) return 1;
                                    } else if (gravity == 2) {
                                        if (MOX::_player[race].traits[11] == 0) return 1;
                                    }
                                }
                            }
                        }
                        return 2;
                    }
                    case 28:
                        if (colony->buildings[24] != 0) return 0;
                        // fallthrough
                    case 24:
                        if (colony->buildings[23] != 0) return 0;
                        return 1;
                    case 37: {
                        if ((MOX::_planet[colony->planet].environmental_alterations & 2) != 0) return 0;
                        if (colony->climate >= 3 && colony->climate <= 9) return 1;
                        return 0;
                    }
                    case 40:
                        if (colony->buildings[8] != 0) return 0;
                        // fallthrough
                    case 8:
                        if (colony->buildings[41] != 0) return 0;
                        return 1;
                    case 44: {
                        if (colony->climate >= 2 && colony->climate <= 7) return 1;
                        return 0;
                    }
                    case 46: {
                        return (colony->food2_per_farmer == 0) ? 2 : 1;
                    }
                    case 48: {
                        return COLCALC::Star_Has_Asteroid_Or_Gas_Giant_(MOX::_planet[colony->planet].star_index);
                    }
                    default:
                        return 1;
                }
            }
        } else {
            if (product <= -50) {
                if (MOX::_player[owner].ship_range == 0 || MOX::_player[owner].ship_speed == 0) return 0;
                
                s_ship_design* ship_design;
                if (product <= -100) {
                    ship_design = &MOX::_ship[-product - 100].d;
                } else {
                    ship_design = &MOX::_player[owner].ship_designs[-product - 50];
                }
                
                switch (ship_design->ship_type) {
                    case 0:
                        if (ship_design->size < 2) return 1;
                        if (COLCALC::Colony_Can_Build_Large_Ships_(colony)) {
                            if (ship_design->size == 4) {
                                if (MOX::_player[owner].tech_applications[186] == 3) return 1;
                                return 0;
                            }
                            if (ship_design->size == 5) {
                                if (MOX::_player[owner].tech_applications[55] == 3) return 1;
                                return 0;
                            }
                            return 1;
                        }
                        return 0;
                    case 1:
                        if (MOX::_player[owner].tech_applications[41] == 3) return 1;
                        return 0;
                    case 2:
                        if (colony->buildings[22] != 0 && MOX::_player[owner].tech_applications[189] == 3) return 1;
                        return 0;
                    case 4:
                        if (MOX::_player[owner].tech_applications[109] == 3) return 1;
                        return 0;
                    default:
                        return 0;
                }
            } else {
                switch (product) {
                    case -38:
                    case -37:
                    case -7:
                    case -2:
                    case -1:
                        return 1;
                    case -15:
                        if (MOX::_player[owner].tech_applications[69] == 3) return 1;
                        return 0;
                    case -6:
                        if (MOX::_player[owner].tech_applications[7] == 3) {
                            return (COLCALC::Room_For_Another_Dude_(colony, 8) == 1) ? 1 : 2;
                        }
                        return 0;
                    case -5:
                        if (MOX::_player[owner].tech_applications[8] == 3) {
                            return (COLCALC::Room_For_Another_Dude_(colony, 8) == 1) ? 1 : 2;
                        }
                        return 0;
                    case -4:
                        if (MOX::_player[owner].tech_applications[6] == 3) {
                            return (COLCALC::Room_For_Another_Dude_(colony, 8) == 1) ? 1 : 2;
                        }
                        return 0;
                    case -3:
                        return 1;
                    default:
                        return 0;
                }
            }
        }
        return 0;
    }

    /**
     * @brief Identifies systems owned by a player that are eligible for surrender.
     * @param player_idx Index of the player potentially surrendering systems.
     * @param target_player_idx Index of the player the systems would surrender to.
     * @param star_list Output array to store indices of eligible stars.
     * @return Number of eligible systems found.
     */
    short __cdecl Get_Possible_Surrender_Systems_(short player_idx, short target_player_idx, uint8_t *star_list) {
        short result_count = 0;
        short range = MOX::_player[target_player_idx].ship_range;
        short colony_system_count = 0;
        
        for (int i = 0; i < MOX::_NUM_STARS; ++i) {
            s_star_data* star = &MOX::_star[i];
            
            // Check if player has any colony in this system via bitmask
            if ((star->has_colony >> (player_idx & 0x1f)) & 1) {
                colony_system_count++;
                
                if (SHIPMOVE::Star_In_Range_Of_Player_(star, target_player_idx, range)) {
                    bool has_warp_gate = false;
                    
                    // Check if any planet in the system has a Warp Gate/Stargate (building index 9)
                    for (int p = 4; p >= 0; --p) {
                        short planet_idx = star->planet_index[p];
                        if (planet_idx != -1) {
                            short colony_idx = MOX::_planet[planet_idx].colony_index;
                            if (colony_idx != -1) {
                                s_colony* colony = &MOX::_colony[colony_idx];
                                if (colony->owner == (char)player_idx && colony->buildings[9] != 0) {
                                    has_warp_gate = true;
                                    break;
                                }
                            }
                        }
                    }
                    
                    // Systems with Warp Gates are not eligible for surrender in this context
                    if (!has_warp_gate) {
                        if (star_list != nullptr) {
                            star_list[result_count] = (uint8_t)i;
                        }
                        result_count++;
                    }
                }
            }
        }
        
        // A player will not surrender if they only have one system remaining
        if (colony_system_count == 1) {
            result_count = 0;
        }
        
        return result_count;
    }

    short __cdecl Get_Surrender_System_(short player_idx, short param_2, short selection_mode) {
        uint8_t possible_stars[72];
        s_star_weight star_weights[72];
        short result_star_idx = -1;

        // Populate potential stars to surrender
        short num_stars = Get_Possible_Surrender_Systems_(player_idx, param_2, possible_stars);

        if (num_stars != 0) {
            for (int i = 0; i < (int)num_stars; ++i) {
                uint8_t star_idx = possible_stars[i];
                star_weights[i].star_idx = star_idx;
                star_weights[i].weight = 0;

                // Calculate weight based on total population of colonies owned by the player in this system
                for (int p = 0; p < 5; ++p) {
                    short planet_idx = MOX::_star[star_idx].planet_index[p];
                    if (planet_idx != -1) {
                        short colony_idx = MOX::_planet[planet_idx].colony_index;
                        if (colony_idx != -1 && MOX::_colony[colony_idx].owner == player_idx) {
                            star_weights[i].weight += MOX::_colony[colony_idx].n_pops;
                        }
                    }
                }
            }

            // Sort systems by population weight
            qsort(star_weights, (size_t)num_stars, sizeof(s_star_weight), Sort_star_weights_);

            int half_count = (num_stars + 1) / 2;
            int chosen_idx;

            if (selection_mode == 0) {
                // Pick from the "bottom" (lower weight) half
                chosen_idx = random::Random_(half_count) - 1;
            } else {
                // Pick from the "top" (higher weight) half
                chosen_idx = num_stars - random::Random_(half_count);
            }

            result_star_idx = (short)star_weights[chosen_idx].star_idx;
        }

        return result_star_idx;
    }

    int32_t __cdecl Colony_N_Pops_Of_Empire_(s_colony* colony, uint32_t empire_id) {
        int32_t pop_count = 0;
        uint32_t* pop = colony->pop;
        uint32_t* pop_end = colony->pop + static_cast<uint8_t>(colony->n_pops);

        while (pop < pop_end) {
            if ((*pop & 0xF) == empire_id) {
                ++pop_count;
            }
            ++pop;
        }

        return pop_count;
    }

    bool __cdecl Room_For_Another_Dude_(s_colony* colony, int race_idx) {
        uint8_t pop_limit_flags[42];
        int pop_counts[10];
        int pop_limits[10];
        bool no_room = false;

        memset(pop_limit_flags, 0, sizeof(pop_limit_flags));
        memset(pop_counts, 0, sizeof(pop_counts));

        for (int i = 0; i < 10; ++i) {
            pop_limits[i] = -1;
        }

        int16_t race_limit = Colony_Race_Pop_Limit_(colony, (int16_t)race_idx);
        pop_limit_flags[race_limit] = 1;
        pop_limits[race_idx] = race_limit;

        for (int i = 0; i < colony->n_pops; ++i) {
            int race = colony->pop[i] & 0xF;
            pop_counts[race]++;

            if (pop_limits[race] == -1) {
                int16_t limit = Colony_Race_Pop_Limit_(colony, (int16_t)race);
                pop_limits[race] = limit;
                pop_limit_flags[limit] = 1;
            }
        }

        for (int limit = pop_limits[race_idx]; limit < 42; ++limit) {
            if (pop_limit_flags[limit] != 0) {
                int sum = 0;
                for (int race = 0; race < 10; ++race) {
                    if (pop_counts[race] > 0 && pop_limits[race] <= limit) {
                        sum += pop_counts[race];
                    }
                }
                if (limit <= sum) {
                    no_room = true;
                }
            }
        }

        return !no_room;
    }

    void __cdecl Compute_Total_Building_Costs_(int32_t* player_costs) {
        memset(player_costs, 0, 8 * sizeof(int32_t));

        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];

            if (colony->owner != -1 && colony->outpost_flag == 0) {
                for (int b = 0; b < 49; ++b) {
                    if (colony->buildings[b] != 0) {
                        player_costs[colony->owner] += TECHDATA::_buildings[b].cost;
                    }
                }
            }
        }
    }

    int32_t __cdecl Sort_star_weights_(const void* a, const void* b) {
        auto lhs = static_cast<const s_star_weight*>(a);
        auto rhs = static_cast<const s_star_weight*>(b);
        return static_cast<int32_t>(lhs->weight) - static_cast<int32_t>(rhs->weight);
    }

    char* __cdecl Load_Colcalc_Strings_(char* strings) {
        char* cursor = strings;

        COLCALC::_anomaly_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_no_barracks_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_unification_has_no_morale_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_government_morale_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_no_capital_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_mixed_races_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_bonus_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_leader_bonus_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_total_morale_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_morale_summary_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_produced_by_farmers_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_aquatic_bonus_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_native_bonus_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_android_bonus_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_base_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_gravity_penalty_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_conquered_penalty_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_blockaded_penalty_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_morale_bonus_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_morale_penalty_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_government_bonus_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_government_penalty_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_total_food_produced_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_food_summary_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_produced_by_workers_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_used_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_pollution_penalty_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_replicated_to_d_food_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_taxed_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_total_industry_produced_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_industry_summary_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_produced_by_scientists_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_artifacts_bonus_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_intelligence_bonus_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_total_research_produced_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_research_summary_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_taxes_collected_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_special_income_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_trade_goods_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_taxed_from_industry_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_total_income_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_bc_summary_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_starting_command_points_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_d_from_d_star_bases_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_d_from_1_star_base_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_d_from_d_battlestations_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_d_from_1_battlestation_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_d_from_d_fortresses_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_d_from_1_fortress_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_from_warlord_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_from_imperium_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_total_command_points_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_d_from_1_s_ship_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_d_from_d_s_ships_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_d_from_1_noncombat_ship_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_d_from_d_noncombat_ships_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_total_command_point_used_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_total_command_points_used_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_ship_maintenance_costs_msg = cursor;
        cursor += strlen(cursor) + 1;

        COLCALC::_command_summary_msg = cursor;
        return cursor + strlen(cursor) + 1;
    }

    void __cdecl Apply_Assimilation_(s_colony* colony) {
        const int32_t colony_index = static_cast<int32_t>(colony - MOX::_colony);
        if (EVENTS::Event_Check_Space_Anomaly_(colony_index) != 0) {
            return;
        }

        if (colony->occupation_policy != 3) {
            return;
        }

        const int32_t colony_owner = static_cast<int8_t>(colony->owner);
        const uint32_t owner_bits = static_cast<uint32_t>(static_cast<uint8_t>(colony_owner)) << 4;
        const uint8_t pop_count = static_cast<uint8_t>(colony->n_pops);

        uint32_t occupation_points = static_cast<uint8_t>(colony->occupation_points);
        uint32_t assimilation_gain =
            static_cast<uint8_t>(
                COLCALC::_assimilation_rate_by_government[
                    static_cast<int8_t>(MOX::_player[colony_owner].traits[TRAIT_CURRENT_GOVERNMENT])]);

        for (uint32_t* pop_record_ptr = colony->pop;
             pop_record_ptr < colony->pop + pop_count;
             ++pop_record_ptr) {
            uint32_t pop_record = *pop_record_ptr;
            const uint32_t pop_owner = (pop_record & 0x70u) >> 4;

            if (MOX::_player[pop_owner].traits[TRAIT_CURRENT_GOVERNMENT] == 0) {
                *pop_record_ptr = (((owner_bits ^ pop_record) & 0x70u) ^ pop_record);
            }
        }

        if (colony->buildings[BUILDING_ALIEN_CONTROL_CENTER] != 0) {
            assimilation_gain = 0x78;
        }

        if (MOX::_player[colony_owner].traits[TRAIT_CHARISMATIC] != 0) {
            assimilation_gain *= 2;
        } else if (MOX::_player[colony_owner].traits[TRAIT_REPULSIVE] != 0) {
            assimilation_gain /= 2;
        }

        occupation_points += assimilation_gain;

        if (occupation_points >= 0xF0) {
            uint32_t conversions_remaining = occupation_points / 0xF0;
            occupation_points %= 0xF0;

            do {
                int16_t candidate_count = 0;
                int16_t chosen_pop_idx = -1;

                for (int16_t pop_idx = 0; pop_idx < pop_count; ++pop_idx) {
                    const uint32_t pop_record = colony->pop[pop_idx];
                    if ((pop_record & 0x400u) != 0 && (pop_record & 0x0Fu) < 8) {
                        ++candidate_count;
                        if (random::Random_(candidate_count) == 1) {
                            chosen_pop_idx = pop_idx;
                        }
                    }
                }

                if (candidate_count != 0) {
                    if (candidate_count == 1) {
                        colony->occupation_policy = 4;

                        s_MSG msg = {};
                        msg.msg_type = MSG_TYPE_COLONY;
                        msg.u.colony.planet_idx = static_cast<uint8_t>(colony->planet);
                        msg.u.colony.subtype = 3;
                        msg.u.colony.c.assimilation.val =
                            static_cast<uint8_t>(colony->pop[chosen_pop_idx] & 0x0Fu);

                        MSG_::Add_Msg_(msg, static_cast<int16_t>(colony_owner));
                    }

                    uint32_t pop_record = colony->pop[chosen_pop_idx] & 0xFFFFFBFFu;
                    colony->pop[chosen_pop_idx] = (((owner_bits ^ pop_record) & 0x70u) ^ pop_record);
                }

                --conversions_remaining;
            } while (conversions_remaining != 0);
        }

        colony->occupation_points = static_cast<char>(occupation_points);
    }

    void __cdecl Apply_Production_(s_colony* colony) {
        const int32_t owner_player_idx = static_cast<int32_t>(colony->owner);
        const int32_t colony_idx = static_cast<int32_t>(colony - MOX::_colony);
        s_player& player = MOX::_player[owner_player_idx];
        bool removed_existing_pop = false;

        if (EVENTS::Event_Check_Space_Anomaly_(static_cast<int16_t>(colony_idx)) != 0) {
            return;
        }

        int16_t current_product = colony->producing[0];
        if (colony->auto_building != 0 && current_product >= -3 && current_product <= -1) {
            AIBUILD::Player_Colony_Autobuild_(colony);
        }

        const int32_t product_cost = static_cast<int32_t>(COLCALC::Colony_Producing_Product_Cost_(colony));
        colony->just_produced = -1;
        current_product = colony->producing[0];

        if (current_product == -3) {
            if (!COLCALC::Housing_Is_Useless_(colony)) {
                return;
            }

            colony->just_produced = current_product;
            colony->producing[0] = -1;
            COLONY::Fix_Colony_Queue_(static_cast<int16_t>(colony_idx));
            return;
        }

        if (current_product >= -2 && current_product <= -1) {
            return;
        }

        int32_t production_pool =
            static_cast<int32_t>(colony->production_spent) + static_cast<int32_t>(colony->bought_outright);
        colony->bought_outright = 0;

        if (static_cast<int32_t>(static_cast<uint8_t>(colony->maintenance[1])) <
            static_cast<int32_t>(colony->production[1])) {
            production_pool += static_cast<int32_t>(colony->production[1]) -
                               static_cast<int32_t>(static_cast<uint8_t>(colony->maintenance[1]));
        }

        if (production_pool >= product_cost) {
            s_MSG build_msg{};
            build_msg.msg_type = MSG_TYPE_COLONY;
            build_msg.u.colony.planet_idx = static_cast<uint8_t>(colony->planet);
            build_msg.u.colony.subtype = 1;
            build_msg.u.colony.c.build.build_id = current_product;

            if (current_product < -99) {
                const int32_t ship_idx = -100 - current_product;
                s_ship_data* ship = &MOX::_ship[ship_idx];

                if (ship->status == 4) {
                    build_msg.u.colony.c.build.is_refit = 1;
                } else if (colony->buildings[0x26] != 0) {
                    ++ship->crew_quality;
                    ship->crew_experience =
                        static_cast<int16_t>(MOX::_crew_data[static_cast<int32_t>(ship->crew_quality)].experience);
                }
            }

            if (colony->producing[1] == -1) {
                colony->just_produced = colony->producing[0];
            }

            if (current_product > 0 && current_product < 0x31) {
                ERICNET::Add_Building_(static_cast<int16_t>(colony_idx), current_product);
                player.bc += static_cast<int32_t>(TECHDATA::_buildings[current_product].maintenance);

                if (colony->producing[1] == -10) {
                    COLCALC::Colony_Calculation_(static_cast<int16_t>(colony_idx));
                    if (!COLCALC::Colony_Can_Build_Product_(colony, current_product)) {
                        colony->producing[0] = -1;
                    }
                } else {
                    colony->producing[0] = -1;
                }

                if (current_product == 0x2C) {
                    if (!COLCALC::Colony_Can_Build_Product_(colony, 0x2C)) {
                        COLBLDG::Delete_Prod_From_Colony_Queue_(static_cast<int16_t>(colony_idx), 0x2C);
                    }
                } else if (current_product == 0x30) {
                    const int32_t planet_idx = static_cast<int32_t>(colony->planet);
                    const int32_t star_idx = static_cast<int32_t>(MOX::_planet[planet_idx].star_index);
                    COLCALC::Delete_Obsolete_Artifical_Planets_Under_Construction_(static_cast<int16_t>(star_idx));
                }
            } else {
                if (current_product < -99) {
                    const int32_t ship_idx = -100 - current_product;
                    s_ship_data* ship = &MOX::_ship[ship_idx];

                    if (ship->status == 4) {
                        ship->status = 0;
                        ship->d.cost = DESIGN::Cost_Of_Design_(static_cast<int16_t>(owner_player_idx), &ship->d);
                        colony->producing[0] = -1;
                    } else if (ship->status == 6) {
                        ship->status = 0;

                        if (colony->producing[1] == -10) {
                            const int16_t created_ship_idx =
                                COLBLDG::Create_Ship_(static_cast<int16_t>(owner_player_idx), 0, static_cast<int16_t>(colony_idx));

                            if (created_ship_idx != -1) {
                                memcpy(&MOX::_ship[created_ship_idx].d, &ship->d, sizeof(s_ship_design));
                                MOX::_ship[created_ship_idx].status = 6;
                                colony->producing[0] = static_cast<int16_t>(-100 - created_ship_idx);
                            } else {
                                colony->producing[0] = -1;
                            }
                        } else {
                            colony->producing[0] = -1;
                        }
                    }

                    ship->just_built = 1;
                    MOX::_star[static_cast<int32_t>(ship->location)].ignore_combat_ships = 0;

                    if (ship->d.ship_type == 1 || ship->d.ship_type == 4) {
                        MOX::_star[static_cast<int32_t>(ship->location)].ignore_colony_ships |=
                            static_cast<uint8_t>(1u << static_cast<uint8_t>(ship->owner));
                    }
                } else {
                    switch (current_product) {
                    case -15:
                        player.n_freighters = static_cast<int16_t>(player.n_freighters + 5);
                        player.bc += 5;
                        break;

                    case -7:
                        if (BILL::Add_Their_Spy_(static_cast<int16_t>(owner_player_idx), static_cast<int16_t>(owner_player_idx)) == 0) {
                            player.bc += 100;
                            COLBLDG::Delete_Prod_From_Colony_Queue_(static_cast<int16_t>(colony_idx), -7);
                        }
                        ++player.bc;
                        break;

                    case -6:
                    case -5:
                    case -4: {
                        bool can_add_pop = true;
                        const int32_t empire_pop_limit =
                            static_cast<int32_t>(COLCALC::Colony_Race_Pop_Limit_(colony, 8));
                        const int32_t empire_pop_count =
                            COLCALC::Colony_N_Pops_Of_Empire_(colony, 8);

                        if (empire_pop_limit <= empire_pop_count) {
                            can_add_pop = false;
                        } else if (!COLCALC::Room_For_Another_Dude_(colony, 8)) {
                            uint32_t* type_9_pop = nullptr;
                            uint32_t* flagged_pop = nullptr;
                            uint32_t* foreign_pop = nullptr;
                            uint32_t* own_pop = nullptr;

                            uint32_t* pop_ptr = &colony->pop[0];
                            uint32_t* pop_end = &colony->pop[static_cast<uint8_t>(colony->n_pops)];

                            while (pop_ptr < pop_end) {
                                const uint32_t pop_value = *pop_ptr;
                                const uint32_t pop_type = pop_value & 0x0F;

                                if (pop_type == 9) {
                                    type_9_pop = pop_ptr;
                                } else if (pop_type != 8) {
                                    if ((pop_value & 0x400) != 0) {
                                        flagged_pop = pop_ptr;
                                    } else if (owner_player_idx != static_cast<int32_t>(pop_type)) {
                                        foreign_pop = pop_ptr;
                                    } else {
                                        own_pop = pop_ptr;
                                    }
                                }

                                ++pop_ptr;
                            }

                            uint32_t* selected_pop = type_9_pop;
                            if (selected_pop == nullptr) {
                                selected_pop = flagged_pop;
                            }
                            if (selected_pop == nullptr) {
                                selected_pop = foreign_pop;
                            }
                            if (selected_pop == nullptr) {
                                selected_pop = own_pop;
                            }

                            if (selected_pop == nullptr) {
                                can_add_pop = false;
                            } else {
                                s_MSG pop_removed_msg{};
                                pop_removed_msg.msg_type = MSG_TYPE_COLONY;
                                pop_removed_msg.u.colony.planet_idx = static_cast<uint8_t>(colony->planet);
                                pop_removed_msg.u.colony.subtype = 11;
                                pop_removed_msg.u.colony.c.android_killed.race_idx =
                                    static_cast<int8_t>(*selected_pop & 0x0F);
                                MSG_::Add_Msg_(pop_removed_msg, static_cast<int16_t>(owner_player_idx));

                                const uint8_t last_pop_index =
                                    static_cast<uint8_t>(static_cast<uint8_t>(colony->n_pops) - 1);
                                colony->n_pops = last_pop_index;
                                *selected_pop = colony->pop[last_pop_index];
                                removed_existing_pop = true;
                            }
                        }

                        if (can_add_pop) {
                            uint32_t& new_pop = colony->pop[static_cast<uint8_t>(colony->n_pops)];
                            new_pop = 0;
                            new_pop |= 0x200;
                            new_pop = (new_pop & 0xFFFFFFF8u) | 0x8u;
                            new_pop = ((((static_cast<uint32_t>(owner_player_idx) << 4) ^ new_pop) & 0x70u) ^ new_pop);

                            if (current_product == -6) {
                                new_pop = (new_pop & 0xFFFFFF7Fu) | 0x100u;
                            } else if (current_product == -5) {
                                new_pop = (new_pop & 0xFFFFFEFFu) | 0x80u;
                            } else if (current_product == -4) {
                                new_pop &= 0xFFFFFE7Fu;
                            }

                            colony->n_pops = static_cast<uint8_t>(colony->n_pops + 1);
                        } else {
                            s_MSG pop_failed_msg{};
                            pop_failed_msg.msg_type = MSG_TYPE_COLONY;
                            pop_failed_msg.u.colony.planet_idx = static_cast<uint8_t>(colony->planet);
                            pop_failed_msg.u.colony.subtype = 10;
                            MSG_::Add_Msg_(pop_failed_msg, static_cast<int16_t>(owner_player_idx));

                            production_pool += product_cost;
                        }
                        break;
                    }

                    default:
                        break;
                    }

                    if (colony->producing[1] == -10) {
                        if (removed_existing_pop) {
                            colony->producing[0] = -1;
                            colony->producing[1] = -1;
                        }
                    } else {
                        colony->producing[0] = -1;
                    }
                }
            }

            production_pool -= product_cost;
            COLONY::Fix_Colony_Queue_(static_cast<int16_t>(colony_idx));

            if (colony->producing[0] == -1 && colony->auto_building != 0) {
                AIBUILD::Player_Colony_Autobuild_(colony);
                build_msg.field6_0xd = 1;
            }

            build_msg.u.colony.c.build.product_id = colony->producing[0];
            MSG_::Add_Msg_(build_msg, static_cast<int16_t>(owner_player_idx));
        }

        if (production_pool >= 0x7FFF) {
            colony->production_spent = 0x7FFF;
            return;
        }

        colony->production_spent = static_cast<int16_t>(production_pool);
    }

    void __cdecl Ghost_Colony_Stuff_(s_colony* colony) {
        int32_t building_idx = 0;
        int32_t surviving_building_count = 0;

        for (building_idx = 0; building_idx < 49; ++building_idx) {
            if (colony->buildings[building_idx] != 0) {
                if (random::Random_(10) == 1) {
                    const int32_t colony_idx = static_cast<int32_t>(colony - MOX::_colony);
                    ERICNET::Remove_Building_(static_cast<int16_t>(colony_idx), static_cast<int16_t>(building_idx));
                } else {
                    ++surviving_building_count;
                }
            }
        }

        if (surviving_building_count == 0) {
            bool has_incoming_settlers = false;

            for (int32_t player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
                s_player* player = &MOX::_player[player_idx];

                if (player->settlers_freighted > 0) {
                    for (int32_t settler_idx = 0; settler_idx < player->settlers_freighted; ++settler_idx) {
                        if ((int32_t)((player->settlers[settler_idx].val >> 8) & 0xffu) == colony->planet) {
                            has_incoming_settlers = true;
                        }
                    }
                }
            }

            if (!has_incoming_settlers) {
                BOMB::Destroy_Colony_(colony, 3);
            }
        }
    }
}
