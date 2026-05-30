#pragma once
#include <cstdint>

struct s_list_item_research;
struct s_list_item;
struct s_list_state;
struct s_colors;
struct s_animation_header;
struct s_fieldsav_data;

namespace TECH {
    void __cdecl Tech_Select_();
    extern char s__00559488[64];

    void __cdecl _Tech_Select_(uint8_t arg0);

    void __cdecl Reset_Screen_();

    extern const char _research_desc_fmt[19];
    int32_t __cdecl New_Get_Tech_Cost_(int16_t id, uint8_t is_tech_app, int16_t player_idx);

    extern uint8_t _saved_hyper_advanced_tech[8];
    extern const char _tech_select_lbx[12];
    extern int32_t _tech_select_pos[8];
    extern int32_t _tech_change_pos[8];
    extern int16_t _app_name_y[8];
    extern int16_t _list_window_x_offsets[8];
    extern int16_t _tech_button_pos[16];
    extern uint8_t _entry_to_group[8];
    extern s_colors _tech_select_colors[5];
    extern s_colors _tech_change_colors[5];
    extern int16_t _g_scrn_y;
    extern int16_t _g_scrn_x;
    extern int16_t _g_first_tech_field;
    extern int16_t _g_last_tech_field;
    extern uint8_t _g_cursor_box_color;
    extern uint8_t _g_changing_tech;
    extern s_list_item_research* _g_selected_entry_p;
    extern s_list_item_research* _g_entries_p;
    extern s_colors _tech_color[5];
    extern s_list_state _g_tech_list_header;
    extern s_fieldsav_data _tech_field_save;
    extern s_list_item* _g_selected_list_entry_p;
    extern char _tech_group_names[10][20];

    void __cdecl Restore_Hyper_Tech_Values_(int16_t player_idx);
    void __cdecl Init_Entry_Positions_(s_list_item_research* entries);
    int16_t __cdecl Init_Entry_Data_(s_list_item_research* param_1, int param_2);
    void __cdecl Setup_Entry_Buttons_(s_list_item_research* entries, s_animation_header** images);
    void __cdecl Draw_Tech_Select_();
    void __cdecl Display_Entry_Text_(s_list_item_research* item);
    void __cdecl Display_All_Entry_Text_(s_list_item_research* entries);
    s_list_item_research* __cdecl Set_Selected_Entry_(s_list_item_research* research_list, int16_t field_id);
    s_list_item_research* __cdecl Get_Selected_Entry_(s_list_item_research* entries);
    void __cdecl Display_Selected_Entry_Text_(s_list_item_research* entry);
    void __cdecl Draw_Little_Arrow_(s_list_item_research* entry);
    void __cdecl Draw_Application_Description_(int32_t param1, uint16_t app_id);
    int16_t __cdecl _Tech_List_(s_list_item_research* tech_research, uint8_t mode);

    void __cdecl Get_Group_List_(s_list_state* list, int16_t group_id);

    void __cdecl Draw_Tech_List_();

    void __cdecl Tech_Change_();

    void __cdecl Save_Hyper_Tech_Values_(int16_t player_idx);

    void __cdecl Promote_Hyper_Field_Values_(int16_t player_idx);

    char* __cdecl Technology_Applications_Name_(int16_t param_1, int16_t param_2);
    int16_t __cdecl Get_Hyper_Tech_App_ID_(int16_t tech_field_id);
    char* __cdecl Technology_Fields_Name_(int16_t tech_field_id);
    extern int16_t _tech_field_y1[4];
    extern int16_t _tech_field_y2[4];
}