#include "pch.h"

namespace timer {
    uint32_t _time_mark;

    void __cdecl Mark_Time_() {
        _time_mark = (uint32_t)SDL_GetTicks();
    }

    int32_t __cdecl Get_Time_Since_Mark_() {
        return (int32_t)((uint32_t)SDL_GetTicks() - _time_mark);
    }

    void __cdecl Release_Time_(int32_t ticks) {
        const uint32_t target_ms = (uint32_t)(ticks) * 55;
        const uint32_t elapsed_ms = (uint32_t)(Get_Time_Since_Mark_());

        if (elapsed_ms < target_ms) {
            SDL_Delay(target_ms - elapsed_ms);
        }
        else {
            SDL_Delay(0);
        }
    }
}