#include "deps.h"

namespace FLEETPOP {

int16_t __cdecl Fleet_Box_Grid_Height_(int16_t grid_type) {
    if (grid_type == 1) {
        return 0x38;
    }

    if (grid_type == 2) {
        return 0x70;
    }

    if (grid_type == 3) {
        MOX::_fleet_box_grid_sm_seg4 = Buffer_Reload_(s_BUFFER0_LBX, 0x36, MOX::_global_cache_seg);
        return animate::Get_Height_((s_animation_header*)MOX::_fleet_box_grid_sm_seg4);
    }

    return 0;
}

void __cdecl Set_Fleet_Movement_Box_Dimensions_(int16_t count) {
    int32_t rows = (static_cast<int32_t>(count) - 1) / 3 + 1;
    if (rows > 3) {
        rows = 3;
    }

    MOX::_fleet_box_map_height = Fleet_Box_Grid_Height_(static_cast<int16_t>(rows));
    MOX::_moveable_box[2].height = Fleet_Box_Height_(count);
}

int16_t __cdecl Fleet_Box_Width_() {
    return animate::Get_Width_(MOX::_fleet_box.name_sm_seg);
}

/**
 * @address 00405990
 */
void __cdecl Reload_Fleet_Box_Data_(void) {
    if (HACCESS::Get_Stack_Start_From_Saved_Ship_Array_() == 0) {
        MOVEBOX::Vanish_Fleet_Movement_Box_();
        return;
    }

    int16_t node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(static_cast<int16_t>(MOX::_fleet_box_ship_stack));
    int16_t num_icons = FLEETPOP::N_Fleet_Icons_(node_ptr);
    FLEETPOP::Fleet_Box_Data_(num_icons);
}

}