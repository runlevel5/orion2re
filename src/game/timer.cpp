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
        const uint32_t target_ticks = (uint32_t)ticks * 55;

        while ((uint32_t)Get_Time_Since_Mark_() < target_ticks) {
        }
    }
}