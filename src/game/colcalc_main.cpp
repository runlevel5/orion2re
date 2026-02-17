#include "deps.h"

namespace COLCALC {
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
                }
                else if (leader->special_skills & 0x4000) {
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
            }
            else if (colony->buildings[BUILDING_ARMOR_BARRACKS]) {
                barracks_bonus = _barracks_morale_lookup[trait0];
            }
            else if (gov_type < 2) {
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
                    }
                    else {
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
                }
                else {
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
                }
                else {
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
                }
                else {
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
                }
                else {
                    consecutive_skips++;
                }
                if (++needy_idx == num_needy) needy_idx = 0;
            }

            int16_t freighted = total_surplus - remaining_food;
            player->food_freighted = freighted;
            if (remaining_capacity == 0) {
                int16_t min_freight = (total_deficit <= total_surplus) ? total_deficit : total_surplus;
                player->surplus_freighters = freighted - min_freight;
            }
            else {
                player->surplus_freighters = capacity - freighted;
            }
        }
        else {
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
            int8_t r = colony->pop[i] & 0x0F;
            if (r < 10) {
                race_pop_count[r]++;
                if (colony->pop[i] & 0x0200 || colony->pop[i] & 0x0400) {
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
                }
                else if (owner_ptr->tech_applications[TECH_APP_MICROBIOTICS] == 3) {
                    growth_modifier += 25;
                }

                if (leader_idx != -1) {
                    uint32_t skills = MOX::_leaders[leader_idx].special_skills;
                    int level = OFFICER::Owned_Officer_Level_(leader_idx);
                    if (skills & 0x800) { // Medicine skill
                        growth_modifier += (level * 3 + 3) * 5;
                    }
                    else if (skills & 0x400) { // Science skill
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
                }
                else if (growth_modifier > 0 && colony->occupation_policy == 0) {
                    growth_modifier = ((race_pop_count[i] - race_conquered_count[i]) * growth_modifier) / race_pop_count[i];
                }

                uint32_t base_growth_val_sq = (static_cast<uint32_t>(race_pop_count[i]) * (limit - total_pops) * 2000) / limit;
                colony->pop_growth[i] += static_cast<int16_t>((isqrt_(base_growth_val_sq) * growth_modifier) / 100 + extra_growth);
            }
        }
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

    void __cdecl Player_Gets_Tech_App_(s_player* plr, int16_t tech_app_id) {
        plr->tech_applications[tech_app_id] = 3;
        plr->got_new_app = 1;

        switch (tech_app_id) {
        case TECH_APP_BATTLEOIDS:
            if (Player_May_Get_Tech_App_(plr, TECH_APP_ARMOR_BARRACKS)) {
                Player_Gets_Tech_App_(plr, TECH_APP_ARMOR_BARRACKS);
            }
            break;
        case TECH_APP_CONFEDERATION:
            plr->traits[TRAIT_CURRENT_GOVERNMENT] = 1;
            break;
        case TECH_APP_FEDERATION:
            plr->traits[TRAIT_CURRENT_GOVERNMENT] = 5;
            break;
        case TECH_APP_GALACTIC_UNIFICATION:
            plr->traits[TRAIT_CURRENT_GOVERNMENT] = 7;
            break;
        case TECH_APP_IMPERIUM:
            plr->traits[TRAIT_CURRENT_GOVERNMENT] = 3;
            break;
        case TECH_APP_SUB_SPACE_COMMUNICATIONS:
        case TECH_APP_TACHYON_COMMUNICATIONS:
        case TECH_APP_JUMP_GATE:
        case TECH_APP_STAR_GATE:
            Compute_All_Star_Colony_Stuff_();
            break;
        default:
            INITSHIP::Update_Player_Ship_Designs_((int16_t)(plr - MOX::_player));
            SHIPMOVE::Update_All_Ship_Ranges_();
            break;
        }

        Do_Player_Colony_Calculations_((int16_t)(plr - MOX::_player));

        if (static_cast<uint8_t>(plr->current_research_application) == static_cast<uint16_t>(tech_app_id) &&
            plr->tech_fields[static_cast<uint8_t>(plr->current_research_field)] == 2) {

            bool advance_field = true;
            if (plr->traits[TRAIT_CREATIVE] != 0 || Field_Is_Gray_Box_(static_cast<uint8_t>(plr->current_research_field))) {
                auto& field_data = TECHDATA::_technology_fields[static_cast<uint8_t>(plr->current_research_field)];
                uint16_t field_techs[4] = { field_data.tech1, field_data.tech2, field_data.tech3, field_data.tech4 };

                for (int i = 0; i < 4; ++i) {
                    if (field_techs[i] != static_cast<uint16_t>(tech_app_id) && plr->tech_applications[field_techs[i]] == 1) {
                        advance_field = false;
                        plr->current_research_application = static_cast<char>(field_techs[i]);
                    }
                }
            }

            if (advance_field) {
                Advance_Player_Field_(plr, static_cast<uint8_t>(plr->current_research_field), 1);
                plr->current_research_field = 0;
                plr->research_accumulated = 0;
            }
        }
        else {
            int field_id = TECHDATA::_technology_applications[tech_app_id].tech_field_id;
            if (plr->traits[TRAIT_UNCREATIVE] != 0) {
                Ensure_Uncreative_Field_OK_(plr, field_id);
            }
            if (plr->tech_fields[field_id] == 2) {
                Advance_Player_Field_(plr, field_id, 1);
            }
        }
    }

    int __cdecl Colony_Job_Production_(s_colony* colony, short econ_type, s_colony_job_production* out) {
        s_colony_job_production final;
        s_colony_job_production cur_stat;

        bool is_blockaded = Colony_Is_Blockaded(colony);
        int leader_idx = Colony_Officer_(colony);
        int owner = colony->owner;
        int govt_type = MOX::_player[owner].traits[0];

        memset(&final, 0, sizeof(s_colony_job_production));

        short officer_bonus = 0;
        bool base_morale_applies = (govt_type / 2 != 3 /* Unification */);

        if (leader_idx != -1) {
            auto& leader = MOX::_leaders[leader_idx];
            if (econ_type == 0 /* ECON_FOOD */) {
                if (leader.special_skills & 0x8 /* Labor Leader */) {
                    officer_bonus = (Owned_Officer_Level_(leader_idx) + 1) * 3;
                }
                else if (leader.special_skills & 0x4 /* Farmer */) {
                    officer_bonus = Owned_Officer_Level_(leader_idx) * 2 + 2;
                }
            }
            else if (econ_type == 1 /* ECON_INDUSTRY */) {
                if (leader.special_skills & 0x200 /* Industrialist */) {
                    officer_bonus = (Owned_Officer_Level_(leader_idx) + 1) * 3;
                }
                else if (leader.special_skills & 0x100 /* Manufacturer */) {
                    officer_bonus = Owned_Officer_Level_(leader_idx) * 2 + 2;
                }
            }
            else if (econ_type == 2 /* ECON_RESEARCH */) {
                if (leader.special_skills & 0x2000 /* Science Leader */) {
                    officer_bonus = (Owned_Officer_Level_(leader_idx) + 1) * 3;
                }
                else if (leader.special_skills & 0x1000 /* Researcher */) {
                    officer_bonus = Owned_Officer_Level_(leader_idx) * 2 + 2;
                }
            }
        }

        for (int i = 0; i < colony->n_pops; ++i) {
            uint32_t pop_val = colony->pop[i];
            if ((pop_val & 0x200) && ((pop_val & 0x180) >> 7 == (uint32_t)econ_type)) {
                uint16_t race = pop_val & 0xf;
                short gravity_factor = Gravity_Player_Production_Factor_(colony, race);

                memset(&cur_stat, 0, sizeof(s_colony_job_production));
                short pop_base_prod = Colony_Pop_Base_Prod_Produced_(colony, colony->pop[i], econ_type, out ? &cur_stat : nullptr);

                if (out) {
                    final.base_prod += cur_stat.base_prod;
                    final.racial_bonuses[race] += cur_stat.racial_bonuses[race];
                    for (int j = 0; j < 212; ++j) final.tech_apps[j] += cur_stat.tech_apps[j];
                    for (int j = 0; j < 49; ++j) final.buildings_prod[j] += cur_stat.buildings_prod[j];
                    final.food_bonus += cur_stat.food_bonus;
                    final.artifacts_bonus += cur_stat.artifacts_bonus;
                    final.intelligence_bonus += cur_stat.intelligence_bonus;
                }

                int scaled_base = (int)pop_base_prod * 20;
                cur_stat.base_prod_scaled = scaled_base;
                final.base_prod_scaled += scaled_base;

                bool morale_applies = base_morale_applies;
                if (econ_type != 1 /* ECON_INDUSTRY */ && race == 8 /* Androids/Cybernetic */) {
                    morale_applies = false;
                }

                cur_stat.gravity_penalty = (4 - gravity_factor) * (int)pop_base_prod * -5;
                final.gravity_penalty += cur_stat.gravity_penalty;

                cur_stat.officer_bonus = officer_bonus * (int)pop_base_prod;
                final.officer_bonus += cur_stat.officer_bonus;

                if (pop_val & 0x400 /* Conquered pop */) {
                    cur_stat.conquered_penalty = (int)pop_base_prod * -5;
                    final.conquered_penalty += cur_stat.conquered_penalty;
                }

                if (morale_applies) {
                    cur_stat.morale_bonus = colony->morale * (int)pop_base_prod;
                }
                final.morale_bonus += cur_stat.morale_bonus;

                if (econ_type <= 1 /* FOOD or INDUSTRY */) {
                    if (govt_type == 6 /* Unification */) {
                        cur_stat.govt_bonus = (int)pop_base_prod * 10;
                    }
                    else if (govt_type == 7 /* Galactic Unification */) {
                        cur_stat.govt_bonus = scaled_base;
                    }
                    final.govt_bonus += cur_stat.govt_bonus;

                    if (is_blockaded) {
                        cur_stat.blockaded_penalty = (int)pop_base_prod * -10;
                        final.blockaded_penalty += cur_stat.blockaded_penalty;
                    }
                }
                else if (econ_type == 2 /* RESEARCH */) {
                    switch (govt_type) {
                    case 0: cur_stat.govt_bonus = (int)pop_base_prod * -10; break;
                    case 1: cur_stat.govt_bonus = (int)pop_base_prod * -5; break;
                    case 4: cur_stat.govt_bonus = (int)pop_base_prod * 10; break;
                    case 5: cur_stat.govt_bonus = (int)pop_base_prod * 15; break;
                    }
                    final.govt_bonus += cur_stat.govt_bonus;
                }

                if (MOX::_player[owner].objectives != 100 /* AI player check */) {
                    int diff_bonus = _colony_job_production_difficulty_lookup[MOX::_settings.game_difficulty];
                    cur_stat.difficulty_bonus = diff_bonus;
                    final.difficulty_bonus += diff_bonus;
                }

                cur_stat.final_prod = cur_stat.artifacts_bonus + cur_stat.officer_bonus + cur_stat.unk +
                    cur_stat.difficulty_bonus + cur_stat.morale_bonus + cur_stat.govt_bonus +
                    cur_stat.gravity_penalty + cur_stat.blockaded_penalty +
                    cur_stat.conquered_penalty + scaled_base;

                if (cur_stat.final_prod > 0) {
                    final.final_prod += cur_stat.final_prod;
                }
            }
        }

        if (out) {
            memcpy(out, &final, sizeof(s_colony_job_production));
        }

        if (econ_type == 0 /* ECON_FOOD */) {
            return (final.final_prod + 20) / 40;
        }
        return (final.final_prod + 10) / 20;
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
                    }
                    else if (leader->general_skills & 0x400) {
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
                }
                else if (col->buildings[BUILDING_BATTLESTATION] != 0) {
                    n_battlestations++;
                }
                else if (col->buildings[BUILDING_STAR_FORTRESS] != 0) {
                    n_fortresses++;
                }
            }
        }

        int32_t comm_tech_idx = -1;
        int32_t comm_tech_bonus_per_base = 0;
        if (plr->tech_applications[TECH_APP_HYPERSPACE_COMMUNICATIONS] == 3) {
            comm_tech_idx = TECH_APP_HYPERSPACE_COMMUNICATIONS;
            comm_tech_bonus_per_base = 3;
        }
        else if (plr->tech_applications[TECH_APP_SUB_SPACE_COMMUNICATIONS] == 3) {
            comm_tech_idx = TECH_APP_SUB_SPACE_COMMUNICATIONS;
            comm_tech_bonus_per_base = 2;
        }
        else if (plr->tech_applications[TECH_APP_TACHYON_COMMUNICATIONS] == 3) {
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

        int32_t ship_counts[9] = { 0 };
        int32_t noncombat_ships = 0;

        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* s = &MOX::_ship[i];
            if (s->owner == player_idx && s->status < 3 && s->just_built == 0) {
                if (s->d.ship_type == SHIP_TYPE_COMBAT) {
                    if (s->d.size < 9) {
                        ship_counts[s->d.size]++;
                    }
                }
                else {
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
                    }
                    else {
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
            }
            else {
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
            int32_t race_idx = colony->pop[i] & 0x0F;
            if (race_idx < 8) {
                if (MOX::_player[race_idx].traits[TRAIT_ENVIRONMENT_IMMUNE] == 0) {
                    total_normal_pops++;
                }
                else {
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
        }
        else if (planet_special == SYSTEM_SPECIAL_GEM_DEPOSITS) {
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
        }
        else if (MOX::_player[owner_idx].traits[TRAIT_CURRENT_GOVERNMENT] == 5) {
            gov_bonus_bc = (income_base * 3 + (income_base * 3 < 0 ? 3 : 0)) / 4;
        }

        if (officer_idx != -1) {
            if (MOX::_leaders[officer_idx].special_skills & 0x20) {
                int16_t lvl = Owned_Officer_Level_(officer_idx);
                leader_bonus_bc = ((lvl + 1) * income_base * 3 + 10) / 20;
            }
            else if (MOX::_leaders[officer_idx].special_skills & 0x10) {
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
            }
            else if (morale_bonus < 0) {
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
}