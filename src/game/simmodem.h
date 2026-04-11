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

    inline bool __cdecl Modem_Dial_Number_(char* phone_number) {
        using Fn = bool(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005355C0});
        return fn(phone_number);
    }

    inline void __cdecl Get_Modem_Error_(char* error_buffer) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535660});
        fn(error_buffer);
    }
}