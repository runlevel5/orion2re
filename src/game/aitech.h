#pragma once

namespace aitech {
    void __cdecl Apply_Evolutionary_Upgrades_();
    void __cdecl Compute_AI_Tech_Info_(s_ai_tech_info* info);

    inline int __cdecl Player_Best_Ship_Shield_(s_player* player) {
        using Fn = int(__cdecl*)(s_player*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00500D00})(player);
    }

    inline int __cdecl Player_Best_Armor_(s_player* player) {
        using Fn = int(__cdecl*)(s_player*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00500D30});
        return fn(player);
    }

    inline int __cdecl Player_Best_Rifle_(s_player* player) {
        using Fn = int(__cdecl*)(s_player*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00500E30});
        return fn(player);
    }

    inline int __cdecl Player_Best_Personal_Shield_(s_player* player) {
        using Fn = int(__cdecl*)(s_player*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00500E60});
        return fn(player);
    }

    inline int __cdecl Player_Best_Computer_(s_player* player) {
        using Fn = int(__cdecl*)(s_player*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00500D60})(player);
    }

    inline int __cdecl Player_Best_Fuel_(s_player* player) {
        using Fn = int(__cdecl*)(s_player*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00500D90})(player);
    }

    inline int __cdecl Player_Best_Planetary_Shield_(s_player* player) {
        using Fn = int(__cdecl*)(s_player*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00500DC0})(player);
    }

    inline int __cdecl Player_Best_Drive_(s_player* player) {
        using Fn = int(__cdecl*)(s_player*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00500E00})(player);
    }

    inline void __cdecl All_AI_Tech_Select_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005014c0 });
        fn();
    }
}