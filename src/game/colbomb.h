#pragma once

struct s_combat_bomb_result;

namespace COLBOMB {
    extern s_combat_bomb_result* _results_ptr;

    extern s_combat_bomb_result* _backup_results_ptr;

    extern char s_Perdita_di_s_00556b14[14];

    extern char s_Edificio_distrutto_s_00556b24[23];

    void __cdecl Do_Summary_String_Popup_();

    void __cdecl Colony_Bombing_Screen_(s_combat_bomb_result* results);
}