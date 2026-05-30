#pragma once
#include <cstdint>

struct s_animation_header;
struct s_colsum_col_pop_ref;

namespace COLSUM {
    inline int16_t (&_x_fields)[14] = *reinterpret_cast<int16_t(*)[14]>(std::uintptr_t{0x00551040});
    inline int16_t& _g_sort_index = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055105C});
    inline s_animation_header* (&_anims)[21] = *reinterpret_cast<s_animation_header* (*)[21]>(std::uintptr_t{0x00551060});
    inline uint8_t& _draw_planet_sum = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005510C0});

    inline int16_t& _first =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005510B4});

    inline int16_t& _slider_bar_position = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005510B8});

    inline int16_t (&_list_col_prod_fields)[10] = *reinterpret_cast<int16_t(*)[10]>(std::uintptr_t{0x00563B98});
    inline int16_t (&_list_col)[10] = *reinterpret_cast<int16_t(*)[10]>(std::uintptr_t{0x00563BB0});

    inline s_colsum_col_pop_ref (&_list_col_pop_ref)[30] =
        *reinterpret_cast<s_colsum_col_pop_ref(*)[30]>(std::uintptr_t{0x00563BC8});

    inline int16_t (&_list_col_coord_vars)[32] =
        *reinterpret_cast<int16_t(*)[32]>(std::uintptr_t{0x005645D0});

    inline int16_t (&_list_col_pop_count)[32] =
        *reinterpret_cast<int16_t(*)[32]>(std::uintptr_t{0x00564628});

    inline int16_t (&_list_buy_fields)[10] = *reinterpret_cast<int16_t(*)[10]>(std::uintptr_t{0x005645A0});
    inline int16_t (&_list_fields)[10] = *reinterpret_cast<int16_t(*)[10]>(std::uintptr_t{0x005645B8});
    inline int16_t (&_list_buy_data)[10] = *reinterpret_cast<int16_t(*)[10]>(std::uintptr_t{0x00564610});

    inline int16_t (&_list_col_job_fields)[30] =
        *reinterpret_cast<int16_t(*)[30]>(std::uintptr_t{0x005646F8});
    inline int16_t& _colony_summary_saved_screen = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00564624});

    void __cdecl Clear_List_Col_Array_();

    void __cdecl Init_Fields_();

    void __cdecl Add_Fields_();

    void __cdecl Add_Fields_Pop_For_(int16_t colony_idx);

    void __cdecl Update_Col_List_();

    void __cdecl Reset_List_Col_Array_();

    void __cdecl Load_Col_Sum_Anims_();

    inline uint8_t& _colsum_strings_been_loaded =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005510ec});

    inline int8_t (&_event_good_lookup)[36] =
        *reinterpret_cast<int8_t(*)[36]>(std::uintptr_t{0x005510C8});

    inline char* (&_wee_event_strings)[36] =
        *reinterpret_cast<char* (*)[36]>(std::uintptr_t{0x00564668});

    void __cdecl Load_Colsum_Strings_();

    bool __cdecl Event_Good_(int16_t event_id);

    void __cdecl Sort_Col_List_();

    int16_t __cdecl cmp_(int16_t param_1, int16_t param_2);

    int16_t __cdecl cmp_Alpha_(int16_t colony_idx_1, int16_t colony_idx_2);

    int16_t __cdecl cmp_Pops_(int16_t colony_idx1, int16_t colony_idx2);

    int16_t __cdecl cmp_Food_(int16_t colony_idx1, int16_t colony_idx2);

    int16_t __cdecl cmp_Industry_(int16_t param_1, int16_t param_2);

    int16_t __cdecl cmp_Research_(int16_t colony_idx_1, int16_t colony_idx_2);

    int16_t __cdecl cmp_Prod_(int16_t colony_idx_1, int16_t colony_idx_2);

    int16_t __cdecl cmp_BC_(int16_t param_1, int16_t param_2);

    int16_t __cdecl Switched_cmp_(int16_t a1, int16_t a2);

    void __cdecl Draw_Colony_Summary_Screen_();

    void __cdecl Draw_Colony_Summary_();

    void __cdecl Update_First_(int16_t param_1, int16_t* param_2, int16_t* param_3);

    void __cdecl Decrement_First_(int16_t num_items);

    void __cdecl Increment_First_(int16_t count);

    inline const char* s_0_0055110c = reinterpret_cast<const char*>(std::uintptr_t{0x0055110C}); // " 0"
    inline const char* s_1_00551110 = reinterpret_cast<const char*>(std::uintptr_t{0x00551110}); // " 1"

    inline const char* s__005510f0 = reinterpret_cast<const char*>(std::uintptr_t{0x005510F0});
    inline const char* s_s_005510f4 = reinterpret_cast<const char*>(std::uintptr_t{0x005510F4});
    inline const char* s_f_005510f8 = reinterpret_cast<const char*>(std::uintptr_t{0x005510F8});
    inline const char* s_p_005510fc = reinterpret_cast<const char*>(std::uintptr_t{0x005510FC});

    void __cdecl Draw_Empire_Info_();

    void __cdecl Draw_Galaxy_Map_();

    void __cdecl Draw_Colony_Summary_For_Colony_(int16_t index);

    void __cdecl Draw_Info_Pop_For_(int16_t colony_idx);

    void __cdecl Colsum_Connect_Galaxy_Map_Stars_(int16_t star_idx1, int16_t star_idx2, s_colors* pattern);

    void __cdecl Draw_Bar_Indicator_();

    bool __cdecl Evaluate_Input_(int16_t* input_val, int16_t* scan_val);

    bool __cdecl Evaluate_Col_List_Input_(int16_t* field_ptr, int16_t* param_2);

    void __cdecl Evaluate_Colony_Pop_Input_(int16_t* input_field_1, int16_t* input_field_2, int16_t colony_idx_in_list);

    int16_t __cdecl Get_Selected_Pop_(int16_t colony_idx, int16_t job_idx);

    int16_t __cdecl Get_Scanned_Pop_(int16_t param_1, int16_t param_2);

    void __cdecl Check_Galaxy_Map_Fields_(int16_t* param_1, int16_t* param_2);

    void __cdecl Draw_Scan_Info_();

    void __cdecl Colony_Summary_Screen_();

    int16_t __cdecl Pick_Best_Planet_To_Goto_At_Star_(int16_t star_idx);

    int16_t __cdecl Prod_To_Sort_Type_(int16_t production_id);

    void __cdecl Draw_Colony_Scan_Info_();
}