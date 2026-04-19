#pragma once

namespace init {
    inline s_animation_header*& _io_buffer =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{ 0x0057E184 });

    void __cdecl Create_IO_Buffer_();

    int8_t __cdecl Init_Drivers_(uint32_t param_1, char* font_name);
}