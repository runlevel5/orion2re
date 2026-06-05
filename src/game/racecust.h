#pragma once

namespace RACECUST {
    extern s_racecust_race_customize _race_customize[130];
    extern s_racecust_race_evolution _race_evolution[130];

    /**
     * Applies the difficulty-selected race customization and evolutionary trait overrides for one player.
     * @param player_idx Index of the player whose trait arrays are updated.
     */
    void __cdecl Race_Customization_(int16_t player_idx);
}