#pragma once

namespace OFFICER {
    int16_t __cdecl Get_Officer_Base_Level_(int16_t xp, uint8_t warlord);

    int16_t __cdecl Owned_Officer_Level_(int leader_idx);

    int16_t __cdecl Officer_Skill_Value_(int16_t leader_index);

    inline bool __cdecl Officer_Has_General_Skill_(int16_t player_num, int16_t leader_index, int16_t skill_id) {
        using Fn = bool(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043e390});
        return fn(player_num, leader_index, skill_id);
    }

    inline bool __cdecl Officer_Has_Special_Skill_(int16_t player_num, int16_t leader_index, int16_t skill_id) {
        using Fn = bool(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043e3d0});
        return fn(player_num, leader_index, skill_id);
    }

    inline int16_t __cdecl Get_Research_Cost_Skill_Value_(uint32_t cost) {
        using Fn = int16_t(__cdecl*)(uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043e570});
        return fn(cost);
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