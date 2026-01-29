#pragma once
#include <cstdint>

namespace MAINSCR {
    inline uint8_t& _check_next_turn_exit_field = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059613A});
    inline uint16_t& _n_twinkling_stars = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0056CEA8});

    void __cdecl Get_PointXY_Draw_Coords_(int16_t x, int16_t y, int16_t* out_x, int16_t* out_y);

    void __cdecl Get_Star_Picture_Seg_(int8_t spectral_class, uint32_t param_2, int8_t param_3);

    void __cdecl Get_Star_Draw_Coords_(int16_t star_idx, int16_t* out_x, int16_t* out_y);

    bool __cdecl Star_On_Screen_(int16_t star_idx);

    void __cdecl Draw_Stars_();

    inline void __cdecl Draw_A_Star_(int16_t star_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004C6EB0 });
        fn(star_idx);
    }

    void __cdecl Update_Star_Owners_();

    void __cdecl Draw_Mini_Main_Screen_();

    void __cdecl Update_Ship_Icons_In_Between_Turns_();

    void __cdecl Draw_Wormhole_Links_();

    void __cdecl Draw_Relocation_Links_();

    inline void __cdecl Draw_Main_Main_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004C5700 });
        fn();
    }

    inline void __cdecl Draw_Main_Screen_Filled_(int16_t filled_flag) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004C6550 });
        fn(filled_flag);
    }
}