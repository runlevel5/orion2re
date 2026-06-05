#pragma once
#include <cstdint>

namespace TECHINIT {
    /**
     * Initializes global technology metadata and populates per-field application lists.
     */
    void __cdecl Init_Tech_();

    /**
     * Loads localized technology, component, and hull names from the techname LBX resource.
     */
    void __cdecl Load_Tech_Names_();

    /**
     * Returns whether a technology application is available to the specified player.
     * @param player_idx Index of the player being checked.
     * @param tech_id Technology application ID to validate.
     */
    int16_t __cdecl Tech_Is_Legal_For_Player_(int16_t player_idx, int16_t tech_id);

    /**
     * Seeds a player's starting technology fields and applications.
     * @param player_idx Index of the player to initialize.
     */
    void __cdecl Init_Player_Tech_(int16_t player_idx);
}