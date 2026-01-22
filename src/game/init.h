#pragma once

namespace init {
    inline int32_t Init_Drivers_(int32_t mode, char* font_file) {
        using Fn = int32_t(*)(int32_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005335B0 });
        return fn(mode, font_file);
    }
}