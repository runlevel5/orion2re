#pragma once
#include <cstdint>

namespace COMBAT1 {
    extern int32_t _end_of_turn_button;

    // Returns nonzero when the indexed combat ship is active, uncaptured, and not in stasis.
    // ship_idx: combat ship index to test.
    int16_t __cdecl Combat_Ship_Is_Functional_(int16_t ship_idx);

    // Resets and refreshes per-turn combat state for one ship.
    // ship_idx: combat ship index to initialize.
    void __cdecl Init_Ship_For_Start_Of_Turn_(int16_t ship_idx);

    // Compares two ship indices by tactical initiative for qsort.
    // p1/p2: pointers to int16_t ship indices.
    int __cdecl qsort_ship_initiative_(const void* p1, const void* p2);

    // Computes the initiative score used for combat turn ordering.
    // ship_idx: combat ship index to score.
    int16_t __cdecl Ship_Initiative_Value_(int16_t ship_idx);

    extern uint16_t _start_of_turn;

    // Advances to the next eligible unit in the combat turn-order list.
    // unused_player_idx: reserved caller context, currently unused.
    int16_t __cdecl Next_Ship_(int16_t unused_player_idx);

    int16_t __cdecl Ship_Controlled_By_Computer_(int16_t ship_idx);

    // Sends one encoded tactical-combat message to the remote player.
    // player_idx: destination player index; message_code/arg1/arg2/arg3: encoded payload values.
    int32_t __cdecl Send_Tactical_Combat_Message_(int16_t player_idx, int16_t message_code, int16_t arg1, int16_t arg2, int16_t arg3);

    void __cdecl Add_Tactical_Combat_Fields_(void);

    void __cdecl Add_Missile_Fields_(void);

    // Rebuilds occupancy and move overlays for the combat grid.
    // active_ship_idx: ship whose movement overlay should be refreshed.
    void __cdecl Assign_Combat_Grids_(int16_t active_ship_idx);

    // Marks reachable combat-grid cells for the given ship.
    // ship_idx: combat ship index; range: movement range to test; check_cost: nonzero to enforce move-cost limits.
    void __cdecl Set_Legal_Moves_(int16_t ship_idx, int16_t range, int16_t check_cost);

    void __cdecl Assign_Mouse_Images_();

    int16_t __cdecl Ship_Done_Check_(void);

    void __cdecl Set_Cur_Ship_To_(int16_t ship_idx);

    void __cdecl Set_Combat_Help_Fields_();

    int16_t __cdecl Enemy_Dissipator_Exists_(int16_t player_idx);

    // Validates and resolves a boarding attempt against the selected target.
    // attacker_idx/defender_idx: combat ship indices.
    void __cdecl Initiate_Boarding_Action_(int16_t attacker_idx, int16_t defender_idx);

    // Advances and resolves seeking missiles for the current combat pass.
    // player_idx: player whose pass is being processed.
    void __cdecl Seeking_Missiles_(int16_t player_idx);

    // Runs one tactical-combat turn pass.
    // active_player_id: side being processed; time_warp_only: nonzero limits work to the special-gated pass.
    void __cdecl Do_Combat_Turn_(int16_t active_player_id, int16_t time_warp_only);
    // Applies end-of-round phasing-cloak timer countdown and reversion for all combat ships.
    void __cdecl Update_Phasing_Cloak_Timers_();
    int16_t __cdecl Check_For_Winner_();
    int16_t __cdecl Calc_Current_Speed_(int16_t ship_idx);

    void __cdecl Retreat_Ship_(int16_t ship_idx);

    void __cdecl Remove_Continuing_Effect_Specials_(int16_t source_idx);

    int16_t __cdecl Get_Number_Of_Ships_In_Stasis_(int16_t source_ship_idx);

    bool __cdecl Weapon_Is_On_(int16_t ship_idx, int16_t weapon_idx);
}