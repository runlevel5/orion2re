#include "deps.h"

namespace COLCALC {
    void __cdecl Twiddle_Initial_Homeworlds_() {
        int min_command_points = 30000;

        CONTACT::Compute_Contacts_();
        MOX::Mox_Update_();

        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            Twiddle_Initial_Homeworld_(&MOX::_colony[i]);
        }

        SHIPMOVE::Update_All_Ship_Ranges_();
        All_Colony_Calculations_();

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            s_player* player = &MOX::_player[i];
            int star_idx = MOX::_planet[player->home_planet_id].star_index;
            MOX::_star[star_idx].colonize_player = (uint8_t)i;

            if (player->command_points < (int16_t)min_command_points) {
                min_command_points = player->command_points;
            }
        }

        if (MOX::_settings.starting_civilization_level == 2) {
            int starting_ship_points = (min_command_points - 2) / 2;

            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                s_player* player = &MOX::_player[i];
                MOX::Mox_Update_();

                int star_idx = MOX::_planet[player->home_planet_id].star_index;
                MOX::_star[star_idx].colonize_player = (uint8_t)i;

                player->bc = (player->traits[0] * 100 + 200);

                for (int s = 0; s < MOX::_NUM_STARS; ++s) {
                    s_star_data* star = &MOX::_star[s];
                    if (star->system_special < SYSTEM_SPECIAL_SPLINTER_COLONY ||
                        star->system_special > SYSTEM_SPECIAL_MAROONED_HERO) {

                        if (SHIPMOVE::Star_In_Normal_Range_Of_Player_(star, (int16_t)i)) {
                            star->visited |= (1 << i);

                            if (star->system_special > SYSTEM_SPECIAL_STABLE_WORMHOLE &&
                                star->system_special < SYSTEM_SPECIAL_GOLD_DEPOSITS) {
                                if (Monster_At_Star_(s) == 0) {
                                    star->system_special = SYSTEM_SPECIAL_NO_SPECIAL;
                                }
                            }
                        }
                    }
                }

                int freighter_pool = 10;
                if (player->surplus_freighters < 0) freighter_pool -= player->surplus_freighters;
                if (player->surplus_food < 0) freighter_pool -= player->surplus_food;
                player->n_freighters = (int16_t)((freighter_pool + 4) / 5) * 5;

                int first_colony = Get_First_Player_Colony_Idx(i);
                int pool_remaining = min_command_points;

                int max_design_idx = 0;
                if (starting_ship_points >= 0) {
                    max_design_idx = starting_ship_points;
                    if (starting_ship_points >= 4) {
                        if (player->tech_applications[TECH_APP_TITAN_CONSTRUCTION] == 3) {
                            max_design_idx = 4;
                        } else {
                            max_design_idx = 3;
                        }
                    }
                }

                while (pool_remaining != 0) {
                    MOX::Mox_Update_();
                    int ship_design = max_design_idx;
                    if (pool_remaining - 1 < ship_design) {
                        ship_design = pool_remaining - 1;
                    }

                    MOX::_ship[MOX::_NUM_SHIPS].owner = (int8_t)i;
                    INITSHIP::Update_Player_Ship_Designs_((int16_t)i);
                    COLBLDG::Create_Ship_((int16_t)i, (int16_t)ship_design, (int16_t)first_colony);

                    pool_remaining -= (ship_design + 1);
                }
            }
            All_Colony_Calculations_();
        }
    }

    void __cdecl Compute_All_Star_Colony_Stuff_() {
        int32_t num_stars = static_cast<int32_t>(MOX::_NUM_STARS);
        s_star_data* star_ptr = MOX::_star;
        s_star_data* end_ptr = &MOX::_star[num_stars];

        if (star_ptr < end_ptr) {
            do {
                Compute_Star_Colony_Stuff_(star_ptr);
                star_ptr++;
            } while (star_ptr < end_ptr);
        }
    }

    void __cdecl Compute_Star_Colony_Stuff_(s_star_data* col) {
        uint8_t old_interdictor_state = col->has_warp_field_interdictor;

        col->has_colony = 0;
        col->has_warp_field_interdictor = 0;
        col->has_tachyon = 0;
        col->has_subspace = 0;
        col->has_stargate = 0;
        col->has_jumpgate = 0;
        col->has_artemis_net = 0;
        col->has_dimensional_portal = 0;

        for (int16_t i = 0; i < 5; ++i) {
            int16_t p_idx = col->planet_index[i];
            if (p_idx == -1) continue;

            int16_t c_idx = MOX::_planet[p_idx].colony_index;
            if (c_idx == -1) continue;

            s_colony* colony = &MOX::_colony[c_idx];
            uint8_t owner = colony->owner;
            uint8_t owner_bit = 1 << owner;

            col->has_colony |= owner_bit;

            if (colony->buildings[BUILDING_WARP_INTERDICTOR] != 0) {
                col->has_warp_field_interdictor |= owner_bit;
            }

            if (colony->buildings[BUILDING_STAR_BASE] != 0 ||
                colony->buildings[BUILDING_BATTLESTATION] != 0 ||
                colony->buildings[BUILDING_STAR_FORTRESS] != 0) {

                if (MOX::_player[owner].tech_applications[TECH_APP_TACHYON_COMMUNICATIONS] == 3) {
                    col->has_tachyon |= owner_bit;
                }
                if (MOX::_player[owner].tech_applications[TECH_APP_SUB_SPACE_COMMUNICATIONS] == 3) {
                    col->has_subspace |= owner_bit;
                }
            }

            if (colony->buildings[BUILDING_ARTEMIS_SYSTEM_NET] != 0) {
                col->has_artemis_net |= owner_bit;
            }

            if (colony->buildings[BUILDING_DIMENSIONAL_PORTAL] != 0) {
                col->has_dimensional_portal |= owner_bit;
            }

            if (MOX::_player[owner].tech_applications[TECH_APP_JUMP_GATE] == 3) {
                col->has_jumpgate |= owner_bit;
            }

            if (MOX::_player[owner].tech_applications[TECH_APP_STAR_GATE] == 3) {
                col->has_stargate |= owner_bit;
            }
        }

        if ((col->has_warp_field_interdictor != 0) != (old_interdictor_state != 0)) {
            No_Op1_();
        }
    }

    void __cdecl Init_Surrenders_() {
        for (int i = 0; i < (int)MOX::_NUM_STARS; ++i) {
            for (int j = 0; j < (int)MOX::_NUM_PLAYERS; ++j) {
                MOX::_star[i].surrender_to[j] = -1;
            }
        }

        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            MOX::_player[i].surrender_to = -1;
        }
    }

    int16_t __cdecl Player_N_Turns_Until_Research_Complete_(s_player* plr) {
        uint8_t field_idx = static_cast<uint8_t>(plr->current_research_field);

        if (plr->tech_fields[field_idx] == 3) {
            return 0;
        }

        int32_t produced = (int32_t)plr->research_produced;
        int32_t cost = Player_Research_Cost_(plr, (uint16_t)field_idx);
        int32_t accumulated = plr->research_accumulated;

        if (produced == 0 && accumulated <= cost) {
            return -1;
        }

        int16_t turns = 0;

        while (accumulated <= cost) {
            turns++;
            accumulated += produced;
        }

        int32_t total_chance = 0;
        do {
            turns++;
            accumulated += produced;
            int32_t current_turn_chance = Chance_For_Research_Breakthrough_Aux_(accumulated, produced, cost);
            total_chance += current_turn_chance;
        } while (total_chance < 100);

        return turns;
    }

    int32_t __cdecl Chance_For_Research_Breakthrough_(s_player* plr) {
        int32_t cost = Player_Research_Cost_(plr, static_cast<uint16_t>(static_cast<uint8_t>(plr->current_research_field)));

        int32_t produced = static_cast<int32_t>(plr->research_produced);
        int32_t accumulated_total = plr->research_accumulated + produced;

        return Chance_For_Research_Breakthrough_Aux_(accumulated_total, produced, cost);
    }

    int32_t __cdecl Chance_For_Research_Breakthrough_Aux_(int32_t accumulated_total, int32_t produced, int32_t cost) {
        int32_t chance = 0;

        if (cost > 0 && cost < accumulated_total) {
            chance = ((accumulated_total - cost) * 100) / cost;

            if (chance > 100) {
                return 100;
            }

            if (chance == 0) {
                chance = 1;
            }
        }

        return chance;
    }
}