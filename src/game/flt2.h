#pragma once
#include <cstdint>

namespace FLT2 {
    inline void __cdecl Add_Fltscrn_Small_Icon_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00487B10});
        fn();
    }

    inline void __cdecl Fleet_Screen_Big_Icon_Fields_(int16_t* ids, int x, int y, int columns, int spacing) {
        using Fn = void(__cdecl*)(int16_t*, int, int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00488070});
        fn(ids, x, y, columns, spacing);
    }

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

    inline void __cdecl Set_Fleet_Screen_Stack_Owner_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004895D0});
        fn();
    }

    inline void __cdecl Get_Fltscrn_Big_Icon_XY_(int16_t index, int16_t* x, int16_t* y) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004885a0});
        fn(index, x, y);
    }

    inline int16_t __cdecl Scan_Fltscrn_Big_Icons_(uint32_t scan_input, int16_t input, int16_t* hovered_icon_idx, int16_t* clicked_icon_idx) {
        using Fn = int16_t(__cdecl*)(uint32_t, int16_t, int16_t*, int16_t*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004894c0})(scan_input, input, hovered_icon_idx, clicked_icon_idx);
    }

    inline void __cdecl Fltscrn_Move_Ships_(int16_t star_id) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00488250})(star_id);
    }

    inline int16_t __cdecl Fltscrn_Ships_To_Be_Displayed_(int16_t ship_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00488490})(ship_idx);
    }

    inline void __cdecl Fix_Palette_(int16_t player_num) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00488040 })(player_num);
    }
}