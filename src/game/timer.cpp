#include "pch.h"

namespace timer {
    void __cdecl Mark_Time_() {
        _time_mark = GetTickCount();
    }

    int32_t __cdecl Get_Time_Since_Mark_() {
        return static_cast<int32_t>(GetTickCount() - _time_mark);
    }

    void __cdecl Release_Time_(int16_t ticks) {
        const uint32_t target_ticks = static_cast<uint32_t>(ticks) * 55;

        while (static_cast<uint32_t>(Get_Time_Since_Mark_()) < target_ticks) {
            // Busy wait loop
        }
    }
}