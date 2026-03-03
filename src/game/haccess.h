#pragma once
#include "deps.h"

namespace HACCESS {
    inline const char* const s__005510f0 =
        reinterpret_cast<const char*>(std::uintptr_t{0x005510F0});

    inline const char* const s_format_star_roman = reinterpret_cast<const char*>(0x00558014); // "%s %s"
    inline const char* const s_I = reinterpret_cast<const char*>(0x0055801C); // "I"

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

    char* __cdecl Design_Name_(int16_t player_idx, int16_t design);

    int16_t __cdecl Design_Weapon_(int16_t player_idx, int16_t design_idx, int16_t weapon_idx);

    int16_t __cdecl Design_Weapon_Count_(int16_t player_idx, int16_t design_idx, int16_t weapon_idx);

    int16_t __cdecl Design_Weapon_Specials_(int16_t player_idx, int16_t design_idx, int16_t weapon_idx);

    int8_t __cdecl Design_Weapon_Firing_Arc_(int16_t player_idx, int16_t design_idx, int16_t weapon_idx);

    int8_t __cdecl Design_Weapon_Ammo_(int16_t player_idx, int16_t design_idx, int16_t weapon_idx);

    int8_t __cdecl Design_Picture_Num_(int16_t player_idx, int16_t design_idx);

    bool __cdecl Fleet_Movement_Icon_Selected_(int16_t icon_idx);

    void __cdecl Do_Get_Planet_Name_(int16_t planet_idx, char* buffer, uint8_t localized_flag);

    char* __cdecl Get_Planet_Name_(int16_t planet_idx);

    int16_t __cdecl Last_Colony_Selected_(int16_t star_idx, int16_t player_idx);

    void __cdecl Set_All_Star_Relocations_(int16_t target_star_idx, int16_t player_idx);

    int16_t __cdecl Fleet_Movement_Ship_Id_(int16_t icon_idx);

    int16_t __cdecl Ship_Stack_From_Ship_Id_(int16_t ship_id);

    uint8_t __cdecl Get_Stack_Start_From_Saved_Ship_Array_();

    int16_t __cdecl Get_Fleet_Box_Selected_Ship_Ids_(int16_t* out, int max, int filter);

    inline void __cdecl Clear_Fleet_Box_Selected_Ship_Status_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00403ee0})();
    }

    int16_t __cdecl Ship_Stack_Owner_(int16_t stack_idx);

    int16_t __cdecl Ship_Stack_From_Ship_Icon_(int16_t ship_icon_id);

    int16_t __cdecl Weapons_Index_From_Tech_App_(int16_t tech_app_id);

    char* __cdecl Race_Name_String_(int16_t race_index);

    bool __cdecl Player_Has_Colony_In_System_(int16_t player_idx, int16_t system_idx);

    void __cdecl Set_Relocation_(int16_t star_idx, int16_t player_idx, int16_t target_star_idx);

    int16_t __cdecl N_Colonies_At_Star_(int16_t star_index, int16_t player_index);

    bool __cdecl Is_Combat_Ship_(int16_t ship_index);

    char* __cdecl Get_Planet_Name_No_Prime_(int16_t planet_idx);

    int16_t __cdecl Ship_Stack_X_(int16_t stack_idx);
    int16_t __cdecl Ship_Stack_Y_(int16_t node_idx);

    int16_t __cdecl Ship_Icon_Star_Id_(int16_t ship_icon_idx);

    inline int16_t __cdecl Ship_Icon_Ship_Id_(int16_t icon_id) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049B210});
        return fn(icon_id);
    }
}