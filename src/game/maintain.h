#pragma once

struct s_player;

namespace maintain {
    bool __cdecl Ship_Is_At_Colony_(s_ship_data* ship);
    void __cdecl Make_Scrap_Ships_Dead_();
    void __cdecl Player_Maintenance_(s_player* param_1);

    inline int (*&_ship_worths)[500] =
        *reinterpret_cast<int (**)[500]>(std::uintptr_t{0x0056CD8C});

    // Suggested name: Phase1_Building_
    inline bool __cdecl Phase1_Building_(s_player* p, int16_t a, int16_t b) {
        using Fn = bool(__cdecl*)(s_player*, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004B4900})(p, a, b);
    }

    bool __cdecl Phase2_Building_(s_player* p, int16_t a, int16_t b);

    bool __cdecl All_But_Capitol_(s_player* p, int16_t a, int16_t b);

    inline uint8_t (&_research_building_table)[4] =
        *reinterpret_cast<uint8_t(*)[4]>(std::uintptr_t{0x0054F088});

    void __cdecl Kill_Research_Buildings_(s_player* player);

    // Suggested name: Kill_Buildings_
    inline void __cdecl Kill_Buildings_(s_player* p, bool(__cdecl* filter)(s_player*, int16_t, int16_t)) {
        using Fn = void(__cdecl*)(s_player*, bool(__cdecl*)(s_player*, int16_t, int16_t));
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B4980});
        fn(p, filter);
    }

    void __cdecl Kill_Ships_(s_player* player, bool at_colony_only, bool restricted_mission, int16_t ship_type);

    void __cdecl Kill_Spy_(s_player* player);

    void __cdecl Kill_Leaders_(s_player* p);
}