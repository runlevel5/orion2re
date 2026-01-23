#pragma once

#include <cstdint>

namespace key {
    // A ring buffer storing the last 10 keys pressed.
    inline int32_t* _last_key_pressed =
        reinterpret_cast<int32_t*>(std::uintptr_t{0x0057E0F8});

    // Boolean flag indicating if there is at least one key remaining in the buffer.
    inline bool& _key_pressed =
        *reinterpret_cast<bool*>(std::uintptr_t{0x0057E120});

    // Write index for the keyboard ring buffer.
    inline uint8_t& _key_write =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0057E124});

    // Read index for the keyboard ring buffer.
    inline uint8_t& _key_read =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0057E128});

    /**
     * Checks if a key has been pressed since the last check.
     * If pressed, resets the flag and returns 1.
     * 
     * @return 1 if a key was pressed, 0 otherwise.
     */
    uint8_t Keyboard_Status_();

    /**
     * @address 00531f20
     * Reads the next key code from the keyboard buffer and updates the buffer pointers.
     * 
     * @return The key code that was read from the buffer.
     */
    int32_t Read_Key_();
}