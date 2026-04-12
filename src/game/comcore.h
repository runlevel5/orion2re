#pragma once

#include "pch.h"

namespace comcore {
    inline void __cdecl ComDoComms_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00539940});
        fn();
    }

    inline void __cdecl ComSendUserMessage_(uint8_t* data, uint32_t size, int32_t target_id, int32_t arg4, int32_t arg5, int16_t msg_type) {
        using Fn = void(__cdecl*)(uint8_t*, uint32_t, int32_t, int32_t, int32_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00539950});
        fn(data, size, target_id, arg4, arg5, msg_type);
    }

    inline void __cdecl ComInit_(int unk, char* session_name, void* recv_fn) {
        using Fn = void(__cdecl*)(int, char*, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00539800});
        fn(unk, session_name, recv_fn);
    }
}