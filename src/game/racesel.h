#pragma once

namespace RACESEL {
    inline int16_t& _custom_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A577E});
    inline int16_t& _displayed_race = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A5780});
    inline int16_t& _old_race = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A5720});
    inline int16_t* _race_button_status = reinterpret_cast<int16_t*>(std::uintptr_t{0x005A56F0});

    inline uint8_t* (&_race_specials)[14] =
        *reinterpret_cast<uint8_t*(*)[14]>(std::uintptr_t{0x005A5730});

    inline s_animation_header* (&_race_button)[14] =
        *reinterpret_cast<s_animation_header*(*)[14]>(std::uintptr_t{0x005A5790});

    inline char (&_last_race_name)[15] =
        *reinterpret_cast<char(*)[15]>(std::uintptr_t{0x005A5710});

    inline s_animation_header*& _last_race_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A5724});

    inline uint8_t* _color_flag = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A56E8});
    inline uint8_t* _race_flag = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A5770});

    inline const char* s_patch13_lbx = reinterpret_cast<const char*>(0x00556330);
    inline const char* s_lastrace_rac = reinterpret_cast<const char*>(0x00556340);
    inline const char* s_RACESEL_LBX = reinterpret_cast<const char*>(0x00557FE4);
    inline const char* s_RACEOPT_LBX_00556320 = reinterpret_cast<const char*>(0x00556320);

    inline void __cdecl Convert_Custom_Race_Flags_(uint8_t* race_specials_ptr) {
        using Fn = void(__cdecl*)(uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00498160});
        fn(race_specials_ptr);
    }

    void __cdecl Reload_Race_Selection_Screen_();
    void __cdecl Draw_Race_Selection_Screen_();
    uint8_t __cdecl Naming_Popup_(char* name, int16_t race_idx);
    int16_t __cdecl Flag_Screen_(uint8_t* color_ptr);
    int16_t __cdecl Racial_Option_Screen_(s_player* plr);
    short Race_Selection_Screen_(s_player *plr);
}