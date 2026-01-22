#pragma once

namespace HAROLD {
    inline void Get_Font_File_(char* buffer) {
        using Fn = void(*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004F24D0 });
        fn(buffer);
    }
    inline void Set_Sound_Volume_To_Settings_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004F1970 });
        fn();
    }
}