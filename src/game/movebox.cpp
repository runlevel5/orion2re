#include "deps.h"
#include "haccess.h"
#include "evanhelp.h"

namespace MOVEBOX {
    bool __cdecl Moveable_Box_Selected_(int16_t box_idx) {
        return MOX::_box_off_flag[box_idx] < MOX::_moveable_box[box_idx].off_flag;
    }

    void __cdecl Vanish_Fleet_Movement_Box_() {
        MOX::_moveable_box[2].off_flag = MOX::_box_off_flag[2];

        if (MOX::_fleet_box.count > 0) {
            for (int16_t i = 0; i < MOX::_fleet_box.count; ++i) {
                MOX::_fleet_movement_icon[i].field = -1000;
            }
        }

        MOX::_fleet_movement_all_button = -1000;
        MOX::_fleet_box_colonize_button = -1000;
        MOX::_moveable_box[2].field = -1000;
        MOX::_moveable_box[2].field8_0x10 = -1000;

        MOX::_mouse_XY.ship_icon_idx = -1;
        MOX::_eta_star_id = -1;
        MOX::_fleet_ship_scanned = -1;

        HACCESS::Clear_Fleet_Box_Selected_Ship_Status_();
        EVANHELP::Set_Main_Screen_Help_List_();

        MOX::_reloaded_ship_palette = 0;
    }

    void __cdecl Vanish_System_Display_Box_() {
        MOX::_moveable_box[0].off_flag = MOX::_box_off_flag[0];

        for (int32_t i = 0; i < 5; ++i) {
            MOX::_planet_buttons[i] = -1000;
        }

        for (int32_t i = 0; i < 8; ++i) {
            MOX::_system_display_ships[i].button = -1000;
        }

        MOX::_moveable_box[0].field = -1000;
        MOX::_moveable_box[0].field8_0x10 = -1000;
    }

    bool __cdecl Mouse_Within_Fleet_Box_() {
        bool within = false;
        int16_t px = mouse::Pointer_X_();
        int16_t py = mouse::Pointer_Y_();

        const s_movable_box& box = MOX::_moveable_box[2];

        int32_t bottom_val = (int32_t)box.height + (int32_t)box.y;
        if (bottom_val >= 0x1A5) {
            bottom_val = 0x1A5;
        }

        int16_t bottom = (int16_t)bottom_val;
        int16_t right = (int16_t)(box.width + box.x);

        if (px >= box.x && px <= right && py >= box.y && py <= bottom) {
            within = true;
        }

        return within;
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