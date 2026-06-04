#pragma once

namespace INITGAME {
    /** Sets baseline game, audio, and UI settings, then writes them to disk. */
    void Set_Default_Game_Settings_();

    /** Clears the global ship array and restores per-slot sentinel defaults. */
    void Init_Ships_Array_();

    /** Clears the global colony array and restores per-slot sentinel defaults. */
    void Init_Colonies_Array_();

    /** Loads leader data and refreshes randomized and derived leader fields. */
    void __cdecl Init_Leaders_();

    /** Initializes player records, including race traits, colors, names, and starting tech state. */
    void __cdecl Init_Players_();

    /** Converts generated system-monster markers into ship instances on the map. */
    void __cdecl Make_System_Monsters_Into_Ships_();

    /**
     * Initializes a colony record and links it to the given planet and owner.
     * @param colony_idx Colony slot to populate.
     * @param planet_idx Planet that will host the colony.
     * @param player_num Owning player ID.
     */
    void __cdecl Init_Colony_(int16_t colony_idx, int16_t planet_idx, uint8_t player_num);

    /** Resets colony auto-select history and computes the initial per-star selection state. */
    void __cdecl Set_Initial_Colony_Auto_Selects_();

    /**
     * Builds a new game state from the requested setup values.
     * @param game_difficulty Difficulty used during generation.
     * @param galaxy_size Requested galaxy size.
     * @param galaxy_age Requested galaxy age.
     * @param num_players Number of active players.
     * @param strategic_combat_flag Strategic-combat setting for the new game.
     * @param starting_civilization_level Starting civilization level for the new game.
     */
    void __cdecl Init_New_Game_(int16_t game_difficulty, int16_t galaxy_size, int16_t galaxy_age, int16_t num_players,
                                int16_t strategic_combat_flag, int16_t starting_civilization_level);

    /** Writes the current settings block to mox.set. */
    void Write_Game_Settings_();
}