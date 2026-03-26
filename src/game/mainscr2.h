#pragma once
#include <cstdint>

struct s_mouse_list;

namespace MAINSCR2 {
    inline s_mouse_list* _mouse_list_from = reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x00559A90});
    inline s_mouse_list* _mouse_list_to = reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x00559AD0});

    inline void __cdecl Draw_Main_Screen_For_Star_Distan() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004d9f30});
        fn();
    }
    inline uint8_t& _main_screen_fade_in_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596013});
    inline uint8_t& _force_main_screen_fade = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596014});

    inline uint8_t& _in_reports_phase = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596012});

    inline void __cdecl Begin_Of_Turn_(int8_t* out_cancel) {
        using Fn = void(__cdecl*)(int8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004DABF0});
        fn(out_cancel);
    }

    void __cdecl Distance_Between_Stars_Screen_();
    void __cdecl Planet_Colonization_In_Main_Screen_();
    void __cdecl Building_Outposts_In_Main_Screen_();
    void __cdecl Unloading_Transports_In_Main_Screen_();
    void __cdecl Scrap_Freighters_();
    void __cdecl Do_Begin_Of_Turn_();
    void __cdecl Reports_Screen_();
    void Show_Command_Points_Screen_();
}