#pragma once
#include <cstdint>

namespace FAKEBUTS {
    inline void __cdecl Clear_Fake_Buttons_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B53F0});
        fn();
    }

    inline void __cdecl Draw_Fake_Buttons_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004b5350 });
        fn();
    }

    inline void Evaluate_Click_Thru_Fields_(int16_t input) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(0x004b5640)(input);
    }

    inline void Clear_Click_Thru_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004b55b0)();
    }

    inline void __cdecl Draw_Click_Thru_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004b5410)();
    }

    inline int16_t __cdecl Add_Click_Thru_Field_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, void* data, int count, void* var, int unk, const char* str, int16_t unk2, uint16_t hidden_id) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t, void*, int, void*, int, const char*, int16_t, uint16_t);
        return reinterpret_cast<Fn>(0x004b55d0)(x1, y1, x2, y2, data, count, var, unk, str, unk2, hidden_id);
    }
}