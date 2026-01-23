#pragma once
#include <cstdint>

namespace exitm {
    inline void __cdecl Exit_With_Message(void* msg_ptr) {
        using Fn = void(__cdecl*)(void*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0052D310})(msg_ptr);
    }

    // Suggested name: g_MapCaptureMessage
    inline void*& DAT_005575b4 = *reinterpret_cast<void**>(std::uintptr_t{0x005575B4});
}