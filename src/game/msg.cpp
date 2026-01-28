#include "msg.h"

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
}