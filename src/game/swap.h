#pragma once
#include <cstdint>

namespace swap {
    inline void __cdecl Save_Block_To_Drive_(void* segment, int32_t size) {
        using Fn = void(__cdecl*)(void*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00534790});
        fn(segment, size);
    }
    inline void __cdecl Restore_Block_From_Drive_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00534830});
        fn();
    }
}