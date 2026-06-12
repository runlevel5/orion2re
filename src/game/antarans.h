#pragma once

namespace antarans {
    /// Processes Antaran resource growth, fleet production, and any invasion launch.
    void __cdecl Antaran_Invasion_Check_();

    /// Returns one deployed Antaran ship of the given size to the reserve pool.
    /// @param ship_type_idx Antaran ship size slot to restock.
    void __cdecl Restock_Antaran_Ship_(int16_t ship_type_idx);

    /// Removes one ship of the given size from the defensive fleet.
    /// @param ship_idx Antaran ship size slot to reduce.
    void __cdecl Kill_Antaran_Defensive_Ship_(int16_t ship_idx);

    /// Removes one ship of the given size from the offensive fleet.
    /// @param ship_idx Antaran ship size slot to reduce.
    void __cdecl Kill_Antaran_Offensive_Ship_(int16_t ship_idx);

    /// Returns the number of stardate ticks since the Antaran timeline baseline.
    int32_t __cdecl Delta_Stardate_();

    /// Returns the initial stardate delay before Antaran growth begins.
    int16_t __cdecl Antaran_Delay_();

    /// Compatibility wrapper for Antaran_Delay_().
    inline int16_t __cdecl Delay_() {
        return Antaran_Delay_();
    }

    /// Returns the defensive resource growth bonus from the current difficulty.
    int16_t __cdecl Antarans_Defensive_Resource_Bonus_();

    /// Returns the offensive resource growth bonus from the current difficulty.
    int16_t __cdecl Antarans_Offensive_Resource_Bonus_();

    /// Returns the largest undeployed offensive ship size currently available.
    int16_t __cdecl Next_Antaran_Ship_Size_();

    /// Returns whether all defensive ship caps have been reached.
    bool __cdecl Antaran_Defensive_Force_At_Maximum_();

    /// Advances Antaran resource pools when the current stardate reaches a growth interval.
    void __cdecl Increment_Antaran_Resource_Level_();

    /// Returns the last turn on which small Antaran ships may still be built.
    int16_t __cdecl No_Small_Ships_After_N_Turns_();

    /// Returns the last turn on which medium Antaran ships may still be built.
    int16_t __cdecl No_Medium_Ships_After_N_Turns_();

    extern uint8_t _antaran_ship_cost[HULL_CLASS_COUNT];
    extern uint8_t _n_max_antaran_off_ships[HULL_CLASS_COUNT];
    extern uint8_t _offensive_titan_ship_built;

    /// Spends an Antaran resource pool to build ships up to the configured caps.
    /// @param resources Resource pool consumed by construction.
    /// @param ships Per-size ship counts to increase.
    /// @param max_ships Per-size cap table used to gate construction.
    /// @param is_offensive Nonzero when building the offensive fleet.
    void __cdecl Build_Antaran_Ships_(int16_t *resources, int16_t *ships, uint8_t *max_ships, int8_t is_offensive);

    /// Builds as many offensive ships as the current offensive resource pool allows.
    void __cdecl Build_Antaran_Offensive_Ships_();

    /// Collects and lightly shuffles the stars colonized by the given player.
    /// @param player_idx Player index to scan for colonies.
    /// @param star_list Output buffer that receives matching star indices.
    /// @param list_count Receives the number of entries written to star_list.
    void __cdecl Build_List_Of_Stars_By_Player_(int16_t player_idx, int16_t *star_list, int16_t *list_count);

    /// Chooses a random colonized star to target for an Antaran attack.
    /// @param player_idx Player index whose colonies are eligible targets.
    int16_t __cdecl Get_Antaran_Target_Star_(int16_t player_idx);

    extern uint8_t _n_max_antaran_def_ships[HULL_CLASS_COUNT];

    /// Builds as many defensive ships as the current defensive resource pool allows.
    void __cdecl Build_Antaran_Defensive_Ships_();

    /// Deploys an Antaran strike fleet toward the current victim's colonies.
    bool __cdecl Deploy_Antaran_Ships_();

    /// Returns the total Antaran resources including ship value and stored reserves.
    int16_t __cdecl Antaran_Total_Resources_();

    /// Returns the total offensive fleet value based on per-size ship costs.
    int16_t __cdecl Antaran_Offensive_Force_();

    /// Returns the total defensive fleet value based on per-size ship costs.
    int16_t __cdecl Antaran_Defensive_Force_();

    /// Returns whether the current offensive pressure is high enough to trigger an attack.
    bool __cdecl Antarans_Should_Attack_();
}