#pragma once
#include <cstdint>

namespace TRADE {
    inline void __cdecl Process_Trade_And_Research_Agreements_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00466240})();
    }
}