#pragma once

namespace RACECUST {
    inline s_racecust_race_customize* _race_customize = reinterpret_cast<s_racecust_race_customize*>(0x00551388);
    inline s_racecust_race_evolution* _race_evolution = reinterpret_cast<s_racecust_race_evolution*>(0x00551C30);

    void __cdecl Race_Customization_(int16_t player_idx);
}