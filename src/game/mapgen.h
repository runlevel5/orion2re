#pragma once
#include <cstdint>

namespace MAPGEN {
    inline uint16_t*& _fill_msg_bitmap = *reinterpret_cast<uint16_t**>(std::uintptr_t{0x005A4B20});
    inline char (&_fill_msg)[32] = *reinterpret_cast<char (*)[32]>(std::uintptr_t{0x005A4B30});
    inline uint16_t& _fill_msg_ctr = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A4B54});

    inline void __cdecl Universe_Generation_(int16_t arg_1c, int32_t arg_20, int16_t arg_18) {
        using Fn = void(__cdecl*)(int16_t, int32_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004B7120})(arg_1c, arg_20, arg_18);
    }

    inline void __cdecl Enforce_Planet_Max_(int limit) {
        using Fn = void(__cdecl*)(int);
        reinterpret_cast<Fn>(std::uintptr_t{0x004B57E0})(limit);
    }

    inline void __cdecl Set_Black_Hole_Visited_Flags_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B6DB0})();
    }

    inline void __cdecl Generate_Wormhole_Links_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B69F0})();
    }

    inline void __cdecl Assign_Marooned_Heroes_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B56B0})();
    }
}