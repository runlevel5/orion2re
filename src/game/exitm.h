#pragma once

namespace exitm {
    // Suggested name: g_is_exiting_with_error
    inline uint8_t& DAT_00578e68 = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00578E68 });

    void __cdecl Exit_With_Message(const char* str);
    void __cdecl Exit_With_message2_(const char* msg);
}