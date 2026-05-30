#pragma once

#include <cstdint>

namespace timer {
    extern uint32_t _time_mark;

    void __cdecl Mark_Time_();

    int32_t __cdecl Get_Time_Since_Mark_();

    void __cdecl Release_Time_(int32_t t);
}