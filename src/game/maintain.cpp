#include "deps.h"
#include "maintain.h"

#include <cstring>

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
}