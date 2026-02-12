#pragma once
#include <cstdint>

namespace TECH {
    inline void __cdecl _Tech_Select_(uint8_t param_1) {
        using Fn = void(__cdecl*)(uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004840C0});
        fn(param_1);
    }

    void Tech_Change_();
}