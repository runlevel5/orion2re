#pragma once

namespace init {
    extern s_animation_header* _io_buffer;

    void __cdecl Create_IO_Buffer_();

    int8_t __cdecl Init_Drivers_(uint32_t param_1, char* font_name);
}