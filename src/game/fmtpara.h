#pragma once
#include <cstdint>

namespace FMTPARA {
    inline int16_t __cdecl Get_Formatted_Paragraph_Max_Height_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00432e70});
        return fn();
    }
}