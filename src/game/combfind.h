#pragma once

namespace COMBFIND {
    extern uint8_t _messed_up_fleet_palette;
    extern uint8_t _in_net_update;
    extern s_combfind_target _target;
    extern uint16_t (*_player_needs_to_attack)[MAX_STARS + 1];
    extern int16_t* _battle_stars;
    extern uint8_t (*_star_has_nonplayer)[MAX_STARS + 1];
    extern int16_t _n_battle_stars;
    extern int8_t (*_attack_confirmed)[MAX_PLAYERS][MAX_PLAYERS];
    extern s_combfind_star_has_player (*_star_has_player)[MAX_STARS + 1][MAX_PLAYERS];
    extern uint8_t* _current_star_has_nonplayer;
    extern s_combfind_star_has_player* _current_star_has_player;
    extern char* _research_treaty_msg;
    extern char* _s_attacks_you_at_s_msg;
    extern char* _outpost_destroyed_msg;
    extern char* _combat_is_reported_between_s_and_s_msg;
    extern char* _you_have_following_treaties_msg;
    extern char* _receiving_tribute_msg;
    extern char* _noncombat_ships_destroyed_msg;
    extern char* _colony_seized_by_mind_control_msg;
    extern char* _really_risk_war_msg;
    extern char* _trade_treaty_msg;

    void __cdecl Search_For_Battles_();

    void __cdecl Process_Retreating_Ships_();

    int16_t __cdecl Closest_Other_Colony_Star_(int16_t player_idx, int16_t star_idx);

    void __cdecl Pick_Star_And_Attacker_(int16_t* star_idx, int16_t* attacker_idx);

    void __cdecl Reset_One_Shot_System_Special_(s_star_data* star);

    void __cdecl Do_System_Discoveries_At_Star_(s_star_data* star);

    int16_t __cdecl Find_Planet_At_Star_Satisfying_(s_star_data* star, bool (__cdecl* predicate)(s_planet_data*));

    void __cdecl Update_Player_Needs_To_Attack_(int16_t star_idx, int player_a_idx, int player_b_idx);

    void __cdecl Get_Local_Human_Target_(int16_t star_idx, int16_t player_idx, int16_t* target_player_idx_out, int16_t* target_colony_idx_out);

    int8_t __cdecl Human_Confirms_Attack_(int player_idx, int target_idx);

    void __cdecl Get_AI_Target_(int16_t star_idx, int16_t player_idx, int16_t* target_player, int16_t* target_colony, int8_t* is_sneak_attack);

    void __cdecl Get_Ship_Strength_Info_(uint32_t* enemy_strength, uint32_t* player_strength, uint32_t* shield_strength, int16_t star_idx, int16_t player_idx);

    void __cdecl Get_Best_Colony_Target_(
        uint32_t* enemy_fleet_space_str_arr,
        uint32_t* our_space_str_arr,
        uint32_t* our_ground_str_arr,
        int16_t star_idx,
        int16_t player_idx,
        uint32_t* best_score,
        int16_t* best_colony_idx,
        int32_t preferred_owner);

    /**
     * Chooses a non-player combat target at a star.
     * @param star_idx Star containing the pending combat.
     * @param attacker_idx Non-player attacker identifier.
     * @param target_player_idx_out Receives the chosen ship-only defender, or -1.
     * @param target_colony_idx_out Receives the chosen colony target, or -1.
     */
    void __cdecl Get_NPC_Target_(int16_t star_idx, int16_t attacker_idx, int16_t* target_player_idx_out, int16_t* target_colony_idx_out);

    void __cdecl Do_Dragon_Bribe_(int player_idx, int16_t star_idx, int16_t dest_idx);

    void __cdecl Client_Combat_Loop_();

    void __cdecl Bomb_Results_Popups_(s_battle_results* res);

    void __cdecl Strategic_Results_Popup_(s_battle_results* battle_results);

    void __cdecl Get_Combat_Ships_(int16_t* ships, int16_t star_idx, int16_t plr1_idx, int16_t* plr1_offset, int16_t* plr1_count, int16_t plr2_idx, int16_t* plr2_offset, int16_t* plr2_count);

    int16_t __cdecl Russ_Combat_(int16_t attacker_idx, int16_t* attacker_ships, int16_t attacker_count, int16_t defender_idx, int16_t* defender_ships, int16_t defender_count, int16_t star_idx, int16_t colony_idx, int8_t is_sneak_attack, s_battle_results* results);

    void __cdecl Kill_Noncombat_Ships_(int16_t* ship_indices, int16_t count);

    /**
     * Marks a defeated fleet to retreat or be destroyed when no colony can shelter it.
     * @param star_idx Star where combat ended.
     * @param player_idx Owner of the retreating fleet.
     * @param colony_idx Colony that can still anchor the fleet, or -1.
     */
    void __cdecl Determine_Retreat_Ships_(int16_t star_idx, int16_t player_idx, int16_t colony_idx);

    void __cdecl Reset_Combat_Ships_At_Current_Star_Has_Player_(int16_t player_idx);

    void __cdecl Do_Attacker_Beat_Colony_Stuff_(int16_t* ships, int n_ships, int16_t colony_idx, s_battle_results* results);

    char* __cdecl Load_Combfind_Strings_(char* strings_block);

    s_colony* __cdecl Free_Colony_Slot_();

    bool __cdecl Planet_Has_Splinter_Colony_(s_planet_data* planet);

    int16_t __cdecl Target_Planet_To_Colony_(int16_t planet_idx);

    int16_t __cdecl Target_Colony_To_Planet_(int16_t colony_idx);

    /**
     * Resolves one combat at a star and optionally fills a battle-results record.
     * @param star_idx Star where combat occurs.
     * @param attacker_idx Attacking player or monster.
     * @param defender_idx Defending player or monster, or -1 to derive from the colony.
     * @param planet_idx Defending planet, or -1 for ship-only combat.
     * @param result Receives battle results when non-null.
     * @param is_sneak_attack Nonzero when sneak-attack handling applies.
     */
    int16_t __cdecl Do_1_Combat_(int16_t star_idx, int16_t attacker_idx, int16_t defender_idx, int16_t planet_idx, s_battle_results* result, int8_t is_sneak_attack);

    int8_t __cdecl AI_Can_Retreat_(int8_t player_idx, int16_t excluded_colony_idx);

    int8_t __cdecl Some_Star_Player_Is_Busy_(int16_t star_idx);

    int16_t __cdecl Pick_Attacker_At_Star_(int16_t star_idx);

    void __cdecl Collect_Targets_For_Combat_(int16_t star_idx, int16_t player_idx);

    int8_t __cdecl Player_May_Attack_Player_(int attacker_idx, int target_idx);

    void __cdecl Copy_Target_Info_(s_combfind_target* src);

    void __cdecl Decode_Star_Player_Presence_(NETPROTO::Packet_Reader_* reader, int16_t star_idx);

    int32_t __cdecl Do_Bombardment_(int8_t attacker_id, int16_t colony_idx);

    uint8_t __cdecl Has_Stellar_Converter_(int16_t* ship_list, int16_t ship_count);

    int32_t __cdecl N_Enemy_Stars_In_Range_(int16_t player_idx, uint8_t enemy_player_idx);

    int32_t __cdecl N_Enemy_Stars_(int8_t player_idx);

    int8_t __cdecl Player_Owns_Transports_(int player_idx);

    void __cdecl GNN_Reports_Combat_(int player1_idx, int player2_idx);

    void __cdecl Do_Nonhuman_Attack_Against_Human_(int player_idx);

    /** Advances multiplayer combat synchronization state. */
    void __cdecl Do_Net_Combat_Update_(bool unused);

    void __cdecl Send_Star_Player_Presence_(NETPROTO::Packet_Writer_* writer, int16_t star_idx);
}