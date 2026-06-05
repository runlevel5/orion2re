#pragma once

struct s_colors;
struct s_player;

namespace raceopt {
    extern int16_t _negative_picks;
    extern int16_t _exit_ken;
    extern s_colors _color_opt;
    extern int8_t* _ability_costs;
    extern char* _opt_string[64];
    extern uint8_t* _accept_button_seg;
    extern uint8_t* _default_button_seg;
    extern int16_t _option_button[22];
    extern int16_t _defalt;
    extern int16_t _group_button[41];
    extern int16_t _accept;
    extern int16_t _picks;
    extern s_animation_header* _off_button_seg;
    extern int16_t _option_toggle[24];
    extern int16_t _button_variable[10];
    extern s_animation_header* _special_button_seg;
    extern int16_t _name_button;
    extern char _cur_race_name[16];

    /**
     * Converts a player's stored trait values into the race-option UI state.
     * @param player Player whose trait array is decoded.
     */
    void __cdecl Convert_Player_To_Flags_(s_player* player);

    /**
     * Writes the current race-option UI state back into a player's trait array.
     * @param player Player whose traits are updated.
     */
    void __cdecl Convert_Flags_To_Player_(s_player* player);

    /** Adds the standard race-option screen input fields. */
    void __cdecl Add_Race_Options_Fields_();

    /**
     * Computes the effective pick cost for an option after dependency rules.
     * @param ability_idx Option index in the racial ability table.
     */
    int16_t __cdecl Real_Ability_Cost_(int16_t ability_idx);

    /** Recalculates the current pick total and negative-pick subtotal. */
    void __cdecl Calculate_Picks_();

    /** Draws the current race-option screen state onto the active page. */
    void __cdecl Draw_Racial_Option_Screen_();

    /** Draws the static background artwork and labels for the race-option screen. */
    void __cdecl Draw_Race_Option_Background_();

    /** Reloads the localized option strings and ability-cost table. */
    void __cdecl Reload_Option_Strings_();

    /** Reloads the race-option screen graphics and text resources. */
    void __cdecl Reload_Racial_Option_Screen_();

    /**
     * Converts an EM trait buffer into the race-option UI state.
     * @param player_flags Trait buffer to decode.
     */
    void __cdecl Convert_Player_To_Flags_EM_(int8_t* player_flags);

    /**
     * Serializes the current EM race-option UI state into a trait buffer.
     * @param flags_out Destination trait buffer.
     */
    void __cdecl Convert_Flags_To_Player_EM_(int8_t* flags_out);

    /** Populates and activates the contextual help regions for the race-option screen. */
    void __cdecl Set_Race_Option_Help_List_();

    /**
     * Adds the EM mutation screen input fields allowed by the baseline traits and pick budget.
     * @param baseline_flags Baseline trait selections used to validate upgrades.
     * @param available_picks Pick budget available for selectable upgrades.
     */
    void __cdecl Add_Race_Options_Fields_EM_(int8_t* baseline_flags, int16_t available_picks);

    /**
     * Prepares and applies the evolutionary-upgrade selection flow for a player.
     * @param player_idx Index of the player receiving the upgrade.
     */
    void __cdecl Setup_Evolutionary_Upgrade_(int16_t player_idx);

    /**
     * Displays the evolutionary mutation picker and writes back the chosen traits.
     * @param traits Trait buffer edited by the mutation screen.
     */
    void __cdecl Evolutionary_Mutation_Screen_(int8_t* traits);

    /**
     * Calculates the score modifier implied by a player's current race picks.
     * @param player_idx Index of the player to score.
     */
    int16_t __cdecl Player_Score_Modifier_(int16_t player_idx);

    /** Recalculates picks for the EM flow, including its fixed bonus picks. */
    void __cdecl Calculate_EM_Picks_();
}