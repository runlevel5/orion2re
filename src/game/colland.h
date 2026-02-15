#pragma once
#include <cstdint>

namespace COLLAND {
    inline s_animation_header*& _l_anims = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00564DB8});
    inline int32_t& _pod1l_sfx = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00564DBC});
    inline int32_t& _pod2d_sfx = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00564DC0});
    inline int32_t& _pod2l_sfx = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00564DC4});
    inline int32_t& _pod1d_sfx = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00564DC8});
    inline uint16_t& _fields =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00564DCC});

    inline const char*& s_format_comma_space = *reinterpret_cast<const char**>(std::uintptr_t{0x0055238C});

    void __cdecl Load_Anims_();
    void Add_Fields_();

    void __cdecl Draw_Colony_Landing_Screen_();

    void __cdecl Print_Planet_Data_(int16_t planet_idx);

    void __cdecl Colony_Landing_Screen_();
}