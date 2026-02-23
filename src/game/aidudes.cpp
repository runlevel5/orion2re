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

                if (colony->pollution >= 5 && current_info->field12_0xc == 0) {
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
}