#pragma once
#include <cstdint>

namespace ERIC {
    inline uint8_t& _fade_percent = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558B40});

    void __cdecl Eric_Init_();

    void __cdecl Eric_Defaults_();

    int16_t __cdecl Get_Sint_Index_From_List_(int16_t value, const int16_t* list, uint16_t count);

    uint16_t __cdecl Get_Fade_Percent_();

    inline void __cdecl Fast_Fade_Out_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ba690});
        fn();
    }

    inline void __cdecl Set_Fade_Percent_(int32_t percent) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ba660});
        fn(percent);
    }
}