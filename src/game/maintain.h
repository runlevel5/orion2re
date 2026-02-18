#pragma once

namespace maintain {
    inline void __cdecl Make_Scrap_Ships_Dead_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004b4360 });
        fn();
    }
}