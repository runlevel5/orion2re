#pragma once

namespace OFFICER {
    inline int16_t __cdecl Officer_Skill_Value_(int16_t leader_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0043E410 });
        return fn(leader_idx);
    }

    inline void __cdecl Get_Random_Skills_(uint32_t* p_special, uint32_t* p_general, int16_t arg3, int16_t arg4, uint8_t arg5) {
        using Fn = void(__cdecl*)(uint32_t*, uint32_t*, int16_t, int16_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0043DF90 });
        fn(p_special, p_general, arg3, arg4, arg5);
    }

    inline void __cdecl Allocate_Adv_Civ_Game_Officers_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00443C60 })();
    }

    inline bool __cdecl One_Leader_With_Galactic_Lore_(int16_t player_idx) {
        using Fn = bool(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004f1700});
        return fn(player_idx);
    }
}