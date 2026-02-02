#pragma once
#include <cstdint>

namespace INFO {
    inline void __cdecl Info_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00425e90})();
    }
}