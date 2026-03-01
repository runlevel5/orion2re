#include "pch.h"

namespace council {
    void Check_For_Council_Meeting_() {
        int16_t active_players = 0;
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].eliminated == 0) {
                active_players++;
            }
        }

        int16_t owned_stars = 0;
        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            if (MOX::_star[i].owner != -1) {
                owned_stars++;
            }
        }

        int32_t current_turn_offset = MOX::_stardate - 35000;
        if (current_turn_offset >= 25) {
            bool time_for_meeting = ((int16_t)diplodef::_next_council_meeting_turn <= (int16_t)current_turn_offset) ||
                                     (diplodef::_COUNCIL_FLAG == 0);

            if (time_for_meeting && active_players > 2 && owned_stars >= (MOX::_NUM_STARS / 2)) {
                Main_Council_Screen_();
                diplodef::_next_council_meeting_turn = (int16_t)MOX::_stardate + 30561;
            }
        }
    }
}