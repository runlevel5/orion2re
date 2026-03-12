#pragma once
#include <cstdint>

namespace MAINPUPS {
    inline int16_t& _last_scanned_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565D8C});
    inline int16_t& _g_temp_tax_rate = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E30});

    inline void __cdecl Load_Tax_Rate_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045F500});
        fn();
    }

    inline void __cdecl Do_Popup_Setup_(int32_t arg1, uint32_t arg2) {
        using Fn = void(__cdecl*)(int32_t, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045AB30});
        fn(arg1, arg2);
    }

    inline void __cdecl Add_Tax_Rate_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045F5E0});
        fn();
    }

    inline void __cdecl Draw_Tax_Rate_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045F7B0});
        fn();
    }

    inline void __cdecl Evaluate_Tax_Rate_Input_(int16_t* input_val, int16_t* scan_val, int16_t* exit_flag) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045F800});
        fn(input_val, scan_val, exit_flag);
    }

    inline void __cdecl Do_Popup_Cleanup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045ADC0});
        fn();
    }

    inline void __cdecl Clear_System_Special_Slots_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045A320});
        fn();
    }

    inline void __cdecl Build_Generic_Planet_Or_Colony_Description_String_Formatted_(int16_t id, char* buffer) {
        using Fn = void(__cdecl*)(int16_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045d420});
        fn(id, buffer);
    }

    inline int16_t __cdecl Dragon_New_Star_Popup_(int32_t defender_idx, int32_t star_idx) {
        using Fn = int16_t(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045f8a0});
        return fn(defender_idx, star_idx);
    }

    inline int16_t __cdecl Defense_Colony_Selection_Popup_(int16_t star_idx, int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045c4a0});
        return fn(star_idx, player_idx);
    }

    inline void __cdecl Player_Attacked_Popup_(int16_t plr1_idx, int16_t plr2_idx, int16_t star_idx, int16_t colony_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045C9B0});
        fn(plr1_idx, plr2_idx, star_idx, colony_idx);
    }
}