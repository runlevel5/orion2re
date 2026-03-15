#pragma once

namespace strings {
    void __cdecl Swap_Short_(int16_t* a, int16_t* b);

    void Load_Strings();

    void __cdecl String_To_Lower_(char* str);

    void __cdecl String_To_Upper_(char* str);

    void __cdecl Trim_(char* str);

    inline void __cdecl Delete_Structure_(int idx, void* array, int elem_size, int count) {
        using Fn = void(__cdecl*)(int, void*, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052cea0});
        fn(idx, array, elem_size, count);
    }
}