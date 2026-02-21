#include "deps.h"

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
}