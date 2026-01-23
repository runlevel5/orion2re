#pragma once

namespace SHIPMOVE {
    // Suggested name: Allocate_New_Ship_Slots_
    inline void __cdecl Allocate_New_Ship_Slots_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00436F50});
        fn();
    }
}