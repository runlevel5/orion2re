#pragma once

namespace OFFICER {
    inline int16_t __cdecl Officer_Skill_Value_(int16_t leader_index) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0043E410 });
        return fn(leader_index);
    }

    inline void __cdecl Get_Random_Skills_(uint32_t* p_special, uint32_t* p_general, int32_t arg3, int32_t arg4, int32_t arg5) {
        using Fn = void(__cdecl*)(uint32_t*, uint32_t*, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0043DF90 });
        fn(p_special, p_general, arg3, arg4, arg5);
    }

    inline void __cdecl Allocate_Adv_Civ_Game_Officers_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00443C60 })();
    }
}