#include "deps.h"
#include <cstring>

namespace COLONIZE {
    int16_t Star_N_Uncolonized_Planets_(s_star_data* star) {
        int16_t uncolonized_count = 0;

        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = star->planet_index[i];

            if (planet_idx != -1) {
                if (MOX::_planet[planet_idx].colony_index == -1) {
                    uncolonized_count++;
                }
            }
        }

        return uncolonized_count;
    }

    int16_t Star_N_Outpostable_Planets_For_Player_(s_star_data* star, int16_t player_id) {
        int16_t outpostable_count = 0;

        for (int i = 0; i < 5; ++i) {
            int16_t planet_idx = star->planet_index[i];
            if (planet_idx != -1) {
                if (MOX::_planet[planet_idx].colony_index == -1) {
                    outpostable_count++;
                }
            }
        }

        return outpostable_count;
    }

    int16_t Star_N_Colonizable_Planets_For_Player_(s_star_data* star, int16_t player_id) {
        int16_t colonizable_count = 0;

        for (int i = 0; i < 5; ++i) {
            int16_t planet_idx = star->planet_index[i];
            if (planet_idx == -1) {
                continue;
            }

            s_planet_data* planet = &MOX::_planet[planet_idx];

            if (planet->planet_type == PLANET_TYPE_PLANET) {
                int16_t colony_idx = planet->colony_index;
                if (colony_idx == -1) {
                    colonizable_count++;
                } else {
                    s_colony* colony = &MOX::_colony[colony_idx];
                    if (colony->outpost_flag != 0 && (int16_t)colony->owner == player_id) {
                        colonizable_count++;
                    }
                }
            }
        }

        return colonizable_count;
    }

    void Find_Colonize_Players_() {
        uint8_t outpost_bits[72]; // Bitmask: bit i set if player i has an outpost ship here
        uint8_t colony_bits[72];  // Bitmask: bit i set if player i has a colony ship or base here

        for (int i = 0; i < 72; ++i) {
            outpost_bits[i] = 0;
            colony_bits[i] = 0;
        }

        // Reset priority for all stars
        for (int i = 0; i < MOX::_NUM_STARS; ++i) {
            MOX::_star[i].colonize_player = 8; // Default: unassigned
        }

        // Phase 1: Check ships (Colony and Outpost ships)
        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            if (ship->status == 0) { // Active
                if (ship->d.ship_type == SHIP_TYPE_COLONY || ship->d.ship_type == SHIP_TYPE_OUTPOST) {
                    int star_idx = ship->location;
                    if (star_idx >= 0 && star_idx < 72) {
                        s_star_data* star = &MOX::_star[star_idx];
                        if (star->colonize_player == 8) {
                            star->colonize_player = ship->owner;
                            if (ship->d.ship_type == SHIP_TYPE_OUTPOST) {
                                outpost_bits[star_idx] |= (1 << (ship->owner & 0x1F));
                            } else {
                                colony_bits[star_idx] |= (1 << (ship->owner & 0x1F));
                            }
                        } else if (star->colonize_player != ship->owner) {
                            star->colonize_player = -1; // Conflict
                        }
                    }
                }
            }
        }

        // Phase 2: Check existing colonies with colony bases
        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];
            if (colony->owner != -1 && colony->buildings[BUILDING_COLONY_BASE] != 0) {
                int star_idx = MOX::_planet[colony->planet].star_index;
                s_star_data* star = &MOX::_star[star_idx];
                if (star->colonize_player == 8) {
                    star->colonize_player = colony->owner;
                    colony_bits[star_idx] |= (1 << (colony->owner & 0x1F));
                } else if (star->colonize_player != colony->owner) {
                    star->colonize_player = -1;
                }
            }
        }

        // Phase 3: Validate priority against actual colonizable/outpostable planets
        for (int i = 0; i < MOX::_NUM_STARS; ++i) {
            s_star_data* star = &MOX::_star[i];
            int8_t priority_player = star->colonize_player;
            if (priority_player != 8 && priority_player != -1) {
                bool valid = false;
                // Check if player has a colony asset and colonizable planets
                if ((colony_bits[i] >> (priority_player & 0x1F)) & 1) {
                    if (Star_N_Colonizable_Planets_For_Player_(star, priority_player) > 0) {
                        valid = true;
                    }
                }
                // If not valid yet, check outpost assets and outpostable planets
                if (!valid && ((outpost_bits[i] >> (priority_player & 0x1F)) & 1)) {
                    if (Star_N_Outpostable_Planets_For_Player_(star, priority_player) > 0) {
                        valid = true;
                    }
                }

                if (!valid) {
                    star->colonize_player = -1;
                }
            }
        }

        // Phase 4: Block priority if an enemy/rival warship is present
        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            if (ship->status == 0 && ship->d.ship_type == SHIP_TYPE_COMBAT) {
                int star_idx = ship->location;
                if (star_idx >= 0 && star_idx < 72) {
                    s_star_data* star = &MOX::_star[star_idx];
                    int8_t colonizer = star->colonize_player;
                    if (colonizer >= 0 && colonizer < 8 && ship->owner != colonizer) {
                        if (ship->owner >= 8) {
                            star->colonize_player = -1;
                        } else {
                            // Check treaty status: if we have peace/alliance (4, 5, 6), priority is contested
                            int8_t treaty = MOX::_player[colonizer].treaty[ship->owner];
                            if (treaty >= 4 && treaty <= 6) {
                                star->colonize_player = -1;
                            }
                        }
                    }
                }
            }
        }

        // Phase 5: Cleanup redundant colony bases
        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];
            if (colony->owner != -1 && colony->buildings[BUILDING_COLONY_BASE] != 0) {
                int star_idx = MOX::_planet[colony->planet].star_index;
                s_star_data* star = &MOX::_star[star_idx];
                
                bool remove_base = false;
                if (star->colonize_player != colony->owner) {
                    remove_base = true;
                } else if (Star_N_Colonizable_Planets_For_Player_(star, colony->owner) == 0) {
                    remove_base = true;
                }

                if (remove_base) {
                    int refund = TECHDATA::_buildings[BUILDING_COLONY_BASE].cost / 2;
                    colony->buildings[BUILDING_COLONY_BASE] = 0;
                    MOX::_player[colony->owner].bc += refund;

                    uint32_t header = (uint32_t)((uint8_t)colony->planet << 16);
                    uint32_t p2 = (uint32_t)BUILDING_COLONY_BASE | ((uint32_t)(uint16_t)refund << 16);
                    MSG_::Add_Message(header, p2, 0, 0, 0, (int16_t)colony->owner);
                }
            }
        }
    }

    void Allocate_New_Colony_Slots_() {
        uint16_t uncolonized_counts[8];
        uint16_t allocated_counts[8];
        uint8_t free_slots_indices[250];

        memset(&uncolonized_counts, 0, sizeof(uncolonized_counts));
        memset(&allocated_counts, 0, sizeof(allocated_counts));
        memset(&free_slots_indices, 0, sizeof(free_slots_indices));

        int16_t free_slots_count = 0;

        Find_Colonize_Players_();

        for (int i = 0; i < MOX::_NUM_STARS; ++i) {
            s_star_data* star = &MOX::_star[i];
            if (star->colonize_player < 8) {
                int16_t count = Star_N_Uncolonized_Planets_(star);
                uncolonized_counts[star->colonize_player] += count;
            }
        }

        for (int16_t i = 0; i < 250; ++i) {
            MOX::_colony[i].allocated_to = (char)0xFF;
            if (i >= MOX::_NUM_COLONIES || MOX::_colony[i].planet == -1) {
                free_slots_indices[free_slots_count++] = (uint8_t)i;
            }
        }

        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        int16_t current_player = (int16_t)(MOX::_stardate % MOX::_NUM_PLAYERS);
        int16_t skipped_players = 0;
        int16_t free_slot_ptr = 0;

        while (skipped_players < MOX::_NUM_PLAYERS) {
            if (allocated_counts[current_player] < uncolonized_counts[current_player]) {
                uint8_t colony_idx = free_slots_indices[free_slot_ptr++];
                MOX::_colony[colony_idx].allocated_to = (char)current_player;
                allocated_counts[current_player]++;
                skipped_players = 0;
            } else {
                skipped_players++;
            }

            current_player++;
            if (current_player == MOX::_NUM_PLAYERS) {
                current_player = 0;
            }
        }
    }
}