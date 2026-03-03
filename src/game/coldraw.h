#pragma once
#include <cstdint>

namespace COLDRAW {
    void __cdecl Calculate_Squish_Step_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4);

    inline void __cdecl Draw_Road_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004df610});
        fn();
    }

    inline void __cdecl Load_Road_List_Anims_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004DC880})();
    }

    inline void __cdecl Build_Road_List_Based_On_Bldg_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004594e0});
        fn();
    }

    inline void __cdecl Build_Veggie_List_Based_On_Bldg_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00459e50});
        fn();
    }

    inline void __cdecl Clear_Veggie_Draw_Flags_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00459df0});
        fn();
    }

    inline void __cdecl Draw_Veggie_CR_(uint16_t col, uint16_t row, bool not_scanning_valid) {
        using Fn = void(__cdecl*)(uint16_t, uint16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045a100});
        fn(col, row, not_scanning_valid);
    }

    int16_t __cdecl Do_Colony_Info_Pop_Stuff_For_Pop_(int16_t colony_idx, int16_t param_2, int16_t mode, int16_t x, int16_t y, int16_t x2, int16_t show_shadows, int16_t* idx_out, int16_t* count_ptr, int16_t* x_limit_ptr, int16_t* field_id_ptr);

    void __cdecl Draw_Colony_Prod_Both_(int16_t colony_idx, int16_t prod_type, int16_t x, int16_t y, int16_t width, int16_t spacing, int8_t frame);

    // TYPE MISMATCH: existing stub had int32_t for some parameters, decompiled has int16_t
    void __cdecl Draw_Colony_Prod_(int16_t colony_handle, int32_t selection, int32_t p3, int16_t y_offset, int32_t p5, int32_t p6);

    void __cdecl Draw_Info_Morale_Both_(int16_t colony_idx, int16_t x, int16_t y, int width, char is_mini);

    void __cdecl Draw_Info_Morale_(int16_t param_1, int16_t param_2, int16_t param_3, int32_t param_4);

    void __cdecl Draw_Colony_PC_Bar_(int16_t x, int16_t y, int16_t total, int16_t current);
}