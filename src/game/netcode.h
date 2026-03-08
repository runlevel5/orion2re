#pragma once
#include <cstdint>

namespace netcode {
    inline int __cdecl Net_Get_Game_Count_() {
        using Fn = int(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005350B0});
        return fn();
    }

    inline char* __cdecl Net_Get_Game_Name_(int index) {
        using Fn = char*(__cdecl*)(int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005350C0});
        return fn(index);
    }

    inline void __cdecl Net_Uninit_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00534B40});
        fn();
    }
}