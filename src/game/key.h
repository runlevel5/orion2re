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
}