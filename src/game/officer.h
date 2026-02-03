#pragma once

namespace OFFICER {
    int16_t __cdecl Get_Officer_Base_Level_(int16_t xp, int8_t warlord);

    int16_t __cdecl Get_Officer_Level_(int16_t leader_idx);

    int16_t __cdecl Bonus_For_Famous_Heroes_(int16_t player_idx);

    int16_t __cdecl Owned_Officer_Level_(int leader_idx);

    int32_t __cdecl Officer_Skill_Bonus_(uint32_t param_1, int16_t leader_idx, int16_t skill_idx);

    int16_t __cdecl Officer_Skill_Value_(int16_t leader_index);

    bool __cdecl Officer_Has_General_Skill_(int16_t player_num, int16_t leader_index, int16_t skill_id);

    bool __cdecl Officer_Has_Special_Skill_(int16_t player_num, int16_t leader_idx, int16_t skill_idx);

    int16_t __cdecl Get_Research_Cost_Skill_Value_(uint32_t cost);

    void __cdecl Get_Random_Skills_(uint32_t* primary_mask, uint32_t* secondary_mask, int16_t skill_class, int16_t count, int8_t randomize_class);

    void __cdecl Increase_Officer_Level_By_One_(int16_t leader_idx);

    inline void __cdecl Officers_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0043e9a0})();
    }

    void __cdecl Deassign_Officer_(int16_t leader_idx);

    void __cdecl Allocate_Adv_Civ_Game_Officers_();

    bool __cdecl One_Leader_With_Galactic_Lore_(int16_t player_idx);

    int16_t __cdecl Officer_Maintenance_(int16_t player_num, int16_t leader_idx);

    int32_t __cdecl Officer_Cost_(int16_t player_idx, int16_t leader_idx);
}