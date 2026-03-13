#pragma once
#include <cstdint>

namespace COMBAT1 {
    inline int32_t& _end_of_turn_button = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00565E98});
    inline char* s__00551100 = reinterpret_cast<char*>(std::uintptr_t{0x00551100});

    inline int16_t __cdecl Combat_Ship_Is_Functional_(int16_t ship_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004668E0});
        return fn(ship_idx);
    }

    inline void __cdecl Init_Ship_For_Start_Of_Turn_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00466AB0});
        fn(ship_idx);
    }

    inline int32_t __cdecl qsort_ship_initiative_(const void* a, const void* b) {
        using Fn = int32_t(__cdecl*)(const void*, const void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00468ED0});
        return fn(a, b);
    }

    inline int16_t __cdecl Next_Ship_(int16_t param_1) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00469750});
        return fn(param_1);
    }

    inline int16_t __cdecl Ship_Controlled_By_Computer_(int16_t ship_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004669B0});
        return fn(ship_idx);
    }

    inline void __cdecl Send_Tactical_Combat_Message_(int16_t player, int16_t msg_type, int16_t arg1, int16_t arg2, int16_t arg3) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00466A20});
        fn(player, msg_type, arg1, arg2, arg3);
    }

    inline void __cdecl Add_Tactical_Combat_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0046A8A0});
        fn();
    }

    inline void __cdecl Assign_Combat_Grids_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0046AFC0});
        fn(ship_idx);
    }

    inline int16_t __cdecl Ship_Done_Check_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00469AC0});
        return fn();
    }

    inline void __cdecl Set_Cur_Ship_To_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004696B0});
        fn(ship_idx);
    }

    inline void __cdecl Set_Combat_Help_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0046A2C0});
        fn();
    }

    inline int16_t __cdecl Enemy_Dissipator_Exists_(int16_t player) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00466930});
        return fn(player);
    }

    inline void __cdecl Initiate_Boarding_Action_(int16_t ship_idx, int16_t target_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00469BD0});
        fn(ship_idx, target_idx);
    }

    inline void __cdecl Seeking_Missiles_(int16_t param_1) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00468F70});
        fn(param_1);
    }

    void __cdecl Do_Combat_Turn_(int16_t param_1, int16_t param_2);
}