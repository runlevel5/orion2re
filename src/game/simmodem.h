#pragma once
#include <cstdint>

namespace simmodem {
    inline void __cdecl Modem_Reset_(char* reset_cmd) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005357a0});
        fn(reset_cmd);
    }

    inline int8_t __cdecl Modem_Answer_() {
        using Fn = int8_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535590});
        return fn();
    }

    inline int32_t __cdecl Modem_Connected_() {
        using Fn = int32_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535780});
        return fn();
    }
}