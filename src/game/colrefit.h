#pragma once

namespace COLREFIT {
#pragma pack(push, 1)
    struct s_scroll_state {
        int32_t data[6];
    };
#pragma pack(pop)

    extern int16_t _first_ship;

    extern int16_t _slider_bar_position;

    extern int16_t _fields[12];

    extern int16_t _esc;

    extern int16_t _scanned_ship_index;

    extern s_animation_header* _anims[4];

    extern int16_t _scanned_ship_d_index;
    extern int16_t _lscanned_field;

    extern const char s_refitpup_LBX_00558394[13]; // "refitpup.LBX"

    void __cdecl Update_First_Ship_(int16_t* first_ship_ptr, int16_t* slider_pos_ptr);

    extern int16_t* _g_ship_field_list_ptr;

    extern MOX2::action_func_t _saved_autofunction;

    extern int16_t* _g_ship_list_ptr;

    extern int16_t _n_ships_in_list;

    extern int16_t _g_ship_n;

    extern s_scroll_state* _g_scroll_ptr;

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

    extern char s_colon_005583a4[2]; // ":"

    void __cdecl Show_Ship_Design_(s_ship_design* design);

    void __cdecl Draw_Refit2_Popup_();

    void __cdecl Do_Add2_Fields_();

    int16_t __cdecl Evaluate2_Input_(int16_t* get_input, int16_t* scan_input);

    int16_t __cdecl Colony_Refit_Popup_();
    int16_t __cdecl Colony_Refit2_Popup_(int16_t ship_idx);
}