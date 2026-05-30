#pragma once

#include <cstdint>

struct s_help_box;

namespace EVANHELP {
    extern s_help_box _main_screen_help_list[25];

    extern s_help_box _static_null_modem_setup_screen_help_list[4];

    extern s_help_box _null_modem_setup_screen_help_list[4];

    extern s_help_box _static_design_screen_help_list[13];

    extern s_help_box _design_screen_help_list[13];

    extern s_help_box _main_game_screen_help_list[12];

    extern s_help_box _main_menu_screen_help_list[6];

    extern s_help_box _multi_player_setup_screen_help_list[10];

    extern s_help_box _load_game_screen_help_list[7];

    extern s_help_box _options_game_screen_help_list[18];

    extern s_help_box _save_game_screen_help_list[7];

    extern s_help_box _plntsum_screen_help_list[15];

    extern s_help_box _input_box_help_list[1];

    extern s_help_box _static_diplomacy_request_help_list[4];

    extern s_help_box _static_diplomacy_request_help_list2[4];

    void __cdecl Set_Load_Game_Screen_Help_List_();

    void __cdecl Set_Options_Game_Screen_Help_List_();

    void __cdecl Set_Save_Game_Screen_Help_List_();

    void __cdecl Set_Diplomacy_Request_Help_List_(int16_t x_offset, int16_t y_offset);

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

    extern s_help_box* _prev_help_struct_pointer;
    extern int16_t _prev_help_list_active;
    extern int16_t _prev_help_list_count;

    void __cdecl Save_Help_List_();

    void __cdecl Set_Input_Box_Help_List_();

    void __cdecl Restore_Help_List_();

    extern s_help_box _static_fleet_screen_help_list[32];

    extern s_help_box _fleet_screen_help_list[32];

    extern s_help_box _static_coloff_screen_help_list[16];

    extern s_help_box _coloff_screen_help_list[16];

    extern s_help_box _static_shipoff_screen_help_list[33];

    extern s_help_box _shipoff_screen_help_list[33];

    void __cdecl Append_Help_List_(s_help_box* help_list, int16_t* list_count, int32_t unused, int16_t help_id, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Set_Fleet_Screen_Help_List_(int16_t param_1);

    extern s_help_box _scrap_freighters_help_list[1];

    void __cdecl Set_Scrap_Freighters_Help_List_();

    void __cdecl Set_Weapon_Replacement_Screen_Help_List_(int16_t total_rows, int16_t x, int16_t y, uint8_t needs_racks);

    extern s_help_box _weapon_replacement_screen_help_list[12];

    extern s_help_box _static_hotseat_setup_screen_help[4];

    extern s_help_box _hotseat_setup_screen_help_list[4];

    void __cdecl Set_Hotseat_Setup_Screen_Help_List_(int16_t x_offset, int16_t y_offset);

    void __cdecl Offset_Help_Field_XYs_(s_help_box* help_boxes, int16_t count, int16_t offset_x, int16_t offset_y);
}