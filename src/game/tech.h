#pragma once
#include <cstdint>

namespace TECH {
    inline void __cdecl _Tech_Select_(uint8_t param_1) {
        using Fn = void(__cdecl*)(uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004840C0});
        fn(param_1);
    }

    void Tech_Change_();

    inline char* __cdecl Technology_Applications_Name_(int16_t app_id, int16_t param_3) {
        using Fn = char*(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00485F20});
        return fn(app_id, param_3);
    }
}