#pragma once

namespace init {
    inline s_animation_header*& _io_buffer =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{ 0x0057E184 });

    inline int32_t Init_Drivers_(int32_t mode, char* font_file) {
        using Fn = int32_t(*)(int32_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005335B0 });
        return fn(mode, font_file);
    }
}