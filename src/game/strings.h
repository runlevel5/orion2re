#pragma once

namespace strings {
    void __cdecl Swap_Short_(int16_t* a, int16_t* b);

    inline void Load_Strings() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00483F60 });
        fn();
    }

    inline void __cdecl String_To_Lower_(char* str) {
        using Fn = void(__cdecl*)(char*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0052CDB0})(str);
    }
}