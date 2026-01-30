#pragma once
#include <cstdint>

namespace MAINSCR {
    inline uint8_t& _check_next_turn_exit_field = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059613A});
    inline int16_t& _n_twinkling_stars = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CEA8});
    inline int16_t& _main_screen_ship_icon_scanned =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596188});
    inline int8_t (&_diplo_lights_anim_frame)[8] = *reinterpret_cast<int8_t(*)[8]>(std::uintptr_t{0x0056CEB8});
    inline animate::s_animation_header* (&_player_lights_seg)[8] = *reinterpret_cast<animate::s_animation_header*(*)[8]>(std::uintptr_t{0x00596110});

    void __cdecl Get_PointXY_Draw_Coords_(int16_t x, int16_t y, int16_t* out_x, int16_t* out_y);

    animate::s_animation_header* __cdecl Get_Star_Picture_Seg_(int8_t spectral_class, int8_t pict_type, int8_t size);

    void __cdecl Get_Star_Draw_Coords_(int16_t star_idx, int16_t* out_x, int16_t* out_y);

    bool __cdecl Star_On_Screen_(int16_t star_idx);

    void __cdecl Draw_Stars_();

    void __cdecl Draw_A_Star_(int16_t star_idx);

    void __cdecl Update_Star_Owners_();

    void __cdecl Draw_Mini_Main_Screen_();

    void __cdecl Update_Ship_Icons_In_Between_Turns_();

    void __cdecl Draw_Wormhole_Links_();

    inline void __cdecl Draw_Paralax_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c6930});
        fn();
    }

    void __cdecl Draw_Relocation_Links_();

    void __cdecl Draw_Black_Holes_();

    void __cdecl Draw_Main_Main_Screen_();

    inline void __cdecl Print_Star_Names_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004cba80});
        fn();
    }

    void __cdecl Draw_Diplomacy_Request_Lights_();

    void __cdecl Draw_Main_Screen_Filled_(int16_t should_copy_to_back);

    inline void __cdecl Draw_A_Gate_Icon_(int32_t star_idx) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c59b0});
        fn(star_idx);
    }

    void __cdecl Draw_Gate_Icons_();

    void __cdecl Draw_Nebulae_();

    void __cdecl Draw_Ship_Icons_(bool force_draw);

    inline void __cdecl Set_Post_Fitted_Font_Style_(char c1, char c2) {
        using Fn = void(__cdecl*)(char, char);
        reinterpret_cast<Fn>(std::uintptr_t{0x004cb8b0})(c1, c2);
    }

    inline void __cdecl Get_Research_Unit_String_(char* buffer) {
        using Fn = void(__cdecl*)(char*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004cd930})(buffer);
    }

    void __cdecl Print_Main_Screen_Data_();
}