#pragma once
#include <cstdint>

namespace ERIC {
    inline uint8_t& _fade_percent = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558B40});

    void __cdecl Eric_Init_();

    void __cdecl Eric_Defaults_();

    int16_t __cdecl Get_Sint_Index_From_List_(int16_t value, const int16_t* list, uint16_t count);

    inline uint16_t __cdecl Get_Fade_Percent_() {
        using Fn = uint16_t(__cdecl*)();
        return reinterpret_cast<Fn>(0x004BA680)();
    }
}