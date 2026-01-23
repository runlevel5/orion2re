#pragma once
#include <cstdint>

namespace MSG_ {
    inline void __cdecl Allocate_Msg_Slots_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004726E0})();
    }
}