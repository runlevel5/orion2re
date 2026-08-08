#include "pch.h"

#include "game/techdata.h"

namespace {
    s_player players[MAX_PLAYERS];
}

namespace TEST_TECHINIT {
    void Reset_() {
        memset(players, 0, sizeof(players));
        memset(&MOX::_settings, 0, sizeof(MOX::_settings));
        memset(
            TECHDATA::_technology_applications,
            0,
            sizeof(TECHDATA::_technology_applications));
    }
}

namespace MOX {
    s_player* _player = players;
    s_settings _settings = {};
}

namespace TECHDATA {
    s_tech_app_data _technology_applications[TECH_APP_COUNT];
}
