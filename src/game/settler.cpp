#include "pch.h"

namespace settler {
    int8_t __cdecl Add_Settler_To_Colony_(uint32_t settler_data, s_colony* colony) {
        bool is_full = false;
        int16_t race_idx = (int16_t)((settler_data & 0x000F0000) >> 16);

        if (!Room_For_Another_(colony, race_idx, 0)) {
            is_full = true;
        } else {
            if ((settler_data & 0x03000000) == 0) {
                int32_t farmer_count = 0;
                uint32_t* target_pop = nullptr;

                for (int32_t i = 0; i < (uint8_t)colony->n_pops; ++i) {
                    uint32_t pop_val = colony->pop[i];
                    if ((pop_val & 0x00000180) == 0) {
                        farmer_count++;
                        if (target_pop == nullptr && (pop_val & 0x0000000F) < 8) {
                            target_pop = &colony->pop[i];
                        }
                    }
                }

                if (farmer_count >= (uint8_t)colony->max_farms) {
                    if ((settler_data & 0x000F0000) < 0x00080000) {
                        settler_data = (settler_data & 0xFEFFFFFF) | 0x02000000;
                    } else if (target_pop != nullptr) {
                        *target_pop = (*target_pop & 0xFFFFFF7F) | 0x00000100;
                    }
                }
            }
        }

        if (!is_full) {
            uint32_t race_info = settler_data >> 16;
            uint8_t n_pops = (uint8_t)colony->n_pops;
            uint32_t* pop_slot = &colony->pop[n_pops];
            uint32_t new_pop = *pop_slot;

            new_pop = (new_pop ^ race_info) & 0x0000000F ^ new_pop;
            new_pop = ((race_info << 4) ^ new_pop) & 0x00000070 ^ new_pop;
            new_pop = ((settler_data >> 17) ^ new_pop) & 0x00000180 ^ new_pop;

            new_pop |= 0x00000200;
            new_pop &= 0xFFFFFBFF;

            *pop_slot = new_pop;
            colony->n_pops++;
        }

        return !is_full;
    }

    int32_t __cdecl N_Freighters_Player_Can_Scrap_(int16_t player_idx) {
        return MOX::_player[player_idx].n_freighters - (MOX::_player[player_idx].settlers_freighted * 5);
    }

    bool __cdecl Room_For_Another_(s_colony* colony, int16_t race_idx, int8_t include_freighters) {
        int32_t pop_limit = (int32_t)COLCALC::Colony_Race_Pop_Limit_(colony, race_idx);
        int32_t matching_pop_count = 0;

        for (int32_t i = 0; i < (uint8_t)colony->n_pops; ++i) {
            int16_t current_pop_race = (int16_t)(colony->pop[i] & 0xF);
            if (COLCALC::Colony_Race_Pop_Limit_(colony, current_pop_race) <= pop_limit) {
                matching_pop_count++;
            }
        }

        if (include_freighters != 0) {
            s_player* player = &MOX::_player[(int)colony->owner];
            int32_t colony_idx = (int32_t)(colony - MOX::_colony);

            if (player->settlers_freighted > 0) {
                for (int32_t i = 0; i < (int32_t)player->settlers_freighted; ++i) {
                    s_settler_info& settler = player->settlers[i];
                    uint8_t dest_planet = (uint8_t)((settler.val >> 8) & 0xffu);

                    if ((int32_t)MOX::_planet[dest_planet].colony_index == colony_idx) {
                        if (COLCALC::Colony_Race_Pop_Limit_(colony, (int16_t)((settler.val >> 16) & 0x0fu)) <= pop_limit) {
                            matching_pop_count++;
                        }
                    }
                }
            }
        }

        return matching_pop_count < pop_limit;
    }

    void __cdecl Move_Settlers_() {
        s_player* curr_player = MOX::_player;
        if (MOX::_player + MOX::_NUM_PLAYERS <= curr_player) {
            return;
        }

        do {
            bool has_arrivals = false;
            int32_t settler_idx = 0;
            int8_t arrivals_per_planet[360];
            int8_t over_limit_per_planet[360];
            const int32_t planet_count = (int32_t)(sizeof(arrivals_per_planet) / sizeof(arrivals_per_planet[0]));

            memset(arrivals_per_planet, 0, sizeof(arrivals_per_planet));
            memset(over_limit_per_planet, 0, sizeof(over_limit_per_planet));

            if (curr_player->settlers_freighted > 0) {
                do {
                    uint32_t val = curr_player->settlers[settler_idx].val;

                    // Settler records are packed into one dword: bits 0-7 are source_colony and bits 20-23 are ETA.
                    // Each turn the original code wraps ETA modulo 16 and stamps source_colony to 0xFA.
                    uint32_t eta = ((val >> 20) - 1u) & 0x0fu;
                    val = (val & 0xff0fff00u) | (eta << 20) | 0xfau;
                    curr_player->settlers[settler_idx].val = val;

                    if ((val & 0xf00000) == 0) {
                        uint8_t dest_planet = (val >> 8) & 0xff;
                        has_arrivals = true;
                        arrivals_per_planet[dest_planet]++;

                        int32_t player_idx = (int32_t)(curr_player - MOX::_player);
                        int16_t star_index = (int16_t)MOX::_planet[dest_planet].star_index;
                        uint8_t blockaded = MOX::_star[star_index].blockaded;

                        if ((((uint8_t)(blockaded >> ((uint8_t)player_idx & 0x1f))) & 1) == 0) {
                            if (EVENTS::Event_Check_Space_Anomaly_At_Star_(star_index) == 0) {
                                int32_t colony_index = MOX::_planet[dest_planet].colony_index;
                                if (colony_index != -1) {
                                    s_colony* colony = &MOX::_colony[colony_index];
                                    if (colony->outpost_flag == 0) {
                                        if (colony->owner == player_idx) {
                                            if (Add_Settler_To_Colony_(val, colony) == 0) {
                                                over_limit_per_planet[dest_planet]++;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        curr_player->settlers_freighted--;
                        int32_t remaining_settler_count = curr_player->settlers_freighted - settler_idx;
                        memmove(
                                &curr_player->settlers[settler_idx],
                                &curr_player->settlers[settler_idx + 1],
                                remaining_settler_count * sizeof(curr_player->settlers[0]));
                    } else {
                        settler_idx++;
                    }
                } while (settler_idx < curr_player->settlers_freighted);
            }

            if (has_arrivals) {
                for (int32_t planet_idx = 0; planet_idx < planet_count; ++planet_idx) {
                    if (arrivals_per_planet[planet_idx] != 0) {
                        s_MSG msg;
                        memset(&msg, 0, sizeof(msg));

                        msg.msg_type = 0 /* MSG_TYPE_COLONY */;
                        // Note: Original game truncates planet_idx > 255 due to 8-bit field, which we accurately preserve.
                        msg.u.colony.planet_idx = planet_idx;
                        msg.u.colony.subtype = 5 /* s_MSG_colony_settlers */;
                        msg.u.colony.c.settlers.settlers = arrivals_per_planet[planet_idx];
                        msg.u.colony.c.settlers.over_limit = over_limit_per_planet[planet_idx];
                        
                        int32_t player_idx = (int32_t)(curr_player - MOX::_player);
                        int16_t star_index = (int16_t)MOX::_planet[planet_idx].star_index;
                        msg.u.colony.c.settlers.blockaded =
                                (uint8_t)(MOX::_star[star_index].blockaded >> ((uint8_t)player_idx & 0x1f)) & 1;

                        MSG_::Add_Msg_(msg, player_idx);
                    }
                }
            }
            
            curr_player++;
        } while (curr_player < MOX::_player + MOX::_NUM_PLAYERS);
    }

    int32_t __cdecl Settler_ETA_(s_player* player, int16_t source_star, int16_t dest_star) {
        int32_t distance = GEO::Parsecs_Between_Stars_(source_star, dest_star);
        if (distance == 0) {
            return 0;
        }

        int32_t speed = (uint8_t)player->ship_speed;
        if (speed < 2) {
            speed = 2;
        }

        distance = (distance + speed - 1) / speed;
        if (distance > 15) {
            distance = 15;
        }

        return distance;
    }

    void __cdecl Pop_Tries_To_Settle_(
            s_colony *source_colony,
            int16_t selected_pop_idx,
            s_colony *dest_colony,
            s_settler_settle_result *result) {
        s_player *player = &MOX::_player[source_colony->owner];
        const int16_t max_freighted_settlers = (int16_t)(sizeof(player->settlers) / sizeof(player->settlers[0]));
        const int32_t source_star_idx = (int32_t)MOX::_planet[source_colony->planet].star_index;
        const int32_t dest_star_idx = (int32_t)MOX::_planet[dest_colony->planet].star_index;

        memset(result, 0, sizeof(*result));

        if (dest_colony->owner != source_colony->owner) {
            result->dest_owner_mismatch = 1;
            return;
        }

        if ((((uint32_t)(uint8_t)MOX::_star[source_star_idx].blockaded >>
              ((uint32_t)(uint8_t)source_colony->owner & 0x1fU)) &
             1U) != 0) {
            result->source_blockaded = 1;
            return;
        }

        if (EVENTS::Event_Check_Space_Anomaly_((int32_t)(source_colony - MOX::_colony)) != 0) {
            result->source_space_anomaly = 1;
            return;
        }

        if (EVENTS::Event_Check_Plague_((int32_t)(source_colony - MOX::_colony)) != 0) {
            result->source_plague = 1;
            return;
        }

        if (dest_colony->outpost_flag != 0) {
            result->dest_is_outpost = 1;
            return;
        }

        if (source_colony->n_pops == 1) {
            result->only_one_pop_left = 1;
            return;
        }

        if ((source_colony->pop[selected_pop_idx] & 0x400) != 0) {
            result->selected_pop_flag_0x400 = 1;
            return;
        }

        if (dest_colony == source_colony) {
            result->can_settle = 1;
            return;
        }

        if (dest_star_idx != source_star_idx) {
            if (player->n_freighters < player->settlers_freighted * 5 + 5) {
                result->insufficient_freighters = 1;
                return;
            }

            if (player->settlers_freighted == max_freighted_settlers) {
                result->max_settlers_in_transit = 1;
                return;
            }
        }

        if (Room_For_Another_(dest_colony, source_colony->pop[selected_pop_idx] & 0x0f, 1) == 0) {
            result->no_room_at_destination = 1;
        }

        if ((((uint32_t)(uint8_t)MOX::_star[dest_star_idx].blockaded >>
              ((uint32_t)(uint8_t)dest_colony->owner & 0x1fU)) &
             1U) != 0) {
            result->dest_blockaded = 1;
        }

        if (EVENTS::Event_Check_Space_Anomaly_((int32_t)(dest_colony - MOX::_colony)) != 0) {
            result->dest_space_anomaly = 1;
        }

        result->eta_turns = (uint8_t)Settler_ETA_(player, source_star_idx, dest_star_idx);
        result->can_settle = 1;
    }

    int32_t __cdecl Settle_Pop_(s_colony* source_colony, int16_t pop_index, s_colony* dest_colony) {
        int32_t ret_value = (int32_t)(uintptr_t)dest_colony;

        if (dest_colony != source_colony) {
            s_player* owner_player = &MOX::_player[(int32_t)source_colony->owner];
            const int32_t source_planet_index = (int32_t)source_colony->planet;
            const int32_t dest_planet_index = (int32_t)dest_colony->planet;
            const int16_t source_star_index =
                    (int16_t)(int8_t)MOX::_planet[source_planet_index].star_index;
            const int16_t dest_star_index =
                    (int16_t)(int8_t)MOX::_planet[dest_planet_index].star_index;
            uint32_t* pop_slot = &source_colony->pop[(int32_t)pop_index];
            const uint32_t pop_value = *pop_slot;
            uint32_t encoded_settler = pop_value;

            encoded_settler ^= ((pop_value << 16) ^ encoded_settler) & 0x000F0000;
            encoded_settler ^= ((pop_value << 17) ^ encoded_settler) & 0x03000000;

            if (dest_star_index == source_star_index) {
                Add_Settler_To_Colony_(encoded_settler, dest_colony);
            } else {
                encoded_settler ^=
                        ((((uint32_t)Settler_ETA_(owner_player, source_star_index, dest_star_index)) << 20) ^
                          encoded_settler) &
                         0x00F00000;
                encoded_settler ^=
                        (((uint32_t)(int32_t)(source_colony - MOX::_colony) ^ encoded_settler) &
                         0x000000FF);
                encoded_settler ^=
                        ((((uint32_t)(int32_t)dest_colony->planet << 8) ^ encoded_settler) &
                         0x0000FF00);

                const int32_t freighted_index = (int32_t)owner_player->settlers_freighted;
                owner_player->settlers[freighted_index].val = encoded_settler;
                owner_player->settlers_freighted =
                        (int16_t)(owner_player->settlers_freighted + 1);
            }

            const uint8_t last_pop_index =
                    (uint8_t)((uint8_t)source_colony->n_pops - 1);
            const uint32_t last_pop_value = source_colony->pop[last_pop_index];

            source_colony->n_pops = last_pop_index;
            ret_value = (int32_t)last_pop_value;
            *pop_slot = last_pop_value;
        }

        return ret_value;
    }
}