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
    extern s_animation_header* _difficulty_anims[5];
    extern s_animation_header* _num_players_anims[7];
    extern s_animation_header* _gal_age_anims[3];
    extern s_animation_header* _gal_size_anims[4];
    extern s_animation_header* _background_seg;
    extern char* _gal_size_button_strings[4];
    extern char* _civ_button_strings[3];
    extern char* _diff_button_strings[5];
    extern char* _gal_age_button_strings[3];
    extern char* _combat_button_strings[2];
    extern char _hack_msg[20];
    extern char* _opp_button_strings[7];

    extern const char s_Media_00557db4[6]; // "Media"
    extern const char s_sen_00557dbc[5]; // "%sen"
    extern const char s_Piccola_00557dc4[8]; // "Piccola"

    void __cdecl Load_New_Game_Strings_();

    void __cdecl Reload_Newgame_Screen_();

    void __cdecl Globals_To_Temps_();

    void __cdecl Update_Fields_();

    void __cdecl Draw_Pictures_();

    void __cdecl Draw_Newgame_Screen_();

    void __cdecl Temps_To_Globals_();

    int16_t __cdecl Newgame_Screen_();
}