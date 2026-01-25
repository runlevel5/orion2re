#pragma once
#include <cstdint>

namespace ERIC {
    void __cdecl Eric_Init_();

    inline uint32_t __cdecl Get_Sint_Index_From_List_(int16_t val, int16_t* list, int16_t count) {
        using Fn = uint32_t(__cdecl*)(int16_t, int16_t*, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x004BA510 })(val, list, count);
    }
}