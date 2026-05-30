#include "pch.h"

namespace key {
    int32_t _last_key_pressed[10];

    uint8_t _key_pressed;

    uint8_t _key_write;

    int8_t _key_read;

    uint8_t Keyboard_Status_() {
        if (_key_pressed == 1) {
            _key_pressed = 0;
            return 1;
        }
        return 0;
    }

    int32_t Read_Key_() {
        int8_t current_index = _key_read;

        // Retrieve the key code at the current read position.
        // Note: There is no explicit bounds check on 'current_index' before access.
        int32_t key_code = _last_key_pressed[current_index];

        // Increment the read pointer with wrap-around (ring buffer size is 10).
        _key_read = (current_index + 1) % 10;

        // Update the key_pressed status based on whether more keys are available.
        // If the read pointer has caught up to the write pointer, the buffer is empty.
        if (_key_read == _key_write) {
            _key_pressed = 0;
        } else {
            _key_pressed = 1;
        }

        return key_code;
    }

    int16_t Scan_Key_() {
        if (Keyboard_Status_() == 1) {
            int16_t key = (int16_t)Read_Key_();
            if (key == 0x0D) {
                return 0x0D;
            }
            return key;
        }
        return 0;
    }
}