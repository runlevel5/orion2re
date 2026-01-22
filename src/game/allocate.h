#pragma once

namespace allocate {
    inline void* Allocate_Space_(uint32_t size) {
        using Fn = void* (*)(uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052E880 });
        return fn(size);
    }

    inline void Allocate_Data_Space_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00500B20 });
        fn();
    }
}