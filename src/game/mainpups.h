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
}