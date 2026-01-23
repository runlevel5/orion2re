#pragma once

namespace farload {
    inline void* __cdecl Far_Reload_(const char* filename, int32_t index, int32_t block_id) {
        using Fn = void* (__cdecl*)(const char*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0051C710 });
        return fn(filename, index, block_id);
    }
}