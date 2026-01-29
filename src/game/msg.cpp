#include "deps.h"

namespace MSG_ {
    void __cdecl Add_Message(uint32_t header, uint32_t p2, uint32_t p3, uint32_t p4, uint16_t p5, int16_t buffer_idx) {
        int idx = (int)buffer_idx;
        uint8_t max_count = _max_msgs[idx];
        
        if (max_count == 0) {
            return;
        }

        uint8_t current_count = _n_msgs[idx];
        uint8_t first_idx = _first_msg[idx];

        if (current_count < max_count) {
            s_MSG* target = &_msgs[first_idx + current_count];
            target->field0_0x0 = header;
            target->field1_0x4 = p2;
            target->field2_0x8 = p3;
            target->field3_0xc = p4;
            target->field4_0x10 = p5;
            _n_msgs[idx] = current_count + 1;
        } else {
            s_MSG* min_msg = &_msgs[first_idx];
            s_MSG* current = min_msg + 1;
            
            while (current < min_msg + current_count + 1) {
                uint8_t current_prio = reinterpret_cast<uint8_t*>(&current->field0_0x0)[1];
                uint8_t min_prio = reinterpret_cast<uint8_t*>(&min_msg->field0_0x0)[1];
                
                if (current_prio < min_prio) {
                    min_msg = current;
                }
                current++;
            }

            uint8_t new_prio = reinterpret_cast<uint8_t*>(&header)[1];
            uint8_t final_min_prio = reinterpret_cast<uint8_t*>(&min_msg->field0_0x0)[1];
            
            if (final_min_prio < new_prio) {
                min_msg->field0_0x0 = header;
                min_msg->field1_0x4 = p2;
                min_msg->field2_0x8 = p3;
                min_msg->field3_0xc = p4;
                min_msg->field4_0x10 = p5;
            }
        }
    }

    /**
     * Calculates and allocates message pool slices (out of 100 slots) 
     * for all active (non-eliminated) players.
     */
    void __cdecl Allocate_Msg_Slots_() {
        int active_players = 0;

        // First pass: Count active players
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            // s_player structure size is 0xea9 (3753 bytes)
            if (MOX::_player[i].eliminated == 0) {
                active_players++;
            }
        }

        // Reset message tracking arrays for all potential 8 players
        for (int i = 0; i < 8; ++i) {
            _first_msg[i] = 0;
            _n_msgs[i] = 0;
            _max_msgs[i] = 0;
        }

        // Second pass: Distribute 100 message slots among active players
        if (MOX::_NUM_PLAYERS > 0 && active_players > 0) {
            int cumulative_pool = 0;
            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                if (MOX::_player[i].eliminated == 0) {
                    uint8_t start_slot = (uint8_t)(cumulative_pool / active_players);
                    cumulative_pool += 100;
                    uint8_t end_slot = (uint8_t)(cumulative_pool / active_players);

                    _first_msg[i] = start_slot;
                    _max_msgs[i] = end_slot - start_slot;
                }
            }
        }
    }
}