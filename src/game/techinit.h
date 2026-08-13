#pragma once
#include <cstdint>

namespace TECHINIT {
    inline constexpr int16_t POSTWARP_TECH_COUNT = 29;
    inline constexpr int16_t POSTWARP_STRATEGIC_EXTRA_TECH_COUNT = 7;

    inline int16_t postwarp_tech[POSTWARP_TECH_COUNT] = {
        56, 20, 192, 25, 82, 13, 13, 22, 132, 26,
        179, 71, 72, 51, 191, 163, 46, 122, 155, 61,
        87, 70, 73, 33, 57, 104, 180, 181, 66,
    };

    /**
     * Grants configured Post-warp technologies to every player.
     */
    void __cdecl Grant_Postwarp_Technologies_();

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