#pragma once

namespace HAROLD {
    void __cdecl Get_Font_File_(char* buffer);

    inline void Set_Sound_Volume_To_Settings_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004F1970 });
        fn();
    }

    inline void __cdecl Confirm_Planet_And_Star_Index_Link_(const char* msg) {
        using Fn = void(__cdecl*)(const char*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004F0640 })(msg);
    }

    inline void __cdecl Set_All_Visited_Flags_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004F18F0 })();
    }

    inline void __cdecl Harold_Init_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004F0DD0 })();
    }

    inline const char* s_after_home_world_gen = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575D4 }); // "after home world gen"
}