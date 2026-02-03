#pragma once

namespace strings {
    void __cdecl Swap_Short_(int16_t* a, int16_t* b);

    inline void Load_Strings() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00483F60 });
        fn();
    }

    void __cdecl String_To_Lower_(char* str);
}