#pragma once
#include <cstdint>

namespace INITSHIP {
    void __cdecl Init_Ship_Designs_(int16_t player_idx);
    int16_t __cdecl Best_Ship_Fuel_(int16_t player_idx);

    void __cdecl Update_Player_Ship_Designs_(int16_t player_idx);

    void __cdecl Update_Player_Strategic_Ship_Designs_(int16_t player_idx);

    void __cdecl Update_Automatic_Design_Equipment_(int16_t player_idx);

    void __cdecl Update_Equipment_On_Ships_Being_Built_(int16_t player_idx);

    int16_t __cdecl Best_Warp_Drive_(int16_t player_idx);

    int16_t __cdecl Calc_Player_FTL_Speed_(int16_t player_idx);

    int16_t __cdecl Best_Armor_(int16_t player_idx);

    int16_t __cdecl Best_Computer_(int16_t player_idx);

    int16_t __cdecl Best_Ship_Shield_(int16_t player_idx);

    int16_t __cdecl Get_FTL_Speed_(int16_t player_idx, int16_t ftl_type);

    int16_t __cdecl Get_Structure_(int16_t hull_index, bool is_fort, int16_t armor_tech);

    inline void __cdecl Get_Design_Combat_Bonuses_(s_ship_design *design, int16_t* bonus1, int16_t* bonus2, int16_t* bonus3) {
        // ABI is weird when passed by value, so just call in assembly
        /*
        using Fn = void(__cdecl*)(s_ship_design, int16_t*, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0047ee30 })(design, bonus1, bonus2, bonus3);
        */
        __asm {
            mov eax, bonus3
            push eax
            mov eax, bonus2
            push eax
            mov eax, bonus1
            push eax

            sub esp, 0x64
            mov edi, esp
            mov esi, design
            mov ecx, 0x18
            rep movsd
            movsw
            movsb

            mov eax, 0x0047ee30
            call eax
            add esp, 0x70
        }
    }
}