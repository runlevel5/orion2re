#pragma once
#include <cstdint>

namespace BILLHELP {
    inline void __cdecl History_Graph_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B5270})();
    }

    inline void __cdecl Tech_Review_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B5230})();
    }

    inline void __cdecl Race_Statistics_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B51F0})();
    }

    inline void __cdecl Turn_Summary_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B51B0})();
    }

    inline void __cdecl Races_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B5310})();
    }

    inline void __cdecl Technology_Change_Help_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b52c0});
        fn();
    }
    inline void __cdecl Technology_Select_Help_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b52b0});
        fn();
    }

    inline void __cdecl Reference_Help_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b50e0});
        fn();
    }

    inline void __cdecl Ref_How_To_Help_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b5120});
        fn();
    }

    inline void __cdecl Ref_Category_Help_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b5160});
        fn();
    }

    inline void __cdecl Tech_Change_List_Right_Help_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B52F0});
        fn();
    }

    inline void __cdecl Tech_Select_List_Right_Help_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B52D0});
        fn();
    }

    inline void __cdecl Tech_Change_List_Left_Help_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B5300});
        fn();
    }

    inline void __cdecl Tech_Select_List_Left_Help_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B52E0});
        fn();
    }
}