#pragma once
#include <cstdint>

namespace FLT2 {
    inline void __cdecl Set_Initial_Stack_Pointer_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004898D0 });
        fn();
    }

    inline void __cdecl Clear_All_Selection_Status_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004899A0 });
        fn();
    }

    inline void __cdecl Build_Fltscrn_Big_Icon_Fields_(int16_t stack_ptr, int16_t visible_rows) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00487B90});
        fn(stack_ptr, visible_rows);
    }
}