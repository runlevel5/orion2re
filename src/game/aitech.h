#pragma once

#include <cstdint>

namespace aitech {
    /// Applies pending evolutionary trait upgrades for players that have selected one.
    void __cdecl Apply_Evolutionary_Upgrades_();

    /// Populates aggregate AI technology metrics for each active player.
    /// @param info Receives the computed per-player technology data.
    void __cdecl Compute_AI_Tech_Info_(s_ai_tech_info* info);

    /// Recomputes AI technology priorities for all active AI players.
    void __cdecl All_AI_Tech_Select_();

    /// Chooses the next research application for a player when a reselection is needed.
    /// @param player Player whose research target may be updated.
    void __cdecl AI_Tech_Select_(s_player* player);

    /// Returns the highest researched ship shield index for a player.
    /// @param player Player whose researched shield technologies are checked.
    int32_t __cdecl Player_Best_Ship_Shield_(s_player* player);

    /// Returns the highest researched armor index for a player.
    /// @param player Player whose researched armor technologies are checked.
    int32_t __cdecl Player_Best_Armor_(s_player* player);

    /// Returns the highest researched rifle index for a player.
    /// @param player Player whose researched rifle technologies are checked.
    int32_t __cdecl Player_Best_Rifle_(s_player* player);

    /// Returns the highest researched personal shield index for a player.
    /// @param player Player whose researched personal shield technologies are checked.
    int32_t __cdecl Player_Best_Personal_Shield_(s_player* player);

    /// Returns the highest researched battle computer index for a player.
    /// @param player Player whose researched computer technologies are checked.
    int32_t __cdecl Player_Best_Computer_(s_player* player);

    /// Returns the highest researched fuel-cell index for a player.
    /// @param player Player whose researched fuel technologies are checked.
    int32_t __cdecl Player_Best_Fuel_(s_player* player);

    /// Returns the best researched planetary shield tier for a player.
    /// @param player Player whose planetary shield technologies are checked.
    int32_t __cdecl Player_Best_Planetary_Shield_(s_player* player);

    /// Returns the highest researched engine drive index for a player.
    /// @param player Player whose researched drive technologies are checked.
    int32_t __cdecl Player_Best_Drive_(s_player* player);

    /// Counts nearby-system proximity pressure from another player's colonies.
    /// @param player_id Player whose nearby range is evaluated.
    /// @param other_player_id Player whose colonies contribute to the score.
    int16_t __cdecl System_Proximity_Count_(int16_t player_id, uint8_t other_player_id);

    /// Returns the scaled cost of the most advanced completed technology field for a player.
    /// @param player_idx Index of the player to inspect.
    int32_t __cdecl Get_Max_Technology_Value_(int16_t player_idx);
}