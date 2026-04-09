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

    inline int __cdecl Net_Is_Sending_() {
        using Fn = int(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00534C30});
        return fn();
    }

    inline int __cdecl Net_Join_Game_(const char* game_name, const char* password, const char* player_name) {
        using Fn = int(__cdecl*)(const char*, const char*, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005350F0});
        return fn(game_name, password, player_name);
    }

    inline void __cdecl Set_Null_Modem_Slave_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005354D0});
        fn();
    }

    inline int16_t __cdecl Net_Host_ID_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535510});
        return fn();
    }
}