#include "pch.h"
#include <cstring>

namespace SHIPMOVE {
    uint8_t _jump_gate_speed_bonus = 3;
    bool _jump_gate_pass_nebula = false;

    namespace {
        constexpr uint8_t JUMP_GATE_ROUTE_MASK = 1;

        bool Ship_Uses_Jump_Gate_(const s_ship_data* ship) {
            return (ship->special_device_damage_flags[0] & JUMP_GATE_ROUTE_MASK) != 0;
        }

        void Set_Ship_Uses_Jump_Gate_(s_ship_data* ship, bool uses_jump_gate) {
            if (uses_jump_gate) {
                ship->special_device_damage_flags[0] |= JUMP_GATE_ROUTE_MASK;
            } else {
                ship->special_device_damage_flags[0] &= (uint8_t)~JUMP_GATE_ROUTE_MASK;
            }
        }

        bool Ship_Is_In_Same_Moving_Fleet_(const s_ship_data* first, const s_ship_data* ship) {
            return SHIPMOVE::Ship_Is_Moving_(ship) &&
                   ship->owner == first->owner &&
                   ship->status == first->status &&
                   ship->location == first->location &&
                   ship->x == first->x &&
                   ship->y == first->y;
        }

        int Build_Moving_Fleet_List_(int16_t ship_idx, int16_t* ship_list) {
            if (ship_idx < 0 || ship_idx >= MOX::_NUM_SHIPS ||
                !SHIPMOVE::Ship_Is_Moving_(&MOX::_ship[ship_idx])) {
                ship_list[0] = -1;
                return 0;
            }

            s_ship_data* first = &MOX::_ship[ship_idx];
            int count = 0;
            for (int16_t i = 0; i < MOX::_NUM_SHIPS && count < MAX_SHIPS - 1; ++i) {
                if (Ship_Is_In_Same_Moving_Fleet_(first, &MOX::_ship[i])) {
                    ship_list[count++] = i;
                }
            }
            ship_list[count] = -1;
            return count;
        }

        bool Ship_Is_In_List_(int16_t ship_idx, const int16_t* ship_list) {
            for (int i = 0; ship_list[i] != -1; ++i) {
                if (ship_list[i] == ship_idx) {
                    return true;
                }
            }
            return false;
        }

        bool Fleet_Uses_Jump_Gate_(const int16_t* ship_list, int16_t target_star_idx) {
            for (int i = 0; ship_list[i] != -1; ++i) {
                s_ship_data* ship = &MOX::_ship[ship_list[i]];
                if (ship->status != 1 ||
                    SHIPMOVE::Ship_Destination_Star_(ship) != target_star_idx ||
                    !Ship_Uses_Jump_Gate_(ship)) {
                    return false;
                }
            }
            return ship_list[0] != -1;
        }

        bool Confirm_Fleet_Change_(int16_t baseline_ship_idx, const s_ship_move_info* move_info) {
            if (baseline_ship_idx < 0 || baseline_ship_idx >= MOX::_NUM_SHIPS ||
                !SHIPMOVE::Ship_Is_Moving_(&MOX::_ship[baseline_ship_idx])) {
                return true;
            }

            s_ship_data* ship = &MOX::_ship[baseline_ship_idx];
            if (move_info->moving == 0) {
                return GENDRAW::Confirmation_Box_(
                    "The fleet on course to pass the black hole will have no navigator. "
                    "If this is not remedied the fleet will be lost at the end of the turn. Continue?") != 0;
            }

            int delay = (int)(uint8_t)move_info->turns_left - (int)(uint8_t)ship->turns_left;
            if (delay <= 0) {
                return true;
            }

            char message[200];
            snprintf(
                message,
                sizeof(message),
                "The fleet without navigator will be delayed by %d turn%s. Continue?",
                delay,
                delay == 1 ? "" : "s");
            return GENDRAW::Confirmation_Box_(message) != 0;
        }

        void Apply_Movement_Result_(int16_t* ship_list, int16_t destination_star_idx, s_ship_move_info* move_info) {
            if (ship_list[0] == -1) {
                return;
            }

            if (move_info->moving != 0) {
                SHIPMOVE::Make_Ships_Move_To_(ship_list, destination_star_idx, move_info);
                return;
            }

            for (int i = 0; ship_list[i] != -1; ++i) {
                MOX::_ship[ship_list[i]].status = 8;
            }
        }
    }

    /**
     * Reserves available ship slots for Antarans and players based on current production demand.
     */
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

        int colony_production_slots[MAX_PLAYERS] = {0};
        for (int16_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* col = &MOX::_colony[i];
            if (col->owner != -1 && col->outpost_flag == 0) {
                for (int j = 0; j < 7; ++j) {
                    if (!COLBLDG::Colony_Production_Is_Queued_Ship_(col->producing[j])) {
                        int owner = col->owner;
                        colony_production_slots[owner]++;
                    }
                }
            }
        }

        int player_max_slots = colony_production_slots[MOX::_PLAYER_NUM];
        if (MOX::_game_type == 0 && player_max_slots > 20) {
            player_max_slots = 20;
        }

        int16_t unassigned_indices[MAX_SHIPS];
        int unassigned_count = 0;
        int antaran_assigned = 0;
        int player_assigned = 0;

        for (int16_t i = 0; i < MAX_SHIPS; ++i) {
            if (i >= MOX::_NUM_SHIPS || MOX::_ship[i].status == 5) {
                if (antaran_assigned < antaran_max_slots) {
                    MOX::_ship[i].owner = (int8_t)MAX_PLAYERS;
                    antaran_assigned++;
                } else if (player_assigned < player_max_slots) {
                    MOX::_ship[i].owner = (int8_t)MOX::_PLAYER_NUM;
                    player_assigned++;
                } else {
                    unassigned_indices[unassigned_count++] = i;
                }
            }
        }

        int ships_assigned_per_player[MAX_PLAYERS] = {0};
        ships_assigned_per_player[MOX::_PLAYER_NUM] = player_assigned;

        int current_player = (int)MOX::_stardate % (int)MOX::_NUM_PLAYERS;
        int assigned_from_unassigned = 0;
        int fail_count = 0;

        if (unassigned_count > 0) {
            while (assigned_from_unassigned < unassigned_count) {
                if (fail_count >= (int)MOX::_NUM_PLAYERS) break;

                if (ships_assigned_per_player[current_player] < colony_production_slots[current_player]) {
                    int16_t ship_idx = unassigned_indices[assigned_from_unassigned++];
                    MOX::_ship[ship_idx].owner = (int8_t)current_player;
                    ships_assigned_per_player[current_player]++;
                    fail_count = 0;
                } else {
                    fail_count++;
                }

                current_player++;
                if (current_player == (int)MOX::_NUM_PLAYERS) {
                    current_player = 0;
                }
            }
        }

        while (assigned_from_unassigned < unassigned_count) {
            if (MOX::_player[current_player].eliminated == 0) {
                int16_t ship_idx = unassigned_indices[assigned_from_unassigned++];
                MOX::_ship[ship_idx].owner = (int8_t)current_player;
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
            current_player->ship_speed = (uint8_t)INITSHIP::Calc_Player_FTL_Speed_(i);
        }
    }

    /**
     * Updates the movement range of the player's ships based on their researched fuel tech tiers.
     * Iterates backwards through fuel tiers 5 down to 1, checking if the required tech is researched.
     */
    void __cdecl Update_Player_Ship_Range_(s_player* player) {
        for (int i = FUEL_COUNT - 1; i > FUEL_NO_FUEL_CELLS; --i) {
            int16_t tech_idx = TECHDATA::_fuel[i].tech_app_id;

            // Check if the tech application for this fuel tier is fully researched
            if (player->tech_applications[tech_idx] == TECH_RESEARCH_STATUS_RESEARCHED) {
                player->ship_range = TECHDATA::_fuel[i].ship_range;
                return;
            }
        }

        // If no fuel upgrades (tiers 1-5) are researched, range is set to 0.
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
        int16_t star_idx = (int16_t)(star - MOX::_star);
        // dist is scaled by 30 pixels per unit (30^2 = 900)
        int32_t dist_sq_limit = (int32_t)dist * dist * 900;

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            s_star_data* current_star = &MOX::_star[i];

            // Check if player_idx bit is set in has_colony bitmask
            if ((current_star->has_colony >> (player_idx & 0x1F)) & 1) {
                // Check if current_star can 'see' the target star through black hole blockages
                if (!struct_::Test_Bit_Field_(current_star->black_hole_blocks, star_idx)) {
                    int32_t dx = (int32_t)star->x - current_star->x;
                    int32_t dy = (int32_t)star->y - current_star->y;

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
        if ((star->has_colony >> ((uint8_t)player_idx & 0x1F)) & 1) {
            return true;
        }

        // Check range normally
        if (Star_In_Range_Of_Player_Aux1_(star, player_idx, dist)) {
            return true;
        }

        // Check range via wormhole if the destination star is visited
        int16_t wormhole_star_idx = star->wormhole_star_id;
        if (wormhole_star_idx != -1) {
            s_star_data* wormhole_star = &MOX::_star[wormhole_star_idx];

            if ((wormhole_star->visited >> ((uint8_t)player_idx & 0x1F)) & 1) {
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
     * Checks if a star system is within the extended ship range of a player.
     * The extended range is calculated as 150% of the player's standard ship range.
     */
    bool __cdecl Star_In_Extended_Range_Of_Player_(s_star_data* star, int16_t player_idx) {
        int16_t extended_range = (int16_t)((MOX::_player[player_idx].ship_range * 3 + 1) / 2);

        return Star_In_Range_Of_Player_(star, player_idx, extended_range);
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
     * Returns whether the ship is currently travelling between stars.
     * @param ship Ship record to inspect.
     */
    bool __cdecl Ship_Is_Moving_(const s_ship_data* ship) {
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
        int16_t owner = (int8_t)ship->owner;

        if (owner >= MOX::_NUM_PLAYERS) {
            return false;
        }

        if (ship->status == 0 || ship->status == 2) {
            return true;
        }

        if (ship->status != 1) {
            return false;
        }

        s_player* player = &MOX::_player[owner];

        // Check for Hyperspace Communications
        if (player->tech_applications[TECH_APP_HYPERSPACE_COMMUNICATIONS] == TECH_RESEARCH_STATUS_RESEARCHED) {
            return true;
        }

        // Check if player has simpler comms tech and is near a relay
        if (player->tech_applications[TECH_APP_TACHYON_COMMUNICATIONS] == TECH_RESEARCH_STATUS_RESEARCHED ||
            player->tech_applications[TECH_APP_SUB_SPACE_COMMUNICATIONS] == TECH_RESEARCH_STATUS_RESEARCHED) {

            uint8_t player_mask = (uint8_t)((uint32_t)1 << ((uint8_t)ship->owner & 0x1F));

            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
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
     * Returns the ship's travel range, including the extended-range special when present.
     * @param ship Ship record to inspect.
     */
    int16_t __cdecl Ship_Range_(s_ship_data* ship) {
        if (ship->owner >= MAX_PLAYERS) {
            return 0xFF;
        }

        int16_t range = MOX::_player[ship->owner].ship_range;

        if (struct_::Test_Bit_Field_(ship->d.special_device_flags, SPECIAL_EXTENDED_FUEL_TANKS)) {
            range = (range * 3 + 1) / 2;
        }

        return range;
    }

    /**
     * Returns the destination star index encoded in the ship's current location field.
     * @param ship Ship record to inspect.
     */
    int16_t __cdecl Ship_Destination_Star_(s_ship_data* ship) {
        int16_t location = ship->location;

        if (ship->status == 1) {
            return location - SHIP_LOCATION_MOVING_OFFSET;
        }

        if (ship->status == 2) {
            return location - SHIP_LOCATION_WORMHOLE_OFFSET;
        }

        return location;
    }

    /**
     * Creates a non-player ship in the first available slot and returns its index.
     * @param owner Owner slot to assign.
     * @param design Optional design to copy into the new ship.
     * @param x Initial x-coordinate.
     * @param y Initial y-coordinate.
     */
    int16_t __cdecl Create_Nonplayer_Ship_(int8_t owner, s_ship_design* design, int16_t x, int16_t y) {
        int16_t i;

        // Search for an available ship slot
        for (i = 0; i < MAX_SHIPS; ++i) {
            if (MOX::_ship[i].owner == MAX_PLAYERS) {
                // Slot is free if status is 5 or it's beyond the current total count
                if (MOX::_ship[i].status == 5 || i >= MOX::_NUM_SHIPS) {
                    break;
                }
            }
        }

        if (i == MAX_SHIPS) {
            return -1;
        }

        if (i >= MOX::_NUM_SHIPS) {
            MOX::_NUM_SHIPS = i + 1;
        }

        // Initialize ship slot
        memset(&MOX::_ship[i], 0, sizeof(s_ship_data));

        if (design != nullptr) {
            memcpy(&MOX::_ship[i].d, design, sizeof(s_ship_design));
        }

        MOX::_ship[i].owner = owner;
        MOX::_ship[i].d.previous_owner = (uint8_t)owner;
        MOX::_ship[i].status = 0;

        // Set orbital position and coordinates
        MOX::_ship[i].location = HAROLD::XY_To_Star_Id_(x, y);
        MOX::_ship[i].x = x;
        MOX::_ship[i].y = y;

        MOX::_ship[i].officer_index = -1;
        MOX::_ship[i].d.date_of_design = (uint16_t)MOX::_stardate;

        return i;
    }

    /**
     * Advances each moving ship by one turn and finalizes any arrivals.
     */
    void __cdecl Move_All_Ships_Toward_Stars_() {
        int8_t unused_navigator_flag;
        int8_t unused_direction;

        if (MOX::_game_type >= 2 && MOX::_game_type <= 3) {
            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                MOX::_star[i].just_visited = 0;
            }
        }

        for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];

            if (Ship_Is_Moving_(ship)) {
                if (ship->location >= SHIP_LOCATION_WORMHOLE_OFFSET) {
                    ship->location -= SHIP_LOCATION_MOVING_OFFSET;
                }

                int16_t star_idx = ship->location - SHIP_LOCATION_MOVING_OFFSET;
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
                        (ship->group_has_navigator == 0 &&
                         (!_jump_gate_pass_nebula || !Ship_Uses_Jump_Gate_(ship)))
                            ? &unused_navigator_flag
                            : nullptr,
                        &unused_direction
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
     * Applies a prepared movement plan to each ship in the list.
     * @param ship_list Sentinel-terminated ship index list.
     * @param destination_star_idx Destination star index before location encoding.
     * @param move_info Precomputed movement result to apply.
     */
    void __cdecl Make_Ships_Move_To_(int16_t* ship_list, int16_t destination_star_idx, s_ship_move_info* move_info) {
        int8_t move_status;
        int i = 0;
        int16_t destination_location = destination_star_idx;

        if (move_info->turns_left == 0) {
            move_status = 0;
        } else {
            int16_t first_ship_idx = ship_list[0];
            int16_t first_loc = MOX::_ship[first_ship_idx].location;

            if (first_loc < MAX_STARS || first_loc >= SHIP_LOCATION_WORMHOLE_OFFSET) {
                move_status = 2;
                destination_location += SHIP_LOCATION_WORMHOLE_OFFSET;
            } else {
                move_status = 1;
                destination_location += SHIP_LOCATION_MOVING_OFFSET;
            }
        }

        int16_t ship_idx;
        while ((ship_idx = ship_list[i]) != -1) {
            s_ship_data* ship = &MOX::_ship[ship_idx];

            ship->location = destination_location;
            ship->turns_left = move_info->turns_left;
            ship->group_has_navigator = move_info->has_navigator;
            ship->travelling_speed = move_info->traveling_speed;
            Set_Ship_Uses_Jump_Gate_(ship, move_info->has_jumpgate_route != 0);
            ship->status = move_status;

            if (ship->d.ship_type == SHIP_TYPE_COLONY || ship->d.ship_type == SHIP_TYPE_OUTPOST) {
                int16_t planet_idx = ship->d.ship_weapon[0].type;
                if (planet_idx == PLANET_INDEX_NONE ||
                    MOX::_planet[planet_idx].star_index != destination_star_idx) {
                    SHIPS::Clear_Auto_Colonization_Flag_(ship);
                }
            }

            i++;
        }
    }

    bool __cdecl Apply_Player_Movement_Order_(int16_t* ship_list, int16_t destination_star_idx, s_ship_move_info* move_info) {
        if (ship_list == nullptr || move_info == nullptr ||
            ship_list[0] < 0 || ship_list[0] >= MOX::_NUM_SHIPS ||
            destination_star_idx < 0 || destination_star_idx > MOX::_NUM_STARS) {
            return false;
        }

        s_ship_data first_before = MOX::_ship[ship_list[0]];
        int16_t target_list[MAX_SHIPS];
        int target_count = 0;
        target_list[0] = -1;
        for (int i = 0; i < MAX_SHIPS - 1 && ship_list[i] != -1; ++i) {
            int16_t ship_idx = ship_list[i];
            if (ship_idx < 0 || ship_idx >= MOX::_NUM_SHIPS) {
                return false;
            }
            if (!Ship_Is_In_List_(ship_idx, target_list)) {
                target_list[target_count++] = ship_idx;
                target_list[target_count] = -1;
            }
        }
        if (target_count == 0) {
            return false;
        }

        int16_t source_list[MAX_SHIPS];
        int source_count = 0;
        if (Ship_Is_Moving_(&first_before)) {
            for (int16_t i = 0; i < MOX::_NUM_SHIPS && source_count < MAX_SHIPS - 1; ++i) {
                if (Ship_Is_In_Same_Moving_Fleet_(&first_before, &MOX::_ship[i]) &&
                    !Ship_Is_In_List_(i, target_list)) {
                    source_list[source_count++] = i;
                }
            }
        }
        source_list[source_count] = -1;

        int16_t target_baseline = -1;
        for (int16_t i = 0; i < MOX::_NUM_SHIPS && target_count < MAX_SHIPS - 1; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            if (!Ship_Is_In_List_(i, target_list) &&
                Ship_Is_Moving_(ship) &&
                ship->owner == first_before.owner &&
                ship->status == first_before.status &&
                ship->x == first_before.x &&
                ship->y == first_before.y &&
                Ship_Destination_Star_(ship) == destination_star_idx) {
                if (target_baseline == -1) {
                    target_baseline = i;
                }
                target_list[target_count++] = i;
            }
        }
        target_list[target_count] = -1;

        int remaining_source_count = 0;
        for (int i = 0; i < source_count; ++i) {
            if (!Ship_Is_In_List_(source_list[i], target_list)) {
                source_list[remaining_source_count++] = source_list[i];
            }
        }
        source_count = remaining_source_count;
        source_list[source_count] = -1;

        s_ship_move_info target_info;
        Ships_Try_To_Move_To_(target_list, destination_star_idx, &target_info);
        if (target_info.moving == 0) {
            return false;
        }

        s_ship_move_info source_info = {};
        int16_t source_destination = -1;
        if (source_count > 0) {
            source_destination = Ship_Destination_Star_(&MOX::_ship[source_list[0]]);
            Ships_Try_To_Move_To_(source_list, source_destination, &source_info);
        }

        if (!Confirm_Fleet_Change_(target_baseline, &target_info) ||
            (source_count > 0 && !Confirm_Fleet_Change_(source_list[0], &source_info))) {
            return false;
        }

        Apply_Movement_Result_(target_list, destination_star_idx, &target_info);
        if (source_count > 0) {
            Apply_Movement_Result_(source_list, source_destination, &source_info);
        }
        *move_info = target_info;
        return true;
    }

    /**
     * Clears the just_built flag on AI-controlled ships.
     */
    void __cdecl Unmark_Just_Built_AI_Ships_() {
        int32_t num_ships = MOX::_NUM_SHIPS;
        s_ship_data* ship_array = MOX::_ship;

        for (int16_t i = 0; i < num_ships; ++i) {
            s_ship_data& ship = ship_array[i];

            // AI players (owner 0-7) who aren't the primary human player
            // have their just_built flag cleared.
            if (ship.owner < MAX_PLAYERS && MOX::_player[(int32_t)ship.owner].objectives != PLAYER_OBJECTIVE_HUMAN) {
                ship.just_built = 0;
            }
        }
    }

    /**
     * Finalizes a ship's arrival at a star and updates related visibility state.
     * @param ship Ship record to update.
     * @param star_idx Destination star index.
     */
    void __cdecl Make_Ship_Arrive_At_Star_(s_ship_data* ship, int16_t star_idx) {
        Set_Ship_Uses_Jump_Gate_(ship, false);
        ship->location = star_idx;
        ship->status = 0;
        ship->x = MOX::_star[star_idx].x;
        ship->y = MOX::_star[star_idx].y;
        ship->mission = 0;

        MOX::_star[star_idx].ignore_combat_ships = 0;

        int8_t owner = ship->owner;

        if (owner < MAX_PLAYERS) {
            // Check for Orion discovery
            if (MOX::_star[star_idx].system_special == SYSTEM_SPECIAL_ORION_STAR && MOX::_star[star_idx].visited == 0) {
                EVENTS::Event_Orion_Discovered_((int16_t)owner);
            }

            // Update colony ship arrival flags
            if (ship->d.ship_type == SHIP_TYPE_COLONY || ship->d.ship_type == SHIP_TYPE_OUTPOST) {
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

        if (owner < MAX_PLAYERS) {
            REPORT::_something_interesting_happened[owner] = 1;

            if (MOX::_player[owner].objectives == PLAYER_OBJECTIVE_HUMAN) {
                MOX::_star[star_idx].not_used3 = 0;
            }
        }
    }

    /**
     * Evaluates whether the listed ships can move to the target star and fills out move_info.
     * @param ship_list Sentinel-terminated ship index list.
     * @param target_star_idx Destination star index.
     * @param move_info Output movement analysis record.
     */
    void __cdecl Ships_Try_To_Move_To_(int16_t* ship_list, int16_t target_star_idx, s_ship_move_info* move_info) {
        int16_t start_x = 0;
        int16_t start_y = 0;
        int32_t navigator_bonus = 0;
        int32_t min_range = 30000;
        int8_t have_start_position = 0;
        int8_t player_idx;
        int32_t ship_speed;

        s_star_data* target_star = &MOX::_star[target_star_idx];
        player_idx = MOX::_ship[*ship_list].owner;

        if (player_idx < MAX_PLAYERS) {
            s_ship_data* first_ship = &MOX::_ship[*ship_list];
            if (first_ship->status == 1) {
                ship_speed = (uint8_t)first_ship->travelling_speed -
                             (uint8_t)first_ship->group_has_navigator;
                if (Ship_Uses_Jump_Gate_(first_ship)) {
                    ship_speed -= _jump_gate_speed_bonus;
                }
            } else {
                ship_speed = (uint8_t)MOX::_player[player_idx].ship_speed;
            }
        } else {
            ship_speed = 1;
        }

        std::memset(move_info, 0, sizeof(*move_info));

        if (MOX::_NUM_STARS != target_star_idx && player_idx < MAX_PLAYERS) {
            if (MOX::_player[player_idx].traits[TRAIT_TRANS_DIMENSIONAL] == 0) {
                if (EVENTS::Event_Check_Hyperspace_Flux_() != 0 &&
                    MOX::_ship[*ship_list].status != 1) {
                    move_info->hyperspace_flux = 1;
                    return;
                }
            }
        }

        for (int i = 0; ship_list[i] != -1; ++i) {
            s_ship_data* ship = &MOX::_ship[ship_list[i]];
            int16_t range = SHIPMOVE::Ship_Range_(ship);

            if (ship->officer_index != -1) {
                s_leader_data* leader = &MOX::_leaders[ship->officer_index];
                if (leader->eta == 0) {
                    int32_t bonus = 0;
                    if ((leader->special_skills & LEADER_SHIP_SKILL_NAVIGATOR2) != 0) {
                        bonus = (OFFICER::Owned_Officer_Level_(ship->officer_index) / 3) + 1;
                    } else if ((leader->special_skills & LEADER_SHIP_SKILL_NAVIGATOR) != 0) {
                        bonus = (OFFICER::Owned_Officer_Level_(ship->officer_index) / 4) + 1;
                    }
                    if (navigator_bonus < bonus) {
                        navigator_bonus = bonus;
                    }
                }
            }

            if (have_start_position == 0) {
                have_start_position = 1;
                start_x = ship->x;
                start_y = ship->y;
            }

            if (range < min_range) {
                min_range = range;
            }
        }

        int16_t start_star_id = HAROLD::XY_To_Star_Id_(start_x, start_y);
        bool continuing_existing_route = MOX::_ship[*ship_list].status == 1 &&
                                         Ship_Destination_Star_(&MOX::_ship[*ship_list]) == target_star_idx;
        bool continuing_jump_gate_route = Fleet_Uses_Jump_Gate_(ship_list, target_star_idx);

        // Dimensional Portal logic for Antares
        if (MOX::_NUM_STARS == target_star_idx && start_star_id != -1) {
            if ((MOX::_star[start_star_id].has_dimensional_portal >> (player_idx & 0x1F)) & 1) {
                move_info->turns_left = 1;
                move_info->moving = 1;
                move_info->traveling_speed = -1;
                return;
            }
        }

        if (player_idx < MAX_PLAYERS) {
            if (!SHIPMOVE::Star_In_Range_Of_Player_(target_star, (int16_t)player_idx, (int16_t)min_range)) {
                int16_t closest_star = COMBFIND::Closest_Other_Colony_Star_((int16_t)player_idx, target_star_idx);
                if (closest_star != -1) {
                    move_info->parsecs = (uint8_t)GEO::Parsecs_Between_Stars_(closest_star, target_star_idx);
                }
                move_info->out_of_range = 1;
            }

            if (start_star_id >= 0) {
                int16_t wormhole_star_idx = MOX::_star[start_star_id].wormhole_star_id;
                if (wormhole_star_idx == target_star_idx) {
                    move_info->uses_wormhole = 1;
                }

                if (((MOX::_star[start_star_id].has_stargate >> (player_idx & 0x1F)) & 1) &&
                    ((target_star->has_stargate >> (player_idx & 0x1F)) & 1)) {
                    move_info->has_stargate_route = 1;
                }

                if (((MOX::_star[start_star_id].has_jumpgate >> (player_idx & 0x1F)) & 1) &&
                    ((target_star->has_jumpgate >> (player_idx & 0x1F)) & 1)) {
                    move_info->has_jumpgate_route = 1;
                }

                if (struct_::Test_Bit_Field_(MOX::_star[start_star_id].black_hole_blocks, target_star_idx)) {
                    if (navigator_bonus == 0 && move_info->uses_wormhole == 0) {
                        if (move_info->has_stargate_route == 0 && move_info->has_jumpgate_route == 0) {
                            move_info->blackhole_blocks = 1;
                        }
                    }
                }
            } else {
                if (continuing_jump_gate_route) {
                    move_info->has_jumpgate_route = 1;
                }
                if (GEO::Black_Hole_Blocks_Points_(start_x, start_y, target_star->x, target_star->y)) {
                    move_info->blackhole_blocks = 1;
                }
            }
        }

        int32_t base_speed = ship_speed + navigator_bonus;
        if (move_info->has_stargate_route == 0 && move_info->uses_wormhole == 0) {
            if (ship_speed <= 0) {
                move_info->immobile = 1;
            } else {
                int16_t cur_x = start_x;
                int16_t cur_y = start_y;
                uint8_t turn_count = 0;
                if (move_info->has_jumpgate_route != 0) {
                    base_speed += _jump_gate_speed_bonus;
                }

                while (cur_x != target_star->x || cur_y != target_star->y) {
                    int8_t unused_direction;
                    int8_t unused_navigator_flag;
                    turn_count++;
                    GEO::Move_Player_1_Turn_To_Star_(
                        (uint8_t)player_idx, 
                        cur_x, 
                        cur_y, 
                        target_star_idx, 
                        (int16_t)base_speed, 
                        &cur_x, 
                        &cur_y, 
                        (navigator_bonus == 0 &&
                         (!_jump_gate_pass_nebula || move_info->has_jumpgate_route == 0))
                            ? &unused_navigator_flag
                            : nullptr,
                        &unused_direction
                    );
                }
                move_info->turns_left = turn_count;
            }
        } else {
            move_info->turns_left = 0;
            if (start_star_id != target_star_idx) {
                move_info->turns_left = 1;
            }
        }

        move_info->has_navigator = (int8_t)navigator_bonus;
        
        // Final validation
        if ((move_info->blackhole_blocks != 0 &&
             move_info->has_jumpgate_route == 0 &&
             move_info->has_stargate_route == 0 &&
             navigator_bonus <= 0) ||
            (move_info->out_of_range != 0 && !continuing_existing_route) ||
            (ship_speed == 0 &&
             !continuing_existing_route &&
             (target_star->x != start_x || target_star->y != start_y))) {
            move_info->turns_left = -1;
            move_info->moving = 0;
            return;
        }

        if (move_info->has_stargate_route == 0 && move_info->uses_wormhole == 0) {
            move_info->moving = 1;
            move_info->traveling_speed = (int8_t)base_speed;
        } else {
            move_info->traveling_speed = -1;
            move_info->moving = 1;
        }
    }

    /**
     * Recomputes ETAs for moving ships that do not belong to the specified player.
     * @param player_id Player index to exclude from the update.
     */
    void __cdecl Update_Ship_ETAs_(int16_t player_id) {
        int num_ships = MOX::_NUM_SHIPS;
        s_ship_data* ships = MOX::_ship;

        for (int16_t i = 0; i < num_ships; ++i) {
            s_ship_data* ship = &ships[i];

            // Only process ships that are active/moving and do not belong to player_id
            if ((ship->status == 1 || ship->status == 2) && (int16_t)ship->owner != (int16_t)player_id) {
                int16_t dest_star_idx = SHIPMOVE::Ship_Destination_Star_(ship);
                s_star_data* star = &MOX::_star[dest_star_idx];

                int16_t target_x = star->x;
                int16_t target_y = star->y;

                int16_t current_x = ship->x;
                int16_t current_y = ship->y;
                uint8_t turn_count = 0;

                int8_t unused_navigator_flag;
                int8_t unused_direction;

                // Simulate movement turn by turn until destination is reached
                while (current_x != target_x || current_y != target_y) {
                    turn_count++;

                    int8_t* navigator_ptr = nullptr;
                    if (ship->group_has_navigator == 0 &&
                        (!_jump_gate_pass_nebula || !Ship_Uses_Jump_Gate_(ship))) {
                        navigator_ptr = &unused_navigator_flag;
                    }

                    GEO::Move_Player_1_Turn_To_Star_(
                        ship->owner,
                        current_x,
                        current_y,
                        dest_star_idx,
                        (int16_t)(uint8_t)ship->travelling_speed,
                        &current_x,
                        &current_y,
                        navigator_ptr,
                        &unused_direction
                    );
                }

                ship->turns_left = turn_count;
            }
        }
    }

    bool __cdecl Confirm_Officer_Change_(int16_t leader_idx, int16_t target_ship_idx, int8_t target_eta) {
        if (leader_idx < 0 || leader_idx >= MAX_LEADERS ||
            MOX::_leaders[leader_idx].type != LEADER_TYPE_SHIP) {
            return true;
        }

        s_leader_data* leader = &MOX::_leaders[leader_idx];
        int16_t source_ship_idx = leader->location;
        int16_t source_officer_idx = -1;
        int16_t target_officer_idx = -1;
        if (source_ship_idx >= 0 && source_ship_idx < MOX::_NUM_SHIPS) {
            source_officer_idx = MOX::_ship[source_ship_idx].officer_index;
        }
        if (target_ship_idx >= 0 && target_ship_idx < MOX::_NUM_SHIPS) {
            target_officer_idx = MOX::_ship[target_ship_idx].officer_index;
        }

        int16_t old_location = leader->location;
        int8_t old_eta = leader->eta;
        if (source_ship_idx >= 0 && source_ship_idx < MOX::_NUM_SHIPS) {
            MOX::_ship[source_ship_idx].officer_index = -1;
        }
        if (target_ship_idx >= 0 && target_ship_idx < MOX::_NUM_SHIPS) {
            MOX::_ship[target_ship_idx].officer_index = leader_idx;
            leader->location = target_ship_idx;
            leader->eta = target_eta;
        } else {
            leader->location = -1;
            leader->eta = 0;
        }

        int16_t source_list[MAX_SHIPS];
        int16_t target_list[MAX_SHIPS];
        int source_count = Build_Moving_Fleet_List_(source_ship_idx, source_list);
        int target_count = Build_Moving_Fleet_List_(target_ship_idx, target_list);
        s_ship_move_info source_info = {};
        s_ship_move_info target_info = {};
        if (source_count > 0) {
            Ships_Try_To_Move_To_(source_list, Ship_Destination_Star_(&MOX::_ship[source_list[0]]), &source_info);
        }
        if (target_count > 0 &&
            (source_count == 0 || !Ship_Is_In_List_(target_list[0], source_list))) {
            Ships_Try_To_Move_To_(target_list, Ship_Destination_Star_(&MOX::_ship[target_list[0]]), &target_info);
        } else {
            target_count = 0;
        }

        if (source_ship_idx >= 0 && source_ship_idx < MOX::_NUM_SHIPS) {
            MOX::_ship[source_ship_idx].officer_index = source_officer_idx;
        }
        if (target_ship_idx >= 0 && target_ship_idx < MOX::_NUM_SHIPS) {
            MOX::_ship[target_ship_idx].officer_index = target_officer_idx;
        }
        leader->location = old_location;
        leader->eta = old_eta;

        return (source_count == 0 || Confirm_Fleet_Change_(source_list[0], &source_info)) &&
               (target_count == 0 || Confirm_Fleet_Change_(target_list[0], &target_info));
    }

    void __cdecl Recalculate_Moving_Fleet_(int16_t ship_idx) {
        int16_t ship_list[MAX_SHIPS];
        if (Build_Moving_Fleet_List_(ship_idx, ship_list) == 0) {
            return;
        }

        int16_t destination_star_idx = Ship_Destination_Star_(&MOX::_ship[ship_list[0]]);
        s_ship_move_info move_info;
        Ships_Try_To_Move_To_(ship_list, destination_star_idx, &move_info);
        Apply_Movement_Result_(ship_list, destination_star_idx, &move_info);
    }

    void __cdecl Recalculate_All_Moving_Fleets_() {
        bool processed[MAX_SHIPS] = {};
        for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
            if (processed[i] || !Ship_Is_Moving_(&MOX::_ship[i])) {
                continue;
            }

            int16_t ship_list[MAX_SHIPS];
            if (Build_Moving_Fleet_List_(i, ship_list) == 0) {
                continue;
            }
            for (int j = 0; ship_list[j] != -1; ++j) {
                processed[ship_list[j]] = true;
            }

            int16_t destination_star_idx = Ship_Destination_Star_(&MOX::_ship[ship_list[0]]);
            s_ship_move_info move_info;
            Ships_Try_To_Move_To_(ship_list, destination_star_idx, &move_info);
            Apply_Movement_Result_(ship_list, destination_star_idx, &move_info);
        }
    }

    /**
     * Clears ignore-combat flags from stars where no opposing force remains.
     */
    void __cdecl Reset_Needless_Ignore_Combat_Ships_Flags_() {
        uint16_t presence_mask[MAX_STARS]; // Tracks which players have assets at each star
        memset(presence_mask, 0, sizeof(presence_mask));

        // Mark player presence via ships
        if (MOX::_NUM_SHIPS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
                s_ship_data* ship = &MOX::_ship[i];
                // Only active ships with a valid owner count
                if (ship->owner != (char)0xFF && ship->status == 0) {
                    presence_mask[ship->location] |= (1 << ship->owner);
                }
            }
        }

        // Mark player presence via colonies
        if (MOX::_NUM_COLONIES > 0) {
            for (int16_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
                s_colony* colony = &MOX::_colony[i];
                if (colony->owner != (char)0xFF) {
                    int16_t star_index = MOX::_planet[colony->planet].star_index;
                    presence_mask[star_index] |= (1 << colony->owner);
                }
            }
        }

        // Review stars and clear ignore_combat_ships flags where no longer applicable
        if (MOX::_NUM_STARS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                s_star_data* star = &MOX::_star[i];

                if (star->ignore_combat_ships != 0) {
                    for (int player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
                        // If the flag is set for this player at this star
                        if ((star->ignore_combat_ships >> player_idx) & 1) {
                            // Clear the flag if no other player has presence at this star
                            if ((presence_mask[i] & ~(1 << player_idx)) == 0) {
                                star->ignore_combat_ships &= ~(1 << player_idx);
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Returns whether the player has an orbiting ship at a star colonized only by another player.
     * @param player_idx Player whose ships are checked.
     * @param colony_bit_idx Colony-owner bit to test for.
     */
    bool __cdecl Player_Has_Ships_Pestering_Other_Player_(int player_idx, uint8_t colony_bit_idx) {
        s_ship_data* ship = MOX::_ship;
        s_ship_data* ship_end = MOX::_ship + MOX::_NUM_SHIPS;

        for (; ship < ship_end; ++ship) {
            if ((int8_t)ship->owner == player_idx && ship->status == 0) {
                int16_t star_idx = ship->location;
                uint8_t colony_mask = MOX::_star[star_idx].has_colony;

                if (((colony_mask >> (colony_bit_idx & 0x1f)) & 1) != 0 &&
                    ((colony_mask >> ((uint8_t)player_idx & 0x1f)) & 1) == 0) {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * Returns the number of turns the player needs to travel between two stars.
     * @param player_idx Player whose travel rules are used.
     * @param start_star_idx Starting star index.
     * @param end_star_idx Destination star index.
     */
    int16_t __cdecl Player_N_Turns_From_Star_To_Star_(int16_t player_idx, int16_t start_star_idx, int16_t end_star_idx) {
        if (start_star_idx == end_star_idx) {
            return 0;
        }

        s_star_data* start_star = &MOX::_star[start_star_idx];
        s_star_data* target_star = &MOX::_star[end_star_idx];
        uint8_t player_bit = (uint8_t)player_idx & 0x1F;

        // Check stargate connectivity (player bitmask)
        if (((start_star->has_stargate >> player_bit) & 1) &&
            ((target_star->has_stargate >> player_bit) & 1)) {
            return 1;
        }

        // Check wormhole link
        int16_t wormhole_star_idx = start_star->wormhole_star_id;
        if (wormhole_star_idx == end_star_idx) {
            return 1;
        }

        int16_t speed = MOX::_player[player_idx].ship_speed;
        bool is_jumpgate_used = false;

        // Check jumpgate connectivity (player bitmask)
        if (((start_star->has_jumpgate >> player_bit) & 1) &&
            ((target_star->has_jumpgate >> player_bit) & 1)) {
            speed += _jump_gate_speed_bonus;
            is_jumpgate_used = true;
        }

        if (!is_jumpgate_used) {
            // Black holes block travel unless jumpgates are used
            if (struct_::Test_Bit_Field_(start_star->black_hole_blocks, end_star_idx)) {
                return -1;
            }
        }

        if (speed == 0) {
            return -1;
        }

        int16_t current_x = start_star->x;
        int16_t current_y = start_star->y;
        int16_t target_x = target_star->x;
        int16_t target_y = target_star->y;
        int16_t n_turns = 0;

        int8_t unused_navigator_flag;
        int8_t unused_direction;

        while (current_x != target_x || current_y != target_y) {
            n_turns++;
            GEO::Move_Player_1_Turn_To_Star_(
                (uint8_t)player_idx,
                current_x,
                current_y,
                end_star_idx,
                (int16_t)speed,
                &current_x,
                &current_y,
                (_jump_gate_pass_nebula && is_jumpgate_used) ? nullptr : &unused_navigator_flag,
                &unused_direction
            );
        }

        return n_turns;
    }

    /**
     * Resolves the Hyperspace Beast event for one player's moving ships.
     * @param player_idx Owning player to search, or -1 for no eligible ship.
     */
    void __cdecl Do_Hyperspace_Beast_(int32_t player_idx) {
        s_MSG msg;
        msg.msg_type = MSG_TYPE_HYPERSPACE_BEAST;

        s_ship_data* selected_ship = nullptr;

        if (player_idx != -1) {
            int32_t candidate_count = 0;
            for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
                s_ship_data* ship = &MOX::_ship[i];
                if (ship->owner == player_idx && ship->status == 1) {
                    candidate_count++;
                    if (game_random::Random_(candidate_count) == 1) {
                        selected_ship = ship;
                    }
                }
            }
        }

        if (selected_ship == nullptr) {
            msg.u.hyperspace_beast.ship_idx = -1;
        } else {
            int16_t ship_index = (int16_t)(selected_ship - MOX::_ship);
            int16_t officer_index = selected_ship->officer_index;

            msg.u.hyperspace_beast.ship_idx = ship_index;
            msg.u.hyperspace_beast.officer_idx = (int8_t)officer_index;
            msg.u.hyperspace_beast.star_idx = Ship_Destination_Star_(selected_ship);

            if (officer_index != -1) {
                OFFICER::Move_Officer_To_Pool_(officer_index);

                s_leader_data* leader = &MOX::_leaders[officer_index];
                if (leader->eta > 0) {
                    msg.u.hyperspace_beast.leader_status = 1;
                } else {
                    if (MOX::_player[(uint8_t)selected_ship->owner].tech_applications[TECH_APP_SURVIVAL_PODS] == TECH_RESEARCH_STATUS_RESEARCHED) {
                        s_ship_data* replacement_ship = nullptr;
                        bool found_similar = false;

                        for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
                            s_ship_data* ship = &MOX::_ship[i];
                            if (ship->owner == player_idx &&
                                ship->status == selected_ship->status &&
                                ship->x == selected_ship->x &&
                                ship->y == selected_ship->y &&
                                ship != selected_ship &&
                                ship->location == selected_ship->location &&
                                ship->travelling_speed == selected_ship->travelling_speed) {
                                
                                found_similar = true;

                                if (ship->officer_index == -1) {
                                    if (replacement_ship == nullptr ||
                                        replacement_ship->d.size < ship->d.size) {
                                        if (ship->d.ship_type == SHIP_TYPE_COMBAT) {
                                            replacement_ship = ship;
                                        }
                                    }
                                }
                            }
                        }

                        if (replacement_ship != nullptr) {
                            replacement_ship->officer_index = officer_index;
                            leader->location = (int16_t)(replacement_ship - MOX::_ship);
                            msg.u.hyperspace_beast.leader_status = 2;
                            leader->status = 1;
                        } else {
                            msg.u.hyperspace_beast.leader_status = 1;
                            if (!found_similar) {
                                msg.u.hyperspace_beast.leader_status = 0;
                            }
                        }
                    } else {
                        msg.u.hyperspace_beast.leader_status = 0;
                    }

                    if (msg.u.hyperspace_beast.leader_status == 0) {
                        OFFICER::Kill_Officer_(officer_index);
                    }
                }
            }

            MAINSCR::Update_Ship_Icons_In_Between_Turns_();
            selected_ship->status = 8;
        }

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            msg.u.hyperspace_beast.fate = (player_idx == i) ? 1 : 0;
            MSG_::Add_Msg_(msg, i);
        }
    }

    /**
     * Resolves the wormhole event by selecting one moving fleet and snapping matching ships to arrival.
     * @param player_idx Player whose ships are eligible.
     * @param wormhole Output event result record.
     */
    void __cdecl Do_Wormhole_Event_(int32_t player_idx, s_wormhole_data* wormhole) {
        wormhole->success = 0;
        wormhole->dest_star_idx = 0;
        wormhole->count = 0;

        int32_t candidate_count = 0;
        s_ship_data* selected_ship = nullptr;

        for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            if (ship->owner == player_idx && ship->status == 1 && ship->turns_left != 1) {
                candidate_count++;
                if (game_random::Random_(candidate_count) == 1) {
                    selected_ship = ship;
                }
            }
        }

        if (selected_ship != nullptr) {
            int16_t location = selected_ship->location;
            int32_t x = selected_ship->x;
            int32_t y = selected_ship->y;
            int16_t star_idx = location - SHIP_LOCATION_MOVING_OFFSET;

            wormhole->success = 1;
            wormhole->count = star_idx;

            for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
                s_ship_data* ship = &MOX::_ship[i];
                if (ship->owner == player_idx &&
                    ship->status == 1 &&
                    ship->x == x &&
                    ship->y == y &&
                    ship->location == location &&
                    ship->travelling_speed == selected_ship->travelling_speed) {

                    Make_Ship_Arrive_At_Star_(ship, star_idx);
                    wormhole->dest_star_idx++;
                }
            }
        }

        MAINSCR::Update_Ship_Icons_In_Between_Turns_();
    }
}