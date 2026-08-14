#include "pch.h"

namespace test_eric_runtime {
    s_player players[2];
    s_colony colonies[1];
    s_planet_data planets[1];
    s_ship_data ships[MAX_SHIPS];
    s_leader_data leaders[MAX_LEADERS];
    int16_t last_help_string;
    char help_string[1];

    void Reset_() {
        memset(players, 0, sizeof(players));
        memset(colonies, 0, sizeof(colonies));
        memset(planets, 0, sizeof(planets));
        memset(ships, 0, sizeof(ships));
        memset(leaders, 0, sizeof(leaders));

        players[0].traits[TRAIT_TELEPATHIC] = 1;
        colonies[0].owner = 1;
        colonies[0].planet = 0;
        planets[0].star_index = 3;

        ships[0].owner = 0;
        ships[0].status = 0;
        ships[0].location = 3;
        ships[0].d.size = SHIP_SIZE_LARGE;

        leaders[0].player_index = 1;
        leaders[0].status = 1;
        leaders[0].eta = 0;
        leaders[0].type = LEADER_TYPE_COLONY;
        leaders[0].location = 3;

        last_help_string = -1;
    }

    s_leader_data* Defender_Leader_() {
        return &leaders[0];
    }

    int16_t Last_Help_String_() {
        return last_help_string;
    }
}

namespace MOX {
    s_player* _player = test_eric_runtime::players;
    s_colony* _colony = test_eric_runtime::colonies;
    s_planet_data* _planet = test_eric_runtime::planets;
    s_ship_data* _ship = test_eric_runtime::ships;
    s_leader_data* _leaders = test_eric_runtime::leaders;
}

namespace OFFICER {
    bool __cdecl Officer_Has_General_Skill_(int16_t, int16_t leader_idx, int16_t skill_idx) {
        uint32_t skill_mask = 0;
        if (skill_idx == 0) {
            skill_mask = LEADER_GENERAL_SKILL_ASSASSIN;
        } else if (skill_idx == 48) {
            skill_mask = LEADER_GENERAL_SKILL_TELEPATH;
        } else if (skill_idx == 49) {
            skill_mask = LEADER_GENERAL_SKILL_TELEPATH2;
        }
        return (MOX::_leaders[leader_idx].general_skills & skill_mask) != 0;
    }
}

namespace ESTRINGS {
    char* __cdecl E_Strings_(int16_t string_id) {
        test_eric_runtime::last_help_string = string_id;
        return test_eric_runtime::help_string;
    }
}

namespace GENDRAW {
    void __cdecl Help_(const char*) {
    }
}