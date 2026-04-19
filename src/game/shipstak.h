#pragma once
#include <cstdint>

namespace SHIPSTAK {
    int16_t __cdecl Ship_Has_Special_(int16_t ship_idx, int16_t special_id);

    bool __cdecl Ship_Has_Stealth_Device_(int16_t ship_idx);

    short __cdecl Ship_Stack_Star_Id_(short node_idx);

    void __cdecl Find_Ship_Stacks_();

    void __cdecl Remove_Non_Detected_Ships_();

    void __cdecl Delete_Ship_Node_(int16_t stack_idx, int16_t prev_node_idx, int16_t node_idx);

    void __cdecl Delete_Ship_Stack_(int16_t stack_idx);

    void __cdecl Build_Ship_Type_Array_(int16_t node_idx, int16_t* stack_array, int16_t* count_ptr);

    int __cdecl qsort_ships_by_type_(const void* param_1, const void* param_2);

    void __cdecl Sort_Ships_In_Stack_(int16_t stack_start_idx);
}