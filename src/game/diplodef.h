#pragma once

#include <cstdint>

namespace diplodef {
    inline int16_t (&_personality_relation_modifiers)[6] =
        *reinterpret_cast<int16_t(*)[6]>(std::uintptr_t{0x00559B10});
    inline int16_t* _threat_demand_chance = reinterpret_cast<int16_t*>(std::uintptr_t{0x00559BB0});
    inline int16_t (&_personality_threat_modifiers)[6] =
        *reinterpret_cast<int16_t(*)[6]>(std::uintptr_t{0x00559B20});
    inline int16_t& _const_50 = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559B2C});
    inline int16_t& _next_council_meeting_turn = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059600C});
    inline int16_t& _COUNCIL_FLAG = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059600E});
    inline int16_t& _council_winner = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596010});
    inline int16_t* _personality_reward_proposal_chance = reinterpret_cast<int16_t*>(std::uintptr_t{0x00559C10});
    inline int16_t* _personality_max_threats = reinterpret_cast<int16_t*>(std::uintptr_t{0x00559B38});
}