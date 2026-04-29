#pragma once

struct s_animation_header;
struct s_invasion_side_info;

namespace NEWPUP {
    inline MOX2::action_func_t& _saved_autofunction =
        *reinterpret_cast<MOX2::action_func_t*>(std::uintptr_t{0x005663D0});

    inline s_animation_header*& _anims = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00557DCC});

    void __cdecl Print_Best_Summary_(s_invasion_side_info* side_info, int16_t* y_pos);

    void __cdecl Print_Race_Summary_(s_invasion_side_info* side_info, int16_t* summary_buffer);

    inline void __cdecl Print_Troop_Totals_(int32_t x, int32_t y, int16_t* info) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004874b0});
        fn(x, y, info);
    }

    void __cdecl Draw_Troop_Popup_();

    void __cdecl Reset_Fields_();

    inline const char* s__00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});

    void __cdecl Troop_Popup_();
}