#include "pch.h"

namespace maintain {
    // Suggested name: Player_Maintenance_
    void __cdecl Player_Maintenance_(s_player* param_1) {
        if (param_1->do_not_research) {
            Kill_Research_Buildings_(param_1);
        }

        if ((int)param_1->surplus_bc + param_1->bc >= 0) {
            return;
        }

        int worths[500];
        memset(worths, 0, sizeof(worths));
        _ship_worths = &worths;

        uint8_t eligible_players[8];
        memset(eligible_players, 0, sizeof(eligible_players));

        int player_idx = (int)(param_1 - MOX::_player);
        int eligible_count = 0;

        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            if (i != player_idx && param_1->contact[i]) {
                eligible_players[i] = 1;
                eligible_count++;
            }
        }

        if (eligible_count == 0) {
            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                if (i != player_idx && !MOX::_player[i].eliminated && MOX::_player[i].objectives == 100 /* 0x64 */) {
                    eligible_players[i] = 1;
                    eligible_count++;
                }
            }
        }

        if (eligible_count == 0) {
            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                if (i != player_idx && !MOX::_player[i].eliminated) {
                    eligible_players[i] = 1;
                    eligible_count++;
                }
            }
        }

        if (eligible_count > 0 && MOX::_NUM_SHIPS > 0) {
            for (int j = 0; j < (int)MOX::_NUM_SHIPS; ++j) {
                s_ship_data& ship = MOX::_ship[j];
                if (ship.owner == player_idx && ship.status < 5 && ship.d.ship_type == 0) {
                    for (int k = 0; k < (int)MOX::_NUM_PLAYERS; ++k) {
                        if (eligible_players[k]) {
                            worths[j] += aipower::Ship_Strength_Vs_Ships_(j, (int16_t)k) & 0xFFFF;
                        }
                    }
                    worths[j] /= (ship.d.size + 1) * eligible_count;
                }
            }
        }

        if (((int)param_1->command_points * 4) / 3 < (int)param_1->command_points_used) {
            Kill_Ships_(param_1, 1, 1, 4);
            Kill_Ships_(param_1, 1, 0, 4);
            Kill_Ships_(param_1, 1, 1, 2);
            Kill_Ships_(param_1, 1, 1, 0);
            Kill_Ships_(param_1, 0, 1, 4);
            Kill_Ships_(param_1, 0, 0, 4);
            Kill_Ships_(param_1, 0, 1, 2);
            Kill_Ships_(param_1, 0, 1, 0);
        }

        Kill_Buildings_(param_1, Phase1_Building_);
        Kill_Ships_(param_1, 1, 1, 4);
        Kill_Ships_(param_1, 1, 0, 4);
        Kill_Ships_(param_1, 0, 1, 4);
        Kill_Ships_(param_1, 0, 0, 4);
        Kill_Ships_(param_1, 1, 1, 2);
        Kill_Ships_(param_1, 1, 1, 0);
        Kill_Ships_(param_1, 0, 1, 2);
        Kill_Ships_(param_1, 0, 1, 0);
        Kill_Buildings_(param_1, Phase2_Building_);
        Kill_Ships_(param_1, 1, 0, 2);
        Kill_Ships_(param_1, 0, 0, 2);
        Kill_Ships_(param_1, 1, 1, 1);
        Kill_Ships_(param_1, 0, 1, 1);
        Kill_Ships_(param_1, 1, 0, 1);
        Kill_Ships_(param_1, 0, 0, 1);
        Kill_Spy_(param_1);
        Kill_Ships_(param_1, 1, 0, 0);
        Kill_Ships_(param_1, 0, 0, 0);
        Kill_Leaders_(param_1);
        Kill_Buildings_(param_1, All_But_Capitol_);

        _ship_worths = nullptr;
    }

    /**
     * @brief Checks if a building has a maintenance cost greater than zero.
     * @param p Pointer to the player.
     * @param a Colony index.
     * @param b Building index.
     * @return True if maintenance > 0, false otherwise.
     */
    bool __cdecl Phase2_Building_(s_player* p, int16_t a, int16_t b) {
        // TYPE MISMATCH: decompiler showed bool __cdecl Phase2_Building_(short building_idx)
        return TECHDATA::_buildings[b].maintenance > 0;
    }

    bool __cdecl All_But_Capitol_(s_player* p, int16_t a, int16_t b) {
        // TYPE MISMATCH: decompiler showed bool __cdecl All_But_Capitol_(uint16_t building_id)
        return b != BUILDING_CAPITOL;
    }

    /**
     * @brief Removes research-specific buildings from all colonies of a specific player and provides BC compensation.
     *
     * This function iterates through all colonies belonging to the provided player. It checks for specific
     * research buildings (IDs 6, 30, 19, 35) and removes them if present. The player is compensated with
     * half of the building's cost, and a difficulty-based bonus is applied unless they have reached objective 100.
     * If the colony was currently producing the removed building, its production is reset.
     */
    void __cdecl Kill_Research_Buildings_(s_player* player) {
        int player_idx = (int)(player - MOX::_player);
        bool any_removed = false;

        for (int i = 0; i < (int)MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];

            if (colony->owner == player_idx) {
                for (int j = 0; j < 4; ++j) {
                    uint8_t building_id = maintain::_research_building_table[j];

                    if (colony->buildings[building_id] != 0) {
                        int cost = TECHDATA::_buildings[building_id].cost;

                        ERICNET::Remove_Building_((int16_t)i, (int16_t)building_id);
                        any_removed = true;

                        int compensation = cost / 2;
                        player->bc += compensation;

                        if (player->objectives != 100) {
                            player->bc += MOX::_settings.game_difficulty;
                        }
                    }

                    if (colony->producing[0] == building_id) {
                        colony->producing[0] = -2; /* Set to default housing/trade */
                    }
                }
            }
        }

        if (any_removed) {
            COLCALC::Do_Player_Colony_Calculations_((int16_t)player_idx);
        }
    }

    /**
     * @brief Checks if the ship is currently located at a star system where its owner has a colony.
     *
     * @param ship Pointer to the ship data.
     * @return true if the ship is at a colony belonging to its owner, false otherwise.
     */
    bool __cdecl Ship_Is_At_Colony_(s_ship_data* ship) {
        int16_t star_id;
        int8_t status = ship->status;
        int8_t owner = ship->owner;

        if (status == 0 || status == 4) {
            star_id = ship->location;
        } else if (status == 2) {
            int16_t x = ship->x;
            int16_t y = ship->y;
            star_id = HAROLD::XY_To_Star_Id_(x, y);
        } else {
            return false;
        }

        if (((MOX::_star[star_id].has_colony >> owner) & 1) != 0) {
            return true;
        }

        return false;
    }

    // Suggested name: MAINTAIN::Make_Scrap_Ships_Dead_
    void __cdecl Make_Scrap_Ships_Dead_() {
        int num_ships = (int)MOX::_NUM_SHIPS;
        s_ship_data* ships = MOX::_ship;

        for (int i = 0; i < num_ships; ++i) {
            if (ships[i].status == 7 || ships[i].status == 8) {
                SHIPS::Kill_Ship_(static_cast<int16_t>(i));
            }
        }
    }

    /**
     * Iteratively scraps the cheapest ships matching criteria until player maintenance deficit is resolved.
     */
    void __cdecl Kill_Ships_(s_player* player, bool at_colony_only, bool restricted_mission, int16_t ship_type) {
        int16_t player_idx = (int16_t)(player - MOX::_player);
        bool no_more_ships_found = false;

        while ((player->bc + player->surplus_bc < 0) && !no_more_ships_found) {
            s_ship_data* cheapest_ship = nullptr;
            int16_t min_worth = 0x7FFF;

            for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
                s_ship_data* ship = &MOX::_ship[i];

                if (ship->owner == player_idx &&
                    (int16_t)ship->d.ship_type == ship_type &&
                    ship->status < 3 /* Active */)
                {
                    if (at_colony_only && !Ship_Is_At_Colony_(ship)) {
                        continue;
                    }

                    if (restricted_mission) {
                        // Restricted missions: None (0), Orbiting (3), or Scrapped? (7)
                        if (!(ship->mission == 0 || ship->mission == 3 || ship->mission == 7)) {
                            continue;
                        }
                    }

                    int16_t worth = (int16_t)(*maintain::_ship_worths)[i];
                    if (worth < min_worth) {
                        min_worth = worth;
                        cheapest_ship = ship;
                    }
                }
            }

            if (cheapest_ship == nullptr) {
                no_more_ships_found = true;
            } else {
                int scrap_cost = maintain::Ship_Scrap_Value_(cheapest_ship);
                player->bc += scrap_cost;

                if (player->objectives != 100) {
                    player->bc += MOX::_settings.game_difficulty;
                }

                COLCALC::Do_Player_Colony_Post_Production_(player_idx);

                // TODO: Fix me
                uint16_t ship_index = (uint16_t)(cheapest_ship - MOX::_ship);

                s_MSG msg;
                msg.msg_type = MSG_TYPE_SCRAP_SHIP;
                msg.u.scrap_ship.where = cheapest_ship->location;
                msg.u.scrap_ship.ship_idx = ship_index;
                msg.u.scrap_ship.bc = scrap_cost;

                MSG_::Add_Msg_(msg, player_idx);

                cheapest_ship->status = 7; /* Scrapped */

                if (cheapest_ship->officer_index != -1) {
                    OFFICER::Move_Officer_To_Pool_(cheapest_ship->officer_index);
                    cheapest_ship->officer_index = -1;
                }

                MAINSCR::Update_Ship_Icons_In_Between_Turns_();
            }
        }
    }

    /**
     * Reduces the number of spies for a player if they have a negative BC balance (deficit).
     * The spies are removed until the deficit is cleared or all spies are gone.
     */
    void __cdecl Kill_Spy_(s_player* player) {
        int deficit = (int)player->bc + (int)player->surplus_bc;

        if (deficit < 0) {
            deficit = -deficit;
            int16_t player_idx = (int16_t)(player - MOX::_player);

            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                if (deficit == 0) {
                    break;
                }

                if (MOX::_player[i].eliminated == 0) {
                    int16_t num_spies = BILL::Get_Their_Spy_Number_(player_idx, (int16_t)i);

                    if (num_spies != 0) {
                        if (deficit < (int)num_spies) {
                            int16_t new_spy_count = num_spies - (int16_t)deficit;
                            BILL::Set_Their_Spy_Number_(player_idx, (int16_t)i, (uint8_t)new_spy_count);
                            deficit = 0;
                        } else {
                            deficit -= (int)num_spies;
                            BILL::Set_Their_Spy_Number_(player_idx, (int16_t)i, 0);
                        }
                    }
                }
            }

            COLCALC::Do_Player_Colony_Post_Production_(player_idx);
        }
    }

    void __cdecl Kill_Leaders_(s_player* p) {
        int16_t player_idx = (int16_t)(p - MOX::_player);
        bool no_more_candidates = false;

        while ((p->bc + (int)p->surplus_bc) < 0) {
            if (no_more_candidates) {
                break;
            }

            no_more_candidates = true;
            for (int16_t leader_idx = 0; leader_idx < MAX_LEADERS; ++leader_idx) {
                if (MOX::_leaders[leader_idx].player_index == (int8_t)player_idx) {
                    if (OFFICER::Officer_Maintenance_(player_idx, leader_idx) > 0) {
                        OFFICER::Deassign_Officer_(leader_idx);
                        COLCALC::Do_Player_Colony_Calculations_(player_idx);

                        s_MSG msg;
                        msg.msg_type = MSG_TYPE_SCRAP_LEADER;
                        msg.u.scrap_leader.leader_idx = (int8_t)leader_idx;

                        MSG_::Add_Msg_(msg, player_idx);

                        no_more_candidates = false;
                        break;
                    }
                }
            }
        }
    }
}