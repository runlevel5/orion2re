#pragma once

namespace EVANHELP {
    inline s_help_box(&_main_screen_help_list)[25] =
        *reinterpret_cast<s_help_box(*)[25]>(std::uintptr_t{ 0x00552428 });

    inline uint32_t& _static_null_modem_setup_screen_help_list =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00552C50 });

    inline s_help_box(&_null_modem_setup_screen_help_list)[4] =
        *reinterpret_cast<s_help_box(*)[4]>(std::uintptr_t{ 0x005649D8 });

    inline s_help_box (&_static_design_screen_help_list)[13] =
        *reinterpret_cast<s_help_box(*)[13]>(std::uintptr_t{0x005525C0});

    inline s_help_box (&_design_screen_help_list)[13] =
        *reinterpret_cast<s_help_box(*)[13]>(std::uintptr_t{0x00564950});

    inline s_help_box(&_main_game_screen_help_list)[12] =
        *reinterpret_cast<s_help_box(*)[12]>(std::uintptr_t{ 0x00552648 });

    inline s_help_box(&_main_menu_screen_help_list)[6] =
        *reinterpret_cast<s_help_box(*)[6]>(std::uintptr_t{ 0x00552B48 });

    inline s_help_box(&_multi_player_setup_screen_help_list)[10] =
        *reinterpret_cast<s_help_box(*)[10]>(std::uintptr_t{ 0x00552B88 });

    inline s_help_box(&_load_game_screen_help_list)[7] =
        *reinterpret_cast<s_help_box(*)[7]>(std::uintptr_t{ 0x005526D0 });

    inline s_help_box(&_options_game_screen_help_list)[18] =
        *reinterpret_cast<s_help_box(*)[18]>(std::uintptr_t{ 0x00552760 });

    inline s_help_box(&_save_game_screen_help_list)[7] =
        *reinterpret_cast<s_help_box(*)[7]>(std::uintptr_t{ 0x00552718 });

    inline s_help_box(&_plntsum_screen_help_list)[15] =
        *reinterpret_cast<s_help_box(*)[15]>(std::uintptr_t{ 0x00552528 });

    inline s_help_box(&_input_box_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{ 0x00552C78 });

    void __cdecl Set_Load_Game_Screen_Help_List_();

    void __cdecl Set_Options_Game_Screen_Help_List_();

    void __cdecl Set_Save_Game_Screen_Help_List_();

    inline void __cdecl Set_Diplomacy_Request_Help_List_(int32_t x, int32_t y) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041E410});
        fn(x, y);
    }

    void __cdecl Set_PlntSum_Screen_Help_List_();

    void __cdecl Set_ColOff_Screen_Help_List_(int16_t* skills);

    void __cdecl Set_ShipOff_Screen_Help_List_(int16_t* help_y_offsets, int16_t start_index);

    void __cdecl Set_Main_Screen_Help_List_();

    void __cdecl Set_Design_Screen_Help_List_(int16_t lines_1, int16_t lines_2);

    void __cdecl Set_Officer_Screen_Help_();
    void __cdecl Get_Application_Description_(char* dest, int16_t item_index);

    void __cdecl Set_Main_Game_Screen_Help_List_();

    void __cdecl Set_Multi_Player_Setup_Screen_Help_List_();
    void __cdecl Set_Null_Modem_Setup_Screen_Help_List_(int16_t x_offset, int16_t y_offset);

    inline s_help_box*& _prev_help_struct_pointer =
        *reinterpret_cast<s_help_box**>(std::uintptr_t{0x00564848});
    inline int16_t& _prev_help_list_active =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00564B78});
    inline int16_t& _prev_help_list_count =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00564D60});

    void __cdecl Save_Help_List_();

    void __cdecl Set_Input_Box_Help_List_();

    void __cdecl Restore_Help_List_();

    inline s_help_box(&_static_fleet_screen_help_list)[32] =
        *reinterpret_cast<s_help_box(*)[32]>(std::uintptr_t{ 0x00552818 });

    inline s_help_box (&_fleet_screen_help_list)[32] =
        *reinterpret_cast<s_help_box(*)[32]>(std::uintptr_t{ 0x00564B80 });

    inline s_help_box (&_static_coloff_screen_help_list)[16] = 
        *reinterpret_cast<s_help_box(*)[16]>(std::uintptr_t{0x00552958});

    inline s_help_box (&_coloff_screen_help_list)[16] = 
        *reinterpret_cast<s_help_box(*)[16]>(std::uintptr_t{0x00564CC0});

    inline s_help_box (&_static_shipoff_screen_help_list)[33] =
        *reinterpret_cast<s_help_box(*)[33]>(std::uintptr_t{0x005529F8});

    inline s_help_box (&_shipoff_screen_help_list)[33] =
        *reinterpret_cast<s_help_box(*)[33]>(std::uintptr_t{0x00564A00});

    void __cdecl Append_Help_List_(s_help_box* help_list, int16_t* list_count, int32_t unused, int16_t help_id, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Set_Fleet_Screen_Help_List_(int16_t param_1);

    inline s_help_box(&_scrap_freighters_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{ 0x005526C0 });

    void __cdecl Set_Scrap_Freighters_Help_List_();

    void __cdecl Set_Weapon_Replacement_Screen_Help_List_(int16_t total_rows, int16_t x, int16_t y, uint8_t needs_racks);

    inline s_help_box(&_weapon_replacement_screen_help_list)[12] =
        *reinterpret_cast<s_help_box(*)[12]>(std::uintptr_t{ 0x005648D8 });

    inline s_help_box(&_static_hotseat_setup_screen_help)[4] =
        *reinterpret_cast<s_help_box(*)[4]>(std::uintptr_t{ 0x00552CB0 });

    inline s_help_box(&_hotseat_setup_screen_help_list)[4] =
        *reinterpret_cast<s_help_box(*)[4]>(std::uintptr_t{0x00564850});

    void __cdecl Set_Hotseat_Setup_Screen_Help_List_(int16_t x_offset, int16_t y_offset);

    void __cdecl Offset_Help_Field_XYs_(s_help_box* help_boxes, int16_t count, int16_t offset_x, int16_t offset_y);
}