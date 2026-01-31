#include "deps.h"

namespace MOVEBOX {
    bool __cdecl Moveable_Box_Selected_(int16_t box_idx) {
        return MOX::_box_off_flag[box_idx] < MOX::_moveable_box[box_idx].off_flag;
    }

    void __cdecl Move_Box_To_Top_(int16_t box_id) {
        int16_t local_order[5] = {0, 0, 0, 0, 0};
        int16_t found_idx = -1;

        // Find the index of the box in the current order
        for (int16_t i = 0; i < 5; ++i) {
            if (MOX::_moveable_box_order[i] == box_id) {
                found_idx = i;
                break;
            }
        }

        // Extract the box from the global array and mark the slot as empty (0xFFFF)
        // If the box was not found, this logic extracts the padding word before the array.
        int16_t* const moveable_box_order = reinterpret_cast<int16_t*>(&MOX::_moveable_box_order);
        int16_t target_box_id = moveable_box_order[found_idx];
        moveable_box_order[found_idx] = -1; // 0xFFFF

        // The specified box is now at the top (index 0) of our local sorted list
        local_order[0] = target_box_id;

        // Fill slots 1 through 4 with the remaining boxes in their existing relative order
        for (int i = 1; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (MOX::_moveable_box_order[j] != -1) {
                    local_order[i] = MOX::_moveable_box_order[j];
                    MOX::_moveable_box_order[j] = -1;
                    break;
                }
            }
        }

        // Copy the reorganized local list back to the global order array
        for (int i = 0; i < 5; ++i) {
            MOX::_moveable_box_order[i] = local_order[i];
        }
    }
}