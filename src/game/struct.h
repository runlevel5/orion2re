#pragma once

namespace struct_ {
    inline void __cdecl Clear_Structure_(void* ptr, uint32_t size) {
        using Fn = void(__cdecl*)(void*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052D0B7 });
        fn(ptr, size);
    }

    inline void __cdecl Copy_Structure_(void* dest, const void* src, uint32_t size) {
        using Fn = void(__cdecl*)(void*, const void*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052D0FA });
        fn(dest, src, size);
    }

    inline uint16_t __cdecl Test_Bit_Field_(uint8_t* field, int32_t bit) {
        using Fn = uint16_t(__cdecl*)(uint8_t*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052cfb5 });
        return fn(field, bit);
    }

    inline void __cdecl Set_Bit_Field_(s_good_star* star, uint16_t field_idx) {
        using Fn = void(__cdecl*)(s_good_star*, uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052CFED });
        fn(star, field_idx);
    }
}