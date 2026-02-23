#include "deps.h"
#include <cstring>

namespace aidudes {

    void All_Colony_AI_() {
        int16_t num_players = (int16_t)MOX::_NUM_PLAYERS;

        for (int16_t i = 0; i < num_players; ++i) {
            if (MOX::_player[i].objectives != 100) {
                MOX::Mox_Update_();
                Colony_AI_(i);
            }
        }

        MOX::Mox_Update_();

        int16_t num_colonies = (int16_t)MOX::_NUM_COLONIES;
        for (int i = 0; i < num_colonies; ++i) {
            if (MOX::_colony[i].producing[0] == -37) {
                MOX::_colony[i].producing[0] = -2;
            }
        }
    }

    void __cdecl Industry_Sanity_Check_() {
        int n_colonies = (int)aidudes::_n_our_unblockaded;
        s_aidudes_colony_info* colony_infos = aidudes::_our_unblockaded;

        for (int i = 0; i < n_colonies; ++i) {
            s_aidudes_colony_info* info = &colony_infos[i];
            uint8_t colony_idx = (uint8_t)info->colony_idx;
            s_colony* col = &MOX::_colony[colony_idx];

            bool is_colony_researching = (col->producing[0] == -2);

            if ((is_colony_researching && aidudes::_p_ai->do_not_research == 0) ||
                EVENTS::Event_Check_Colony_Researching_((int16_t)colony_idx) != 0) {

                for (int j = (int)info->pop_tail_idx - 1; j >= (int)info->first_assignable_pop_idx; --j) {
                    uint32_t* pop_entry = &col->pop[j];

                    if ((*pop_entry & 0x180) != 0x80) {
                        break;
                    }

                    *pop_entry = (*pop_entry & ~0x80) | 0x100;
                }

                COLCALC::Colony_Calculation_((int16_t)colony_idx);
            }
        }
    }

    void __cdecl Colony_AI_Given_Tax_() {
        for (int i = 0; i < aidudes::_n_our_blockaded; ++i) {
            aidudes::Do_Blockaded_Colony_(&aidudes::_our_blockaded[i]);
        }

        for (int i = 0; i < aidudes::_n_our_unblockaded; ++i) {
            aidudes::Do_Unblockaded_Colony_(&aidudes::_our_unblockaded[i]);
        }

        aidudes::Assign_Additional_Research_Industry_();
        aidudes::Assign_Empire_Building_(aidudes::_i_ai);
        aidudes::Assign_Buildings_();
        aidudes::Industry_Sanity_Check_();

        if (aidudes::_n_our_unblockaded > 0) {
            aidudes::Assign_Additional_Unblockaded_Farmers_();
        } else {
            COLCALC::Do_Player_Colony_Post_Production_(aidudes::_i_ai);
        }
    }

    void __cdecl Colony_AI_(int16_t i_ai) {
        s_aidudes_colony_info our_colonies_local[250];

        aidudes::_i_ai = i_ai;
        aidudes::_our_colonies = &our_colonies_local;
        aidudes::_p_ai = &MOX::_player[i_ai];

        aidudes::Collect_AI_Colonies_();

        if (MOX::_settings.game_difficulty > 0 /* DIFF_TUTOR */) {
            if (AIBUILD::Player_War_Level_(aidudes::_i_ai) < 3) {
                aidudes::Clear_Build_Queues_If_New_Tech_(aidudes::_p_ai->got_new_app);
            }
        }

        aidudes::Keep_Ship_Designs_Up_To_Date_();
        aidudes::Freighter_Cheat_(aidudes::_i_ai);
        aidudes::Do_Ship_Upgrade_Cheats_();
        AIBUILD::Empire_Buy_Outright_(aidudes::_i_ai);
        aidudes::Colony_AI_Given_Tax_();
        MOX::Mox_Update_();

        aidudes::_our_colonies = nullptr;
    }

    void __cdecl Clear_Build_Queues_If_New_Tech_(bool force_clear) {
        s_aidudes_colony_info* current_info = *_our_colonies;
        int num_colonies = static_cast<int>(_n_our_colonies);

        if (num_colonies == 0) return;

        for (int i = 0; i < num_colonies; ++i, ++current_info) {
            int8_t colony_idx = current_info->colony_idx;
            s_colony* colony = &MOX::_colony[colony_idx];

            bool should_rebuild = force_clear;
            if (!should_rebuild) {
                int16_t producing = colony->producing[0];
                if (producing == -1 /* Nothing */ || producing == -3 /* Housing */ || 
                    producing == -2 /* Trade Goods */ || producing == -10 /* Spy */) {
                    should_rebuild = true;
                }
            }

            if (should_rebuild) {
                int16_t producing = colony->producing[0];
                if (producing <= -100) {
                    int ship_idx = -(producing + 100);
                    if (MOX::_ship[ship_idx].status == 4) {
                        return; // Abandon evaluation entirely if an outdated ship is being built
                    }
                }

                int8_t weights[50];
                memset(weights, 0, sizeof(weights));
                weights[49] = 55; // Default/Keep Current weight

                if (colony->n_pops >= 5) {
                    // TECH_APP_SPACEPORT (164), BUILDING_SPACEPORT (39)
                    if (_p_ai->tech_applications[164] == 3 && colony->buildings[39] == 0) {
                        weights[39] = colony->n_pops + 25;
                    }
                    // TECH_APP_PLANETARY_STOCK_EXCHANGE (135), BUILDING_STOCK_EXCHANGE (29)
                    if (_p_ai->tech_applications[135] == 3 && colony->buildings[29] == 0) {
                        weights[29] = colony->n_pops + 20;
                    }
                }

                // TECH_APP_SUBTERRANEAN_FARMS (178), BUILDING_SUBTERRANEAN_FARMS (43)
                if (_p_ai->tech_applications[178] == 3 && colony->buildings[43] == 0) {
                    weights[43] = 20;
                }
                // TECH_APP_HYDROPONIC_FARM (87), BUILDING_HYDROPONIC_FARM (21)
                if (_p_ai->tech_applications[87] == 3 && colony->buildings[21] == 0) {
                    weights[21] = 20;
                }
                // TECH_APP_AUTOMATED_FACTORIES (22), BUILDING_AUTOMATED_FACTORY (7)
                if (_p_ai->tech_applications[22] == 3 && colony->buildings[7] == 0) {
                    weights[7] = 35;
                }
                // TECH_APP_ROBO_MINERS (156), BUILDING_ROBO_MINER_PLANT (36)
                if (_p_ai->tech_applications[156] == 3 && colony->buildings[36] == 0) {
                    weights[36] = 20;
                }
                // TECH_APP_DEEP_CORE_MINING (49), BUILDING_DEEP_CORE_MINE (12)
                if (_p_ai->tech_applications[49] == 3 && colony->buildings[12] == 0) {
                    weights[12] = 10;
                }
                // TECH_APP_ROBOTIC_FACTORY (154), BUILDING_ROBOTIC_FACTORY (34)
                if (_p_ai->tech_applications[154] == 3 && colony->buildings[34] == 0) {
                    weights[34] = 7;
                }

                if (colony->n_pops >= 5) {
                    // TECH_APP_RECYCLOTRON (152), BUILDING_RECYCLOTRON (33)
                    if (_p_ai->tech_applications[152] == 3 && colony->buildings[33] == 0) {
                        weights[33] = colony->n_pops + 15;
                    }
                    if (_p_ai->do_not_research == 0) {
                        // TECH_APP_RESEARCH_LABORATORY (155), BUILDING_RESEARCH_LAB (35)
                        if (_p_ai->tech_applications[155] == 3 && colony->buildings[35] == 0) {
                            weights[35] = 20;
                        }
                        // TECH_APP_PLANETARY_SUPERCOMPUTER (136), BUILDING_SUPERCOMPUTER (30)
                        if (_p_ai->tech_applications[136] == 3 && colony->buildings[30] == 0) {
                            weights[30] = 10;
                        }
                    }
                }

                int player_idx = static_cast<int>(_p_ai - MOX::_player);
                int trait_check = MOX::_player[player_idx].traits[0] / 2;

                if (trait_check != 3) {
                    if (trait_check == 1 || trait_check == 0) {
                        // TECH_APP_MARINE_BARRACKS (103), BUILDING_MARINE_BARRACKS (22), BUILDING_ARMOR_BARRACKS (2)
                        if (_p_ai->tech_applications[103] == 3 && colony->buildings[22] == 0 && colony->buildings[2] == 0) {
                            weights[22] = 37;
                        }
                        // TECH_APP_ARMOR_BARRACKS (14), BUILDING_ARMOR_BARRACKS (2), BUILDING_MARINE_BARRACKS (22)
                        if (_p_ai->tech_applications[14] == 3 && colony->buildings[2] == 0 && colony->buildings[22] == 0) {
                            weights[2] = 10;
                        }
                    }
                    if (colony->n_pops >= 3) {
                        // TECH_APP_HOLO_SIMULATOR (86), BUILDING_HOLO_SIMULATOR (20)
                        if (_p_ai->tech_applications[86] == 3 && colony->buildings[20] == 0) {
                            weights[20] = colony->n_pops + 25;
                        }
                        // TECH_APP_PLEASURE_DOME (141), BUILDING_PLEASURE_DOME (31)
                        if (_p_ai->tech_applications[141] == 3 && colony->buildings[31] == 0) {
                            weights[31] = colony->n_pops + 10;
                        }
                    }
                }

                // TECH_APP_BIOSPHERES (61), BUILDING_BIOSPHERES (15)
                if (_p_ai->tech_applications[61] == 3 && colony->buildings[15] == 0) {
                    weights[15] = 30;
                }

                if (colony->pollution >= 5 && current_info->eats_minerals == 0) {
                    // TECH_APP_CORE_WASTE_DUMPS (50), BUILDING_CORE_WASTE_DUMP (13)
                    if (_p_ai->tech_applications[50] == 3 && colony->buildings[13] == 0) {
                        weights[13] = static_cast<int8_t>(isqrt_(colony->pollution)) + 8;
                    }
                    // TECH_APP_ATMOSPHERIC_RENEWER (19), BUILDING_ATMOSPHERE_RENEWER (5)
                    if (_p_ai->tech_applications[19] == 3 && colony->buildings[5] == 0) {
                        weights[5] = static_cast<int8_t>(isqrt_(colony->pollution)) + 8;
                    }
                    // TECH_APP_POLLUTION_PROCESSOR (142), BUILDING_POLLUTION_PROCESSOR (32)
                    if (_p_ai->tech_applications[142] == 3 && colony->buildings[32] == 0) {
                        weights[32] = 10;
                    }
                }

                // TECH_APP_PLANETARY_GRAVITY_GENERATOR (131), BUILDING_GRAVITY_GENERATOR (25)
                if (_p_ai->tech_applications[131] == 3 && colony->buildings[25] == 0) {
                    int8_t gravity = MOX::_planet[colony->planet].gravity_class;
                    if (gravity == 0) {
                        if (_p_ai->traits[10 /* TRAIT_LOW_G_WORLD */] == 0) {
                            weights[25] = 10;
                        }
                    } else if (gravity == 1) {
                        if (_p_ai->traits[10 /* TRAIT_LOW_G_WORLD */] != 0) {
                            weights[25] = 10;
                        }
                    } else if (gravity == 2) {
                        if (_p_ai->traits[11 /* TRAIT_HEAVY_G_WORLD */] == 0) {
                            weights[25] = 20;
                        }
                    }
                }

                int8_t choice = static_cast<int8_t>(RUSS::Get_Weighted_Choice_Char_(weights, 50));
                if (choice < 49) {
                    COLBLDG::Clear_Colony_Queue_(colony_idx);
                    colony->producing[0] = static_cast<int16_t>(choice);
                }
            }
        }
    }

    int __cdecl Sort_Colonies_Blockaded_(const void* a, const void* b) {
        const s_aidudes_colony_info* info1 = static_cast<const s_aidudes_colony_info*>(a);
        const s_aidudes_colony_info* info2 = static_cast<const s_aidudes_colony_info*>(b);

        s_colony* colony1 = &MOX::_colony[info1->colony_idx];
        s_colony* colony2 = &MOX::_colony[info2->colony_idx];

        bool blockaded2 = COLCALC::Colony_Is_Blockaded(colony2);
        bool blockaded1 = COLCALC::Colony_Is_Blockaded(colony1);

        return (int)blockaded2 - (int)blockaded1;
    }

    void __cdecl Keep_Ship_Designs_Up_To_Date_() {
        s_ship_design doom_star_design;
        doom_star_design.cost = 0;

        for (int i = 0; i < (int)aidudes::_n_our_colonies; ++i) {
            int8_t colony_idx = (*aidudes::_our_colonies)[i].colony_idx;
            short producing_id = MOX::_colony[colony_idx].producing[0];

            if (producing_id <= -100) {
                int ship_idx = -producing_id - 100;
                s_ship_data& ship = MOX::_ship[ship_idx];

                if (ship.d.ship_type == 0) {
                    s_ship_design* source_design;
                    if (ship.d.size < 5 /* SIZE_DOOMSTAR */) {
                        source_design = &aidudes::_p_ai->ship_designs[(int)ship.d.size];
                    } else {
                        if (doom_star_design.cost == 0) {
                            Load_Doom_Star_Ship_Design_(aidudes::_i_ai, &doom_star_design);
                        }
                        source_design = &doom_star_design;
                    }
                    memcpy(&ship.d, source_design, sizeof(s_ship_design));
                }
            }
        }
    }

    void __cdecl Collect_AI_Colonies_() {
        _n_our_colonies = 0;
        _n_our_unblockaded = 0;
        _n_our_blockaded = 0;

        memset(*_our_colonies, 0, (size_t)MOX::_NUM_COLONIES * sizeof(s_aidudes_colony_info));

        for (int i = 0; i < (int)MOX::_NUM_COLONIES; ++i) {
            s_colony* colony_ptr = &MOX::_colony[i];

            if (colony_ptr->owner == (char)_i_ai) {
                if (EVENTS::Event_Check_Space_Anomaly_(i) == 0 && colony_ptr->outpost_flag == 0) {
                    (*_our_colonies)[_n_our_colonies].colony_idx = (int8_t)i;

                    if (COLCALC::Colony_Is_Blockaded(colony_ptr)) {
                        _n_our_blockaded++;
                    } else {
                        _n_our_unblockaded++;
                    }
                    _n_our_colonies++;
                }
            }
        }

        _qsort(*_our_colonies, (size_t)_n_our_colonies, sizeof(s_aidudes_colony_info), Sort_Colonies_Blockaded_);

        _our_blockaded = (s_aidudes_colony_info*)*_our_colonies;
        _our_unblockaded = &((*_our_colonies)[_n_our_blockaded]);
    }

    void __cdecl Assign_Buildings_() {
        int count = (int)_n_our_colonies;

        if (count > 0) {
            for (int i = 0; i < count; ++i) {
                uint8_t colony_idx = (uint8_t)(*_our_colonies)[i].colony_idx;
                AIBUILD::Assign_Colony_Building_(&MOX::_colony[colony_idx]);
            }
        }
    }

    void __cdecl Freighter_Cheat_(int32_t player_idx) {
        s_player* plr = &MOX::_player[player_idx];

        if (plr->surplus_freighters < 0 || plr->surplus_food < 0) {
            int32_t threshold = 1 - (int32_t)MOX::_settings.game_difficulty * (int32_t)plr->surplus_freighters;

            if (threshold >= random::Random_(50)) {
                plr->n_freighters += 5;
            } else if (plr->n_freighters == 0 && MOX::_settings.game_difficulty >= 1) {
                plr->n_freighters = 5;
            }
        }
    }

    void __cdecl Do_Ship_Upgrade_Cheats_() {
        s_ai_refit_player* refit_player = &(*AIDATA::_airefit)[aidudes::_i_ai];
        int16_t num_entries = refit_player->n_entries;

        if (MOX::_stardate <= 35035 /* 0x88db */) {
            return;
        }

        if (num_entries == 0) {
            return;
        }

        // Determine if AI is eligible for an upgrade cheat based on difficulty and last refit entry desirability
        int ship_design_idx = refit_player->entry[num_entries - 1].ship_idx;
        int difficulty_bonus = AIBUILD::_difficulty_ship_design[MOX::_settings.game_difficulty];
        int upgrade_count = 0;

        if (random::Random_(900) <= (difficulty_bonus + ship_design_idx) * 2) {
            upgrade_count = 1;
        }

        // AI must have a stable empire (at least 2 colonies and sufficient population) to cheat
        if ((*AIDATA::_ai_player_info)[aidudes::_i_ai].colony_count < 2) {
            upgrade_count = 0;
        } else {
            int total_pop = MOX::_player[aidudes::_i_ai].total_pop;
            if (random::Random_(total_pop) <= 8) {
                upgrade_count = 0;
            }
        }

        // Iterate backwards through refit entries and perform the upgrade if conditions met
        for (int i = num_entries; i > 0; i--) {
            if (upgrade_count == 0) {
                break;
            }

            s_ai_refit_entry* entry = &refit_player->entry[i - 1];
            s_ship_data* ship = &MOX::_ship[entry->field0_0x0];
            int location = ship->location;

            // Skip ships in systems with incoming human activity
            if ((*AIDATA::_enroute_human_ships)[location][3] != 0) {
                continue;
            }

            // Only upgrade if the system is either uncolonized or owned strictly by this AI
            uint8_t has_colony_mask = MOX::_star[location].has_colony;
            uint32_t other_players_mask = ~(1 << aidudes::_i_ai);
            if ((has_colony_mask & other_players_mask) != 0) {
                continue;
            }

            // Apply design upgrade
            if (ship->d.size < 5 /* SIZE_DOOMSTAR */) {
                ship->d = aidudes::_p_ai->ship_designs[(int)ship->d.size];
            } else {
                INITSHIP::Load_Doom_Star_Ship_Design_(aidudes::_i_ai, &ship->d);
            }

            upgrade_count--;

            // Remove the entry from the refit list
            refit_player->n_entries--;
            int entries_to_move = refit_player->n_entries - (i - 1);
            if (entries_to_move > 0) {
                memcpy(&refit_player->entry[i - 1], &refit_player->entry[i], entries_to_move * sizeof(s_ai_refit_entry));
            }
        }
    }

    void __cdecl Assign_Additional_Research_Industry_() {
        int industry_mult = 10;
        int research_mult = 18;

        if (aidudes::_p_ai->surplus_bc < 0) {
            research_mult = 18 + (int)isqrt_((uint32_t)-aidudes::_p_ai->surplus_bc);
        }

        if (aidudes::_p_ai->do_not_research == 0) {
            if (aidudes::_p_ai->objectives == 3) {
                industry_mult = 12;
            } else if (aidudes::_p_ai->objectives == 4) {
                research_mult += 2;
            }
        } else {
            industry_mult = 0;
        }

        COLCALC::Do_Player_Colony_Calculations_(aidudes::_i_ai);

        while (true) {
            s_aidudes_colony_info* best_colony = nullptr;

            if ((uint32_t)aidudes::_p_ai->research_produced * (uint32_t)research_mult < (uint32_t)aidudes::_p_ai->industry_produced * (uint32_t)industry_mult) {
                short max_score = (short)0x8001;
                for (int i = 0; i < (int)aidudes::_n_our_unblockaded; ++i) {
                    if (aidudes::_our_unblockaded[i].worker_to_farmer_score > max_score) {
                        max_score = aidudes::_our_unblockaded[i].worker_to_farmer_score;
                        best_colony = &aidudes::_our_unblockaded[i];
                    }
                }

                if (best_colony == nullptr) {
                    return;
                }
                aidudes::Grab_Scientist_(best_colony);
            } else {
                short min_score = 0x7fff;
                for (int i = 0; i < (int)aidudes::_n_our_unblockaded; ++i) {
                    if (aidudes::_our_unblockaded[i].field9_0xa < min_score) {
                        min_score = aidudes::_our_unblockaded[i].field9_0xa;
                        best_colony = &aidudes::_our_unblockaded[i];
                    }
                }

                if (best_colony == nullptr) {
                    return;
                }
                aidudes::Grab_Worker_(best_colony);
            }

            COLCALC::Colony_Calculation_((int16_t)best_colony->colony_idx);
        }
    }

    void __cdecl Grab_Scientist_(s_aidudes_colony_info* colony_info) {
        uint8_t colony_idx = (uint8_t)colony_info->colony_idx;
        uint8_t cur_pop_idx = (uint8_t)colony_info->cur_pop_idx;

        s_colony& colony = MOX::_colony[colony_idx];

        uint32_t& pop_flags = colony.pop[cur_pop_idx];

        pop_flags = (pop_flags & 0xffffff7f) | 0x100;

        pop_flags |= 0x200;

        colony_info->cur_pop_idx = (int8_t)(cur_pop_idx + 1);

        Compute_Max_Min_R_I_Worthiness_(colony_info);
    }

    void __cdecl Grab_Worker_(s_aidudes_colony_info* info) {
        s_colony* colony = &MOX::_colony[(uint8_t)info->colony_idx];

        info->pop_end_idx--;

        uint32_t& pop_record = colony->pop[(uint8_t)info->pop_end_idx];

        pop_record = (pop_record & 0xFFFFFEFF) | 0x80;

        pop_record |= 0x200;

        Compute_Max_Min_R_I_Worthiness_(info);
    }

    void __cdecl Do_Unblockaded_Colony_(s_aidudes_colony_info* info) {
        s_colony* colony = &MOX::_colony[info->colony_idx];

        // Calculate minimum required workers (ceiling of n_pops / 5)
        int32_t n_required_workers = ((uint8_t)colony->n_pops + 4) / 5;

        Assign_Required_Colonists_(info);

        _Sort_Pops_colony = colony;
        _qsort(&colony->pop[info->first_assignable_pop_idx],
              (uint8_t)colony->n_pops - info->first_assignable_pop_idx,
              sizeof(uint32_t),
              Sort_Pops_Compare_1);

        if (EVENTS::Event_Check_Colony_Researching_((int16_t)info->colony_idx)) {
            n_required_workers = 0;
        }

        COLCALC::Pre_Import_Computing_(colony);

        // Main loop to shift population to workers based on industry needs
        while (info->cur_pop_idx < info->pop_tail_idx) {
            int32_t needed_industry = (uint8_t)colony->industry2_needed_for_conquered +
                                      (uint8_t)colony->industry2_needed_for_our_empire +
                                      (uint8_t)colony->industry2_needed_for_androids +
                                      (uint8_t)colony->industry2_needed_for_assimilated;

            if (info->n_workers >= n_required_workers && needed_industry <= colony->production[1] * 2) {
                break;
            }

            info->pop_tail_idx--;
            uint8_t idx = info->pop_tail_idx;
            // Mark pop as worker and set AI worker flags
            colony->pop[idx] = (colony->pop[idx] & 0xFFFFFEFF) | 0x80 | 0x200;
            info->n_workers++;

            COLCALC::Pre_Import_Computing_(colony);
        }

        info->pop_end_idx = info->pop_tail_idx;
        info->pollution_at_start = (int8_t)colony->pollution;

        _Sort_Pops_colony = colony;
        _qsort(&colony->pop[info->first_assignable_pop_idx],
              info->pop_end_idx - info->cur_pop_idx,
              sizeof(uint32_t),
              Sort_Pops_Compare_2);

        Compute_Max_Min_R_I_Worthiness_(info);
    }

    void __cdecl Do_Blockaded_Colony_(s_aidudes_colony_info* colony_info) {
        s_colony* colony = &MOX::_colony[colony_info->colony_idx];

        Assign_Required_Colonists_(colony_info);

        aidudes::_Sort_Pops_colony = colony;
        _qsort(&colony->pop[colony_info->first_assignable_pop_idx],
              (size_t)colony->n_pops - colony_info->first_assignable_pop_idx,
              sizeof(uint32_t),
              Sort_Pops_FI_);

        if (colony->food2_per_farmer != 0) {
            COLCALC::Pre_Import_Computing_(colony);

            while (colony_info->cur_pop_idx < colony_info->pop_end_idx) {
                int food_needed = (uint8_t)colony->food2_needed_for_our_empire +
                                  (uint8_t)colony->food2_needed_for_assimilated +
                                  (uint8_t)colony->food2_needed_for_conquered +
                                  (uint8_t)colony->food2_needed_for_natives;

                if (food_needed > (colony->production[0] * 2)) {
                    if (colony_info->n_farmers < (int8_t)colony->max_farms) {
                        colony->pop[colony_info->cur_pop_idx] &= ~0x180;
                        colony->pop[colony_info->cur_pop_idx] |= 0x200;
                        colony_info->cur_pop_idx++;
                        colony_info->n_farmers++;
                    }
                } else {
                    colony_info->pop_end_idx--;
                    uint8_t idx = colony_info->pop_end_idx;
                    colony->pop[idx] = (colony->pop[idx] & ~0x100) | 0x80;
                    colony->pop[idx] |= 0x200;
                }
                COLCALC::Pre_Import_Computing_(colony);
            }
        } else {
            int16_t research_status = EVENTS::Event_Check_Colony_Researching_((int16_t)colony_info->colony_idx);
            uint32_t target_bits = (research_status == 0) ? 0x80 : 0x100;

            while (colony_info->cur_pop_idx < colony_info->pop_end_idx) {
                uint32_t current_pop = colony->pop[colony_info->cur_pop_idx];
                colony->pop[colony_info->cur_pop_idx] = (current_pop & ~0x180) | target_bits;
                colony->pop[colony_info->cur_pop_idx] |= 0x200;
                colony_info->cur_pop_idx++;
            }
        }

        COLCALC::Pre_Import_Computing_(colony);
    }
}