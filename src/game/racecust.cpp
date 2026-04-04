#include "pch.h"

namespace RACECUST {

void __cdecl Race_Customization_(int16_t player_idx)
{
    s_player* p = &MOX::_player[player_idx];
    
    // Only applies on Hard (3) or Impossible (4) difficulties
    if (MOX::_settings.game_difficulty > 2) {
        int diff = MOX::_settings.game_difficulty - 3;
        int rnd_idx = random::Random_(3) - 1;
        
        // The arrays are grouped in blocks of 5.
        // Base calculation: (race * 2 + diff) * 5 + random(-1..1 offset + 1)
        int index = (p->race * 2 + diff) * 5 + rnd_idx;
        
        // 1. Apply Customization Traits
        s_racecust_race_customize* cust = &RACECUST::_race_customize[index];
        for (int i = 0; i < cust->count; i++) {
            p->traits[cust->items[i].trait_idx] = cust->items[i].trait_val;
            p->evolutionary_upgrade_traits[cust->items[i].trait_idx] = cust->items[i].trait_val;
        }
        
        // 2. Apply Evolutionary Upgrade Traits
        s_racecust_race_evolution* evo = &RACECUST::_race_evolution[index];
        for (int i = 0; i < evo->count; i++) {
            p->evolutionary_upgrade_traits[evo->items[i].trait_idx] = evo->items[i].trait_val;
        }
    }
}

} // namespace RACECUST