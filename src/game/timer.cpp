#include "deps.h"

namespace timer {
    /**
     * @address 0052b220
     */
    void __cdecl Mark_Time_() {
        _time_mark = GetTickCount();
    }

    /**
     * @address 0052b270
     */
    int32_t __cdecl Get_Time_Since_Mark_() {
        return static_cast<int32_t>(GetTickCount() - _time_mark);
    }

    /**
     * @address 0052b230
     */
    void __cdecl Release_Time_(int16_t ticks) {
        const uint32_t target_ticks = static_cast<uint32_t>(ticks) * 55;

        while (static_cast<uint32_t>(Get_Time_Since_Mark_()) < target_ticks) {
            // Busy wait loop
        }
    }
}