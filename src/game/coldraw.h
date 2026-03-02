#pragma once
#include <cstdint>

namespace COLDRAW {
    // Suggested name: Calculate_Squish_Step
    inline void __cdecl Calculate_Squish_Step_(int16_t x1, int16_t x2, int16_t count, int16_t base_step) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00458410});
        fn(x1, x2, count, base_step);
    }

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

    inline int16_t __cdecl Do_Colony_Info_Pop_Stuff_For_Pop_(
        int16_t colony_id, int16_t pop_idx, int32_t arg3, int32_t y, int32_t x,
        int32_t y2, int32_t arg7, int16_t* refs, int16_t* pop_count,
        int16_t* coord_vars, int16_t* job_fields)
    {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int32_t, int32_t, int32_t,
                                 int32_t, int32_t, int16_t*, int16_t*,
                                 int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00458470});
        return fn(colony_id, pop_idx, arg3, y, x, y2, arg7, refs, pop_count, coord_vars, job_fields);
    }

    inline void __cdecl Draw_Colony_Prod_Both_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4, int16_t param_5, int16_t param_6, uint8_t param_7) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00458c40});
        fn(param_1, param_2, param_3, param_4, param_5, param_6, param_7);
    }

    // TYPE MISMATCH: existing stub had int32_t for some parameters, decompiled has int16_t
    void __cdecl Draw_Colony_Prod_(int16_t colony_handle, int32_t selection, int32_t p3, int16_t y_offset, int32_t p5, int32_t p6);

    inline void __cdecl Draw_Info_Morale_Both_(int16_t param_1, int16_t param_2, int16_t param_3, int32_t param_4, int8_t param_5) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int32_t, int8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00458a60});
        fn(param_1, param_2, param_3, param_4, param_5);
    }

    void __cdecl Draw_Info_Morale_(int16_t param_1, int16_t param_2, int16_t param_3, int32_t param_4);

    void __cdecl Draw_Colony_PC_Bar_(int16_t x, int16_t y, int16_t total, int16_t current);
}