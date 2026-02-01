#include "deps.h"

namespace COLCALC {
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
        colony->food_maintenance = 0;
        colony->industry_maintenance = 0;
        colony->research_maintenance = 0;
        colony->bc_maintenance = 0;

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
        int16_t maintenance_val = static_cast<int16_t>(static_cast<uint8_t>(colony->industry_maintenance));
        colony->imports_industry = maintenance_val;
        if (colony->industry_production <= maintenance_val) {
            colony->imports_industry = colony->industry_production;
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
        while (p_pop < p_pop_end && colony->industry_production < (int16_t)colony->industry_maintenance) {
            *p_pop = (*p_pop & 0xfffffeff) | 0x00000080; // Worker flag set, Scientist flag cleared
            Recalculate_Colony_(colony);
            p_pop++;
        }

        // Convert Pops to Farmers if colony has food multipliers and needs to meet maintenance
        if (colony->food2_per_farmer != 0) {
            while (p_pop < p_pop_end && (int16_t)colony->food_maintenance > colony->food_production) {
                *p_pop = (*p_pop & 0xfffffe7f); // Clear Scientist and Worker flags -> Farmer
                Recalculate_Colony_(colony);
                p_pop++;
            }
        }

        // Convert Scientists to Workers if Research production exceeds Industry
        while (p_pop < p_pop_end && colony->research_production > colony->industry_production) {
            *p_pop = (*p_pop & 0xfffffeff) | 0x00000080; // Worker flag set, Scientist flag cleared
            Recalculate_Colony_(colony);
            p_pop++;
        }

        // If Industry is balanced or higher than Research, switch the last adjusted Pop back to Scientist
        if (colony->industry_production >= colony->research_production) {
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
                    int16_t balance = c->food_production - (uint8_t)c->food_maintenance;
                    if (balance < 0) {
                        c->imports_food = 0;
                        if (!Colony_Is_Blockaded(c)) {
                            needy_colony_indices[num_needy++] = i;
                            total_deficit -= balance; // Make positive
                        }
                    } else {
                        c->imports_food = -balance; // Negative means exporting
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
                if (remaining_food > 0 && (uint8_t)c->food2_needed_for_our_empire > (c->food_production + c->imports_food) * 2) {
                    remaining_food--;
                    remaining_capacity--;
                    c->imports_food++;
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
                if (remaining_food > 0 && needs > (c->food_production + c->imports_food) * 2) {
                    remaining_food--;
                    remaining_capacity--;
                    c->imports_food++;
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
                if (remaining_food > 0 && needs > (c->food_production + c->imports_food) * 2) {
                    remaining_food--;
                    remaining_capacity--;
                    c->imports_food++;
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
                if (remaining_food > 0 && (uint8_t)c->food_maintenance > (c->food_production + c->imports_food)) {
                    remaining_food--;
                    remaining_capacity--;
                    c->imports_food++;
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
                c->imports_food = (uint8_t)c->food_maintenance - c->food_production;
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

        int food_pool = (static_cast<int>(colony->imports_food) + static_cast<int>(colony->food_production)) * 2;
        int industry_pool = static_cast<int>(colony->industry_production) * 2;

        int food_needs_empire = static_cast<uint8_t>(colony->food2_needed_for_our_empire);
        int food_needs_assimilated = static_cast<uint8_t>(colony->food2_needed_for_assimilated);
        int food_needs_conquered = static_cast<uint8_t>(colony->food2_needed_for_conquered);
        int food_needs_natives = static_cast<uint8_t>(colony->food2_needed_for_natives);
        int ind_needs_empire = static_cast<uint8_t>(colony->industry2_needed_for_our_empire);
        int ind_needs_androids = static_cast<uint8_t>(colony->industry2_needed_for_androids);
        int ind_needs_assimilated = static_cast<uint8_t>(colony->industry2_needed_for_assimilated);
        int ind_needs_conquered = static_cast<uint8_t>(colony->industry2_needed_for_conquered);

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
                    growth_modifier += ((colony->industry_production - static_cast<uint8_t>(colony->industry_maintenance)) * 40) / total_pops;
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

    void __cdecl Update_Colony_Pop_Specialty_(s_colony* colony) {
        COLCALC::Colony_Pop_Grows_(colony);
        COLCALC::Colony_Specialty_(colony);
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
}
