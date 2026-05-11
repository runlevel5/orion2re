#pragma once

namespace COLXPORT {
    inline uint8_t& _cheezy_hack_col_count_state =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005560C8});

    inline int16_t (&_cheezy_hack_col_count)[8] =
        *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x00564DE0});

    short __cdecl N_Colonies_(short player_idx);

    inline int16_t*& _g_colony_list_ptr = *reinterpret_cast<int16_t**>(std::uintptr_t{0x005A7B74});

    inline MOX2::action_func_t& _saved_autofunction = *reinterpret_cast<MOX2::action_func_t*>(std::uintptr_t{0x0056501C});

    inline int16_t (&_list_col)[8] = *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x00564DF0});

    inline int16_t (&_list_col_pop_ref)[252] = *reinterpret_cast<int16_t(*)[252]>(std::uintptr_t{0x00564E00});

    inline int16_t (&_list_col_coord_vars)[8] = *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x00565010});

    inline int16_t (&_list_col_pop_count)[8] = *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x00565020});

    inline int16_t (&_list_col_job_fields)[6] = *reinterpret_cast<int16_t(*)[6]>(std::uintptr_t{0x00565030});

    inline int16_t (&_list_fields)[6] = *reinterpret_cast<int16_t(*)[6]>(std::uintptr_t{0x00565000});

    inline int16_t& _x_fields = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00564FF8});
    inline int16_t (&_x_fields_array)[4] = *reinterpret_cast<int16_t(*)[4]>(std::uintptr_t{0x00564FF8});

    void __cdecl Build_Global_Colony_List_();

    void __cdecl Clear_List_Col_Array_();

    void __cdecl Draw_Xport_Popup_();

    void __cdecl Draw_Colony_Summary_For_Colony_(int16_t colony_idx, int16_t entry_idx);

    void __cdecl Draw_Bar_Indicator_();

    void __cdecl Draw_Info_Pop_For_(int16_t entry_index);

    void __cdecl Add_Fields_();

    inline s_animation_header* (&_anims)[4] =
        *reinterpret_cast<s_animation_header* (*)[4]>(std::uintptr_t{0x005560B8});

    void __cdecl Load_Xport_Anims_();

    short __cdecl Xport_Popup_();
}