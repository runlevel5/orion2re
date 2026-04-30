#include "pch.h"

namespace settler {
    int8_t __cdecl Add_Settler_To_Colony_(uint32_t settler_data, s_colony* colony) {
        bool is_full = false;
        int16_t race_idx = (int16_t)((settler_data & 0x000F0000) >> 16);

        if (!Room_For_Another_(colony, race_idx, 0)) {
            is_full = true;
        } else {
            if ((settler_data & 0x03000000) == 0) {
                int farmer_count = 0;
                uint32_t* target_pop = nullptr;

                for (int i = 0; i < (uint8_t)colony->n_pops; ++i) {
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

        for (int i = 0; i < (uint8_t)colony->n_pops; ++i) {
            int16_t current_pop_race = (int16_t)(colony->pop[i] & 0xF);
            if (COLCALC::Colony_Race_Pop_Limit_(colony, current_pop_race) <= pop_limit) {
                matching_pop_count++;
            }
        }

        if (include_freighters != 0) {
            s_player* player = &MOX::_player[(int)colony->owner];
            int32_t colony_idx = (int32_t)(colony - MOX::_colony);

            if (player->settlers_freighted > 0) {
                for (int i = 0; i < (int32_t)player->settlers_freighted; ++i) {
                    s_settler_info& settler = player->settlers[i];

                    if ((int32_t)MOX::_planet[settler.dest_planet].colony_index == colony_idx) {
                        // Note: eta field is used as race index
                        if (COLCALC::Colony_Race_Pop_Limit_(colony, (int16_t)settler.eta) <= pop_limit) {
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
            int settler_idx = 0;
            char arrivals_per_planet[360];
            char over_limit_per_planet[360];

            memset(arrivals_per_planet, 0, sizeof(arrivals_per_planet));
            memset(over_limit_per_planet, 0, sizeof(over_limit_per_planet));

            if (curr_player->settlers_freighted > 0) {
                uint32_t* settlers_raw = reinterpret_cast<uint32_t*>(curr_player->settlers);
                do {
                    uint32_t val = settlers_raw[settler_idx];

                    // Decrement eta (bits 20-23) handling bit borrowing, and force source_colony (bits 0-7) to 250 (0xFA).
                    val = ((val - 0x100000) ^ val) & 0xf00000 ^ val;
                    val = (val & 0xfffffffa) | 0xfa;
                    settlers_raw[settler_idx] = val;

                    if ((val & 0xf00000) == 0) {
                        uint8_t dest_planet = (val >> 8) & 0xff;
                        has_arrivals = true;
                        arrivals_per_planet[dest_planet]++;

                        int player_idx = curr_player - MOX::_player;
                        int star_index = MOX::_planet[dest_planet].star_index;

                        if (((MOX::_star[star_index].blockaded >> player_idx) & 1) == 0) {
                            if (EVENTS::Event_Check_Space_Anomaly_At_Star_(star_index) == 0) {
                                int colony_index = MOX::_planet[dest_planet].colony_index;
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
                        int to_copy = curr_player->settlers_freighted - settler_idx;
                        memmove(&settlers_raw[settler_idx], &settlers_raw[settler_idx + 1], to_copy * 4);
                    } else {
                        settler_idx++;
                    }
                } while (settler_idx < curr_player->settlers_freighted);
            }

            if (has_arrivals) {
                for (int planet_idx = 0; planet_idx < 360; ++planet_idx) {
                    if (arrivals_per_planet[planet_idx] != 0) {
                        s_MSG msg;
                        memset(&msg, 0, sizeof(msg));

                        msg.msg_type = 0 /* MSG_TYPE_COLONY */;
                        // Note: Original game truncates planet_idx > 255 due to 8-bit field, which we accurately preserve.
                        msg.u.colony.planet_idx = planet_idx;
                        msg.u.colony.subtype = 5 /* s_MSG_colony_settlers */;
                        msg.u.colony.c.settlers.settlers = arrivals_per_planet[planet_idx];
                        msg.u.colony.c.settlers.over_limit = over_limit_per_planet[planet_idx];
                        
                        int player_idx = curr_player - MOX::_player;
                        int star_index = MOX::_planet[planet_idx].star_index;
                        msg.u.colony.c.settlers.blockaded = (MOX::_star[star_index].blockaded >> player_idx) & 1;

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

        int32_t speed = static_cast<uint8_t>(player->ship_speed);
        if (speed < 2) {
            speed = 2;
        }

        distance = (distance + speed - 1) / speed;
        if (distance > 15) {
            distance = 15;
        }

        return distance;
    }

    void __cdecl Settle_Pop_(s_colony* source_colony, int16_t pop_index, s_colony* dest_colony) {
        if (dest_colony != source_colony) {
            s_player* owner_player = &MOX::_player[static_cast<int32_t>(source_colony->owner)];
            const int32_t source_planet_index = static_cast<int32_t>(source_colony->planet);
            const int32_t dest_planet_index = static_cast<int32_t>(dest_colony->planet);
            const int16_t source_star_index =
                    static_cast<int16_t>(static_cast<int8_t>(MOX::_planet[source_planet_index].star_index));
            const int16_t dest_star_index =
                    static_cast<int16_t>(static_cast<int8_t>(MOX::_planet[dest_planet_index].star_index));
            uint32_t* pop_slot = &source_colony->pop[static_cast<int32_t>(pop_index)];
            const uint32_t pop_value = *pop_slot;
            uint32_t encoded_settler; // BUG: original code uses this dword without initialization

            encoded_settler ^= ((pop_value << 16) ^ encoded_settler) & 0x000F0000;
            encoded_settler ^= ((pop_value << 17) ^ encoded_settler) & 0x03000000;

            if (dest_star_index == source_star_index) {
                Add_Settler_To_Colony_(encoded_settler, dest_colony);
            } else {
                encoded_settler ^=
                        (((static_cast<uint32_t>(Settler_ETA_(owner_player, source_star_index, dest_star_index)) << 20) ^
                          encoded_settler) &
                         0x00F00000);
                encoded_settler ^=
                        ((static_cast<uint32_t>(static_cast<int32_t>(source_colony - MOX::_colony)) ^ encoded_settler) &
                         0x000000FF);
                encoded_settler ^=
                        (((static_cast<uint32_t>(static_cast<int32_t>(dest_colony->planet)) << 8) ^ encoded_settler) &
                         0x0000FF00);

                const int32_t freighted_index = static_cast<int32_t>(owner_player->settlers_freighted);
                memcpy(&owner_player->settlers[freighted_index], &encoded_settler, sizeof(encoded_settler));
                owner_player->settlers_freighted =
                        static_cast<int16_t>(owner_player->settlers_freighted + 1);
            }

            const uint8_t last_pop_index =
                    static_cast<uint8_t>(static_cast<uint8_t>(source_colony->n_pops) - 1);
            source_colony->n_pops = static_cast<int8_t>(last_pop_index);
            *pop_slot = source_colony->pop[last_pop_index];
        }
    }
}