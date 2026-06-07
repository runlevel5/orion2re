#pragma once

#include <cstdint>

namespace diplodef {
    extern int16_t _personality_relation_modifiers[MAX_PLAYERS];
    extern int16_t _threat_demand_chance[10];
    extern int16_t _personality_threat_modifiers[10];
    extern int16_t _next_council_meeting_turn;
    extern int16_t _COUNCIL_FLAG;
    extern int16_t _council_winner;
    extern int16_t _personality_reward_proposal_chance[10];
    extern int16_t _personality_max_threats[10];
    extern int16_t _personality_break_treaty_war[10];
}

namespace DIPLODEF = diplodef;