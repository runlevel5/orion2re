#include "pch.h"

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

    int16_t Star_N_Outpostable_Planets_(s_star_data* star) {
        int16_t outpostable_count = 0;

        for (int32_t i = 0; i < 5; ++i) {
            int16_t planet_idx = star->planet_index[i];
            if (planet_idx != -1) {
                if (MOX::_planet[planet_idx].colony_index == -1) {
                    outpostable_count++;
                }
            }
        }

        return outpostable_count;
    }

    int16_t Star_N_Colonizable_Planets_For_Player_(s_star_data* star, int16_t player_idx) {
        int16_t colonizable_count = 0;

        for (int32_t i = 0; i < 5; ++i) {
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
                    if (colony->outpost_flag != 0 && colony->owner == player_idx) {
                        colonizable_count++;
                    }
                }
            }
        }

        return colonizable_count;
    }

    void Find_Colonize_Players_() {
        uint8_t colony_bits[MAX_STARS];
        uint8_t outpost_bits[MAX_STARS];

        memset(colony_bits, 0, sizeof(colony_bits));
        memset(outpost_bits, 0, sizeof(outpost_bits));

        // Reset priority for all stars
        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            MOX::_star[i].colonize_player = MAX_PLAYERS;
        }

        // Phase 1: Check ships (Colony and Outpost ships)
        for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            if (ship->status == 0) { // Active
                if (ship->d.ship_type == SHIP_TYPE_COLONY || ship->d.ship_type == SHIP_TYPE_OUTPOST) {
                    int16_t star_idx = ship->location;
                    if (star_idx >= 0 && star_idx < MAX_STARS) {
                        s_star_data* star = &MOX::_star[star_idx];
                        if (star->colonize_player == MAX_PLAYERS) {
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
        for (int16_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];
            if (colony->owner != -1 && colony->buildings[BUILDING_COLONY_BASE] != 0) {
                int16_t star_idx = MOX::_planet[colony->planet].star_index;
                s_star_data* star = &MOX::_star[star_idx];
                if (star->colonize_player == MAX_PLAYERS) {
                    star->colonize_player = colony->owner;
                    colony_bits[star_idx] |= (1 << (colony->owner & 0x1F));
                } else if (star->colonize_player != colony->owner) {
                    star->colonize_player = -1;
                }
            }
        }

        // Phase 3: Validate priority against actual colonizable/outpostable planets
        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            s_star_data* star = &MOX::_star[i];
            int8_t priority_player = star->colonize_player;
            if (priority_player == MAX_PLAYERS) {
                star->colonize_player = -1;
            } else {
                bool keep = false;
                if ((colony_bits[i] >> (priority_player & 0x1F)) & 1) {
                    if (Star_N_Colonizable_Planets_For_Player_(star, priority_player) > 0) {
                        keep = true;
                    }
                }
                if (!keep && ((outpost_bits[i] >> (priority_player & 0x1F)) & 1)) {
                    if (Star_N_Outpostable_Planets_(star) > 0) {
                        keep = true;
                    }
                }

                if (!keep) {
                    star->colonize_player = -1;
                }
            }
        }

        // Phase 4: Block priority if an enemy/rival warship is present
        for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            if (ship->status == 0 && ship->d.ship_type == SHIP_TYPE_COMBAT) {
                int16_t star_idx = ship->location;
                if (star_idx >= 0 && star_idx < MAX_STARS) {
                    s_star_data* star = &MOX::_star[star_idx];
                    int8_t colonizer = star->colonize_player;
                    if (colonizer >= 0 && colonizer < MAX_PLAYERS && ship->owner != colonizer) {
                        if (ship->owner >= MAX_PLAYERS) {
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
        for (int16_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];
            if (colony->owner != -1 && colony->buildings[BUILDING_COLONY_BASE] != 0) {
                int16_t star_idx = MOX::_planet[colony->planet].star_index;
                s_star_data* star = &MOX::_star[star_idx];
                
                bool remove_base = false;
                if (star->colonize_player != colony->owner) {
                    remove_base = true;
                } else if (Star_N_Colonizable_Planets_For_Player_(star, colony->owner) == 0) {
                    remove_base = true;
                }

                if (remove_base) {
                    int16_t owner = colony->owner;
                    int32_t refund32 = TECHDATA::_buildings[BUILDING_COLONY_BASE].cost / 2;
                    int16_t refund16 = (int16_t)refund32;
                    colony->buildings[BUILDING_COLONY_BASE] = 0;
                    MOX::_player[owner].bc += refund32;

                    s_MSG msg = {};
                    msg.msg_type = MSG_TYPE_COLONY;
                    msg.u.colony.planet_idx = colony->planet;
                    msg.u.colony.subtype = 0;
                    msg.u.colony.c.scrap.building_id = BUILDING_COLONY_BASE;
                    msg.u.colony.c.scrap.bc = refund16;
                    MSG_::Add_Msg_(msg, owner);
                }
            }
        }
    }

    void Allocate_New_Colony_Slots_() {
        int16_t uncolonized_counts[MAX_PLAYERS] = { 0 };
        int16_t allocated_counts[MAX_PLAYERS] = { 0 };
        int16_t free_slots_indices[MAX_COLONIES];

        int16_t free_slots_count = 0;

        Find_Colonize_Players_();

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            s_star_data* star = &MOX::_star[i];
            if (star->colonize_player != -1) {
                int16_t count = Star_N_Uncolonized_Planets_(star);
                uncolonized_counts[(int8_t)star->colonize_player] += count;
            }
        }

        for (int16_t i = 0; i < MAX_COLONIES; ++i) {
            MOX::_colony[i].allocated_to = (char)0xFF;
            if (i >= MOX::_NUM_COLONIES || MOX::_colony[i].planet == -1) {
                free_slots_indices[free_slots_count++] = i;
            }
        }

        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        int16_t current_player = (int16_t)((int32_t)MOX::_stardate % MOX::_NUM_PLAYERS);
        int16_t skipped_players = 0;
        int16_t free_slot_ptr = 0;

        while (skipped_players < MOX::_NUM_PLAYERS) {
            if (allocated_counts[current_player] < uncolonized_counts[current_player]) {
                int16_t colony_idx = free_slots_indices[free_slot_ptr++];
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

    void __cdecl AI_Colonize_(int16_t player_idx) {
        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            s_star_data* star = &MOX::_star[i];
            bool done_with_star = false;

            while (!done_with_star) {
                if (star->colonize_player == player_idx) {
                    s_colony* colony_base_ptr = REPORT::Player_Colony_Base_At_Star_(player_idx, i);
                    s_ship_data* colony_ship_ptr = REPORT::Player_Ship_Type_At_Star_(player_idx, SHIP_TYPE_COLONY, i, 1);

                    if (colony_base_ptr == nullptr && colony_ship_ptr == nullptr) {
                        done_with_star = true;
                    } else {
                        uint32_t best_worth = 0;
                        int16_t best_planet_idx = -1;

                        for (int p = 0; p < 5; ++p) {
                            int16_t planet_idx = star->planet_index[p];
                            if (planet_idx != -1 && MOX::_planet[planet_idx].colony_index == -1) {
                                uint32_t worth = AIDATA::Uncolonized_Planet_Worth_To_Player_(&MOX::_planet[planet_idx], &MOX::_player[player_idx]);
                                if (best_worth < (worth & 0xFFFF)) {
                                    best_planet_idx = planet_idx;
                                    best_worth = worth & 0xFFFF;
                                }
                            }
                        }

                        if (best_planet_idx == -1) {
                            done_with_star = true;
                            if (colony_base_ptr != nullptr) {
                                // Sell colony base buildings if no worthwhile planets found
                                do {
                                    MOX::_player[player_idx].bc += TECHDATA::_buildings[BUILDING_COLONY_BASE].cost / 2;
                                    colony_base_ptr->buildings[BUILDING_COLONY_BASE] = 0;
                                    colony_base_ptr = REPORT::Player_Colony_Base_At_Star_(player_idx, i);
                                } while (colony_base_ptr != nullptr);
                            }
                        } else {
                            COLONIZE::Make_New_Colony_(best_planet_idx, player_idx);

                            if (colony_base_ptr == nullptr) {
                                // Consumed a colony ship
                                for (int16_t s = 0; s < MOX::_NUM_SHIPS; ++s) {
                                    s_ship_data* ship = &MOX::_ship[s];
                                    if (ship->mission == 4 && ship->owner == (char)player_idx && ship->location == i) {
                                        ship->mission = 0;
                                    }
                                }
                                SHIPS::Kill_Ship_((int16_t)(colony_ship_ptr - MOX::_ship));
                                MAINSCR::Update_Ship_Icons_In_Between_Turns_();
                            } else {
                                // Consumed an existing colony base building
                                ERICNET::Remove_Building_((int16_t)(colony_base_ptr - MOX::_colony), BUILDING_COLONY_BASE);
                            }
                            // Continue checking system (logic loops to see if more ships/bases exist)
                        }
                    }
                } else {
                    done_with_star = true;
                }
            }
        }
    }

    void __cdecl All_AI_Colonize_() {
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].objectives != PLAYER_OBJECTIVE_HUMAN) {
                AI_Colonize_(i);
            }
        }
    }

    void __cdecl Make_New_Colony_Or_Outpost_(int16_t planet_idx, int16_t player_idx, int16_t is_outpost) {
        bool is_reused = false;
        s_planet_data* planet = &MOX::_planet[planet_idx];
        s_colony* colony = nullptr;

        if (is_outpost == 0 && planet->colony_index != -1) {
            is_reused = true;
            colony = &MOX::_colony[planet->colony_index];
        } else {
            for (int16_t colony_idx = 0; colony_idx < MAX_COLONIES; ++colony_idx) {
                if (MOX::_colony[colony_idx].allocated_to == (int8_t)player_idx) {
                    colony = &MOX::_colony[colony_idx];
                    break;
                }
            }
        }

        if (colony == nullptr) {
            return;
        }

        if (colony >= (MOX::_colony + MOX::_NUM_COLONIES)) {
            MOX::_NUM_COLONIES = (int16_t)(colony - MOX::_colony) + 1;
        }

        int16_t colony_idx = (int16_t)(colony - MOX::_colony);
        INITGAME::Init_Colony_(colony_idx, planet_idx, (uint8_t)player_idx);

        colony->allocated_to = -1;

        if (is_outpost == 0) {
            colony->n_pops = 1;

            if (planet->food_per_farmer < 1 || MOX::_player[player_idx].traits[TRAIT_EATS_MINERALS] != 0 || MOX::_player[player_idx].traits[TRAIT_CYBERNETIC] != 0) {
                colony->pop[0] = (colony->pop[0] & 0xFFFFFEFF) | 0x80;
            } else {
                colony->pop[0] = colony->pop[0] & 0xFFFFFE7F;
            }

            colony->pop[0] = (colony->pop[0] & ~0x0F) | (player_idx & 0x0F);
            colony->pop[0] = (colony->pop[0] & ~0x70) | ((player_idx << 4) & 0x70);
            colony->pop[0] |= 0x200;

            if (planet->planet_special == 6 /* ORION */) {
                for (int32_t i = 1; i < 4; ++i) {
                    uint32_t pop_flags = colony->pop[i];
                    pop_flags = pop_flags & 0xFFFFFE7F;
                    pop_flags = (pop_flags & 0xFFFFFFF9) | 9;
                    pop_flags = (pop_flags & ~0x70) | ((player_idx << 4) & 0x70);
                    pop_flags |= 0x200;
                    colony->pop[i] = pop_flags;
                }
                colony->n_pops = 4;
                planet->planet_special = 0;
                MOX::_star[planet->star_index].system_special = 0;
            }

            if (is_reused) {
                colony->buildings[BUILDING_MARINE_BARRACKS] = 1;
            }

            COLCALC::Colony_Calculation_(colony_idx);
            colony->just_produced = COLONY_PRODUCTION_JUST_COLONIZED;
        } else {
            colony->outpost_flag = 1;
            colony->n_pops = 0;
            colony->climate = planet->climate;
        }

        colony->producing[0] = COLONY_PRODUCTION_NONE;
        COLCALC::Compute_Star_Colony_Stuff_(&MOX::_star[planet->star_index]);
    }

    void __cdecl Make_New_Colony_(int16_t planet_idx, int16_t player_idx) {
        COLONIZE::Make_New_Colony_Or_Outpost_(planet_idx, player_idx, 0);
    }

    void __cdecl Make_New_Outpost_(int16_t planet_idx, int16_t player_idx) {
        COLONIZE::Make_New_Colony_Or_Outpost_(planet_idx, player_idx, 1);
    }
}