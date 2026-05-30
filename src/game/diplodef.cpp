#include "pch.h"

namespace diplodef {
    int16_t _personality_relation_modifiers[8] = {-50, -20, -20, 0, 20, 30, -70, 0};
    int16_t _threat_demand_chance[10] = {0, 30, 80, 20, 10, 0, 80, 0, 0, 0};
    int16_t _personality_threat_modifiers[10] = {40, 30, 20, 5, 0, -10, 50, 0, 0, 0};
    int16_t _next_council_meeting_turn;
    int16_t _COUNCIL_FLAG;
    int16_t _council_winner;
    int16_t _personality_reward_proposal_chance[10] = {0, 5, 5, 10, 15, 20, 0, 0, 0, 0};
    int16_t _personality_max_threats[10] = {1, 2, 3, 3, 4, 5, 2, 0, 0, 0};
    int16_t _personality_break_treaty_war[10] = {100, 50, 50, 100, 50, 20, 0, 0, 0, 0};
}