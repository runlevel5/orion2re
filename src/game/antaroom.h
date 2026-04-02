#pragma once
#include <cstdint>

namespace ANTAROOM {
    inline void __cdecl Main_Antaran_Room_Screen_(uint16_t flag) {
        using Fn = void(__cdecl*)(uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042A920});
        fn(flag);
    }
}