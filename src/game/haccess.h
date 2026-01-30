#pragma once
#include "deps.h"

namespace HACCESS {
    inline void __cdecl Race_Name_(int16_t race_idx, char* buffer) {
        using Fn = void(__cdecl*)(int16_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049acd0});
        fn(race_idx, buffer);
    }

    inline void __cdecl Set_Colony_Auto_Select_For_Star_(int16_t star_idx, int16_t player_num, int16_t* out_val) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049AF30});
        fn(star_idx, player_num, out_val);
    }

    inline bool __cdecl Star_Has_Relocation_(int16_t star_idx, int16_t player_num) {
        using Fn = bool(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049b5e0});
        return fn(star_idx, player_num);
    }

    inline int16_t __cdecl Relocation_(int16_t star_idx, int16_t player_num) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049ada0});
        return fn(star_idx, player_num);
    }

    int16_t __cdecl Ship_Owner_(int16_t ship_idx);
    int16_t __cdecl Ship_Stack_From_Saved_Array_();
    int16_t __cdecl Node_Ptr_From_Ship_Stack_(int16_t index);

    inline int16_t __cdecl Ship_Icon_Id_From_Node_Ptr_(int16_t node_ptr) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049b150});
        return fn(node_ptr);
    }

    inline int16_t __cdecl Ship_Icon_Owner_(int16_t icon_id) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049b1e0});
        return fn(icon_id);
    }
}