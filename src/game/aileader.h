#pragma once

namespace aileader {
    void __cdecl Do_AI_Leaders_();

    inline void __cdecl Assign_Unassigned_Leaders_(int32_t player_idx) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042a430});
        fn(player_idx);
    }
}