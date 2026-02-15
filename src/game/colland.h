#pragma once
#include <cstdint>

namespace COLLAND {
    inline uint32_t& _l_anims = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00564DB8});
    inline uint32_t& _pod1l_sfx = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00564DBC});
    inline uint32_t& _pod2d_sfx = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00564DC0});
    inline uint32_t& _pod2l_sfx = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00564DC4});
    inline uint32_t& _pod1d_sfx = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00564DC8});
    inline uint16_t& _fields =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00564DCC});

    void __cdecl Load_Anims_();
    void Add_Fields_();

    inline void __cdecl Draw_Colony_Landing_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00429da0})();
    }

    void __cdecl Colony_Landing_Screen_();
}