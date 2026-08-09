#include "pch.h"

namespace {
    s_colony colonies[1];
    s_player players[MAX_PLAYERS];
    int pre_import_calls;
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

    int Pre_Import_Calls_() {
        return pre_import_calls;
    }
}

namespace MOX {
    s_colony* _colony = colonies;
    s_player* _player = players;
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
}

void Legacy_Qsort_(void*, size_t, size_t, legacy_qsort_compare) {
}