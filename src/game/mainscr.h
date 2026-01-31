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

    void __cdecl Draw_Paralax_();

    void __cdecl Draw_Relocation_Links_();

    void __cdecl Draw_Black_Holes_();

    void __cdecl Draw_Main_Main_Screen_();

    inline void __cdecl Print_A_Star_Name_(int16_t star_idx, int32_t type, char* name_str, uint32_t data) {
        using Fn = void(__cdecl*)(int16_t, int32_t, char*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004ca720 });
        fn(star_idx, type, name_str, data);
    }

    inline uint8_t __cdecl Print_Star_Name_To_Bitmap_(int16_t star_idx, char* str, void* p3) {
        using Fn = uint8_t(__cdecl*)(int16_t, char*, void*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004CB910})(star_idx, str, p3);
    }

    void __cdecl Get_Star_Name_(int16_t star_idx, int8_t is_selected, int8_t* param_3, char* str, void* param_5);

    inline uint8_t& _already_faded_into_main_screen = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596180});

    inline void __cdecl Add_Map_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c4430});
        fn();
    }

    inline void __cdecl Do_System_Popup_(uint16_t off_flag) {
        using Fn = void(__cdecl*)(uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c4fe0});
        fn(off_flag);
    }

    void __cdecl Fade_Into_Main_Screen_();

    void __cdecl Print_Star_Names_();

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

    void __cdecl Set_Post_Fitted_Font_Style_(uint8_t param_1, uint8_t param_2);

    void __cdecl Get_Research_Unit_String_(char* buffer);

    void __cdecl Print_Main_Screen_Data_();
}