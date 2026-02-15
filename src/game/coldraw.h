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
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004598a0});
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
}