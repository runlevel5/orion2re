#include "pch.h"

namespace {
    s_player players[MAX_PLAYERS];
}

namespace TEST_TECHINIT {
    void Reset_() {
        memset(players, 0, sizeof(players));
        memset(&MOX::_settings, 0, sizeof(MOX::_settings));
    }
}

namespace MOX {
    s_player* _player = players;
    s_settings _settings = {};
}
