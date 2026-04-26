#pragma once

struct s_combat_bomb_result;

namespace COLBOMB {
    inline s_combat_bomb_result*& _results_ptr =
        *reinterpret_cast<s_combat_bomb_result**>(std::uintptr_t{0x005A7798});

    inline s_combat_bomb_result*& _backup_results_ptr =
        *reinterpret_cast<s_combat_bomb_result**>(std::uintptr_t{0x005A779C});

    inline char* s_Perdita_di_s_00556b14 =
        reinterpret_cast<char*>(std::uintptr_t{0x00556B14});

    inline char* s_Edificio_distrutto_s_00556b24 =
        reinterpret_cast<char*>(std::uintptr_t{0x00556B24});

    void __cdecl Do_Summary_String_Popup_();

    void __cdecl Colony_Bombing_Screen_(s_combat_bomb_result* results);
}