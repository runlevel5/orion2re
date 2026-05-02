#pragma once

namespace LIST {
    using draw_app_fn = void(*)(int16_t, int16_t, uint8_t);

    void __cdecl Display_List_Page_(s_list_state* state);

    void __cdecl Set_List_Page_Pointer_(s_list_state* state);

    void __cdecl Display_List_Text_(s_list_state* state, s_list_item* item);

    int16_t __cdecl Add_Fields_To_List_Page_(s_list_state* state);

    void __cdecl Set_List_Up_Down_Field_Drawing_(s_list_state* state, int16_t up_field_id, int16_t down_field_id);

    int8_t __cdecl Next_List_Page_(s_list_state* state);

    int8_t __cdecl Previous_List_Page_(s_list_state* state);

    s_list_item* __cdecl Set_Selected_List_Entry_(s_list_state* state, int16_t target_app_id);

    s_list_item* __cdecl Get_Selected_List_Entry_(s_list_state* state);

    void __cdecl Init_List_Data_(s_list_state* state);

    void __cdecl Display_Selected_List_Text_(s_list_state* state, s_list_item* item);

    int8_t __cdecl Goto_Page_With_Entry_(s_list_state* state, s_list_item* target_item);

    s_list_item* __cdecl Select_And_Page_By_Head_ID_(s_list_state* state, uint8_t head_id);

    void __cdecl Display_Selected_List_Text_For_Tech_List_(s_list_state* list_state, s_list_item* item);
}