#pragma once

#include <cstdint>

namespace timer {
    inline uint32_t& _time_mark = 
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0057B1A8});

    /**
     * @address 0052b220
     */
    void __cdecl Mark_Time_();

    /**
     * @address 0052b270
     */
    int32_t __cdecl Get_Time_Since_Mark_();

    /**
     * @address 0052b230
     */
    void __cdecl Release_Time_(int16_t ticks);
}