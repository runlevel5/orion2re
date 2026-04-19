#pragma once

namespace LOADER {
    inline void __cdecl Load_Main_Screen_Pictures_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b31a0});
        fn();
    }
    void __cdecl Load_Pictures_();
}