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
    extern const char s__00551100[1];
    extern int16_t _name_button;
    extern char _cur_race_name[16];

    void __cdecl Convert_Player_To_Flags_(s_player* player);

    void __cdecl Convert_Flags_To_Player_(s_player* player);

    void __cdecl Add_Race_Options_Fields_();

    int16_t __cdecl Real_Ability_Cost_(int16_t ability_idx);

    void __cdecl Calculate_Picks_();

    void __cdecl Draw_Racial_Option_Screen_();

    void __cdecl Draw_Race_Option_Background_();

    void __cdecl Reload_Option_Strings_();

    void __cdecl Reload_Racial_Option_Screen_();

    void __cdecl Convert_Player_To_Flags_EM_(int8_t *player_flags);

    void __cdecl Convert_Flags_To_Player_EM_(int8_t *flags_out);

    void __cdecl Set_Race_Option_Help_List_();

    void __cdecl Add_Race_Options_Fields_EM_(int8_t *flags, int16_t picks);

    void __cdecl Setup_Evolutionary_Upgrade_(short player_idx);

    void __cdecl Evolutionary_Mutation_Screen_(int8_t* traits);

    int16_t __cdecl Player_Score_Modifier_(int16_t player_idx);

    void __cdecl Calculate_EM_Picks_();
}