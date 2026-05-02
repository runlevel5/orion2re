#pragma once

namespace RACESEL {
    inline int16_t& _custom_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A577E});
    inline int16_t& _displayed_race = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A5780});
    inline int16_t& _old_race = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A5720});
    inline int16_t (&_race_button_status)[14] = *reinterpret_cast<int16_t (*)[14]>(std::uintptr_t{0x005A56F0});

    inline uint8_t* (&_race_specials)[14] =
        *reinterpret_cast<uint8_t*(*)[14]>(std::uintptr_t{0x005A5730});

    inline s_animation_header* (&_race_button)[14] =
        *reinterpret_cast<s_animation_header*(*)[14]>(std::uintptr_t{0x005A5790});

    inline char (&_last_race_name)[15] =
        *reinterpret_cast<char(*)[15]>(std::uintptr_t{0x005A5710});

    inline s_animation_header*& _last_race_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A5724});

    inline uint8_t (&_color_flag)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005A56E8});
    inline uint8_t (&_race_flag)[14] = *reinterpret_cast<uint8_t(*)[14]>(std::uintptr_t{0x005A5770});

    inline const char* s_patch13_lbx = reinterpret_cast<const char*>(0x00556330);
    inline const char* s_lastrace_rac = reinterpret_cast<const char*>(0x00556340);
    inline const char* s_RACESEL_LBX = reinterpret_cast<const char*>(0x00557FE4);
    inline const char* s_rb_0055117C = reinterpret_cast<const char*>(0x0055117C);
    inline const char* s_empty_00551100 = reinterpret_cast<const char*>(0x00551100);
    inline const char* s_space_00551120 = reinterpret_cast<const char*>(0x00551120);

    void __cdecl Convert_Custom_Race_Flags_(uint8_t* race_flags);

    void __cdecl Reload_Race_Selection_Screen_();
    void __cdecl Draw_Race_Selection_Screen_();
    int8_t __cdecl Naming_Popup_(char* output_name, int16_t race_idx);
    int16_t __cdecl Flag_Screen_(uint8_t* color_ptr);
    int16_t __cdecl Racial_Option_Screen_(s_player* player);
    short Race_Selection_Screen_(s_player *plr);

    void __cdecl Set_Race_Select_Help_();
    void __cdecl Unrequest_Race_(uint8_t race_idx);
    void __cdecl Draw_Flag_Screen_();
    void __cdecl Set_Flag_Screen_Help_();

    inline char*& _temp_ruler_name = *reinterpret_cast<char**>(std::uintptr_t{0x005A57C8});

    void __cdecl Draw_Naming_Popup_();
    void __cdecl Message_Build_(int16_t* message_count, int32_t split_at_count, char* output);
    void __cdecl Draw_Race_Selection_Screen_();
}