#pragma once

struct s_combat_bomb_result;

namespace COLBOMB {
    extern s_combat_bomb_result* _results_ptr;

    extern s_combat_bomb_result* _backup_results_ptr;

    /**
     * Builds and displays the colony bombing summary popup from the backed-up combat results.
     */
    void __cdecl Do_Summary_String_Popup_();

    /**
     * Snapshots the bombing results, shows the summary popup, and restores colony-screen globals.
     *
     * @param results Bombardment results to display.
     */
    void __cdecl Colony_Bombing_Screen_(s_combat_bomb_result* results);
}