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

    uint8_t Keyboard_Status_();

    int32_t Read_Key_();

    inline void __cdecl Init_Keyboard_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052d5d0});
        fn();
    }
}