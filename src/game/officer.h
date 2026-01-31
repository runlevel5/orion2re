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

    void __cdecl Get_Random_Skills_(uint32_t* primary_mask, uint32_t* secondary_mask, int16_t skill_class, int16_t count, int8_t randomize_class);

    inline void __cdecl Allocate_Adv_Civ_Game_Officers_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00443C60 })();
    }

    bool __cdecl One_Leader_With_Galactic_Lore_(int16_t player_idx);
}