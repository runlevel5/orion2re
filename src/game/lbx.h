#pragma once
#include <cstdint>

namespace lbx {
    inline void __cdecl LBX_init_(void* header, const char* filename) {
        using Fn = void(__cdecl*)(void*, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00534880});
        fn(header, filename);
    }

    inline int32_t __cdecl LBX_getRecordLength_(intptr_t header, uint16_t rec_idx) {
        using Fn = int32_t(__cdecl*)(intptr_t, uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00534A50});
        return fn(header, rec_idx);
    }

    inline void __cdecl LBX_uninit_(uint32_t* header) {
        using Fn = void(__cdecl*)(uint32_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005349E0});
        fn(header);
    }

    inline void __cdecl LBX_init_stream_(void* handle, const char* filename) {
        using Fn = void(__cdecl*)(void*, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537FC0});
        fn(handle, filename);
    }
}