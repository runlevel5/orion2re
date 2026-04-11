#pragma once

namespace exitm {
    // Suggested name: g_is_exiting_with_error
    inline uint8_t& DAT_00578e68 = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00578E68 });

    void __cdecl Exit_With_Message(const char* str);

    inline void __cdecl Exit_With_message2_(const char* msg) {
        using Fn = void(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00515690});
        fn(msg);
    }
}