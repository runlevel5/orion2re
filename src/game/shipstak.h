#pragma once
#include <cstdint>

namespace SHIPSTAK {
    enum class e_stealth_size_penalty : uint8_t {
        none,
        race,
        race_tech,
    };

    struct s_scan_bonus {
        uint16_t battle_scanner;
        uint16_t scout_lab;
        uint16_t star_base;
        uint8_t battlestation;
        uint8_t star_fortress;
        uint8_t no_scanner;
        uint8_t tachyon_scanner;
        uint8_t neutron_scanner;
        uint8_t sensors;
        uint16_t planet;
        uint16_t fleet;
    };

    struct s_stealth_bonus {
        int16_t stealthy_ships;
        int16_t stealth_field;
        int16_t cloaking_device;
        int16_t phasing_cloak;
    };

    inline s_scan_bonus scan_bonus = {
        2, 0, 2, 4, 6, 2, 4, 6, 9, 32000, 32000,
    };
    inline s_stealth_bonus stealth_bonus = {
        500, 500, 500, 500,
    };
    inline e_stealth_size_penalty stealth_size_penalty =
        e_stealth_size_penalty::race_tech;

    inline bool Traveling_Size_Penalty_Applies_(
            bool stealthy_race, bool has_stealth_device) {
        if (stealthy_race &&
            stealth_size_penalty != e_stealth_size_penalty::none) {
            return false;
        }
        if (has_stealth_device &&
            stealth_size_penalty == e_stealth_size_penalty::race_tech) {
            return false;
        }
        return true;
    }

    // Returns nonzero when the ship has the specified undamaged special device.
    // ship_idx: Ship table index to inspect.
    // special_idx: Special-device bit index to test.
    int16_t __cdecl Ship_Has_Special_(int16_t ship_idx, int16_t special_idx);

    // Returns true when the ship has any working stealth device.
    // ship_idx: Ship table index to inspect.
    bool __cdecl Ship_Has_Stealth_Device_(int16_t ship_idx);

    // Returns the star/location id for the ship referenced by a stack node.
    // node_idx: Ship-stack node index to resolve.
    int16_t __cdecl Ship_Stack_Star_Id_(int16_t node_idx);

    // Builds ship stacks from active ships and sorts each resulting stack.
    void __cdecl Find_Ship_Stacks_();

    // Removes enemy ships from stacks when the current player cannot detect them.
    void __cdecl Remove_Non_Detected_Ships_();

    // Unlinks one node from a stack.
    // stack_idx: Stack containing the node.
    // prev_node_idx: Previous node in the stack, or -1 for the head node.
    // node_idx: Node to remove.
    void __cdecl Delete_Ship_Node_(int16_t stack_idx, int16_t prev_node_idx, int16_t node_idx);

    // Removes an empty stack entry from the stack list.
    // stack_idx: Stack list index to delete.
    void __cdecl Delete_Ship_Stack_(int16_t stack_idx);

    // Builds interleaved {ship_idx, sort_key} entries for a ship stack.
    // node_idx: First node in the stack.
    // stack_array: Output array of interleaved ship indexes and sort keys.
    // count_ptr: Receives the number of entries written.
    void __cdecl Build_Ship_Type_Array_(int16_t node_idx, int16_t* stack_array, int16_t* count_ptr);

    // Orders interleaved {ship_idx, sort_key} entries by descending sort key.
    // left_entry: First qsort record.
    // right_entry: Second qsort record.
    int __cdecl qsort_ships_by_type_(const void* left_entry, const void* right_entry);

    // Sorts the ships in one stack by the generated type key.
    // stack_start_idx: First node in the stack to sort.
    void __cdecl Sort_Ships_In_Stack_(int16_t stack_start_idx);
}