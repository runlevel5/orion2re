#pragma once
#include <cstdint>

namespace CHAT {
    inline void*& _chat_info = *reinterpret_cast<void**>(std::uintptr_t{0x005A7B70});

    inline void __cdecl Send_Chat_Msg_(char* msg) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042CFA0});
        fn(msg);
    }
}