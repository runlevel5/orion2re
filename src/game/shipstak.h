#pragma once
#include <cstdint>

namespace SHIPSTAK {
    int16_t __cdecl Ship_Has_Special_(int16_t ship_idx, int16_t special_id);

    bool __cdecl Ship_Has_Stealth_Device_(int16_t ship_idx);

    short __cdecl Ship_Stack_Star_Id_(short node_idx);

    void __cdecl Find_Ship_Stacks_();

    void __cdecl Remove_Non_Detected_Ships_();

    inline void __cdecl Delete_Ship_Node_(int16_t stack_idx, int16_t prev_node_idx, int16_t node_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00460d00})(stack_idx, prev_node_idx, node_idx);
    }

    inline void __cdecl Delete_Ship_Stack_(int16_t stack_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00460cd0})(stack_idx);
    }

    inline void __cdecl Build_Ship_Type_Array_(int16_t ship_stack_idx, int16_t *ship_entries, int16_t *ship_count) {
        using Fn = void(__cdecl *)(int16_t, int16_t *, int16_t *);
        reinterpret_cast<Fn>(std::uintptr_t{0x00460DC0})(ship_stack_idx, ship_entries, ship_count);
    }

    inline int __cdecl qsort_ships_by_type_(const void *lhs, const void *rhs) {
        using Fn = int(__cdecl *)(const void *, const void *);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00460E90})(lhs, rhs);
    }

    void __cdecl Sort_Ships_In_Stack_(int16_t stack_start_idx);
}