#pragma once

namespace LOADER {
    void __cdecl Load_Main_Screen_Pictures_();
    void __cdecl Load_Pictures_();
    inline void __cdecl Load_Player_Special_Names_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B3610});
        fn();
    }
}