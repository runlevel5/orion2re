#pragma once

namespace strings {
    void __cdecl Swap_Short_(int16_t* a, int16_t* b);

    inline void __cdecl Swap_Char_(uint8_t* a, uint8_t* b) {
        using Fn = void(__cdecl*)(uint8_t*, uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052CE80});
        fn(a, b);
    }

    inline const char* s__00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});
    inline const char* s_plus_00551118 = reinterpret_cast<const char*>(std::uintptr_t{0x00551118});
    inline const char* s_minus_0055111c = reinterpret_cast<const char*>(std::uintptr_t{0x0055111C});
    inline const char* s_space_00551120 = reinterpret_cast<const char*>(std::uintptr_t{0x00551120});
    inline const char* s_B_005560e4 = reinterpret_cast<const char*>(std::uintptr_t{0x005560E4});
    inline const char* s_S_005569cc = reinterpret_cast<const char*>(std::uintptr_t{0x005569CC});
    inline const char* s_O_00556afc = reinterpret_cast<const char*>(std::uintptr_t{0x00556AFC});
    inline const char* s_A_00557ed4 = reinterpret_cast<const char*>(std::uintptr_t{0x00557ED4});
    inline const char* s_M_00557ed8 = reinterpret_cast<const char*>(std::uintptr_t{0x00557ED8});
    inline const char* s_fmt_plus_d_00557ed0 = reinterpret_cast<const char*>(std::uintptr_t{0x00557ED0});

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