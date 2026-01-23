#pragma once

#include <cstdint>

namespace timer {
    inline uint32_t& _time_mark = 
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0057B1A8});

    /**
     * @address 0052b220
     */
    void __cdecl Mark_Time_();

    inline void __cdecl Release_Time_(int16_t ticks) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0052b230 })(ticks);
    }
}