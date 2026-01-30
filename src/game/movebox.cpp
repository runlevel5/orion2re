#include "deps.h"

namespace MOVEBOX {
    /**
     * @brief Checks if a specific moveable box is currently selected based on its offset flag.
     * @param box_idx The index of the moveable box to check.
     * @return True if the box's state flag is less than its threshold offset flag.
     */
    bool __cdecl Moveable_Box_Selected_(int16_t box_idx) {
        return MOX::_box_off_flag[box_idx] < MOX::_moveable_box[box_idx].off_flag;
    }
}