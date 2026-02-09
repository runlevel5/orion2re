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

    bool __cdecl Fleet_Movement_Icon_Selected_(int16_t icon_idx);

    inline void __cdecl Do_Get_Planet_Name_(int16_t planet_idx, char* buffer, uint8_t param_3) {
        using Fn = void(__cdecl*)(int16_t, char*, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049a610});
        fn(planet_idx, buffer, param_3);
    }

    char* __cdecl Get_Planet_Name_(int16_t planet_idx);

    int16_t __cdecl Last_Colony_Selected_(int16_t star_idx, int16_t player_idx);

    inline void __cdecl Set_All_Star_Relocations_(int16_t star_idx, int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(0x0049AEE0)(star_idx, player_idx);
    }

    inline int16_t __cdecl Fleet_Movement_Ship_Id_(int16_t unk) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(0x0049A7D0)(unk);
    }

    inline int16_t __cdecl Ship_Stack_From_Ship_Id_(int16_t id) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(0x0049B490)(id);
    }

    inline uint8_t __cdecl Get_Stack_Start_From_Saved_Ship_Array_() {
        using Fn = uint8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x0049A8B0})();
    }

    inline int16_t __cdecl Get_Fleet_Box_Selected_Ship_Ids_(int16_t* out, int max, int filter) {
        using Fn = int16_t(__cdecl*)(int16_t*, int, int);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0049A7F0})(out, max, filter);
    }

    inline void __cdecl Clear_Fleet_Box_Selected_Ship_Status_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0049A400})();
    }

    inline int16_t __cdecl Ship_Stack_Owner_(int16_t ship_stack_id) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0049b530})(ship_stack_id);
    }

    inline int16_t __cdecl Ship_Stack_From_Ship_Icon_(int16_t ship_icon_id) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0049b430})(ship_icon_id);
    }

    inline int16_t __cdecl Weapons_Index_From_Tech_App_(int16_t tech_app) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049B650});
        return fn(tech_app);
    }

    inline char* __cdecl Race_Name_String_(int16_t owner) {
        using Fn = char*(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049ac80});
        return fn(owner);
    }
}