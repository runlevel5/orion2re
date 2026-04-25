#pragma once

struct s_combat_bomb_result;

namespace COLBOMB {
    inline s_combat_bomb_result*& _results_ptr =
        *reinterpret_cast<s_combat_bomb_result**>(std::uintptr_t{0x005A7798});

    inline s_combat_bomb_result*& _backup_results_ptr =
        *reinterpret_cast<s_combat_bomb_result**>(std::uintptr_t{0x005A779C});

    inline void __cdecl Do_Summary_String_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044b770});
        fn();
    }

    void __cdecl Colony_Bombing_Screen_(s_combat_bomb_result* results);
}