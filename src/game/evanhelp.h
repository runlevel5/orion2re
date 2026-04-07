#pragma once
#include <cstdint>

namespace EVANHELP {
    inline uint8_t& _main_game_screen_help_list =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552648});

    inline uint8_t& _main_menu_screen_help_list = 
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552B48});

    inline uint8_t& _multi_player_setup_screen_help_list =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552B88});

    inline void __cdecl Set_Load_Game_Screen_Help_List_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0041e130})();
    }

    inline void __cdecl Set_Options_Game_Screen_Help_List_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0041e150})();
    }

    inline void __cdecl Set_Save_Game_Screen_Help_List_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0041e140})();
    }

    inline void __cdecl Set_PlntSum_Screen_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041DDF0});
        fn();
    }

    inline void __cdecl Set_Officer_Screen_Help_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00443E70 });
        fn();
    }

    inline void __cdecl Set_Main_Screen_Help_List_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0041dbb0})();
    }

    inline void __cdecl Set_Design_Screen_Help_List_(int16_t help1, int16_t help2) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0041DE00})(help1, help2);
    }

    void __cdecl Set_Officer_Screen_Help_();
    inline void __cdecl Get_Application_Description_(char* buffer, int16_t tech_id) {
        using Fn = void(__cdecl*)(char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041E4F0});
        fn(buffer, tech_id);
    }

    void __cdecl Set_Main_Game_Screen_Help_List_();

    void __cdecl Set_Multi_Player_Setup_Screen_Help_List_();
    inline void __cdecl Set_Null_Modem_Setup_Screen_Help_List_(int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0041e3c0 });
        fn(x, y);
    }

    inline void __cdecl Save_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0041e5e0 });
        fn();
    }

    inline void __cdecl Restore_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0041e610 });
        fn();
    }

    inline void __cdecl Set_Fleet_Screen_Help_List_(int16_t val) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0041e160})(val);
    }

    inline void __cdecl Set_Scrap_Freighters_Help_List_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x0041e120)();
    }

    inline void __cdecl Set_Weapon_Replacement_Screen_Help_List_(int16_t total_rows, int16_t x, int16_t y, bool needs_racks) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041dea0});
        fn(total_rows, x, y, needs_racks);
    }

    inline void __cdecl Set_Hotseat_Setup_Screen_Help_List_(int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0041e450})(x, y);
    }
}