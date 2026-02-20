#include <cstring>
#include "deps.h"
#include "aibuild.h"

namespace AIBUILD {
    int16_t __cdecl Refit_Cost_(int16_t player_idx, s_ship_design* old_design, s_ship_design* new_design) {
        // Minimum cost is half of the hull's available space
        int32_t min_cost = (int32_t)TECHDATA::_hull_data[(uint8_t)new_design->size].hull_space / 2;
        
        // Base refit cost is twice the difference in design costs
        int32_t refit_cost = ((int32_t)new_design->cost - (int32_t)old_design->cost) * 2;

        // Apply government-based cost reductions to the floor price
        DESIGN::Cost_Reduction_For_Govt_Type_(player_idx, &min_cost);

        // Result cannot be lower than the floor price
        if (refit_cost < min_cost) {
            refit_cost = min_cost;
        }

        return (int16_t)refit_cost;
    }

    void __cdecl Compute_Empire_Building_Needs_() {
        MOX::Mox_Update_();

        memset(_player_total_strength_vs_ships, 0, sizeof(_player_total_strength_vs_ships));
        memset(_player_ship_sizes, 0, sizeof(_player_ship_sizes));
        memset(_player_total_strength_vs_colonies, 0, sizeof(_player_total_strength_vs_colonies));
        memset(_player_n_colony_ships, 0, sizeof(_player_n_colony_ships));
        memset(_player_n_transports, 0, sizeof(_player_n_transports));

        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];

            if ((ship->status < 5 || ship->status == 6) && (uint8_t)ship->owner < 8) {
                int8_t owner = (int8_t)ship->owner;

                if (ship->d.ship_type == SHIP_TYPE_COMBAT) {
                    int strength = (int)Mean_Ship_Strength_(ship);
                    _player_total_strength_vs_ships[owner * 3] += strength;

                    if (ship->status != 6) {
                        _player_total_strength_vs_ships[owner * 3 + 1] += strength;
                    }

                    _player_ship_sizes[owner][(int)ship->d.size]++;
                    _player_ship_sizes[owner][13] += (int)_player_ship_size_table[(int)ship->d.size];
                    _player_ship_sizes[owner][11] += ((int)ship->d.size + 1);
                    _player_ship_sizes[owner][10]++;

                    if (ship->status != 6) {
                        _player_ship_sizes[owner][12] += ((int)ship->d.size + 1);
                    }
                } else if (ship->d.ship_type == SHIP_TYPE_COLONY) {
                    _player_n_colony_ships[owner][0]++;
                    if (ship->status == 1 || ship->status == 2) {
                        _player_n_colony_ships[owner][2]++;
                    }
                } else if (ship->d.ship_type == SHIP_TYPE_TRANSPORT) {
                    _player_n_transports[owner][0]++;
                }
            }
        }

        MOX::Mox_Update_();
        memset(_player_n_spies, 0, sizeof(_player_n_spies));

        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t owner = 0; owner < MOX::_NUM_PLAYERS; ++owner) {
                for (int16_t target = 0; target < MOX::_NUM_PLAYERS; ++target) {
                    int16_t spy_count = BILL::Get_Their_Spy_Number_(owner, target);
                    _player_n_spies[owner][0] += (int8_t)spy_count;
                }
            }
        }

        MOX::Mox_Update_();
        for (int16_t player = 0; player < MOX::_NUM_PLAYERS; ++player) {
            Compute_Needs_(player);
        }
    }
}