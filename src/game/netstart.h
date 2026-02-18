#pragma once
#include <cstdint>

namespace NETSTART {
    inline uint8_t __cdecl Start_Net_Screen_() {
        using Fn = uint8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x004c36b0})();
    }
    inline uint8_t __cdecl Join_Net_Screen_() {
        using Fn = uint8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x004c27a0})();
    }
    inline uint8_t __cdecl Load_Net_Screen_() {
        using Fn = uint8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x004c2fb0})();
    }

    inline char* __cdecl Load_Netstart_Strings_(char* data) {
        using Fn = char*(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004C2670});
        return fn(data);
    }
    inline int16_t __cdecl Request_Race_(uint8_t race_idx) {
        using Fn = int16_t(__cdecl*)(uint8_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004C3E70})(race_idx);
    }
    inline void __cdecl Unrequest_Race_(uint8_t race_idx) {
        using Fn = void(__cdecl*)(uint8_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004C3F30})(race_idx);
    }
}