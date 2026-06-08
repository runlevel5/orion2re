#include "pch.h"

namespace aidudes {

    int16_t _n_our_blockaded;
    int16_t _n_our_unblockaded;
    s_aidudes_colony_info* _our_blockaded;
    s_player* _p_ai;
    s_aidudes_colony_info* _our_unblockaded;
    int16_t _n_our_colonies;
    s_aidudes_colony_info* _Sort_Pops_colony_info;
    s_colony* _Sort_Pops_colony_data;
    int16_t _i_ai;
    s_aidudes_colony_info (*_our_colonies)[MAX_COLONIES];

    void __cdecl All_Colony_AI_() {
        int16_t num_players = (int16_t)MOX::_NUM_PLAYERS;
        s_player* plr = MOX::_player;

        for (int16_t i = 0; i < num_players; ++i, ++plr) {
            if (plr->objectives != 100) {
                RUSS::Mox_Update_();
                Colony_AI_(i);
            }
        }

        RUSS::Mox_Update_();

        s_colony* col = MOX::_colony;
        s_colony* end = MOX::_colony + (int16_t)MOX::_NUM_COLONIES;
        while (col < end) {
            if (col->producing[0] == -37) {
                col->producing[0] = -2;
            }
            ++col;
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
        s_aidudes_colony_info our_colonies_local[MAX_COLONIES];

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
        RUSS::Mox_Update_();

        aidudes::_our_colonies = nullptr;
    }

    void __cdecl Clear_Build_Queues_If_New_Tech_(bool force_clear) {
        s_aidudes_colony_info* current_info = *_our_colonies;
        int num_colonies = (int)_n_our_colonies;

        if (num_colonies == 0) return;

        for (int i = 0; i < num_colonies; ++i, ++current_info) {
            uint8_t colony_idx = current_info->colony_idx;
            s_colony* colony = &MOX::_colony[colony_idx];

            bool should_rebuild = force_clear;
            if (!should_rebuild) {
                int16_t producing = colony->producing[0];
                if (producing == -1 || producing == -3 || 
                    producing == -2 || producing == -10) {
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

                uint8_t weights[50];
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

                int player_idx = (int)(_p_ai - MOX::_player);
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
                        weights[13] = (uint8_t)special::isqrt_(colony->pollution) + 8;
                    }
                    // TECH_APP_ATMOSPHERIC_RENEWER (19), BUILDING_ATMOSPHERE_RENEWER (5)
                    if (_p_ai->tech_applications[19] == 3 && colony->buildings[5] == 0) {
                        weights[5] = (uint8_t)special::isqrt_(colony->pollution) + 8;
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

                int16_t choice = RUSS::Get_Weighted_Choice_Char_(weights, 50);
                if (choice < 49) {
                    COLBLDG::Clear_Colony_Queue_(colony_idx);
                    colony->producing[0] = choice;
                }
            }
        }
    }

    int __cdecl Sort_Colonies_Blockaded_(const void* a, const void* b) {
        const s_aidudes_colony_info* info1 = (const s_aidudes_colony_info*)a;
        const s_aidudes_colony_info* info2 = (const s_aidudes_colony_info*)b;

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
            uint8_t colony_idx = (*aidudes::_our_colonies)[i].colony_idx;
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
                            INITSHIP::Load_Doom_Star_Ship_Design_(aidudes::_i_ai, &doom_star_design);
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
                    (*_our_colonies)[_n_our_colonies].colony_idx = (uint8_t)i;

                    if (COLCALC::Colony_Is_Blockaded(colony_ptr)) {
                        _n_our_blockaded++;
                    } else {
                        _n_our_unblockaded++;
                    }
                    _n_our_colonies++;
                }
            }
        }

        qsort(*_our_colonies, (size_t)_n_our_colonies, sizeof(s_aidudes_colony_info), Sort_Colonies_Blockaded_);

        _our_blockaded = (s_aidudes_colony_info*)*_our_colonies;
        _our_unblockaded = &((*_our_colonies)[_n_our_blockaded]);
    }

    void __cdecl Compute_Max_Min_R_I_Worthiness_(s_aidudes_colony_info* info) {
        if (info->cur_pop_idx < info->pop_end_idx) {
            s_colony* colony = &MOX::_colony[info->colony_idx];

            uint32_t* cur_pop_ptr = &colony->pop[info->cur_pop_idx];
            int16_t cur_prod_2 = COLCALC::Colony_Pop_Prod_Produced_(colony, cur_pop_ptr, 2);
            int16_t cur_prod_1 = COLCALC::Colony_Pop_Prod_Produced_(colony, cur_pop_ptr, 1);
            info->best_worker_to_farmer_score = cur_prod_2 - cur_prod_1;

            uint32_t* end_pop_ptr = &colony->pop[info->pop_end_idx - 1];
            int16_t end_prod_2 = COLCALC::Colony_Pop_Prod_Produced_(colony, end_pop_ptr, 2);
            int16_t end_prod_1 = COLCALC::Colony_Pop_Prod_Produced_(colony, end_pop_ptr, 1);
            info->worst_worker_to_farmer_score = end_prod_2 - end_prod_1;
        } else {
            info->best_worker_to_farmer_score = (int16_t)0x8001;
            info->worst_worker_to_farmer_score = 0x7FFF;
        }
    }

    void __cdecl Assign_Buildings_() {
        s_aidudes_colony_info* it = *_our_colonies;
        s_aidudes_colony_info* end = it + _n_our_colonies;

        while (it < end) {
            AIBUILD::Assign_Colony_Building_(&MOX::_colony[it->colony_idx]);
            ++it;
            end = (*_our_colonies) + _n_our_colonies;
        }
    }

    void __cdecl Freighter_Cheat_(int32_t player_idx) {
        s_player* plr = &MOX::_player[player_idx];

        if (plr->surplus_freighters < 0 || plr->surplus_food < 0) {
            int32_t threshold = 1 - (int32_t)MOX::_settings.game_difficulty * (int32_t)plr->surplus_freighters;
            int32_t roll = (int32_t)random::Random_(50);

            if (threshold >= roll) {
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
        int32_t desirability = refit_player->entry[num_entries - 1].desirability;
        uint8_t difficulty_idx = (uint8_t)MOX::_settings.game_difficulty;
        int32_t difficulty_bonus = (int32_t)AIBUILD::_difficulty_ship_design[difficulty_idx];
        int32_t upgrade_threshold = (difficulty_bonus + desirability) * 2;
        int upgrade_count = 0;

        if ((int32_t)random::Random_(900) <= upgrade_threshold) {
            upgrade_count = 1;
        }

        // AI must have a stable empire (at least 2 colonies and sufficient population) to cheat
        if ((*AIDATA::_ai_player_info)[aidudes::_i_ai].colony_count < 2) {
            upgrade_count = 0;
        } else {
            int32_t total_pop = MOX::_player[aidudes::_i_ai].total_pop;
            if ((int32_t)random::Random_((uint32_t)total_pop) <= 8) {
                upgrade_count = 0;
            }
        }

        // Iterate backwards through refit entries and perform the upgrade if conditions met
        for (int i = num_entries; i > 0; i--) {
            if (upgrade_count == 0) {
                break;
            }

            s_ai_refit_entry* entry = &refit_player->entry[i - 1];
            s_ship_data* ship = &MOX::_ship[entry->ship_idx];
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
                memmove(&refit_player->entry[i - 1], &refit_player->entry[i], entries_to_move * sizeof(s_ai_refit_entry));
            }
        }
    }

    void __cdecl Assign_Additional_Research_Industry_() {
        int industry_mult = 10;
        int research_mult = 18;

        if (aidudes::_p_ai->surplus_bc < 0) {
            research_mult = 18 + (int)special::isqrt_((uint32_t)-aidudes::_p_ai->surplus_bc);
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
                    if (aidudes::_our_unblockaded[i].best_worker_to_farmer_score > max_score) {
                        max_score = aidudes::_our_unblockaded[i].best_worker_to_farmer_score;
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
                    if (aidudes::_our_unblockaded[i].worst_worker_to_farmer_score < min_score) {
                        min_score = aidudes::_our_unblockaded[i].worst_worker_to_farmer_score;
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

        colony_info->cur_pop_idx = cur_pop_idx + 1;

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
        uint8_t n_pops = (uint8_t)colony->n_pops;

        // Calculate minimum required workers (ceiling of n_pops / 5)
        int32_t n_required_workers = (n_pops + 4) / 5;

        Assign_Required_Colonists_(info);

        _Sort_Pops_colony_data = colony;
        qsort(&colony->pop[info->first_assignable_pop_idx],
              n_pops - info->first_assignable_pop_idx,
              4,
              Sort_Pops_FR_I_);

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
        info->pollution_at_start = (uint8_t)colony->pollution;

        _Sort_Pops_colony_data = colony;
        qsort(&colony->pop[info->first_assignable_pop_idx],
              info->pop_end_idx - info->cur_pop_idx,
              4,
              Sort_Pops_R_I_);

        Compute_Max_Min_R_I_Worthiness_(info);
    }

    void __cdecl Do_Blockaded_Colony_(s_aidudes_colony_info* colony_info) {
        s_colony* colony = &MOX::_colony[(uint8_t)colony_info->colony_idx];
        Assign_Required_Colonists_(colony_info);

        _Sort_Pops_colony_data = colony;

        qsort(&colony->pop[(uint8_t)colony_info->first_assignable_pop_idx],
              (uint8_t)colony->n_pops - (uint8_t)colony_info->first_assignable_pop_idx,
              4,
              Sort_Pops_FI_);

        if (colony->food2_per_farmer == 0) {
            int16_t is_researching = EVENTS::Event_Check_Colony_Researching_((uint16_t)(uint8_t)colony_info->colony_idx);
            uint32_t job_mask = (is_researching == 0) ? 0x80 : 0x100;

            while ((uint8_t)colony_info->cur_pop_idx < (uint8_t)colony_info->pop_end_idx) {
                colony->pop[(uint8_t)colony_info->cur_pop_idx] = (colony->pop[(uint8_t)colony_info->cur_pop_idx] & 0xFFFFFE7F) | job_mask;
                colony->pop[(uint8_t)colony_info->cur_pop_idx] |= 0x200;
                colony_info->cur_pop_idx++;
            }
        } else {
            COLCALC::Pre_Import_Computing_(colony);

            while ((uint8_t)colony_info->cur_pop_idx < (uint8_t)colony_info->pop_end_idx) {
                int32_t total_food_needed = (uint8_t)colony->food2_needed_for_our_empire +
                                            (uint8_t)colony->food2_needed_for_assimilated +
                                            (uint8_t)colony->food2_needed_for_conquered +
                                            (uint8_t)colony->food2_needed_for_natives;

                if (total_food_needed > colony->production[0] * 2) {
                    if ((uint8_t)colony_info->n_farmers < (uint8_t)colony->max_farms) {
                        colony->pop[(uint8_t)colony_info->cur_pop_idx] &= 0xFFFFFE7F;
                        colony->pop[(uint8_t)colony_info->cur_pop_idx] |= 0x200;
                        colony_info->cur_pop_idx++;
                        colony_info->n_farmers++;
                    }
                } else {
                    colony_info->pop_end_idx--;
                    colony->pop[(uint8_t)colony_info->pop_end_idx] = (colony->pop[(uint8_t)colony_info->pop_end_idx] & 0xFFFFFEFF) | 0x80;
                    colony->pop[(uint8_t)colony_info->pop_end_idx] |= 0x200;
                }
                COLCALC::Pre_Import_Computing_(colony);
            }
        }

        COLCALC::Pre_Import_Computing_(colony);
    }

    int16_t __cdecl FI_Worthiness_(const s_colony* colony, const uint32_t* pop) {
        int16_t worthiness_0;
        int16_t worthiness_1;

        worthiness_0 = COLCALC::Colony_Pop_Prod_Produced_(colony, pop, 0);
        worthiness_1 = COLCALC::Colony_Pop_Prod_Produced_(colony, pop, 1);

        return worthiness_0 - worthiness_1;
    }

    int __cdecl Sort_Pops_FI_(const void* a, const void* b) {
        const uint32_t* pop1 = (const uint32_t*)a;
        const uint32_t* pop2 = (const uint32_t*)b;
        int16_t worth1 = FI_Worthiness_(_Sort_Pops_colony_data, pop1);
        int16_t worth2 = FI_Worthiness_(_Sort_Pops_colony_data, pop2);

        if (worth2 == worth1) {
            int16_t prod1 = COLCALC::Colony_Pop_Prod_Produced_(_Sort_Pops_colony_data, pop1, 0);
            int16_t prod2 = COLCALC::Colony_Pop_Prod_Produced_(_Sort_Pops_colony_data, pop2, 0);

            // If worthiness is equal, sort by production ascending
            return (int)prod1 - (int)prod2;
        }

        // Sort by worthiness descending
        return (int)worth2 - (int)worth1;
    }

    void __cdecl Assign_Required_Colonists_(s_aidudes_colony_info* colony_info) {
        s_colony* colony = &MOX::_colony[colony_info->colony_idx];
        uint32_t* pop_ptr = colony->pop;
        char do_not_research = MOX::_player[(int)colony->owner].do_not_research;
        int16_t non_immune_pops = 0;

        colony_info->first_assignable_pop_idx = 0;
        colony_info->n_farmers = 0;
        colony_info->n_workers = 0;

        for (int i = 0; i < (int)colony->n_pops; ++i) {
            uint32_t pop_entry = pop_ptr[i];
            uint32_t race_idx = pop_entry & 0xF;

            if (race_idx == 8 /* Android */ || race_idx == 9 /* Native */) {
                colony_info->first_assignable_pop_idx++;
                uint32_t job = (pop_entry >> 7) & 0x3;

                if (job == 0 /* Farmer */) {
                    colony_info->n_farmers++;
                } else if (job == 1 /* Worker */) {
                    colony_info->n_workers++;
                } else if (job == 2 /* Scientist */) {
                    if (do_not_research != 0) {
                        // Clear bit 9 (manual/locked assignment bit) if research is disabled
                        pop_ptr[i] &= ~0x200;
                    }
                }
            } else {
                // For other biological races, clear manual assignment bit 9
                pop_ptr[i] &= ~0x200;
            }

            // Re-evaluate current pop entry state after potential modifications
            uint32_t current_pop = pop_ptr[i];
            uint32_t current_race = current_pop & 0xF;

            // Check if population is affected by pollution (not Android/Native and not Tolerant)
            if (current_race < 8) {
                if (MOX::_player[current_race].traits[TRAIT_ENVIRONMENT_IMMUNE] == 0) {
                    // If not rioting (bit 10) or colony is being assimilated/conquered
                    if (!(current_pop & 0x400) || (colony->occupation_policy >= 2 && colony->occupation_policy <= 3)) {
                        non_immune_pops++;
                    }
                }
            }
        }

        // Determine if Lithovore trait (eats minerals) applies to this colony
        if ((non_immune_pops * 2 < (int)colony->n_pops) &&
            (MOX::_player[(int)colony->owner].traits[TRAIT_EATS_MINERALS] != 0) &&
            (MOX::_player[(int)colony->owner].surplus_food >= 0)) {
            colony_info->eats_minerals = 1;
        }

        // Sort population for AI processing
        aidudes::_Sort_Pops_colony_data = colony;
        qsort(colony->pop, (size_t)colony->n_pops, 4, aidudes::Sort_Pops_Fixed_);

        colony_info->cur_pop_idx = colony_info->first_assignable_pop_idx;
        colony_info->pop_end_idx = colony->n_pops;
        colony_info->pop_tail_idx = colony->n_pops;
    }

    int __cdecl Sort_Pops_Fixed_(const void* a, const void* b) {
        const uint32_t* pop1 = (const uint32_t*)a;
        const uint32_t* pop2 = (const uint32_t*)b;
        uint8_t flag1 = 0;
        uint32_t val1 = *pop1 & 0xF;

        if (val1 == 8 || val1 == 9) {
            flag1 = 1;
        }

        uint8_t flag2 = 0;
        uint32_t val2 = *pop2 & 0xF;

        if (val2 == 8 || val2 == 9) {
            flag2 = 1;
        }

        return (int)flag2 - (int)flag1;
    }

    int __cdecl _Sort_Pops_colony(const void* a, const void* b) {
        s_colony* colony = aidudes::_Sort_Pops_colony_data;

        // Get production values for both pops across two different production types
        int16_t p1_prod_1 = COLCALC::Colony_Pop_Prod_Produced_(colony, (const uint32_t*)a, 1);
        int16_t p2_prod_1 = COLCALC::Colony_Pop_Prod_Produced_(colony, (const uint32_t*)b, 1);
        int16_t p1_prod_2 = COLCALC::Colony_Pop_Prod_Produced_(colony, (const uint32_t*)a, 2);
        int16_t p2_prod_2 = COLCALC::Colony_Pop_Prod_Produced_(colony, (const uint32_t*)b, 2);

        // Calculate differences in production contributions
        int16_t diff1 = p1_prod_2 - p1_prod_1;
        int16_t diff2 = p2_prod_2 - p2_prod_1;

        // Primary sort: difference between production types
        if (diff2 != diff1) {
            return (int)diff2 - (int)diff1;
        }

        // Secondary sort: specific production type 1 contribution
        if (p1_prod_1 != p2_prod_1) {
            return (int)p1_prod_1 - (int)p2_prod_1;
        }

        // Tertiary sort: masked raw value comparison
        uint32_t val1 = *(const uint32_t*)a;
        uint32_t val2 = *(const uint32_t*)b;

        return (int)(val1 & 0x0F) - (int)(val2 & 0x0F);
    }

    void __cdecl Assign_Empire_Building_(int16_t player_idx) {
        s_player* p = &MOX::_player[player_idx];
        s_ai_refit_player* p_refit = &(*AIDATA::_airefit)[player_idx];

        int war_level = AIBUILD::Player_War_Level_(player_idx);
        int colony_queue_count = 0;

        bool is_objective_1 = (p->objectives == 1);
        bool low_money = (p->bc + p->surplus_bc < p->total_maintenance);

        int isqrt_val = 0;
        if (low_money || p->bc < 1500) {
            isqrt_val = 0;
        } else {
            isqrt_val = special::isqrt_((p->surplus_bc + ((p->surplus_bc >> 31) & 0x3f)) >> 6);
        }

        if (isqrt_val > 10) {
            isqrt_val = 10;
        }

        RUSS::Mox_Update_();

        if (p->war_buildup_duration != 0 && war_level == 0) {
            war_level = is_objective_1 + 1;
        }

        bool recalculate_colony = false;
        uint8_t colony_queue[256];

        if (MOX::_NUM_COLONIES > 0) {
            for (int current_colony_idx = 0; current_colony_idx < MOX::_NUM_COLONIES; ++current_colony_idx) {
                s_colony* temp_colony = &MOX::_colony[current_colony_idx];

                if (temp_colony->owner == (char)player_idx &&
                    temp_colony->outpost_flag == 0 &&
                    temp_colony->n_pops != 0) {

                    if (EVENTS::Event_Check_Space_Anomaly_(current_colony_idx) == 0) {
                        RUSS::Mox_Update_();

                        if (COLCALC::Colony_Is_Blockaded(temp_colony) ||
                            (*AIDATA::_ai_colony_info)[current_colony_idx].threatened != 0 ||
                            low_money) {

                            bool delete_from_queue = false;
                            int16_t prod = temp_colony->producing[0];
                            if (prod <= -100) {
                                if (MOX::_ship[-prod - 100].d.ship_type != 0) {
                                    delete_from_queue = true;
                                }
                            } else if (prod > -8) {
                                if (prod > -4) {
                                    if (prod == -2) {
                                        recalculate_colony = true;
                                        delete_from_queue = true;
                                    }
                                } else {
                                    delete_from_queue = true;
                                }
                            }

                            if (delete_from_queue) {
                                COLBLDG::Delete_From_Colony_Queue_(current_colony_idx, 0);
                            }
                        }

                        int16_t prod = temp_colony->producing[0];
                        if (prod == -3) {
                            // pass
                        } else if (prod == -2) {
                            recalculate_colony = true;
                        } else if (prod == -1) {
                            // pass
                        } else {
                            continue;
                        }

                        temp_colony->producing[0] = -38;
                        colony_queue[colony_queue_count] = current_colony_idx;
                        colony_queue_count++;
                    }
                }
            }
        }

        if (recalculate_colony) {
            COLCALC::Do_Player_Colony_Calculations_(player_idx);
        }

        if (colony_queue_count <= 0) {
            return;
        }

        bool no_range_or_speed;
        if (p->ship_range == 0 || p->ship_speed == 0) {
            no_range_or_speed = true;
        } else {
            no_range_or_speed = false;
        }

        int freighter_groups_assigned = 0;
        int16_t best_planet = AIMOVE::Best_Uncolonized_Undestined_Planet_(player_idx);
        bool is_objective_2 = (p->objectives == 2);

        s_ship_design ship_design;
        ship_design.cost = 0;

        for (int i = 0; i < colony_queue_count;) {
            s_colony* temp_colony = &MOX::_colony[colony_queue[i]];
            RUSS::Mox_Update_();
            bool changed = false;

            if ((war_level != 0 && (*AIDATA::_enroute_human_ships)[MOX::_planet[temp_colony->planet].star_index][9] != 0) ||
                AIBUILD::Colony_Is_Conquered_(temp_colony)) {

                if (COLCALC::Colony_Can_Build_Product_(temp_colony, 26)) {
                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 26);
                    changed = true;
                } else if (COLCALC::Colony_Can_Build_Product_(temp_colony, 27)) {
                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 27);
                    changed = true;
                } else if (COLCALC::Colony_Can_Build_Product_(temp_colony, 40)) {
                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 40);
                    changed = true;
                } else if (COLCALC::Colony_Can_Build_Product_(temp_colony, 8)) {
                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 8);
                    changed = true;
                } else if (COLCALC::Colony_Can_Build_Product_(temp_colony, 41)) {
                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 41);
                    changed = true;
                } else if (COLCALC::Colony_Can_Build_Product_(temp_colony, 28)) {
                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 28);
                    changed = true;
                } else if (COLCALC::Colony_Can_Build_Product_(temp_colony, 24)) {
                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 24);
                    changed = true;
                } else if (COLCALC::Colony_Can_Build_Product_(temp_colony, 23)) {
                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 23);
                    changed = true;
                }
            }

            if (!changed) {
                ++i;
            }
        }

        bool want_colony_base = false;
        if (best_planet != -1) {
            if ((*AIDATA::_ai_player_info)[player_idx].n_contacts == 0 ||
                (war_level == 0 && AIBUILD::_player_total_strength_vs_ships[player_idx][0] > 0) ||
                AIBUILD::_player_total_strength_vs_ships[player_idx][2] < 10 ||
                isqrt_val > 1) {

                if (p->tech_applications[41] == 3 && !no_range_or_speed) {
                    int val = (*AIDATA::_contextual_planet_values)[player_idx * MAX_PLANETS + best_planet];
                    if (war_level > 0) {
                        val /= 4;
                    }

                    int deficit = (*AIDATA::_ai_player_info)[player_idx].colony_deficit;
                    int bonus = deficit * 10;
                    if (val / 2 < bonus) {
                        bonus = val / 2;
                    }
                    val += bonus;

                    int rand_val = random::Random_(700);
                    int adjusted_rand = rand_val + 100 + (is_objective_2 ? 0 : 100);

                    if (adjusted_rand < (isqrt_val + 64) * val) {
                        want_colony_base = true;
                    }
                }
            }
        }

        RUSS::Mox_Update_();
        qsort(colony_queue, (uint32_t)colony_queue_count, 1, aidudes::colony_less_industry_cmp_);

        if (p->tech_applications[40] == 3) {
            int i = colony_queue_count;
            while (want_colony_base && i > 0) {
                i--;
                s_colony* temp_colony = &MOX::_colony[colony_queue[i]];
                if (!COLCALC::Colony_Is_Blockaded(temp_colony) && AIBUILD::Industry_Worth(temp_colony) >= 13) {
                    if (MOX::_planet[temp_colony->planet].star_index == MOX::_planet[best_planet].star_index) {
                        if (temp_colony->buildings[11] != 1) {
                            AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 11);
                        }
                        want_colony_base = false;
                    }
                }
            }
        }

        RUSS::Mox_Update_();
        if (want_colony_base && p->tech_applications[41] == 3) {
            int r = 0;
            if (is_objective_2 != 0) {
                if (random::Random_(5) == 1) {
                    r = 1;
                }
            }

            if (AIBUILD::_player_n_colony_ships[player_idx][0] <= r) {
                int i = colony_queue_count;
                while (want_colony_base && i > 0) {
                    i--;
                    s_colony* temp_colony = &MOX::_colony[colony_queue[i]];
                    if (!COLCALC::Colony_Is_Blockaded(temp_colony) && AIBUILD::Industry_Worth(temp_colony) >= 20) {
                        if ((*AIDATA::_ai_player_info)[player_idx].colony_deficit + 50 < (int32_t)random::Random_(100)) {
                            INITSHIP::Load_Colony_Ship_Design_(player_idx, &p->ship_designs[5]);
                            int16_t ship_idx = COLBLDG::Create_Ship_(player_idx, 5, (int16_t)colony_queue[i]);
                            if (ship_idx == -1) {
                                no_range_or_speed = true;
                            } else {
                                MOX::_ship[ship_idx].status = 6;
                                AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, -100 - ship_idx);
                            }
                            want_colony_base = false;
                        }
                    }
                }
            }
        }

        while (colony_queue_count > 0) {
            int prev_count = colony_queue_count;
            int i = colony_queue_count - 1;
            s_colony* temp_colony = &MOX::_colony[colony_queue[i]];

            RUSS::Mox_Update_();

            if (colony_queue_count > 0 &&
                p->bc + p->surplus_bc < p->total_maintenance &&
                p->surplus_bc < 0 &&
                temp_colony->maintenance[1] < temp_colony->production[1]) {

                int current_colony_idx = colony_queue[i];
                AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, -37);
                COLCALC::Colony_Calculation_((int16_t)current_colony_idx);
                continue;
            }

            int vs_ships_2 = AIBUILD::_player_total_strength_vs_ships[player_idx][2];
            int current_colony_idx = colony_queue[i];

            int ship_build_weight = 0;
            if (vs_ships_2 > 0 && p->ship_range != 0 && p->ship_speed != 0 && !no_range_or_speed) {
                int strength_0 = AIBUILD::_player_total_strength_vs_ships[player_idx][0];
                ship_build_weight = (vs_ships_2 * 1000) / (strength_0 + vs_ships_2);
                if (war_level > 0) {
                    ship_build_weight *= 2;
                }
            }

            int freighter_weight = 0;
            int spy_weight = 0;
            int transport_weight = 0;
            int defer_weight = 0;

            uint8_t threatened = (*AIDATA::_ai_colony_info)[current_colony_idx].threatened;
            if (threatened == 0) {
                if (AIBUILD::_player_n_freighter_groups_wanted[player_idx] != 0 &&
                    freighter_groups_assigned <= 1 &&
                    p->tech_applications[69] == 3) {

                    freighter_weight = (AIBUILD::_player_n_freighter_groups_wanted[player_idx] - freighter_groups_assigned * 2) * 20 + 100;
                    if (war_level > 0) {
                        freighter_weight /= (war_level * war_level);
                    }
                }

                if (AIBUILD::_player_n_spies[player_idx][1] != 0) {
                    spy_weight = (AIBUILD::_player_n_spies[player_idx][1] * 4 + 40) * 25;
                    if (war_level > 0) {
                        spy_weight /= (war_level * war_level);
                    }
                }

                if (AIBUILD::_player_n_transports[player_idx][1] != 0 &&
                    p->tech_applications[189] == 3 &&
                    !no_range_or_speed) {
                    transport_weight = (AIBUILD::_player_n_transports[player_idx][1] * 4 + 20) * 25;
                    if (war_level > 0) {
                        transport_weight = (transport_weight * 3) / 2;
                    }
                }
            }

            if (war_level == 0) {
                defer_weight = 333;
            } else {
                defer_weight = 350 - war_level * 100;
            }

            uint32_t choice_weights[6] = {0, 0, 0, 0, 0, 0};
            choice_weights[0] = ship_build_weight;
            choice_weights[1] = freighter_weight;
            choice_weights[3] = spy_weight;
            choice_weights[4] = transport_weight;
            choice_weights[5] = defer_weight;

            while (true) {
                RUSS::Mox_Update_();
                int random_choice = RUSS::Get_Weighted_Choice_Int_(choice_weights, 6);

                if (random_choice == -1 || random_choice == 5) {
                    colony_queue_count--;
                    break;
                }

                switch (random_choice) {
                    case 0:
                    {
                        bool flag_34 = false;
                        bool flag_38 = false;
                        bool flag_2c = false;

                        int ind_worth = AIBUILD::Industry_Worth(temp_colony);
                        if (p->surplus_bc > 5 && p->bc > 50) {
                            ind_worth += special::isqrt_(special::isqrt_(p->bc)) + special::isqrt_(p->surplus_bc);
                        }

                        uint16_t ship_weights[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

                        if (p->ship_designs[0].cost <= (war_level + 15) * ind_worth * 2) {
                            ship_weights[0] = 2;
                            int cost_threshold_1 = (war_level * 2 + 33) * ind_worth;
                            if (p->ship_designs[1].cost <= cost_threshold_1) {
                                ship_weights[1] = 16;
                                ship_weights[0] = 1;

                                if (temp_colony->buildings[41] == 0) {
                                    if (p->tech_applications[169] == 3 &&
                                        COLCALC::Colony_Product_Cost_(MOX::_colony, 41) <= (war_level * 2 + 31) * ind_worth) {
                                        flag_38 = true;
                                        ship_weights[8] = (uint16_t)(((*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].neutral_count + 2 +
                                                           ((*AIDATA::_ai_colony_info)[current_colony_idx].threatened + (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].alliance_count) * 2) * 2 +
                                                           (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].enemy_count) * 2;
                                        if (p->command_points < p->command_points_used) {
                                            ship_weights[8] += (p->command_points_used - p->command_points) * 2;
                                        }
                                    }
                                }

                                if (temp_colony->buildings[8] == 0) {
                                    if (p->tech_applications[27] == 3 &&
                                        COLCALC::Colony_Product_Cost_(MOX::_colony, 8) <= cost_threshold_1) {
                                        flag_34 = true;
                                        ship_weights[7] = (uint16_t)(((*AIDATA::_ai_colony_info)[current_colony_idx].threatened + (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].alliance_count) * 2 +
                                                           (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].neutral_count + 2) * 5 +
                                                          (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].enemy_count * 3;
                                        if (p->command_points < p->command_points_used) {
                                            ship_weights[7] += (p->command_points_used - p->command_points) * 2;
                                        }
                                    }
                                }

                                if (temp_colony->buildings[40] == 0) {
                                    if (p->tech_applications[168] == 3 &&
                                        COLCALC::Colony_Product_Cost_(MOX::_colony, 40) <= (war_level * 2 + 35) * ind_worth) {
                                        flag_2c = true;
                                        ship_weights[6] = (uint16_t)((*AIDATA::_ai_colony_info)[current_colony_idx].threatened + (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].alliance_count) * 10 +
                                                          (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].enemy_count * 4 + 12 +
                                                          (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].neutral_count * 6;
                                        if (p->command_points < p->command_points_used) {
                                            ship_weights[6] += (p->command_points_used - p->command_points) * 2;
                                        }
                                    }
                                }
                            }
                        }

                        if (COLCALC::Colony_Can_Build_Large_Ships_(temp_colony)) {
                            int size_9 = AIBUILD::_player_ship_sizes[player_idx].target_ship_size;
                            if (size_9 >= 2 && p->ship_designs[2].cost <= (war_level * 2 + 45) * ind_worth) {
                                ship_weights[2] = 32;
                                ship_weights[1] /= 5;
                                if (size_9 >= 3 && p->ship_designs[3].cost <= (war_level * 2 + 55) * ind_worth) {
                                    ship_weights[3] = 64;
                                    ship_weights[2] = 5;
                                    if (p->tech_applications[186] == 3 && size_9 >= 4 && p->ship_designs[4].cost <= (war_level * 2 + 48) * ind_worth) {
                                        ship_weights[4] = 100;
                                    }
                                    if (p->tech_applications[55] == 3 && size_9 >= 5) {
                                        if (ship_design.cost == 0) {
                                            INITSHIP::Load_Doom_Star_Ship_Design_(player_idx, &ship_design);
                                        }
                                        if (ship_design.cost <= (war_level * 2 + 48) * ind_worth) {
                                            ship_weights[5] = 150;
                                        }
                                    }
                                }
                            }
                        } else {
                            if (flag_2c) {
                                ship_weights[6] += (*AIDATA::_ai_colony_info)[current_colony_idx].threatened * 7 + 2;
                            }
                            if (flag_34) {
                                ship_weights[7] += (*AIDATA::_ai_colony_info)[current_colony_idx].threatened * 6 + 2;
                            }
                            if (flag_38) {
                                ship_weights[8] += (*AIDATA::_ai_colony_info)[current_colony_idx].threatened * 5 + 2;
                            }
                        }

                        if (AIBUILD::_player_total_strength_vs_ships[player_idx][0] > 0 && random::Random_(5) == 1) {
                            ship_weights[0] = 0;
                        }

                        int ship_choice = RUSS::Get_Weighted_Choice_Short_(ship_weights, 9);
                        switch (ship_choice + 1) {
                            case 0:
                                break;
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            {
                                int16_t created_idx = COLBLDG::Create_Ship_(player_idx, (int16_t)ship_choice, (int16_t)current_colony_idx);
                                if (created_idx == -1) {
                                    no_range_or_speed = true;
                                } else {
                                    MOX::_ship[created_idx].status = 6;
                                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, -100 - created_idx);
                                    AIBUILD::_player_total_strength_vs_ships[player_idx][2] -= AIBUILD::Mean_Ship_Strength_(&MOX::_ship[created_idx]);
                                }
                                break;
                            }
                            case 6:
                            {
                                memcpy(&p->ship_designs[5], &ship_design, sizeof(s_ship_design));
                                int16_t ds_idx = COLBLDG::Create_Ship_(player_idx, 5, (int16_t)current_colony_idx);
                                if (ds_idx == -1) {
                                    no_range_or_speed = true;
                                } else {
                                    MOX::_ship[ds_idx].status = 6;
                                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, -100 - ds_idx);
                                    AIBUILD::_player_total_strength_vs_ships[player_idx][2] -= AIBUILD::Mean_Ship_Strength_(&MOX::_ship[ds_idx]);
                                }
                                break;
                            }
                            case 7:
                                AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 40);
                                AIBUILD::_player_total_strength_vs_ships[player_idx][2] -= ((*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].alliance_count * 2 + (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].neutral_count + (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].enemy_count);
                                break;
                            case 8:
                                AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 8);
                                AIBUILD::_player_total_strength_vs_ships[player_idx][2] -= ((*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].alliance_count * 3 + (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].neutral_count * 2 + (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].enemy_count);
                                break;
                            case 9:
                                AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 41);
                                AIBUILD::_player_total_strength_vs_ships[player_idx][2] -= ((*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].alliance_count * 6 + (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].neutral_count * 4 + (*AIDATA::_ai_star_info)[MOX::_planet[temp_colony->planet].star_index].per_player[player_idx].enemy_count);
                                break;
                        }
                        choice_weights[0] = 0;
                        break;
                    }
                    case 1:
                        if (!COLCALC::Colony_Is_Blockaded(temp_colony) && AIBUILD::Industry_Worth(temp_colony) >= 12) {
                            AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, -15);
                            freighter_groups_assigned++;
                            AIBUILD::_player_n_freighter_groups_wanted[player_idx]--;
                        }
                        choice_weights[1] = 0;
                        break;
                    case 2:
                    {
                        int refit_planet_idx = MOX::_planet[temp_colony->planet].star_index;
                        int n_entries = p_refit->n_entries - 1;
                        bool refitted = false;
                        for (int j = n_entries; j >= 0; j--) {
                            int16_t ship_idx = p_refit->entry[j].ship_idx;
                            s_ship_data* s = &MOX::_ship[ship_idx];
                            if (s->location == refit_planet_idx &&
                                (s->d.size < 2 || COLCALC::Colony_Can_Build_Large_Ships_(temp_colony))) {

                                s_ship_design* refit_design;
                                if (s->d.size == 5) {
                                    if (ship_design.cost == 0) {
                                        INITSHIP::Load_Doom_Star_Ship_Design_(player_idx, &ship_design);
                                    }
                                    refit_design = &ship_design;
                                } else {
                                    refit_design = &p->ship_designs[s->d.size];
                                }

                                int16_t cost = AIBUILD::Refit_Cost_(player_idx, &s->d, refit_design);
                                if (cost <= refit_design->cost &&
                                    cost <= AIBUILD::Industry_Worth(temp_colony) * (war_level + 10)) {

                                    if (s->officer_index != -1) {
                                        OFFICER::Move_Officer_To_Pool_(s->officer_index);
                                        s->officer_index = -1;
                                    }

                                    memcpy(&s->d, refit_design, sizeof(s_ship_design));
                                    s->d.cost = cost;
                                    s->status = 4;
                                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, -100 - ship_idx);

                                    p_refit->n_entries--;
                                    memmove(&p_refit->entry[j], &p_refit->entry[j + 1], (p_refit->n_entries - j) * sizeof(s_ai_refit_entry));
                                    refitted = true;
                                    break;
                                }
                            }
                        }
                        if (!refitted) {
                            choice_weights[2] = 0;
                        }
                        break;
                    }
                    case 3:
                        if (!COLCALC::Colony_Is_Blockaded(temp_colony) && AIBUILD::Industry_Worth(temp_colony) >= 15) {
                            AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, -7);
                            AIBUILD::_player_n_spies[player_idx][1]--;
                        }
                        choice_weights[3] = 0;
                        break;
                    case 4:
                        if (!COLCALC::Colony_Is_Blockaded(temp_colony) && AIBUILD::Industry_Worth(temp_colony) >= 14) {
                            if (temp_colony->buildings[22] == 0) {
                                AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, 22);
                                AIBUILD::_player_n_transports[player_idx][1]--;
                            } else {
                                INITSHIP::Load_Transport_Ship_Design_(player_idx, &p->ship_designs[5]);
                                int16_t tr_idx = COLBLDG::Create_Ship_(player_idx, 5, (int16_t)current_colony_idx);
                                if (tr_idx == -1) {
                                    no_range_or_speed = true;
                                } else {
                                    MOX::_ship[tr_idx].status = 6;
                                    AIBUILD::Force_Produce_(colony_queue, (int16_t)i, (int32_t*)&colony_queue_count, -100 - tr_idx);
                                    AIBUILD::_player_n_transports[player_idx][1]--;
                                }
                            }
                        }
                        choice_weights[4] = 0;
                        break;
                }

                if (colony_queue_count != prev_count) {
                    break;
                }
            }
        }
    }

    int __cdecl Sort_Pops_F_IR_(const void* a, const void* b) {
        const uint32_t* pop1 = (const uint32_t*)a;
        const uint32_t* pop2 = (const uint32_t*)b;

        int prod1 = (int)COLCALC::Colony_Pop_Prod_Produced_(_Sort_Pops_colony_data, pop1, 0);
        int prod2 = (int)COLCALC::Colony_Pop_Prod_Produced_(_Sort_Pops_colony_data, pop2, 0);

        int score1;
        if ((*pop1 & 0x180) == 0x80 /* Farmer */) {
            score1 = prod1 - (int)COLCALC::Colony_Pop_Prod_Produced_(_Sort_Pops_colony_data, pop1, 1);
            if (_Sort_Pops_colony_info->pollution_at_start < _Sort_Pops_colony_data->pollution) {
                score1 += 1000;
            }
        } else {
            score1 = prod1 - (int)COLCALC::Colony_Pop_Prod_Produced_(_Sort_Pops_colony_data, pop1, 2);
        }

        int score2;
        if ((*pop2 & 0x180) == 0x80 /* Farmer */) {
            score2 = prod2 - (int)COLCALC::Colony_Pop_Prod_Produced_(_Sort_Pops_colony_data, pop2, 1);
            if (_Sort_Pops_colony_info->pollution_at_start < _Sort_Pops_colony_data->pollution) {
                score2 += 1000;
            }
        } else {
            score2 = prod2 - (int)COLCALC::Colony_Pop_Prod_Produced_(_Sort_Pops_colony_data, pop2, 2);
        }

        if (score2 != score1) {
            return score2 - score1;
        }

        if (prod1 != prod2) {
            return prod1 - prod2;
        }

        uint32_t val1 = *pop1;
        uint32_t val2 = *pop2;

        if (((val1 ^ val2) & 0x180) == 0) {
            return (int)(val1 & 0xF) - (int)(val2 & 0xF);
        }

        return (int)((val1 & 0x180) >> 7) - (int)((val2 & 0x180) >> 7);
    }

    void __cdecl Make_First_Unassigned_Into_Farmer_(s_aidudes_colony_info* info) {
        s_colony* colony = &MOX::_colony[(uint8_t)info->colony_idx];
        int16_t old_pollution = colony->pollution;

        colony->pop[(uint8_t)info->cur_pop_idx] &= 0xFFFFFE7F;
        colony->pop[(uint8_t)info->cur_pop_idx] |= 0x0200;

        info->cur_pop_idx++;
        info->n_farmers++;

        COLCALC::Pre_Import_Computing_(colony);

        if (colony->pollution < old_pollution) {
            aidudes::_Sort_Pops_colony_info = info;
            aidudes::_Sort_Pops_colony_data = colony;

            size_t n_to_sort = (size_t)(info->pop_end_idx - info->cur_pop_idx);
            qsort(&colony->pop[(uint8_t)info->cur_pop_idx], n_to_sort, sizeof(uint32_t), aidudes::Sort_Pops_F_IR_);
        }

        if (info->n_farmers < (uint8_t)colony->max_farms && info->cur_pop_idx < (uint8_t)colony->n_pops) {
            info->best_worker_to_farmer_score = aidudes::F_IR_Worthiness_(info, &colony->pop[(uint8_t)info->cur_pop_idx]);
        } else {
            info->best_worker_to_farmer_score = (int16_t)0x8001;
        }
    }

    void __cdecl Assign_Additional_Unblockaded_Farmers_() {
        s_aidudes_colony_info* candidates[MAX_COLONIES];
        int num_candidates = 0;
        bool freighter_shortage_flag = false;
        bool allocation_failed = false;

        if (aidudes::_n_our_unblockaded > 0) {
            s_aidudes_colony_info* info = aidudes::_our_unblockaded;
            s_aidudes_colony_info* end = aidudes::_our_unblockaded + aidudes::_n_our_unblockaded;

            while (info < end) {
                s_colony* col = &MOX::_colony[(uint8_t)info->colony_idx];
                info->cur_pop_idx = info->first_assignable_pop_idx;
                info->pop_end_idx = info->pop_tail_idx;

                if ((uint8_t)info->cur_pop_idx < (uint8_t)info->pop_tail_idx && 
                    (uint8_t)info->n_farmers < (uint8_t)col->max_farms && 
                    col->food2_per_farmer != 0) 
                {
                    aidudes::_Sort_Pops_colony_data = col;
                    aidudes::_Sort_Pops_colony_info = info;

                    qsort(&col->pop[(uint8_t)info->cur_pop_idx], 
                          (uint8_t)info->pop_end_idx - (uint8_t)info->cur_pop_idx, 
                          4, 
                          aidudes::Sort_Pops_F_IR_);

                    info->best_worker_to_farmer_score = aidudes::F_IR_Worthiness_(info, &col->pop[(uint8_t)info->cur_pop_idx]);
                    candidates[num_candidates++] = info;
                }
                info++;
            }

            COLCALC::Do_Player_Colony_Post_Production_(aidudes::_i_ai);

            while ((aidudes::_p_ai->surplus_food < 0 || aidudes::_p_ai->surplus_freighters < 0) && 
                   (num_candidates > 0 && !allocation_failed)) 
            {
                int best_candidate_idx;

                if (aidudes::_p_ai->surplus_freighters < 1) {
                    best_candidate_idx = -1;
                    freighter_shortage_flag = true;

                    for (int i = 0; i < num_candidates; ++i) {
                        s_aidudes_colony_info* candidate = candidates[i];
                        s_colony* col = &MOX::_colony[(uint8_t)candidate->colony_idx];

                        int food_needed = (uint8_t)col->food2_needed_for_our_empire + 
                                          (uint8_t)col->food2_needed_for_assimilated + 
                                          (uint8_t)col->food2_needed_for_conquered + 
                                          (uint8_t)col->food2_needed_for_natives;

                        if (food_needed > (col->production[0] * 2)) {
                            if (best_candidate_idx == -1 || candidate->best_worker_to_farmer_score > candidates[best_candidate_idx]->best_worker_to_farmer_score) {
                                best_candidate_idx = i;
                            }
                        }
                    }
                } else {
                    best_candidate_idx = 0;
                    for (int i = num_candidates - 1; i > 0; --i) {
                        if (candidates[i]->best_worker_to_farmer_score > candidates[best_candidate_idx]->best_worker_to_farmer_score) {
                            best_candidate_idx = i;
                        }
                    }
                }

                if (best_candidate_idx == -1) {
                    allocation_failed = true;
                } else {
                    s_aidudes_colony_info* target_info = candidates[best_candidate_idx];
                    aidudes::Make_First_Unassigned_Into_Farmer_(target_info);
                    COLCALC::Colony_Calculation_(target_info->colony_idx);

                    s_colony* target_col = &MOX::_colony[(uint8_t)target_info->colony_idx];
                    if ((uint8_t)target_info->cur_pop_idx >= (uint8_t)target_info->pop_end_idx || 
                        (uint8_t)target_info->n_farmers >= (uint8_t)target_col->max_farms) 
                    {
                        num_candidates--;
                        if (best_candidate_idx < num_candidates) {
                            memmove(&candidates[best_candidate_idx], &candidates[best_candidate_idx + 1], (num_candidates - best_candidate_idx) * sizeof(s_aidudes_colony_info*));
                        }
                    }
                }
            }
        }

        if (freighter_shortage_flag) {
            if ((int)random::Random_(10) <= (int)MOX::_settings.game_difficulty) {
                aidudes::_p_ai->n_freighters += 5;
            }
        }
    }

    int __cdecl F_IR_Worthiness_(s_aidudes_colony_info* info, uint32_t* flags) {
        s_colony* colony = &MOX::_colony[info->colony_idx];
        int16_t base_production = COLCALC::Colony_Pop_Prod_Produced_(colony, flags, 0);
        int worthiness;

        if ((*flags & 0x180) == 0x80) {
            int16_t mode1_production = COLCALC::Colony_Pop_Prod_Produced_(colony, flags, 1);
            worthiness = (int)base_production - (int)mode1_production;

            // If pollution has increased since the start of evaluation
            if (colony->pollution > info->pollution_at_start) {
                return worthiness + 1000;
            }
        } else {
            int16_t mode2_production = COLCALC::Colony_Pop_Prod_Produced_(colony, flags, 2);
            worthiness = (int)base_production - (int)mode2_production;
        }

        return worthiness;
    }

    int __cdecl colony_less_industry_cmp_(const void* p1, const void* p2) {
        int16_t worth1 = AIBUILD::Industry_Worth(&MOX::_colony[*((const uint8_t*)p1)]);
        int16_t worth2 = AIBUILD::Industry_Worth(&MOX::_colony[*((const uint8_t*)p2)]);

        return (int)worth1 - (int)worth2;
    }

    int __cdecl Sort_Pops_FR_I_(const void* a, const void* b) {
        const uint32_t* pop1 = (const uint32_t*)a;
        const uint32_t* pop2 = (const uint32_t*)b;

        if (aidudes::_Sort_Pops_colony_data->food2_per_farmer == 0) {
            return aidudes::_Sort_Pops_colony(a, b);
        }

        int16_t prod_0_p1 = COLCALC::Colony_Pop_Prod_Produced_(aidudes::_Sort_Pops_colony_data, pop1, 0);
        int16_t prod_1_p1 = COLCALC::Colony_Pop_Prod_Produced_(aidudes::_Sort_Pops_colony_data, pop1, 1);
        int16_t prod_2_p1 = COLCALC::Colony_Pop_Prod_Produced_(aidudes::_Sort_Pops_colony_data, pop1, 2);

        int16_t score_p1 = prod_2_p1 - (prod_1_p1 * 2) + prod_0_p1;

        int16_t prod_0_p2 = COLCALC::Colony_Pop_Prod_Produced_(aidudes::_Sort_Pops_colony_data, pop2, 0);
        int16_t prod_1_p2 = COLCALC::Colony_Pop_Prod_Produced_(aidudes::_Sort_Pops_colony_data, pop2, 1);
        int16_t prod_2_p2 = COLCALC::Colony_Pop_Prod_Produced_(aidudes::_Sort_Pops_colony_data, pop2, 2);

        int16_t score_p2 = prod_2_p2 - (prod_1_p2 * 2) + prod_0_p2;

        if (score_p2 != score_p1) {
            return (int32_t)score_p2 - (int32_t)score_p1;
        }

        if (prod_0_p2 != prod_0_p1) {
            return (int32_t)prod_0_p2 - (int32_t)prod_0_p1;
        }

        if (prod_1_p2 != prod_1_p1) {
            return (int32_t)prod_1_p1 - (int32_t)prod_1_p2;
        }

        return (int)(*pop1 & 0x0F) - (int)(*pop2 & 0x0F);
    }

    int __cdecl Sort_Pops_R_I_(const void* p1, const void* p2) {
        int16_t p1_prod1 = COLCALC::Colony_Pop_Prod_Produced_(aidudes::_Sort_Pops_colony_data, (const uint32_t*)p1, 1);
        int16_t p2_prod1 = COLCALC::Colony_Pop_Prod_Produced_(aidudes::_Sort_Pops_colony_data, (const uint32_t*)p2, 1);
        int16_t p1_prod2 = COLCALC::Colony_Pop_Prod_Produced_(aidudes::_Sort_Pops_colony_data, (const uint32_t*)p1, 2);
        int16_t p2_prod2 = COLCALC::Colony_Pop_Prod_Produced_(aidudes::_Sort_Pops_colony_data, (const uint32_t*)p2, 2);

        int16_t p2_delta = p2_prod2 - p2_prod1;
        int16_t p1_delta = p1_prod2 - p1_prod1;

        if (p2_delta != p1_delta) {
            return (int)p2_delta - (int)p1_delta;
        }

        if (p2_prod1 == p1_prod1) {
            return (int)(*(const uint32_t*)p1 & 0xF) - (int)(*(const uint32_t*)p2 & 0xF);
        }

        return (int)p1_prod1 - (int)p2_prod1;
    }
}