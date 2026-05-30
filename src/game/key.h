#pragma once

#include <cstdint>

namespace key {
    // A ring buffer storing the last 10 keys pressed.
    extern int32_t _last_key_pressed[10];

    // Byte flag indicating if there is at least one key remaining in the buffer.
    extern uint8_t _key_pressed;

    // Write index for the keyboard ring buffer.
    extern uint8_t _key_write;

    // Signed read index for the keyboard ring buffer.
    extern int8_t _key_read;

    uint8_t Keyboard_Status_();

    int32_t Read_Key_();

    int16_t Scan_Key_();
}