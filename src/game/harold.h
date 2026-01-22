#pragma once

namespace HAROLD {
    void __cdecl Get_Font_File_(char* buffer);

    inline void Set_Sound_Volume_To_Settings_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004F1970 });
        fn();
    }
}