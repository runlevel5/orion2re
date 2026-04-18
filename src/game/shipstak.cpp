#include "pch.h"
#include <cstring>

namespace SHIPSTAK {
    short __cdecl Ship_Stack_Star_Id_(short node_idx) {
        return MOX::_ship[MOX::_ship_node[node_idx].ship_idx].location;
    }

    bool __cdecl Ship_Has_Stealth_Device_(int16_t ship_idx) {
        if (Ship_Has_Special_(ship_idx, 0x1F) != 0) {
            return true;
        }

        if (Ship_Has_Special_(ship_idx, 0x06) != 0) {
            return true;
        }

        if (Ship_Has_Special_(ship_idx, 0x17) != 0) {
            return true;
        }

        return false;
    }

    void __cdecl Find_Ship_Stacks_() {
        uint8_t* ship_skip_flags = static_cast<uint8_t*>(allocate::Allocate_First_Block_(MOX::_global_data_seg, 500));

        if (MOX::_NUM_SHIPS > 0) {
            memset(ship_skip_flags, 0, MOX::_NUM_SHIPS);
        }

        MOX::_ship_stack_count = 0;
        MOX::_next_free_node = 0;

        for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
            if (MOX::_ship[i].status >= 3) {
                ship_skip_flags[i] = 1;
            }
        }

        for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
            if (ship_skip_flags[i] == 1) {
                continue;
            }

            bool found_stack = false;
            for (int16_t j = 0; j < (int16_t)MOX::_ship_stack_count; ++j) {
                int16_t first_node_idx = MOX::_ship_stack_start[j];
                int16_t first_ship_idx = MOX::_ship_node[first_node_idx].ship_idx;

                if (MOX::_ship[first_ship_idx].location == MOX::_ship[i].location &&
                    MOX::_ship[first_ship_idx].x == MOX::_ship[i].x &&
                    MOX::_ship[first_ship_idx].y == MOX::_ship[i].y &&
                    MOX::_ship[first_ship_idx].owner == MOX::_ship[i].owner) {

                    int16_t node_idx = first_node_idx;
                    int16_t last_node = node_idx;

                    while (node_idx != -1) {
                        last_node = node_idx;
                        node_idx = MOX::_ship_node[node_idx].next_node;
                    }

                    int16_t next_free = MOX::_next_free_node;
                    MOX::_ship_node[last_node].next_node = next_free;
                    MOX::_ship_node[next_free].ship_idx = i;
                    MOX::_ship_node[next_free].next_node = -1;
                    MOX::_ship_node[next_free].selected = 0;
                    MOX::_next_free_node++;

                    found_stack = true;
                    break;
                }
            }

            if (!found_stack) {
                int16_t next_free = MOX::_next_free_node;
                MOX::_ship_stack_start[MOX::_ship_stack_count] = next_free;
                MOX::_ship_node[next_free].ship_idx = i;
                MOX::_ship_node[next_free].next_node = -1;
                MOX::_ship_node[next_free].selected = 0;
                MOX::_next_free_node++;
                MOX::_ship_stack_count++;
            }
        }

        if (MOX::_player[MOX::_PLAYER_NUM].traits[27] == 0) {
            Remove_Non_Detected_Ships_();
        }

        for (int16_t j = 0; j < (int16_t)MOX::_ship_stack_count; ++j) {
            Sort_Ships_In_Stack_(MOX::_ship_stack_start[j]);
        }
    }
}