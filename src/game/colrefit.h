#pragma once

namespace COLREFIT {
#pragma pack(push, 1)
    struct s_scroll_state {
        int32_t data[6];
    };
#pragma pack(pop)

    inline int16_t& _first_ship =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00558368});

    inline int16_t& _slider_bar_position =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055836C});

    inline int16_t (&_fields)[12] =
        *reinterpret_cast<int16_t(*)[12]>(std::uintptr_t{0x0056C450});

    inline int16_t& _esc =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00558370});

    inline int16_t& _scanned_ship_index =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00558388});

    inline s_animation_header* (&_anims)[4] =
        *reinterpret_cast<s_animation_header* (*)[4]>(std::uintptr_t{0x00558378});

    inline int16_t& _scanned_ship_d_index = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055838C});
    inline int16_t& _lscanned_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00558390});

    inline const char* s_refitpup_LBX_00558394 = reinterpret_cast<const char*>(std::uintptr_t{0x00558394});

    void __cdecl Update_First_Ship_(int16_t* first_ship_ptr, int16_t* slider_pos_ptr);

    inline int16_t*& _g_ship_field_list_ptr =
        *reinterpret_cast<int16_t**>(std::uintptr_t{0x0056C438});

    inline MOX2::action_func_t& _saved_autofunction =
        *reinterpret_cast<MOX2::action_func_t*>(std::uintptr_t{0x0056C43C});

    inline int16_t*& _g_ship_list_ptr =
        *reinterpret_cast<int16_t**>(std::uintptr_t{0x0056C440});

    inline int16_t& _n_ships_in_list =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056C448});

    inline int16_t& _g_ship_n =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056C468});

    inline s_scroll_state*& _g_scroll_ptr =
        *reinterpret_cast<s_scroll_state**>(std::uintptr_t{0x005A5320});

    void __cdecl Build_Ship_List_();

    void __cdecl Load_Refit_Ship_Anims_();

    void __cdecl Draw_Refit_Popup_();

    void __cdecl Draw_Bar_Indicator_();

    void __cdecl Draw_Ship_List_();

    void __cdecl Draw_Ship2_(int16_t ship_idx, int16_t position_idx);

    void __cdecl Scan_Ship_Info_(int16_t ship_idx);

    void __cdecl Add_Ship_List_Fields_();

    void __cdecl Add_Fields_();

    void __cdecl Add2_Fields_();

    int16_t __cdecl Evaluate_Input_(int16_t* input_id, int16_t* scan_code);

    void __cdecl Load_Refit2_Ship_Anims_();

    void __cdecl Draw_Ship_D_List_();

    inline char* s_colon_005583a4 = reinterpret_cast<char*>(0x005583A4); // ":"

    void __cdecl Show_Ship_Design_(s_ship_design* design);

    void __cdecl Draw_Refit2_Popup_();

    void __cdecl Do_Add2_Fields_();

    int16_t __cdecl Evaluate2_Input_(int16_t* get_input, int16_t* scan_input);

    int16_t __cdecl Colony_Refit_Popup_();
    int16_t __cdecl Colony_Refit2_Popup_(int16_t ship_idx);
}