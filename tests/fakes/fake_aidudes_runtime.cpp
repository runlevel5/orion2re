#include "pch.h"

namespace {
    s_colony colonies[1];
    s_player players[MAX_PLAYERS];
    s_ship_data ships[2];
    s_leader_data leaders[1];
    s_star_data stars[1];
    int32_t ship_worths[MAX_SHIPS];
    int pre_import_calls;
    int8_t status_during_recalculation;
    int16_t pooled_officer;
}

namespace TEST_AIDUDES {
    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        memset(players, 0, sizeof(players));
        pre_import_calls = 0;

        colonies[0].owner = 0;
        colonies[0].n_pops = 1;
        colonies[0].food2_per_farmer = 2;
        colonies[0].max_farms = 1;
        colonies[0].food2_needed_for_our_empire = 2;
    }

    void Reset_Maintenance_Scrap_(bool add_replacement) {
        memset(players, 0, sizeof(players));
        memset(ships, 0, sizeof(ships));
        memset(leaders, 0, sizeof(leaders));
        memset(stars, 0, sizeof(stars));
        memset(ship_worths, 0, sizeof(ship_worths));
        memset(&MOX::_settings, 0, sizeof(MOX::_settings));

        status_during_recalculation = -1;
        pooled_officer = -1;
        MOX::_NUM_SHIPS = add_replacement ? 2 : 1;
        maintain::_ship_worths = &ship_worths;

        players[0].bc = -10;
        players[0].objectives = PLAYER_OBJECTIVE_HUMAN;
        stars[0].has_colony = 1;

        ships[0].owner = 0;
        ships[0].status = 0;
        ships[0].location = 0;
        ships[0].d.ship_type = SHIP_TYPE_COMBAT;
        ships[0].d.cost = 40;
        ships[0].d.size = 4;
        ships[0].d.shield_type = 4;
        ships[0].d.armor_type = 4;
        ships[0].d.computer_type = 4;
        ships[0].crew_experience = 100;
        ships[0].officer_index = 0;

        ships[1].owner = 0;
        ships[1].status = 0;
        ships[1].location = 0;
        ships[1].d.ship_type = SHIP_TYPE_COMBAT;
        ships[1].officer_index = -1;
        ship_worths[1] = 1;

        leaders[0].player_index = 0;
        leaders[0].type = LEADER_TYPE_SHIP;
        leaders[0].status = 1;
        leaders[0].location = 0;
    }

    int Pre_Import_Calls_() {
        return pre_import_calls;
    }

    int8_t Status_During_Recalculation_() {
        return status_during_recalculation;
    }

    int16_t Pooled_Officer_() {
        return pooled_officer;
    }
}

namespace MOX {
    s_colony* _colony = colonies;
    s_player* _player = players;
    s_ship_data* _ship = ships;
    s_leader_data* _leaders = leaders;
    s_star_data* _star = stars;
    int16_t _NUM_SHIPS;
    s_settings _settings;
}

namespace EVENTS {
    int16_t __cdecl Event_Check_Colony_Researching_(int16_t) {
        return 0;
    }
}

namespace COLCALC {
    void __cdecl Pre_Import_Computing_(s_colony* colony) {
        pre_import_calls++;
        colony->food2_per_farmer = 0;
        colony->max_farms = 0;

        if (pre_import_calls >= 3) {
            colony->food2_needed_for_our_empire = 0;
        }
    }

    int16_t __cdecl Colony_Pop_Prod_Produced_(const s_colony*, const uint32_t*, int16_t) {
        return 0;
    }

    void __cdecl Do_Player_Colony_Post_Production_(int16_t) {
        status_during_recalculation = MOX::_ship[0].status;
    }
}

namespace HAROLD {
    int16_t __cdecl XY_To_Star_Id_(int16_t, int16_t) {
        return 0;
    }
}

namespace MAINSCR {
    void __cdecl Update_Ship_Icons_In_Between_Turns_() {
    }
}

namespace MSG_ {
    void __cdecl Add_Msg_(s_MSG, int16_t) {
    }
}

namespace OFFICER {
    bool __cdecl Ships_In_Same_Place_(int16_t, int16_t) {
        return true;
    }

    void __cdecl Ai_Assign_Leader_To_Ship_(s_player*, s_leader_data* leader, s_ship_data* ship) {
        int16_t leader_idx = (int16_t)(leader - MOX::_leaders);
        int16_t ship_idx = (int16_t)(ship - MOX::_ship);

        MOX::_ship[leader->location].officer_index = -1;
        leader->location = ship_idx;
        leader->status = 1;
        ship->officer_index = leader_idx;
    }

    void __cdecl Move_Officer_To_Pool_(int16_t leader_idx) {
        s_leader_data* leader = &MOX::_leaders[leader_idx];
        pooled_officer = leader_idx;

        if (leader->location >= 0 && MOX::_ship[leader->location].officer_index == leader_idx) {
            MOX::_ship[leader->location].officer_index = -1;
        }

        leader->status = 0;
        leader->eta = 0;
        leader->location = -1;
    }
}

void Legacy_Qsort_(void*, size_t, size_t, legacy_qsort_compare) {
}