#pragma once

namespace strings {
    void __cdecl Swap_Short_(int16_t* a, int16_t* b);

    void Load_Strings();

    void __cdecl String_To_Lower_(char* str);

    inline void __cdecl Trim_(char* str) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052CDE0});
        fn(str);
    }
}