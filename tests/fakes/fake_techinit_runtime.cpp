#include "pch.h"

namespace {
    s_player players[MAX_PLAYERS];
}

namespace TEST_TECHINIT {
    void Reset_() {
        memset(players, 0, sizeof(players));
        memset(&MOX::_settings, 0, sizeof(MOX::_settings));
        MOX::_NUM_PLAYERS = 1;
    }
}

namespace MOX {
    s_player* _player = players;
    s_settings _settings = {};
    int16_t _NUM_PLAYERS = 1;
}

namespace COLCALC {
    void __cdecl Player_Gets_Tech_App_(s_player* player, int16_t tech_app_id) {
        player->tech_applications[tech_app_id] = TECH_RESEARCH_STATUS_RESEARCHED;
    }
}
