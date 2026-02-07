#pragma once
#include <cstdint>

struct s_animation_header;

namespace KEN {
    inline void __cdecl Ken_Init_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004AA050 })();
    }

    s_animation_header* __cdecl Do_Get_Ship_Picture_Seg(int16_t player_idx, int16_t ship_type);
    s_animation_header* __cdecl Get_Ship_Picture_Seg_(int16_t ship_id, int16_t frame_index);

    // Suggested name: Ken_Get_Text_Message
    inline void __cdecl Ken_Get_Text_Message_(int32_t msg_idx, char* buffer, int32_t max_len) {
        using Fn = void(__cdecl*)(int32_t, char*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c2630});
        fn(msg_idx, buffer, max_len);
    }

    inline void __cdecl Init_NPC_Personalities_Objectives_Themes_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004C1FC0 })();
    }

    inline void __cdecl Ken_Init2_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004c1d50 });
        fn();
    }

    inline void __cdecl Load_Player_Ship_Palette_(int16_t player) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004C1B30 });
        fn(player);
    }
}