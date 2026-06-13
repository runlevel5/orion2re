#include "pch.h"

namespace key {
    int32_t _last_key_pressed[10];

    uint8_t _key_pressed;

    uint8_t _key_write;

    int8_t _key_read;

    constexpr int32_t kLastKeyPressedCount = sizeof(_last_key_pressed) / sizeof(_last_key_pressed[0]);

    uint8_t Keyboard_Status_() {
        platform::Pump_Game_Input_Queue_();
        if (_key_pressed == 1) {
            _key_pressed = 0;
            return 1;
        }
        return 0;
    }

    int32_t Read_Key_() {
        platform::Pump_Game_Input_Queue_();

        if (_key_pressed == 0 && _key_read == _key_write) {
            return 0;
        }

        int8_t current_index = _key_read;
        int32_t key_code = _last_key_pressed[current_index];
        _key_read = (current_index + 1) % kLastKeyPressedCount;

        if (_key_read == _key_write) {
            _key_pressed = 0;
        } else {
            _key_pressed = 1;
        }

        return key_code;
    }

    int32_t Wait_For_Key_() {
        int32_t key_code;

        do {
            key_code = Read_Key_();
            if (key_code == 0) {
                SDL_Delay(1);
            }
        } while (key_code == 0);

        return key_code;
    }

    int16_t Scan_Key_() {
        platform::Pump_Game_Input_Queue_();
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