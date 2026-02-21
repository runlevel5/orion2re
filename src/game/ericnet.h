#pragma once
#include <cstdint>

namespace ERICNET {
    inline void __cdecl Init_Homeworld_Colony2_(int16_t colony_idx, int16_t player_idx, uint8_t flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00408340});
        fn(colony_idx, player_idx, flag);
    }

    void __cdecl Init_Homeworld_Colony_(int16_t colony_idx, int16_t player_idx);

    int __cdecl Mox_Play_Sound_(int16_t sound_id);

    void __cdecl Init_Non_Homeworld_Colony_(int16_t colony_idx, int16_t player_idx);
    
    void __cdecl Do_All_Ships_XP_Check_();

    inline void __cdecl Do_Academy_At_Location_Check_(int16_t ship_index) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004092C0});
        fn(ship_index);
    }

    inline void __cdecl Calc_Ship_Level_(int16_t ship_index) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00409250});
        fn(ship_index);
    }

    void __cdecl Do_XP_For_Ship_(int16_t ship_index, int16_t xp_gain);
}