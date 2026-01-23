#pragma once

namespace allocate {
    inline void* Allocate_Space_(uint32_t size) {
        using Fn = void* (*)(uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052E880 });
        return fn(size);
    }

    void Allocate_Data_Space_();

    inline uint32_t __cdecl Get_Remaining_Block_Space_(void* ptr) {
        using Fn = uint32_t(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052EBB0 });
        return fn(ptr);
    }

    inline int32_t __cdecl Get_Remaining_Block_Space_(uint32_t seg) {
        using Fn = int32_t(__cdecl*)(uint32_t);
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x0052ebb0 })(seg);
    }

    // Suggested name: Allocate_Cache_
    inline uint32_t __cdecl FUN_0052c200(uint32_t size) {
        using Fn = uint32_t(__cdecl*)(uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052C200 });
        return fn(size);
    }

    inline void __cdecl Push_Block_(uint32_t seg) {
        using Fn = void(__cdecl*)(uint32_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0052eb30 })(seg);
    }

    inline void __cdecl Pop_Block_(uint32_t seg) {
        using Fn = void(__cdecl*)(uint32_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0052eb70 })(seg);
    }

    inline uint32_t __cdecl Allocate_Next_Block_(uint32_t seg, int32_t size) {
        using Fn = uint32_t(__cdecl*)(uint32_t, int32_t);
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x0052e950 })(seg, size);
    }

    inline void* __cdecl Allocate_Dos_Space_(uint32_t size) {
        using Fn = void* (__cdecl*)(uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052E8E0 });
        return fn(size);
    }
}