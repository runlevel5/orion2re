#pragma once

namespace KEN {
    inline void __cdecl Ken_Init_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004AA050 })();
    }

    inline void __cdecl Init_NPC_Personalities_Objectives_Themes_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004C1FC0 })();
    }

    inline void __cdecl Ken_Init2_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c1d50});
        fn();
    }

    inline void __cdecl Load_Player_Ship_Palette_(int16_t player) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004C1B30});
        fn(player);
    }

    inline void* __cdecl Get_Ship_Picture_Seg_(int16_t player, int16_t picture) {
        using Fn = void*(__cdecl*)(int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x004c1cc0 })(player, picture);
    }
}