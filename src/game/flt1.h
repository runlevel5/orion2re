#pragma once

namespace FLT1 {
    void __cdecl Draw_Box_Around_Scanned_Ship_(int16_t icon_idx);

    void __cdecl Draw_Fltscrn_Big_Ship_Icons_(int16_t param_1);

    void __cdecl Draw_Fltscrn_Relocation_Lines_(int16_t param_1);

    int16_t __cdecl First_Selected_Ship_();

    int16_t __cdecl F_Ship_Icon_From_Node_Ptr_(int16_t node_idx);

    void __cdecl Draw_Fltscrn_Ship_Destination_Lines_(int16_t param_1);

    void __cdecl Draw_Fltscrn_Small_Ship_Icons_();

    void __cdecl Draw_Flashing_Small_Ship_Icon_();

    void __cdecl Fill_FltScrn_Scroll_Bar_();

    void __cdecl Fleet_Screen_();

    void __cdecl Initialize_Scroll_Bar_(s_scroll_bar* sb);

    int16_t __cdecl Scan_Small_Ship_Icons_(int16_t param_1, int16_t param_2, int16_t* param_3, int16_t* param_4);

    void __cdecl Allocate_Fltscrn_Next_Screen_Seg_();

    void __cdecl Set_Small_Ship_Stack_Pointer_(bool check_box);

    void* __cdecl Load_Fltscrn_Officer_Picture_(int16_t player_num, int16_t leader_idx, void* buffer, uint8_t reload_flag);

    void __cdecl Reload_Small_Ship_Icons_();

    void __cdecl Load_Galaxy_Stars_();

    void __cdecl Reload_Fleet_Screen_();

    void __cdecl Draw_Fleet_Screen_();

    void __cdecl Add_Fleet_Screen_Fields_(int16_t* relocate_fleet_field_id, int16_t* scrap_button_field_id,
                                          int16_t* fleet_return_button_field_id, int16_t* up_scroll_button_field_id,
                                          int16_t* down_scroll_button_field_id, int16_t* next_fleet_button_field_id,
                                          int16_t* prev_fleet_button_field_id, int16_t* officers_button_field_id,
                                          int16_t* all_button_field_id, int16_t* big_icon_field_ids);

    bool __cdecl At_Least_One_Officer_(int16_t player_num);

    extern const char s_empty[];
    extern const char s_plus[];
    extern const char s_minus[];
    extern const char s_space[];
    extern const char s_C[];
    extern const char s_L[];
    extern const char s_dot[];
    extern const char s_comma[];
    extern const char s_S[];
    extern const char s_A[];
    extern const char s_R[];
    extern const char s_U[];

    void __cdecl Cancel_Star_Relocation_(int16_t star_id);

    void __cdecl Star_Relocation_(int16_t* star_from_ptr, int16_t* star_to_ptr, int16_t star_idx);

    bool __cdecl Okay_To_Set_Relocate_Star_(int16_t star_idx, uint8_t prompt_flag);

    uint8_t __cdecl Okay_To_Set_Relocate_Star2_(int16_t star_idx);

    uint8_t __cdecl Okay_To_Set_Relocate_To_Star_(int16_t src_star_idx, int16_t dst_star_idx);

    void __cdecl Cant_Order_Ship_Message_(const s_ship_data* ship);

    void __cdecl Set_Fltscrn_Big_Icons_(int16_t select_flag);

    void __cdecl Build_Scrap_List_(int16_t* scrap_list, int16_t* list_count);

    int8_t __cdecl Scrap_Ships_(int16_t player_idx, int16_t* ship_indices, int16_t num_ships, int8_t interactive);

    int16_t __cdecl Get_Scrap_Ship_Value_();

    void __cdecl Get_Technology_From_Scrapped_Ship_(s_ship_data* ship, int16_t player_id, char show_msg);

    void __cdecl Plunder_Ships_For_Tech_(int16_t* ship_indices, int16_t count, int8_t flag);

    int16_t __cdecl N_Fltscrn_Ships_Selected_();

    void __cdecl Next_Ship_Icon_();

    void __cdecl Previous_Ship_Icon_();

    int16_t __cdecl Update_Selection_Flags_();

    extern uint8_t _relocate_button_mode;
    extern int16_t _fleet_screen_bad_ship_id;
    extern int16_t _combat_filter_button;
    extern int16_t _n_player_ships;
    extern int16_t _support_filter_button;
    extern void* _multi_button_support_seg;
    extern uint8_t _merging_relocations;
    extern void* _multi_button_combat_seg;
    extern s_mouse_list _mouse_list_from[4];
    extern s_mouse_list _mouse_list_to[4];
    extern s_mouse_list _mouse_list_all_to[4];
    inline const char* PTR_s_FLEET_LBX = "fleet.lbx";
}