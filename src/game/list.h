#pragma once
#include <cstdint>

struct s_list_item;
struct s_list_state;
struct s_colors;

namespace LIST {
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

    inline s_list_item* __cdecl Select_And_Page_By_Head_ID_(s_list_state* list, uint8_t id) {
        using Fn = s_list_item*(__cdecl*)(s_list_state*, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00460460});
        return fn(list, id);
    }

    inline void __cdecl Display_Selected_List_Text_For_Tech_List_(s_list_state* list, s_list_item* item) {
        using Fn = void(__cdecl*)(s_list_state*, s_list_item*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00460120});
        fn(list, item);
    }
}