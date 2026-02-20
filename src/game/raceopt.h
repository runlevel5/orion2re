#pragma once

namespace raceopt {
    inline void __cdecl Setup_Evolutionary_Upgrade_(short player_idx) {
        using Fn = void(__cdecl*)(short);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00439CA0});
        fn(player_idx);
    }
}