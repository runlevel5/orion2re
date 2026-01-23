#pragma once

namespace struct_ {
    inline void __cdecl Clear_Structure_(void* ptr, uint32_t size) {
        using Fn = void(__cdecl*)(void*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052D0B7 });
        fn(ptr, size);
    }

    inline void __cdecl Copy_Structure_(void* dest, const void* src, uint32_t size) {
        using Fn = void(__cdecl*)(void*, const void*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052D0FA });
        fn(dest, src, size);
    }
}