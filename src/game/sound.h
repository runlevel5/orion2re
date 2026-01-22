#pragma once

namespace sound {
    inline void Set_Audio_Buffer_(void* buffer) {
        using Fn = void(*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052D580 });
        fn(buffer);
    }
}