#pragma once
#include <cstdint>

namespace CMBTDRW1 {
    inline void*& _k_save = *reinterpret_cast<void**>(std::uintptr_t{0x005960FC});

    void __cdecl Detailed_View_Ship_(int16_t ship_idx);
}