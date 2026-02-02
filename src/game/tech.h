#pragma once
#include <cstdint>

namespace TECH {
    inline void __cdecl Tech_Change_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00484040})();
    }
}