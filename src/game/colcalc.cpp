#include "deps.h"

namespace COLCALC {
    int __cdecl Calc_Player_Climate_(int player_idx, int climate) {
        if (player_idx < 8 && MOX::_player[player_idx].traits[TRAIT_AQUATIC] != 0) {
            switch (climate) {
                case PLANET_CLIMAGE_TUNDRA: // 4
                case PLANET_CLIMAGE_SWAMP: // 6
                    return PLANET_CLIMAGE_TERRAN; // 8
                case PLANET_CLIMAGE_OCEAN: // 5
                case PLANET_CLIMAGE_TERRAN: // 8
                    return PLANET_CLIMAGE_GAIA; // 9
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
        MOX::Mox_Update_();

        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            Twiddle_Initial_Homeworld_(&MOX::_colony[i]);
        }

        SHIPMOVE::Update_All_Ship_Ranges_();
        All_Colony_Calculations_();

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            s_player* player = &MOX::_player[i];
            int star_idx = MOX::_planet[player->home_planet_id].star_index;
            MOX::_star[star_idx].colonize_player = (uint8_t)i;

            if (player->command_points < (int16_t)min_command_points) {
                min_command_points = player->command_points;
            }
        }

        if (MOX::_settings.starting_civilization_level == 2) {
            int starting_ship_points = (min_command_points - 2) / 2;

            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                s_player* player = &MOX::_player[i];
                MOX::Mox_Update_();

                int star_idx = MOX::_planet[player->home_planet_id].star_index;
                MOX::_star[star_idx].colonize_player = (uint8_t)i;

                player->bc = (player->traits[0] * 100 + 200);

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

                int first_colony = Get_First_Player_Colony_Idx(i);
                int pool_remaining = min_command_points;

                int max_design_idx = 0;
                if (starting_ship_points >= 0) {
                    max_design_idx = starting_ship_points;
                    if (starting_ship_points >= 4) {
                        if (player->tech_applications[TECH_APP_TITAN_CONSTRUCTION] == 3) {
                            max_design_idx = 4;
                        } else {
                            max_design_idx = 3;
                        }
                    }
                }

                while (pool_remaining != 0) {
                    MOX::Mox_Update_();
                    int ship_design = max_design_idx;
                    if (pool_remaining - 1 < ship_design) {
                        ship_design = pool_remaining - 1;
                    }

                    MOX::_ship[MOX::_NUM_SHIPS].owner = (int8_t)i;
                    INITSHIP::Update_Player_Ship_Designs_((int16_t)i);
                    COLBLDG::Create_Ship_((int16_t)i, (int16_t)ship_design, (int16_t)first_colony);

                    pool_remaining -= (ship_design + 1);
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
            uint8_t owner = colony->owner;
            uint8_t owner_bit = 1 << owner;

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

    int16_t __cdecl Player_N_Turns_Until_Research_Complete_(s_player* plr) {
        uint8_t field_idx = static_cast<uint8_t>(plr->current_research_field);

        if (plr->tech_fields[field_idx] == 3) {
            return 0;
        }

        int32_t produced = (int32_t)plr->research_produced;
        int32_t cost = Player_Research_Cost_(plr, (uint16_t)field_idx);
        int32_t accumulated = plr->research_accumulated;

        if (produced == 0 && accumulated <= cost) {
            return -1;
        }

        int16_t turns = 0;

        while (accumulated <= cost) {
            turns++;
            accumulated += produced;
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

        colony->food2_per_farmer = (int8_t)Colony_Food2_Per_Farmer_(colony, static_cast<int16_t>(static_cast<int8_t>(colony->owner)), nullptr);
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

    int16_t __cdecl Colony_Food2_Per_Farmer_(s_colony* colony, int16_t player_idx, void* out) {
        // TECH_APP_BIOMORPHIC_FUNGI is assumed to be index 8 in tech_applications array.
        int16_t food_total = (int16_t)MOX::_planet[colony->planet].food_per_farmer * 2;

        if (out != nullptr) {
            *reinterpret_cast<int32_t*>(out) = (int32_t)food_total;
        }

        if (food_total == 0) {
            // Check if player has Biomorphic Fungi (level 3)
            if (MOX::_player[player_idx].tech_applications[TECH_APP_BIOMORPHIC_FUNGI] == 3) {
                food_total = 2;
                if (out != nullptr) {
                    *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x52) = 2; // Offset 0x52
                }
            }
        }

        // Apply building bonuses if current base production is above zero
        if (food_total > 0) {
            // Weather Controller bonus: +4 food
            if (colony->buildings[BUILDING_WEATHER_CONTROLLER] != 0) {
                food_total += 4;
                if (out != nullptr) {
                    *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x21c) = 4; // Offset 0x21c
                }
            }
            // Astro University bonus: +2 food
            if (colony->buildings[BUILDING_ASTRO_UNIVERSITY] != 0) {
                food_total += 2;
                if (out != nullptr) {
                    *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x1c8) = 2; // Offset 0x1c8
                }
            }
        }

        return food_total;
    }

    // Suggested name: Colony_Industry_Per_Worker
    int16_t __cdecl Colony_Industry_Per_Worker_(s_colony* colony, int16_t player_idx, void* out) {
        int16_t industry;

        int8_t mineral_class = MOX::_planet[colony->planet].mineral_class;
        industry = _minerals_per_mine[mineral_class];

        if (out != nullptr) {
            *reinterpret_cast<int32_t*>(out) = industry;
        }

        if (colony->buildings[BUILDING_ASTRO_UNIVERSITY]) {
            industry += 1;
            if (out != nullptr) {
                *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x1c8) = 1;
            }
        }

        if (colony->buildings[BUILDING_AUTOMATED_FACTORY]) {
            industry += 1;
            if (out != nullptr) {
                *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x1ce) = 1;
            }
        }

        if (colony->buildings[BUILDING_ROBO_MINER_PLANT]) {
            industry += 2;
            if (out != nullptr) {
                *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x208) = 2;
            }
        }

        if (colony->buildings[BUILDING_DEEP_CORE_MINE]) {
            industry += 3;
            if (out != nullptr) {
                *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x1d8) = 3;
            }
        }

        // Tech: Microlite Construction
        if (MOX::_player[player_idx].tech_applications[TECH_APP_MICROLITE_CONSTRUCTION] == 3) {
            industry += 1;
            if (out != nullptr) {
                *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x208) = 2;
                reinterpret_cast<int16_t*>(out)[0xf0] = 2;
            }
        }

        return (int16_t)industry;
    }

    // Suggested name: Colony_Research_Per_Scientist
    int16_t __cdecl Colony_Research_Per_Scientist_(s_colony* colony, int16_t player_idx, void* out) {
        int16_t research_per_scientist = 3;

        if (out != nullptr) {
            *reinterpret_cast<int32_t*>(out) = 3;
        }

        if (colony->buildings[BUILDING_ASTRO_UNIVERSITY] != 0) {
            research_per_scientist = 4;
            if (out != nullptr) {
                *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x1c8) = 1;
            }
        }

        if (colony->buildings[BUILDING_SUPERCOMPUTER] != 0) {
            research_per_scientist += 2;
            if (out != nullptr) {
                *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x1fc) = 2;
            }
        }

        if (colony->buildings[BUILDING_GALACTIC_CYBERNET] != 0) {
            research_per_scientist += 3;
            if (out != nullptr) {
                *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x1e6) = 3;
            }
        }

        if (colony->buildings[BUILDING_RESEARCH_LAB] != 0) {
            research_per_scientist += 1;
            if (out != nullptr) {
                *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x206) = 1;
            }
        }

        int8_t special = MOX::_planet[colony->planet].planet_special;

        if (special == SYSTEM_SPECIAL_ANCIENT_ARTIFACTS) {
            research_per_scientist += 2;
            if (out != nullptr) {
                *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x226) += 2;
            }
        } else if (special == SYSTEM_SPECIAL_ORION_STAR) {
            research_per_scientist += 5;
            if (out != nullptr) {
                *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(out) + 0x226) += 5;
            }
        }

        return research_per_scientist;
    }

    void __cdecl Colony_Environmental_Stuff_(s_colony* colony) {
        int8_t climate = MOX::_planet[colony->planet].climate;
        colony->climate = climate;

        if (climate == PLANET_CLIMAGE_RADIATED) {
            if (colony->buildings[BUILDING_BARRIER_SHIELD] != 0 ||
                colony->buildings[BUILDING_FLUX_SHIELD] != 0 ||
                colony->buildings[BUILDING_RADIATION_SHIELD] != 0)
            {
                colony->climate = PLANET_CLIMAGE_BARREN;
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
        int n_pops = colony->n_pops;

        if (n_pops < 2 || colony->buildings[BUILDING_ALIEN_CONTROL_CENTER] != 0) {
            return 0;
        }

        uint32_t first_standard_race = 0xFFFFFFFF;

        for (int i = 0; i < n_pops; ++i) {
            uint32_t current_race = colony->pop[i].race;

            if (current_race < 8) {
                if (first_standard_race == 0xFFFFFFFF) {
                    first_standard_race = current_race;
                } else if (current_race != first_standard_race) {
                    return -4;
                }
            }
        }

        return 0;
    }

    void __cdecl Colony_Morale_(s_colony* colony, char* debug) {
        int8_t owner = colony->owner;
        s_player* player = &MOX::_player[owner];
        int8_t* traits = player->traits;
        int8_t trait0 = traits[TRAIT_CURRENT_GOVERNMENT];
        int8_t gov_type = trait0 / 2;

        int leader_bonus = 0;
        int no_capital_penalty = 0;
        int psychophysics_bonus = 0;
        int holo_simulator_bonus = 0;
        int vr_network_bonus = 0;
        int pleasure_dome_bonus = 0;
        int barracks_bonus = 0;
        int marine_barracks_bonus = 0;
        const char* msg_barracks = s__00551100;

        int mixed_race_penalty = Mixed_Race_Morale_Penalty_(colony);
        colony->morale = _morale_lookup_table[trait0];

        if (gov_type != 3) { // 3 = Unification
            int officer_idx = Colony_Officer_(colony);
            if (officer_idx != -1) {
                s_leader_data* leader = &MOX::_leaders[officer_idx];
                if (leader->special_skills & 0x8000) {
                    leader_bonus = (Owned_Officer_Level_(officer_idx) * 3 + 4) / 2;
                } else if (leader->special_skills & 0x4000) {
                    leader_bonus = Owned_Officer_Level_(officer_idx) + 1;
                }
            }

            if (colony->buildings[BUILDING_HOLO_SIMULATOR]) holo_simulator_bonus = 4;
            if (player->tech_applications[TECH_APP_VIRTUAL_REALITY_NETWORK] == 3) vr_network_bonus = 4;
            if (colony->buildings[BUILDING_PLEASURE_DOME]) pleasure_dome_bonus = 6;

            int8_t owner_trait0 = MOX::_player[owner].traits[TRAIT_CURRENT_GOVERNMENT];
            int8_t owner_gov = owner_trait0 / 2;
            if (owner_gov != 3) {
                short home_id = player->home_planet_id;
                bool has_capitol = false;
                if (home_id != -1) {
                    short cap_col_idx = MOX::_planet[home_id].colony_index;
                    if (MOX::_colony[cap_col_idx].buildings[BUILDING_CAPITOL]) has_capitol = true;
                }
                if (!has_capitol) no_capital_penalty = _non_capital_morale_lookup[owner_gov];
            }

            if (gov_type < 2 && player->tech_applications[TECH_APP_PSIONICS] == 3) psychophysics_bonus = 2;

            if (colony->buildings[BUILDING_MARINE_BARRACKS]) {
                marine_barracks_bonus = _barracks_morale_lookup[trait0];
            } else if (colony->buildings[BUILDING_ARMOR_BARRACKS]) {
                barracks_bonus = _barracks_morale_lookup[trait0];
            } else if (gov_type < 2) {
                msg_barracks = _no_barracks_msg;
            }

            colony->morale += mixed_race_penalty + leader_bonus + holo_simulator_bonus +
                             vr_network_bonus + pleasure_dome_bonus + psychophysics_bonus +
                             barracks_bonus + marine_barracks_bonus + no_capital_penalty;
        }

        if (debug) {
            if (gov_type == 3) {
                const char* src = _unification_has_no_morale_msg;
                while ((*debug++ = *src++) != '\0');
                return;
            }

            debug += _sprintf(debug, (const char*)0x00551130, (const char*)0x0055A8A4);
            debug += _sprintf(debug, (const char*)0x00551130, (const char*)0x0055110C);

            debug += _sprintf(debug, (const char*)0x0055A894,
                              _morale_lookup_table[trait0] * 5, _government_morale_msg, msg_barracks);

            if (no_capital_penalty != 0)
                debug += _sprintf(debug, (const char*)0x0055A888,
                                  no_capital_penalty * 5, _no_capital_msg);

            if (mixed_race_penalty != 0)
                debug += _sprintf(debug, (const char*)0x0055A87C,
                                  mixed_race_penalty * 5, _mixed_races_msg);

            if (barracks_bonus != 0)
                debug += _sprintf(debug, (const char*)0x0055A888,
                                  barracks_bonus * 5, TECHDATA::_buildings[BUILDING_ARMOR_BARRACKS].name);

            if (marine_barracks_bonus != 0)
                debug += _sprintf(debug, (const char*)0x0055A888,
                                  marine_barracks_bonus * 5, TECHDATA::_buildings[BUILDING_MARINE_BARRACKS].name);

            if (psychophysics_bonus != 0)
                debug += _sprintf(debug, (const char*)0x0055A888,
                                  psychophysics_bonus * 5, TECHDATA::_technology_applications[TECH_APP_PSIONICS].name);

            if (holo_simulator_bonus != 0)
                debug += _sprintf(debug, (const char*)0x0055A888,
                                  holo_simulator_bonus * 5, TECHDATA::_buildings[BUILDING_HOLO_SIMULATOR].name);

            if (vr_network_bonus != 0)
                debug += _sprintf(debug, (const char*)0x0055A888,
                                  vr_network_bonus * 5, TECHDATA::_technology_applications[TECH_APP_VIRTUAL_REALITY_NETWORK].name);

            if (pleasure_dome_bonus != 0)
                debug += _sprintf(debug, (const char*)0x0055A888,
                                  pleasure_dome_bonus * 5, TECHDATA::_buildings[BUILDING_PLEASURE_DOME].name);

            if (leader_bonus != 0)
                debug += _sprintf(debug, (const char*)0x0055A888,
                                  leader_bonus * 5, _leader_bonus_msg);

            debug += _sprintf(debug, (const char*)0x0055136C);
            _sprintf(debug, (const char*)0x0055A888,
                     colony->morale * 5, _total_morale_msg);
        }
    }

    void __cdecl Twiddle_Initial_Homeworld_(s_colony* colony) {
        uint32_t* p_pop = (uint32_t*)colony->pop;
        uint32_t* p_pop_end = &p_pop[colony->n_pops];

        MOX::Mox_Update_();

        // Convert all initial population to Scientists
        for (uint32_t* p = p_pop; p < p_pop_end; ++p) {
            *p = (*p & 0xffffff7f) | 0x00000100; // Scientist flag set, Worker flag cleared
        }

        int8_t star_idx = MOX::_planet[colony->planet].star_index;
        MOX::_star[star_idx].ignore_colony_ships |= (1 << (colony->owner & 0x1F));

        Recalculate_Colony_(colony);

        // Convert Scientists to Workers to meet industry maintenance requirements
        while (p_pop < p_pop_end && colony->production[ECON_INDUSTRY] < (int16_t)colony->maintenance[ECON_INDUSTRY]) {
            *p_pop = (*p_pop & 0xfffffeff) | 0x00000080; // Worker flag set, Scientist flag cleared
            Recalculate_Colony_(colony);
            p_pop++;
        }

        // Convert Pops to Farmers if colony has food multipliers and needs to meet maintenance
        if (colony->food2_per_farmer != 0) {
            while (p_pop < p_pop_end && (int16_t)colony->maintenance[ECON_FOOD] > colony->production[ECON_FOOD]) {
                *p_pop = (*p_pop & 0xfffffe7f); // Clear Scientist and Worker flags -> Farmer
                Recalculate_Colony_(colony);
                p_pop++;
            }
        }

        // Convert Scientists to Workers if Research production exceeds Industry
        while (p_pop < p_pop_end && colony->production[ECON_RESEARCH] > colony->production[ECON_INDUSTRY]) {
            *p_pop = (*p_pop & 0xfffffeff) | 0x00000080; // Worker flag set, Scientist flag cleared
            Recalculate_Colony_(colony);
            p_pop++;
        }

        // If Industry is balanced or higher than Research, switch the last adjusted Pop back to Scientist
        if (colony->production[ECON_INDUSTRY] >= colony->production[ECON_RESEARCH]) {
            *(p_pop - 1) = (*(p_pop - 1) & 0xffffff7f) | 0x00000100;
            Recalculate_Colony_(colony);
        }

        // Handle unique system specials (Orion/Guardians/Monsters/Artifacts)
        switch (MOX::_star[star_idx].system_special) {
            case 2:
            case 3:
                MOX::_star[star_idx].system_special = 0;
                break;
            case 6:
            case 7:
                MOX::_star[star_idx].system_special = 0;
                for (int i = 0; i < 5; ++i) {
                    int16_t p_idx = MOX::_star[star_idx].planet_index[i];
                    if (p_idx != -1) {
                        MOX::_planet[p_idx].planet_special = 0;
                    }
                }
                break;
            case 8:
                // Deassign officers if the system special is removed (e.g., Orion conquered)
                for (int16_t i = 0; i < 67; ++i) {
                    if (MOX::_leaders[i].status == 5 && MOX::_leaders[i].location == star_idx) {
                        OFFICER::Deassign_Officer_(i);
                    }
                }
                MOX::_star[star_idx].system_special = 0;
                break;
            case 10:
                MOX::_star[star_idx].artifacts_gave_app = 1;
                break;
        }
    }

    /**
     * @brief Checks if a colony is currently blockaded for its owner.
     * 
     * The blockade status for each star is stored as a bitmask where each bit 
     * represents a player. This function retrieves the star index associated 
     * with the colony's planet and checks the blockade bit corresponding 
     * to the colony's owner.
     */
    bool __cdecl Colony_Is_Blockaded(s_colony* colony) {
        int16_t planet_idx = colony->planet;

        int8_t star_idx = MOX::_planet[planet_idx].star_index;

        uint8_t blockaded_mask = MOX::_star[star_idx].blockaded;

        return (bool)((blockaded_mask >> (colony->owner & 0x1F)) & 1);
    }

    void __cdecl Pass_Out_Imports_(int16_t player_idx, s_colony* colony) {
        s_player* player = &MOX::_player[player_idx];
        int16_t needy_colony_indices[250];
        int16_t num_needy = 0;
        int16_t total_surplus = 0;
        int16_t total_deficit = 0;
        int16_t capacity = player->n_freighters - (player->settlers_freighted * 5);

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_COLONIES; ++i) {
            s_colony* c = &MOX::_colony[i];
            if (c->owner == player_idx && c->outpost_flag == 0) {
                if (EVENTS::Event_Check_Space_Anomaly_(i) == 0) {
                    int16_t balance = c->production[ECON_FOOD] - (uint8_t)c->maintenance[ECON_FOOD];
                    if (balance < 0) {
                        c->imports[ECON_FOOD] = 0;
                        if (!Colony_Is_Blockaded(c)) {
                            needy_colony_indices[num_needy++] = i;
                            total_deficit -= balance; // Make positive
                        }
                    } else {
                        c->imports[ECON_FOOD] = -balance; // Negative means exporting
                        if (!Colony_Is_Blockaded(c)) {
                            total_surplus += balance;
                        }
                    }
                }
            }
        }

        player->surplus_food = total_surplus - total_deficit;

        if (total_surplus < total_deficit || capacity < total_deficit) {
            int16_t remaining_capacity = capacity;
            int16_t remaining_food = total_surplus;
            int16_t consecutive_skips;
            int16_t needy_idx;

            // Pass 1: "Our Empire" food needs
            needy_idx = 0;
            consecutive_skips = 0;
            while (remaining_capacity > 0 && consecutive_skips < num_needy) {
                int16_t c_idx = needy_colony_indices[needy_idx];
                s_colony* c = &MOX::_colony[c_idx];
                if (remaining_food > 0 && (uint8_t)c->food2_needed_for_our_empire > (c->production[ECON_FOOD] + c->imports[ECON_FOOD]) * 2) {
                    remaining_food--;
                    remaining_capacity--;
                    c->imports[ECON_FOOD]++;
                    consecutive_skips = 0;
                } else {
                    consecutive_skips++;
                }
                if (++needy_idx == num_needy) needy_idx = 0;
            }

            // Pass 2: "Our Empire" + "Assimilated" food needs
            needy_idx = 0;
            consecutive_skips = 0;
            while (remaining_capacity > 0 && consecutive_skips < num_needy) {
                int16_t c_idx = needy_colony_indices[needy_idx];
                s_colony* c = &MOX::_colony[c_idx];
                int needs = (uint8_t)c->food2_needed_for_our_empire + (uint8_t)c->food2_needed_for_assimilated;
                if (remaining_food > 0 && needs > (c->production[ECON_FOOD] + c->imports[ECON_FOOD]) * 2) {
                    remaining_food--;
                    remaining_capacity--;
                    c->imports[ECON_FOOD]++;
                    consecutive_skips = 0;
                } else {
                    consecutive_skips++;
                }
                if (++needy_idx == num_needy) needy_idx = 0;
            }

            // Pass 3: "Our Empire" + "Assimilated" + "Conquered" food needs
            needy_idx = 0;
            consecutive_skips = 0;
            while (remaining_capacity > 0 && consecutive_skips < num_needy) {
                int16_t c_idx = needy_colony_indices[needy_idx];
                s_colony* c = &MOX::_colony[c_idx];
                int needs = (uint8_t)c->food2_needed_for_our_empire + (uint8_t)c->food2_needed_for_assimilated + (uint8_t)c->food2_needed_for_conquered;
                if (remaining_food > 0 && needs > (c->production[ECON_FOOD] + c->imports[ECON_FOOD]) * 2) {
                    remaining_food--;
                    remaining_capacity--;
                    c->imports[ECON_FOOD]++;
                    consecutive_skips = 0;
                } else {
                    consecutive_skips++;
                }
                if (++needy_idx == num_needy) needy_idx = 0;
            }

            // Pass 4: Basic maintenance needs
            needy_idx = 0;
            consecutive_skips = 0;
            while (remaining_capacity > 0 && consecutive_skips < num_needy) {
                int16_t c_idx = needy_colony_indices[needy_idx];
                s_colony* c = &MOX::_colony[c_idx];
                if (remaining_food > 0 && c->maintenance[ECON_FOOD] > (c->production[ECON_FOOD] + c->imports[ECON_FOOD])) {
                    remaining_food--;
                    remaining_capacity--;
                    c->imports[ECON_FOOD]++;
                    consecutive_skips = 0;
                } else {
                    consecutive_skips++;
                }
                if (++needy_idx == num_needy) needy_idx = 0;
            }

            int16_t freighted = total_surplus - remaining_food;
            player->food_freighted = freighted;
            if (remaining_capacity == 0) {
                int16_t min_freight = (total_deficit <= total_surplus) ? total_deficit : total_surplus;
                player->surplus_freighters = freighted - min_freight;
            } else {
                player->surplus_freighters = capacity - freighted;
            }
        } else {
            // Resources are sufficient to cover everything
            for (int16_t i = 0; i < num_needy; ++i) {
                int16_t c_idx = needy_colony_indices[i];
                s_colony* c = &MOX::_colony[c_idx];
                c->imports[ECON_FOOD] = (uint8_t)c->maintenance[ECON_FOOD] - c->production[ECON_FOOD];
            }
            player->food_freighted = total_deficit;
            player->surplus_freighters = capacity - total_deficit;
        }

        if (colony != nullptr) {
            bool colony_was_updated = false;
            for (int16_t i = 0; i < num_needy; ++i) {
                s_colony* c = &MOX::_colony[needy_colony_indices[i]];
                Update_Colony_Pop_Specialty_(c);
                if (c == colony) {
                    colony_was_updated = true;
                }
            }
            if (!colony_was_updated) {
                Update_Colony_Pop_Specialty_(colony);
            }
        }
    }

    void __cdecl Colony_Pop_Grows_(s_colony* colony) {
        int total_pops = static_cast<uint8_t>(colony->n_pops);
        if (total_pops == 0) {
            for (int i = 0; i < 10; ++i) {
                colony->pop_growth[i] = 0;
                colony->pop_roundoff[i] = 0;
            }
            return;
        }

        int16_t colony_idx = static_cast<int16_t>(colony - MOX::_colony);
        if (EVENTS::Event_Check_Space_Anomaly_(colony_idx)) {
            for (int i = 0; i < 10; ++i) {
                colony->pop_growth[i] = 0;
            }
            return;
        }

        bool pop_boom = static_cast<bool>(EVENTS::Event_Check_Population_Boom_(colony_idx));
        bool plague = static_cast<bool>(EVENTS::Event_Check_Plague_(colony_idx));

        int food_pool = colony->imports[ECON_FOOD] + colony->production[ECON_FOOD] * 2;
        int industry_pool = colony->production[ECON_INDUSTRY] * 2;

        int food_needs_empire = colony->food2_needed_for_our_empire;
        int food_needs_assimilated = colony->food2_needed_for_assimilated;
        int food_needs_conquered = colony->food2_needed_for_conquered;
        int food_needs_natives = colony->food2_needed_for_natives;
        int ind_needs_empire = colony->industry2_needed_for_our_empire;
        int ind_needs_androids = colony->industry2_needed_for_androids;
        int ind_needs_assimilated = colony->industry2_needed_for_assimilated;
        int ind_needs_conquered = colony->industry2_needed_for_conquered;

        // Prioritize food needs
        while (food_pool > 0 && food_needs_empire > 0) { food_pool--; food_needs_empire--; }
        while (food_pool > 0 && food_needs_assimilated > 0) { food_pool--; food_needs_assimilated--; }
        while (food_pool > 0 && food_needs_conquered > 0) { food_pool--; food_needs_conquered--; }
        while (food_pool > 0 && food_needs_natives > 0) { food_pool--; food_needs_natives--; }

        // Prioritize industry needs
        while (industry_pool > 0 && ind_needs_empire > 0) { industry_pool--; ind_needs_empire--; }
        while (industry_pool > 1 && ind_needs_androids > 1) { industry_pool -= 2; ind_needs_androids -= 2; }
        while (industry_pool > 0 && ind_needs_assimilated > 0) { industry_pool--; ind_needs_assimilated--; }
        while (industry_pool > 0 && ind_needs_conquered > 0) { industry_pool--; ind_needs_conquered--; }

        for (int i = 0; i < 10; ++i) colony->pop_growth[i] = 0;

        int owner_penalty_sum = ind_needs_empire + food_needs_empire;
        colony->pop_growth[static_cast<uint8_t>(colony->owner)] = (owner_penalty_sum < 1) ? 0 : static_cast<int16_t>(owner_penalty_sum * -25);
        colony->pop_growth[8] = (ind_needs_androids < 1) ? 0 : static_cast<int16_t>(ind_needs_androids * -500);
        colony->pop_growth[9] = (food_needs_natives < 1) ? 0 : static_cast<int16_t>(food_needs_natives * -25);

        int conquered_penalty_sum = food_needs_assimilated + ind_needs_conquered + ind_needs_assimilated + food_needs_conquered;
        int total_lack_denom = static_cast<uint8_t>(colony->industry2_needed_for_conquered) + static_cast<uint8_t>(colony->food2_needed_for_assimilated) + static_cast<uint8_t>(colony->food2_needed_for_conquered) + static_cast<uint8_t>(colony->industry2_needed_for_assimilated);

        if (conquered_penalty_sum > 0 && total_lack_denom > 0) {
            for (int i = 0; i < static_cast<int>(MOX::_NUM_PLAYERS); ++i) {
                if (i != static_cast<uint8_t>(colony->owner)) {
                    int race_needs = static_cast<uint8_t>(colony->food2_needed_for_empire[i]) + static_cast<uint8_t>(colony->industry2_needed_for_empire[i]);
                    colony->pop_growth[i] += static_cast<int16_t>((race_needs * conquered_penalty_sum * -25) / total_lack_denom);
                }
            }
        }

        uint8_t race_pop_count[10] = { 0 };
        uint8_t race_conquered_count[10] = { 0 };
        for (int i = 0; i < total_pops; ++i) {
            uint8_t r = colony->pop[i].race;
            if (r < 10) {
                race_pop_count[r]++;
                if (colony->pop[i].conquered || (reinterpret_cast<uint32_t*>(&colony->pop[i])[0] & 0x00000400)) {
                    race_conquered_count[r]++;
                }
            }
        }

        for (int16_t i = 0; i < static_cast<int>(MOX::_NUM_PLAYERS); ++i) {
            if (race_pop_count[i] == 0) continue;

            int limit = static_cast<int>(Colony_Race_Pop_Limit_(colony, i));
            if (total_pops < limit) {
                int leader_idx = Colony_Officer_(colony);
                int growth_modifier = MOX::_player[i].traits[TRAIT_POP_GROWTH_BONUS] + 100;
                s_player* owner_ptr = &MOX::_player[static_cast<uint8_t>(colony->owner)];

                if (owner_ptr->tech_applications[TECH_APP_UNIVERSAL_ANTIDOTE] == 3) {
                    growth_modifier += 50;
                } else if (owner_ptr->tech_applications[TECH_APP_MICROBIOTICS] == 3) {
                    growth_modifier += 25;
                }

                if (leader_idx != -1) {
                    uint32_t skills = MOX::_leaders[leader_idx].special_skills;
                    int level = OFFICER::Owned_Officer_Level_(leader_idx);
                    if (skills & 0x800) { // Medicine skill
                        growth_modifier += (level * 3 + 3) * 5;
                    } else if (skills & 0x400) { // Science skill
                        growth_modifier += (level * 5 + 5) * 2;
                    }
                }

                int extra_growth = 0;
                if (colony->buildings[BUILDING_CLONING_CENTER] != 0) {
                    extra_growth = (race_pop_count[i] * 100) / total_pops;
                }
                if (pop_boom) growth_modifier += 100;
                if (plague) growth_modifier -= 200;

                if (colony->producing[0] == -3) { // Housing
                    growth_modifier += ((colony->production[ECON_INDUSTRY] - colony->maintenance[ECON_INDUSTRY]) * 40) / total_pops;
                }

                if (i == static_cast<uint8_t>(colony->owner) || race_conquered_count[i] == 0) {
                    if (owner_ptr->objectives != 100) {
                        growth_modifier += static_cast<int>(MOX::_settings.game_difficulty);
                    }
                } else if (growth_modifier > 0 && colony->occupation_policy == 0) {
                    growth_modifier = ((race_pop_count[i] - race_conquered_count[i]) * growth_modifier) / race_pop_count[i];
                }

                uint32_t base_growth_val_sq = (static_cast<uint32_t>(race_pop_count[i]) * (limit - total_pops) * 2000) / limit;
                colony->pop_growth[i] += static_cast<int16_t>((isqrt_(base_growth_val_sq) * growth_modifier) / 100 + extra_growth);
            }
        }
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

    void __cdecl Update_Colony_Pop_Specialty_(s_colony* colony) {
        COLCALC::Colony_Pop_Grows_(colony);
        COLCALC::Colony_Specialty_(colony);
    }

    int16_t __cdecl Size_And_Climate_Race_Pop_Limit_(int8_t size, int climate, int16_t player_idx) {
        bool is_immune = false;

        if (player_idx < 8) {
            // TRAIT_ENVIRONMENT_IMMUNE is at index 16
            if (MOX::_player[player_idx].traits[16] != 0) {
                is_immune = true;
            }
        } else if (player_idx == 8) {
            // Player index 8 is considered immune (e.g., Neutral/Orion)
            is_immune = true;
        }

        int adjusted_climate = COLCALC::Calc_Player_Climate_(static_cast<int>(player_idx), climate);
        int max_pop = COLCALC::Size_And_Climate_Max_Population_(size, adjusted_climate, is_immune);

        // TRAIT_SUBTERRANEAN is at index 6
        if (player_idx < 8 && MOX::_player[player_idx].traits[6] != 0) {
            max_pop += static_cast<int>(COLCALC::Double_Add_Two_(static_cast<short>(size)));
        }

        return static_cast<short>(max_pop);
    }

    void __cdecl Colony_Food_Production_(s_colony* colony, char* debug_out) {
        if (colony->n_pops == 0) {
            colony->production[ECON_FOOD] = 0;
            return;
        }

        int16_t colony_idx = static_cast<int16_t>(colony - MOX::_colony);
        if (EVENTS::Event_Check_Space_Anomaly_(colony_idx) != 0) {
            colony->production[ECON_FOOD] = 0;
            if (debug_out) {
                char* src = _colony_debug_msg;
                while ((*debug_out++ = *src++));
            }
            return;
        }

        s_food_production_breakdown breakdown;
        int job_food = 0;
        if (colony->food2_per_farmer == 0) {
            memset(&breakdown, 0, sizeof(breakdown));
        } else {
            job_food = Colony_Job_Production_(colony, 0, debug_out ? &breakdown : nullptr);
        }

        int16_t hydro_bonus = (colony->buildings[BUILDING_HYDROPONIC_FARM] != 0) ? 2 : 0;
        int16_t subterranean_bonus = (colony->buildings[BUILDING_SUBTERRANEAN_FARMS] != 0) ? 4 : 0;
        colony->production[ECON_FOOD] = hydro_bonus + subterranean_bonus + (int16_t)job_food;

        if (debug_out) {
            char* ptr = debug_out;
            ptr += _sprintf(ptr, s_fmt_s_00551130, s_T_15_65);
            ptr += _sprintf(ptr, s_fmt_s_00551130, s_0);

            if (breakdown.workers != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, breakdown.workers / 2, _produced_by_workers_msg);
            }
            if (breakdown.field_222 != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, breakdown.field_226 / 2, DAT_00571bc0);
            }

            for (int i = 0; i < 8; ++i) {
                int16_t bonus = breakdown.race_bonus[i];
                if (bonus > 0) {
                    ptr += _sprintf(ptr, s_fmt_plus_d_s_s, (int)(bonus / 2), 
                                    MOX::_player[i].race_name, 
                                    _bonus_msg);
                }
                if (bonus < 0) {
                    ptr += _sprintf(ptr, s_fmt_s_s_s, Format_Quotient_(bonus, 2), 
                                    MOX::_player[i].race_name, 
                                    _bonus_msg);
                }
            }

            if (breakdown.other_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s_2, (int)(breakdown.other_bonus / 2), DAT_00571afc);
            }
            if (breakdown.android_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, (int)(breakdown.android_bonus / 2), _android_bonus_msg);
            }

            for (int i = 0; i < 204; ++i) {
                if (breakdown.tech_bonus[i] != 0) {
                    ptr += _sprintf(ptr, s_fmt_plus_d_s, (int)(breakdown.tech_bonus[i] / 2), TECHDATA::_technology_applications[i].name);
                }
            }

            for (int i = 0; i < 49; ++i) {
                if (breakdown.building_bonus[i] != 0) {
                    ptr += _sprintf(ptr, s_fmt_plus_d_s, (int)(breakdown.building_bonus[i] / 2), TECHDATA::_buildings[i].name);
                }
            }

            if (breakdown.gravity_penalty != 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(breakdown.gravity_penalty, 0x28), _gravity_penalty_msg);
            }
            if (breakdown.conquered_penalty != 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(breakdown.conquered_penalty, 0x28), _conquered_penalty_msg);
            }
            if (breakdown.blockade_penalty != 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(breakdown.blockade_penalty, 0x28), _blockaded_penalty_msg);
            }
            if (breakdown.morale_modifier > 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(breakdown.morale_modifier, 0x28), _morale_bonus_msg);
            } else if (breakdown.morale_modifier < 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(breakdown.morale_modifier, 0x28), _morale_penalty_msg);
            }
            if (breakdown.government_modifier > 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(breakdown.government_modifier, 0x28), _government_bonus_msg);
            } else if (breakdown.government_modifier < 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(breakdown.government_modifier, 0x28), _government_penalty_msg);
            }
            if (breakdown.leader_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(breakdown.leader_bonus, 0x28), _leader_bonus_msg);
            }

            if (hydro_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, (int)hydro_bonus, TECHDATA::_buildings[BUILDING_HYDROPONIC_FARM].name);
            }
            if (subterranean_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, (int)subterranean_bonus, TECHDATA::_buildings[BUILDING_SUBTERRANEAN_FARMS].name);
            }

            if (colony->n_food_replicated != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, (int)colony->n_food_replicated, TECHDATA::_buildings[BUILDING_FOOD_REPLICATORS].name);
                colony->production[ECON_FOOD] += (int16_t)colony->n_food_replicated;
            }

            ptr += _sprintf(ptr, s_fmt_s_00551130, s_newline_0055136c);
            _sprintf(ptr, s_fmt_plus_d_s, (int)colony->production[ECON_FOOD], DAT_00571b80);
        }
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
                Update_Colony_Pop_Specialty_(colony);
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

    short __cdecl Get_First_Player_Colony_Idx(int player_idx) {
        short index = 0;
        while (MOX::_colony[index].owner != (int8_t)player_idx) {
            index++;
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

        int8_t owner_race_idx = colony->owner;
        int n_pops = (uint8_t)colony->n_pops;

        if (n_pops != 0) {
            for (int i = 0; i < n_pops; ++i) {
                // Accessing Pop_Info as a uint32 for manual bit extraction as per assembly
                uint32_t pop_data = *reinterpret_cast<uint32_t*>(&colony->pop[i]);
                uint32_t race = pop_data & 0xF;

                if (race < 8) {
                    // If trait is set, the population adds 1 to its industry maintenance requirement
                    int8_t cost = (MOX::_player[race].traits[TRAIT_PRODUCTION_BONUS] != 0) ? 1 : 0;

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

    void __cdecl Colony_Replicators_(s_colony* colony) {
        colony->n_food_replicated = 0;

        if (colony->buildings[BUILDING_FOOD_REPLICATORS] == 0) {
            return;
        }

        int16_t industry_spent = 0;
        int16_t industry_pool = colony->production[ECON_INDUSTRY] * 2;
        int16_t food_pool = colony->production[ECON_FOOD] * 2;

        int16_t ind_our = colony->industry2_needed_for_our_empire;
        int16_t ind_androids = colony->industry2_needed_for_androids;
        int16_t ind_assimilated = colony->industry2_needed_for_assimilated;
        int16_t ind_conquered = colony->industry2_needed_for_conquered;

        // Tier 1: Our Empire requirements
        int16_t surplus_ind = (industry_pool - ind_our) / 2;
        if (surplus_ind > 0) {
            int16_t food_goal = colony->food2_needed_for_our_empire;
            int16_t deficit = (food_goal - food_pool + 1) / 2;
            if (deficit > 0) {
                int16_t cost = (surplus_ind < (deficit * 2)) ? surplus_ind : (deficit * 2);
                cost = (cost / 2) * 2;
                industry_spent = cost;
                food_pool += cost;
                industry_pool -= (cost / 2);
            }

            // Tier 2: Androids and Assimilated populations
            surplus_ind = (industry_pool - ind_our - ind_androids - ind_assimilated) / 2;
            if (surplus_ind > 0) {
                int16_t food_goal_t2 = (int16_t)static_cast<uint8_t>(colony->food2_needed_for_our_empire) + (int16_t)static_cast<uint8_t>(colony->food2_needed_for_assimilated);
                int16_t deficit_t2 = (food_goal_t2 - food_pool + 1) / 2;
                if (deficit_t2 > 0) {
                    int16_t cost = (surplus_ind < (deficit_t2 * 2)) ? surplus_ind : (deficit_t2 * 2);
                    cost = (cost / 2) * 2;
                    industry_spent += cost;
                    food_pool += cost;
                    industry_pool -= (cost / 2);
                }

                // Tier 3: Conquered populations
                surplus_ind = (industry_pool - ind_conquered - ind_our - ind_androids - ind_assimilated) / 2;
                if (surplus_ind > 0) {
                    int16_t food_goal_t3 = (int16_t)colony->food2_needed_for_our_empire + 
                                          (int16_t)colony->food2_needed_for_assimilated + 
                                          (int16_t)colony->food2_needed_for_conquered;
                    int16_t deficit_t3 = (food_goal_t3 - food_pool + 1) / 2;
                    if (deficit_t3 > 0) {
                        int16_t cost = (surplus_ind < (deficit_t3 * 2)) ? surplus_ind : (deficit_t3 * 2);
                        cost = (cost / 2) * 2;
                        industry_spent += cost;
                        food_pool += cost;
                        industry_pool -= (cost / 2);
                    }

                    // Tier 4: Native populations
                    surplus_ind = (industry_pool - ind_conquered - ind_our - ind_androids - ind_assimilated) / 2;
                    if (surplus_ind > 0) {
                        int16_t food_goal_t4 = (int16_t)colony->food2_needed_for_our_empire + 
                                              (int16_t)colony->food2_needed_for_assimilated + 
                                              (int16_t)colony->food2_needed_for_conquered + 
                                              (int16_t)colony->food2_needed_for_natives;
                        int16_t deficit_t4 = (food_goal_t4 - food_pool + 1) / 2;
                        if (deficit_t4 > 0) {
                            int16_t cost = (surplus_ind < (deficit_t4 * 2)) ? surplus_ind : (deficit_t4 * 2);
                            industry_spent += (cost / 2) * 2;
                        }
                    }
                }
            }
        }

        // Apply final changes. Replicator ratio is 2 Industry : 1 Food
        int16_t final_food_gain = industry_spent / 2;
        colony->n_food_replicated = (int8_t)final_food_gain;
        colony->production[ECON_FOOD] += final_food_gain;
        colony->production[ECON_INDUSTRY] -= industry_spent;
    }

    void __cdecl Colony_Industry_To_Tax_(s_colony* colony) {
        int16_t industry_prod = colony->production[ECON_INDUSTRY];
        int32_t taxable_industry = (int32_t)industry_prod - (int32_t)colony->maintenance[ECON_INDUSTRY];

        if (taxable_industry < 1) {
            colony->n_industry_taxed = 0;
            return;
        }

        uint8_t tax_rate = (uint8_t)MOX::_player[(uint8_t)colony->owner].tax_rate;
        int16_t taxed_amount = (int16_t)((tax_rate * taxable_industry) / 100);

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

    void __cdecl Colony_Industry_Production_(struct s_colony* colony, char* debug_out) {
        int worker_prod;
        uint32_t android_recycler_bonus = 0;
        uint32_t automated_factory_bonus = 0;
        uint32_t robo_miner_bonus = 0;
        uint32_t deep_core_mine_bonus = 0;
        uint32_t robotic_factory_bonus = 0;
        uint32_t recycler_bonus = 0;
        int pollution_val = 0;
        
        struct DebugBuffers {
            int worker_prod_debug;
            int16_t racial_bonuses[8];
            int16_t android_bonus[2];
            int16_t tech_app_bonuses[204];
            int16_t building_bonuses[49];
            int gravity_pct;
            int conquered_pct;
            int blockaded_pct;
            int morale_pct;
            int government_pct;
            int leader_pct;
            int pollution_pct;
        } dbg;

        colony->n_industry_recyclers = 0;
        if (colony->n_pops == 0) {
            colony->production[ECON_INDUSTRY] = 0;
            return;
        }

        int16_t colony_idx = (int16_t)(((uintptr_t)colony - (uintptr_t)MOX::_colony) / 361 /* sizeof(s_colony) */);
        if (EVENTS::Event_Check_Space_Anomaly_(colony_idx)) {
            colony->production[1] = 0;
            if (debug_out) {
                char* src = _colony_debug_msg;
                char* dst = debug_out;
                while ((*dst++ = *src++));
            }
            return;
        }

        worker_prod = Colony_Job_Production_(colony, 1, debug_out ? (char*)&dbg.worker_prod_debug : nullptr);
        
        if (colony->buildings[BUILDING_AUTOMATED_FACTORY]) automated_factory_bonus = 5;
        if (colony->buildings[BUILDING_ROBO_MINER_PLANT]) robo_miner_bonus = 10;
        if (colony->buildings[BUILDING_DEEP_CORE_MINE]) deep_core_mine_bonus = 15;
        
        if (colony->buildings[BUILDING_ROBOTIC_FACTORY]) {
            robotic_factory_bonus = (uint32_t)_planet_robotic_factory_lookup[(uint8_t)MOX::_planet[colony->planet].planet_type];
        }

        if (colony->buildings[BUILDING_RECYCLOTRON]) {
            colony->n_industry_recyclers = colony->n_pops;
            recycler_bonus = (uint32_t)(uint8_t)colony->n_pops;
        }

        if (MOX::_player[colony->owner].objectives != 100) {
            int base_recycler = (int)MOX::_settings.game_difficulty * (int)(uint8_t)colony->n_pops + 4;
            colony->n_industry_recyclers += (int8_t)(base_recycler >> 3);
        }

        int total_prod = (int)worker_prod + (int)recycler_bonus + (int)automated_factory_bonus + 
                         (int)robo_miner_bonus + (int)deep_core_mine_bonus + (int)robotic_factory_bonus;

        if (colony->buildings[BUILDING_CORE_WASTE_DUMP]) {
            colony->pollution = 0;
        } else {
            int leader_idx = Colony_Officer_(colony);
            int poll_base = worker_prod + robotic_factory_bonus;
            int threshold = (uint8_t)_industry_planet_size_lookup[(uint8_t)MOX::_planet[colony->planet].size];
            
            if (MOX::_player[colony->owner].tech_applications[TECH_APP_NANO_DISASSEMBLERS] == 3) {
                threshold *= 2;
            }

            if (colony->buildings[BUILDING_POLLUTION_PROCESSOR]) {
                poll_base = (poll_base + 1) / 2;
            }
            if (colony->buildings[BUILDING_ATMOSPHERE_RENEWER]) {
                poll_base = (poll_base + 3) / 4;
            }

            if (leader_idx != -1) {
                int modifier = 0;
                if (MOX::_leaders[leader_idx].special_skills & 2) { // Environmentalist (Mega)
                    modifier = (Owned_Officer_Level_(leader_idx) * 3 + 3) * 5;
                } else if (MOX::_leaders[leader_idx].special_skills & 1) { // Environmentalist
                    modifier = (Owned_Officer_Level_(leader_idx) * 5 + 5) * 2;
                }
                poll_base -= (modifier * poll_base) / 100;
            }

            int calc_pollution = (poll_base - threshold + 1) / 2;
            if (calc_pollution < 1) {
                pollution_val = 0;
            } else {
                int immune_pops = 0;
                for (int i = 0; i < (int)(uint8_t)colony->n_pops; ++i) {
                    uint32_t race_idx = colony->pop[i].race;
                    if (race_idx == 8 /* Android */ || MOX::_player[race_idx].traits[TRAIT_ENVIRONMENT_IMMUNE]) {
                        immune_pops++;
                    }
                }
                if (immune_pops != 0) {
                    calc_pollution = ((int)(uint8_t)colony->n_pops / 2 + ((int)(uint8_t)colony->n_pops - immune_pops) * calc_pollution) / (int)(uint8_t)colony->n_pops;
                }
                pollution_val = (calc_pollution < 1) ? 0 : calc_pollution;
                total_prod -= pollution_val;
                dbg.pollution_pct = pollution_val * -20;
            }
            colony->pollution = (int16_t)pollution_val;
        }

        colony->production[1] = (int16_t)total_prod;

        if (debug_out) {
            int off = _sprintf(debug_out, s_fmt_s_00551130, s_T_15_65);
            off += _sprintf(debug_out + off, s_fmt_s_00551130, s_0);
            
            char* ptr = debug_out + off;
            if (dbg.worker_prod_debug != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, dbg.worker_prod_debug, _produced_by_workers_msg);
            }
            
            for (int i = 0; i < 8; ++i) {
                if (dbg.racial_bonuses[i] != 0) {
                    ptr += _sprintf(ptr, s_fmt_plus_d_s_s, (int)dbg.racial_bonuses[i], MOX::_player[0].race_name + (i * 0xea9), _bonus_msg);
                }
            }
            
            if (dbg.android_bonus[0] != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, (int)dbg.android_bonus[0], _android_bonus_msg);
            }

            for (int i = 0; i < 204; ++i) {
                if (dbg.tech_app_bonuses[i] != 0) {
                    ptr += _sprintf(ptr, s_fmt_plus_d_s, (int)dbg.tech_app_bonuses[i], TECHDATA::_technology_applications[i].name);
                }
            }

            for (int i = 0; i < 49; ++i) {
                if (dbg.building_bonuses[i] != 0) {
                    ptr += _sprintf(ptr, s_fmt_plus_d_s, (int)dbg.building_bonuses[i], TECHDATA::_buildings[i].name);
                }
            }

            if (dbg.gravity_pct != 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(dbg.gravity_pct, 20), _gravity_penalty_msg);
            }
            if (dbg.conquered_pct != 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(dbg.conquered_pct, 20), _conquered_penalty_msg);
            }
            if (dbg.blockaded_pct != 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(dbg.blockaded_pct, 20), _blockaded_penalty_msg);
            }
            if (dbg.morale_pct > 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(dbg.morale_pct, 20), _morale_bonus_msg);
            } else if (dbg.morale_pct < 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(dbg.morale_pct, 20), _morale_penalty_msg);
            }
            if (dbg.government_pct > 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(dbg.government_pct, 20), _government_bonus_msg);
            } else if (dbg.government_pct < 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(dbg.government_pct, 20), _government_penalty_msg);
            }
            if (dbg.leader_pct != 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(dbg.leader_pct, 20), _leader_bonus_msg);
            }
            if (recycler_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, recycler_bonus, TECHDATA::_buildings[33].name);
            }
            if (automated_factory_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, automated_factory_bonus, TECHDATA::_buildings[7].name);
            }
            if (robo_miner_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, robo_miner_bonus, TECHDATA::_buildings[36].name);
            }
            if (deep_core_mine_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, deep_core_mine_bonus, TECHDATA::_buildings[12].name);
            }
            if (robotic_factory_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s_s, robotic_factory_bonus, TECHDATA::_buildings[34].name, _used_msg);
            }
            if (dbg.pollution_pct != 0) {
                ptr += _sprintf(ptr, s_fmt_s_s, Format_Quotient_(dbg.pollution_pct, 20), _pollution_penalty_msg);
            }

            if (colony->n_food_replicated != 0) {
                char temp_buf[64];
                _sprintf(temp_buf, _replicated_to_d_food_msg, (uint32_t)(uint8_t)colony->n_food_replicated);
                ptr += _sprintf(ptr, s_fmt_d_s, (uint32_t)(uint8_t)colony->n_food_replicated * -2, temp_buf);
                colony->production[1] -= (uint16_t)(uint8_t)colony->n_food_replicated * 2;
            }

            if (colony->n_industry_taxed != 0) {
                ptr += _sprintf(ptr, s_fmt_minus_d_s, (int)colony->n_industry_taxed, _taxed_from_industry_msg);
                colony->production[1] -= (int16_t)colony->n_industry_taxed;
            }

            ptr += _sprintf(ptr, s_fmt_s_00551130, s_newline_0055136c);
            _sprintf(ptr, s_fmt_plus_d_s, (int)colony->production[1], _total_industry_produced_msg);
        }
    }

    void __cdecl Colony_BC_Production_(s_colony* colony, char* debug_buf) {
        if (colony->n_pops == 0) {
            colony->production[ECON_BC] = 0;
            return;
        }

        int16_t colony_idx = (int16_t)((uintptr_t)colony - (uintptr_t)MOX::_colony) / sizeof(s_colony);
        if (EVENTS::Event_Check_Space_Anomaly_(colony_idx) != 0) {
            colony->production[ECON_BC] = 0;
            if (debug_buf) {
                char* src = _colony_debug_msg;
                while ((*debug_buf++ = *src++) != '\0');
            }
            return;
        }

        s_player* owner_ptr = &MOX::_player[colony->owner];
        int officer_idx = Colony_Officer_(colony);

        int morale_bonus = 0;
        int trade_goods_bc = 0;
        int galactic_currency_bc = 0;
        int stock_exchange_bc = 0;
        int spaceport_bc = 0;
        int gov_bonus_bc = 0;
        int leader_bonus_bc = 0;

        int total_normal_pops = 0;
        int total_immune_pops = 0;

        int base_tax_per_pop = owner_ptr->traits[TRAIT_TAX_BONUS] * 2 + 4;

        for (int i = 0; i < (uint8_t)colony->n_pops; ++i) {
            s_pop_info* p = &colony->pop[i];
            uint32_t race_idx = (uint32_t)p->race;
            if (race_idx < 8) {
                if (MOX::_player[race_idx].traits[TRAIT_ENVIRONMENT_IMMUNE] == 0) {
                    total_normal_pops++;
                } else {
                    total_immune_pops++;
                }
            }
        }

        int owner_idx = (int)colony->owner;
        if (MOX::_player[owner_idx].objectives != 'd') {
            base_tax_per_pop += (int)_difficulty_bc_lookup_table[MOX::_settings.game_difficulty];
        }

        int total_pops = total_normal_pops + total_immune_pops;
        int taxes_collected = (base_tax_per_pop * total_pops + 2) / 4;

        if (total_pops > 0 && MOX::_player[owner_idx].traits[TRAIT_CURRENT_GOVERNMENT] != 3) {
            morale_bonus = colony->morale * taxes_collected;
            morale_bonus = (morale_bonus + (morale_bonus < 0 ? -10 : 10)) / 20;
        }

        int special_planet_bc = 0;
        int8_t planet_special = MOX::_planet[colony->planet].planet_special;
        if (planet_special == SYSTEM_SPECIAL_GOLD_DEPOSITS) {
            special_planet_bc = 5;
        } else if (planet_special == SYSTEM_SPECIAL_GEM_DEPOSITS) {
            special_planet_bc = 10;
        }

        int16_t producing = colony->producing[0];
        if (producing == -37 || (producing < 0 && producing > -3)) {
            int surplus_ind = (int)colony->production[ECON_INDUSTRY] - (int)colony->maintenance[ECON_INDUSTRY];
            if (surplus_ind > 0) {
                trade_goods_bc = surplus_ind;
                if (owner_ptr->traits[TRAIT_FANTASTIC_TRADERS] == 0) {
                    trade_goods_bc = (surplus_ind + 1) / 2;
                }
            }
        }

        int income_base = taxes_collected + special_planet_bc;

        if (MOX::_player[owner_idx].tech_applications[TECH_APP_GALACTIC_CURRENCY_EXCHANGE] == 3) {
            galactic_currency_bc = income_base / 2;
        }

        if (colony->buildings[BUILDING_STOCK_EXCHANGE] != 0) {
            stock_exchange_bc = income_base;
        }

        if (colony->buildings[BUILDING_SPACEPORT] != 0) {
            spaceport_bc = income_base / 2;
        }

        if (MOX::_player[owner_idx].traits[TRAIT_CURRENT_GOVERNMENT] == 4) {
            gov_bonus_bc = income_base / 2;
        } else if (MOX::_player[owner_idx].traits[TRAIT_CURRENT_GOVERNMENT] == 5) {
            gov_bonus_bc = (income_base * 3 + (income_base * 3 < 0 ? 3 : 0)) / 4;
        }

        if (officer_idx != -1) {
            if (MOX::_leaders[officer_idx].special_skills & 0x20) {
                int16_t lvl = Owned_Officer_Level_(officer_idx);
                leader_bonus_bc = ((lvl + 1) * income_base * 3 + 10) / 20;
            } else if (MOX::_leaders[officer_idx].special_skills & 0x10) {
                int16_t lvl = Owned_Officer_Level_(officer_idx);
                leader_bonus_bc = ((lvl + 1) * income_base + 5) / 10;
            }
        }

        colony->production[ECON_BC] = (int16_t)(colony->n_industry_taxed + stock_exchange_bc +
                                         galactic_currency_bc + trade_goods_bc + 
                                         special_planet_bc + morale_bonus + taxes_collected + 
                                         leader_bonus_bc + gov_bonus_bc + spaceport_bc);

        if (debug_buf) {
            char* ptr = debug_buf;
            ptr += _sprintf(ptr, s_fmt_s_00551130, s_T_15_65);
            ptr += _sprintf(ptr, s_fmt_s_00551130, s_0);
            
            if (taxes_collected != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, taxes_collected, _taxes_collected_msg);
            }
            if (morale_bonus > 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, morale_bonus, _morale_bonus_msg);
            } else if (morale_bonus < 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, morale_bonus, _morale_penalty_msg);
            }
            if (special_planet_bc != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, special_planet_bc, _special_income_msg);
            }
            if (leader_bonus_bc != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, leader_bonus_bc, _leader_bonus_msg);
            }
            if (galactic_currency_bc != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, galactic_currency_bc, TECHDATA::_buildings[BUILDING_CURRENCY_EXCHANGE].name);
            }
            if (stock_exchange_bc != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, stock_exchange_bc, TECHDATA::_buildings[BUILDING_STOCK_EXCHANGE].name);
            }
            if (spaceport_bc != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, spaceport_bc, TECHDATA::_buildings[BUILDING_SPACEPORT].name);
            }
            if (gov_bonus_bc != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, gov_bonus_bc, _government_bonus_msg);
            }
            if (trade_goods_bc != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, trade_goods_bc, _trade_goods_msg);
            }
            if (colony->n_industry_taxed != 0) {
                ptr += _sprintf(ptr, s_fmt_plus_d_s, (int)colony->n_industry_taxed, _taxed_from_industry_msg);
            }
            ptr += _sprintf(ptr, "%s", s_newline_0055136c);
            _sprintf(ptr, s_fmt_plus_d_s, (int)colony->production[ECON_BC], _total_income_msg);
        }
    }

    void __cdecl Colony_Research_Production_(s_colony* colony, char* debug_out) {
        if (colony->n_pops == 0) {
            colony->production[ECON_RESEARCH] = 0;
            return;
        }

        int16_t colony_idx = static_cast<int16_t>(colony - MOX::_colony);
        if (EVENTS::Event_Check_Space_Anomaly_(colony_idx)) {
            colony->production[ECON_RESEARCH] = 0;
            if (debug_out) {
                char* msg = _anomaly_msg;
                while ((*debug_out++ = *msg++) != '\0');
            }
            return;
        }

        struct {
            int scientist_prod;         // Offset 0
            int16_t race_bonuses[8];    // Offset 4
            int16_t android_bonus;      // Offset 20
            int16_t tech_apps[212];     // Offset 24
            int16_t building_prods[49]; // Offset 448
            int32_t padding_0x222;      // Offset 546
            int32_t artifacts_bonus;    // Offset 550
            int32_t intelligence_bonus; // Offset 554
            int32_t padding_0x22E;      // Offset 558
            int32_t gravity_penalty;    // Offset 562
            int32_t conquered_penalty;  // Offset 566
            int32_t morale_mod;         // Offset 574
            int32_t gov_mod;            // Offset 578
            int32_t blockaded_penalty;  // Offset 582
            int32_t leader_bonus;       // Offset 586
        } debug_data;
        memset(&debug_data, 0, sizeof(debug_data));

        void* debug_ptr = debug_out ? &debug_data : nullptr;
        int base_research = Colony_Job_Production_(colony, ECON_RESEARCH, debug_ptr);

        int res_lab_bonus = (colony->buildings[BUILDING_RESEARCH_LAB] != 0) ? 5 : 0;
        int supercomp_bonus = (colony->buildings[BUILDING_SUPERCOMPUTER] != 0) ? 10 : 0;
        int cybernet_bonus = (colony->buildings[BUILDING_GALACTIC_CYBERNET] != 0) ? 15 : 0;
        int autolab_bonus = (colony->buildings[BUILDING_AUTOLAB] != 0) ? 30 : 0;

        int total_research = base_research + res_lab_bonus + supercomp_bonus + cybernet_bonus + autolab_bonus;
        colony->production[ECON_RESEARCH] = static_cast<int16_t>(total_research);

        if (debug_out) {
            char* p = debug_out;
            p += _sprintf(p, s_fmt_s_00551130, s_T_15_65);
            p += _sprintf(p, s_fmt_s_00551130, s_0);

            if (debug_data.scientist_prod != 0) {
                p += _sprintf(p, s_fmt_plus_d_s, debug_data.scientist_prod, _produced_by_scientists_msg);
            }
            if (debug_data.artifacts_bonus != 0) {
                p += _sprintf(p, s_fmt_plus_d_s, debug_data.artifacts_bonus, _artifacts_bonus_msg);
            }
            if (debug_data.intelligence_bonus != 0) {
                p += _sprintf(p, s_fmt_plus_d_s, debug_data.intelligence_bonus, _intelligence_bonus_msg);
            }

            for (int i = 0; i < 8; ++i) {
                if (debug_data.race_bonuses[i] != 0) {
                    p += _sprintf(p, s_fmt_plus_d_s_s, static_cast<int>(debug_data.race_bonuses[i]), MOX::_player[i].race_name, _bonus_msg);
                }
            }

            if (debug_data.android_bonus != 0) {
                p += _sprintf(p, s_fmt_plus_d_s, static_cast<int>(debug_data.android_bonus), _android_bonus_msg);
            }

            for (int i = 0; i < 212; ++i) {
                if (debug_data.tech_apps[i] != 0) {
                    p += _sprintf(p, s_fmt_plus_d_s, static_cast<int>(debug_data.tech_apps[i]), TECHDATA::_technology_applications[i].name);
                }
            }

            for (int i = 0; i < 49; ++i) {
                if (debug_data.building_prods[i] != 0) {
                    p += _sprintf(p, s_fmt_plus_d_s, static_cast<int>(debug_data.building_prods[i]), TECHDATA::_buildings[i].name);
                }
            }

            if (debug_data.gravity_penalty != 0) {
                p += _sprintf(p, s_fmt_s_s, Format_Quotient_(debug_data.gravity_penalty, 20), _gravity_penalty_msg);
            }
            if (debug_data.conquered_penalty != 0) {
                p += _sprintf(p, s_fmt_s_s, Format_Quotient_(debug_data.conquered_penalty, 20), _conquered_penalty_msg);
            }
            if (debug_data.blockaded_penalty != 0) {
                p += _sprintf(p, s_fmt_s_s, Format_Quotient_(debug_data.blockaded_penalty, 20), _blockaded_penalty_msg);
            }
            if (debug_data.morale_mod > 0) {
                p += _sprintf(p, s_fmt_s_s, Format_Quotient_(debug_data.morale_mod, 20), _morale_bonus_msg);
            } else if (debug_data.morale_mod < 0) {
                p += _sprintf(p, s_fmt_s_s, Format_Quotient_(debug_data.morale_mod, 20), _morale_penalty_msg);
            }
            if (debug_data.gov_mod > 0) {
                p += _sprintf(p, s_fmt_s_s, Format_Quotient_(debug_data.gov_mod, 20), _government_bonus_msg);
            } else if (debug_data.gov_mod < 0) {
                p += _sprintf(p, s_fmt_s_s, Format_Quotient_(debug_data.gov_mod, 20), _government_penalty_msg);
            }
            if (debug_data.leader_bonus != 0) {
                p += _sprintf(p, s_fmt_s_s, Format_Quotient_(debug_data.leader_bonus, 20), _leader_bonus_msg);
            }

            if (res_lab_bonus != 0) {
                p += _sprintf(p, s_fmt_plus_d_s_3, res_lab_bonus, TECHDATA::_buildings[BUILDING_RESEARCH_LAB].name);
            }
            if (supercomp_bonus != 0) {
                p += _sprintf(p, s_fmt_plus_d_s_3, supercomp_bonus, TECHDATA::_buildings[BUILDING_SUPERCOMPUTER].name);
            }
            if (cybernet_bonus != 0) {
                p += _sprintf(p, s_fmt_plus_d_s_3, cybernet_bonus, TECHDATA::_buildings[BUILDING_GALACTIC_CYBERNET].name);
            }
            if (autolab_bonus != 0) {
                p += _sprintf(p, s_fmt_plus_d_s_3, autolab_bonus, TECHDATA::_buildings[BUILDING_AUTOLAB].name);
            }

            p += _sprintf(p, s_fmt_s_00551130, s_newline_0055136c);
            _sprintf(p, s_fmt_plus_d_s, static_cast<int>(colony->production[ECON_RESEARCH]), _total_research_produced_msg);
        }
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

    /**
     * @address 004f7560
     */
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
        s_player* receiver = &MOX::_player[receiver_idx];

        int32_t tribute_percent = static_cast<int32_t>(receiver->tribute_treaty[payer_idx]);

        int32_t net_production = static_cast<int32_t>(receiver->bc_produced) - receiver->tribute_income;

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
            int16_t colony_idx = (int16_t)(((uintptr_t)colony - (uintptr_t)MOX::_colony) / sizeof(s_colony));
            if (EVENTS::Event_Check_Space_Anomaly_(colony_idx) != 0) {
                colony->maintenance[ECON_BC] = 0;
            } else {
                int32_t total_building_maintenance = 0;
                for (int32_t i = 0; i < 49; ++i) {
                    if (colony->buildings[i] != 0) {
                        total_building_maintenance += TECHDATA::_buildings[i].maintenance;
                    }
                }

                int32_t climate_modifier = static_cast<int32_t>(_climate_maintenance_modifier[static_cast<uint8_t>(colony->climate)]) + 100;
                int32_t base_cost = static_cast<uint8_t>(colony->n_food_replicated) + total_building_maintenance;

                int32_t calculated_bc = (((climate_modifier * base_cost) * 4 + 200) * 25) / 10000;
                colony->maintenance[ECON_BC] = static_cast<int8_t>(calculated_bc);
            }
        }

        colony->imports[ECON_BC] = static_cast<int16_t>(static_cast<uint8_t>(colony->maintenance[ECON_BC])) - colony->production[ECON_BC];
    }

    void __cdecl Update_Player_Stats_(s_player* plr) {
        int player_idx = static_cast<int>(plr - MOX::_player);

        int total_research = 0;
        int total_bc = 0;
        int total_pop = 0;
        int total_food = 0;
        int total_industry = 0;
        int total_surplus_food = 0;
        int total_food_maintenance = 0;

        // Process Colonies
        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];
            if (colony->owner == player_idx && colony->outpost_flag == 0) {
                total_pop += static_cast<uint8_t>(colony->n_pops);
                total_food += colony->production[ECON_FOOD];

                if (!Colony_Is_Blockaded(colony)) {
                    total_surplus_food += colony->production[ECON_FOOD];
                    total_food_maintenance += static_cast<uint8_t>(colony->maintenance[ECON_FOOD]);
                }

                total_industry += colony->production[ECON_INDUSTRY];

                if (EVENTS::Event_Check_Colony_Researching_(static_cast<int16_t>(i)) == 0) {
                    total_research += colony->production[2];
                }
                total_bc += colony->production[ECON_BC];
            }
        }

        // Process Ships (Science Labs)
        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            if (ship->owner == player_idx) {
                // Check for Science Lab bit (0x1c = 28)?
                if (struct_::Test_Bit_Field_(ship->d.special_device_flags, 0x1c) && ship->status < 4) {
                    total_research += (1 << (ship->d.size & 0x1F));
                }
            }
        }

        // Process Leaders
        for (int i = 0; i < 67; ++i) {
            s_leader_data* leader = &MOX::_leaders[i];
            if (Player_Owns_Officer_(plr, leader) && Leader_At_Anomaly_(leader) == 0) {
                uint32_t skills = leader->general_skills;

                // Financial skills (Famous, Charismatic)
                if (skills & 0x200) {
                    total_bc += 15;
                }
                else if (skills & 0x100) {
                    total_bc += 10;
                }

                // Science skills
                if (skills & 0x2000) {
                    int level_bonus = Owned_Officer_Level_(i) * 5 + 5;
                    total_research += (level_bonus * 15) / 10;
                }
                else if (skills & 0x1000) {
                    int level_bonus = Owned_Officer_Level_(i) * 5 + 5;
                    total_research += level_bonus;
                }
            }
        }

        // Process Diplomacy & Agreements
        plr->tribute_income = 0;
        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (plr->trade_treaty[i]) {
                total_bc += plr->current_trade_agreement_level[i];
            }

            if (plr->research_treaty[i]) {
                int16_t res_level = plr->current_research_agreement_level[i];
                if (res_level < 0) {
                    total_bc += res_level;
                }
                else {
                    total_research += res_level;
                }
            }

            if (plr->tribute_treaty[i] != 0) {
                int tribute = Tribute_Received_From_(player_idx, i);
                plr->tribute_income += tribute;
            }
        }

        total_bc += plr->tribute_income;

        // Update Player Record
        plr->total_pop = static_cast<int16_t>(total_pop);
        plr->food_produced = static_cast<int16_t>(total_food);
        plr->industry_produced = static_cast<int16_t>(total_industry);

        if (total_research > 0x7FFF) {
            plr->research_produced = 0x7FFF;
        }
        else {
            plr->research_produced = static_cast<int16_t>(total_research);
        }

        int surplus_food = total_surplus_food - total_food_maintenance;
        plr->surplus_food = static_cast<int16_t>(surplus_food);

        if (surplus_food > 0) {
            if (plr->traits[TRAIT_FANTASTIC_TRADERS] == 0) {
                surplus_food /= 2;
            }
            total_bc += surplus_food;
        }

        if (total_bc > 0x7FFF) {
            total_bc = 0x7FFF;
        }
        plr->bc_produced = static_cast<int16_t>(total_bc);

        Compute_Player_Maintenance_(plr, nullptr);
        plr->surplus_bc = plr->bc_produced - static_cast<int16_t>(plr->total_maintenance);
    }

    short __cdecl Double_Add_Two_(short value) {
        return (value * 2) + 2;
    }

    int __cdecl Size_And_Climate_Max_Population_(int8_t size, int climate, bool is_immune) {
        // Calculate population factor based on size and climate
        int32_t population_factor = (int32_t)(is_immune ? 1 : 0) * 25 + _size_climate_max_pop_lookup[climate];

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

    /**
     * @address 004f75a0
     */
    /**
     * Calculates the gravity-based production factor for a colony and player.
     * 
     * @param colony Pointer to the colony data.
     * @param player_idx Index of the player (0-7).
     * @return A production factor (e.g., 4 for 100%, 3 for 75%, 2 for 50%).
     */
    void __cdecl Do_Player_Colony_Calculations_(int16_t player_idx) {
        MOX::Mox_Update_();

        s_colony* colony = MOX::_colony;

        while (colony < MOX::_colony + MOX::_NUM_COLONIES) {
            if (colony->owner == (int8_t)player_idx && colony->outpost_flag == 0) {
                Pre_Import_Computing_(colony);
                MOX::Mox_Update_();
            }
            colony++;
        }

        Do_Player_Colony_Post_Production_(player_idx);
        MOX::Mox_Update_();
    }

    /**
     * Determines if a player is eligible to acquire a specific technology application.
     * Checks requirements based on player traits, game settings (Xenons, Strategic Combat),
     * and technology metadata.
     */
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

    int __cdecl Gravity_Player_Production_Factor_(void* colony, int16_t player_idx) {
        s_colony* c = static_cast<s_colony*>(colony);
        if (c->buildings[BUILDING_GRAVITY_GENERATOR] != 0 || player_idx >= 8) {
            return 4;
        }

        int16_t planet_idx = c->planet;
        int8_t gravity_class = MOX::_planet[planet_idx].gravity_class;
        
        auto& player = MOX::_player[player_idx];

        if (gravity_class == PLANET_GRAVITY_LOW_G) { // Low Gravity
            return (player.traits[TRAIT_LOW_G_WORLD] == 0) ? 3 : 4;
        }

        if (gravity_class == PLANET_GRAVITY_NORMAL_G) { // Normal Gravity
            return (player.traits[TRAIT_LOW_G_WORLD] == 0) ? 4 : 3;
        }

        if (gravity_class == PLANET_GRAVITY_HEAVY_G) { // Heavy Gravity
            return (player.traits[TRAIT_HEAVY_G_WORLD] == 0) ? 2 : 4;
        }

        return 4; 
    }

    int16_t __cdecl Colony_Job_Production_(struct s_colony* colony, int job_type, void* breakdown) {
        char* debug_out = (char*)breakdown;
        bool is_blockaded = Colony_Is_Blockaded(colony);
        int officer_idx = Colony_Officer_(colony);
        
        s_player* player = &MOX::_player[colony->owner];
        int8_t race_trait_0 = player->traits[0];
        bool is_uncreative = (race_trait_0 / 2) != 3;

        int16_t officer_bonus = 0;
        if (officer_idx != -1) {
            s_leader_data* leader = &MOX::_leaders[officer_idx];
            if (job_type == 0) { // Farming
                if (leader->special_skills & 0x8) {
                    officer_bonus = (Owned_Officer_Level_(officer_idx) + 1) * 3;
                } else if (leader->special_skills & 0x4) {
                    officer_bonus = Owned_Officer_Level_(officer_idx) * 2 + 2;
                }
            } else if (job_type == 1) { // Industry
                if (leader->special_skills & 0x200) {
                    officer_bonus = (Owned_Officer_Level_(officer_idx) + 1) * 3;
                } else if (leader->special_skills & 0x100) {
                    officer_bonus = Owned_Officer_Level_(officer_idx) * 2 + 2;
                }
            } else if (job_type == 2) { // Research
                if (leader->special_skills & 0x2000) {
                    officer_bonus = (Owned_Officer_Level_(officer_idx) + 1) * 3;
                } else if (leader->special_skills & 0x1000) {
                    officer_bonus = Owned_Officer_Level_(officer_idx) * 2 + 2;
                }
            }
        }

        s_food_production_breakdown total_debug, current_pop_debug;

        if (debug_out) {
            memset(&total_debug, 0, sizeof(total_debug));
        }

        int total_accumulated_prod = 0;
        int n_pops = (uint8_t)colony->n_pops;

        for (int i = 0; i < n_pops; ++i) {
            s_pop_info* p = &colony->pop[i];
            uint32_t pop_data = *reinterpret_cast<uint32_t*>(p);
            
            // Check if population is assigned to this job
            if ((pop_data & 0x200) && ((pop_data & 0x180) >> 7) == (uint32_t)job_type) {
                uint8_t pop_race = pop_data & 0xF;
                int16_t gravity_penalty = (int16_t)Gravity_Player_Production_Factor_(colony, (int16_t)pop_race);

                if (debug_out) {
                    memset(&current_pop_debug, 0, sizeof(current_pop_debug));
                }

                int16_t base_prod = Colony_Pop_Base_Prod_Produced_(colony, p, (int16_t)job_type, debug_out ? &current_pop_debug : nullptr);
                
                int32_t pop_total = 0;
                int32_t base_scaled = (int32_t)base_prod * 20;
                int32_t gravity_mod = (4 - gravity_penalty) * (int32_t)base_prod * -5;
                int32_t off_mod = (int32_t)officer_bonus * base_prod;
                int32_t captive_mod = 0;
                if (pop_data & 0x400) captive_mod = (int32_t)base_prod * -5;
                
                int32_t morale_mod = 0;
                bool calc_morale = is_uncreative;
                if (job_type != 1 && (pop_race > 7 && pop_race != 9 && pop_race == 8)) {
                    calc_morale = false;
                }
                if (calc_morale) morale_mod = (int32_t)colony->morale * base_prod;

                int32_t race_mod = 0;
                int32_t blockade_mod = 0;
                if (job_type < 2) {
                    if (race_trait_0 == 6) race_mod = (int32_t)base_prod * 10;
                    else if (race_trait_0 == 7) race_mod = base_scaled;
                    if (is_blockaded) blockade_mod = (int32_t)base_prod * -10;
                } else if (job_type == 2) {
                    if (race_trait_0 == 0) race_mod = (int32_t)base_prod * -10;
                    else if (race_trait_0 == 1) race_mod = (int32_t)base_prod * -5;
                    else if (race_trait_0 == 4) race_mod = (int32_t)base_prod * 10;
                    else if (race_trait_0 == 5) race_mod = (int32_t)base_prod * 15;
                }

                int32_t difficulty_mod = 0;
                if (player->objectives != 100) {
                    difficulty_mod = (int32_t)_colony_job_production_difficulty_lookup[MOX::_settings.game_difficulty];
                }

                pop_total = base_scaled + gravity_mod + off_mod + captive_mod + morale_mod + race_mod + blockade_mod + difficulty_mod;
                
                if (pop_total > 0) total_accumulated_prod += pop_total;

                if (debug_out) {
                    total_debug.workers += current_pop_debug.workers;
                    for (int k = 0; k < 204; ++k) {
                        reinterpret_cast<int16_t*>(&total_debug.race_bonus[0])[k] += 
                            reinterpret_cast<int16_t*>(&current_pop_debug.race_bonus[0])[k];
                    }
                    total_debug.gravity_penalty += gravity_mod;
                    total_debug.conquered_penalty += captive_mod;
                    total_debug.morale_modifier += morale_mod;
                    total_debug.blockade_penalty += blockade_mod;
                    total_debug.leader_bonus += off_mod;
                }
            }
        }

        if (debug_out) {
            memcpy(debug_out, &total_debug, sizeof(total_debug));
        }

        if (job_type == 0) return (int16_t)((total_accumulated_prod + 20) / 40);
        return (int16_t)((total_accumulated_prod + 10) / 20);
    }

    void __cdecl Compute_Player_Maintenance_(s_player* plr, char* debug_out) {
        int16_t player_idx = static_cast<int16_t>(plr - MOX::_player);

        plr->command_points = 0;
        plr->command_points_used = 0;
        for (int i = 0; i < 6; ++i) {
            plr->maintenance[i] = 0;
        }

        int32_t leader_cp_bonus = 0;
        int32_t bonus_leader_idx = -1;

        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            s_leader_data* leader = &MOX::_leaders[i];
            if (leader->player_index == player_idx && leader->status >= 0 && leader->status < 3) {
                if (Leader_At_Anomaly_(leader) == 0) {
                    plr->maintenance[5] += OFFICER::Officer_Maintenance_(player_idx, i);
                    
                    int32_t cp_val = 0;
                    if (leader->general_skills & 0x800) {
                        cp_val = OFFICER::Owned_Officer_Level_(i) * 3 + 3;
                    } else if (leader->general_skills & 0x400) {
                        cp_val = OFFICER::Owned_Officer_Level_(i) * 2 + 2;
                    }

                    if (leader_cp_bonus < cp_val) {
                        leader_cp_bonus = cp_val;
                        bonus_leader_idx = i;
                    }
                }
            }
        }

        int32_t n_star_bases = 0;
        int32_t n_battlestations = 0;
        int32_t n_fortresses = 0;
        int32_t warlord_bonus = 0;

        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* col = &MOX::_colony[i];
            if (col->owner == player_idx && col->outpost_flag == 0) {
                plr->maintenance[0] += (uint16_t)(uint8_t)col->maintenance[ECON_BC];
                
                if (plr->traits[TRAIT_WARLORD] != 0) {
                    warlord_bonus += 2;
                }

                if (col->buildings[BUILDING_STAR_BASE] != 0) {
                    n_star_bases++;
                } else if (col->buildings[BUILDING_BATTLESTATION] != 0) {
                    n_battlestations++;
                } else if (col->buildings[BUILDING_STAR_FORTRESS] != 0) {
                    n_fortresses++;
                }
            }
        }

        int32_t comm_tech_idx = -1;
        int32_t comm_tech_bonus_per_base = 0;
        if (plr->tech_applications[TECH_APP_HYPERSPACE_COMMUNICATIONS] == 3) {
            comm_tech_idx = TECH_APP_HYPERSPACE_COMMUNICATIONS;
            comm_tech_bonus_per_base = 3;
        } else if (plr->tech_applications[TECH_APP_SUB_SPACE_COMMUNICATIONS] == 3) {
            comm_tech_idx = TECH_APP_SUB_SPACE_COMMUNICATIONS;
            comm_tech_bonus_per_base = 2;
        } else if (plr->tech_applications[TECH_APP_TACHYON_COMMUNICATIONS] == 3) {
            comm_tech_idx = TECH_APP_TACHYON_COMMUNICATIONS;
            comm_tech_bonus_per_base = 1;
        }

        int32_t comm_tech_total = comm_tech_bonus_per_base * (n_star_bases + n_battlestations + n_fortresses);
        int16_t base_cp = (int16_t)(n_fortresses * 3 + n_battlestations * 2 + warlord_bonus + comm_tech_total + n_star_bases + 5 + leader_cp_bonus);
        plr->command_points = base_cp;

        int32_t imperium_bonus = 0;
        if (plr->traits[TRAIT_CURRENT_GOVERNMENT] == 3) { // FEAT_GOVERNMENT_IMPERIUM
            imperium_bonus = base_cp / 2;
            plr->command_points = base_cp + (int16_t)imperium_bonus;
        }

        if (debug_out) {
            char* ptr = debug_out;
            ptr += _sprintf(ptr, s_fmt_val_msg, 5, _d_from_1_fortress_msg);
            
            if (n_star_bases > 0) {
                if (n_star_bases == 1) ptr += _sprintf(ptr, _d_from_1_star_base_msg, 1);
                else ptr += _sprintf(ptr, _d_from_d_star_bases_msg, n_star_bases, n_star_bases);
            }
            if (n_battlestations > 0) {
                if (n_battlestations == 1) ptr += _sprintf(ptr, _d_from_1_battlestation_msg, 2);
                else ptr += _sprintf(ptr, _d_from_d_battlestations_msg, n_battlestations * 2, n_battlestations);
            }
            if (n_fortresses > 0) {
                if (n_fortresses == 1) ptr += _sprintf(ptr, _d_from_1_fortress_msg_alt, 3);
                else ptr += _sprintf(ptr, _d_from_d_fortresses_msg, n_fortresses * 3, n_fortresses);
            }
            if (comm_tech_total != 0) {
                ptr += _sprintf(ptr, s_fmt_val_name, comm_tech_total, TECHDATA::_technology_applications[comm_tech_idx].name);
            }
            if (leader_cp_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_val_name, leader_cp_bonus, MOX::_leaders[bonus_leader_idx].name);
            }
            if (warlord_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_val_msg, warlord_bonus, _from_warlord_msg);
            }
            if (imperium_bonus != 0) {
                ptr += _sprintf(ptr, s_fmt_val_msg, imperium_bonus, _from_imperium_msg);
            }
            if (plr->command_points > 5) {
                ptr += _sprintf(ptr, s_fmt_val_msg, (int)plr->command_points, _total_command_points_msg);
            }
            ptr += _sprintf(ptr, s_separator);
            debug_out = ptr;
        }

        int32_t active_freighters = plr->n_freighters;
        if (plr->surplus_freighters > 0) {
            active_freighters -= plr->surplus_freighters;
        }
        plr->maintenance[1] = (int16_t)(active_freighters / 2);

        int32_t ship_counts[9] = {0};
        int32_t noncombat_ships = 0;

        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* s = &MOX::_ship[i];
            if (s->owner == player_idx && s->status < 3 && s->just_built == 0) {
                if (s->d.ship_type == SHIP_TYPE_COMBAT) {
                    if (s->d.size < 9) {
                        ship_counts[s->d.size]++;
                    }
                } else {
                    noncombat_ships++;
                }
                plr->command_points_used += (int16_t)(s->d.size + 1);
            }
        }

        if (debug_out) {
            char* ptr = debug_out;
            for (int i = 0; i < 9; ++i) {
                if (ship_counts[i] > 0) {
                    if (ship_counts[i] == 1) {
                        ptr += _sprintf(ptr, _d_from_1_s_ship_msg, i + 1, TECHDATA::_hull_data[i].name);
                    } else {
                        ptr += _sprintf(ptr, _d_from_d_s_ships_msg, (i + 1) * ship_counts[i], ship_counts[i], TECHDATA::_hull_data[i].size_name);
                    }
                }
            }
            if (noncombat_ships > 0) {
                if (noncombat_ships == 1) ptr += _sprintf(ptr, _d_from_1_noncombat_ship_msg, 1);
                else ptr += _sprintf(ptr, _d_from_d_noncombat_ships_msg, noncombat_ships, noncombat_ships);
            }
            const char* used_msg = (plr->command_points_used == 1) ? _total_command_point_used_msg : _total_command_points_used_msg;
            ptr += _sprintf(ptr, s_fmt_val_msg, (int)plr->command_points_used, used_msg);
            ptr += _sprintf(ptr, s_separator);
            debug_out = ptr;
        }

        if (plr->command_points_used != 0) {
            if (plr->command_points < plr->command_points_used) {
                int32_t diff = plr->command_points_used - plr->command_points;
                int32_t cost_mult = (plr->objectives == 'd') ? 10 : (12 - MOX::_settings.game_difficulty);
                plr->maintenance[2] = (int16_t)(cost_mult * diff);
                if (debug_out) {
                    _sprintf(debug_out, s_msg_bc_calc, _ship_maintenance_costs_msg, 0, (int)plr->command_points, (int)cost_mult, (int)diff, (int)plr->maintenance[2]);
                }
            } else {
                plr->maintenance[2] = 0;
                if (debug_out) {
                    _sprintf(debug_out, s_msg_bc_zero, _ship_maintenance_costs_msg);
                }
            }
        }

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            plr->maintenance[3] += (int16_t)BILL::Get_Their_Spy_Number_(player_idx, (int16_t)i);
        }

        plr->maintenance[4] = 0;
        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (plr->tribute_treaty[i] != 0) {
                plr->maintenance[4] += (int16_t)Tribute_Received_From_(player_idx, i);
            }
        }

        plr->total_maintenance = 0;
        for (int i = 0; i < 6; ++i) {
            plr->total_maintenance += plr->maintenance[i];
        }
    }
}
