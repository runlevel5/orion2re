#pragma once
#include <cstdint>

namespace EVANHELP {
    inline uint32_t& _static_null_modem_setup_screen_help_list =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00552C50 });

    inline uint32_t& _null_modem_setup_screen_help_list =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x005649D8 });

    inline uint32_t& _static_design_screen_help_list =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005525C0});

    inline uint8_t& _main_game_screen_help_list =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552648});

    inline uint8_t& _main_menu_screen_help_list = 
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552B48});

    inline uint8_t& _multi_player_setup_screen_help_list =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552B88});

    inline uint8_t& _load_game_screen_help_list =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005526D0});

    inline uint8_t& _options_game_screen_help_list =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552760});

    inline uint8_t& _save_game_screen_help_list =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552718});

    inline uint8_t& _plntsum_screen_help_list =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552528});

    inline uint8_t& _input_box_help_list =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552C78});

    void __cdecl Set_Load_Game_Screen_Help_List_();

    void __cdecl Set_Options_Game_Screen_Help_List_();

    void __cdecl Set_Save_Game_Screen_Help_List_();

    void __cdecl Set_PlntSum_Screen_Help_List_();

    inline void __cdecl Set_ColOff_Screen_Help_List_(int32_t* skills) {
        using Fn = void(__cdecl*)(int32_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041e240});
        fn(skills);
    }

    inline void __cdecl Set_ShipOff_Screen_Help_List_(int32_t* skills, int16_t n_added) {
        using Fn = void(__cdecl*)(int32_t*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041e2a0});
        fn(skills, n_added);
    }

    inline void __cdecl Set_Main_Screen_Help_List_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0041dbb0})();
    }

    void __cdecl Set_Design_Screen_Help_List_(int16_t count_1, int16_t count_2);

    void __cdecl Set_Officer_Screen_Help_();
    void __cdecl Get_Application_Description_(char* dest, int16_t item_index);

    void __cdecl Set_Main_Game_Screen_Help_List_();

    void __cdecl Set_Multi_Player_Setup_Screen_Help_List_();
    void __cdecl Set_Null_Modem_Setup_Screen_Help_List_(int16_t x_offset, int16_t y_offset);

    inline void*& _prev_help_struct_pointer =
        *reinterpret_cast<void**>(std::uintptr_t{0x00564848});
    inline int16_t& _prev_help_list_active =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00564B78});
    inline int16_t& _prev_help_list_count =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00564D60});

    void __cdecl Save_Help_List_();

    void __cdecl Set_Input_Box_Help_List_();

    void __cdecl Restore_Help_List_();

    inline uint32_t* _static_fleet_screen_help_list = reinterpret_cast<uint32_t*>(std::uintptr_t{0x00552818});
    inline uint32_t* _fleet_screen_help_list = reinterpret_cast<uint32_t*>(std::uintptr_t{0x00564B80});

    inline void __cdecl Append_Help_List_(void* list, int16_t* count, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8) {
        using Fn = void(__cdecl*)(void*, int16_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041E490});
        fn(list, count, a3, a4, a5, a6, a7, a8);
    }

    void __cdecl Set_Fleet_Screen_Help_List_(int16_t param_1);

    inline int16_t& _scrap_freighters_help_list =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005526C0});

    void __cdecl Set_Scrap_Freighters_Help_List_();

    void __cdecl Set_Weapon_Replacement_Screen_Help_List_(int16_t total_rows, int16_t x, int16_t y, uint8_t needs_racks);

    inline int16_t (&_weapon_replacement_screen_help_list)[12][10] =
        *reinterpret_cast<int16_t(*)[12][10]>(std::uintptr_t{0x005648D8});

    inline uint32_t (&_static_hotseat_setup_screen_hel)[10] =
        *reinterpret_cast<uint32_t(*)[10]>(std::uintptr_t{0x00552CB0});

    inline uint32_t (&_hotseat_setup_screen_help_list)[10] =
        *reinterpret_cast<uint32_t(*)[10]>(std::uintptr_t{0x00564850});

    void __cdecl Set_Hotseat_Setup_Screen_Help_List_(int16_t x_offset, int16_t y_offset);

    void __cdecl Offset_Help_Field_XYs_(void* help_list, int32_t count, int16_t x_offset, int16_t y_offset);
}