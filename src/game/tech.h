#pragma once
#include <cstdint>

struct s_list_item_research;
struct s_list_state;
struct s_colors;
struct s_animation_header;

namespace TECH {
    void __cdecl _Tech_Select_(char param_1);

    inline const char* _research_desc_fmt = reinterpret_cast<const char*>(std::uintptr_t{0x00557C28});
    int32_t __cdecl New_Get_Tech_Cost_(int16_t id, uint8_t is_tech_app, int16_t player_idx);

    inline s_tech_field_data* _technology_fields = reinterpret_cast<s_tech_field_data*>(0x00553050);
    inline int8_t (&_saved_hyper_advanced_tech)[8] = *reinterpret_cast<int8_t(*)[8]>(std::uintptr_t{0x005A6338});
    inline char* _tech_select_lbx = reinterpret_cast<char*>(std::uintptr_t{0x00557BF8});
    inline int32_t* _tech_select_pos = reinterpret_cast<int32_t*>(std::uintptr_t{0x00557C40});
    inline int32_t* _tech_change_pos = reinterpret_cast<int32_t*>(std::uintptr_t{0x00557C60});
    inline int16_t* _app_name_y = reinterpret_cast<int16_t*>(std::uintptr_t{0x00557C90});
    inline int16_t* _list_window_x_offsets = reinterpret_cast<int16_t*>(std::uintptr_t{0x00557CA0});
    inline int16_t* _tech_button_pos = reinterpret_cast<int16_t*>(std::uintptr_t{0x00557CB0});
    inline uint8_t* _entry_to_group = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00557CD0});
    inline s_colors* _tech_select_colors = reinterpret_cast<s_colors*>(std::uintptr_t{0x00557CD8});
    inline s_colors* _tech_change_colors = reinterpret_cast<s_colors*>(std::uintptr_t{0x00557D00});
    inline int16_t& _g_scrn_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005662B8});
    inline int16_t& _g_scrn_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005662BC});
    inline int16_t& _g_first_tech_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005662C0});
    inline int16_t& _g_last_tech_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005662E0});
    inline uint8_t& _g_cursor_box_color = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005662E4});
    inline uint8_t& _g_changing_tech = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005662E8});
    inline s_list_item_research*& _g_selected_entry_p = *reinterpret_cast<s_list_item_research**>(std::uintptr_t{0x005A626C});
    inline s_list_item_research*& _g_entries_p = *reinterpret_cast<s_list_item_research**>(std::uintptr_t{0x005A6340});
    inline s_colors* _tech_color = reinterpret_cast<s_colors*>(std::uintptr_t{0x005A6350});
    inline s_list_state& _g_tech_list_header = *reinterpret_cast<s_list_state*>(std::uintptr_t{0x00557D28});
    inline int16_t (&_tech_field_save)[32] = *reinterpret_cast<int16_t(*)[32]>(std::uintptr_t{0x005662C8});
    inline s_list_item*& _g_selected_list_entry_p = *reinterpret_cast<s_list_item**>(std::uintptr_t{0x005662DC});
    inline char (&_tech_group_names)[10][20] = *reinterpret_cast<char(*)[10][20]>(std::uintptr_t{0x005A6270});

    void __cdecl Restore_Hyper_Tech_Values_(int16_t player_idx);
    void __cdecl Init_Entry_Positions_(s_list_item_research* entries);
    int16_t __cdecl Init_Entry_Data_(s_list_item_research* param_1, int param_2);
    void __cdecl Setup_Entry_Buttons_(s_list_item_research* entries, s_animation_header** images);
    void __cdecl Draw_Tech_Select_();
    void __cdecl Display_Entry_Text_(s_list_item_research* item);
    void __cdecl Display_All_Entry_Text_(s_list_item_research* entries);
    int __cdecl Set_Selected_Entry_(s_list_item_research* research_list, int16_t field_id);
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
    inline char* s__005510f0 = reinterpret_cast<char*>(std::uintptr_t{0x005510F0}); // " "
    int16_t __cdecl Get_Hyper_Tech_App_ID_(int16_t tech_field_id);
    char* __cdecl Technology_Fields_Name_(int16_t tech_field_id);
    inline short (&_tech_field_y1)[4] = *reinterpret_cast<short(*)[4]>(0x00557C80);
    inline short (&_tech_field_y2)[4] = *reinterpret_cast<short(*)[4]>(0x00557C88);
}