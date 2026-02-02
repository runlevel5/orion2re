#pragma once
#include <cstdint>

namespace SCORE {
    inline void __cdecl Hall_Of_Fame_Screen_(int16_t value) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004e3880})(value);
    }
}