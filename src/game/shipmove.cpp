#include "pch.h"

namespace SHIPMOVE {
    void __cdecl Allocate_New_Ship_Slots_() {
        int antaran_max_slots = 0;
        for (int i = 0; i < 9; ++i) {
            antaran_max_slots += MOX::_antarans.offensive_ships[i];
        }

        if (antaran_max_slots > 20) {
            antaran_max_slots = 20;
        } else {
            antaran_max_slots += 2;
        }

        int colony_production_slots[8] = {0};
        for (int i = 0; i < (int)MOX::_NUM_COLONIES; ++i) {
            s_colony* col = &MOX::_colony[i];
            if (col->owner != -1 && col->outpost_flag == 0) {
                for (int j = 0; j < 7; ++j) {
                    if (col->producing[j] > -100) {
                        colony_production_slots[(uint8_t)col->owner]++;
                    }
                }
            }
        }

        int player_max_slots = colony_production_slots[MOX::_PLAYER_NUM];
        if (MOX::_game_type == 0 && player_max_slots > 20) {
            player_max_slots = 20;
        }

        int unassigned_indices[500];
        int unassigned_count = 0;
        int antaran_assigned = 0;
        int player_assigned = 0;

        for (int i = 0; i < 500; ++i) {
            if (i >= (int)MOX::_NUM_SHIPS || MOX::_ship[i].status == 5) {
                if (antaran_assigned < antaran_max_slots) {
                    MOX::_ship[i].owner = 8;
                    antaran_assigned++;
                } else if (player_assigned < player_max_slots) {
                    MOX::_ship[i].owner = (char)MOX::_PLAYER_NUM;
                    player_assigned++;
                } else {
                    unassigned_indices[unassigned_count++] = i;
                }
            }
        }

        int ships_assigned_per_player[8] = {0};
        ships_assigned_per_player[MOX::_PLAYER_NUM] = player_assigned;

        int current_player = (int)MOX::_stardate % (int)MOX::_NUM_PLAYERS;
        int assigned_from_unassigned = 0;
        int players_tried = 0;

        if (unassigned_count > 0) {
            while (assigned_from_unassigned < unassigned_count) {
                if (players_tried >= (int)MOX::_NUM_PLAYERS) break;

                if (ships_assigned_per_player[current_player] < colony_production_slots[current_player]) {
                    int ship_idx = unassigned_indices[assigned_from_unassigned++];
                    MOX::_ship[ship_idx].owner = (char)current_player;
                    ships_assigned_per_player[current_player]++;
                    players_tried = 0;
                } else {
                    players_tried++;
                }

                current_player++;
                if (current_player == (int)MOX::_NUM_PLAYERS) {
                    current_player = 0;
                }
            }
        }

        while (assigned_from_unassigned < unassigned_count) {
            if (MOX::_player[current_player].eliminated == 0) {
                int ship_idx = unassigned_indices[assigned_from_unassigned++];
                MOX::_ship[ship_idx].owner = (char)current_player;
                ships_assigned_per_player[current_player]++;
            }

            current_player++;
            if (current_player == (int)MOX::_NUM_PLAYERS) {
                current_player = 0;
            }
        }
    }

    /**
     * Iterates through all players to refresh their ship ranges,
     * best warp drive technology, and current FTL speed.
     */
    void __cdecl Update_All_Ship_Ranges_() {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            s_player* current_player = &MOX::_player[i];

            Update_Player_Ship_Range_(current_player);

            current_player->best_ftl_drive = (char)INITSHIP::Best_Warp_Drive_(i);
            current_player->ship_speed = (char)INITSHIP::Calc_Player_FTL_Speed_(i);
        }
    }

    /**
     * Updates the movement range of the player's ships based on their researched fuel tech tiers.
     * Iterates backwards through fuel tiers 5 down to 1, checking if the required tech is level 3.
     */
    void __cdecl Update_Player_Ship_Range_(s_player* player) {
        for (int i = 5; i >= 1; --i) {
            int16_t tech_idx = TECHDATA::_fuel[i].tech_app;

            // Check if the tech application for this fuel tier is fully researched (level 3)
            if (player->tech_applications[tech_idx] == 3) {
                player->ship_range = TECHDATA::_fuel[i].ship_range;
                return;
            }
        }

        // If no fuel upgrades (tiers 1-5) are researched to level 3, range is set to 0.
        player->ship_range = 0;
    }

    /**
     * Checks if any star colonized by a specific player is within a given range of the target star.
     * Considers black hole blockages.
     * @param star The target star to check from.
     * @param player_idx Index of the player to check colonies for.
     * @param dist The maximum range (in parsecs, where 1 parsec is roughly 30 pixels).
     */
    bool __cdecl Star_In_Range_Of_Player_Aux1_(s_star_data* star, int16_t player_idx, int16_t dist) {
        if (MOX::_NUM_STARS == 0) {
            return false;
        }

        // Calculate the index of the input star in the star array
        int16_t star_idx = static_cast<int16_t>(star - MOX::_star);
        // dist is scaled by 30 pixels per unit (30^2 = 900)
        int32_t dist_sq_limit = static_cast<int32_t>(dist) * dist * 900;

        for (int i = 0; i < static_cast<int>(MOX::_NUM_STARS); ++i) {
            s_star_data* current_star = &MOX::_star[i];

            // Check if player_idx bit is set in has_colony bitmask
            if ((current_star->has_colony >> (player_idx & 0x1F)) & 1) {
                // Check if current_star can 'see' the target star through black hole blockages
                if (!struct_::Test_Bit_Field_(current_star->black_hole_blocks, star_idx)) {
                    int32_t dx = static_cast<int32_t>(star->x) - current_star->x;
                    int32_t dy = static_cast<int32_t>(star->y) - current_star->y;

                    if ((dx * dx + dy * dy) <= dist_sq_limit) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    /**
     * Checks if a star is in range of a player, considering colonies, direct range, and wormholes.
     * @param star The target star to check.
     * @param player_idx Index of the player.
     * @param dist Maximum range in parsecs.
     * @return True if the star is in range.
     */
    bool __cdecl Star_In_Range_Of_Player_Aux2_(s_star_data* star, int16_t player_idx, int16_t dist) {
        // Check if player has a colony in this star system
        if ((star->has_colony >> (static_cast<uint8_t>(player_idx) & 0x1F)) & 1) {
            return true;
        }

        // Check range normally
        if (Star_In_Range_Of_Player_Aux1_(star, player_idx, dist)) {
            return true;
        }

        // Check range via wormhole if the destination star is visited
        if (star->wormhole_star_id != -1) {
            s_star_data* wormhole_star = &MOX::_star[star->wormhole_star_id];

            if ((wormhole_star->visited >> (static_cast<uint8_t>(player_idx) & 0x1F)) & 1) {
                if (Star_In_Range_Of_Player_Aux1_(wormhole_star, player_idx, dist)) {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * Checks if a star is within the normal ship range of a player.
     * @param star The target star.
     * @param player_idx Index of the player.
     * @return True if in range.
     */
    bool __cdecl Star_In_Normal_Range_Of_Player_(s_star_data* star, int16_t player_idx) {
        int16_t range = MOX::_player[player_idx].ship_range;
        return Star_In_Range_Of_Player_(star, player_idx, range);
    }

    /**
     * Checks if a star is within the extended range of a player, considering alliances.
     * @param star The target star.
     * @param player_idx Index of the player.
     * @param dist Maximum range in parsecs.
     * @return True if in range for the player or any ally.
     */
    bool __cdecl Star_In_Range_Of_Player_(s_star_data* star, int16_t player_idx, int16_t dist) {
        if (Star_In_Range_Of_Player_Aux2_(star, player_idx, dist)) {
            return true;
        }

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            // Check if player is an ally (treaty type 2) and the star is in their range
            if (player_idx != i && MOX::_player[player_idx].treaty[i] == 2) {
                if (Star_In_Range_Of_Player_Aux2_(star, (int16_t)i, dist)) {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * Determines if a ship can receive orders, based on its status and player's communications technology.
     * @param ship The ship to check.
     * @return True if the ship can be ordered.
     */
    /**
     * @address 00436b40
     */
    bool __cdecl Ship_Is_Moving_(s_ship_data* ship) {
        if (ship->status >= 1 && ship->status <= 2) {
            return true;
        }
        return false;
    }

    /**
     * Determines if a ship can receive orders, based on its status and player's communications technology.
     * @param ship The ship to check.
     * @return True if the ship can be ordered.
     */
    bool __cdecl Can_Order_Ship_(s_ship_data* ship) {
        if (ship->owner < 0 || ship->owner >= (int32_t)MOX::_NUM_PLAYERS) {
            return false;
        }

        if (ship->status == 0 || ship->status == 2) {
            return true;
        }

        if (ship->status != 1) {
            return false;
        }

        s_player* player = &MOX::_player[(uint8_t)ship->owner];

        // Check for Hyperspace Communications
        if (player->tech_applications[TECH_APP_HYPERSPACE_COMMUNICATIONS] == 3) {
            return true;
        }

        // Check if player has simpler comms tech and is near a relay
        if (player->tech_applications[TECH_APP_SUB_SPACE_COMMUNICATIONS] == 3 ||
            player->tech_applications[TECH_APP_TACHYON_COMMUNICATIONS] == 3) {
            
            uint8_t player_mask = 1 << (ship->owner & 0x1F);
            
            for (int i = 0; i < (int)MOX::_NUM_STARS; ++i) {
                s_star_data* star = &MOX::_star[i];
                int dist = GEO::Parsecs_Between_Points_(star->x, star->y, ship->x, ship->y);

                if (dist <= 5) {
                    if (star->has_subspace & player_mask) {
                        return true;
                    }
                    if (dist <= 2) {
                        if (star->has_tachyon & player_mask) {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    /**
     * @address 00435fc0
     */
    int16_t __cdecl Ship_Range_(s_ship_data* ship) {
        if (ship->owner >= 8) {
            return 0xFF;
        }

        int16_t range = MOX::_player[(uint8_t)ship->owner].ship_range;

        if (struct_::Test_Bit_Field_(ship->d.special_device_flags, 11)) {
            range = (range * 3 + 1) / 2;
        }

        return range;
    }

    /**
     * @address 00437260
     */
    int16_t __cdecl Ship_Destination_Star_(s_ship_data* ship) {
        int16_t location = ship->location;

        if (ship->status == 1) {
            return location - 500;
        }

        if (ship->status == 2) {
            return location - 1000;
        }

        return location;
    }

    /**
     * @address 00436e30
     */
    int32_t __cdecl Create_Nonplayer_Ship_(int16_t owner, s_ship_design* design, int16_t x, int16_t y) {
        int16_t i;

        // Search for an available ship slot
        for (i = 0; i < 500; ++i) {
            if (MOX::_ship[i].owner == 8) {
                // Slot is free if status is 5 or it's beyond the current total count
                if (MOX::_ship[i].status == 5 || i >= (int16_t)MOX::_NUM_SHIPS) {
                    break;
                }
            }
        }

        if (i == 500) {
            return -1;
        }

        if (i >= (int16_t)MOX::_NUM_SHIPS) {
            MOX::_NUM_SHIPS = i + 1;
        }

        // Initialize ship slot
        memset(&MOX::_ship[i], 0, sizeof(s_ship_data));

        if (design != nullptr) {
            memcpy(&MOX::_ship[i].d, design, sizeof(s_ship_design));
        }

        MOX::_ship[i].owner = (char)owner;
        MOX::_ship[i].d.previous_owner = (char)owner;
        MOX::_ship[i].status = 0;

        // Set orbital position and coordinates
        MOX::_ship[i].location = HAROLD::XY_To_Star_Id_(x, y);
        MOX::_ship[i].x = x;
        MOX::_ship[i].y = y;

        MOX::_ship[i].officer_index = -1;
        MOX::_ship[i].d.date_of_design = (uint16_t)MOX::_stardate;

        return (int32_t)i;
    }

    /**
     * @address 00436b60
     */
    void __cdecl Move_All_Ships_Toward_Stars_() {
        int8_t local_8;
        int8_t local_7;

        if (MOX::_game_type >= 2 && MOX::_game_type <= 3) {
            for (int i = 0; i < (int)MOX::_NUM_STARS; ++i) {
                MOX::_star[i].just_visited = 0;
            }
        }

        for (int i = 0; i < (int)MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];

            if (Ship_Is_Moving_(ship)) {
                if (ship->location >= 1000) {
                    ship->location -= 500;
                }

                int16_t star_idx = ship->location - 500;
                int16_t star_x = MOX::_star[star_idx].x;
                int16_t star_y = MOX::_star[star_idx].y;

                if (ship->travelling_speed == (int8_t)0xFF) {
                    ship->x = star_x;
                    ship->y = star_y;
                } else {
                    GEO::Move_Player_1_Turn_To_Star_(
                        ship->owner,
                        ship->x,
                        ship->y,
                        star_idx,
                        (uint16_t)(uint8_t)ship->travelling_speed,
                        &ship->x,
                        &ship->y,
                        (ship->group_has_navigator == 0) ? &local_8 : nullptr,
                        &local_7
                    );
                }

                ship->turns_left--;

                if (ship->x == star_x && ship->y == star_y) {
                    Make_Ship_Arrive_At_Star_(ship, star_idx);
                } else {
                    ship->status = 1;
                }
            } else {
                ship->mission = 0;
            }
        }

        MAINSCR::Update_Ship_Icons_In_Between_Turns_();
    }

    /**
     * @address 00436a50
     */
    void __cdecl Make_Ships_Move_To_(int16_t* ship_list, int16_t destination_id, s_ship_move_info* move_info) {
        int8_t status;
        bool colony_flag_needed = false;
        int i = 0;

        if (move_info->turns_left == 0) {
            status = 0;
        } else {
            int16_t first_ship_idx = ship_list[0];
            int16_t first_loc = MOX::_ship[first_ship_idx].location;

            if (first_loc < 72 || first_loc >= 1000) {
                status = 2;
                destination_id += 1000;
            } else {
                status = 1;
                destination_id += 500;
            }
        }

        int16_t ship_idx;
        while ((ship_idx = ship_list[i]) != -1) {
            s_ship_data* ship = &MOX::_ship[ship_idx];

            if (!colony_flag_needed) {
                if (ship->d.ship_type == 1 || ship->d.ship_type == 4) {
                    colony_flag_needed = true;
                }
            }

            ship->location = destination_id;
            ship->turns_left = move_info->turns_left;
            ship->group_has_navigator = move_info->has_navigator;
            ship->travelling_speed = move_info->traveling_speed;
            ship->status = status;

            i++;
        }

        if (colony_flag_needed) {
            SHIPS::Clear_All_Ships_Auto_Colonization_Flag_(ship_list);
        }
    }

    /**
     * @address 00437750
     */
    void __cdecl Unmark_Just_Built_AI_Ships_() {
        int32_t num_ships = static_cast<int32_t>(MOX::_NUM_SHIPS);
        s_ship_data* ship_array = MOX::_ship;

        for (int32_t i = 0; i < num_ships; ++i) {
            s_ship_data& ship = ship_array[i];

            // AI players (owner 0-7) who aren't the primary human player (objectives != 100)
            // have their just_built flag cleared.
            if (ship.owner < 8 && MOX::_player[static_cast<int32_t>(ship.owner)].objectives != 100) {
                ship.just_built = 0;
            }
        }
    }

    /**
     * @address 00436cd0
     */
    void __cdecl Make_Ship_Arrive_At_Star_(s_ship_data* ship, int16_t star_idx) {
        ship->location = (short)star_idx;
        ship->status = 0;
        ship->x = MOX::_star[star_idx].x;
        ship->y = MOX::_star[star_idx].y;
        ship->mission = 0;

        MOX::_star[star_idx].ignore_combat_ships = 0;

        int8_t owner = ship->owner;

        if (owner < 8) {
            // Check for Orion discovery
            if (MOX::_star[star_idx].system_special == 11 /* Orion */ && MOX::_star[star_idx].visited == 0) {
                EVENTS::Event_Orion_Discovered_((int16_t)owner);
            }

            // Update colony ship arrival flags
            if (ship->d.ship_type == 1 /* Colony Ship */ || ship->d.ship_type == 4 /* Colony Base */) {
                MOX::_star[ship->location].ignore_colony_ships &= ~(1 << owner);
            }

            // Mark system as visited by player
            if (!((MOX::_star[ship->location].visited >> owner) & 1)) {
                MOX::_star[ship->location].visited |= (1 << owner);
                MOX::_star[ship->location].just_visited |= (1 << owner);
            }
        } else if (owner == 13 /* Monster/Antaran */) {
            ship->d.date_of_design = 0;
        }

        if (owner < 8) {
            reinterpret_cast<int8_t*>(REPORT::_something_interesting_happened)[owner] = 1;

            // Check if player's objective is to defend Orion
            if (MOX::_player[owner].objectives == 100 /* 'd' */) {
                MOX::_star[star_idx].not_used3 = 0;
            }
        }
    }

    /**
     * @address 00437290
     */
    void __cdecl Update_Ship_ETAs_(int32_t player_id) {
        int num_ships = (int)MOX::_NUM_SHIPS;
        s_ship_data* ships = MOX::_ship;

        for (int i = 0; i < num_ships; ++i) {
            s_ship_data* ship = &ships[i];

            // Only process ships that are active/moving and do not belong to player_id
            if ((ship->status == 1 || ship->status == 2) && (int16_t)ship->owner != (int16_t)player_id) {
                int16_t dest_star_idx = SHIPMOVE::Ship_Destination_Star_(ship);
                s_star_data* star = &MOX::_star[dest_star_idx];

                int32_t target_x = (int32_t)star->x;
                int32_t target_y = (int32_t)star->y;

                int16_t current_x = ship->x;
                int16_t current_y = ship->y;
                uint8_t turn_count = 0;

                int8_t local_a;
                int8_t local_9;

                // Simulate movement turn by turn until destination is reached
                while ((int32_t)current_x != target_x || (int32_t)current_y != target_y) {
                    turn_count++;

                    int8_t* navigator_ptr = nullptr;
                    if (ship->group_has_navigator != 1) {
                        navigator_ptr = &local_a;
                    }

                    GEO::Move_Player_1_Turn_To_Star_(
                        (uint8_t)ship->owner,
                        current_x,
                        current_y,
                        dest_star_idx,
                        (int16_t)(uint8_t)ship->travelling_speed,
                        &current_x,
                        &current_y,
                        navigator_ptr,
                        &local_9
                    );
                }

                ship->turns_left = (char)turn_count;
            }
        }
    }
}