#pragma once

namespace COLXPORT {
    extern uint8_t _cheezy_hack_col_count_state;

    extern int16_t _cheezy_hack_col_count[8];

    short __cdecl N_Colonies_(short player_idx);

    extern int16_t* _g_colony_list_ptr;

    extern MOX2::action_func_t _saved_autofunction;

    extern int16_t _list_col[6];

    extern s_colsum_col_pop_ref _list_col_pop_ref[6];

    extern int16_t _list_col_coord_vars[6];

    extern int16_t _list_col_pop_count[6];

    extern int16_t _list_col_job_fields[6];

    extern int16_t _list_fields[6];

    extern int16_t _x_fields_array[4];

    void __cdecl Build_Global_Colony_List_();

    void __cdecl Clear_List_Col_Array_();

    void __cdecl Draw_Xport_Popup_();

    void __cdecl Draw_Colony_Summary_For_Colony_(int16_t colony_idx, int16_t entry_idx);

    void __cdecl Draw_Bar_Indicator_();

    void __cdecl Draw_Info_Pop_For_(int16_t entry_index);

    void __cdecl Add_Fields_();

    extern s_animation_header* _anims[4];

    void __cdecl Load_Xport_Anims_();

    short __cdecl Xport_Popup_();
}