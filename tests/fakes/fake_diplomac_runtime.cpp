#include "pch.h"

namespace {
    s_player players[MAX_PLAYERS];
}

namespace TEST_DIPLOMAC {
    void Reset_() {
        memset(players, 0, sizeof(players));
        MOX::_NUM_PLAYERS = 2;
    }
}

namespace MOX {
    s_player* _player = players;
    int16_t _NUM_PLAYERS = 2;
}

namespace diplodef {
    int16_t _next_council_meeting_turn;
    int16_t _COUNCIL_FLAG;
    int16_t _council_winner;
}

namespace npcdiplo {
    void __cdecl Clear_Last_Proposal_Turn_(int16_t, int16_t) {
    }
}