#include "deps.h"

namespace FLEETPOP {

int16_t __cdecl Fleet_Box_Height_(int16_t fleet_count) {
    int16_t close_btn_height = animate::Get_Height_(MOX::_fleet_box_close_lg_button_seg);
    int16_t name_height;
    int16_t bottom_height;

    if (MOX::_fleet_box.field12_0x1a == 0) {
        MOX::_fleet_box_name_sm_seg_0059736C = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, 0x33, MOX::_global_cache_seg);
        MOX::_fleet_box.name_sm_seg = MOX::_fleet_box_name_sm_seg_0059736C;
        name_height = animate::Get_Height_(MOX::_fleet_box.name_sm_seg);

        MOX::_fleet_box.name_sm_seg2 = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, 0x37, MOX::_global_cache_seg);
        MOX::_fleet_box_bottom_sm_seg2 = MOX::_fleet_box.name_sm_seg2;
    } else {
        MOX::_fleet_box_name_sm_seg_0059B430 = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, 0x34, MOX::_global_cache_seg);
        MOX::_fleet_box.name_sm_seg = MOX::_fleet_box_name_sm_seg_0059B430;
        name_height = animate::Get_Height_(MOX::_fleet_box.name_sm_seg);

        MOX::_fleet_box.name_sm_seg2 = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, 0x38, MOX::_global_cache_seg);
        MOX::_fleet_box_bottom_sm_seg_0059BA00 = MOX::_fleet_box.name_sm_seg2;
    }

    bottom_height = animate::Get_Height_(MOX::_fleet_box.name_sm_seg2);

    int16_t grid_height;
    if (fleet_count > 9) {
        MOX::_fleet_box_grid_lg_seg = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, 0x35, MOX::_global_cache_seg);
        grid_height = animate::Get_Height_(MOX::_fleet_box_grid_lg_seg);
    } else {
        int32_t rows = (static_cast<int32_t>(fleet_count) - 1) / 3 + 1;
        if (rows > 3) {
            rows = 3;
        }
        grid_height = FLEETPOP::Fleet_Box_Grid_Height_(static_cast<int16_t>(rows));
    }

    return grid_height + close_btn_height + bottom_height + name_height;
}

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

void __cdecl Set_Fleet_Movement_Icon_Selections_(int16_t state, uint8_t auto_select_flag) {
    for (int16_t i = 0; i < MOX::_fleet_box.count; ++i) {
        int16_t node_idx = MOX::_fleet_movement_icon[i].ship_node_idx;
        s_ship_node& node = MOX::_ship_node[node_idx];
        int16_t ship_idx = node.ship_idx;

        if (HACCESS::Ship_Owner_(ship_idx) == MOX::_PLAYER_NUM) {
            if (state == 0) {
                node.selected = 0;
            } else if (state == 1) {
                if (auto_select_flag != 0) {
                    if (FLEETPOP::Ship_Should_Be_Auto_Selected_(ship_idx)) {
                        node.selected = 1;
                    }
                } else {
                    if (MAINSCR::Ship_Can_Be_Selected_(ship_idx)) {
                        node.selected = 1;
                    }
                }
            }
        }
    }

    MOX::_fleet_movement_icons_on = static_cast<uint8_t>(state);
}

/**
 * @address 00406700
 */
bool __cdecl Some_Icon_Selected_() {
    int16_t selected_ship_ids[500];

    if (MOVEBOX::Moveable_Box_Selected_(2)) {
        int16_t count = HACCESS::Get_Fleet_Box_Selected_Ship_Ids_(selected_ship_ids, 500, 255);
        return count > 0;
    }

    return false;
}

/**
 * @address 004059e0
 */
void __cdecl Reload_Ship_Selection_Status_() {
    int16_t node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
    int16_t stack_owner = HACCESS::Ship_Stack_Owner_(node_ptr);

    if (static_cast<int16_t>(MOX::_PLAYER_NUM) == stack_owner) {
        for (int16_t i = 0; i < 500; ++i) {
            int16_t ship_idx = MOX::_ship_node[i].ship_idx;

            bool is_selectable = false;
            if (struct_::Test_Bit_Field_(&MOX::_black_hole_anim_bitfield, ship_idx)) {
                if (MAINSCR::Ship_Can_Be_Selected_(ship_idx)) {
                    is_selectable = true;
                }
            }

            MOX::_ship_node[i].selected = is_selectable ? 1 : 0;
        }
    }
}

/**
 * @address 00404cc0
 */
int16_t __cdecl First_Unselected_Icon_() {
    int16_t first_unselected_index = 0;
    bool found = false;

    if (MOX::_fleet_box.count > 0) {
        for (int16_t i = 0; i < MOX::_fleet_box.count; ++i) {
            if (found) {
                break;
            }

            if (!HACCESS::Fleet_Movement_Icon_Selected_(i)) {
                first_unselected_index = i;
                found = true;
            }
        }
    }

    return first_unselected_index;
}

/**
 * @address 00405a60
 */
void __cdecl Save_Ship_Selection_Status_() {
    int16_t node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
    int16_t owner = HACCESS::Ship_Stack_Owner_(node_ptr);

    if (MOX::_PLAYER_NUM == owner) {
        for (int16_t i = 0; i < MOX::_fleet_box.count; ++i) {
            int16_t ship_node_idx = MOX::_fleet_movement_icon[i].ship_node_idx;

            if (ship_node_idx != -1) {
                s_ship_node& node = MOX::_ship_node[ship_node_idx];

                if (node.selected == 0) {
                    struct_::Clear_Bit_Field_(&MOX::_black_hole_anim_bitfield, node.ship_idx);
                } else {
                    struct_::Set_Bit_Field_(&MOX::_black_hole_anim_bitfield, node.ship_idx);
                }
            }
        }
    }
}

/**
 * @address 00405070
 */
int16_t __cdecl Fleet_Buttons_Chosen_() {
    int16_t selected_count = 0;
    int16_t i = 0;

    if (MOX::_fleet_box.count > 0) {
        do {
            int16_t node_idx = MOX::_fleet_movement_icon[i].ship_node_idx;

            if (MOX::_ship_node[node_idx].selected != 0) {
                selected_count++;
            }

            i++;
        } while (i < MOX::_fleet_box.count);
    }

    return selected_count;
}

/**
 * @address 00406750
 */
bool __cdecl Can_Select_Fleet_Box_Ship_(int16_t index) {
    int16_t ship_node_idx = MOX::_fleet_movement_icon[index].ship_node_idx;
    int16_t owner_id = HACCESS::Ship_Stack_Owner_(ship_node_idx);

    return owner_id == MOX::_PLAYER_NUM;
}

/**
 * @address 00406610
 */
int16_t __cdecl Icon_Selection_Status_(int16_t icon_field_id) {
    int16_t first_icon_idx = static_cast<int16_t>(MOX::_first_fleet_movement_icon);
    int16_t limit_idx = MOX::_fleet_icon_buttons_added + first_icon_idx;

    if (limit_idx > MOX::_fleet_box.count) {
        limit_idx = MOX::_fleet_box.count;
    }

    for (int16_t i = first_icon_idx; i < limit_idx; ++i) {
        if (MOX::_fleet_movement_icon[i].field == icon_field_id) {
            int16_t ship_node_idx = MOX::_fleet_movement_icon[i].ship_node_idx;
            return static_cast<int16_t>(MOX::_ship_node[ship_node_idx].selected);
        }
    }

    return 0;
}

/**
 * @address 00403ec0
 */
void __cdecl Clear_Ship_Node_Selection_Flags_() {
    MOX::_fleet_movement_icons_on = 0;

    for (int i = 0; i < 500; ++i) {
        MOX::_ship_node[i].selected = 0;
    }
}

/**
 * @address 004059c0
 */
void __cdecl Reload_Fleet_Box_Palette_(int16_t player_idx, uint32_t is_not_player) {
    KEN::Load_Player_Ship_Palette_(player_idx);
    MOX::_mainscr_palette = 2;
}

/**
 * @address 00406590
 */
bool __cdecl Ship_Should_Be_Auto_Selected_(int16_t ship_idx) {
    if (MAINSCR::Ship_Can_Be_Selected_(ship_idx) && MOX::_just_moved_stack == 0) {
        return true;
    }
    return false;
}

/**
 * @address 004051e0
 */
int16_t __cdecl N_Fleet_Icons_(int16_t node_idx) {
    int16_t count = 1;

    // Traverse the ship node chain starting from node_idx
    while (MOX::_ship_node[node_idx].next_node != -1 && count < 500) {
        count++;
        HACCESS::Ship_Owner_(MOX::_ship_node[node_idx].ship_idx);
        node_idx = MOX::_ship_node[node_idx].next_node;
    }

    return count;
}

}