#pragma once

namespace council {
    void Check_For_Council_Meeting_();

    inline void __cdecl Main_Council_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004d7f50 });
        fn();
    }
}