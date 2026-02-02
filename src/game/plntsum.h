#pragma once
#include <cstdint>

namespace PLNTSUM {
    inline void __cdecl Planet_Summary_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004488e0})();
    }
}