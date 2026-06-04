#pragma once

namespace MOX2 {
    /** Seeds the game RNG from the command-line override or randomized startup state. */
    void __cdecl Set_Game_Random_Seed_();

    /** Convenience wrapper that reseeds the game RNG. */
    inline void __cdecl Set_Game_Seed() {
        Set_Game_Random_Seed_();
    }

    /** Resets map-related globals and screen state to their default values. */
    void __cdecl Init_Map_Defaults_();
    /** Loads default nebula art, then resets the map state. */
    void __cdecl Init_Map_Pictures_Default__();
    extern int8_t _user_wants_n_space_monsters;
    extern uint8_t _pan_main_menu_panel;
    extern uint8_t _skip_intro;
    extern uint8_t _quick_start;
    extern char _leave_game_message[100];

    /** Attempts to read language.ini, then restores the language setting to its default value. */
    void __cdecl Set_Mox_Language_();

    /** Loads the alternate data path from orioncd.ini and verifies required assets are reachable. */
    void __cdecl Set_Mox_Alt_Path_();

    /** Runs the main screen-dispatch loop until the exit screen is selected. */
    void __cdecl Screen_Control_();

    extern const char s_language_ini[13];
    extern const char s_language_ini_write_mode[3];
    /** Performs startup, enters the main screen loop, and exits with a localized leave message. */
    void __cdecl main2_();
    /**
     * Adapts main2_ to a callback-style entry point.
     * @param unused Unused callback context.
     */
    void __cdecl thunk_main2_(void* unused);
}

namespace MOX2 {
    typedef void(__cdecl* action_func_t)();

    /**
     * Stores the current auto-update callback in the caller-provided slot.
     * @param function_slot Receives the current callback pointer.
     */
    void __cdecl Get_Auto_Function_(action_func_t* function_slot);

    /**
     * Invokes the callback stored in the caller-provided slot if one is present.
     * @param function_entry Points to the callback pointer to execute.
     */
    void __cdecl Execute_Auto_Function_(action_func_t* function_entry);

    /**
     * Replaces the current auto-update callback with the caller-provided one.
     * @param function_slot Supplies the callback pointer to store.
     */
    void __cdecl Put_Auto_Function_(action_func_t* function_slot);
}
