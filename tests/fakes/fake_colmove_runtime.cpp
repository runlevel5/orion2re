#include "pch.h"

namespace {
    s_colony colonies[MAX_COLONIES];
    char help_message[] = "help";
    char planet_name[] = "Planet";
}

namespace TEST_COLMOVE {
    int16_t help_call_count;
    int16_t settle_call_count;

    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        memset(&MOX::_settings, 0, sizeof(MOX::_settings));
        help_call_count = 0;
        settle_call_count = 0;
        COLMOVE::_cluster_colony_n = -1;
    }
}

namespace MOX {
    s_colony* _colony = colonies;
    s_settings _settings;
}

namespace COLONY {
    uint8_t _full_draw;

    int16_t __cdecl Pop_To_Pop_State_(int16_t, int16_t) {
        return 0;
    }

    int16_t __cdecl Sum_Colonists_(
            int16_t colony_idx,
            int16_t job,
            int16_t,
            int16_t,
            int16_t,
            int16_t) {
        int16_t count = 0;
        const s_colony& colony = MOX::_colony[colony_idx];

        for (int16_t i = 0; i < colony.n_pops; ++i) {
            const uint32_t pop = colony.pop[i];
            if (POP::Is_Assigned(pop) && (int16_t)POP::Get_Prof(pop) == job) {
                ++count;
            }
        }

        return count;
    }

    s_animation_header* __cdecl C_Anims_(int16_t) {
        return nullptr;
    }

    s_animation_header* __cdecl Colony_Pop_Anim_(int16_t, int16_t) {
        return nullptr;
    }

    char* __cdecl Pop_State_String_(int16_t) {
        return help_message;
    }

    char* __cdecl Conquered_String_(bool) {
        return help_message;
    }

    char* __cdecl Pop_Race_String_(int16_t, int16_t) {
        return help_message;
    }

    char* __cdecl Job_String_(int16_t) {
        return help_message;
    }

    void __cdecl Reset_Mouse_Picture_() {
    }

    void __cdecl Clear_Mouse_Picture_() {
    }

    void __cdecl Col_Calc_Wrapper_(int16_t) {
    }

    void __cdecl Reset_Field_Mode_() {
    }
}

namespace ESTRINGS {
    char* __cdecl E_Strings_(int16_t) {
        return help_message;
    }
}

namespace GENDRAW {
    void __cdecl Help_(const char*) {
        ++TEST_COLMOVE::help_call_count;
    }

    int16_t __cdecl Do_Confirmation_Box_(const char*) {
        return 1;
    }
}

namespace ERIC {
    void __cdecl String_Builder2_(char*, int32_t, int16_t, const char*, ...) {
    }
}

namespace HAROLD {
    int8_t __cdecl Was_Mouse_List_Saved_() {
        return 0;
    }
}

namespace COLCALC {
    void __cdecl Colony_Calculation_(int16_t) {
    }
}

namespace HACCESS {
    char* __cdecl Get_Planet_Name_(int16_t) {
        return planet_name;
    }
}

namespace SETTLER {
    void __cdecl Pop_Tries_To_Settle_(
            s_colony*,
            int16_t,
            s_colony*,
            s_settler_settle_result* result) {
        memset(result, 0, sizeof(*result));
        result->can_settle = 1;
    }

    void __cdecl Settle_Pop_(s_colony* source_colony, int16_t pop_idx, s_colony* destination_colony) {
        destination_colony->pop[destination_colony->n_pops] = source_colony->pop[pop_idx];
        ++destination_colony->n_pops;

        for (int16_t i = pop_idx; i + 1 < source_colony->n_pops; ++i) {
            source_colony->pop[i] = source_colony->pop[i + 1];
        }

        --source_colony->n_pops;
        ++TEST_COLMOVE::settle_call_count;
    }
}

namespace TEXTBOX {
    void __cdecl Text_Box_(const char*, const char*) {
    }
}