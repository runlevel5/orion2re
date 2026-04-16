#pragma once

namespace buffer {
    inline void __cdecl Free_Buffer_Space_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052C190});
        fn();
    }

    inline void __cdecl Flush_Buffer_Space_(void* cache_seg) {
        using Fn = void(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052C1C0});
        fn(cache_seg);
    }

    inline uint32_t __cdecl Allocate_Buffer_Space_(uint32_t size) {
        using Fn = uint32_t(__cdecl*)(uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052C200});
        return fn(size);
    }
}