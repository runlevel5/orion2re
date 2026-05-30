#pragma once

struct s_colors;
struct s_player;

namespace raceopt {
    inline uint16_t& raceopt_flag_005a7ab6 = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A7AB6});
    inline int16_t (&_trait_option_flags)[32] = *reinterpret_cast<int16_t(*)[32]>(std::uintptr_t{0x005A7AAC});
    inline int16_t& _negative_picks = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A78E0});
    inline int16_t& _exit_ken = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A78E2});
    inline s_colors& _color_opt = *reinterpret_cast<s_colors*>(std::uintptr_t{0x005A78E8});
    inline int8_t*& _ability_costs = *reinterpret_cast<int8_t**>(std::uintptr_t{0x005A78F0});
    inline char* (&_opt_string)[64] = *reinterpret_cast<char* (*)[64]>(std::uintptr_t{0x005A7900});
    inline uint8_t*& _accept_button_seg = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x005A7A00});
    inline uint8_t*& _default_button_seg = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x005A7A04});
    inline int16_t (&_option_button)[22] = *reinterpret_cast<int16_t(*)[22]>(std::uintptr_t{0x005A7A10});
    inline int16_t& _defalt = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A7A50});
    inline int16_t (&_group_button)[41] = *reinterpret_cast<int16_t(*)[41]>(std::uintptr_t{0x005A7A60});
    inline int16_t& _accept = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A7AB2});
    inline int16_t& _picks = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A7AB4});
    inline s_animation_header*& _off_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A7AB8});
    inline int16_t (&_option_toggle)[24] = *reinterpret_cast<int16_t(*)[24]>(std::uintptr_t{0x005A7AC0});
    inline int16_t (&_button_variable)[10] = *reinterpret_cast<int16_t(*)[10]>(std::uintptr_t{0x005A7AF0});
    inline s_animation_header*& _special_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A7B04});
    inline char* const s__00551100 = reinterpret_cast<char*>(std::uintptr_t{0x00551100});
    inline int16_t& _name_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A78E4});
    inline char (&_cur_race_name)[16] = *reinterpret_cast<char (*)[16]>(std::uintptr_t{0x005A7A40});

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