#pragma once

namespace palette {
    void __cdecl Set_Palette_Changes_(int32_t start_index, int32_t end_index);

    void __cdecl Clear_Palette_Changes_(int32_t start, int32_t end);

    inline void __cdecl Refresh_Palette_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052B660});
        fn();
    }
}