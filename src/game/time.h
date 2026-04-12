#pragma once

namespace time_ {
    inline uint32_t& _time =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005897F0});

    inline uint32_t& _time2 =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00589A10});

    inline void __cdecl time_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00534A70});
        fn();
    }
}