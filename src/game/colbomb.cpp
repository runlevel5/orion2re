#include "pch.h"

namespace COLBOMB {
    void __cdecl Colony_Bombing_Screen_(s_combat_bomb_result* results) {
        s_combat_bomb_result backup;

        _results_ptr = results;
        struct_::Copy_Structure_(&backup, results, 0x49);
        _backup_results_ptr = &backup;

        Do_Summary_String_Popup_();

        COLONY::Set_Colony_Globals_(_results_ptr->colony_idx);

        MOX::_current_screen = MOX::_return_screen;
    }
}