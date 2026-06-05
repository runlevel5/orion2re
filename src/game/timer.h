#pragma once

#include <cstdint>

namespace timer {
    extern uint32_t _time_mark;

    /** Stores the current tick count as the baseline for later elapsed-time queries. */
    void __cdecl Mark_Time_();

    /** Returns the number of milliseconds elapsed since the last call to Mark_Time_(). */
    int32_t __cdecl Get_Time_Since_Mark_();

    /** Busy-waits until the requested number of timer ticks has elapsed since the last mark.
     *  @param ticks Number of 55 ms ticks to wait.
     */
    void __cdecl Release_Time_(int32_t ticks);
}