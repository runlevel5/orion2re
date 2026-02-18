#include "deps.h"

namespace antarans {
    int32_t __cdecl Delta_Stardate_() {
        return MOX::_stardate - 35000;
    }

    int16_t __cdecl Delay_() {
        int8_t level = static_cast<int8_t>(MOX::_settings.starting_civilization_level);

        switch (level) {
        case 0:
            return 200;
        case 1:
            return 100;
        case 2:
        default:
            return 0;
        }
    }

    // Suggested name: Antarans_Defensive_Resource_Bonus
    int16_t __cdecl Antarans_Defensive_Resource_Bonus_() {
        if (MOX::_settings.game_difficulty == 3) {
            return 150;
        }

        if (MOX::_settings.game_difficulty == 4) {
            return 200;
        }

        return 100;
    }
}