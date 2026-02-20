#pragma once

namespace aidudes {
    inline void __cdecl Colony_AI_(short plr_idx) {
        using Fn = void(__cdecl*)(short);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004460B0});
        fn(plr_idx);
    }

    void All_Colony_AI_();
}