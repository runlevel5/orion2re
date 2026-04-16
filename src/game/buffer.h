#pragma once

namespace buffer {
    inline void __cdecl Flush_Buffer_Space_(void* cache_seg) {
        using Fn = void(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052C1C0});
        fn(cache_seg);
    }
}