#pragma once

namespace antarans {
    inline void __cdecl Antaran_Invasion_Check_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004b3690 });
        fn();
    }

    int32_t __cdecl Delta_Stardate_();
    int16_t __cdecl Delay_();
    int16_t __cdecl Antarans_Defensive_Resource_Bonus_();
}