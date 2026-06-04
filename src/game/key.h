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

    // Returns non-zero when the keyboard ring buffer currently contains unread input.
    uint8_t Keyboard_Status_();

    // Returns the next key code from the ring buffer and advances the read position.
    int32_t Read_Key_();

    // Returns the next buffered key code as a 16-bit value, or 0 when no key is available.
    int16_t Scan_Key_();
}