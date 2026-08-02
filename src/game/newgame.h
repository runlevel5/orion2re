#pragma once
#include <cstdint>

namespace NEWGAME {
    extern int16_t _g_x;
    extern int16_t _g_y;
    extern s_colors _color_ken;
    extern int16_t _opp_button_variable;
    extern int16_t _diff_button_variable;
    extern int16_t _xenon_attacks_button_variable;
    extern int16_t _civ_button_variable;
    extern uint16_t _combat_button_variable;
    extern int16_t _gal_size_button_variable;
    extern int16_t _random_events_button_variable;
    extern int16_t _gal_age_button_variable;
    extern int16_t _fields[10];
    extern s_animation_header* _anims[7];
    extern s_animation_header* _tech_level_anims[3];
    extern s_animation_header* _difficulty_anims[GAME_DIFFICULTY_COUNT];
    extern s_animation_header* _num_players_anims[7];
    extern s_animation_header* _gal_age_anims[GALAXY_AGE_COUNT];
    extern s_animation_header* _gal_size_anims[GALAXY_SIZE_MAXIMUM];
    extern s_animation_header* _background_seg;
    extern char* _gal_size_button_strings[GALAXY_SIZE_COUNT];
    extern char* _civ_button_strings[3];
    extern char* _diff_button_strings[GAME_DIFFICULTY_COUNT];
    extern char* _gal_age_button_strings[GALAXY_AGE_COUNT];
    extern char* _combat_button_strings[2];
    extern char _localized_gal_size_label[20];
    extern char* _opp_button_strings[7];

    /**
     * Loads the localized labels used by the new game option controls.
     */
    void __cdecl Load_New_Game_Strings_();

    /**
     * Reloads the new game background and option animations from LBX resources.
     */
    void __cdecl Reload_Newgame_Screen_();

    /**
     * Copies persisted game settings into the screen-local selection variables.
     */
    void __cdecl Globals_To_Temps_();

    /**
     * Rebuilds the interactive fields for the current new game options.
     */
    void __cdecl Update_Fields_();

    /**
     * Draws the preview pictures for the currently selected options.
     */
    void __cdecl Draw_Pictures_();

    /**
     * Draws the full new game screen for the current selection state.
     */
    void __cdecl Draw_Newgame_Screen_();

    /**
     * Copies the screen-local selections back into the persisted game settings.
     */
    void __cdecl Temps_To_Globals_();

    /**
     * Runs the new game setup screen and returns whether setup was accepted.
     */
    int16_t __cdecl Newgame_Screen_();
}