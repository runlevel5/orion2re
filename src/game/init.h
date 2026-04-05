#pragma once

namespace init {
    inline s_animation_header*& _io_buffer =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{ 0x0057E184 });

    inline void __cdecl Create_IO_Buffer_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00533650});
        fn();
    }

    int8_t __cdecl Init_Drivers_(uint32_t param_1, char* font_name);
}