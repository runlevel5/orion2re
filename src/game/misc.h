#pragma once

namespace MISC {
    inline uint32_t* Font_Colors2_(int32_t arg1, int32_t arg2, int32_t arg3) {
        using Fn = uint32_t * (*)(int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0049DDB0 });
        return fn(arg1, arg2, arg3);
    }
}