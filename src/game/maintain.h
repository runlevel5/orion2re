#pragma once

namespace maintain {
    void __cdecl Make_Scrap_Ships_Dead_();

    inline void __cdecl Player_Maintenance_(s_player* plr) {
        using Fn = void(__cdecl*)(s_player*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004b43d0 });
        fn(plr);
    }
}