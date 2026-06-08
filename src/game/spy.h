#pragma once

struct s_leader_data;

namespace spy {
    extern int8_t _lookup_table[MAX_PLAYERS];

    /* Returns the spy bonus granted by the player's completed spy-related technologies.
     * @param player Player whose technology bonuses are evaluated.
     */
    int16_t __cdecl Tech_Spy_Bonus_(s_player* player);

    /* Computes the player's defensive and offensive spy bonuses.
     * @param player Player whose spy modifiers are evaluated.
     * @param defense_bonus_out Receives the computed defensive bonus.
     * @param attack_bonus_out Receives the computed offensive bonus.
     */
    void __cdecl Compute_Spy_Bonuses_(s_player* player, int16_t* defense_bonus_out, int16_t* attack_bonus_out);

    /* Resolves the turn's global spy phase for all active players. */
    void __cdecl Resolve_Spies_();

    extern int16_t _spy_bonuses[16];

    /* Resolves assassin attempts made by eligible leaders this turn. */
    void __cdecl Resolve_Assassins_();

    /* Resolves a completed assassination attempt against the given leader.
     * @param leader Leader targeted by the assassination event.
     */
    void __cdecl Leader_Assassinates_(s_leader_data* leader);

    /* Allocates an AI player's available spies across home and foreign targets.
     * @param player AI player whose spies are being reassigned.
     */
    void __cdecl Allocate_AI_Spies_(s_player* player);

    /* Resolves all spy actions currently targeting the given player.
     * @param player Player whose incoming spy actions are processed.
     */
    void __cdecl Resolve_Player_Spies_(s_player* player);

    /* Returns the player's spies home from the specified enemy.
     * @param player_idx Player whose spies are being recalled.
     * @param enemy_idx Enemy player the spies are returning from.
     */
    void __cdecl Bring_Home_Spies_Vs_(int16_t player_idx, int16_t enemy_idx);

    /* Returns the roll bonus contributed by the given number of spies.
     * @param spy_count Number of spies contributing to the bonus.
     */
    int32_t __cdecl N_Spies_Bonus_(int32_t spy_count);

    /* Attempts to steal a technology application from the victim.
     * @param attacker Player performing the theft.
     * @param victim Player being targeted.
     * @param frame_flag Nonzero to attempt framing another player for the theft.
     */
    void __cdecl Steal_App_(s_player* attacker, s_player* victim, int8_t frame_flag);

    /* Selects another player to frame for a spy action when possible.
     * @param player_idx_a Acting player performing the spy action.
     * @param player_idx_b Victim player being targeted.
     * @param framed_player_idx Receives the selected framed player index.
     * @param tech_app_id Optional stolen tech id used to filter candidates, or -1.
     */
    bool __cdecl Frame_Another_(int16_t player_idx_a, int16_t player_idx_b, int16_t* framed_player_idx, int32_t tech_app_id);

    /* Destroys a random victim building and reports the sabotage outcome.
     * @param framed_player_idx Player blamed for the sabotage.
     * @param victim_player_idx Player whose building is destroyed.
     * @param acting_player_idx Player who actually performed the sabotage.
     */
    void __cdecl Destroy_Random_Building_(int16_t framed_player_idx, int16_t victim_player_idx, int32_t acting_player_idx);
}