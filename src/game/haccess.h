#pragma once
#include "deps.h"

namespace HACCESS {
    inline void __cdecl Race_Name_(int16_t race_idx, char* buffer) {
        using Fn = void(__cdecl*)(int16_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049acd0});
        fn(race_idx, buffer);
    }

    void __cdecl Set_Colony_Auto_Select_For_Star_(int16_t star_idx, int16_t player_idx, int16_t* out_planet_idx);

    bool __cdecl Star_Has_Relocation_(int16_t star_idx, int16_t player_idx);

    int16_t __cdecl Relocation_(int16_t star_idx, int16_t player_idx);

    int16_t __cdecl Ship_Owner_(int16_t ship_idx);
    int16_t __cdecl Ship_Stack_From_Saved_Array_();
    int16_t __cdecl Node_Ptr_From_Ship_Stack_(int16_t index);

    int16_t __cdecl Ship_Icon_Id_From_Node_Ptr_(int16_t node_ptr);

    int16_t __cdecl Ship_Icon_Owner_(int16_t icon_id);

    inline char* __cdecl Design_Name_(int16_t player, int16_t design_idx) {
        using Fn = char*(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049A2B0});
        return fn(player, design_idx);
    }

    inline int16_t __cdecl Design_Weapon_(int16_t player_idx, int16_t design_idx, int16_t weapon_slot) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t);
        return reinterpret_cast<Fn>(0x0049A4D0)(player_idx, design_idx, weapon_slot);
    }

    inline int16_t __cdecl Design_Weapon_Count_(int16_t player_idx, int16_t design_idx, int16_t weapon_slot) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t);
        return reinterpret_cast<Fn>(0x0049A550)(player_idx, design_idx, weapon_slot);
    }

    inline int16_t __cdecl Design_Weapon_Specials_(int16_t player_idx, int16_t design_idx, int16_t weapon_slot) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t);
        return reinterpret_cast<Fn>(0x0049A5D0)(player_idx, design_idx, weapon_slot);
    }

    inline int8_t __cdecl Design_Weapon_Firing_Arc_(int16_t player_idx, int16_t design_idx, int16_t weapon_slot) {
        using Fn = int8_t(__cdecl*)(int16_t, int16_t, int16_t);
        return reinterpret_cast<Fn>(0x0049A590)(player_idx, design_idx, weapon_slot);
    }

    inline int8_t __cdecl Design_Weapon_Ammo_(int16_t player_idx, int16_t design_idx, int16_t weapon_slot) {
        using Fn = int8_t(__cdecl*)(int16_t, int16_t, int16_t);
        return reinterpret_cast<Fn>(0x0049A510)(player_idx, design_idx, weapon_slot);
    }

    inline int16_t __cdecl Design_Picture_Num_(int16_t player_idx, int16_t design_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        return reinterpret_cast<Fn>(0x0049A450)(player_idx, design_idx);
    }

    inline void __cdecl Do_Get_Planet_Name_(int16_t planet_idx, char* buffer, uint8_t param_3) {
        using Fn = void(__cdecl*)(int16_t, char*, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049a610});
        fn(planet_idx, buffer, param_3);
    }

    char* __cdecl Get_Planet_Name_(int16_t planet_idx);
}