#include "deps.h"
#include "haccess.h"
#include "mox.h"
#include "exitm.h"
#include "funcs.h"

namespace HACCESS {
    int16_t __cdecl Ship_Owner_(int16_t ship_idx) {
        int16_t error_code = 0;

        if (ship_idx < 0) {
            error_code = 1;
        } else if (ship_idx >= 500) {
            error_code = 2;
        } else {
            int8_t owner = MOX::_ship[ship_idx].owner;
            if (owner < 0) {
                error_code = 3;
            } else if (owner > 15) {
                error_code = 4;
            }
        }

        if (error_code == 0) {
            return (int16_t)MOX::_ship[ship_idx].owner;
        }

        char message[200];
        // Note: The original code accesses the array even if the index is invalid in error paths.
        int32_t current_owner = (int32_t)MOX::_ship[ship_idx].owner;

        _sprintf(message, s_Memory_Corruption_val_00558028, (int)error_code, (int)ship_idx, current_owner, MOX::_stardate);

        exitm::Exit_With_Message(message);

        return 0;
    }

    int16_t __cdecl Ship_Stack_From_Saved_Array_() {
        int16_t best_owner_match_idx = -1;
        int16_t stack_index = -1;
        bool found = false;

        if (MOX::_ship_stack_count < 1) {
            return -1;
        }

        for (int16_t i = 0; i < 500; ++i) {
            // Status >= 3 typically indicates the ship is inactive/destroyed
            if (MOX::_ship[i].status < 3 && struct_::Test_Bit_Field_(&MOX::_fleet_box_ship_ids, i)) {
                int16_t target_owner = Ship_Owner_(i);

                for (int16_t j = 0; j < (int16_t)MOX::_ship_stack_count; ++j) {
                    if (found) break;

                    int16_t node_idx = MOX::_ship_stack_start[j];
                    int16_t stack_ship_idx = MOX::_ship_node[node_idx].ship_idx;

                    if (best_owner_match_idx == -1) {
                        if (Ship_Owner_(stack_ship_idx) == target_owner) {
                            best_owner_match_idx = j;
                        }
                    }

                    if (stack_ship_idx == i) {
                        found = true;
                        stack_index = j;
                    }
                }
            }
            if (found) break;
        }

        if (!found && best_owner_match_idx != -1) {
            stack_index = best_owner_match_idx;
        }

        return stack_index;
    }

    int16_t __cdecl Node_Ptr_From_Ship_Stack_(int16_t index) {
        if (MOX::_ship_stack_count == 0) {
            return 0;
        }

        if (MOX::_ship_stack_count < index) {
            MOVEBOX::Vanish_Fleet_Movement_Box_();
            return 0;
        }

        if (MOX::_ship_stack_count == index) {
            return Ship_Stack_From_Saved_Array_();
        }

        return MOX::_ship_stack_start[index];
    }
}