#pragma once

struct s_player;

namespace maintain {
    void __cdecl Make_Scrap_Ships_Dead_();
    void __cdecl Player_Maintenance_(s_player* param_1);

    inline int (*&_ship_worths)[500] =
        *reinterpret_cast<int (**)[500]>(std::uintptr_t{0x0056CD8C});

    // Suggested name: Phase1_Building_
    inline bool __cdecl Phase1_Building_(s_player* p, int16_t a, int16_t b) {
        using Fn = bool(__cdecl*)(s_player*, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004B4900})(p, a, b);
    }

    // Suggested name: Phase2_Building_
    inline bool __cdecl Phase2_Building_(s_player* p, int16_t a, int16_t b) {
        using Fn = bool(__cdecl*)(s_player*, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004B4950})(p, a, b);
    }

    // Suggested name: All_But_Capitol_
    inline bool __cdecl All_But_Capitol_(s_player* p, int16_t a, int16_t b) {
        using Fn = bool(__cdecl*)(s_player*, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004B4970})(p, a, b);
    }

    // Suggested name: Kill_Research_Buildings_
    inline void __cdecl Kill_Research_Buildings_(s_player* p) {
        using Fn = void(__cdecl*)(s_player*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B47E0});
        fn(p);
    }

    // Suggested name: Kill_Buildings_
    inline void __cdecl Kill_Buildings_(s_player* p, bool(__cdecl* filter)(s_player*, int16_t, int16_t)) {
        using Fn = void(__cdecl*)(s_player*, bool(__cdecl*)(s_player*, int16_t, int16_t));
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B4980});
        fn(p, filter);
    }

    // Suggested name: Kill_Ships_
    inline void __cdecl Kill_Ships_(s_player* p, uint8_t arg2, uint8_t arg3, int32_t arg4) {
        using Fn = void(__cdecl*)(s_player*, uint8_t, uint8_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B4DE0});
        fn(p, arg2, arg3, arg4);
    }

    // Suggested name: Kill_Spy_
    inline void __cdecl Kill_Spy_(s_player* p) {
        using Fn = void(__cdecl*)(s_player*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B4D20});
        fn(p);
    }

    // Suggested name: Kill_Leaders_
    inline void __cdecl Kill_Leaders_(s_player* p) {
        using Fn = void(__cdecl*)(s_player*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B4FD0});
        fn(p);
    }
}