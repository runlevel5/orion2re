#pragma once

#include <cstdint>

namespace key {
    // Returns non-zero when the keyboard ring buffer currently contains unread input.
    uint8_t Keyboard_Status_();

    // Returns the next key code from the ring buffer and advances the read position.
    int32_t Read_Key_();

    // Blocks until a key code is available, yielding briefly between polls.
    int32_t Wait_For_Key_();

    // Returns the next buffered key code as a 16-bit value, or 0 when no key is available.
    int16_t Scan_Key_();

    // Queues one translated key code into the legacy keyboard ring buffer.
    void Push_Key_Code_(uint32_t key_code, uint32_t scan_code);

    // Restores the legacy keyboard state captured at the start of an oracle turn.
    void Restore_Input_State_(uint8_t key_pressed, uint8_t key_write, int8_t key_read,
        uint32_t last_scan_code, const int32_t* key_buffer);
}